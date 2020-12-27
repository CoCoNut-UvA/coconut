=========
 Codegen
=========

From the specification CoCoNut generates C code. In this section, the generated code and how to integrate the rest of your program with CoCoNut is described.

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

    enum binop {
        BT_add,
        BT_sub,
        BT_mul,
        BT_div,
        BT_rem,
    };

The generated enums are found in the *ccngen/enum.h* file.


Nodes
=====
Every defines node gets a constructor generator. A constructor will get the name AST<node name>(<constructor children> and <constructor attributes), where the node name is all lowercase.
The arguments to the constructor are the children and attributes with the constructor specification, with the children first and then the attributes.
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

will result in the constructor: node_st *ASTbinop(node_st *left, node_st *right, enum binop_type op);
the *node_st* type is an opaque structure type to represent nodes. It is not possible to directly access
fields in this structure. Instead, access macros are used to access the fields. Access macros follow the
form of <node_name>_<field_name>, where field name is either the name of child or attribute and both the node name and field are fully capitalized.
So, the BinOp node will get the following access macros:

.. code-block:: C

    BINOP_LEFT(node)
    BINOP_RIGHT(node)
    BINOP_TYPE(node)

These access macros can be used as an lvalue or rvalue. For example, changing the type of a BinOp, when it is a mul, is done as follows

.. code-block:: C

    if (BINOP_TYPE(node) == BT_mul) {
        BINOP_TYPE(node) = BT_add
    }


Every node has a shared part, in object oriented programming a base type, which can be accessed with the NODE_ macros.
For now, only the type of a node can queried. Node types are an enum in the form NT_<node_name>, where the node name is fully capitalized.
So, the BinOp node would be of the type NT_BINOP and can be queried by doing: NODE_TYPE(node).


Passes
======
CoCoNut generates the declaration of the pass function and you are required to write the definition. The generated function declaration depends on
the way the pass is specified in the DSL. If the pass has the function specified CoCoNut will generate that as the function declaration.
So, the following pass:

::

    pass ScanParse {
        info = "Scan and parse the source files and construct the AST.";
        uid = SP,
        func = doScanParse,
    }

will generate the following C declaration: node_st *doScanParse(node_st *node);
You then have to define it. If no func is specified, the pass name will be used.
So, the pass would then be declared as: node_st *ScanParse(node_st *node);


Traversals
==========
CoCoNut generates the declaration for every function the traversal targets(specified in nodes). To do this, the uid is used as the prefix, in all caps, and the node name, all lowercase, is appended to the uid. The following traversal specification

.. code-block:: text

    traversal RenameFor {
        uid = RFOR,
        nodes = {For, VarLet, Var},
        travdata {
            int changes_made
        }
    }

will get the following functions:

.. code-block:: c

   node_st *RFORfor(node_st *)
   node_st *RFORvarlet(node_st *)
   node_st *RFORvar(node_st *)

Traversal Data
==============
Traversal data defined in the DSL is mapped to a struct and can be queried with a macro. The struct is of the type *struct data_<uid>* and the macro is given by
*DATA_<UID>_GET()*. So, the travdata for the previous traversal has the following struct and get macro:

.. code-block:: c

    struct data_rfor;
    DATA_RFOR_GET()



Files
=====
The generated code is distributed among several files, where the headers are sometimes required to operatore on generated data, like the nodes. All include files
are in the directory used for generation under the "ccngen" directory. So, if the gen directory specified is "gen_files", then all files are found under "gen_files/ccngen/"

+------------------+---------------+
| Primitive        |  Include file |
+==================+===============+
| enums            | enum.h        |
+------------------+---------------+
| nodes            | ast.h         |
+------------------+---------------+
| traversal data   | trav_data.h   |
+------------------+---------------+
