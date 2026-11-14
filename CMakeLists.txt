cmake_minimum_required(VERSION 3.20)

project(terminalinput LANGUAGES C CXX VERSION 0.0.0)

include(FetchContent)

FetchContent_Declare(
    coututils
    GIT_REPOSITORY https://github.com/Werfer02/coututils.git
    GIT_TAG main
)
FetchContent_MakeAvailable(coututils)
message(STATUS "coututils in: ${coututils_SOURCE_DIR}")

add_library(tilib src/inputManager.cpp)

target_include_directories(tilib PUBLIC "${coututils_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/include/tilib")

option(tilib_BUILD_DEMO "build demo.cpp" OFF)
if(tilib_BUILD_DEMO)
    FetchContent_Declare(
        terminalrenderer
        GIT_REPOSITORY https://github.com/Werfer02/terminalrenderer.git
        GIT_TAG master
    )
    FetchContent_MakeAvailable(terminalrenderer)

    add_executable(demo demo/demo.cpp)
    target_link_libraries(demo PRIVATE trlib tilib)
endif()
