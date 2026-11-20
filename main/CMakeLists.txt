idf_component_register(SRCS "test_nvs.c" "cli_manager.c" "main.c" "nvs_manager.c"
    INCLUDE_DIRS "."
    REQUIRES nvs_flash)
