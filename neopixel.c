#ifndef __NEOPIXEL_INC
#define __NEOPIXEL_INC

#include <stdlib.h>
#include "ws2818b.pio.h"  // Arquivo que possibilita programacao do pino

// Predefinicoes de funcoes


void esquerda_joystick(void); // Cria uma seta na direcao esquerda
void direita_joystick(void);  // Cria uma seta na direcao direita
void frente_joystick(void);     // Cria uma seta na direcao pra frente
void atras_joystick(void);    // Cria uma seta na direcao pra tras
void neutro_joystick(void); // zona neutra

// Definição do número de LEDs e pino.
#define LED_PIN_MALHA 7
#define LED_COUNT_MALHA 25

// Definição de pixel GRB
struct pixel_t {
  uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t; // Mudança de nome de "struct pixel_t" para "npLED_t" por clareza.

// Declaração do buffer de pixels que formam a matriz.
static npLED_t *leds;
static uint led_count;

// Variáveis para uso da máquina PIO.
static PIO np_pio;
static uint np_sm;

/**
 * Inicializa a máquina PIO para controle da matriz de LEDs.
 */
// Variáveis globais para armazenar a SM usada anteriormente
static int last_sm = -1;  
static PIO last_pio = NULL; 

void npInit(uint pin, uint amount) {
    // Se já havia uma SM sendo usada, libera antes de pegar outra.
    if (last_sm >= 0 && last_pio != NULL) {
        printf("Liberando SM %d de PIO%d\n", last_sm, (last_pio == pio0) ? 0 : 1);
        pio_sm_unclaim(last_pio, last_sm);
    }

    led_count = amount;
    leds = (npLED_t *)calloc(led_count, sizeof(npLED_t));

    // Cria programa PIO.
    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_pio = pio0;

    // Toma posse de uma máquina PIO.
    np_sm = pio_claim_unused_sm(np_pio, false);
    if (np_sm < 0) {
        np_pio = pio1;
        np_sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
    }

    if (np_sm < 0) {
        printf("ERRO: Nenhuma SM disponível!\n");
        return;
    }

    // Salva qual PIO e SM foram usados, para liberar na próxima vez
    last_sm = np_sm;
    last_pio = np_pio;

    printf("Usando SM %d no PIO%d\n", np_sm, (np_pio == pio0) ? 0 : 1);

    // Inicia programa na máquina PIO obtida.
    ws2818b_program_init(np_pio, np_sm, offset, pin, 800000.f);

    // Define todos os LEDs como apagados
    for (uint i = 0; i < led_count; ++i) {
        leds[i].R = 0;
        leds[i].G = 0;
        leds[i].B = 0;
    }
}

/*
* funcao axiliar para facilitar o entendimento
*/
void npSetLED(const uint index,  uint8_t r,  uint8_t g, uint8_t b) {

  r = ((r & 0xF0) >> 4) | ((r & 0x0F) << 4); // Troca os 4 MSB com os 4 LSB
  r = ((r & 0xCC) >> 2) | ((r & 0x33) << 2); // Troca pares de bits
  r = ((r & 0xAA) >> 1) | ((r & 0x55) << 1); // Troca bits individuais

  g = ((g & 0xF0) >> 4) | ((g & 0x0F) << 4); // Troca os 4 MSB com os 4 LSB
  g = ((g & 0xCC) >> 2) | ((g & 0x33) << 2); // Troca pares de bits
  g = ((g & 0xAA) >> 1) | ((g & 0x55) << 1); // Troca bits individuais

  b = ((b & 0xF0) >> 4) | ((b & 0x0F) << 4); // Troca os 4 MSB com os 4 LSB
  b = ((b & 0xCC) >> 2) | ((b & 0x33) << 2); // Troca pares de bits
  b = ((b & 0xAA) >> 1) | ((b & 0x55) << 1); // Troca bits individuais

  leds[index].R = r;
  leds[index].G = g;
  leds[index].B = b;
}

/**
 * Limpa o buffer de pixels.
 */
void npClear() {
  for (uint i = 0; i < led_count; ++i)
    npSetLED(i, 0, 0, 0);
}

/**
 * Escreve os dados do buffer nos LEDs.
 */
void npWrite() {
  // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
  for (uint i = 0; i < led_count; ++i) {
    pio_sm_put_blocking(np_pio, np_sm, leds[i].G);
    pio_sm_put_blocking(np_pio, np_sm, leds[i].R);
    pio_sm_put_blocking(np_pio, np_sm, leds[i].B);
  }
  sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}

/*
// Funcoes auxiliares
*/

void frente_joystick(void){
  npClear();
  npSetLED(2, 255, 255, 255);
  npSetLED(7, 255, 255, 255);
  npSetLED(10, 255, 255, 255);
  npSetLED(12, 255, 255, 255);
  npSetLED(14, 255, 255, 255);
  npSetLED(16, 255, 255, 255);
  npSetLED(17, 255, 255, 255);
  npSetLED(18, 255, 255, 255);
  npSetLED(22, 255, 255, 255);
  npWrite();
}

void atras_joystick(void){
  npClear();
  npSetLED(2, 255, 255, 255);
  npSetLED(7, 255, 255, 255);
  npSetLED(10, 255, 255, 255);
  npSetLED(12, 255, 255, 255);
  npSetLED(14, 255, 255, 255);
  npSetLED(6, 255, 255, 255);
  npSetLED(17, 255, 255, 255);
  npSetLED(8, 255, 255, 255);
  npSetLED(22, 255, 255, 255);
  npWrite();
}
void direita_joystick(void){
  npClear();
  npSetLED(10, 255, 255, 255);
  npSetLED(11, 255, 255, 255);
  npSetLED(13, 255, 255, 255);
  npSetLED(12, 255, 255, 255);
  npSetLED(14, 255, 255, 255);
  npSetLED(2, 255, 255, 255);
  npSetLED(8, 255, 255, 255);
  npSetLED(18, 255, 255, 255);
  npSetLED(22, 255, 255, 255);
  npWrite();
}
void esquerda_joystick(void){
  npClear();
  npSetLED(10, 255, 255, 255);
  npSetLED(11, 255, 255, 255);
  npSetLED(13, 255, 255, 255);
  npSetLED(12, 255, 255, 255);
  npSetLED(14, 255, 255, 255);
  npSetLED(2, 255, 255, 255);
  npSetLED(6, 255, 255, 255);
  npSetLED(16, 255, 255, 255);
  npSetLED(22, 255, 255, 255);
  npWrite();
}
void neutro_joystick(void){
  npClear();
  npSetLED(7, 255, 255, 255);
  npSetLED(11, 255, 255, 255);
  npSetLED(12, 255, 255, 255);
  npSetLED(13, 255, 255, 255);
  npSetLED(17, 255, 255, 255);
  npWrite();
}

void ajuda_neoled(void) {
  for (uint i = 0; i < LED_COUNT_MALHA; ++i) {
    npSetLED(i, 255, 0, 0);
  }
  npWrite();
  sleep_ms(100);
  for (uint i = 0; i < LED_COUNT_MALHA; ++i) {
    npSetLED(i, 0, 0, 255);
  }
  npWrite();
  sleep_ms(100);
  npClear();
}

void perigo_mic(void) {
  for (uint i = 0; i < LED_COUNT_MALHA; ++i) {
    npSetLED(i, 50, 255, 255);
  }
  npWrite();
}

void perigo_mic_2(void) {
  for (uint i = 0; i < LED_COUNT_MALHA; ++i) {
    npSetLED(i, 255, 0, 0);
  }
  npWrite();
}

// Função para inverter os bits de um número de 8 bits para o neoled
uint8_t reverse_bits(uint8_t num) {
  uint8_t result = 0;
  for (int i = 0; i < 8; i++) {
      result <<= 1;         // Desloca os bits para a esquerda
      result |= (num & 1);  // Adiciona o bit menos significativo de 'num' ao 'result'
      num >>= 1;            // Desloca 'num' para a direita
  }
  return result;
}

#endif