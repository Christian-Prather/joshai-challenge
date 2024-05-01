cmake_minimum_required(VERSION 3.8)
project(josh-ai-interface)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/connection_manager.cpp
    src/data_store.cpp
    src/light.cpp
    src/light_interface.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third-party

)



# Tests
enable_testing()
# add_subdirectory(tests)

install (TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})