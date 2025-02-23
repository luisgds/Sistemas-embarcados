#ifndef __GLOBAIS
#define __GLOBAIS

#include <stdio.h>  // Biblioteca padrão de entrada e saída
#include <stdlib.h> // Biblioteca padrão para alocação dinâmica de memória (malloc, free, etc.).
#include "pico/stdlib.h"   // Biblioteca padrão do Raspberry Pi Pico
#include "hardware/pio.h" // Biblioteca para manipulação da interface de periféricos programáveis (PIO).
#include "hardware/clocks.h" // Biblioteca para manipulação dos clocks do microcontrolador.
#include "hardware/pwm.h"  // Biblioteca para controle de PWM no RP2040
#include "hardware/i2c.h"  // Biblioteca para comunicação I2C com periféricos (ex: sensores, displays).
#include "hardware/adc.h" // Biblioteca para manipulação do ADC no RP2040
#include "hardware/dma.h" // Biblioteca para manipulação do DMA (Direct Memory Access), permitindo transferência direta de dados sem intervenção da CPU.
#include <string.h>   // Biblioteca padrão para manipulação de strings.
#include <math.h>     // Biblioteca para funções matemáticas avançadas (ex: sqrt, pow, sin, cos, etc.).
#include "ssd1306.h"  // Biblioteca para controle do display OLED SSD1306 via I2C.
#include "neopixel.c" // Inclusão do código do controle de LEDs RGB endereçáveis (Neopixel).
#include <mic_dma.c>  // Inclusão do código do microfone com DMA.
#include <joystick.c> // Inclusão do código do controle do joystick.
#include <pwm.c>      // Inclusão do código do controle de PWM.
#include <buzzer.c>   // Inclusão do código do buzzer (som).

/*
FUNÇOES AUXILIARES PARA A MAIN
*/

// Configuração do pino do buzzer
#define BUZZER_PIN_1 10

// Configuração da frequência do buzzer (em Hz)
#define BUZZER_FREQUENCY1 5000

// Pino e canal do microfone no ADC.
#define MIC_CHANNEL 2
#define MIC_PIN (26 + MIC_CHANNEL)

// Parâmetros e macros do ADC.
#define ADC_CLOCK_DIV 96.f  // numero que vai dividir
#define SAMPLES 200 // Número de amostras que serão feitas do ADC.
#define ADC_ADJUST(x) (x * 3.3f / (1 << 12u) - 1.65f) // Ajuste do valor do ADC para Volts.
#define ADC_STEP (3.3f/5.f) // Intervalos de volume do microfone.

#define abs(x) ((x < 0) ? (-x) : (x))

/*
Para controlar o PIO da matriz de led

*/
// Biblioteca gerada pelo arquivo .pio durante compilação.
#include "ws2818b.pio.h"

/*
configurar oled
*/

char *text_init[] = {
  "  MONITORANDO   ",
  "   BATIMENTO    ",
  "   CARDIACO     ",
  "                ",
  " BOTAO A MEXER  ",
  " BOTAO B AJUDA  ",};

char *text_joystick[] = {
  " USE O JOYSTICK ",
  " PARA LOCOMOVER ",
  "     FRENTE     ",
  " ESQ        DIR ",
  "     ATRAS      "};

char *text_ajuda[] = {
  "  CHAMANDO  ",
  "   AJUDA "};

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

// Configurar pinos dos botoes

#define BUTTON_A_PIN 5  // Botão A no GPIO 5
#define BUTTON_B_PIN 6  // Botão B no GPIO 6
/*
FUNÇOES AUXILIARES
*/
void BOTOES(){
  // pro botão pra acender a matriz de leds de forma colorida
  // Configuração dos pinos dos botões como entrada com pull-up
  gpio_init(BUTTON_A_PIN);
  gpio_init(BUTTON_B_PIN);
  gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
  gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
  gpio_pull_up(BUTTON_A_PIN);
  gpio_pull_up(BUTTON_B_PIN);
}
void i2c() {
  // Inicialização do i2c
  i2c_init(i2c1, ssd1306_i2c_clock * 1000);
  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
  gpio_pull_up(I2C_SDA);
  gpio_pull_up(I2C_SCL);

  // Processo de inicialização completo do OLED SSD1306
  ssd1306_init();
}
#endif