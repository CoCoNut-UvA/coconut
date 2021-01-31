=============
DSL
=============
The DSL used by CoCoNut is mostly case-insensitive for names and other attributes. In some cases, the fully uppercase/lowercase version of a name is used.
The CoCoNut compiler gives an error if you have two names that are equal in their lowercase form.
Nonetheless, casing can be useful in node names. Since CoCoNut provides primitives types like *float*, you can not name your nodes *float*, but you
can name them *Float*. This means that you need to reference *Float* as such and not as *float*. Otherwise, there is no distinction between
the primitive type and the user defined type and it would be impossible to know when used as an attribute. Still, besides the conflicts with primitive types,
it is not possible to define names that only differ in casing. So, a node named *Float* and one named *FLOAT* is not possible.

Enum
================
An enum in the DSL maps to an enum in C, can be used to add extra type information to a node and requires a unique prefix. A unique prefix can not collide with
the prefix of another enum or a CoCoNut reserved prefix. (Link to reserved prefix).
An enum exists out of a set of values, which are defined in the values block. These values map to the values in the C enum and the order
used is kept. Every value will be prefixed with the prefix and to every enum a *NULL* value will be added. The *NULL* value becomes the first entry in the enum.
Lastly, an enum has a name, which corresponds to the name in the generated C enum.

An enum is defined as follows:

.. code-block:: text

    enum <name> {
        prefix = <identifier>,
        values {
            <value 1>, ..., <value n>
        }
    };

An example enum definition looks as follows:

.. code-block:: text

    enum binop {
        prefix = BO,
        values {
            add, sub, mul, div, rem
        }
    };


Node
===============
A node consists out of children, attributes and node lifetimes. Children link to other nodes, which provides the tree building.
Attributes are information stored in the node, every attribute has a type and a name, which becomes a field in the C struct of the node.
Node lifetimes allow specifying the stages in a compiler where the node is disallowed.
In every 'program' there must be one *root* node present. The *root* node will be the root in your AST and the starting point of most of your actions.

.. code-block:: text

    [root] node <name> {
        [children {
            <child 1>,
            ...,
            <child n>
        },]
        [attributes {
            <attribute 1>,
            ...,
            <attribute n>
        },]
        [lifetime {
            <node lifetime 1>,
            ...,
            <lifetime n>
        }]
    };

Children and attributes, for a node, are defined in the following way, respectively:
<child> :: <node type> <name> [ { [constructor], [<lifetimes>] } ]
<attribute> :: <type> <name> [ { constructor } ]

The constructor specifier denotes that a child or attribute should be placed inside the node constructor.
*cocogen* generates a constructor for every node and the order of the arguments to this constructor are children then attributes, but
only if they have the 'constructor' specification.

An example node, without lifetimes, is as follows:

.. code-block:: text

    root node BinOp {
        children {
            Expr left { constructor },
            Expr right { constructor }
        },
        attributes {
            BinOpEnum op { constructor }
        }
    };

This will result in the constructor: ASTbinop(left, right, op);


Lifetimes
==========
Lifetimes allow you to describe the lifetime of a node or a child. When a lifetime is violated, the AST is incosistent and CoCoNut will error.
Lifetimes are described in the following way:

<lifetime> :: <lifetime specifier> <lifetime range>

,where the lifetime range is: \
<start range bracket> [ID.][ID] -> [ID.][ID] <end range bracket>

with the start range bracket being: '(' or '[' and the end range bracket being: ')' or ']'.

The brackets are the mathematical range, so '(' is exclusive and '[' is inclusive.
The IDs are references to actions, which can be specified with namespaces using the '.'.
If no ID is specified it means either the beginning or the end of the compilation.
For example, if a node is disallowed for the whole compilation it can be described in the following way:
disallowed (->)
However, to make this shorter, it is also possible to omit the range and just specify 'disallowed'.

The lifetime specifier depends on the context it is used in. In the node, the lifetime specifier can be 'disallowed' or 'allowed'.
Disallowed means that the node is disallowed in the specified range. So if the node is present in the AST during the range, the AST is inconsistent
and CoCoNut will crash with an appropriate error.
If the lifetime is 'allowed' the node is disallowed outside the given range.
For example, if we have the following actions A -> B -> C -> D -> E
and a node is disallowed from A->B and D->E, we can use allow to specify this by stating: allowed (B -> C]

In children, the lifetime specifier can be 'disallowed' and 'allowed', but also 'mandatory' or 'optional'.
Mandatory means that the child must be present in the node, if not, the AST is inconsistent.
Optional means that outside the given range the child is mandatory.
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
            BinOpEnum op { constructor }
        },
        lifetime {
            disallowed (Stage1.TBO -> Stage3.OPT]
        }
    };

We state that the two children are mandatory in the BinOp node throughout the whole compilation. So if the phase driver finds a
BinOp node where a child is equal to NULL, the AST is inconsistent.
Also, the node itself is disallowed after the 'TBO' action, located in the 'Stage1' phase and up to and including the 'OPT' action in the 'Stage3' phase.
The 'TBO' and 'OPT' values are unique ids and not full names. This is especially useful when you want to target an action that is a couple of levels deep.

When no specific location is specified, using the '.' operator, the first encounter of the action is used. Therefore, in the example, if we had 'OPT' instead of 'Stage3.OPT'
and 'Stage2' also has an 'OTP' then the 'OTP' from 'Stage2' would be seen as the end of the lifetime because it is the first encounter. If only one 'OPT' is present, the need
for something like 'Stage3' is not required but is still allowed.



Nodeset
==================
Some nodes might have children that can be of multiple types. To enable this a nodeset can be created. The node then gets the nodeset
as a child and all the types in the nodeset can be used as a child. A nodeset requires a name and a set of nodes.
The nodes specifier in a nodeset uses a set expression, providing the option to compose nodesets to build a new nodeset.

.. code-block:: text

    nodeset <name> {
        nodes = <set expr>
    };


It is also possible to use a short notation for nodesets.
::

    nodeset <name> = <set expr>;

A set expression is given by a combination of set operations, inline set definitions and references to defined nodesets.
The following set operations are supported:
::

    | Set union
    & Set intersect
    - Set difference

Combining the set operations with inline definitions and references, we can define a simple
Expr nodeset as follows:
::

    nodeset Expr = {Var, Cast} | Constant;

While in the longer form it looks as follows:
::

    nodeset Expr {
        nodes = {Var, Cast} | Constant
    };

The {Var, Cast} statement is an inline set definition and the *Constant* is a reference to another defined nodeset. So, when an identifier is not
enclosed with {} it is seen as a reference to another nodeset. It is also possible to use () to group set expressions and define the evaluation order.


Pass
===============
Passes are the simplest form of an action that can be defined. A pass is simply a function that gets called.
A pass needs a name and a function name. The function name will map to the function name generated in the C code.
It is possible to define information in the info field and unique identifier available in lifetimes.

.. code-block:: text

    pass <name> {
        [info = <string>,]
        [uid = <identifier>,]
        func = <function name>
    };

An example of a pass looks as follows:

::

    pass ScanParse {
        info = "Scan and parse the source files and construct the AST.",
        uid = SP,
        func = doScanParse
    };


It is also possible to define a pass using a shorter notation. With the shorter notation the
name of the pass will be the function name.

.. code-block:: text

    pass <name>;


In C you need to define the pass yourself. A pass accept the specified root node and should return a node
of the same type.



Traversal
====================
A traversal can defined a unique id, info string and the nodes to traverse.
The nodes are in the form of a set expression and can use defined nodesets.

.. code-block:: text

    traversal <name> {
        [info = <string>,]
        uid = <identifier>,
        [nodes = <set expression>,]
        [travdata {
            [[user] <type> <name>,]
        }]
    };

An example of a traversal is as follows:

.. code-block:: text

    traversal RenameFor {
        uid = RFOR,
        nodes = {For, VarLet, Var},
        travdata {
            int changes_made
        }
    };


Some traversals need to traverse all nodes, in such cases, the *nodes* block can be left out.
If the previous traversal targets all nodes it can be defined as follows:

.. code-block:: text

    traversal RenameFor {
        uid = RFOR
    };


The meta compiler will generate a function for every node in the traversal and you need to provide a definition for the
generated functions.

Traversal Data
==============
Some traversals need to pass around data between functions inside the traversal. To make this convenient, CoCoNut provides the option
to denote traversal data in a traversal. Traversal data body is similar to that of attributes, with the extension of user types.
User types are signalled with the 'user' keyword and requires the file "user_types.h" to be on the include path of your compiler. 
CoCoNut automatically creates and destroys the structure of the traversal data. However, CoCoNut does not assume ownership of the members,
therefore, you are required to malloc/free them yourself.

Phase
================
Phases are used to group actions together. Phases contain an actions body, which contains a list of action statements. Action can be
passes, traversals or other phases. Besides actions, phases can also define a gate function and a root. If the gate function is defined
it will be called before the phase is started. If the gate function returns *false*, the phase is skipped. By specifying a root node the full AST will be divided into sub-trees, with the specified root node as the root of these trees.
The actions in the phase will then be executed on the sub-trees. This is useful in optimisations where optimisations can be run on functions in isolation.
To create these sub-trees it is required that these nodes define a child named *next*. If no child named *next* is present, the node can not be a sub-root.
During actions that target a sub-tree, the *next* child is set to *NULL*. It is important to not set a value to the *next* child in these actions because the assigned value will be overwritten with the original value that was pointed to.

The phase also accepts the info string and a unique id.


.. code-block:: text

    phase <name> {
        [info = <string>,]
        [uid = <identifier>,]
        [gate [= <function name>],]

        actions {
            <action>;
            ...
        }

    };

An example phase is as follows:

.. code-block:: text

    phase ConstantFolding {
        uid = CF,
        root = Fundef,
        gate = isConstantFoldingEnabled,

        actions {
            constantFoldOperators;
        }
    };

Cycles
========
In some cases, actions in a phase need to be repeated until a fixed point is reached.
A fixed point is a point where performing the actions does not alter the AST
in any way. For these cases, cycles can be used. Cycles are phases except the actions
are repeated until a fixed point is reached or the maximum number of cycles is reached.

A cycle is defined as follows:

.. code-block:: text

    cycle <name> {
        [info = <string>,]
        [uid = <identifier>,]
        [gate [= <function name>],]

        actions {
            <action>;
        }
    };


Fixed-point detection
======================
The cycles use fixed-point detection to stop a cycle. Fixed point detection is done
by calling the 'CCNcycleNotify()' function. This function notifies the phase driver
that a change is made and a fixed point is not reached. So, the programmer is responsible
for signalling a change. However, in some cases, one change leads to a new one, which leads
back to the original change, and so on. This way you have an ongoing effect and a fixed-point
is never reached. Therefore, a maximum cycle can be specified to prevent this from getting out of control.


Combining primitives
====================
Now it is possible to define the common structure of your compiler using the defined primitives.
A valid CoCoNut program is a combination of these primitives, with 1 root node, 1 start phase and all top-level
primitives are ended by a ';'. There is no scope or namespace in CoCoNut and it is not required to define a primitive before
referencing it.

