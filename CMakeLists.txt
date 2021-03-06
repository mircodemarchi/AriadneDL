cmake_minimum_required(VERSION 3.16)
message(STATUS "CMake version ${CMAKE_VERSION}")

project(AriadneDL VERSION 0.1)

if(CMAKE_CXX_COMPILER_ID MATCHES "Intel")
    message(WARNING "Intel compiler not supported.")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    message(WARNING "Microsoft compiler not supported.")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0) # 10.2
        message(FATAL_ERROR "GCC version must be at least 9.0!")  # 10.2
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 11.0)
        message(FATAL_ERROR "Clang version must be at least 11.0!")
    endif()
else()
    message(WARNING "You are using an unsupported compiler! Compilation has
            only been tested with Clang and GCC.")
endif()

set(ANY_TARGET_WARN all extra pedantic sign-conversion cast-qual
    disabled-optimization init-self missing-include-dirs sign-promo
    switch-default undef redundant-decls)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_MACOSX_RPATH 1)
set(BUILD_SHARED_LIBS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")

foreach(WARN ${ANY_TARGET_WARN})
    add_compile_options(-W${WARN})
endforeach()

option(COVERAGE "Enable coverage reporting" OFF)
add_library(coverage_config INTERFACE)
if(COVERAGE)
    message(STATUS "Enabled coverage reporting")
    target_compile_options(coverage_config INTERFACE -O0 -g --coverage)
    if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
        target_link_options(coverage_config INTERFACE --coverage)
    else()
        target_link_libraries(coverage_config INTERFACE --coverage)
    endif()
endif()

enable_testing()
include(CTest)

# Options
set(ENABLE_MLPACK false CACHE BOOL "Enable MLPACK library. Warning: not supported.")
add_definitions(-DENABLE_MLPACK=${ENABLE_MLPACK})

# Find Ariadne. TODO: uncomment the following line.
# find_package(Ariadne REQUIRED)

# Find MLPACK and dependencies
find_package(Armadillo 8.400.0 QUIET)
find_package(MLPACK QUIET)

include_directories(${PROJECT_SOURCE_DIR}/source)
if(ENABLE_MLPACK)
    include_directories(${ARMADILLO_INCLUDE_DIRS})
    include_directories(${MLPACK_INCLUDE_DIRS})
endif()

add_subdirectory(source)
add_subdirectory(tests)

add_library(ariadnedl SHARED
    $<TARGET_OBJECTS:ariadnedl-estimators>
    $<TARGET_OBJECTS:ariadnedl-parser>
    $<TARGET_OBJECTS:ariadnedl-dnn>
)

target_link_libraries(ariadnedl dl)
if(ENABLE_MLPACK)
    target_link_libraries(${MLPACK_LIBRARIES} ${ARMADILLO_LIBRARIES})
endif()

if(COVERAGE)
    target_link_libraries(ariadnedl coverage_config)
endif()
