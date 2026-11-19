#include <stdio.h>
#include"nvs_manager.h"
#include"cli_manager.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    esp_err_t err =nvs_mgr_init(); //przygotowanie partycji
    if(err!=ESP_OK){
        printf("Blad inicializacji partycji '%s'\n",esp_err_to_name(err));
        return;
    }
    printf("ESP32 TERMINAL RAM->FLASH GOTOWY!\n");
    
    char line_buf[128]; //bufor na wpisywane linijki
    int idx =0;
    
    while (1) {
        int c = getchar(); //pobiera znak KOD ASCII
        if(c==EOF){   //brak danych 
            vTaskDelay(pdMS_TO_TICKS(10)); //zabezpieczenie przed watchdogiem
        }
        else if(c == '\n'||c == '\r'){ //obsługa wciśnięcia enter
            if(idx>0){
                line_buf[idx] = '\0';
                printf("\r\n");
                cli_process_line(line_buf); // dodaje nowe polecenie do obsługi
                idx=0;
            }
        }
        else{ //wpisano zwykły znak
            if(idx < sizeof(line_buf)-1){
                line_buf[idx] = (char)c;
                idx++;
                putchar(c); 
                fflush(stdout); //wypisuje znaki na ekranie na strumien stdout
            }
        }
    }
}