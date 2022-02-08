.. CoCoNut documentation master file, created by
   sphinx-quickstart on Tue May  2 11:48:11 2017.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to CoCoNut's documentation!
===================================

CoCoNut is a framework to ease the construction of a compiler by providing a DSL to define the common compiler structure.
From this specification, CoCoNut generates the boilerplate to work with the defined structure. The structure is defined using several compiler primitives: enums, nodes, nodesets, passes, traversals and phases.
This documentation starts with an explanation of the DSL, continues with the generated code from the structure, and then shows how to
communicate with the CoCoNut driver from within your compiler, and, finally, walk through an example building a simple REPL calculator.


.. toctree::
    :maxdepth: 2
    :caption: Contents:

    dsl_syntax
    codegen
    working_with_traversals
    primitive_types
    reserved
    copra
    palm
    calculator
    faq
    bugs


==================
Indices and tables
==================
* :ref:`genindex`
* :ref:`search`
