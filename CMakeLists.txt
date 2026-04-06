cmake_minimum_required(VERSION 3.16)

project(cpp20test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Path for Google Test
set(GTEST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/googletest")

# Проверяем, есть ли уже собранные библиотеки
if(NOT EXISTS "${GTEST_LIBRARY_DIRS}/libgtest.a" AND
   NOT EXISTS "${GTEST_LIBRARY_DIRS}/gtest.lib")
    message(WARNING "Google Test libraries not found. Building them...")

    # Собираем Google Test автоматически
    add_subdirectory(${GTEST_ROOT} ${CMAKE_CURRENT_BINARY_DIR}/gtest_build)
else()
    # Используем готовые библиотеки
    include_directories(${GTEST_INCLUDE_DIRS})
    link_directories(${GTEST_LIBRARY_DIRS})
endif()

# include files from google
set(GTEST_INCLUDE_DIRS "${GTEST_ROOT}/googletest/include")
# libs dir from google
set(GTEST_LIBRARY_DIRS "${GTEST_ROOT}/build/lib")

# add include files from google to our project
include_directories(${GTEST_INCLUDE_DIRS})

# add path for google in our project
link_directories(${GTEST_LIBRARY_DIRS})

add_executable(cpp20test main.cpp
    array.h)

# define for global using GoogleTest
target_compile_definitions(cpp20test PUBLIC GOOGLE_TEST_ON)

# Подключаем библиотеки
target_link_libraries(cpp20test
    gtest_main
    gtest
    ${CMAKE_THREAD_LIBS_INIT}  # для Windows обычно не требуется
)

include(GoogleTest)
gtest_discover_tests(cpp20test)
