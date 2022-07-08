# CMake minimum required version
cmake_minimum_required(VERSION 3.12)

# Find git
find_package(Git)

if(NOT Git_FOUND)
	message(FATAL_ERROR "Could not find 'git' tool for RP2040-WizFi360-Gateway-C patching")
endif()

message("RP2040-WizFi360-Gateway-C patch utils found")

set(RP2040_WIFI360_GATEWAY_C_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(AWS_IOT_DEVICE_SDK_EMBEDDED_C_SRC_DIR "${RP2040_WIFI360_GATEWAY_C_SRC_DIR}/libraries/aws-iot-device-sdk-embedded-C")
set(AWS_IOT_DEVICE_SDK_EMBEDDED_C_COREHTTP_SRC_DIR "${RP2040_WIFI360_GATEWAY_C_SRC_DIR}/libraries/aws-iot-device-sdk-embedded-C/libraries/standard/coreHTTP")
set(AWS_IOT_DEVICE_SDK_EMBEDDED_C_COREMQTT_SRC_DIR "${RP2040_WIFI360_GATEWAY_C_SRC_DIR}/libraries/aws-iot-device-sdk-embedded-C/libraries/standard/coreMQTT")
set(AWS_IOT_DEVICE_SDK_EMBEDDED_C_COREHTTP_HTTP_PARSER_SRC_DIR "${RP2040_WIFI360_GATEWAY_C_SRC_DIR}/libraries/aws-iot-device-sdk-embedded-C/libraries/standard/coreHTTP/source/dependency/3rdparty/http_parser")
set(CMSIS_5_SRC_DIR "${RP2040_WIFI360_GATEWAY_C_SRC_DIR}/libraries/CMSIS_5")
set(CMSIS_DRIVER_SRC_DIR "${RP2040_WIFI360_GATEWAY_C_SRC_DIR}/libraries/CMSIS-Driver")
set(CMSIS_FREERTOS_SRC_DIR "${RP2040_WIFI360_GATEWAY_C_SRC_DIR}/libraries/CMSIS-FreeRTOS")
set(IOT_SOCKET_SRC_DIR "${RP2040_WIFI360_GATEWAY_C_SRC_DIR}/libraries/IoT_Socket")
set(MBEDTLS_SRC_DIR "${RP2040_WIFI360_GATEWAY_C_SRC_DIR}/libraries/CMSIS-Driver")
set(PICO_EXTRAS_SRC_DIR "${RP2040_WIFI360_GATEWAY_C_SRC_DIR}/libraries/pico-extras")
set(PICO_SDK_SRC_DIR "${RP2040_WIFI360_GATEWAY_C_SRC_DIR}/libraries/pico-sdk")
set(PICO_SDK_TINYUSB_SRC_DIR "${RP2040_WIFI360_GATEWAY_C_SRC_DIR}/libraries/lib/tinyusb")
set(RP2040_WIFI360_GATEWAY_C_PATCH_DIR "${RP2040_WIFI360_GATEWAY_C_SRC_DIR}/patches")

# Delete untracked files in aws-iot-device-sdk-embedded-C
if(EXISTS "${AWS_IOT_DEVICE_SDK_EMBEDDED_C_SRC_DIR}/.git")
	message("cleaning aws-iot-device-sdk-embedded-C...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${AWS_IOT_DEVICE_SDK_EMBEDDED_C_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${AWS_IOT_DEVICE_SDK_EMBEDDED_C_SRC_DIR} reset --hard)
	message("aws-iot-device-sdk-embedded-C cleaned")
endif()

# Delete untracked files in CMSIS_5
if(EXISTS "${CMSIS_5_SRC_DIR}/.git")
	message("cleaning CMSIS_5...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${CMSIS_5_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${CMSIS_5_SRC_DIR} reset --hard)
	message("CMSIS_5 cleaned")
endif()

# Delete untracked files in CMSIS-Driver
if(EXISTS "${CMSIS_DRIVER_SRC_DIR}/.git")
	message("cleaning CMSIS-Driver...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${CMSIS_DRIVER_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${CMSIS_DRIVER_SRC_DIR} reset --hard)
	message("CMSIS-Driver cleaned")
endif()

# Delete untracked files in CMSIS-FreeRTOS
if(EXISTS "${CMSIS_FREERTOS_SRC_DIR}/.git")
	message("cleaning CMSIS-FreeRTOS...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${CMSIS_FREERTOS_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${CMSIS_FREERTOS_SRC_DIR} reset --hard)
	message("CMSIS-FreeRTOS cleaned")
endif()

# Delete untracked files in pico-sdk
if(EXISTS "${COREJSON_DIR}/.git")
	message("cleaning coreJSON...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${COREJSON_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${COREJSON_DIR} reset --hard)
	message("coreJSON cleaned")
endif()

# Delete untracked files in IoT_Socket
if(EXISTS "${IOT_SOCKET_SRC_DIR}/.git")
	message("cleaning IoT_Socket...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${IOT_SOCKET_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${IOT_SOCKET_SRC_DIR} reset --hard)
	message("IoT_Socket cleaned")
endif()

# Delete untracked files in mbedtls
if(EXISTS "${MBEDTLS_SRC_DIR}/.git")
	message("cleaning mbedtls...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${MBEDTLS_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${MBEDTLS_SRC_DIR} reset --hard)
	message("mbedtls cleaned")
endif()

# Delete untracked files in pico-extras
if(EXISTS "${PICO_EXTRAS_SRC_DIR}/.git")
	message("cleaning pico-extras...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_EXTRAS_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_EXTRAS_SRC_DIR} reset --hard)
	message("pico-extras cleaned")
endif()

# Delete untracked files in pico-sdk
if(EXISTS "${PICO_SDK_SRC_DIR}/.git")
	message("cleaning pico-sdk...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_SDK_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_SDK_SRC_DIR} reset --hard)
	message("pico-sdk cleaned")
endif()

execute_process(COMMAND ${GIT_EXECUTABLE} -C ${RP2040_WIFI360_GATEWAY_C_SRC_DIR} submodule update --init)

# Delete untracked files in coreHTTP
if(EXISTS "${AWS_IOT_DEVICE_SDK_EMBEDDED_C_COREHTTP_SRC_DIR}/.git")
	message("cleaning aws-iot-device-sdk-embedded-C coreHTTP...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${AWS_IOT_DEVICE_SDK_EMBEDDED_C_COREHTTP_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${AWS_IOT_DEVICE_SDK_EMBEDDED_C_COREHTTP_SRC_DIR} reset --hard)
	message("aws-iot-device-sdk-embedded-C coreHTTP cleaned")
endif()

# Delete untracked files in coreMQTT
if(EXISTS "${AWS_IOT_DEVICE_SDK_EMBEDDED_C_COREMQTT_SRC_DIR}/.git")
	message("cleaning aws-iot-device-sdk-embedded-C coreMQTT...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${AWS_IOT_DEVICE_SDK_EMBEDDED_C_COREMQTT_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${AWS_IOT_DEVICE_SDK_EMBEDDED_C_COREMQTT_SRC_DIR} reset --hard)
	message("aws-iot-device-sdk-embedded-C coreMQTT cleaned")
endif()

execute_process(COMMAND ${GIT_EXECUTABLE} -C ${AWS_IOT_DEVICE_SDK_EMBEDDED_C_SRC_DIR} submodule update --init)

# Delete untracked files in http_parser
if(EXISTS "${AWS_IOT_DEVICE_SDK_EMBEDDED_C_COREHTTP_HTTP_PARSER_SRC_DIR}/.git")
	message("cleaning aws-iot-device-sdk-embedded-C coreHTTP http_parser...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${AWS_IOT_DEVICE_SDK_EMBEDDED_C_COREHTTP_HTTP_PARSER_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${AWS_IOT_DEVICE_SDK_EMBEDDED_C_COREHTTP_HTTP_PARSER_SRC_DIR} reset --hard)
	message("aws-iot-device-sdk-embedded-C coreHTTP http_parser cleaned")
endif()

execute_process(COMMAND ${GIT_EXECUTABLE} -C ${AWS_IOT_DEVICE_SDK_EMBEDDED_C_COREHTTP_SRC_DIR} submodule update --init)


# Delete untracked files in tinyusb
if(EXISTS "${PICO_SDK_TINYUSB_SRC_DIR}/.git")
	message("cleaning pico-sdk tinyusb...")
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_SDK_TINYUSB_SRC_DIR} clean -fdx)
	execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_SDK_TINYUSB_SRC_DIR} reset --hard)
	message("pico-sdk tinyusb cleaned")
endif()

execute_process(COMMAND ${GIT_EXECUTABLE} -C ${PICO_SDK_SRC_DIR} submodule update --init)

# CMSIS-FreeRTOS patch
message("submodules CMSIS-FreeRTOS initialised")
 
file(GLOB CMSIS_FREERTOS_PATCHES
	"${RP2040_WIFI360_GATEWAY_C_PATCH_DIR}/01_cmsis_freertos_cmsis_os2.patch"
	)

foreach(CMSIS_FREERTOS_PATCHES IN LISTS CMSIS_FREERTOS_PATCHES)
	message("Running patch ${CMSIS_FREERTOS_PATCHES}")
	execute_process(
		COMMAND ${GIT_EXECUTABLE}  apply --ignore-whitespace ${CMSIS_FREERTOS_PATCHES}
		WORKING_DIRECTORY ${CMSIS_FREERTOS_SRC_DIR}
	)
endforeach()

# coreHTTP patch
message("submodules aws-iot-device-sdk-embedded-C coreHTTP initialised")

file(GLOB AWS_IOT_DEVICE_SDK_EMBEDDED_C_COREHTTP_PATCHES 
	"${RP2040_WIFI360_GATEWAY_C_PATCH_DIR}/02_aws_iot_device_sdk_embedded_c_corehttp_network_interface.patch"
	)

foreach(AWS_IOT_DEVICE_SDK_EMBEDDED_C_COREHTTP_PATCH IN LISTS AWS_IOT_DEVICE_SDK_EMBEDDED_C_COREHTTP_PATCHES)
	message("Running patch ${AWS_IOT_DEVICE_SDK_EMBEDDED_C_COREHTTP_PATCH}")
	execute_process(
		COMMAND ${GIT_EXECUTABLE} apply --ignore-whitespace ${AWS_IOT_DEVICE_SDK_EMBEDDED_C_COREHTTP_PATCH}
		WORKING_DIRECTORY ${AWS_IOT_DEVICE_SDK_EMBEDDED_C_COREHTTP_SRC_DIR}
	)
endforeach()

# CMSIS-Driver patch
message("submodules CMSIS-Driver initialised")

file(GLOB CMSIS_DRIVER_PATCHES 
	"${RP2040_WIFI360_GATEWAY_C_PATCH_DIR}/03_cmsis_driver_wifi_wizfi360_added_func_for_server.patch"
	)

foreach(CMSIS_DRIVER_PATCH IN LISTS CMSIS_DRIVER_PATCHES)
	message("Running patch ${CMSIS_DRIVER_PATCH}")
	execute_process(
		COMMAND ${GIT_EXECUTABLE} apply --ignore-whitespace ${CMSIS_DRIVER_PATCH}
		WORKING_DIRECTORY ${CMSIS_DRIVER_SRC_DIR}
	)
endforeach()
