.. CoCoNut documentation master file, created by
   sphinx-quickstart on Tue May  2 11:48:11 2017.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to CoCoNut's documentation!
===================================

CoCoNut is a framework to ease the construction of a compiler by providing a DSL to define the common compiler structure. From this specification,
CoCoNut generates boilerplate that defines the common structure, allowing a user to focus on the compiler transformations.

In CoCoNut, a distinction between actions and types is made. Actions denote the structure of the compiler and types denote the structure of your AST.
Actions are defined in terms of types and operate on your AST. There are three forms of actions: phases, traversals and passes.
There are three types of types: enums, nodes and nodesets.

Continue by learning how to specify both actions and types in the CoCoNut DSL; then read to what the DSL is generated;
after, read how to communicate with
CoCoNut from your compiler; and, finally, tie it all together by building a simple REPL calculator.



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
