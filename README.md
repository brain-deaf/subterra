# Subterra

Subterra is a C++ project aiming to recreate the style and feel of a PlayStation 1-era Japanese Role-Playing Game (JRPG), inspired by classics like Final Fantasy VIII.

## Technologies

*   **Language:** C++17
*   **Windowing/Input:** SDL3
*   **Graphics:** bgfx
*   **Build System:** CMake

## Goals

*   Develop a JRPG with PSX-style visuals (low-poly models, specific texture filtering, pre-rendered backgrounds).
*   Implement core JRPG mechanics (battle system, character progression, dialogue, etc.).

## Getting Started

### Prerequisites

*   CMake (version 3.23 or higher)
*   A C++17 compliant compiler (e.g., GCC, Clang, MSVC)
*   [vcpkg](https://github.com/microsoft/vcpkg) for managing dependencies.

### Dependencies

Install the required libraries using vcpkg:

```bash
vcpkg install sdl3 bgfx --triplet <your-target-triplet> # e.g., x64-windows, x64-linux, x64-osx
```

### Building

1.  **Configure CMake:** Use CMake presets (see `CMakePresets.json`) or configure manually, making sure to point CMake to your vcpkg toolchain file:
    ```bash
    cmake -B out/build -S . -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake
    ```
2.  **Build:**
    ```bash
    cmake --build out/build
    ```

The executable will be located in the build directory (e.g., `out/build/Debug/`). 