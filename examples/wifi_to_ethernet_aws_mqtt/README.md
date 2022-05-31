# How to Test Connect to AWS IoT through MQTT Example



## Step 1: Prepare software

The following serial terminal program is required for Connect AWS IoT through MQTT example test, download and install from below link.

- [**Tera Term**][link-tera_term]



## Step 2: Prepare hardware

1. Connect W5x00-EVB-Pico to desktop or laptop using 5 pin micro USB cable.



## Step 3: Setup AWS IoT

In order to connect to AWS IoT through MQTT, the development environment must be configured to use AWS IoT.

This example was tested by configuring AWS IoT Core. Please refer to the 'Create AWS IoT resources' section of document below and configure accordingly.

- [**Create AWS IoT resources**][link-create_aws_iot_resources]

For more information on AWS IoT, refer to the document below.

- [**What is AWS IoT?**][link-what_is_aws_iot]



## Step 4: Setup Connect AWS IoT through MQTT Example

To test the Connect AWS IoT through MQTT example, minor settings shall be done in code.

1. Setup Wi-Fi configuration in 'socket_startup.c' in 'RP2040-WizFi360-Gateway-C/examples/aws_mqtt_demo/' directory.

Setup the Wi-Fi network information you use.

```cpp
/* Wi-Fi configuration */
#define SSID "ssid"
#define PASSWORD "password"
#define SECURITY_TYPE ARM_WIFI_SECURITY_WPA2
```

2. Setup AWS IoT configuration.

MQTT_DOMAIN should be setup as AWS IoT data endpoint, and MQTT_USERNAME and MQTT_PASSWORD do not need to be setup. Setup MQTT_CLIENT_ID same as thing created during AWS IoT Core setup.

AWS IoT configuration can be set in 'iot_demo.c' in 'RP2040-WizFi360-Gateway-C/examples/aws_mqtt_demo/' directory.

```cpp
/* AWS IoT */
#define MQTT_DOMAIN "account-specific-prefix-ats.iot.ap-northeast-2.amazonaws.com"
#define MQTT_PUB_TOPIC "$aws/things/example_rp2040_thing/shadow/update"
#define MQTT_SUB_TOPIC "$aws/things/example_rp2040_thing/shadow/update/accepted"
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL
#define MQTT_CLIENT_ID "example_rp2040_thing"
```

3. Setup device certificate and key.

You must enter the root certificate, client certificate and private key that were downloaded in Step 3.

Root certificate uses the RSA 2048 bit key, Amazon Root CA 1, and does not use the public key.

Device certificate and key can be set in 'mqtt_certificate.h' in 'RP2040-WizFi360-Gateway-C/examples/aws_mqtt_demo/' directory.

```cpp
uint8_t mqtt_root_ca[] =
"-----BEGIN CERTIFICATE---------\r\n"
" FILL IN YOUR ACCOUNT ROOT CA  \r\n"
"-----END CERTIFICATE-----------\r\n";

uint8_t mqtt_client_cert[] =
"-----BEGIN CERTIFICATE---------\r\n"
" FILL IN CLIENT CERTFICATE     \r\n"
"-----END CERTIFICATE-----------\r\n";

uint8_t mqtt_private_key[] =
"-----BEGIN RSA PRIVATE KEY-----\r\n"
" FILL IN PRVATE KEY            \r\n"
"-----END RSA PRIVATE KEY-------\r\n";
```



## Step 5: Build

1. After completing the configuration, click 'build' in the status bar at the bottom of Visual Studio Code or press the 'F7' button on the keyboard to build.

2. When the build is completed, 'wifi_to_ethernet_aws_mqtt.uf2' is generated in 'RP2040-WizFi360-Gateway-C/build/examples/wifi_to_ethernet_aws_mqtt/' directory.



## Step 6: Upload and Run

1. While pressing the BOOTSEL button of W5x00-EVB-Pico power on the board, the USB mass storage 'RPI-RP2' is automatically mounted.

![][link-raspberry_pi_pico_usb_mass_storage]

2. Drag and drop 'wifi_to_ethernet_aws_mqtt.uf2' onto the USB mass storage device 'RPI-RP2'.

3. Connect to the serial COM port of W5x00-EVB-Pico with Tera Term.

![][link-connect_to_serial_com_port]

4. Reset your board.

5. If the Connect AWS IoT through MQTT example works normally on W5x00-EVB-Pico, you can see the Ethernet and Wi-Fi connection log, connecting to the AWS IoT and publishing the message.
![][link-SSL_WIFI_MQTT_success]

![][link-see_wi-fi_connection_log_connecting_to_aws_iot_and_publishing_message]

![][link-subscribe_to_publish_topic_and_receive_publish_message_through_test_function]

6. If WizFi360-mini or WizFi360-EVB-Shield client(station) connected to PC is connected normally to RP2040-WizFi360-Gateway, you can see Wi-Fi connection logs on your terminal and you can operate readily WizFi360(client) by terminal tool. Refer to the below.

![][link-wifi360_to_gateway_connect_send_receive]


<!--
Link
-->

[link-tera_term]: https://osdn.net/projects/ttssh2/releases/
[link-create_aws_iot_resources]: https://docs.aws.amazon.com/iot/latest/developerguide/create-iot-resources.html
[link-what_is_aws_iot]: https://docs.aws.amazon.com/iot/latest/developerguide/what-is-aws-iot.html
[link-raspberry_pi_pico_usb_mass_storage]: https://github.com/Wiznet/RP2040-WizFi360-Gateway-C/blob/main/static/images/wifi_to_ethernet_aws_mqtt/raspberry_pi_pico_usb_mass_storage.png
[link-connect_to_serial_com_port]: https://github.com/Wiznet/RP2040-WizFi360-Gateway-C/blob/main/static/images/wifi_to_ethernet_aws_mqtt/connect_to_serial_com_port.png
[link-SSL_WIFI_MQTT_success]: https://github.com/Wiznet/RP2040-WizFi360-Gateway-C/blob/main/static/images/wifi_to_ethernet_aws_mqtt/SSL_WIFI_MQTT_success.png
[link-see_wi-fi_connection_log_connecting_to_aws_iot_and_publishing_message]: https://github.com/Wiznet/RP2040-WizFi360-Gateway-C/blob/main/static/images/wifi_to_ethernet_aws_mqtt/see_wi-fi_connection_log_connecting_to_aws_iot_and_publishing_message.png
[link-subscribe_to_publish_topic_and_receive_publish_message_through_test_function]: https://github.com/Wiznet/RP2040-WizFi360-Gateway-C/blob/main/static/images/wifi_to_ethernet_aws_mqtt/subscribe_to_publish_topic_and_receive_publish_message_through_test_function.png
[link-publish_message_through_test_function]:https://github.com/Wiznet/RP2040-WizFi360-Gateway-C/blob/main/static/images/wifi_to_ethernet_aws_mqtt/publish_message_through_test_function.png
[link-receive_message_about_subcribe_topic]:https://github.com/Wiznet/RP2040-WizFi360-Gateway-C/blob/main/static/images/wifi_to_ethernet_aws_mqtt/receive_message_about_subscribe_topic.png
[link-wifi360_to_gateway_connect_send_receive]:https://github.com/Wiznet/RP2040-WizFi360-Gateway-C/blob/main/static/images/wifi_to_ethernet_aws_mqtt/wifi360-to-Gateway-connect-send-receive.png
