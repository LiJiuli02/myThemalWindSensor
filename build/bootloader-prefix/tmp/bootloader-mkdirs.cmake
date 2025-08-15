# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/mztyj/Documents/ESP-IDF_Project/Espressif/frameworks/esp-idf-v5.1.2/components/bootloader/subproject"
  "E:/ESP-IDF_Project/HotWireV1.0/build/bootloader"
  "E:/ESP-IDF_Project/HotWireV1.0/build/bootloader-prefix"
  "E:/ESP-IDF_Project/HotWireV1.0/build/bootloader-prefix/tmp"
  "E:/ESP-IDF_Project/HotWireV1.0/build/bootloader-prefix/src/bootloader-stamp"
  "E:/ESP-IDF_Project/HotWireV1.0/build/bootloader-prefix/src"
  "E:/ESP-IDF_Project/HotWireV1.0/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "E:/ESP-IDF_Project/HotWireV1.0/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "E:/ESP-IDF_Project/HotWireV1.0/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
