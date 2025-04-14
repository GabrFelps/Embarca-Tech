#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812_driver.h"  // Supondo que você tenha um driver ou cabeçalho p/ WS2812

// Definições de pinos
#define LED_PIN       7   // Matriz de LEDs WS2812B
#define BUTTON_A_PIN  5
#define BUTTON_B_PIN  6
#define BUZZER_PIN   21   // Opcional
#define NUM_LEDS     25   // Tamanho da matriz (5x5)

// Tempo máximo para o jogador responder (ms)
#define TIME_LIMIT_MS 5000

// Cores em formato GRB (caso use WS2812, atente à ordem)
#define RED    0x00FF0000
#define BLUE   0x000000FF
#define OFF    0x00000000

int main() {
    // Inicializa stdio e GPIO
    stdio_init_all();

    // Inicializa os botões (entrada + pull-up)
    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);

    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN);

    // Opcional: inicializa buzzer
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_put(BUZZER_PIN, 0);

    // Inicializa driver da WS2812
    ws2812_init(LED_PIN);  // Função hipotética no driver

    // Semente de aleatoriedade (baseada no tempo)
    srand((unsigned) time_us_32());

    while (true) {
        printf("\n--- Nova Rodada ---\n");

        // 1) Gera padrão aleatório de vermelhos e azuis
        int redCount = 0;
        int blueCount = 0;

        // Armazena as cores para cada LED
        uint32_t colors[NUM_LEDS];

        for (int i = 0; i < NUM_LEDS; i++) {
            // Sorteia 0 ou 1
            int randomColor = rand() % 2;
            if (randomColor == 0) {
                colors[i] = RED;
                redCount++;
            } else {
                colors[i] = BLUE;
                blueCount++;
            }
        }

        // Exibe na matriz de LEDs
        for (int i = 0; i < NUM_LEDS; i++) {
            ws2812_set_pixel(i, colors[i]);  // Função do driver
        }
        ws2812_show(); // Atualiza os LEDs

        // 2) Jogador deve pressionar Botão A "redCount" vezes e Botão B "blueCount" vezes
        int userRedPresses = 0;
        int userBluePresses = 0;

        uint64_t startTime = to_ms_since_boot(get_absolute_time());
        bool done = false;

        printf("Precisa pressionar A = %d vezes (vermelho), B = %d vezes (azul)\n", redCount, blueCount);

        while (!done) {
            // Verifica tempo
            uint64_t now = to_ms_since_boot(get_absolute_time());
            if (now - startTime > TIME_LIMIT_MS) {
                // Tempo acabou
                printf("Tempo Esgotado!\n");
                break;
            }

            // Leitura Botão A (pulso simples)
            if (!gpio_get(BUTTON_A_PIN)) {
                userRedPresses++;
                printf("Botao A pressionado, count=%d\n", userRedPresses);
                // Debounce simples
                sleep_ms(200);
            }

            // Leitura Botão B (pulso simples)
            if (!gpio_get(BUTTON_B_PIN)) {
                userBluePresses++;
                printf("Botao B pressionado, count=%d\n", userBluePresses);
                // Debounce simples
                sleep_ms(200);
            }

            // Se jogador já passou do limite, pode encerrar
            if (userRedPresses > redCount || userBluePresses > blueCount) {
                break;
            }

            // Se já atingiu exatamente a contagem
            if ((userRedPresses == redCount) && (userBluePresses == blueCount)) {
                done = true;
            }
        }

        // 3) Verifica se acertou
        if (userRedPresses == redCount && userBluePresses == blueCount) {
            // Acertou
            printf("Acertou!\n");
            // Opcional: piscar buzzer ou LED
            for (int i = 0; i < 3; i++) {
                gpio_put(BUZZER_PIN, 1);
                sleep_ms(100);
                gpio_put(BUZZER_PIN, 0);
                sleep_ms(100);
            }
        } else {
            // Errou
            printf("Errou! (redPress=%d/%d, bluePress=%d/%d)\n",
                   userRedPresses, redCount, userBluePresses, blueCount);

            // Mantém LED vermelho aceso ou buzzer contínuo para indicar erro
            gpio_put(BUZZER_PIN, 1);
            sleep_ms(1000);
            gpio_put(BUZZER_PIN, 0);
        }

        // 4) Apaga LEDs antes da próxima rodada
        for (int i = 0; i < NUM_LEDS; i++) {
            ws2812_set_pixel(i, OFF);
        }
        ws2812_show();

        // Pausa breve antes de reiniciar
        sleep_ms(2000);
    }

    return 0;
}
