# CoCoNut 

Dependencies:
- Bison. Bison requires a modern version (>=3). Default on macOS Monterey is too old.
- Flex
- Cmake (> 3.13)
- GCC or Clang. Clang needs the `zero-variadic-macro-arguments` extension and you might need to supress warnings for functions with variadic arguments.
- Graphviz is required if you want to turn the generated dot diagram into an image via `make dot`

For Cmake, Ubuntu 18 or older provides a very old version so it is best to add the kitware ppa or to build from source:
[cmake download](https://cmake.org/download/) or [cmake apt page](https://apt.kitware.com/)
Ubuntu 20 seems to work fine with the version present in the apt repo.

Latest version is tested on Ubuntu 20 and openSUSE Tumbleweed.
A previous version was tested on WSL (Ubuntu 20). Latest not yet, but high chance it works.

# Documentation
Documentation can be found [here](https://coconut-uva.github.io/coconut/)

# Editor support
There is support for the CoCoNut DSL in VScode. See (https://marketplace.visualstudio.com/items?itemName=jeroen45.nutcracker) for more information.
Updates regarding this plugin should go to the corresponding repository (https://github.com/Jeroen-45/nutcracker)
