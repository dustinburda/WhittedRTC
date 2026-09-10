cmake_minimum_required(VERSION 3.24)
project(Whitted)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


############################################################################################################################################
## Source
############################################################################################################################################


add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)

file(GLOB SOURCE_FILES "src/*.cpp")
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
add_library(Whitted_Lib ${SOURCE_FILES})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Whitted_Lib)

# Set compiler flags
if (MSVC)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /W4 /WX)
    target_compile_options(Whitted_Lib PRIVATE /W4 /WX)
else()
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Werror -Wextra)
    target_compile_options(Whitted_Lib PRIVATE -Wall -Werror -Wextra)
endif()


############################################################################################################################################
## TEST
############################################################################################################################################

enable_testing()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB TEST_FILES "test/*.cpp")
add_executable(test_app ${TEST_FILES})

target_link_libraries(
        test_app
        PRIVATE
        GTest::gtest_main
        Whitted_Lib
)

include(GoogleTest)
gtest_discover_tests(test_app)