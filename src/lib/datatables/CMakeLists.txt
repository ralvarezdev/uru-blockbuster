add_library(datatables STATIC
output.cpp
output.h)

target_link_libraries(datatables
  PRIVATE terminal)

target_include_directories(datatables PRIVATE ${CMAKE_CURRENT_LIST_DIR})