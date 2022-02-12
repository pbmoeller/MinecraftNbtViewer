# Header Path Prefix
set(AWESOMENBTVIEWER_HEADER_PATH "${AwesomeNbtViewer_SOURCE_DIR}/include/AwesomeNbtViewer")

### Includes
set(AWESOMENBTVIEWER_WIDGETS_INCLUDES
    "${AWESOMENBTVIEWER_HEADER_PATH}/widgets/main_window.hpp"
)

set(AWESOMENBTVIEWER_INCLUDES
    ${AWESOME_WIDGETS_INCLUDES}
)

### Sources
set(AWESOMENBTVIEWER_WIDGETS_SOURCES
    "widgets/main_window.cpp"
)

set(AWESOMENBTVIEWER_SOURCES
    ${AWESOMENBTVIEWER_WIDGETS_SOURCES}
    "main.cpp"
)


### Targets
add_executable(AwesomeNbtViewer 
    ${AWESOMENBTVIEWER_SOURCES}
)
target_link_libraries(AwesomeNbtViewer 
    PRIVATE AwesomeMC
    PRIVATE Qt6::Widgets
)
target_compile_options(AwesomeNbtViewer PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
          -std=c++20 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Woverloaded-virtual -Wpedantic>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4>
)