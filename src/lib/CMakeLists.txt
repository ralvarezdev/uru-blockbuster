add_library(namespaces STATIC
namespaces.cpp)

target_include_directories(namespaces PRIVATE ${CMAKE_CURRENT_LIST_DIR})