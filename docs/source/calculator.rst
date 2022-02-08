===================
Calculator Example
===================

In this tutorial, we will build a small calculator with a REPL where an expression is parsed and evaluated by
CoCoNut. A skeleton structure can be found at https://github.com/CoCoNut-UvA/calculator-example

The skeleton contains a basic REPL that parses a line into an AST and then prints the AST. Currently, the AST only
exists out of one number. The parser and lexer are found in the src/scanParse directory and the REPL is present in
src/main.c. There is also the src/calc.ccn file, which defines the AST and three actions: A repl phase,
a scanParse pass and a print traversal. The code for the print traversal is found in src/print.c and the code for the
scanParse pass is found in the src/scanParse/scanParse.c file.

Run the *configure.sh* script to setup an environment. This downloads *CoCoNut* and builds it.
For building, the example uses cmake and can be build by creating a *build* directory by running
*cmake -B build -S ./* in the root of the project. After, you can build the calculator with *make -C build/*.

We will extend the example by adding support for add and substract operations and by evaluating the given
expression.


Adding a binop node
===================
First, to support binary operations, we add a binary operation(Binop) node. This node contains a left and right child
and an operation type. The left and right child can be another binop or a number, therefore we have to extend the nodeset.
We extend the nodeset by changing the current *Expr* nodeset to the following(./src/calc.ccn):

.. code-block:: text

    nodeset Expr = {Binop} | Literals;


Now, we can define the binop node by extending the src/calc.ccn file with the following lines:

.. code-block:: text

    node Binop {
        children {
            Expr left { constructor },
            Expr right { constructor }
        }
    };


Because our print traversal targets all nodes, we need to add the function to handle a binop. Add the following lines
to the src/print.c file:

.. code-block:: text

    node_st *PRTbinop(node_st *node)
    {
        return node;
    }


This result in the calculator being compiled again. However, we have no way to distingoush between the operation in a
binop node. Therefore, we add an enum to the specification by extending the src/calc.ccn file with the following lines:

.. code-block:: text

    enum binop_type {
        prefix = BT,
        values {
            add, sub
        }
    };


and add that enum, as an attribute, to the binop the node. This is done by changing the binop definition to the following:

.. code-block:: text

    node Binop {
        children {
            Expr left { constructor },
            Expr right { constructor }
        },

        attributes {
            binop_type type { constructor }
        }
    };


Now, we have the full binop node defined and can extend our parser to understand add and substraction operations:

.. code-block:: text

    expr: expr '+' expr { $$ = ASTbinop($1, $3, BT_add); }
    |     expr '-' expr { $$ = ASTbinop($1, $3, BT_sub); }
    |     NUM { $$ = ASTnum(yylval.intval); }
    ;


, because we refer to enums, we have to include the *ccngen/enum.h* file, so add that to the top of the parser file to make it compile.


The 'constructor' specifier ensures the nodes can be used in the call to ASTbinop. Constructors do children first and then attributes, hence the call
order in the example. If the constructor specifier is not used you can add change values via the access macros.

Now, after compiling, the repl understands add and substract operations on numbers. However, the REPL does not show anything. We have to adapt the print traversal
to print the binop:

.. code-block:: text

    node_st *PRTbinop(node_st *node)
    {
        TRAVdo(BINOP_LEFT(node));
        printf(" %s ", operatorToString(BINOP_TYPE(node)));
        TRAVdo(BINOP_RIGHT(node));
        return node;
    }

You can uncomment the operatorToString function now, since we defined the enum.

The print traversal first traverses its left child, then prints the operator as a string, lastly it traverses the right child. This results in our REPL printing
the original expression. 

*CoCoNut* also defines TRAV<child> function to traverse children in an easier manner. Therefore, the example of printing in the binop can be adapted to the following:

.. code-block:: text

    node_st *PRTbinop(node_st *node)
    {
        TRAVleft(node);
        printf(" %s ", operatorToString(BINOP_TYPE(node)));
        TRAVright(node);
        return node;
    }

You do need to include the *ccngen/trav.h* file for these functions.

Adding a traversal
==================

Thus, we have a read and print part, but are still missing the evaluate part of the REPL. To add the evaluate we add a new traversal to coconut.
Add the following to the src/calc.ccn file:

.. code-block:: text

    traversal eval {
        uid = EV,
        nodes = Expr
    };

Now, run make and see the following errors:

.. code-block:: text

     undefined reference to `EVbinop'
     undefined reference to `EVnum'


CoCoNut generated the declaration for the traversal functions, but we need to define it. Luckily, CoCoNut generates a skeleton for this traversal and places it
in *<gendir>/user/trav_eval.c*, <gendir> is in our example *build/ccngen/*. So, we can copy that file to our src directory and name it whatever we want, in this
case we name it eval.c by executing the following command from the project root:

.. code-block:: text

    cp ./build/ccngen/user/trav_eval.c ./src/eval.c

Add the ./src/eval.c to your build step(in the example the CMakeLists.txt and add it to add_executable) and the calculator can be compiled again, this time without errors.

We added the traversal to the specification, but have not referenced it yet and thus will never be called. Reference it add by adding it as an action
to the REPL phase, between the scanParse and print actions, in *src/calc.ccn*.

We introduced and referenced a whole new traversal, let's implement the traversal now. The eval traversal evaluates the left and right children and then performs the operation on them. Since left and right are our children, we can use *TRAVchildren*:

.. code-block:: text

    node_st *EVbinop(node_st *node)
    {
        TRAVchildren(node);
        return node;
    }

After the children traversal, we should have a left and right child of type NUM. Now, the operation can be performed on the children, resulting in a new NUM node.
We then return the NUM node replacing the original binop node. To do this, we introduce a new node called *new* and assign it the value of the operation in a NUM
node:

.. code-block:: text

    node_st *EVbinop(node_st *node)
    {
        TRAVchildren(node);
        node_st *new = NULL;
        if (BINOP_TYPE(node) == BT_add) {
            new = ASTnum(NUM_VAL(BINOP_LEFT(node)) + NUM_VAL(BINOP_RIGHT(node)));
        } else {
            new = ASTnum(NUM_VAL(BINOP_LEFT(node)) - NUM_VAL(BINOP_RIGHT(node)));
        }
        return new;
    }

However, we now have a leak because the argument is not returned, so it needs to be freed. This can be done by calling *CCNfree(node)* before returning.

This results in a working REPL calculator for simple add and substract operation. 


Fully using CoCoNut
===================
However, we do not take full advantage of CoCoNut features.
First of all, the eval traversal does nothing with the NUM node, so we can specify that the eval only targets the binop node:

.. code-block:: text

    traversal eval {
        uid = EV,
        nodes = {Binop}
    };

After, we can remove the EVnum function in *eval.c*. Another improvement we can make is denote that a binop should always have a left and right child by specifying
mandatory.

.. code-block:: text

    node Binop {
        children {
            Expr left { constructor, mandatory },
            Expr right { constructor, mandatory }
        },

        attributes {
            binop_type type { constructor }
        }
    };

If CoCoNut finds a binop node in the AST with a left or right child being NULL an error is signalled.
The binop node itself also has a lifetime, because after the eval traversal all binop nodes should be evaluated to a num node. This can be specified by a
lifetime on the node:

.. code-block:: text

    node Binop {
        children {
            Expr left { constructor, mandatory },
            Expr right { constructor, mandatory }
        },

        attributes {
            binop_type type { constructor }
        },

        lifetime {
            disallowed (eval -> )
        }
    }

Here, we specify that the binop node is disallowed after the *eval* action, therefore, if CoCoNut finds a binop node after the eval traversal it will error.
You can try it out by returning the node in the eval traversal instead of evaluating it. That will result in the following:
*error: Found disallowed node(binop) in tree.*

