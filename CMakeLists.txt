cmake_minimum_required(VERSION 3.12)

if(1) # PROJECT CONFIG BLOCK

    set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")

    project(otus_allocator VERSION 0.0.${PATCH_VERSION})

    # === Путь для модулей CMake ===
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    # === Путь до исходников ===
    set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
    # Корень покрываемых исходников — на уровень выше
    get_filename_component(COVERAGE_BASE_DIR "${SRC_DIR}" DIRECTORY)

    set(PROJECT_INCLUDE_DIR
        ${SRC_DIR}
    )

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

    include(compiler_standard)
    include(warnings_config)
    include(build_type_config)
    include(options_config)

endif()

if(1) # CORE LIBRARY BLOCK
    set(CORE_NAME ${PROJECT_NAME}_CORE)

    set(CORE_SRC ${CORE_NAME}_SRC)

    set(${CORE_SRC}
        ${SRC_DIR}/app.cpp
        ${SRC_DIR}/my_container.cpp
        ${SRC_DIR}/utils.cpp
    )

    add_library(${CORE_NAME} ${${CORE_SRC}})

    target_include_directories(${CORE_NAME} PUBLIC
        ${PROJECT_INCLUDE_DIR}
    )

    target_compile_options(${CORE_NAME} PRIVATE ${WARN_FLAGS})

    if(BUILD_COVERAGE)
        target_compile_options(${CORE_NAME} PRIVATE ${COVERAGE_COMPILE_FLAGS})
    endif()
endif()

if(1) # MAIN APPLICATION BLOCK
    add_executable(${PROJECT_NAME}
        ${SRC_DIR}/main.cpp
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE ${CORE_NAME})

    target_compile_options(${PROJECT_NAME} PRIVATE ${WARN_FLAGS})

    if (BUILD_COVERAGE)
        if (CMAKE_VERSION VERSION_GREATER "3.13" OR
            CMAKE_VERSION VERSION_EQUAL "3.13")
            target_link_options(${PROJECT_NAME} PRIVATE ${COVERAGE_LINK_FLAGS})
        else()
            set_target_properties(${PROJECT_NAME} PROPERTIES
                LINK_FLAGS "${COVERAGE_LINK_FLAGS}"
            )
        endif()
    endif()
endif()

if(1) # ACTIVATE BUILD TESTS BLOCK
    if (BUILD_TESTING)
        enable_testing()
        add_subdirectory(tests)
    endif()
endif()

if(1) # CPACK BLOCK
    install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

    set(CPACK_GENERATOR DEB)

    set(CPACK_PACKAGE_NAME ${PROJECT_NAME})

    set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

    set(CPACK_PACKAGE_CONTACT "d.v.bogomaz@gmail.com")

    include(CPack)
endif()

include(summary_config)
#include(post_build)
