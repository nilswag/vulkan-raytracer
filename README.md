# vulkan-raytracer

A real-time ray tracer written in C++23 using the Vulkan API (and trying to use minimal libraries).

## Status

Early stage.

## Dependencies

- **C++23** compiler
  - Tested with MSVC (Visual Studio 2022) on Windows 11
  - Tested with g++ on Arch Linux
- **CMake** >= 4.2
- **Vulkan SDK** (provides the Vulkan headers/loader etc., and is required for validation layers in debug builds)
  - [Download here](https://vulkan.lunarg.com/)
- **GLFW** — included as a git submodule (`third_party/glfw`), built from source, no separate install needed

## Cloning

GLFW is pulled in as a submodule, so clone recursively:

```bash
git clone --recurse-submodules https://github.com/nilswag/vulkan-raytracer.git
cd vulkan-raytracer
```

If you already cloned without `--recurse-submodules`, fetch the submodule with:

```bash
git submodule update --init --recursive
```

## Building

The project uses CMake presets (`debug` and `release`), both configuring into a `build/` directory.

### Linux (Arch)

Make sure the Vulkan SDK and a C++23-capable compiler are installed, e.g.:

```bash
sudo pacman -S cmake vulkan-devel vulkan-validation-layers g++
```

Then configure and build:

```bash
cmake --preset release
cmake --build build
```

(swap `release` for `debug` to build with validation layers / debug symbols)

### Windows 11

1. Install the [Vulkan SDK](https://vulkan.lunarg.com/) and a C++23-capable toolchain (Visual Studio 2022 with the "Desktop development with C++" workload works well).
2. Configure and build from a Developer Command Prompt, or open the folder directly in Visual Studio (which picks up `CMakePresets.json` automatically):

```bash
cmake --preset release
cmake --build build --config Release
```

## Running

After building, the executable is in the build output directory:

```bash
# Linux
./build/vulkan-raytracer

# Windows
.\build\Release\vulkan-raytracer.exe
```

## Tested on

- Arch Linux
- Windows 11

## License

MIT — see [LICENSE](LICENSE).
