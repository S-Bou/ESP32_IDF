set(aws_platform_dir "$ENV{IDF_PATH}/components/esp-aws-iot/aws-iot-device-sdk-embedded-C/platform")

message("$ENV{IDF_PATH}/components/esp-aws-iot/aws-iot-device-sdk-embedded-C/platform")
idf_component_register(SRC_DIRS "."
"./src/console"
"./src/network"
"./src/system"
"./src/uart"
"./src/wirelessCommunication"
"./src/dummy"
INCLUDE_DIRS "./inc"                              
                    REQUIRES
                    freertos
                    console
                    nvs_flash
                    spi_flash
                    esp-aws-iot
                    esp_https_server
                    esp_common
                    esp_event
                    json
                    bt)


