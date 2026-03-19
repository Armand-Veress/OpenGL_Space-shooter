cmake_minimum_required(VERSION 3.14) 
project(OpenGL_SpaceShooter VERSION 1.0.0 LANGUAGES C CXX)

# ==============================================================================
# [ SECTION 1: C++ STANDARD & OPTIONS ]
# ==============================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==============================================================================
# [ SECTION 2: AUTOMATIC DEPENDENCY FETCHING (GLFW) ]
# ==============================================================================
include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.8
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)

# ==============================================================================
# [ SECTION 3: HEADER SEARCH PATHS ]
# ==============================================================================
include_directories(
    include                  
    external/glad/include    # GLAD
    external/imgui/include   # ImGui
    external/glm             # GLM 
)

# ==============================================================================
# [ SECTION 4: SOURCE FILE AGGREGATION ]
# ==============================================================================
set(IMGUI_SOURCES
    external/imgui/src/imgui.cpp
    external/imgui/src/imgui_draw.cpp
    external/imgui/src/imgui_widgets.cpp
    external/imgui/src/imgui_tables.cpp
    external/imgui/src/imgui_impl_glfw.cpp
    external/imgui/src/imgui_impl_opengl3.cpp
)

file(GLOB PROJECT_SOURCES "src/*.cpp")

set(ALL_SOURCES 
    ${PROJECT_SOURCES}
    ${IMGUI_SOURCES}
    external/glad/src/glad.c
)

# ==============================================================================
# [ SECTION 5: EXECUTABLE & LINKING ]
# ==============================================================================
add_executable(${PROJECT_NAME} ${ALL_SOURCES})

# Link GLFW
target_link_libraries(${PROJECT_NAME} glfw)

# Link platform-specific OpenGL libraries
if(WIN32)
    target_link_libraries(${PROJECT_NAME} opengl32)
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} GL dl pthread)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)