#include "nvs_manager.h"
#include <nvs.h>
#include <stdint.h>//koniczne do typu int32_t
#include "nvs_flash.h" //dolaczenie biblioteki obslugujacej pamiec flash
#include<stdbool.h>//do obsługi typu bool
static const char *NAMESPACE ="rocket_cfg";//stała globalna nazwa naszej przestrzeni
//esp_err_t - specjalny typ do raportowania bledow (ESP_OK -dy brak bledu, gdy blad - konkretny kad bledu)
esp_err_t nvs_mgr_init(void){ 
    esp_err_t err = nvs_flash_init(); //funkcja szuka partycji NVM w pamieci Flash i zapisuje gdzie sie znajduje do pamieci RAM
    if(err==ESP_ERR_NVS_NO_FREE_PAGES ||err==ESP_ERR_NVS_NEW_VERSION_FOUND){
        nvs_flash_erase();//czyszczenie partycji 
        err=nvs_flash_init();//ponowna proba inicjacji pamieci
    
    }
return err; // zwraca czy wystapil blad w trakcie wykonywania nvs_flash_init()
}
/* (pierwszy setter) nvs_mgr_set_itn - funkcja zapisuje zmienną calkowita do naszego folderu w pamieci nieulotnej mikrokontrolera
funkcja przyjmuje argumenty: key-nazwa zmiennej,value-wartosc zmiennej 
handle - numer identyfikacyjny naszej danej liczbowej (uchwyt) W PAMIECI RAM!
nvs_open(NASZ NAMESPACE,TRYB(ZAPIS ODCZYT...),ADRES NUMERU IDENTYFIKACYJNEGO ZMIENNEJ)
powyższa funkcja otwiera nasz "folder" w NVS i przypisuje mu uchwyt(handle)  
nvs_set_i32(uchwyt,klucz,wartosc) - ustawia nowa wartosc dla danego klucza w szybkim buforze RAM
nvs_commit(uchwyt) - sapisuje dane z RAM na Flash
ZAPIS INT DO FLASH*/ 
esp_err_t nvs_mgr_set_int(const char *key, int32_t value){
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NAMESPACE,NVS_READWRITE,&handle);
    if(err==ESP_OK){
        err=nvs_set_i32(handle,key,value);//zapisanie danych (int_32) w buforze RAM
    }
    if(err==ESP_OK){
         err  = nvs_commit(handle);//fizyczny zapis do pamieci flash
    }
    nvs_close(handle);//zamkniecie pamieci flash 
    return err;
}
/*(drugi setter) to samo co funkcja wyzej, tylko w pierwszym ifie 
dokonujemy konwersji bool na uint_8
HANDLE TO UCHWYT DO NASZEJ DANEJ W RAM KTORY PRZEKAZUJEMY 
FUNKCJOM OTWIERAJACYM, ZAPISUJACYM I ZAMYKAJACYM FLASH 
ZAPIS BOOL DO FLASH*/
esp_err_t nvs_mgr_set_bool(const char *key,bool value){
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NAMESPACE,NVS_READWRITE,&handle);
    if(err==ESP_OK){
        uint8_t raw_bool;
        if (value==true){
            raw_bool=1;
        }
        else{
            raw_bool=0;
        }
        err = nvs_set_u8(handle,key,raw_bool);//zapisanie danych (uint_8) w buforze RAM
    }
    if(err==ESP_OK){
        err  = nvs_commit(handle);//fizyczny zapis do pamieci flash
    }
    nvs_close(handle);//zamkniecie pamieci flash 
    return err;
}
/*(trzeci setter)
NVS nie ma wydzielonej czesci na float wiec wrzucamy go do 
szuflady uniwersalnej "BLOB" - binary large object (ciag bajtow) 
nvs_set_blob(uchwyt,klucz, adres wartosci,rozmiar danej (ilosc bajtow)
powyzsza funkcja odczytuje ile bajtów z jakiego adresu ma wziac, 
i nastepnie zapisuje ja w buforze RAM  
FUNKCJA OPEN ZAPISUJE UCHWYT DO NASZEGO FOLDERU BUFORA W RAM 
ZAPIS FLOAT DO FLASH*/
esp_err_t nvs_mgr_set_float(const char *key, float value){
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NAMESPACE,NVS_READWRITE,&handle);
    if(err==ESP_OK){
        err = nvs_set_blob(handle,key,&value,sizeof(float));
    }
    if(err==ESP_OK){
        err=nvs_commit(handle);
    }
    nvs_close(handle);
    return err;
}
/*(czwarty setter)
dokładnie to samo co powyżej tylko używamy funkcji dedykowanej 
do stringow - mvs_set_str(uchwyt,klucz,nasze slowo)
ZAPIS STRING DO FLASH*/
esp_err_t nvs_mgr_set_string(const char *key,const char *value){
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NAMESPACE,NVS_READWRITE,&handle);
    if(err==ESP_OK){
        err = nvs_set_str(handle,key,value);
    }
    if(err==ESP_OK){
        err=nvs_commit(handle);
    }    
    nvs_close(handle);
    return err;
}
/*(pierwszy getter)
zmieniamy tryb na odczyt
odczyt nie wymaga commita  
ODCZYT INT Z FLASH*/
esp_err_t nvs_mgr_get_int(const char* key,int32_t* value){
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NAMESPACE,NVS_READONLY,&handle);    
    if(err==ESP_OK){
        err = nvs_get_i32(handle,key,value);
    }
    nvs_close(handle);
    return err;
}
/*(drugi getter)  
ponieważ zapisaliśmy bool jako 0 lub 1 tyak samo musimy odczytać 
ODCZYT BOOL Z FLASH*/
esp_err_t nvs_mgr_get_bool(const char* key,bool* value){
    nvs_handle_t handle;
    uint8_t raw_bool = 0;
    esp_err_t err =nvs_open(NAMESPACE,NVS_READONLY,&handle);
    if(err == ESP_OK){
        err = nvs_get_u8(handle,key, &raw_bool);
        if(err==ESP_OK){   // GDYBY NP. WARTOSC NIE ISTNIAŁA
            if((raw_bool)==0){
            *value = false;
            } 
            else{
                *value = true;
            }
        }
    }
    nvs_close(handle);
    return err;
}
/*(trzeci getter)
float został zapisany jako blob (binay large object) (32 bity)*
ODCZYT FLOAT Z FLASH*/
esp_err_t nvs_mgr_get_float(const char* key, float*value){
    nvs_handle_t handle;
    size_t size = sizeof(float);
    esp_err_t err = nvs_open(NAMESPACE,NVS_READONLY,&handle);
    if(err==ESP_OK){
        err  = nvs_get_blob(handle,key,value,&size);
    }   
    nvs_close(handle);
    return err;
}
/*(czwarty getter)
max_len jest wymagany do działania funkcji nvs_get_str
ODCZYT STRING Z FLASH*/
esp_err_t nvs_mgr_get_string(const char* key,char* value,size_t max_len){
    nvs_handle_t handle;
    size_t size =  max_len;
    esp_err_t err =nvs_open(NAMESPACE,NVS_READONLY,&handle);
    if(err==ESP_OK){
        err = nvs_get_str(handle,key,value,&size);
    }
    nvs_close(handle);
    return err;
}
/*FUNKCJA SPRAWDZA ISTNIENIE KLUCZA W FLASH
zwraca true gdy istnieje w flash zmienna o danym kluczu*/
bool nvs_mgr_key_exists(const char* key){
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NAMESPACE, NVS_READONLY,&handle);
    if(err==ESP_OK){
        nvs_type_t out_type;//typ poszukiwanej zmiennej
        err =nvs_find_key(handle,key,&out_type); //funkcja szuka w flash elementu o zadanym kluczu
        nvs_close(handle);
    }
    if(err==ESP_OK){
        return true;
    }
    else{
        return false;
    }
}
/*FUNKCJA USUWA ZMIENNĄ O ZADANYM KLUCZU*/
esp_err_t nvs_mgr_delete_key(const char *key){
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NAMESPACE,NVS_READWRITE,&handle);
    if(err==ESP_OK){
        err=nvs_erase_key(handle,key);//funkcja usuwa daną z pamieci flash
        if(err==ESP_OK){
            err=nvs_commit(handle);//po jakiejkolwiek zmianie w flash wymagany commit
        }
        nvs_close(handle);
    }
    return err;
}