===
FAQ
===

Can cycles have a gate?
------------------------
No, at the moment not. You can implement it yourself by adding your root node as a target for the traversal
and checking inside the root handler if the gate is met, if not, you can just do an early return.


Do unreachable nodes need to be handled?
---------------------------------------------------
Yes. If you specify a node as disallowed in a certain range, it
still needs to be handled even if your action is only active in that
range. This is the case, because you can always start the traversal
at another point in your compiler via the *TRAV_START* function.


How do I create a png of my AST?
--------------------------------
In a build directory run ``make && make dot``.
``make`` runs CoCoNut which generates a dot file and ``make dot`` turns your dot file into a png.
However, there is a small bug with the generation of the dot file, see :doc:`bugs`.


Can I define a user type for attributes?
----------------------------------------
Yes, there is experimental support for user type attributes.
Do note however that CoCoNut will not handle user type memory, meaning that the copy
traversal only copies the attributes by value (which can be problematic for pointer types),
and CoCoNut will also not free user type attributes.

Can I have syntax highlighting for the DSL files in vscode?
-----------------------------------------------------------
Yes, we provide a Visual Studio Code extension in the Visual Studio Marketplace,
named `NutCracker <https://marketplace.visualstudio.com/items?itemName=CoCoNut-UvA.nutcracker>`_.
The extension provides syntax highlighting for the CoCoNut DSL files (.ccn). If you have
any issues with the highlighting, or want to contribute, head over to the
corresponding repository here: `https://github.com/CoCoNut-UvA/nutcracker <https://github.com/CoCoNut-UvA/nutcracker>`_
