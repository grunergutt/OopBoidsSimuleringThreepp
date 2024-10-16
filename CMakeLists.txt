cmake_minimum_required(VERSION 3.19)
project(threepp_fetchcontent_demo)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)


set(THREEPP_BUILD_TESTS OFF)
set(THREEPP_BUILD_EXAMPLES OFF)
FetchContent_Declare(
        threepp
        GIT_REPOSITORY https://github.com/markaren/threepp.git
        GIT_TAG 49c14a7445e3870ad2307f626076a05b0c915d6b
)
FetchContent_MakeAvailable(threepp)


FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.89.2
)
FetchContent_MakeAvailable(imgui)


set(IMGUI_BACKEND_PATH ${imgui_SOURCE_DIR}/backends)

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


target_link_libraries(imgui PUBLIC threepp::threepp)

add_executable(threepp_BOIDS src/main.cpp)

target_link_libraries(threepp_BOIDS PRIVATE threepp::threepp imgui)
target_link_libraries(imgui PUBLIC threepp::threepp glfw)

target_include_directories(threepp_BOIDS PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(threepp_BOIDS PRIVATE ${threepp_SOURCE_DIR}/../include)

######### Emscripten #######
if (DEFINED EMSCRIPTEN)

    set(LINK_FLAGS " --bind -sUSE_GLFW=3 -sGL_DEBUG=1 -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 \
    -sFULL_ES3 -sASSERTIONS -sALLOW_MEMORY_GROWTH -sWASM=1")

    set_target_properties("threepp_BOIDS"
            PROPERTIES SUFFIX ".html"
            LINK_FLAGS "${LINK_FLAGS}"
    )

endif ()
###########################