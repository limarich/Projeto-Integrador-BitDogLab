#include <stdio.h>
#include "pico/stdlib.h"

// Define os pinos GPIO para o LED RGB
#define LED_G_PIN 11  // VERDE
#define LED_B_PIN 12  // AZUL
#define LED_R_PIN 13  // VERMELHO
#define BUZZER_PIN 10 // Pino do buzzer

// inicialização do GPIO
void setup_gpio_pin(uint pin, uint dir);

int main()
{
    stdio_init_all();
    setup_gpio_pin(LED_R_PIN, GPIO_OUT);
    setup_gpio_pin(LED_G_PIN, GPIO_OUT);

    while (true)
    {
        gpio_put(LED_R_PIN, 1);
        printf("Hello, world!\n");
        sleep_ms(1000);
        gpio_put(LED_G_PIN, 1);
        sleep_ms(1000);
        gpio_put(LED_R_PIN, 0);
        sleep_ms(1000);
        gpio_put(LED_G_PIN, 0);
        sleep_ms(1000);
    }
}

void setup_gpio_pin(uint pin, uint dir)
{
    gpio_init(pin);
    gpio_set_dir(pin, dir);
}