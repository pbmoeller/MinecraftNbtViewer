[![MinecraftNbtViewer Version v0.1.0](https://img.shields.io/badge/MinecraftNbtViewer-v0.1.0-green)](https://github.com/pbmoeller/MinecraftNbtViewer)
[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://github.com/pbmoeller/MinecraftNbtViewer/blob/main/LICENSE)

# MinecraftNbtViewer

MinecraftNbtViewer is an open source graphical tool so view and edit Minecraft NBT and level data.
It is based on [cpp-anvil](https://github.com/pbmoeller/cpp-anvil).

## Features

- View, edit or create Minecraft NBT (Named Binary Tag) files
- Browse and modify region files and world data
- Tree-based visualization of NBT structures
- Support for various Minecraft data types
- Cross-platform (Windows, Linux)
- Modern C++20 codebase

## Screenshots

![MinecraftNbtViewer](/doc/minecraft_nbt_viewer.png)

## Getting Started

### Prerequisites

- CMake >= 3.16
- C++20 compatible compiler (MSVC, GCC, or Clang)
- Qt6 (tested with Qt 6.7.2)

### Building

Clone the repository:

```bash
git clone https://github.com/pbmoeller/MinecraftNbtViewer.git 
cd MinecraftNbtViewer
```

Configure and build with CMake and Ninja:

```bash
cmake -S . -B build cmake --build build
```

## Usage

- Open Minecraft world folders or NBT files
- Browse and edit data in a tree view
- Save changes back to disk

## License

This project is licensed under the BSD 3-Clause License. See the [LICENSE](LICENSE) file for details.

## Acknowledgments

- [cpp-anvil](https://github.com/pbmoeller/cpp-anvil) for Minecraft region/NBT parsing
- Qt for the GUI framework

## Contributing

Contributions are welcome! Please open issues or pull requests on GitHub.