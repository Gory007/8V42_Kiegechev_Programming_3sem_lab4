cmake_minimum_required(VERSION 3.10.0)
project(lab4 VERSION 0.1.0 LANGUAGES C CXX)

set_property(DIRECTORY . PROPERTY CMAKE_CONFIGURE_DEPENDS main.cpp)

if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-list --all --count

        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_COUNT
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

else()
    set(GIT_COMMIT_COUNT "0")
endif()

add_definitions(-DVERSION_PROJECT=${GIT_COMMIT_COUNT})

add_executable(
    lab4 main.cpp
    allocator.h
    my_forward_list.h)

set_source_files_properties(
    allocator.h
    my_forward_list.h
    PROPERTIES HEADER_FILE_ONLY TRUE
)

target_include_directories(lab4 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

set(CPACK_PACKAGE_NAME "lab4")

set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})

if(WIN32)
    set(CPACK_GENERATOR "ZIP")
endif()

if(UNIX AND NOT APPLE)
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Kiegechev Pavel <kiegechev09@gmail.com>")
endif()

install(TARGETS lab4 DESTINATION bin)

include(CPack)