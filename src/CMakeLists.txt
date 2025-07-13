# src/CMakeLists.txt

# Cria o executável com os arquivos principais
add_executable(picow_freertos
    main.c
    tasks/tarefa_joystick.c
    tasks/tarefa_botao.c
    tasks/tarefa_processamento.c
)

# Corrige a saída do executável para a raiz do build/
set_target_properties(picow_freertos PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Adiciona o gerenciador de heap do FreeRTOS
target_sources(picow_freertos PRIVATE
    ${FREERTOS_KERNEL_PATH}/portable/MemMang/heap_4.c
)

# Diretórios de headers incluídos
target_include_directories(picow_freertos PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}          # src/
    ${CMAKE_CURRENT_LIST_DIR}/config   # src/config/
    ${CMAKE_CURRENT_LIST_DIR}/tasks    # src/tasks/
    ${CMAKE_CURRENT_LIST_DIR}/drivers  # src/drivers/ (se precisar no futuro)
)

# Bibliotecas a serem linkadas
target_link_libraries(picow_freertos
    pico_stdlib
    hardware_adc
    FreeRTOS-Kernel
)

# Gera arquivos UF2, bin e outros
pico_add_extra_outputs(picow_freertos)
