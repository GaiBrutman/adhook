cmake_minimum_required(VERSION 3.5)

project(
  adhook
  VERSION 0.1
  DESCRIPTION "A simple, embeddable hooking framework"
)

include(cmake/getArch.cmake)
enable_testing()

# Have cmake create a compile database
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(adhook STATIC
    src/hook.c
    src/patch.c
    "src/arch/${ARCH}/jump.c"
    "src/arch/${ARCH}/symsize.c"
)

target_include_directories(adhook
    PUBLIC
        include
    PRIVATE
        src
)

target_compile_options(adhook PRIVATE -Werror -Wall -Wextra)

add_subdirectory(examples)
add_subdirectory(tests)
