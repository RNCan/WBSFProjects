# WBSFProjects
A repository that contains all WBSF related repos for a single build

## Setup
1. Clone the repository and its submodules using command : 
```
git clone --recurse-submodules https://github.com/RNCan/WBSFProjects.git
```
*Note : Please make sure that your local clone path doesn't not contain special characters like parenthesis as those can prevent proper configuration or build*

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
## Configuring with CMake (Windows & Linux)
1. Make sure the proper buildtools are installed

On Linux (Ubuntu), use the following command:

```
sudo apt-get install bison flex autoconf libtool
```

On Windows, make sure the BuildTools 2022 are installed. Those can be installed using the Visual Studio 2022 installer. Download Visual Studio 2022, run the installer, and check the options related to the C++ BuildTools 2022.

2. In the terminal, execute one of the following commands:

On Windows:
```
cmake --preset vs2022-ci
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
cmake --build --preset vs2022-ci
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
Everything built using this method will go under the ./build/preset-name folder (vs2022, vs2022-ci, linux-debug or linux-release)

For example, the WeatherGenerator executable will be built into ./build/vs2022/WBSFTools/WeatherGenerator/[Debug,Release].  CMake will automatically copy all needed dlls into the output build folder, except the Models DLLs.

## Installing files for deployment
When build is finished, the BioSIM_API files must be installed so that they can be consumed by the C++ grpc processes in the WebAPI.
### Installing using Visual Studio 2022
Open the CMakePredefinedTargets folder, and then build the INSTALL target
### Installing using CMake
In the terminal, execute one of the following commands:

On Windows:
```
cmake --build --preset vs2022-ci --target INSTALL
```
On Linux - debug configuration:
```
cmake --build --preset linux-debug --target INSTALL
```
On Linux - release configuration:
```
cmake --build --preset linux-release --target INSTALL
```
Those commands will install all needed files for the BioSIM_API DLL to run including the model .mdl and .dll files into the ./package directory.

At this point, the BioSIM_API DLL is ready to be used for building the WebAPI project.  Please refer to its README file for further instructions.
