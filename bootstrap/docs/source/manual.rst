=============
Manual
=============

Overview
=========
CoCoNut is a framework to ease the construction of a compiler. By providing a DSL to define the common structure of a compiler, the boilerplate
for this common structure can be generated. This removes the time spent on writing the boilerplate yourself and gives you more time to spend
on interesting parts of a compiler, like optimisations.

Terminology
=============
CoCoNut divides the common structure into four parts: phases, traversals, passes and the abstract syntax tree (AST). The AST exists out of nodes
and the traversals traverse the AST and perform operations on the AST. A pass is an action performed on the AST but does not
traverse the AST from within the framework. A phase is a structure to combine passes, traversals and other phases. This provides a clear
distinction between certain stages in your compiler. From now on, we will often denote phases, passes and traversals as actions.


Domain specific language
=========================
To define the common structure CoCoNut uses a domain specific language (DSL).

Nodes
------

In the DSL nodes can be defined that make up your AST. Nodes exist out of children, attributes and lifetimes. Children link to other nodes, this allows
the building of the tree. Attributes are data fields that are part of the node, for example, an integer in a Num node. Lifetimes allow specifying
when a node is allowed or disallowed in the AST. When CoCoNut finds a node that does not adhere to its lifetime, the compilation is stopped with an error.
Every AST requires a root node, in the DSL this is done with the 'root' keyword, and is only allowed on one node.

So, a node is defined in the following way:

.. code-block:: text

    [root] node <name> {
        [children {
            <child 1>,
            <child 2>
        },]
        [attributes {
            <attribute 1>,
            <attribute 2>
        },]
        [lifetime {
            <node lifetime 1>,
            <node lifetime 2>,
        }]
    }

Children and attributes are defined in the following way, respectively:

.. code-block:: text

    <node type> <name> [ { [constructor], [<lifetimes>] } ]
    <type> <name> [= <default value>] [ { [constructor], [<lifetimes>] }]

This applied to a real definition we get the following example:

.. code-block:: text

    root node BinOp {
        children {
            Expr left { constructor },
            Expr right { constructor}
        },
        attributes {
            BinOpEnum op { constructor }
        }
    };

CoCoNut generated the create and free functions for the defined nodes. To determine which children and attributes should be parameters
in the create functions, the 'constructor' keyword is needed. The 'constructor' keyword signals to CoCoNut that the child or attribute
should be present as a parameter in the create function.

Enums
-----
In the node example, the attribute is an enum. The DSL also provides a way to define enums, which can then be used as attributes in nodes.
The enums defined in the DSL will map to enums in C. To prevent naming collisions, every defined enum requires a prefix. Also, CoCoNut will
automatically inject a 'NULL' value in your enums. This can be used to disable an enum in the AST.

The following is the definition for an enum:

.. code-block:: text

    enum <name> {
        prefix = <identifier>,
        values {
            <value 1>, <value 2>, ...
        }
    }

So, the enum will be named in C by the given name and the values will be prefixed with the prefix.
Applying this we get the following example for the enums used in the node example:


.. code-block:: text

    enum binop {
        prefix = BO,
        values {
            add, sub, mul, div, rem
        }
    }

This would result in the enum in C, with the fields: BO_add, BO_sub, ..., BO_rem and BO_NULL.

Nodesets
---------
In the node example, we see that the children are of type 'Expr'. An expression is often a combination
of multiple node types. To enable this, it is possible to define nodesets. Nodesets combine nodes under one name, which allows
it to use as a child in a node definition. Nodesets map to C unions.

The following is the definition for a nodeset:

.. code-block:: text

    nodeset <name> {
        nodes = <set expr>
    }


It is also possible to use a short notation for nodesets.
::

    nodeset <name> = <set expr>

A set expression is given by a combination of set operations, inline set definitions and references to defined nodesets.
The following set operations are supported:
::

    | Set union
    & Set intersect
    - Set difference

Applying this, the 'Expr' example from the node definition might look as follows:

::

    nodeset Expr = {BinOp} | Constants

where the '{ ... }' defines an anonymous nodeset, which can wrap defined nodes. In this example 'Constants' is another defined nodeset.
So, by using nodesets as buildings blocks the definitions become very modular.

Lifetimes
---------
In the node definition, we also see lifetime options for the node, children and attributes. Lifetimes can be applied on all nodes and all children, but only
on attributes that are enums or pointers.

The possible lifetime specification for a node is as follows:

::

    <lifetime specifier> <lifetime range>

where the lifetime specifier is either 'disallowed' or 'allowed. Disallowed means that the node is not allowed in the AST during the range.
Allowed means that the node is disallowed outside the given range. The range specifies the range in which the lifetime specifier should be applied.
The range uses the ids of actions to determine this.
The definition for a range is as follows:

::

    <start bracket> [ID.][ID] -> [ID.][ID] <end bracket>

where the start bracket is either '(' or ']' and the and bracket is either ')' or ']'.
This is based on mathematical ranges, so '(' is exclusive and '[' is inclusive. The used IDs correspond to the name or prefix
of an action.

In some cases, an action is part of multiple phases. In such cases, the first occurrence of the action is used in the range.
So, to be more specific, you can use the '.' operator to signal the right occurrence to use by specifying its parent phase.
For example, if action C, is part of A and B, and A happens before B, by giving the following range '(-> C)', it means that the
lifetime specifier is applied from the beginning of the compilation up to the first occurrence of 'C', which is in A. However, if
we want it to be the 'C' in 'B', we could specify it as follows: '(-> B.C)'. So, using the '.' operator, we signal that we want the 'C' present
in phase B.

In some occasion, a range should cover the full compilation, start from the beginning or go till the end. In such cases, the ID can be left out.
If the ID is not present in front of '->' operator, it means that the lifetime specifier is active from the start of the compilation.
If the ID is not present after the '->' operator, it means that the lifetime specifier is active till the end of the compilation.
So, if a lifetime specifier needs to be active the whole compilation, the following can be used: '(->)'. In such cases, the whole range
can be omitted, so only the lifetime specifier is required.


The lifetime specification for the possible attributes and children is a bit more eloberate and is as follows:

::

    <lifetime specifier> <lifetime range> [= <lifetime target>]

The lifetime specifier has disallowed and allowed, but also mandatory and optional. Mandatory means that the attribute or child
should be present in the node in the AST during the range. If it is not present, the AST is inconsistent. Optional means that the
child or attribute is mandatory outside the given range. The lifetime range is the same as with the node lifetime.
Also, lifetimes of attributes and children can be performed on a subset of the child or attribute by using the lifetime target.
For example, if the lifetime is applied on a nodeset, the lifetime target can be used to apply it on a subset.
So, let us have the following nodeset:

::

    nodeset A = {B, C, D, E}

And we want to disallows B during a stage, it would look as follows:

::

    disallowed (Stage1 -> Stage3) = {B}


It is important to note, that mandatory and optional can only target one element in a subset. *Cocogen* will error when mandatory or optional
is applied on a larger subset.


Passes
-------
Now that we can define nodes, we need to apply actions on the nodes.
The simplest action is a pass.
A pass takes in a node and returns a node of the same type. Passes do not traverse the AST. A pass can, for example, be used to read in a source file
and construct the AST. Or provide information during compilation, which is stored in the Root node.

A pass is defined in the following way:

::

    pass <name> {
        [info = <string>,]
        [prefix = <identifier>,]
        func = <function name>
    }

A pass will be mapped to a C function. It is possible to specify this name with the func keyword. The prefix is used as an identifier, as seen in lifetimes,
but can also be used as a prefix for the function. So, it is required to keep your prefixes unique, which is enforced by *cocogen*. If no prefix is given
*cocogen* will generate one based on the pass name.

A full example pass specified is as follows:

::

    pass ScanParse {
        info = "Scan and parse the source files and construct the AST.",
        prefix = SP,
        func = doScanParse
    }

If the pass function is the same as a pass name and no extra information is required, a shorter notation can be used.
The shorter notation is defined as follows:

::

     pass <name>


Traversals
----------
When the AST needs to be traversed a traversal can be used. A traversal will traverse the AST and call functions for the specified nodes.
A traversal is defined as follows:

::

    traversal <name> {
        [info = <string>,]
        [prefix = <identifier>,]
        nodes = <set expression>
    }

Again, a traversal accepts an info string and a prefix and follows the same rules for the prefix as the pass. Besides, the traversal also
accepts the nodes to visit in the AST, which is, again, a set expression, as seen with nodesets.

An example of a traversal is as follows:

::

    traversal RenameFor {
        prefix = RFOR,
        nodes = {For, VarLet, Var}
    }

When a traversal needs to traverse all the nodes, the nodes can be omitted. Also, there is a possible shorthand notation for
a traversal in the following style:

::

    traversal <name> [= <set expression>]

This defines the traversal in a shorter description and the set expression will be seen as the nodes to traverse.
So, the previous example can be written as:

::

    traversal RenameFor = {For, VarLet, Var}

However, we can not define prefix, so the prefix of the shorter notation will be inferred from the name, in this case
it would result in the prefix: 'RF'.

Phases
--------
Now, that we can define actions, we need a way to group them. Therefore phases can be used.
Phases are defined in the following way:

::

    phase <name> {
        [info = <string>,]
        [prefix = <identifier>,]
        [root = <node identifier>,]
        [gate [= <function name>],]

        actions {
            <action 1>;
            <action 2>;
        }

    }

A phase has the same info and prefix options as passes and traversals. However, the phase also offers to specify a 'root' node. A root node can be used
to state that the specified root should be treated as the root node for that phase. This means that the actions in the phase are executed on subtrees.
The subtrees are made by making the specified root the root of the subtree and these subtrees are isolated. Nodes that are specified as such,
need to have a child named 'next'. If such a child is not present, *cocogen* will complain. The subtrees are isolated by setting the *next* child to *NULL*.
Therefore it is not possible to set the *next* child in phases that execute on a subtree. Also, it is undefined behaviour to change the root of the subtree.

Also, it is possible to define a gate function. A gate function is called before the phase starts and should return a boolean. True when the phase should
be executed and false otherwise. This can be useful to provide optional phases in the compiler, for example, optimisations.

An example of a phase is as follows:

::

    phase ConstantFolding {
        prefix = CF,
        root = Fundef,
        gate = isConstantFoldingEnabled,

        actions {
            constantFoldOperators;
        }
    }

The actions in a phase are traversals, passes or other phases. It is possible to reference other actions in the action body as seen in the example
below, where constantFoldOperators is a traversal defined somewhere else. It is also possible to define actions in the action body of a phase.

Cycles
-------
Sometimes, it is useful to repeat the actions in a phase. However, it is not always known, how often the phase should be repeated or it is often program dependant.
Therefore, cycles can be used. Cycles are phases that are repeated until a fixed point is reached. A fixed point is a point where executing the actions
on the AST does not change the AST. However, in some cases actions are ping-ponging changes and a fixed point is never reached. Therefore, the maximum number
of cycles can be specified as well.

Cycles are defined in the following way:

::

    cycle <name> {
        [info = <string>,]
        [prefix = <identifier>,]
        [root = <node identifier>,]
        [gate [= <function name>],]

        actions {
            <action 1>;
            <action 2>;
        }

    }


Fixed point detection is implemented in the form of signals. You, the programmer, are responsible for calling a function when a change is made
in a cycle that affects the fixed point. The function to do this is called: *ccn_cycle_notify()* and found in the 'core/cycle.h' header.


Phase driver
============
During the compilation, occasionally communication between your compiler and the CoCoNut framework is required. Therefore, the phase driver
provides an API to communicate. The phase driver is the part in CoCoNut that is responsible for the phases, which is the top level of the compiler flow.

Starting the compilation
------------------------
To start the compilation you need call CCNstart(syntaxtree). If the full compilation process is done via
*CoCoNut* the syntaxtree is likely NULL. However, it is also possibly to leave the parsing process out of 
*CoCoNut* and start after with a built AST.
.. code-block:: C

    #include "ccn/phase_driver.h"

    int main() {
        CCNstart(NULL);
    }


Plugins
========
Besides the start command, *CoCoNut* offers commands that target plugins you can enable.
The plugins can be used to ease your development process during debugging or profiling.

The code for the plugins are not automatically generated but need to be enabled with a flag
to the metacompiler *cocogen*. Executing *cocogen --plugins* will give you a list of all the
plugins and how they are enabled.

After enabling them you need to provide the required data to the phase driver. The plugins
are designed in such a way that they do not impose any flags on your compiler, therefore
you need to provide the required flags for your compiler. These command-line
arguments should then be passed to the phase driver.

Below are all the plugins explained, how you enable them and the possible commands for the phase driver.

Consistency checking
--------------------
Using the *--consistency-checks* flag with *cocogen* generates the code for consistency checking of the AST during compilation.
Consistency checking uses the lifetimes defined in the DSL file to check if the AST is still consistent according
to your specification. If the AST is inconsistent, an error will be displayed with the violation. Consistency checking
is meant for debugging your compiler. Consistency checking has no API in the phase driver as everything is constructed
from the definition in your DSL file.

