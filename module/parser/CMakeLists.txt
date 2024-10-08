# Copyright (c) 2024 SiMAS
# All rights reserved

set(project_name simas_module_parser)

set(sources_dir ${CMAKE_CURRENT_LIST_DIR}/src)
set(headers_dir ${CMAKE_CURRENT_LIST_DIR}/include)

set(sources
    ${sources_dir}/parser.cpp
    ${sources_dir}/parser_exception.cpp
)
set(headers
    ${headers_dir}/parser.hpp
    ${headers_dir}/parser_exception.hpp
)

add_library(${project_name} STATIC ${sources} ${headers})

set_target_properties(${project_name} PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 17
)

target_include_directories(${project_name} PUBLIC ${simas_dir})

target_link_libraries(${project_name} LINK_PUBLIC
    simas_module_instruction
    simas_module_decoder
)