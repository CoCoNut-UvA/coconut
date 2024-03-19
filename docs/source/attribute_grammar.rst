.. _attribute_grammar:

=============================
Attribute Grammars in CoCoNut
=============================
Attribute Grammars, as originally proposed by Donald E. Knuth `here <https://doi.org/10.1007/BF01692511>`_, provide an intuitive way to do semantic analysis on an AST without using traversals. CoCoNut already has attributes which are associated to nodes to store information, but attribute grammars allow us to define simple evaluation rules to determine the value of the attributes, instead of manually assigning values.

AG support in CoCoNut is still very experimental, so feel free to contact `Tristan Laan <https://www.uva.nl/en/profile/l/a/t.laan/t.laan.html>`_ if you run into any problems, or need additional features.

------------
Introduction
------------
Attribute Grammars denote two types of attributes, synthesized and inherited attributes.
Synthesized attributes are computed from the values of child nodes, which means attribute values will be propagated up the AST.
Inherited attributes are computed from parent or sibling nodes, which allows attribute values to propagate down the AST as well, and provide context on where the node is located in the tree.

CoCoNut has three types of attributes: (1) synthesized attributes, which can have dependencies on attributes from children nodes, or on attributes from the current node; (2) inherited attributes, which can have dependencies on attributes from parent or sibling nodes; and (3) classic attributes, which can not have any dependencies or evaluation rules, and instead must have their value set manually.

---------------------------------
Adding attributes to the DSL file
---------------------------------
Synthesized and inherited attributes can either be assigned to nodesets, or directly to nodes themselves. Do note that when you assign an inherited attribute to a nodeset, member nodes of the nodeset are not allowed to have *additional* inherited attributes. You can add additional synthesized attributes in CoCoNut, even though this does not strictly comply with attribute grammar rules.

As an example, this is a valid configuration:

.. code-block:: text

    nodeset S {
        nodes = {A}
        attributes {
            int p { inherited }
        }
    };

    node A {
        attributes {
            int q { synthesized }
        }
    };

While this is not allowed:

.. code-block:: text

    nodeset S {
        nodes = {A}
        attributes {
            int p { inherited }
        }
    };

    node A {
        attributes {
            int q { inherited }
        }
    };

Synthesized and inherited attributes need to have their dependencies specified at the node level. This is done in equation blocks, where evaluation rules are be declared. We need to know the dependencies (i.e. inputs of the evaluation rules) know, such that CoCoNut can generate the evaluation rule stubs later on. Synthesized attributes have their evaluation rule specified in the corresponding nodes, while inherited attributes have their evaluation rule specified in the parent nodes. This means that root nodes cannot have any inherited attributes. When compiling the DSL file, cocogen will specify where evaluation rules are missing.

An evaluation rule declaration uses the following syntax:

.. code-block::  text

   <target node specifier>.<target attribute name> = [ { args = [ { <dependency 1>, ..., <dependency n> } ] } ]

, where the dependency is in the form:

.. code-block:: text

    <dependency node specifier>.<dependency attribute name>

Node specifiers can either be `this` to target the current node, or a child node of the current node. As a simple example, this is an AST definition of a binary tree with attributes for the sum of the tree, and the depth of the nodes:

.. code-block:: text

    nodeset tree {
        nodes = {binop, leaf},
        attributes {
            int sum {synthesized},
            int depth {inherited}
        }
    };

    root node prog {
        children {
            tree tree
        },
        attributes {
            int sum {synthesized}
        },
        equations {
            tree.depth = {args = {}},
            this.sum = {args = {tree.sum}}
        }
    };

    node binop {
        children {
            tree left,
            tree right
        },
        equations {
            this.sum = {args = {left.sum, right.sum}},
            left.depth = {args = {this.depth}},
            right.depth = {args = {this.depth}}
        }
    };

    node leaf {
        attributes {
            int val {constructor}
        },
        equations {
            this.sum = {args = {this.val}}
        }
    };

----------------
Visit generation
----------------
If we compile the example binary tree AST definition, cocogen will first divide the attributes over separate partitions, where nodes in later partitions can only depend on nodes in the same or lower partitions. Graphs of this process are stored in ``ccngen/ag_dot``. The attributes in each partition are then scheduled into visit sequences. A log of this process is stored in ``ccngen/log/ag_schedule.log``. More details on this process can be found in `this paper <https://doi.org/10.1145/2678015.2682543>`_.

The resulting visits can be found in ``ccngen/visit.c``. As an example, here is the visit for the binop node:

.. code-block:: c

    int CCNvisit_binop_1(node_st *node, int this_depth) {
        int right_depth = EVALbinop_right_depth(this_depth);

        int right_sum;
        if (NODE_TYPE(BINOP_RIGHT(node)) == NT_BINOP) {
            right_sum = CCNvisit_binop_1(BINOP_RIGHT(node), right_depth);
        } else if (NODE_TYPE(BINOP_RIGHT(node)) == NT_LEAF) {
            right_sum = CCNvisit_leaf_1(BINOP_RIGHT(node), right_depth);
        } else {
            DBUG_ASSERT(false, "Problem in visit"); // Should not be able to get here
        }

        int left_depth = EVALbinop_left_depth(this_depth);

        int left_sum;
        if (NODE_TYPE(BINOP_LEFT(node)) == NT_BINOP) {
            left_sum = CCNvisit_binop_1(BINOP_LEFT(node), left_depth);
        } else if (NODE_TYPE(BINOP_LEFT(node)) == NT_LEAF) {
            left_sum = CCNvisit_leaf_1(BINOP_LEFT(node), left_depth);
        } else {
            DBUG_ASSERT(false, "Problem in visit"); // Should not be able to get here
        }

        int this_sum = EVALbinop_this_sum(left_sum, right_sum);

        return this_sum;
    }

We can see the visit receives the inherited attributes as argument, and returns the synthesized attributes. For each child, first the corresponding depth is calculated, and then the visit calls into the child visit to retrieve the corresponding sum. At the end the child sums are used to evaluate and return the total sum.

----------------
Evaluation rules
----------------
You may have noticed that the implementations of the EVAL* functions are missing. This is because you have to provide them! cocogen creates header files for the evaluation rules in ``ccngen/equation.h`` and provides stub files for you to use in ``ccngen/user/eq*.c``. As an example here is the stub file for the binop node:

.. code-block:: c

    #include "ccngen/equations/binop.h"

    int EVALbinop_this_sum(int left_sum, int right_sum) {
        return 0;
    }

    int EVALbinop_left_depth(int this_depth) {
        return 0;
    }

    int EVALbinop_right_depth(int this_depth) {
        return 0;
    }

A correct implementation might look like this:

.. code-block:: c

    #include "ccngen/equations/binop.h"

    int EVALbinop_this_sum(int left_sum, int right_sum) {
        return left_sum + right_sum;
    }

    int EVALbinop_left_depth(int this_depth) {
        return this_depth + 1;
    }

    int EVALbinop_right_depth(int this_depth) {
        return this_depth + 1;
    }

----------------
More information
----------------
For more information, `Wikipedia <https://en.wikipedia.org/wiki/Attribute_grammar>`_ has a short article which gives a brief explanation of the theory behind attribute grammars. As mentioned before `this paper <https://doi.org/10.1145/2678015.2682543>`_ by L. Thomas van Binsbergen, Jeroen Bransen, and Atze Dijkstra provides detailed information on the scheduling part of attribute grammars.
