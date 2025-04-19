#include <stdio.h>
#include "pico/stdlib.h"

// Define os pinos GPIO para o LED RGB
#define LED_G_PIN 11    // VERDE
#define LED_B_PIN 12    // AZUL
#define LED_R_PIN 13    // VERMELHO
#define BUZZER_PIN 10   // Pino do buzzer
#define BUTTON_A 5      // Pino do buzzer
#define BUTTON_B 6      // Pino do buzzer
#define DEBOUNCE_MS 200 // intervalo minimo de 200ms para o debounce

typedef enum
{
    RED,
    GREEN,
    BLUE,
    WHITE
} color_mode;

// inicialização do GPIO
void setup_gpio_pin(uint pin, uint dir);
// muda a cor do sistema
void switch_color_mode(color_mode mode);

// o led inicia em vermelho
uint current_color_mode = RED;
// variaveis para gerencia o debounce
uint last_interrupt_a = 0;
uint last_interrupt_b = 0;

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

    while (true)
    {
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

        sleep_ms(10);
    }
}

void setup_gpio_pin(uint pin, uint dir)
{
    gpio_init(pin);
    gpio_set_dir(pin, dir);
}

void switch_color_mode(color_mode mode)
{
    current_color_mode = mode;
    // efeito sonoro para indicar a mudança
}