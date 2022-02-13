# Header Path Prefix
set(AWESOMENBTVIEWER_HEADER_PATH "${AwesomeNbtViewer_SOURCE_DIR}/include/AwesomeNbtViewer")

### Includes
set(AWESOMENBTVIEWER_WIDGETS_INCLUDES
    "${AWESOMENBTVIEWER_HEADER_PATH}/widgets/main_window.hpp"
)

set(AWESOMENBTVIEWER_INCLUDES
    ${AWESOME_WIDGETS_INCLUDES}
    "${AWESOMENBTVIEWER_HEADER_PATH}/awesome_nbt_viewer_app.hpp"
)

### Sources
set(AWESOMENBTVIEWER_WIDGETS_SOURCES
    "widgets/main_window.cpp"
)

set(AWESOMENBTVIEWER_SOURCES
    ${AWESOMENBTVIEWER_WIDGETS_SOURCES}
    "main.cpp"
    "awesome_nbt_viewer_app.cpp"
)

### Targets
add_executable(AwesomeNbtViewer 
    ${AWESOMENBTVIEWER_SOURCES}
    ${AWESOMENBTVIEWER_INCLUDES}
)

target_include_directories(AwesomeNbtViewer
    PUBLIC $<BUILD_INTERFACE:${AwesomeNbtViewer_SOURCE_DIR}/include/>
           $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
           $<INSTALL_INTERFACE:${AwesomeNbtViewer_INSTALL_INCLUDE_DIR}>
)
target_link_libraries(AwesomeNbtViewer 
    PRIVATE AwesomeMC
    PRIVATE Qt6::Widgets
)
set_target_properties(AwesomeNbtViewer PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
target_compile_options(AwesomeNbtViewer PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
          -std=c++20 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Woverloaded-virtual -Wpedantic>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4>
)

# MSVC:
# We do not want to see the follwoing warning. This is caused by CMake setting certain flags to default values.
# "cl : commandline  warning D9025: overriding '/W3' with '/W4'
if (MSVC)
    foreach (flag_var
             CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
             CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        string(REPLACE "/W3" "/W4" ${flag_var} "${${flag_var}}")
    endforeach()
endif()


### Install
install(TARGETS AwesomeNbtViewer EXPORT AwesomeNbtViewer DESTINATION "${AwesomeNbtViewer_INSTALL_BINARY_DIR}")