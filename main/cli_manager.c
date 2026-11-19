#include"cli_manager.h"
#include"nvs_manager.h"
/*funkcja strtok(adres_początku," kiedy cięcie(ważna jest spacja) ") - funkcja służy do cięcia tekstu. 
Wstawia znak konca tekstu "\0" tam, gdzie kończy się słowo i zwraca odczytane do 
tego momenmtu słowo"*/
/*funkcja */
void cli_process_line(char *line){ 
    char *cmd = strtok(line, " \r\n"); //PIERWSZE SŁOWO -akcja
        if(cmd==NULL){ //zabezpieczenie przed linią pustą
        return;
    }

    char *type = strtok(NULL," \r\n");//DRUGIE SŁOWO - typ zmiennej
    char *key = strtok(NULL," \r\n");//TRZECIE SŁOWO - klucz(nazwa zmiennej)
    char *val_str =strtok(NULL," \r\n");//CZWARTE SŁOWO -wartośc zmiennej
    /*strcmp porownuje dwa stringi i znwraca 0 gdy identyczne */
    /*OBSŁUGA SET W LINI KOMEND*/
    if(strcmp(cmd, "set") == 0){
        if(type == NULL || key ==NULL || val_str == NULL){
            printf("Blad skladni! Użyj formatu set <typ> <klucz> <wartosc>\n");
        return;
        }
        if(nvs_mgr_key_exists(key)==true){
            printf("Klucz %s już istnieje! Użyj komendy 'mod'.\n",key);
        return;
        }
        /*
        atoi-asci to intiger
        atof-asci to float*/
        esp_err_t err = ESP_FAIL;
        if(strcmp(type,"int")==0){
            err=nvs_mgr_set_int(key, atoi(val_str));
        }
        else if(strcmp(type,"float")==0){
            err=nvs_mgr_set_float(key,atof(val_str));
        }
        else if(strcmp(type,"bool")==0){
            bool val;
            if(strcmp(val_str,"true")==0 || strcmp(val_str,"1")==0 ){
                 val=1;
            }
            else{
                 val=0;
            }
            err=nvs_mgr_set_bool(key,val);
        }
        else if(strcmp(type,"str")==0){
            err = nvs_mgr_set_string(key,val_str); //nie jnest wymagana żadna konwersja bo to string :)
        }
        else{
            printf("Podaj obslugiwany typ danych: int, float, bool, str\n");
            return;
        }
        if(err == ESP_OK){
            printf("Zapis udany. Zapisano '%s' \n",key);
        }
        else{
            printf("Blad zapisu '%s'\n", esp_err_to_name(err));//funkcja zamienia typ błędu na tekst
        }
    }
    /*OBSŁUGA GET W LINI KOMEND*/
    else if(strcmp(cmd,"get")==0){
        esp_err_t err= ESP_FAIL;
        if(type == NULL || key ==NULL){
            printf("Blad skladni! Użyj formatu get <typ> <klucz>\n");
        return;
        }
        if(strcmp(type,"int")==0){
            int32_t val=0;
            err=nvs_mgr_get_int(key,&val);
            if(err==ESP_OK){
                printf("%s = %ld\n",key,(long)val);
            }
        }
        else if(strcmp(type,"float")==0){
            float val = 0.0;
            err = nvs_mgr_get_float(key,&val);
            if(err==ESP_OK){
                printf("%s = %.4f\n",key,val);
            }
        }
        else if(strcmp(type,"bool")==0){
            bool val = false;
            err = nvs_mgr_get_bool(key,&val);
            if(err==ESP_OK){
                if(val==false){
                    printf("%s = false\n",key);
                }
                else{
                    printf("%s = true\n",key);
                }
            }
        }
        else if(strcmp(type,"str")==0){
            char val[64];
            err = nvs_mgr_get_string(key,val,sizeof(val));//nazwa tablicy jest wskaznikiem na 1 element
            if(err==ESP_OK){
                printf("%s = \"%s\"\n",key,val);                
            }
        }
        else{
            printf("Podaj obslugiwany typ danych: int, float, bool, str\n");
            return;
        }
        if(err!=ESP_OK){
            printf("Blad odczytu '%s'\n", esp_err_to_name(err));//funkcja zamienia typ błędu na tekst
        }
    }  
    /*OBSŁUGA MOD W LINI KOMEND
    sprawdzamy warunki a następnie 
    wywołujemy dokłądnie to samo co dla set*/
    else if(strcmp(cmd,"mod")==0){
       if(type == NULL || key ==NULL || val_str == NULL){
            printf("Blad skladni! Użyj formatu mod <typ> <klucz> <wartosc>\n");
        return;
        }
        if(nvs_mgr_key_exists(key)==false){
            printf("Klucz %s nie istnieje! Użyj komendy 'set' .\n",key);
            return;
        } 
        esp_err_t err = ESP_FAIL;
        if(strcmp(type,"int")==0){
            err=nvs_mgr_set_int(key, atoi(val_str));
        }
        else if(strcmp(type,"float")==0){
            err=nvs_mgr_set_float(key,atof(val_str));
        }
        else if(strcmp(type,"bool")==0){
            bool val;
            if(strcmp(val_str,"true")==0 || strcmp(val_str,"1")==0 ){
                 val=1;
            }
            else{
                 val=0;
            }
            err=nvs_mgr_set_bool(key,val);
        }
        else if(strcmp(type,"str")==0){
            err = nvs_mgr_set_string(key,val_str); //nie jnest wymagana żadna konwersja bo to string :)
        }
        else{
            printf("Podaj obslugiwany typ danych: int, float, bool, str\n");
            return;
        }
        if(err == ESP_OK){
            printf("Modyfikacja udana. Zapisano '%s' \n",key);
        }
        else{
            printf("Blad zapisu '%s'\n", esp_err_to_name(err));//funkcja zamienia typ błędu na tekst
        }
    }
    /*OBSŁUGA DEL W LINI KOMEND
    *ISTOTNY KOMENTARZ: Generalnie to komenda na usuwanie
     ma dwa słowa, czyli drugie słowo wpadnie do zmiennej type.
     OZNACZA TO ŻE W TYPE SIEDZI NAZWA KLUCZA !!!
    */
    else if(strcpmp(cmd,"del")==0){
        if(type==NULL){ //jesli nie podano klucza
            printf("Uzyj prawidlowego formatu: del <klucz>\n");
            return;
        }
        esp_err_t err = nvs_mgr_delete_key(type);//type to klucz
        if(err==ESP_OK){
            printf("Usunieto klucz '%s'\n",type);
        }
        else{
            printf("Blad usuwania '%s': %s\n",type,esp_err_to_name(err));
        }
    }
    else {
        printf("Podaj prawidlowa komende: set, get, mod, del\n");
    }  
}

