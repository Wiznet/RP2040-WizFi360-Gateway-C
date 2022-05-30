set(TARGET_NAME wifi_to_ethernet_aws_mqtt)

add_executable(${TARGET_NAME}
        ${TARGET_NAME}.c
        app_main.c
        wifi_socket_startup.c
        wifi_tcp_ap_thread.c
        aws_ethernet_mqtt_thread.c
        )

target_include_directories(${TARGET_NAME} PUBLIC
        ${PORT_DIR}
        )

target_link_libraries(${TARGET_NAME} PRIVATE
        pico_stdlib
        hardware_spi
        hardware_dma
        ETHERNET_FILES
        IOLIBRARY_FILES
        DHCP_FILES
        DNS_FILES
        mbedcrypto
        mbedx509
        mbedtls
        TIMER_FILES
        cmsis_core
        CMSIS_FREERTOS_FILES
        FREERTOS_FILES
        #WIZFI360_FILES
        WIZFI360_DRIVER_FILES 
        IOT_SOCKET_FILES
        AWS_SDK_PORT_FILES
        )

pico_enable_stdio_usb(${TARGET_NAME} 0)
pico_enable_stdio_uart(${TARGET_NAME} 1)

pico_add_extra_outputs(${TARGET_NAME})
