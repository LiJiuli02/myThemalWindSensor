set(src_dirs
            ADC
            KEY
            WIND_CALCU
            QMC5883P
            )

set(include_dirs
            ADC
            KEY
            WIND_CALCU
            QMC5883P
            )

set(requires
            driver
            esp_timer
            nvs_flash)

idf_component_register(SRC_DIRS ${src_dirs} INCLUDE_DIRS ${include_dirs} REQUIRES ${requires})

component_compile_options(-ffast-math -O3 -Wno-error=format=-Wno-format)