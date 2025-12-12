# WBSFProjects
A repository that contains all WBSF related repos for a single build

## Setup
1. Clone the repository and its submodules using command : 
```
git clone --recurse-submodules https://github.com/RNCan/WBSFProjects.git
```

2. Open a terminal into the cloned directory and initialize the vcpkg toolchain using commands : (this is required only once after cloning the repository)

On Windows:
```
.\vcpkg\bootstrap-vcpkg.bat
```
OR

On Linux :
```
./vcpkg/bootstrap-vcpkg.sh
```
# Configuring
## Configuring with Visual Studio 2022
1. If using Visual Studio, make sure VS2022 is configured to use vcpkg manifests as explained here : https://learn.microsoft.com/en-us/vcpkg/consume/manifest-mode?tabs=msbuild%2Cbuild-visual-studio
2. Launch Visual Studio, then click the "Open a local folder" button, and select the location where the WBSFProjects repo was cloned
3. At this point, if this is the first time you're building this repository on your PC, vcpkg will download all dependencies (including GDAL and Boost) and build them, which takes a while
4. When the "CMake generation finished" shows in the output window, the final VS2022 solution files have been generated.  Open the global solution located in "./build/vs2022/WBSFProjects.sln".

## Configuring with CMake (Windows & Linux)
1. (Linux) Make sure the following packages are installed using command (on Ubuntu))
```
sudo apt-get install bison flex autoconf libtool
```

2. In the terminal, execute one of the following commands:

On Windows:
```
cmake --preset vs2022
```
On Linux - debug configuration:
```
cmake --preset linux-debug
```
On Linux - release configuration:
```
cmake --preset linux-release
```

# Building
## Building with Visual Studio 2022
Open the CMakePredefinedTargets folder, and then build the ALL_BUILD target

## Building with CMake (Windows & Linux)
In the terminal, execute one of the following commands:

On Windows:
```
cmake --build --preset vs2022
```
On Linux - debug configuration:
```
cmake --build --preset linux-debug
```
On Linux - release configuration:
```
cmake --build --preset linux-release
```

## Build Artifacts
Everything built using this method will go under the ./build/preset-name folder (vs2022, linux-debug or linux-release)

For example, the WeatherGenerator executable will be built into ./build/vs2022/WBSFTools/WeatherGenerator/[Debug,Release].  CMake will automatically copy all needed dlls into the output build folder, except the Models DLLs.

## Installing files for deployment
When build is finished, the BioSIM_API files must be installed so that they can be consumed by the C++ grpc processes in the WebAPI.
### Installing using Visual Studio 2022
Open the CMakePredefinedTargets folder, and then build the INSTALL target
### Installing using CMake
In the terminal, execute one of the following commands:

On Windows:
```
cmake --build --preset vs2022 --target INSTALL
```
On Linux - debug configuration:
```
cmake --build --preset linux-debug --target INSTALL
```
On Linux - release configuration:
```
cmake --build --preset linux-release --target INSTALL
```
Those commands will install all needed files for the SioSIM_API DLL to run including the model .mdl and .dll files into the ./package directory.

At this point, the BioSIM_API DLL is ready to be used for building the WebAPI project.  Please refer to its README file for further instructions.