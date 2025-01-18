#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

// Definindo o pino da matriz
#define PIN 7

// Criando o objeto da matriz 5x5
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(5, 5, PIN, NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG, NEO_GRB + NEO_KHZ800);

void setup() {
  matrix.begin(); // Inicia a matrix
  matrix.setBrightness(10);  // Define o brilho da matriz (0 a 255)
  matrix.fillScreen(0); // Limpa a tela
}

void loop() {
  // Limpa a tela
  matrix.fillScreen(0);

  // O primeiro for controla o eixo x
  for (int layer = 0; layer < matrix.width(); layer++) {

    // O segundo for controla o eixo y
    for (int i = 0; i < matrix.height(); i++) {

      // Nesta combinação, o todos os y de um mesmo x serão acesos antes de ir para o próximo
      matrix.drawPixel(i, layer, matrix.Color(255, 0, 0)); // Acende LED em vermelho
      matrix.show(); // Aplicar alterações
      delay(100); // Aguarda um pouco antes de passar para o próximo LED
    }
  }
}