#include <blink.h>  // Traz todas as informacoes de outros arquivos 

/*
funcao principal
*/

int main() {

  // Inicializa entradas e saídas.
  stdio_init_all();

  // inicializa os sensores e atuadores
  BOTOES();
  i2c();

  //PWM
  setup_pwm();      // Configura o PWM
  npClear();
  
  uint16_t vrx_value, vry_value, sw_value; // Variáveis para armazenar os valores do joystick (eixos X e Y) e botão
  
  // Inicializar o PWM no pino do buzzer

  pwm_init_buzzer(BUZZER_PIN_1);

  // Preparar área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
  struct render_area frame_area = {
      start_column : 0,
      end_column : ssd1306_width - 1,
      start_page : 0,
      end_page : ssd1306_n_pages - 1
  };

  calculate_render_area_buffer_length(&frame_area); // calcula a area de renderizacao com o frame_area

  // zera o display inteiro
  uint8_t ssd[ssd1306_buffer_length]; // cria um novo nome
  memset(ssd, 0, ssd1306_buffer_length);
  render_on_display(ssd, &frame_area);
  limpar_o_display(ssd, &frame_area);
  restart:
 
  // Parte do código para exibir a mensagem no display (opcional: mudar ssd1306_height para 32 em ssd1306_i2c.h)
  escrever_display(ssd, &frame_area, text_init, count_of(text_init));

  // Delay para o usuário abrir o monitor serial...
  sleep_ms(5000);
  mic_config();
  escrever_display(ssd, &frame_area, text_init, count_of(text_init));
  while (true) {
    sample_mic();// Realiza uma amostragem do microfone.
    execute_mic(); //vai aparecer a intensidade/batimento cardiaco
    // Se apertar o botao A vai dirigir a cadeira
    // Se apertar o botao B vai chamar um cuidador

    // Se o botão A for pressionado
    if (gpio_get(BUTTON_A_PIN) == 0) {
      limpar_o_display(ssd, &frame_area);
      escrever_display(ssd, &frame_area, text_joystick, count_of(text_joystick));
      setup();
      npClear();
      printf("Locomover com o Joystick \n");
      sleep_ms(1000);
      while (gpio_get(BUTTON_A_PIN) == 1){ // So sai do loop depois que os dois forem pressionados;
        joystick_read_axis(&vrx_value, &vry_value); // Lê os valores dos eixos do joystick
        int zona_segura_min = 2048 - 400;
        int zona_segura_max = 2048 + 400;
        if (vrx_value > zona_segura_max) {
            if (vry_value >= zona_segura_max) {
                direita_joystick();
            } else if (vry_value < zona_segura_min) {
              esquerda_joystick();
            } else {
                frente_joystick();
              }
          }
        else if (vrx_value < zona_segura_min) {
          if (vry_value >= zona_segura_max) {
            direita_joystick();
          } else if (vry_value < zona_segura_min) {
            esquerda_joystick();
          } else {
            atras_joystick();
          }
        }
        else {
          if (vry_value >= zona_segura_max) {
            direita_joystick();
          } else if (vry_value < zona_segura_min) {
            esquerda_joystick();
          }
          else {
            neutro_joystick();
          }
        }
        // Pequeno delay antes da próxima leitura
        sleep_ms(10);
        npClear();
      }
      // Reconfigura novamente para o loop principal
      mic_config();
      sleep_ms(1000);
      pwm_init_buzzer(BUZZER_PIN_1);
      limpar_o_display(ssd, &frame_area);
      escrever_display(ssd, &frame_area, text_init, count_of(text_init));
    }
    else if (gpio_get(BUTTON_B_PIN) == 0) {
      limpar_o_display(ssd, &frame_area);
      escrever_display(ssd, &frame_area, text_ajuda, count_of(text_ajuda));
      beep_liga(BUZZER_PIN_1, 25); // Bipe de 500ms
      // possivel enviar mensagem wifi
      sleep_ms(1000);
      while (gpio_get(BUTTON_B_PIN) == 1) {
        ajuda_neoled(); // sinal de sirene pelo neoled
      }
      beep_desliga(BUZZER_PIN_1, 25); // desliga o Bipe 
      sleep_ms(1000);
      limpar_o_display(ssd, &frame_area);
      escrever_display(ssd, &frame_area, text_init, count_of(text_init));
    }
  }
}