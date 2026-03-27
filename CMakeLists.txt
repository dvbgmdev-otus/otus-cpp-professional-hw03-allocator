cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")

project(otus_allocator VERSION 0.0.${PATCH_VERSION})

add_library(${PROJECT_NAME}_lib
    src/my_container.cpp
)

set_target_properties(${PROJECT_NAME}_lib PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_include_directories(${PROJECT_NAME}_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_link_libraries(${PROJECT_NAME}
    ${PROJECT_NAME}_lib
)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
    target_compile_options(${PROJECT_NAME}_lib PRIVATE /W4)
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
    )
    target_compile_options(${PROJECT_NAME}_lib PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
    )
endif()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})

set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

set(CPACK_PACKAGE_CONTACT "d.v.bogomaz@gmail.com")

enable_testing()

add_test(
    NAME self_check
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tests/self_check.sh $<TARGET_FILE:${PROJECT_NAME}>
)

# -----------------------------
# GoogleTest (FetchContent)
# -----------------------------
include(FetchContent)

set(BUILD_GMOCK OFF)
set(INSTALL_GTEST OFF)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googletest)

# -----------------------------
# GoogleTest target
# -----------------------------
add_executable(${PROJECT_NAME}_gtest
    tests/my_allocator_test.cpp
    tests/my_container_test.cpp
)

target_link_libraries(${PROJECT_NAME}_gtest
    ${PROJECT_NAME}_lib
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(${PROJECT_NAME}_gtest)

include(CPack)