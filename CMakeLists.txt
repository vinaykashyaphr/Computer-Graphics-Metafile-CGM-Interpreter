cmake_minimum_required(VERSION 3.20)


# SETUP

project(
    cgm_interpreter 
    VERSION 1.0
    DESCRIPTION "CGM interpretation and conversion to SVG"
    LANGUAGES CXX
    )


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)


# Executable

add_executable(
    cgm_interpreter
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/loader.cpp
    ${SRC_DIR}/reader.cpp
    ${SRC_DIR}/param_readers.cpp
    ${SRC_DIR}/dispatchers/dispatcher.cpp
    ${SRC_DIR}/dispatchers/delimiter.cpp
    ${SRC_DIR}/dispatchers/metafile_desc.cpp
    ${SRC_DIR}/utils/generic.cpp

)


target_include_directories(
    cgm_interpreter
    PRIVATE ${INCLUDE_DIR}
)


