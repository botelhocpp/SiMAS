# Copyright (c) 2024 SiMAS
# All rights reserved

set(project_name simas_app)

set(sources_dir ${CMAKE_CURRENT_LIST_DIR}/src)

set(sources
    ${sources_dir}/main.cpp
)

set(simas_executable simas)

add_executable(${simas_executable} ${sources})

target_link_libraries(${simas_executable} LINK_PUBLIC
    simas_module_instruction
    simas_module_decoder
    simas_module_parser
    simas_utils_getopt
)