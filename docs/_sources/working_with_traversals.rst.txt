=======================
Working With CoCoNut
=======================
During the compilation process of your compiler, you need to interact with the CoCoNut runtime.
For example, to specify new traversal directions, freeing nodes, and so on.

When functions are prefixed with *CCN* they are found in the *ccn/ccn.h* header file.


Traversals
===========
Traversals require most interaction with CoCoNut, therefore, CoCoNut defines some helpers to make working
with traversal smoother.

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


, and is expanded to.

.. code-block:: C

     BINOP_LEFT(binop) = TRAVopt(BINOP_LEFT(binop))
     BINOP_RIGHT(binop) = TRAVopt(BINOP_RIGHT(binop))

The *children* variant makes TRAVopt calls but if combined with consistency checking, errors are caught soon enough.

Besides these standard functions, *CoCoNut* also generates a TRAV<child> function, where the child is lower case form
of a child name. Thus, the previous example can also be defined as follows:

.. code-block:: C

    TRAVleft(binop);
    TRAVright(binop);

since the binop has children named *left* and *right*, the functions will traverse the left and right child of the binop.
This, again, does the assignment for you and uses *opt* calls. Hence, the pattern of *target = TRAV<do/opt>(target)* can often be prevented.


Cycles
======
Cycles

The maximum cycles has the default set to 100, however, that can be changed any time by calling the *CCNsetCycles(size_t cycle_count)* function.

Nodes
=====
CoCoNut provides copy and free functions for defined nodes in the form of *CCNcopy* and *CCNfree*.
When a node is copied, all children are copied and all attributes except node attributes are deep-copied.
When a node is freed, all children of the node are freed as well and all attributes except node attributes are cleaned up.
Hence, node attributes can be used to link to other nodes without causing double free issues.

Error signals
=============
There are two ways to signal an error to CoCoNut: *CCNerrorPhase()* and *CCNerrorAction()*. The error phase function stops the compiler
after the current phase and the error action function stops the compiler after the current action.

