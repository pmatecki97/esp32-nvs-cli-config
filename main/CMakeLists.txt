idf_component_register(
    SRCS "cli_manager.c" "main.c" "nvs_manager.c"
    INCLUDE_DIRS "."
    REQUIRES nvs_flash
)