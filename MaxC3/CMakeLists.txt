cmake_minimum_required(VERSION 3.18)
set(TARGET MaxC3)
project(${TARGET})

include(FetchContent)
FetchContent_Declare(
    minhook
    GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(minhook)

add_library(${TARGET} SHARED "dllmain.cpp")
target_include_directories(${TARGET} PRIVATE .)
target_link_libraries(${TARGET} PUBLIC UE4SS minhook)