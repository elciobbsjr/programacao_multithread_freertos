#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "tasks.h"

extern QueueHandle_t fila_eventos;

void tarefa_joystick(void *params) {
    adc_init();
    adc_gpio_init(26); // VRy
    adc_gpio_init(27); // VRx

    while (true) {
        adc_select_input(0); // GPIO26 (VRy)
        uint16_t vry = adc_read();

        adc_select_input(1); // GPIO27 (VRx)
        uint16_t vrx = adc_read();

        uint32_t dados = (vry << 16) | (vrx & 0xFFFF);
        xQueueSend(fila_eventos, &dados, portMAX_DELAY);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
