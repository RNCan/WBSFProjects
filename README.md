# WBSFProjects
A repository that contains all WBSF related repos for a single build

# Setup
1. Clone the repository and its submodules using command : 
git clone --recurse-submodules https://github.com/RNCan/WBSFProjects.git

2. Open a terminal into the cloned directory and initialize the vcpkg toolchain using commands : (this is required only once after cloning the repository)
git clone https://github.com/microsoft/vcpkg.git
.\vcpkg\bootstrap-vcpkg.bat (windows) OR
./vcpkg/bootstrap-vcpkg.sh (linux)

3. If using Visual Studio, make sure VS2022 is configured to use vcpkg manifests as explained here : https://learn.microsoft.com/en-us/vcpkg/consume/manifest-mode?tabs=msbuild%2Cbuild-visual-studio
