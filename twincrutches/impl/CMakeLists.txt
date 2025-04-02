cmake_minimum_required(VERSION 3.16)
set(EXTRA_COMPONENT_DIRS "../../components" "../../esp-idf-lib/components")
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

add_compile_definitions(EXTERNAL_EVENT_NO_HEAP_DATA)

if(TWIN_CRUTCH_RIGHT)
    add_compile_definitions(TWIN_CRUTCH_RIGHT)
elseif(TWIN_CRUTCH_LEFT)
    add_compile_definitions(TWIN_CRUTCH_LEFT)
endif()

project(twincrutches)
