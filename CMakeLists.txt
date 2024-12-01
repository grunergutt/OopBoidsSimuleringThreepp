cmake_minimum_required(VERSION 3.19)
project(BoidSimulation)

set(CMAKE_CXX_STANDARD 20)

# FetchContent for dependencies
include(FetchContent)

# FetchContent: threepp
set(THREEPP_BUILD_TESTS OFF)
set(THREEPP_BUILD_EXAMPLES OFF)

FetchContent_Declare(
        threepp
        GIT_REPOSITORY https://github.com/markaren/threepp.git
        GIT_TAG 49c14a7445e3870ad2307f626076a05b0c915d6b
)
FetchContent_MakeAvailable(threepp)

# FetchContent: ImGui
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.89.2
)
FetchContent_MakeAvailable(imgui)

# Define paths for ImGui backends
set(IMGUI_BACKEND_PATH ${imgui_SOURCE_DIR}/backends)

# Create the ImGui library target
add_library(imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${IMGUI_BACKEND_PATH}/imgui_impl_glfw.cpp
        ${IMGUI_BACKEND_PATH}/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
        ${imgui_SOURCE_DIR}
        ${IMGUI_BACKEND_PATH}
)

# Link ImGui with GLFW and threepp
target_link_libraries(imgui PUBLIC threepp::threepp glfw)


# Collect source files for the main project
file(GLOB SRC_FILES src/*.cpp)

# Add the main executable target
add_executable(BoidSimulation
        main.cpp
        ${SRC_FILES}
)

# Link dependencies to the main executable
target_link_libraries(BoidSimulation PRIVATE threepp::threepp imgui)

# Include directories for the project
target_include_directories(BoidSimulation PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(BoidSimulation PRIVATE ${threepp_SOURCE_DIR}/../include)

# Emscripten-specific settings (optional, if targeting web builds)
if (DEFINED EMSCRIPTEN)
    set(LINK_FLAGS " --bind -sUSE_GLFW=3 -sGL_DEBUG=1 -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 \
    -sFULL_ES3 -sASSERTIONS -sALLOW_MEMORY_GROWTH -sWASM=1")

    set_target_properties("BoidSimulation"
            PROPERTIES SUFFIX ".html"
            LINK_FLAGS "${LINK_FLAGS}"
    )
endif ()
