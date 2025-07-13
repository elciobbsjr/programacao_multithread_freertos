#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "tasks.h"

QueueHandle_t fila_eventos;
SemaphoreHandle_t mutex_terminal;
SemaphoreHandle_t sem_buzzer;

int main() {
    stdio_init_all();

    fila_eventos = xQueueCreate(10, sizeof(uint32_t));
    mutex_terminal = xSemaphoreCreateMutex();
    sem_buzzer = xSemaphoreCreateCounting(2, 2); // Máx. 2 acessos

    xTaskCreate(tarefa_joystick, "LeituraJoystick", 1024, NULL, 1, NULL);
    xTaskCreate(tarefa_botao, "LeituraBotao", 1024, NULL, 1, NULL);
    xTaskCreate(tarefa_processamento, "Processamento", 1024, NULL, 2, NULL);

    vTaskStartScheduler();

    while (true); // Nunca deve chegar aqui
    return 0;
}
