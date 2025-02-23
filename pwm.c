#ifndef __PWM
#define __PWM

#include "hardware/pwm.h"  // Biblioteca para controle de PWM no RP2040
#include <stdio.h>  // Biblioteca padrão de entrada e saída
#include "pico/stdlib.h"   // Biblioteca padrão do Raspberry Pi Pico

/*
Para controlar o LED RGB 

*/
// Biblioteca gerada pelo arquivo .pio durante compilação.
#include "ws2818b.pio.h"

/*
* configuraçao para pwm

*/

void setup_pwm(); // inicializacao da funcao de configuraçao de pwm

const uint LED = 12;            // Pino do LED conectado
const uint16_t PERIOD = 1000;   // Período do PWM (valor máximo do contador)
const float DIVIDER_PWM = 1.0; // Divisor fracional do clock para o PWM
const uint16_t LED_STEP = 1;  // Passo de incremento/decremento para o duty cycle do LED
uint16_t led_level = 1;       // Nível inicial do PWM (duty cycle)

void setup_pwm()    // Funcao de configuraçao de pwm
{
    uint slice;
    gpio_set_function(LED, GPIO_FUNC_PWM); // Configura o pino do LED para função PWM
    slice = pwm_gpio_to_slice_num(LED);    // Obtém o slice do PWM associado ao pino do LED
    pwm_set_clkdiv(slice, DIVIDER_PWM);    // Define o divisor de clock do PWM
    pwm_set_wrap(slice, PERIOD);           // Configura o valor máximo do contador (período do PWM)
    pwm_set_gpio_level(LED, led_level);    // Define o nível inicial do PWM para o pino do LED
    pwm_set_enabled(slice, true);          // Habilita o PWM no slice correspondente
}

#endif