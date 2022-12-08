# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/Programas/Espressif/Esp-idf/frameworks/esp-idf-v4.4.3/components/bootloader/subproject"
  "D:/Projects/ESP32_IDF/ESP32-OLED/build/bootloader"
  "D:/Projects/ESP32_IDF/ESP32-OLED/build/bootloader-prefix"
  "D:/Projects/ESP32_IDF/ESP32-OLED/build/bootloader-prefix/tmp"
  "D:/Projects/ESP32_IDF/ESP32-OLED/build/bootloader-prefix/src/bootloader-stamp"
  "D:/Projects/ESP32_IDF/ESP32-OLED/build/bootloader-prefix/src"
  "D:/Projects/ESP32_IDF/ESP32-OLED/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Projects/ESP32_IDF/ESP32-OLED/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
