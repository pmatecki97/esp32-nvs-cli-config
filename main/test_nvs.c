#include "test_nvs.h"
#include "nvs_manager.h"
#include <stdio.h>
#include <string.h>
/*FUNKCJA TESTÓW JEDNOSTKOWYCH*/
void run_nvs_unit_tests(void) {
    int passed = 0;
    int failed = 0;

    printf("\nTESTY JEDNOSTKOWE\n");
    /*TESTUJEMY INT*/
    int32_t out_int = 0;
    nvs_mgr_set_int("test_int", 4250);
    if (nvs_mgr_get_int("test_int", &out_int) == ESP_OK && out_int == 4250) {
        printf("[OK] Test INT\n");
        passed++;
    } else {
        printf("[BLAD] Test INT\n");
        failed++;
    }
    nvs_mgr_delete_key("test_int");

    /*TESTUJEMY FLOAT*/
    float out_flt = 0.0f;
    nvs_mgr_set_float("test_flt", 85.5f);
    if (nvs_mgr_get_float("test_flt", &out_flt) == ESP_OK && out_flt > 85.49f && out_flt < 85.51f) {
        printf("[OK] Test FLOAT\n");
        passed++;
    } else {
        printf("[BLAD] Test FLOAT\n");
        failed++;
    }
    nvs_mgr_delete_key("test_flt");

    /*TESTUJEMY BOOL*/
    bool out_bool = false;
    nvs_mgr_set_bool("test_bool", true);
    if (nvs_mgr_get_bool("test_bool", &out_bool) == ESP_OK && out_bool == true) {
        printf("[OK] Test BOOL\n");
        passed++;
    } else {
        printf("[BLAD] Test BOOL\n");
        failed++;
    }
    nvs_mgr_delete_key("test_bool");

    /*TESTUJEMY STRING*/
    char out_str[32] = {0};
    nvs_mgr_set_string("test_str", "Rakieta");
    if (nvs_mgr_get_string("test_str", out_str, sizeof(out_str)) == ESP_OK && strcmp(out_str, "Rakieta") == 0) {
        printf("[OK] Test STRING\n");
        passed++;
    } else {
        printf("[BLAD] Test STRING\n");
        failed++;
    }
    nvs_mgr_delete_key("test_str");

    /*TESTUJEMY MODYFIKACJE I USUWANIE*/
    nvs_mgr_set_int("test_mod", 10);
    nvs_mgr_set_int("test_mod", 99);
    nvs_mgr_get_int("test_mod", &out_int);
    nvs_mgr_delete_key("test_mod");

    if (out_int == 99 && nvs_mgr_key_exists("test_mod") == false) {
        printf("[OK] Test MOD i DEL\n");
        passed++;
    } else {
        printf("[BLAD] Test MOD i DEL\n");
        failed++;
    }

    printf("WYNIK TESTOW: %d/5 zaliczonych (bledy: %d) ===\n", passed, failed);
}