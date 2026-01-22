cmake_minimum_required(VERSION 3.16)

project(ptd
    VERSION 1.0
    LANGUAGES CXX
)

# ---- Options ---------------------------------------------------------------
option(BUILD_SHARED_LIBS "Build shared library instead of static" ON)

# ---- C++ standard ----------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Sources ---------------------------------------------------------------
file(GLOB_RECURSE PTD_SOURCES
    src/ptd/*.cpp
    src/hporro/*.cpp
)

file(GLOB_RECURSE PTD_HEADERS
    src/ptd/*.h
    src/hporro/*.h
    src/nanoflann/*.h
)

# ---- Library ---------------------------------------------------------------
add_library(ptd
    ${PTD_SOURCES}
    ${PTD_HEADERS}
)

# ---- Include directories ---------------------------------------------------
target_include_directories(ptd
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ptd
        ${CMAKE_CURRENT_SOURCE_DIR}/src/hporro
        ${CMAKE_CURRENT_SOURCE_DIR}/src/nanoflann
)

# ---- Compile options -------------------------------------------------------
target_compile_features(ptd PUBLIC cxx_std_17)

# ---- Compile options -------------------------------------------------------
target_compile_features(ptd PUBLIC cxx_std_17)

if (MSVC)
    target_compile_options(ptd PRIVATE /W4)
else()
    target_compile_options(ptd PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wno-unused-variable
    )
endif()
