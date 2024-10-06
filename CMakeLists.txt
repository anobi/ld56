cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 23)
project(Game)
include(FetchContent)

add_executable(${PROJECT_NAME} src/main.cpp)

##################
# External deps  #
##################
FetchContent_Declare(
  glfw
  GIT_REPOSITORY  https://github.com/glfw/glfw
  GIT_TAG         3.4

)
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG        1.0.1
)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG        11.0.2
)
FetchContent_MakeAvailable(
  glfw
  glm
  fmt
)
target_link_libraries(${PROJECT_NAME}
  glfw
  glm
  fmt
)

find_package(Freetype REQUIRED)

##################
# Internal stuff #
##################
target_sources(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/renderable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/text_rendering.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ant.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/goop.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/baddie.cpp
)
target_link_libraries(${PROJECT_NAME}
  ${FREETYPE_LIBRARIES}
)
target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE
  ${FREETYPE_INCLUDE_DIRS}
)


##################
# Output         #
##################
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_BINARY_DIR}/shaders/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders/
)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_BINARY_DIR}/assets/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets/
)