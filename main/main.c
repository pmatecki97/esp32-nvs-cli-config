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
    
    nvs_mgr_load_defaults();//ładuje wartości domyslne parametrów

    printf("\n============================================\n");
    printf("   ESP32 NVS CLI - KONFIGURATOR RAKIETY\n");
    printf("============================================\n");
    printf("Dostepne typy: int, float, bool, str\n");
    printf("Komendy:\n");
    printf("  set <typ> <klucz> <wartosc>  - nowy parametr\n");
    printf("  mod <typ> <klucz> <wartosc>  - zmiana parametru\n");
    printf("  get <typ> <klucz>            - odczyt parametru\n");
    printf("  del <klucz>                  - usuniecie klucza\n");
    printf("  list                         - lista zapisanych danych\n");
    printf("  test                         - testy jednostkowe NVS\n");
    printf("Przyklad: mod int pulap 4500\n");
    printf("============================================\n\n");
    
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
        else if (c == '\b' || c == 127) { // obsługa backspace i delete
            if (idx > 0) {
                idx--;              
                printf("\b \b"); // usunięcie znaku z ekranu terminala
                fflush(stdout);
            }
        }
        else{ //wpisano zwykły znak
            if(idx < sizeof(line_buf)-1){
                line_buf[idx] = (char)c;
                idx++;
                putchar(c); //wypisuje znaki na ekranie na strumien stdout 
                fflush(stdout); 
            }
        }
    }
}