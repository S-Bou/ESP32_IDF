# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/sergiobou/esp/esp-idf/components/bootloader/subproject"
  "/Users/sergiobou/esp/ESP32_IDF/OledTemp/build/bootloader"
  "/Users/sergiobou/esp/ESP32_IDF/OledTemp/build/bootloader-prefix"
  "/Users/sergiobou/esp/ESP32_IDF/OledTemp/build/bootloader-prefix/tmp"
  "/Users/sergiobou/esp/ESP32_IDF/OledTemp/build/bootloader-prefix/src/bootloader-stamp"
  "/Users/sergiobou/esp/ESP32_IDF/OledTemp/build/bootloader-prefix/src"
  "/Users/sergiobou/esp/ESP32_IDF/OledTemp/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/sergiobou/esp/ESP32_IDF/OledTemp/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
