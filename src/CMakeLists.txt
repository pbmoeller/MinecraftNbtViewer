
configure_file(
    "version.cpp.in"
    "version.cpp"
    @ONLY
)

### Includes
set(ANV_HEADER
    "core/minecraft_nbt_viewer_app.hpp"

    "models/nbt_data_treemodel.hpp"
    "models/treeitems/nbt_treeitem_base.hpp"
    "models/treeitems/nbt_treeitem_folder.hpp"
    "models/treeitems/nbt_treeitem_nbtfile.hpp"
    "models/treeitems/nbt_treeitem_nbttag.hpp"
    "models/treeitems/nbt_treeitem_regionfile.hpp"
    "models/treeitems/nbt_treeitem_regionchunk.hpp"
    "models/treeitems/treeitem_util.hpp"

    "util/validators/float_validator.hpp"
    "util/validators/int8_validator.hpp"
    "util/validators/int16_validator.hpp"
    "util/validators/int32_validator.hpp"
    "util/validators/int64_validator.hpp"
    "util/minecraft_util.hpp"
    "util/tag_mime_data.hpp"
    "util/iconprovider.hpp"

    "widgets/main_window.hpp"
    "widgets/new_tag_dialog.hpp"
    "widgets/edit_dialog.hpp"
    "widgets/compression_dialog.hpp"

    "version.hpp"
)

### Sources
set(ANV_SOURCES
    "core/minecraft_nbt_viewer_app.cpp"

    "models/nbt_data_treemodel.cpp"
    "models/treeitems/nbt_treeitem_base.cpp"
    "models/treeitems/nbt_treeitem_folder.cpp"
    "models/treeitems/nbt_treeitem_nbtfile.cpp"
    "models/treeitems/nbt_treeitem_nbttag.cpp"
    "models/treeitems/nbt_treeitem_regionfile.cpp"
    "models/treeitems/nbt_treeitem_regionchunk.cpp"
    "models/treeitems/treeitem_util.cpp"

    "util/validators/float_validator.cpp"
    "util/validators/int8_validator.cpp"
    "util/validators/int16_validator.cpp"
    "util/validators/int32_validator.cpp"
    "util/validators/int64_validator.cpp"
    "util/minecraft_util.cpp"
    "util/tag_mime_data.cpp"
    "util/iconprovider.cpp"

    "widgets/main_window.cpp"
    "widgets/new_tag_dialog.cpp"
    "widgets/edit_dialog.cpp"
    "widgets/compression_dialog.cpp"

    "main.cpp"
    "${PROJECT_BINARY_DIR}/src/version.cpp"
)

### Ui
set(ANV_UI
    "widgets/main_window.ui"
    "widgets/new_tag_dialog.ui"
    "widgets/compression_dialog.ui"
)

### Resources
set(ANV_RESOURCES
    "${MinecraftNbtViewer_SOURCE_DIR}/resources/resources.qrc"
)

### Target
add_executable(MinecraftNbtViewerApp
    ${ANV_HEADER}
    ${ANV_SOURCES}
    ${ANV_UI}
    ${ANV_RESOURCES}
)

target_include_directories(MinecraftNbtViewerApp
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC 
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
        $<BUILD_INTERFACE:${MinecraftNbtViewer_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:${MinecraftNbtViewer_INSTALL_INCLUDE_DIR}>
)

target_link_libraries(MinecraftNbtViewerApp 
    PRIVATE cppAnvil::cppAnvil
    PRIVATE Qt6::Widgets Qt6::Core
)

set_target_properties(MinecraftNbtViewerApp PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

target_compile_options(MinecraftNbtViewerApp PRIVATE
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
install(
    TARGETS 
        MinecraftNbtViewerApp
    EXPORT
        MinecraftNbtViewerApp
    DESTINATION
        "${MinecraftNbtViewer_INSTALL_BINARY_DIR}"
)
