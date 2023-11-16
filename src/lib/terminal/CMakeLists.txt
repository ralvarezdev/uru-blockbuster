add_library(terminal STATIC
ansiEsc.cpp
ansiEsc.h
input.cpp
input.h)

target_include_directories(terminal PRIVATE ${CMAKE_CURRENT_LIST_DIR})