cmake_minimum_required(VERSION 3.10)
project(project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)

# 1. Adăugăm calea către headerele ImGui
include_directories(
    external/glad/include 
    /usr/include/glm
    external/imgui/include   # <--- Nou
)

# 2. Definim sursele ImGui
set(IMGUI_SOURCES
    external/imgui/src/imgui.cpp
    external/imgui/src/imgui_draw.cpp
    external/imgui/src/imgui_widgets.cpp
    external/imgui/src/imgui_tables.cpp
    external/imgui/src/imgui_impl_glfw.cpp
    external/imgui/src/imgui_impl_opengl3.cpp
)

set(SOURCE_FILES 
    src/main.cpp 
    external/glad/src/glad.c
    ${IMGUI_SOURCES}         # <--- Adăugăm sursele ImGui aici
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} glfw dl pthread)