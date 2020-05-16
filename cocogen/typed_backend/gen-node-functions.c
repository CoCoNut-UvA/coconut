/**
 * This module generates the functions concerning nodes.
 * However, it does not generate the ctors and dtors, look
 * in the corresponding file for that.
 *
 * Functions generated:
 * - Return macro's and functions:
 * Because the trav functions have an actual return type,
 * we need a way to return other nodes from a trav/pass function.
 * This is achieved by providing ccn_return_<NodeType> functions
 * that will return a void* and do a type checked wrap to a _ccn_return_<NodeType>
 * function to provide a type safe way of doing this return.
 *
 * - check functions:
 *  To ensure the functions returned are the correct type, we do a check after returning
 * and for this we need check functions. For example is a returned node part of
 * a nodeset which can fit correctly in the tree. Yes, even though we have static types
 * we still do dynamic type checking, because there is no other way via returning.
 *
 */