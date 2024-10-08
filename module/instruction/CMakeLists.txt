# Copyright (c) 2024 SiMAS
# All rights reserved

set(project_name simas_module_instruction)

set(sources_dir ${CMAKE_CURRENT_LIST_DIR}/src)
set(headers_dir ${CMAKE_CURRENT_LIST_DIR}/include)

set(sources
    ${sources_dir}/instruction.cpp
)
set(headers
    ${headers_dir}/instruction.hpp
    ${headers_dir}/instructionlist.hpp
)

add_library(${project_name} STATIC ${sources} ${headers})

set_target_properties(${project_name} PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 17
)

target_include_directories(${project_name} PUBLIC ${simas_dir})

target_link_libraries(${project_name} LINK_PUBLIC
    simas_utils_strmanip
)