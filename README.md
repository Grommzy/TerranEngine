# Terran Engine
A simple rendering engine boilerplate using OpenGL and SDL3.
> This engine project is a stepping stone to being able to build a more functional engine for indie game production.

## Build Environment
Terran Engine is build using CMake generated by Ninja, using MinGW GCC (g++) for compilation.
> To generated the build files, all dependencies (CMake, Ninja, GCC) must be on your system path.

An SDL3.dll must be next to the outputted executable for it to work. This `.dll` is compiled seperately using the latest release of SDL3 using the same method as below.

The generate command used from within an empty /build directory (you need to make this folder in the root folder) is:
`cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug` or `cmake -G Ninja -DCMAKE_BUILD_TYPE=Release`
The executable (used also from within the /build directory) can then be created using:
`Ninja`