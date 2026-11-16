#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    printf("\n========================================\n");
    printf("PWr in Space - System startuje poprawnie!\n");
    printf("========================================\n");

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}