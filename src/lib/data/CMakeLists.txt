add_library(data STATIC
dataOp.cpp
dataOp.h)

target_link_libraries(data
  PRIVATE datatables
  PRIVATE clients
  PRIVATE movies
  PRIVATE terminal)

target_include_directories(data PRIVATE ${CMAKE_CURRENT_LIST_DIR})