#include <stdio.h>
#include <stdlib.h>  // Necessário para a função rand()
#include "pico/stdlib.h"
#include "ws2812b_animation.h"

// Máscara original do círculo 5x5
static uint8_t MASK_CIRCLE_5X5[] = {
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1
};

int main() {
    ws2812b_set_global_dimming(6);
    // Inicializa a comunicação serial
    stdio_init_all();

    // Inicialização do WS2812B
    ws2812b_init(pio0, 7, 25);

    // Inverter a máscara do círculo 5x5
    fixingBitDogLab(MASK_CIRCLE_5X5, 5, 5);

    while (true) {
        // Aplicar a máscara do círculo 5x5 ajustada
        ws2812b_set_mask(MASK_CIRCLE_5X5);

        // Percorrer cada índice da matriz e alterar a cor de cada pixel
        // for (int i = 0; i < 25; i++) {
        //     if (MASK_CIRCLE_5X5[i]) {
        //         ws2812b_clear();  // Limpa todos os LEDs antes de acender o próximo
        //         ws2812b_put(i, ws2812b_random_color(100.0));  // Define a cor aleatória para o LED atual
        //         ws2812b_render();  // Renderiza o LED atual
        //         sleep_ms(200);  // Pausa para visualização
        //     }
        // }

        // Renderizar a máscara com as cores aplicadas
        // ws2812b_render();

        // Pausar por 1 segundo para visualização
        // sleep_ms(1000);

        while(true){
            if (MASK_CIRCLE_5X5) {
                ws2812b_clear();  // Limpa todos os LEDs antes de acender o próximo
                ws2812b_fill_all(GRB_RED);
                sleep_ms(100);
            }

        }

        // Limpar todos os LEDs (desliga todos)
        ws2812b_render();

        // Pausa para garantir que a tela esteja limpa
        sleep_ms(500);

        // Remover a máscara para permitir o controle completo dos LEDs
        ws2812b_clear_mask();
    }
}

void invert_matrix_vertical(uint8_t* matrix, int rows, int cols) {
    for (int i = 0; i < rows / 2; i++) {
        for (int j = 0; j < cols; j++) {
            uint8_t temp = matrix[i * cols + j];
            matrix[i * cols + j] = matrix[(rows - i - 1) * cols + j];
            matrix[(rows - i - 1) * cols + j] = temp;
        }
    }
}

// Função para inverter apenas as linhas pares da matriz horizontalmente 
void invert_matrix_horizontal_even_rows(uint8_t* matrix, int rows, int cols) { 
    for (int i = 0; i < rows; i += 2) {
         // Incrementa de 2 em 2 para pegar apenas as linhas pares (0, 2, 4, ...)
        for (int j = 0; j < cols / 2; j++) {
            uint8_t temp = matrix[i * cols + j];
            matrix[i * cols + j] = matrix[i * cols + (cols - j - 1)];
            matrix[i * cols + (cols - j - 1)] = temp; 
        }
    } 
}

// Função para os desenhos funcionar normalmente, e placa ficar corretamente ajustada conforme as imagens da documentação
void fixingBitDogLab(uint8_t* matrix, int rows, int cols){
    invert_matrix_vertical(matrix, rows, cols);
    invert_matrix_horizontal_even_rows(matrix, rows, cols);
}
