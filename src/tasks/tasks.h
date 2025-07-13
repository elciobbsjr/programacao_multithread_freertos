#ifndef TASKS_H
#define TASKS_H

void tarefa_joystick(void *params);   // Leitura dos eixos VRx e VRy
void tarefa_botao(void *params);      // Leitura do botão SW
void tarefa_processamento(void *params); // Processamento e controle do buzzer

#endif
