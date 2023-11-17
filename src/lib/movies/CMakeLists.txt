add_library(movies STATIC
moviesOp.cpp
moviesOp.h)

target_link_libraries(movies
  PRIVATE terminal
  PRIVATE datatables)

target_include_directories(movies PRIVATE ${CMAKE_CURRENT_LIST_DIR})