# CoCoNut
Compiler construction framework

# Requirements
You need cmake with the minimum version being 3.13 because it allows relative paths in target\_sources.
It always builds with the latest cmake version(currently 3.17).
[Installing cmake](https://cmake.org/install/)

# Structure
- palm is the library used by many components of the CoCoNut framework.
- cocogen is the meta compiler for the CoCoNut framework.

# Build instructions
Running the following commands creates an out-of-source build in the build/ directory.
```sh
mkdir build
cd build
cmake ../
make
```
If this succeeds, you can find the cocogen executable in the build/cocogen/ directory.
