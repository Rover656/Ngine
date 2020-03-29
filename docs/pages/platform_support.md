Platform Support
================

[TOC]

Ngine supports a wide range of platforms. This page details build instructions for each platform.
All build instructions are compatible with the CMake config guides.

## Windows (Native)
Ngine primarily supports native Windows games and therefore Windows will have every feature (where applicable).

### Graphics APIs
Windows supports OpenGL, OpenGL ES (via ANGLE) and DirectX graphics APIs.

### System Requirements
| OS Version                        | GRAPHICS                    |
| --------------------------------- | --------------------------- |
| Windows 10 1809 (Version 17763.0) | DirectX 11 or OpenGL 3.3    |

### Building
To build, you must have at least Visual Studio 2017 installed with relevant workloads. Once installed, run the VS command line and run the following:
MinGW support is untested.
```
mkdir build & cd build
cmake .. <Additional CMake Options>
cmake --build . --config <Debug or Release>
```

## Linux
All Ngine features are available on Linux.

### System Requirements
| OS Version                        | GRAPHICS                    |
| --------------------------------- | --------------------------- |
| Undetermined                      | OpenGL 3.3                  |

### Graphics APIs
Linux only supports the OpenGL graphics API.

### Building
To build on linux, follow the following instructions.
The following packages need installed (example here is for linux):
```
sudo apt-get update
sudo apt-get install build-essential libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev
```
Ngine can then be built with the following command:
```
mkdir build & cd build
cmake -DCMAKE_BUILD_TYPE=<Release or Debug> <Other CMake config> ..
make
```

## Mac OS X
Ngine will be tested against Mac OS X in the near future.

## UWP (W10 Desktop, Xbox One)
Ngine supports UWP across Desktop and Xbox One. No Mobile UWP features will be added as Windows 10 Mobile is discontinued.

### System Requirements
| OS Version                        | GRAPHICS      |
| --------------------------------- | ------------- |
| TBD (likely same as Desktop)      | DirectX 11    |

### Graphics APIs
Ngine on UWP uses DirectX and OpenGL ES (via ANGLE).

### Missing Features
- Ngine does not support touch input currently.

### Building
Building for Ngine requires Visual Studio 2017 or 2019 with UWP workloads.
Ngine includes a build script `BuildUWP.bat` which will generate a directory `build-uwp` with Visual Studio 2019 project files.
It is recommended to implement this script for your own projects for ease.

## Android
Ngine for Android is planned.

### Estimated System Requirements
| OS Version (Projected)            | GRAPHICS                    |
| --------------------------------- | --------------------------- |
| Android 8.0 (Oreo)                | OpenGL ES 3.0               |

## Not listed?
If a platform is not listed, it means it has not been considered yet, feel free to ask about this on the GitHub page.
It is planned to add easier platform porting in the future (allowing community ports to be made easily).

## CMake Options
| Option             | Description                                      | Default |
| ------------------ | ------------------------------------------------ | ------- |
| BUILD_TEST         | Build the test program                           | ON      |
| BUILD_DOCS         | Build the documentation (release only)           | ON      |
| BUILD_EXAMPLES     | Build example code                               | OFF     |
| BUILD_NGINE_STATIC | Build Ngine as a static library                  | ON      |
| BUILD_NGINE_SHARED | Build Ngine as a shared library                  | OFF     |
| PLATFORM           | The platform to build for (Desktop, UWP)         | Desktop |
| ENABLE_OPENGL      | Enable use of OpenGL (On supported platforms)    | ON      |
| ENABLE_OPENGLES    | Enable use of OpenGL ES (On suported platforms)  | ON      |
| ENABLE_DIRECTX     | Enable use of DirectX (On suported platforms)    | ON      |
