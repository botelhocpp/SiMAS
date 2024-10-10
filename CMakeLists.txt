# Copyright (c) 2024 SiMAS
# All rights reserved

cmake_minimum_required(VERSION ${CMAKE_VERSION})
project("Simple MIPS Assembler" C CXX)

set(simas_dir ${PROJECT_SOURCE_DIR})

add_subdirectory(${simas_dir}/app)
add_subdirectory(${simas_dir}/module)
add_subdirectory(${simas_dir}/utils)
