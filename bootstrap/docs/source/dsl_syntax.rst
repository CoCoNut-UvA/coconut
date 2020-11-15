==========================
Syntax and internals
==========================

Some parts of the DSL make use of a 'prefix'. The 'prefix' can be used to prefix the generated functions
to prevent naming collisions. The 'prefix' will also be used as a shorted identifier for your actions, which
allows the prefix to be used as an identifier. This does mean that a prefix must be unique.


Enum
================
An enum in the DSL maps to an enum in C. It can be used to add extra type information to a node.
It always requires a prefix that needs to be identical and should not collide with prefixes owned by *CoCoNut*.
In the values block, the values of the enum can be written. These will map to the values in the C enum and the order
used is kept. Every value will be prefixed with the prefix and to every enum, a *NULL* value will be added.

.. code-block:: text

    enum <name> {
        prefix = <identifier>,
        values {
            <value 1>, <value 2>, ...
        }
    }

An example enum definition looks as follows:

.. code-block:: text

    enum binop {
        prefix = BO,
        values {
            add, sub, mul, div, rem
        }
    }


Node
===============
A node can consist out of children, attributes and node lifetimes. Children link to other nodes, which allows to build a tree.
Attributes are information stored in the node, every attribute has a type and a name. This then becomes a field in the C struct of the node. Node lifetimes
allow specifying the stages in a compiler where the node is disallowed. So if the node is present the AST is inconsistent, which is an error.
In every DSL 'program' there must be one *root* node present. The *root* node will be the root in your AST.

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

Children and attributes, for a node, are defined in the following way, respectively:
<node type> <name> [ { [constructor], [<lifetimes>] } ]
<type> <name> [= <default value>] [ { constructor } ]

An example node, without lifetimes, is as follows:

.. code-block:: text

    root node BinOp {
        children {
            Expr left { constructor},
            Expr right { constructor}
        },
        attributes {
            BinOpEnum op { constructor }
        }
    };


Lifetimes
==========
Lifetimes allow you to describe the lifetime of a node, attribute or child. When a lifetime is violated, the AST is incosistent.
Lifetimes are described in the following way:

<lifetime specifier> <lifetime range>

,where the lifetime range is: \
<start range bracket> [ID.][ID] -> [ID.][ID] <end range bracket>

with the start range bracket being: '(' or '[' and the end range bracket being: ')' or ']'.

The brackets are the mathematical range, so '(' is exclusive and '[' is inclusive.
The IDs are references to actions, which can be specified with namespaces using the '.'.
If no ID is specified it means either the beginning or the end of the compilation.
For example, if a node is disallowed for the whole compilation it can be described in the following way:
disallowed (->)
However, to make this shorter, it is also possible to omit the range and just specify 'disallowed'.

The lifetime specifier depends on the context. In the node, the lifetime specifier can be 'disallowed' or 'allowed'.
Disallowed means that the node is disallowed in the specified range. So if the node is present in the AST, during the range, the AST is inconsistent
and the phase driver will crash with an appropriate error.
If the lifetime is 'allowed' the node is disallowed outside the given range.
So, for example, if we have the following actions A -> B -> C -> D -> E
and a node is disallowed from A->B and D->E, we can use allow to specify this by stating: allowed (B -> C]

In children and some attributes, namely enums and pointers, the lifetime specifier can be 'disallowed' and 'allowed', but also 'mandatory' or 'optional'.
Mandatory specifier means that the child or attribute must be present in the node if not, the AST is inconsistent.
Optional means that outside the given range the attribute or child is mandatory.
Attributes and children can also apply the lifetime on a subset of its values by giving the lifetime
a set to target. This is done by using the '=' sign and a set of values corresponding to the type.

So, we can rewrite the previous node definition with lifetimes the following way:

.. code-block:: text

    root node BinOp {
        children {
            Expr left { constructor, mandatory},
            Expr right { constructor, mandatory}
        },
        attributes {
            BinOpEnum op { constructor, disallowed (CompileBooleanExpressions -> ) = {and, or} }
        },
        lifetime {
            disallowed (Stage1.TBO -> Stage3.OPT]
        }
    };

We state that the two children are mandatory in the BinOp node throughout the whole compilation. So if the phase driver finds a
BinOp node where a child is equal to NULL, the AST is inconsistent.
The attribute has two values, 'and' and 'or', that are disallowed after the action named 'CompileBooleanExpressions'.
Also, the node itself is disallowed after the 'TBO' action, located in the 'Stage1' phase and up to and including the 'OPT' action in the 'Stage3' phase.
The 'TBO' and 'OPT' values are identifiers and not full names. This is especially useful when you want to target an action that is a couple of levels deep.

When no specific location is specified, using the '.' operator, the first encounter of the action is used. So in the example, if we had 'OPT' instead of 'Stage3.OPT'
and 'Stage2' also has an 'OTP' then the 'OTP' from 'Stage2' would be seen as the end of the lifetime because it is the first encounter. If only one 'OPT' is present, the need
for something like 'Stage3' is not required but is still allowed.



Nodeset
==================
Some nodes might have children that can be of multiple types. To enable this a nodeset can be created. The node then gets the nodeset
as a child and all the types in the nodeset can then be used as a child. A nodeset requires a name and a set of nodes.
Nodes use a set expression, which allows to combined several nodesets to build new ones. The set of nodes should contain
defined nodes in the DSL.

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

Combining the set operations with inline definitions and references, we can define a simple
Expr nodeset as follows:
::

    nodeset Expr = {Var, Cast} | Constant

While in the longer form it looks as follows:
::

    nodeset Expr {
        nodes = {Var, Cast} | Constant
    }

The {Var, Cast} statement is an inline set definition and the *Constant* is a reference to another defined nodeset. So, when an identifier is not
enclosed with {} it is seen as a reference to another nodeset. It is also possible to use () to group set expressions and define the determination order.


Pass
===============
Passes are the simplest form of an action that can be defined. A pass is simply a function that gets called.
A pass needs a name and a function name. The function name will map to the function name generated in the C code.
It is possible to define information in the info field and again a prefix.

.. code-block:: text

    pass <name> {
        [info = <string>,]
        [prefix = <identifier>,]
        func = <function name>
    }

An example of a pass looks as follows:

::

    pass ScanParse {
        info = "Scan and parse the source files and construct the AST.",
        prefix = SP,
        func = doScanParse
    }


It is also possible to define a pass using a shorter notation. With the shorter notation the
name of the pass will be the function name and a prefix will be generated based on the name.

.. code-block:: text

    pass <name>


In C you need to define the pass yourself. A pass accept the specified root node and should return a node
of the same type.



Traversal
====================
A traversal needs to define the nodes to traverse. Besides that it can also define a info string and a prefix.
The nodes are in the form of a set expression and can use defined nodesets as well.

.. code-block:: text

    traversal <name> {
        [info = <string>,]
        [prefix = <identifier>,]
        nodes = <set expression>
    }

An example of a traversal is as follows:

.. code-block:: text

    traversal RenameFor {
        prefix = RFOR,
        nodes = {For, VarLet, Var}
    }


Some traversals need to traverse all nodes, in such cases, the *nodes* block can be left out.
Or a shorthand traversal can be used in the form:

.. code-block:: text

    traversal <name>

The meta compiler will generate a function for every node in the traversal and you need to provide a definition for the
generated functions.

Phase
================
Phases are used to group actions together. Phases contain an actions body, which contains a list of action statements. Action can be
passes, traversals or other phases. Besides actions, phases can also define a gate function and a root. If the gate function is defined
it will be called before the phase is started. If the gate function returns *false*, the phase is skipped. This can be useful to implement
optional optimisations. By specifying a root node the full AST will be divided into sub-trees, with the specified root node as the root of these trees.
The actions in the phase will then be executed on the sub-trees. This is useful in optimisations where optimisations can be run on functions in isolation.
To create these sub-trees it is required that these nodes define a child named *next*. If no child named *next* is present, the node can not be a sub-root.
During actions that target a sub-tree, the *next* child is set to *NULL*. It is important to not set a value to the *next* child in these actions because the assigned value will be overwritten with the original value that was pointed to.

The phase also accepts the info string and a prefix.


.. code-block:: text

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

An example phase is as follows:

.. code-block:: text

    phase ConstantFolding {
        prefix = CF,
        root = Fundef,
        gate = isConstantFoldingEnabled,

        actions {
            constantFoldOperators;
        }
    }

Cycles
========
In some cases, actions in a phase need to be repeated until a fixed point is reached.
A fixed point is a point where performing the actions does not alter the AST
in any way. For these cases, cycles can be used. Cycles are phases except the actions
are repeated for until a fixed point is reached or the maximum number of cycles is reached.

A cycle is defined as follows:

.. code-block:: text

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


Fixed-point detection
======================
The cycles use fixed-point detection to stop a cycle. Fixed point detection is done
by calling the 'ccn_cycle_notify()' function. This function notifies the phase driver
that a change is made and a fixed point is not reached. So, the programmer is responsible
for signalling a change. However, in some cases, one change leads to a new one, which leads
back to the original change, and so on. This way you have an ongoing effect and a fixed-point
is never reached. Therefore, a maximum cycle can be specified to prevent this from getting out of control.
