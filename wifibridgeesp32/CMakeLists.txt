cmake_minimum_required(VERSION 3.13)

if(NOT DEFINED ENV{IDF_PATH})
    message(FATAL_ERROR "Error: need to define IDF_PATH")
endif()

set(esp_idf_dir "$ENV{IDF_PATH}")

set(aws_platform_dir "$ENV{IDF_PATH}/components/esp-aws-iot/aws-iot-device-sdk-embedded-C/platform")

include_directories(BEFORE include/mbedtls_config/)

include(${esp_idf_dir}/tools/cmake/project.cmake)

project(wifibridgeesp32)