.. _attribute_grammar:
=============================
Attribute Grammars in CoCoNut
=============================
Attribute Grammars, as originally proposed by Donald E. Knuth `here <https://doi.org/10.1007/BF01692511>`_, provide an intuitive way to do semantic analysis on an AST without using traversals. CoCoNut already has attributes which are associated to nodes to store information, but attribute grammars allow us to define simple evaluation rules to determine the value of the attributes, instead of manually assigning values.

AG support in CoCoNut is still very experimental, so feel free to contact `Tristan Laan <https://www.uva.nl/en/profile/l/a/t.laan/t.laan.html>`_ if you run into any problems, or need additional features.

------------
Introduction
------------
Attribute Grammars denote two types of attributes, synthesized and inherited attributes.
Synthesized attributes are computed from the values of child nodes, which means attribute values will be propagated up the AST.
Inherited attributes are computed from parent or sibling nodes, which allows attribute values to propagate down the AST as well, and provide context on where the node is located in the tree.

CoCoNut has three types of attributes: (1) synthesized attributes, which can have dependencies on attributes from children nodes, or on attributes from the current node; (2) inherited attributes, which can have dependencies on attributes from parent or siblin nodes; and (3) classic attributes, which can not have any dependencies or evaluation rules, and instead must have their value set manually.

TODO: continue documentation
