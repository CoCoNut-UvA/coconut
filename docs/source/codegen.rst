=========
 Codegen
=========

In this section, the code CoCoNut generates is described.
This section also explains how to work with the opaque node structure used by CoCoNut and where to find generated
declarations.

Enum
====
Every enum is generated to a C enum, where the prefix of the enum becomes a prefix for every value in the enum and the type of the enum
is the enum name in lowercase.
So, the following enum definition

.. code-block:: text

    enum binop_type {
        prefix = BT,
        values {
            add, sub, mul, div, rem
        }
    };

will result in the following code:

.. code-block:: c

    enum binop_type {
        BT_add,
        BT_sub,
        BT_mul,
        BT_div,
        BT_rem,
    };

The generated enums are found in the *ccngen/enum.h* file.


Nodes
=====
Every defined node gets a constructor generated. A constructor will get the name *AST<node name>(<constructor children...>',' <constructor attributes...>)*, where the node name is all lowercase.
The arguments to the C constructor are the children and attributes with the *constructor* specificier. The arguments are ordered with children first and then attributes.
Hence, the following node:

.. code-block:: text

    root node BinOp {
        children {
            Expr left { constructor};
            Expr right { constructor};
        },
        attributes {
            binop_type type { constructor };
        }
    };

will result in the constructor:

.. code-block:: C

    node_st *ASTbinop(node_st *left, node_st *right, enum binop_type op);


the *node_st* type is an opaque structure type to represent nodes. It is not possible to directly access
fields in this structure. Instead, access macros are used to access the fields. Access macros follow the
form of <node_name>_<field_name>, where field name is either the name of a child or attribute. Both the node name and field name are fully capitalized.
So, the BinOp node will get the following access macros:

.. code-block:: C

    BINOP_LEFT(node)
    BINOP_RIGHT(node)
    BINOP_TYPE(node)

These access macros can be used as an lvalue or rvalue. For example, changing the type of a BinOp, when it is a mul, is done as follows

.. code-block:: C

    BINOP_TYPE(node) = BINOP_TYPE(node) == BT_mul ? BT_add : BINOP_TYPE(node)


Every node has a shared part, which can be accessed with macros of type *NODE_*.
For now, this is a fixed structure that can be not changed and is mostly used to track source locations.



+------------+---------------+----------------------------------------------------+
| Field name | Access macro  | Usage                                              |
+============+===============+====================================================+
| type       | NODE_TYPE     | The type of the node                               |
+------------+---------------+----------------------------------------------------+
| bline      | NODE_BLINE    | The source line this node starts on                |
+------------+---------------+----------------------------------------------------+
| eline      | NODE_ELINE    | The source line this node ends on                  |
+------------+---------------+----------------------------------------------------+
| bcol       | NODE_BCOL     | The source column this node starts on              |
+------------+---------------+----------------------------------------------------+
| ecol       | NODE_ECOL     | The source column this node ends on                |
+------------+---------------+----------------------------------------------------+
| filename   | NODE_FILENAME | The source filename where this node is located in. |
+------------+---------------+----------------------------------------------------+

Every node has a unique type identified by the *NT_<node_name>* enum entries.
For example, the binop node has type *NT_BINOP*. These types can be obtained using the *NODE_TYPE* macro on a node.


Passes
======
CoCoNut generates the declaration of the pass function and you are required to write the definition. The generated function declaration depends on
the way the pass is specified in the DSL. If the pass has the function specified, CoCoNut will generate that as the function declaration.
So, the following pass:

::

    pass scanParse {
        info = "Scan and parse the source files and construct the AST.";
        uid = SP,
        func = doScanParse,
    }

will generate the following C declaration:

.. code-block:: C

    node_st *doScanParse(node_st *node);


You then have to define it. If no func is specified, the pass name will be used, with the uid as a prefix, if no uid is specified only the pass name
is used.
Thus, the previous pass without a func defined would be declared as:

.. code-block:: C

    *node_st *SPscanParse(node_st *node);*


Traversals
==========
CoCoNut generates the declaration for every function the traversal targets (specified in nodes).
To do this, the uid is used, in all caps, as the prefix and the node name, all lowercase, is appended to the uid.
Thus, the following traversal specification

.. code-block:: text

    traversal RenameFor {
        uid = RFOR,
        nodes = {For, VarLet, Var},
        travdata {
            int changes_made
        }
    }

, will get the following functions.

.. code-block:: c

   node_st *RFORfor(node_st *)
   node_st *RFORvarlet(node_st *)
   node_st *RFORvar(node_st *)

Traversal Data
==============
Traversal data defined in the DSL is mapped to a struct and can be queried with a macro. The struct is of the type *struct data_<uid>* and the macro is given by
*DATA_<UID>_GET()*. So, the travdata for the previous traversal has the following struct and get macro.

.. code-block:: c

    struct data_rfor;
    DATA_RFOR_GET()

Also, every traversal that uses travdata needs to define two functions:

.. code-block:: c
    
    void <trav_uid>init();
    void <trav_uid>fini();

so, for rfor, the following two functions are required.
    
.. code-block:: c

    void RFORinit();
    void RFORfini();


Init is called after construction of the trav data struct, but before starting the traversal. The fini function is called after
the traversal. Note: do not free the traversal data struct, as it is controlled by CoCoNut.

If a user type is used, a header file, called *user_types.h* is required to be available on the include path of your compiler containing the user type.
For example, if you need a FILE* in your travdata, you can define it as a typedef FILE* fileptr and then use the fileptr as a user type.


Generated Skeleton
==================
For passes and traversels, CoCoNut generates a skeleton based on the specification in the DSL, which can be found in the *<gendir>/user* folder.
The passes are placed in the *passes.c* file and every traversal gets its own file with the following name: *trav_<trav_name>.c*, with the name being fully lowercase.


Files
=====
The generated code is distributed among several files, where the headers are sometimes required to operate on generated data, like nodes.
All include files are in the directory used for generation under the "ccngen" directory.
So, if the gen directory specified is "ccngen", then all files are found under "ccngen/ccngen/"

+------------------+---------------+
| Primitive        |  Include file |
+==================+===============+
| enums            | enum.h        |
+------------------+---------------+
| nodes            | ast.h         |
+------------------+---------------+
| traversal data   | trav_data.h   |
+------------------+---------------+
| Child traversals | trav.h        |
+------------------+---------------+
