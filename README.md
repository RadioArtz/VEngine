# VEngine

A basic C++ game engine built with OpenGL.

## Build

1. Clone the repository and submodules:

   ```text
   git clone --recurse-submodules https://github.com/RadioArtz/VEngine.git
   ```

2. Open `VEngine.sln` in Visual Studio 2026.
3. Build the `x64` `Debug` configuration.

### Assimp

Assimp is not included in this repository. You can download prebuilt binaries here:
[Assimp's downloads](https://kimkulling.itch.io/the-asset-importer-lib), or
build Assimp with CMake yourself.

For the itch.io download make sure to copy the following files:
```text
Assimp/include/assimp/*                   -> Lib/include/assimp/
Assimp/lib/x64/assimp-vc145-mt.lib			-> Lib/library/
Assimp/bin/x64/assimp-vc145-mt.dll			-> Lib/library/
```

if you build with CMake:
```text
Assimp/include/assimp/*						   -> Lib/include/assimp/
Assimp/lib/Debug/assimp-vc145-mt.lib		-> Lib/library/
Assimp/bin/Debug/assimp-vc145-mt.dll		-> Lib/library/
```

## Dependencies

Included dependencies are GLFW, GLM, glad, Khronos headers, and stb_image.
Dear ImGui is a Git submodule. Assimp is external.
[THIRD-PARTY-NOTICES.md](THIRD-PARTY-NOTICES.md).

## License

VEngine's original code is licensed under the MIT License. See [LICENSE](LICENSE).
