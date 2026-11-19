#ifndef NVS_MANAGER_H
#define NVS_MANAGER_H
#include "esp_err.h" //umożliwa używanie typu esp_err_t
#include<stdint.h>
#include<stdbool.h>
#include<stddef.h>
esp_err_t nvs_mgr_init(void); //funkcja przygotowująca pamięć Flash do pracuy
esp_err_t nvs_mgr_set_int(const char *key, int32_t value);
esp_err_t nvs_mgr_set_bool(const char *key, bool value);
esp_err_t nvs_mgr_set_float(const char *key, float value);
esp_err_t nvs_mgr_set_string(const char *key,const char* value);
esp_err_t nvs_mgr_get_int(const char *key, int32_t* value);
esp_err_t nvs_mgr_get_bool(const char *key, bool* value);
esp_err_t nvs_mgr_get_float(const char *key, float* value);
esp_err_t nvs_mgr_get_string(const char *key, char* value,size_t max_len);
bool nvs_mgr_key_exists(const char *key);
esp_err_t nvs_mgr_delete_key(const char *key);
#endif 