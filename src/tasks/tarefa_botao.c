#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "tasks.h"

extern QueueHandle_t fila_eventos;

void tarefa_botao(void *params) {
    const uint BUTTON_PIN = 22;
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    bool last_state = true;

    while (true) {
        bool current_state = gpio_get(BUTTON_PIN);
        if (!current_state && last_state) {
            uint32_t evento_botao = 0xFFFFFFFF; // Valor especial para o botão
            xQueueSend(fila_eventos, &evento_botao, portMAX_DELAY);
        }
        last_state = current_state;
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
