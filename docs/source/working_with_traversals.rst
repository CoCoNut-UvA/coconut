=======================
Working With Traversals
=======================

Traversals require most interaction with CoCoNut, therefore, CoCoNut defines some helpers to make working
with traversal smoother. CoCoNut also makes optimisations and only traverses sub-trees that can contain
nodes the traversal targets.

Reachability
============
Reachability is the concept that a node is reachable in a sub-tree for the current traversal.
If you specify that a traversal only targets(via the nodes) the *BinOp* node, then CoCoNut only traverses the
paths in the full AST that can reach a BinOp. This way, you only have to write functions for nodes you target.


Traversing
==========
Traversing is done by using any of the *TRAV* functions. Currently, CoCoNut defines three such functions, namely:

.. code-block: C

    TRAVopt(node);
    TRAVdo(node);
    TRAVchildren(node);


The *opt* and *do* variant will traverse the given node, where the *do* variant requires that the node is not NULL.
These variants return the result of the traversal. Therefore, if the traversal makes changes to a node, the result should be assigned to something or it is lost.
For example, changing the left and right child of a binop is done as follows:

..code-block: C

    BINOP_LEFT(binop) = TRAVopt(BINOP_LEFT(binop))
    BINOP_RIGHT(binop) = TRAVopt(BINOP_RIGHT(binop))

The *children* variant takes a node and traverses all its children via a *TRAVopt*. The *children* variant does
the assignment for you. So, the previous example becomes:

..code-block: C

    TRAVchildren(binop)


, under the hood this is expanded to:

..code-block: C

     BINOP_LEFT(binop) = TRAVopt(BINOP_LEFT(binop))
     BINOP_RIGHT(binop) = TRAVopt(BINOP_RIGHT(binop))

The *children* variant makes TRAVopt calls but if combined with consistency checking, errors are caught soon enough.