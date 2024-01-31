# CoCoNut

Dependencies:
- Bison. Bison requires a modern version (>=3). Default on macOS Monterey is too old.
- Flex
- Cmake (> 3.13)
- GCC or Clang. Clang needs the `zero-variadic-macro-arguments` extension and you might need to supress warnings for functions with variadic arguments.
- Graphviz is required if you want to turn the generated dot diagram into an image via `make dot`

The latest version is tested on Ubuntu 22.04 LTS (including WSL), Fedora 39 and openSUSE Tumbleweed.

# Documentation
Documentation can be found [here](https://coconut-uva.github.io/coconut/)

# Editor support
There is support for the CoCoNut DSL in VScode. See (https://marketplace.visualstudio.com/items?itemName=CoCoNut-UvA.nutcracker) for more information.
Updates regarding this plugin should go to the corresponding repository (https://github.com/CoCoNut-UvA/nutcracker).
