cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(Blockbuster LANGUAGES CXX)

add_executable(${PROJECT_NAME} ./src/main.cpp)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/lib/clients)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/lib/data)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/lib/datatables)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/lib/movies)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/lib/terminal)

target_include_directories(${PROJECT_NAME}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/clients
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/data
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/datatables
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/movies
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/terminal)

target_link_libraries(${PROJECT_NAME}
  PRIVATE clients
  PRIVATE data
  PRIVATE datatables
  PRIVATE movies
  PRIVATE terminal
  PRIVATE stdc++fs)