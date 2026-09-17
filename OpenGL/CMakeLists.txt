cmake_minimum_required(VERSION 3.10)
project(Triangles
  VERSION 1.0.0
  LANGUAGES C
)

option(GLFW_DLL "Dynamic build of glfw" ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/lib)

set(CMAKE_EXECUTABLE_SUFFIX "-${PROJECT_VERSION}")
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/OpenGL.c
)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/lib/glad)

target_link_libraries(${PROJECT_NAME} PRIVATE
  GLFW/glfw3
)
