=======================
Working With CoCoNut
=======================

All *CCN* functions are found in the *ccn/ccn.h* header file.

Traversals
===========
Traversals require most interaction with CoCoNut, therefore, CoCoNut defines some helpers to make working
with traversal smoother. CoCoNut also makes optimisations and only traverses sub-trees that can contain
nodes the traversal targets.

Reachability
------------
Reachability is the concept that a node is reachable in a sub-tree for the current traversal.
If you specify that a traversal only targets(via the nodes) the *BinOp* node, then CoCoNut only traverses the
paths in the full AST that can reach a BinOp. This way, you only have to write functions for nodes you target.


Traversing
----------
Traversing is done by using any of the *TRAV* functions. Currently, CoCoNut defines three such functions, namely:

.. code-block:: C

    TRAVopt(node);
    TRAVdo(node);
    TRAVchildren(node);


The *opt* and *do* variant will traverse the given node, where the *do* variant requires that the node is not NULL.
These variants return the result of the traversal. Therefore, if the traversal makes changes to a node, the result should be assigned to something or it is lost.
For example, changing the left and right child of a binop is done as follows:

.. code-block:: C

    BINOP_LEFT(binop) = TRAVopt(BINOP_LEFT(binop))
    BINOP_RIGHT(binop) = TRAVopt(BINOP_RIGHT(binop))

The *children* variant takes a node and traverses all its children, in top to bottom order of your definition, via a *TRAVopt*. The *children* variant does
the assignment for you. So, the previous example becomes:

.. code-block:: C

    TRAVchildren(binop)


, and is expanded to:

.. code-block:: C

     BINOP_LEFT(binop) = TRAVopt(BINOP_LEFT(binop))
     BINOP_RIGHT(binop) = TRAVopt(BINOP_RIGHT(binop))

The *children* variant makes TRAVopt calls but if combined with consistency checking, errors are caught soon enough.

Besides these standard functions, *CoCoNut* also generates a TRAV<child> function, where the child is lower case form
of a child name. Thus, the previous example can also be defined as follows:

.. code-block:: C

    TRAVleft(binop);
    TRAVright(binop);

since the binop has children named *left* and *right*, the functions will traverse the right child. This, again, does the
assignment for you and uses *opt* calls. Hence, the patter of *target = TRAVopt(target)* can often be prevented.


Cycles
======
Cycles

The maximum cycles has the default set to 100, however, that can be changed any time by calling the *CCNsetCycles(size_t cycle_count)* function.

Error signals
=============
There are two ways to signal an error to CoCoNut: *CCNerrorPhase()* and *CCNerrorAction()*. The error phase function stops the compiler
after the current phase and the error action function stops the compiler after the current action.

