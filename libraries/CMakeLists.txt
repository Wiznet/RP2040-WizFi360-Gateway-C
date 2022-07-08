
# FreeRTOS
add_library(FREERTOS_FILES STATIC)

target_sources(FREERTOS_FILES PUBLIC
        ${FREERTOS_DIR}/event_groups.c
        ${FREERTOS_DIR}/list.c
        ${FREERTOS_DIR}/queue.c
        ${FREERTOS_DIR}/stream_buffer.c
        ${FREERTOS_DIR}/tasks.c
        ${FREERTOS_DIR}/timers.c
        ${FREERTOS_DIR}/portable/GCC/ARM_CM0/port.c  		
        ${FREERTOS_DIR}/portable/MemMang/heap_4.c
)

target_include_directories(FREERTOS_FILES PUBLIC
        ${PORT_DIR}/FreeRTOS-Kernel/inc
        ${PORT_DIR}/RTE/inc
        ${FREERTOS_DIR}/include
        ${FREERTOS_DIR}/portable/GCC/ARM_CM0
        ${PICO_SDK_PATH}/src/rp2_common/cmsis/stub/CMSIS/Device/RaspberryPi/RP2040/Include
        ${PICO_SDK_PATH}/src/rp2_common/cmsis/stub/CMSIS/Core/Include
)

target_link_libraries(FREERTOS_FILES PRIVATE
        pico_stdlib
        cmsis_core
        CMSIS_FREERTOS_FILES
)

# CMSIS-FreeRTOS
add_library(CMSIS_FREERTOS_FILES STATIC)

target_sources(CMSIS_FREERTOS_FILES PUBLIC
        ${CMSIS_FREERTOS_DIR}/source/ARM/clib_arm.c        
        ${CMSIS_FREERTOS_DIR}/source/cmsis_os2.c        
        ${CMSIS_FREERTOS_DIR}/source/freertos_evr.c
        ${CMSIS_FREERTOS_DIR}/source/os_systick.c
)

target_include_directories(CMSIS_FREERTOS_FILES PUBLIC
        ${CMSIS_FREERTOS_DIR}/include
        ${CMSIS_5_DIR}/CMSIS/RTOS2/Include
        ${CMSIS_5_DIR}/CMSIS/Core/Include
        ${PICO_SDK_PATH}/src/rp2_common/cmsis/stub/CMSIS/Device/RaspberryPi/RP2040/Include
        ${PICO_SDK_PATH}/src/rp2_common/cmsis/stub/CMSIS/Core/Include
        ${PORT_DIR}/FreeRTOS-Kernel/inc
        ${PORT_DIR}/RTE/inc
)

target_link_libraries(CMSIS_FREERTOS_FILES PRIVATE
        pico_stdlib
        cmsis_core
        FREERTOS_FILES
)

# WizFi360 CMSIS-Driver
add_library(WIZFI360_DRIVER_FILES STATIC)

target_sources(WIZFI360_DRIVER_FILES PUBLIC
        ${WIZFI360_DRIVER_DIR}/BufList/BufList.c
        ${WIZFI360_DRIVER_DIR}/BufList/LinkList.c
        ${WIZFI360_DRIVER_DIR}/WiFi_WizFi360.c
        ${WIZFI360_DRIVER_DIR}/WiFi_WizFi360_Os.c
        ${WIZFI360_DRIVER_DIR}/WizFi360.c
        ${WIZFI360_DRIVER_DIR}/WizFi360_Serial.c
)

target_include_directories(WIZFI360_DRIVER_FILES PUBLIC
        ${WIZFI360_DRIVER_DIR}
        ${WIZFI360_DRIVER_DIR}/BufList
        ${CMSIS_5_DIR}/CMSIS/Driver/Include
        ${CMSIS_5_DIR}/CMSIS/RTOS2/Include
        ${CMSIS_5_DIR}/CMSIS/Core/Include
        ${FREERTOS_DIR}/include
        ${FREERTOS_DIR}/portable/GCC/ARM_CM0
        ${PORT_DIR}/FreeRTOS-Kernel/inc
)

target_link_libraries(WIZFI360_DRIVER_FILES PRIVATE
        pico_stdlib
        cmsis_core
        FREERTOS_FILES
        CMSIS_FREERTOS_FILES
        WIZFI360_FILES
)
# Ethernet
add_library(ETHERNET_FILES STATIC)

target_sources(ETHERNET_FILES PUBLIC
        ${WIZNET_DIR}/Ethernet/socket.c
        ${WIZNET_DIR}/Ethernet/wizchip_conf.c
        )

if(${WIZNET_CHIP} STREQUAL W5100S)
target_include_directories(ETHERNET_FILES INTERFACE
        ${WIZNET_DIR}/Ethernet
        ${WIZNET_DIR}/Ethernet/W5100S
        )

target_link_libraries(ETHERNET_FILES PUBLIC
        W5100S_FILES
        )
elseif(${WIZNET_CHIP} STREQUAL W5500)
target_include_directories(ETHERNET_FILES INTERFACE
        ${WIZNET_DIR}/Ethernet
        ${WIZNET_DIR}/Ethernet/W5500
        )

target_link_libraries(ETHERNET_FILES PUBLIC
        W5500_FILES
        )
endif()

if(${WIZNET_CHIP} STREQUAL W5100S)
add_library(W5100S_FILES STATIC)

target_sources(W5100S_FILES PUBLIC
        ${WIZNET_DIR}/Ethernet/W5100S/w5100s.c
        )

target_include_directories(W5100S_FILES INTERFACE
        ${WIZNET_DIR}/Ethernet
        ${WIZNET_DIR}/Ethernet/W5100S
        )

target_link_libraries(W5100S_FILES PRIVATE
        ETHERNET_FILES
        )
elseif(${WIZNET_CHIP} STREQUAL W5500)
add_library(W5500_FILES STATIC)

target_sources(W5500_FILES PUBLIC
        ${WIZNET_DIR}/Ethernet/W5500/w5500.c
        )

target_include_directories(W5500_FILES INTERFACE
        ${WIZNET_DIR}/Ethernet
        ${WIZNET_DIR}/Ethernet/W5500
        )

target_link_libraries(W5500_FILES PRIVATE
        ETHERNET_FILES
        )
endif()

# Loopback
add_library(LOOPBACK_FILES STATIC)

target_sources(LOOPBACK_FILES PUBLIC
        ${WIZNET_DIR}/Application/loopback/loopback.c
        )

target_include_directories(LOOPBACK_FILES PUBLIC
        ${WIZNET_DIR}/Ethernet
        ${WIZNET_DIR}/Application/loopback
        )

# DHCP
add_library(DHCP_FILES STATIC)

target_sources(DHCP_FILES PUBLIC
        ${WIZNET_DIR}/Internet/DHCP/dhcp.c
        )

target_include_directories(DHCP_FILES PUBLIC
        ${WIZNET_DIR}/Ethernet
        ${WIZNET_DIR}/Internet/DHCP
        )

# DNS
add_library(DNS_FILES STATIC)

target_sources(DNS_FILES PUBLIC
        ${WIZNET_DIR}/Internet/DNS/dns.c
        )

target_include_directories(DNS_FILES PUBLIC
        ${WIZNET_DIR}/Ethernet
        ${WIZNET_DIR}/Internet/DNS
        )

# FTP Client
add_library(FTPCLIENT_FILES STATIC)

target_sources(FTPCLIENT_FILES PUBLIC
        ${WIZNET_DIR}/Internet/FTPClient/ftpc.c
        )

target_include_directories(FTPCLIENT_FILES PUBLIC
        ${WIZNET_DIR}/Ethernet
        ${WIZNET_DIR}/Internet/FTPClient
        )

# FTP Server
add_library(FTPSERVER_FILES STATIC)

target_sources(FTPSERVER_FILES PUBLIC
        ${WIZNET_DIR}/Internet/FTPServer/ftpd.c
        )

target_include_directories(FTPSERVER_FILES PUBLIC
        ${WIZNET_DIR}/Ethernet
        ${WIZNET_DIR}/Internet/FTPServer
        )

# HTTP Server
add_library(HTTPSERVER_FILES STATIC)

target_sources(HTTPSERVER_FILES PUBLIC
        ${WIZNET_DIR}/Internet/httpServer/httpParser.c
        ${WIZNET_DIR}/Internet/httpServer/httpServer.c
        ${WIZNET_DIR}/Internet/httpServer/httpUtil.c
        )

target_include_directories(HTTPSERVER_FILES PUBLIC
        ${WIZNET_DIR}/Ethernet
        ${WIZNET_DIR}/Internet/httpServer
        )

# MQTT
add_library(MQTT_FILES STATIC)

target_sources(MQTT_FILES PUBLIC
        ${WIZNET_DIR}/Internet/MQTT/mqtt_interface.c
        ${WIZNET_DIR}/Internet/MQTT/MQTTClient.c
        ${WIZNET_DIR}/Internet/MQTT/MQTTPacket/src/MQTTConnectClient.c
        ${WIZNET_DIR}/Internet/MQTT/MQTTPacket/src/MQTTConnectServer.c
        ${WIZNET_DIR}/Internet/MQTT/MQTTPacket/src/MQTTDeserializePublish.c
        ${WIZNET_DIR}/Internet/MQTT/MQTTPacket/src/MQTTFormat.c
        ${WIZNET_DIR}/Internet/MQTT/MQTTPacket/src/MQTTPacket.c
        ${WIZNET_DIR}/Internet/MQTT/MQTTPacket/src/MQTTSerializePublish.c
        ${WIZNET_DIR}/Internet/MQTT/MQTTPacket/src/MQTTSubscribeClient.c
        ${WIZNET_DIR}/Internet/MQTT/MQTTPacket/src/MQTTSubscribeServer.c
        ${WIZNET_DIR}/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.c
        ${WIZNET_DIR}/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.c
        )

target_include_directories(MQTT_FILES PUBLIC
        ${WIZNET_DIR}/Ethernet
        ${WIZNET_DIR}/Internet/MQTT
        ${WIZNET_DIR}/Internet/MQTTPacket/src
        ) 

# SNTP
add_library(SNTP_FILES STATIC)

target_sources(SNTP_FILES PUBLIC
        ${WIZNET_DIR}/Internet/SNTP/sntp.c
        )

target_include_directories(SNTP_FILES PUBLIC
        ${WIZNET_DIR}/Ethernet
        ${WIZNET_DIR}/Internet/SNTP
        )

# AWS SDK
include(${AWS_SDK_DIR}/libraries/standard/coreHTTP/httpFilePaths.cmake)
include(${AWS_SDK_DIR}/libraries/standard/coreMQTT/mqttFilePaths.cmake)

add_library(AWS_SDK_FILES STATIC)

target_sources(AWS_SDK_FILES PUBLIC
        ${HTTP_SOURCES}
        ${MQTT_SOURCES}
        ${MQTT_SERIALIZER_SOURCES}
        )

target_include_directories(AWS_SDK_FILES PUBLIC
        ${HTTP_INCLUDE_PUBLIC_DIRS}
        ${MQTT_INCLUDE_PUBLIC_DIRS}
        ${PORT_DIR}/aws-iot-device-sdk-embedded-C/inc
        )

# IoT_Socket
add_library(IOT_SOCKET_FILES STATIC)

target_sources(IOT_SOCKET_FILES PUBLIC
        ${IOT_SOCKET_DIR}/source/wifi/iot_socket.c
        )

target_include_directories(IOT_SOCKET_FILES PUBLIC
        ${IOT_SOCKET_DIR}/include
        )

target_link_libraries(IOT_SOCKET_FILES PRIVATE
        pico_stdlib
        cmsis_core
        FREERTOS_FILES
        CMSIS_FREERTOS_FILES
        WIZFI360_FILES
)

include(${COREJSON_DIR}/jsonFilePaths.cmake)
add_library(COREJSON_FILES STATIC)

target_sources(COREJSON_FILES PUBLIC
        ${JSON_SOURCES}
        )

target_include_directories(COREJSON_FILES PUBLIC
        ${JSON_INCLUDE_PUBLIC_DIRS}
        )