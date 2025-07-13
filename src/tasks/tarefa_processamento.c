#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "tasks.h"

// Variáveis externas definidas em main.c
extern QueueHandle_t fila_eventos;
extern SemaphoreHandle_t mutex_terminal;
extern SemaphoreHandle_t sem_buzzer;

void tarefa_processamento(void *params) {
    const uint BUZZER_PIN = 21;
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);

    uint16_t ultimo_vrx = 0;
    uint16_t ultimo_vry = 0;
    bool primeiro_leitura = true;

    while (true) {
        uint32_t dados;
        if (xQueueReceive(fila_eventos, &dados, portMAX_DELAY)) {

            // === Impressão protegida com mutex ===
            if (xSemaphoreTake(mutex_terminal, portMAX_DELAY)) {
                if (dados == 0xFFFFFFFF) {
                    printf("[BOTÃO] Pressionado\n");
                } else {
                    uint16_t vrx = dados & 0xFFFF;
                    uint16_t vry = (dados >> 16) & 0xFFFF;
                    printf("[JOYSTICK] X: %u, Y: %u\n", vrx, vry);
                }
                xSemaphoreGive(mutex_terminal);
            }

            // === Detecção de evento válido ===
            bool evento_valido = false;

            if (dados == 0xFFFFFFFF) {
                evento_valido = true;  // Botão clicado
            } else {
                uint16_t vrx = dados & 0xFFFF;
                uint16_t vry = (dados >> 16) & 0xFFFF;

                if (!primeiro_leitura) {
                    int diff_x = abs(vrx - ultimo_vrx);
                    int diff_y = abs(vry - ultimo_vry);

                    if (diff_x > 200 || diff_y > 200) {
                        evento_valido = true;
                    }
                }

                // Atualiza os últimos valores para a próxima verificação
                ultimo_vrx = vrx;
                ultimo_vry = vry;
                primeiro_leitura = false;
            }

            // === Acionamento do buzzer ===
            if (evento_valido) {
                if (xSemaphoreTake(sem_buzzer, 0) == pdTRUE) {
                    gpio_put(BUZZER_PIN, 1);
                    vTaskDelay(pdMS_TO_TICKS(100));
                    gpio_put(BUZZER_PIN, 0);
                    xSemaphoreGive(sem_buzzer);
                }
            }
        }
    }
}
