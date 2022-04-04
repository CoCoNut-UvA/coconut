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
No, at the moment you can not and you likely do not need it.
If you have a usecase where you really need it, please let me know.