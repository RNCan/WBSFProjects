# WBSFProjects
A repository that contains all WBSF related repos for a single build

# Setup
1. Clone the repository and its submodules using command : 
```
git clone --recurse-submodules https://github.com/RNCan/WBSFProjects.git
```

2. Open a terminal into the cloned directory and initialize the vcpkg toolchain using commands : (this is required only once after cloning the repository)
```
.\vcpkg\bootstrap-vcpkg.bat (windows)
```
OR
```
./vcpkg/bootstrap-vcpkg.sh (linux)
```

## Configuring with Visual Studio 2022
1. If using Visual Studio, make sure VS2022 is configured to use vcpkg manifests as explained here : https://learn.microsoft.com/en-us/vcpkg/consume/manifest-mode?tabs=msbuild%2Cbuild-visual-studio
2. Launch Visual Studio, then click the "Open a local folder" button, and select the location where the WBSFProjects repo was cloned
3. When the "CMake generation finished" shows in the output window, the final VS2022 solution files have been generated.  Open the global solution located in "./build/vs2022/WBSFProjects.sln".

## Configuring with CMake (Windows & Linux)
1. In the terminal, execute command 
```
cmake --preset vs2022"
```

## Building with Visual Studio 2022
1. Open the CMakePredefinedTargets folder, and then build the ALL_BUILD target

## Building with CMake (Windows & Linux)
1. In the terminal, execute command 
```
cmake --build --preset vs2022
```