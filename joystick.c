#ifndef __JOYSTICK
#define __JOYSTICK

#include <stdlib.h>
#include "pico/stdlib.h"   // Biblioteca padrão do Raspberry Pi Pico
#include "hardware/pwm.h"  // Biblioteca para controle de PWM no RP2040
#include "hardware/adc.h" // Biblioteca para manipulação do ADC no RP2040

// Definição dos pinos usados para o joystick e LEDs
const int VRX = 26;          // Pino de leitura do eixo X do joystick (conectado ao ADC)
const int VRY = 27;          // Pino de leitura do eixo Y do joystick (conectado ao ADC)
const int ADC_CHANNEL_0 = 0; // Canal ADC para o eixo X do joystick
const int ADC_CHANNEL_1 = 1; // Canal ADC para o eixo Y do joystick
const int SW = 22;           // Pino de leitura do botão do joystick

const float DIVIDER_PWM_ADC = 16.0;          // Divisor fracional do clock para o PWM
const uint16_t PERIOD_ADC = 4096;            // Período do PWM (valor máximo do contador)


// Função para configurar o joystick (pinos de leitura e ADC)
void setup_joystick()
{
  adc_fifo_drain(); // Limpa a FIFO antes de ler diretamente
  // Inicializa o ADC e os pinos de entrada analógica
  adc_init();         // Inicializa o módulo ADC
  adc_gpio_init(VRX); // Configura o pino VRX (eixo X) para entrada ADC
  adc_gpio_init(VRY); // Configura o pino VRY (eixo Y) para entrada ADC

  // Inicializa o pino do botão do joystick
  gpio_init(SW);             // Inicializa o pino do botão
  gpio_set_dir(SW, GPIO_IN); // Configura o pino do botão como entrada
  gpio_pull_up(SW);          // Ativa o pull-up no pino do botão para evitar flutuações
}


// Canal e configurações do DMA
uint joystick_channel;
dma_channel_config joystick_cfg;

// Função de configuração geral
void setup()
{
  setup_joystick();                                // Chama a função de configuração do joystick
}

// Função para ler os valores dos eixos do joystick (X e Y)
void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value)
{
  // Leitura do valor do eixo X do joystick
  adc_select_input(ADC_CHANNEL_0); // Seleciona o canal ADC para o eixo X
  sleep_us(2);                     // Pequeno delay para estabilidade
  *vrx_value = adc_read();         // Lê o valor do eixo X (0-4095)

  // Leitura do valor do eixo Y do joystick
  adc_select_input(ADC_CHANNEL_1); // Seleciona o canal ADC para o eixo Y
  sleep_us(2);                     // Pequeno delay para estabilidade
  *vry_value = adc_read();         // Lê o valor do eixo Y (0-4095)
  printf("%4d-> x , %4d-> y \n\r", *vrx_value, *vry_value);
}

#endif