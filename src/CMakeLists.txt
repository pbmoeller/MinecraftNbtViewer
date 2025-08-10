####################################################################################################
### Files

### Includes
set(MinecraftNbtViewer_HEADER
    "version.hpp"

    # Core
    "core/minecraft_nbt_viewer_app.hpp"

    # Models
    "models/nbt_data_treemodel.hpp"
    "models/nbt_tree_search_helper.hpp"
    "models/treeitems/nbt_treeitem_base.hpp"
    "models/treeitems/nbt_treeitem_folder.hpp"
    "models/treeitems/nbt_treeitem_nbtfile.hpp"
    "models/treeitems/nbt_treeitem_nbttag.hpp"
    "models/treeitems/nbt_treeitem_regionfile.hpp"
    "models/treeitems/nbt_treeitem_regionchunk.hpp"
    "models/treeitems/treeitem_util.hpp"

    # Utils
    "util/validators/float_validator.hpp"
    "util/validators/int8_validator.hpp"
    "util/validators/int16_validator.hpp"
    "util/validators/int32_validator.hpp"
    "util/validators/int64_validator.hpp"
    "util/minecraft_util.hpp"
    "util/tag_mime_data.hpp"
    "util/iconprovider.hpp"

    # Widgets
    "widgets/about_dialog.hpp"
    "widgets/find_dialog.hpp"
    "widgets/main_window.hpp"
    "widgets/new_tag_dialog.hpp"
    "widgets/edit_dialog.hpp"
    "widgets/compression_dialog.hpp"
)

### Sources
set(MinecraftNbtViewer_SOURCES
    "main.cpp"
    "${PROJECT_BINARY_DIR}/src/version.cpp"

    # Core 
    "core/minecraft_nbt_viewer_app.cpp"

    # Models
    "models/nbt_data_treemodel.cpp"
    "models/nbt_tree_search_helper.cpp"
    "models/treeitems/nbt_treeitem_base.cpp"
    "models/treeitems/nbt_treeitem_folder.cpp"
    "models/treeitems/nbt_treeitem_nbtfile.cpp"
    "models/treeitems/nbt_treeitem_nbttag.cpp"
    "models/treeitems/nbt_treeitem_regionfile.cpp"
    "models/treeitems/nbt_treeitem_regionchunk.cpp"
    "models/treeitems/treeitem_util.cpp"

    # Utils
    "util/validators/float_validator.cpp"
    "util/validators/int8_validator.cpp"
    "util/validators/int16_validator.cpp"
    "util/validators/int32_validator.cpp"
    "util/validators/int64_validator.cpp"
    "util/minecraft_util.cpp"
    "util/tag_mime_data.cpp"
    "util/iconprovider.cpp"

    # Widgets
    "widgets/about_dialog.cpp"
    "widgets/find_dialog.cpp"
    "widgets/main_window.cpp"
    "widgets/new_tag_dialog.cpp"
    "widgets/edit_dialog.cpp"
    "widgets/compression_dialog.cpp"
)

### Ui
set(MinecraftNbtViewer_UI
    "widgets/main_window.ui"
    "widgets/new_tag_dialog.ui"
    "widgets/compression_dialog.ui"
)

### Resources
set(MinecraftNbtViewer_RESOURCES
    "${MinecraftNbtViewer_SOURCE_DIR}/resources/resources.qrc"
)

set(MinecraftNbtViewer_APP_NAME "MinecraftNbtViewer")

configure_file(
    "version.cpp.in"
    "version.cpp"
    @ONLY
)

####################################################################################################
### Library

add_executable(MinecraftNbtViewerApp
    ${MinecraftNbtViewer_HEADER}
    ${MinecraftNbtViewer_SOURCES}
    ${MinecraftNbtViewer_UI}
    ${MinecraftNbtViewer_RESOURCES}
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
    PRIVATE 
        CppAnvil::CppAnvil
        Qt6::Widgets
        Qt6::Core
)

set_target_properties(MinecraftNbtViewerApp
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
)

target_compile_options(MinecraftNbtViewerApp 
    PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
          -std=c++20 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Woverloaded-virtual -Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:
          /W4>
)

# MSVC:
# We do not want to see the follwoing warning. This is caused by CMake setting certain flags to default values.
# "cl : commandline  warning D9025: overriding '/W3' with '/W4'
if (MSVC)
    foreach(flag_var
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        string(REPLACE "/W3" "/W4" ${flag_var} "${${flag_var}}")
    endforeach()
endif()

####################################################################################################
### Install

set(MinecraftNbtViewer_INSTALL_BINARY_DIR "bin")
set(MinecraftNbtViewer_INSTALL_INCLUDE_DIR "include")
set(MinecraftNbtViewer_INSTALL_LIBRARY_DIR "lib/${CMAKE_BUILD_TYPE}")
set(MinecraftNbtViewer_INSTALL_CMAKE_CONFIG_DIR "share/MinecraftNbtViewer/cmake")
set(MinecraftNbtViewer_INSTALL_ADDITIONAL_FILES_DIR "share/MinecraftNbtViewer")

install(
    TARGETS 
        MinecraftNbtViewerApp
    EXPORT
        MinecraftNbtViewerApp
    DESTINATION
        "${MinecraftNbtViewer_INSTALL_BINARY_DIR}"
)

install(
    FILES
        "${PROJECT_SOURCE_DIR}/LICENSE"
        "${PROJECT_SOURCE_DIR}/readme.md"
    DESTINATION
        ${MinecraftNbtViewer_INSTALL_ADDITIONAL_FILES_DIR}
)
