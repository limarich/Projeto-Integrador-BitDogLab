#include <stdio.h>
#include "hardware/clocks.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/adc.h"
#include "pio_matrix.pio.h"
#include "libs/leds.h"
#include "libs/buzzer.h"

// Define os pinos GPIO para o LED RGB
#define LED_G_PIN 11    // VERDE
#define LED_B_PIN 12    // AZUL
#define LED_R_PIN 13    // VERMELHO
#define BUZZER_PIN 10   // Pino do buzzer
#define BUTTON_A 5      // Pino do buzzer
#define BUTTON_B 6      // Pino do buzzer
#define DEBOUNCE_MS 200 // intervalo minimo de 200ms para o debounce
#define VY_PIN 26       // eixo Y do joystick
#define VX_PIN 27       // eixo X do joystick
#define DEADZONE 300    // valor para ignorar a leitura do joystick

typedef color_options color_mode; // tipo criado na biblioteca leds.h

// inicialização do GPIO
void setup_gpio_pin(uint pin, uint dir);
// muda a cor do sistema
void switch_color_mode(color_mode mode);
// inicializacao da PIO
void PIO_setup(PIO *pio, uint *sm);
// calcula a direção indicada pela matriz de leds
void handle_arrow_direction(uint *dir);

// o led inicia em vermelho
uint current_color_mode = RED;
// variaveis para gerencia o debounce
uint last_interrupt_a = 0;
uint last_interrupt_b = 0;
// variaveis relacionadas a matriz de led
PIO pio;
uint sm;
// grava os pontos iniciais do joystick
uint initial_vx = 0;
uint initial_vy = 0;
// controle manual do buzzer
volatile bool should_play_buzzer = false;
float buzzer_frequency = 0;

// gerenciador de interrupcoes
void gpio_irq_handler(uint gpio, uint32_t events)
{
    // variavel para o controle do debounce
    uint current_time = to_ms_since_boot(get_absolute_time());
    if (gpio == BUTTON_B)
    {
        if (current_time - last_interrupt_b > DEBOUNCE_MS)
        {
            last_interrupt_b = current_time;

            if (current_color_mode >= WHITE)
            {
                switch_color_mode(RED);
            }
            else
            {
                switch_color_mode(current_color_mode + 1);
            }
            buzzer_frequency = 1046.50; // algo como 'C'
        }
    }
    else if (gpio == BUTTON_A)
    {
        if (current_time - last_interrupt_a > DEBOUNCE_MS)
        {
            last_interrupt_a = current_time;

            if (current_color_mode <= RED)
            {
                switch_color_mode(WHITE);
            }
            else
            {
                switch_color_mode(current_color_mode - 1);
            }

            buzzer_frequency = 1396.91; // algo como 'F'
        }
    }
}

int main()
{
    stdio_init_all();
    // setup dos leds
    setup_gpio_pin(LED_R_PIN, GPIO_OUT);
    setup_gpio_pin(LED_G_PIN, GPIO_OUT);
    setup_gpio_pin(LED_B_PIN, GPIO_OUT);

    // setup dos botões
    setup_gpio_pin(BUTTON_A, GPIO_IN);
    setup_gpio_pin(BUTTON_B, GPIO_IN);
    // pull up
    gpio_pull_up(BUTTON_A);
    gpio_pull_up(BUTTON_B);

    // habilita a interrupção
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled(BUTTON_B, GPIO_IRQ_EDGE_FALL, true);

    // inicializa a PIO
    PIO_setup(&pio, &sm);
    // função de teste
    test_matrix(pio, sm); // faz uma animação indicando que está tudo OK e logo apaga a matriz

    // setup do joystick
    adc_init();
    adc_gpio_init(VY_PIN);
    adc_gpio_init(VX_PIN);

    // setup do buzzer
    initialization_buzzers(BUZZER_A, BUZZER_B); // BUZZER A - 10 e BUZZER B - 21 definidos na biblioteca buzzers.h

    uint current_direction = NORTH;

    adc_select_input(1);
    initial_vx = adc_read();

    adc_select_input(0);
    initial_vy = adc_read();

    while (true)
    {
        // reset dos leds
        gpio_put(LED_R_PIN, 0);
        gpio_put(LED_G_PIN, 0);
        gpio_put(LED_B_PIN, 0);

        switch (current_color_mode)
        {
        case RED:
            gpio_put(LED_R_PIN, 1);
            break;
        case GREEN:
            gpio_put(LED_G_PIN, 1);
            break;
        case BLUE:
            gpio_put(LED_B_PIN, 1);
            break;

        case WHITE:
            gpio_put(LED_R_PIN, 1);
            gpio_put(LED_G_PIN, 1);
            gpio_put(LED_B_PIN, 1);

            break;
        default:
            break;
        }

        handle_arrow_direction(&current_direction);
        // aponta a direção do joystick
        draw_arrow(pio, sm, current_direction, current_color_mode);

        // efeito sonoro
        if (should_play_buzzer)
        {
            buzzer_pwm(BUZZER_A, buzzer_frequency, 50);
            should_play_buzzer = false;
        }
        sleep_ms(100);
    }
}

void setup_gpio_pin(uint pin, uint dir)
{
    gpio_init(pin);
    gpio_set_dir(pin, dir);
}

void PIO_setup(PIO *pio, uint *sm)
{
    // configurações da PIO
    *pio = pio0;
    uint offset = pio_add_program(*pio, &pio_matrix_program);
    *sm = pio_claim_unused_sm(*pio, true);
    pio_matrix_program_init(*pio, *sm, offset, LED_PIN);
}

void switch_color_mode(color_mode mode)
{
    current_color_mode = mode;
    should_play_buzzer = true; // indica a mundança da cor
}

void handle_arrow_direction(uint *dir)
{
    adc_select_input(1);
    int vx = adc_read();

    adc_select_input(0);
    int vy = adc_read();

    // Cálculo do desvio a partir do centro
    int dx = vx - initial_vx;
    int dy = vy - initial_vy;

    printf("dx: %d, dy: %d\n", dx, dy);

    bool left = false, right = false;
    bool up = false, down = false;

    if (dx > DEADZONE)
        right = true;
    else if (dx < -DEADZONE)
        left = true;

    if (dy > DEADZONE)
        up = true;
    else if (dy < -DEADZONE)
        down = true;

    // Decidir a direção com base nos flags
    if (up && right)
        *dir = NORTHEAST;
    else if (up && left)
        *dir = NORTHWEST;
    else if (down && right)
        *dir = SOUTHEAST;
    else if (down && left)
        *dir = SOUTHWEST;
    else if (up)
        *dir = NORTH;
    else if (down)
        *dir = SOUTH;
    else if (right)
        *dir = EAST;
    else if (left)
        *dir = WEST;
}