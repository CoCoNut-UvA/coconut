.. CoCoNut documentation master file, created by
   sphinx-quickstart on Tue May  2 11:48:11 2017.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to CoCoNut's documentation!
===================================

CoCoNut is a framework to ease the construction of a compiler, by providing a DSL to define the common compiler structure. From this specification
CoCoNut can generate the boilerplate to work with the defined structure. CoCoNut provides several primitives to define the structure: enums, nodes, nodesets, passes, traversals and phases. Start with the syntax of the DSL, then read to what the DSL is generated, after read how to communicate with
CoCoNut from your compiler, and, finally, walk trough building a simple REPL calculator.



.. toctree::
    :maxdepth: 2
    :caption: Contents:

    dsl_syntax
    codegen
    working_with_traversals
    primitive_types
    calculator


==================
Indices and tables
==================
* :ref:`genindex`
* :ref:`search`
