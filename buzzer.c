#ifndef __BUZZER
#define __BUZZER

#define BUZZER_PIN_1 10 // pino 10 do RP2040

// Configuração da frequência do buzzer (em Hz)
#define BUZZER_FREQUENCY1 5000  // frequencia que vai ser usada no alarme

// Definição de uma função para inicializar o PWM no pino do buzzer
void pwm_init_buzzer(uint pin1) {
    // Configurar o pino como saída de PWM
    gpio_set_function(pin1, GPIO_FUNC_PWM);

    // Obter o slice do PWM associado ao pino
    uint slice_num1 = pwm_gpio_to_slice_num(pin1);

    // Configurar o PWM com frequência desejada
    pwm_config config1 = pwm_get_default_config();
    pwm_config_set_clkdiv(&config1, clock_get_hz(clk_sys) / (BUZZER_FREQUENCY1 * 4096)); // Divisor de clock
    pwm_init(slice_num1, &config1, true);

    // Iniciar o PWM no nível baixo
    pwm_set_gpio_level(pin1, 0);
}

// Definição de uma função para emitir um beep com duração especificada
void beep_liga(uint pin1, uint duration_ms) {
    // Obter o slice do PWM associado ao pino
    uint slice_num1 = pwm_gpio_to_slice_num(pin1);

    // Configurar o duty cycle para 100% (ativo)
    pwm_set_gpio_level(pin1, 4095);

}
void beep_desliga(uint pin1, uint duration_ms) {
    // Obter o slice do PWM associado ao pino
    uint slice_num1 = pwm_gpio_to_slice_num(pin1);
    // Desativar o sinal PWM (duty cycle 0)
    pwm_set_gpio_level(pin1, 0);

}

#endif