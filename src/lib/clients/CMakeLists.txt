add_library(clients STATIC
clientsOp.cpp
clientsOp.h)

target_link_libraries(clients
  PRIVATE terminal
  PRIVATE datatables)

target_include_directories(clients PRIVATE ${CMAKE_CURRENT_LIST_DIR})