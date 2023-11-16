add_library(output STATIC
output.cpp
output.h)

target_link_libraries(output
  PRIVATE terminal)

target_include_directories(output PRIVATE ${CMAKE_CURRENT_LIST_DIR})