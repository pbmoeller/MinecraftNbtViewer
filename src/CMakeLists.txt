### Includes
set(ANV_CORE_HEADER
    "core/awesome_nbt_viewer_app.hpp"
)

set(ANV_MODELS_HEADER
    "models/nbt_data_treemodel.hpp"
    "models/treeitems/nbt_treeitem_base.hpp"
    "models/treeitems/nbt_treeitem_folder.hpp"
    "models/treeitems/nbt_treeitem_nbtfile.hpp"
    "models/treeitems/nbt_treeitem_nbttag.hpp"
    "models/treeitems/nbt_treeitem_regionfile.hpp"
    "models/treeitems/nbt_treeitem_regionchunk.hpp"
    "models/treeitems/treeitem_util.hpp"
)

set(ANV_UTIL_HEADER
    "util/minecraft_util.hpp"
    "util/tag_mime_data.hpp"
)

set(ANV_WIDGETS_HEADER
    "widgets/main_window.hpp"
    "widgets/rename_tag_dialog.hpp"
    "widgets/new_tag_dialog.hpp"
    "widgets/edit_value_dialog.hpp"
)

set(ANV_HEADER
    ${ANV_CORE_HEADER}
    ${ANV_MODELS_HEADER}
    ${ANV_UTIL_HEADER}
    ${ANV_WIDGETS_HEADER}
)

### Sources
set(ANV_CORE_SOURCES
    "core/awesome_nbt_viewer_app.cpp"
)

set(ANV_MODELS_SOURCES
    "models/nbt_data_treemodel.cpp"
    "models/treeitems/nbt_treeitem_base.cpp"
    "models/treeitems/nbt_treeitem_folder.cpp"
    "models/treeitems/nbt_treeitem_nbtfile.cpp"
    "models/treeitems/nbt_treeitem_nbttag.cpp"
    "models/treeitems/nbt_treeitem_regionfile.cpp"
    "models/treeitems/nbt_treeitem_regionchunk.cpp"
    "models/treeitems/treeitem_util.cpp"
)

set(ANV_UTIL_SOURCES
    "util/minecraft_util.cpp"
    "util/tag_mime_data.cpp"
)

set(ANV_WIDGETS_SOURCES
    "widgets/main_window.cpp"
    "widgets/rename_tag_dialog.cpp"
    "widgets/new_tag_dialog.cpp"
    "widgets/edit_value_dialog.cpp"
)

set(ANV_SOURCES
    ${ANV_CORE_SOURCES}
    ${ANV_MODELS_SOURCES}
    ${ANV_UTIL_SOURCES}
    ${ANV_WIDGETS_SOURCES}
    "main.cpp"
)

### Ui
set(ANV_WIDGETS_UI
    "widgets/main_window.ui"
    "widgets/rename_tag_dialog.ui"
    "widgets/new_tag_dialog.ui"
    "widgets/edit_value_dialog.ui"
)

set(ANV_UI
    ${ANV_WIDGETS_UI}
)

### Resources
set(ANV_RESOURCES
    "${AwesomeNbtViewer_SOURCE_DIR}/resources/resources.qrc"
)

### Target
add_executable(AwesomeNbtViewer 
    ${ANV_HEADER}
    ${ANV_SOURCES}
    ${ANV_UI}
    ${ANV_RESOURCES}
)

target_include_directories(AwesomeNbtViewer
    PUBLIC $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
           $<BUILD_INTERFACE:${AwesomeNbtViewer_SOURCE_DIR}/src>
           $<INSTALL_INTERFACE:${AwesomeNbtViewer_INSTALL_INCLUDE_DIR}>
)

target_link_libraries(AwesomeNbtViewer 
    PRIVATE AwesomeMC
    PRIVATE Qt6::Widgets Qt6::Core
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