cmake_minimum_required(VERSION 3.16)

set(COMPONENT_ADD_INCLUDEDIRS
       "$ENV{IDF_PATH}/components/bt/include/esp32/include"
)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(protoBLE)