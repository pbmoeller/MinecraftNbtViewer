####################################################################################################
### Project
cmake_minimum_required(VERSION 3.16)

project(MinecraftNbtViewer
    VERSION     0.2.0
    LANGUAGES   CXX
)

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

### Git Version & Git Hash
execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  OUTPUT_VARIABLE MinecraftNbtViewer_GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
  ERROR_QUIET
)
execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  OUTPUT_VARIABLE MinecraftNbtViewer_GIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
  ERROR_QUIET
)

### Qt Stuff
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

####################################################################################################
### Dependencies

find_package(Qt6 REQUIRED COMPONENTS Widgets Core)
message(STATUS "Found Qt6: ${Qt6_VERSION} (${Qt6Widgets_VERSION})")

find_package(CppAnvil REQUIRED)
message(STATUS "Found CppAnvil: ${CppAnvil_VERSION}")

####################################################################################################
### Tools

if(MinecraftNbtViewer_CLANG_TIDY)
    find_program(CLANG_TIDY_EXECUTABLE
        NAMES clang-tidy
        REQUIRED
    )
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXECUTABLE} -p ${CMAKE_BINARY_DIR})
    set(CMAKE_C_CLANG_TIDY   ${CLANG_TIDY_EXECUTABLE} -p ${CMAKE_BINARY_DIR})
endif()

####################################################################################################
### Project Files

add_subdirectory("src")


