// Ignore clang-format to keep the values aligned
// clang-format off
#pragma once

// ******************** Prefixes of generated functions ********************

// Prefix of functions to create nodes of the AST
#define CREATE_FUNC_PREFIX          "create_"

// Prefix of functions to free nodes of the AST
#define FREE_FUNC_PREFIX            "free_"

// Prefix of functions to copy nodes of the AST
#define COPY_FUNC_PREFIX            "copy_"

// Prefix of functions to replace nodes in the AST
#define REPLACE_NODE_FUNC_PREFIX    "replace_"

// Prefix of functions traversal related functions
#define TRAV_PREFIX                 "trav_"

// Prefix of functions to start a new traversal
#define TRAV_START_FUNC_PREFIX      "trav_start_"

// Prefix of the functions of passes
#define PASS_PREFIX                 "pass_"

// Prefix of functions of the phasedriver
#define PHASEDRIVER_PREFIX          "phasedriver_"

// Prefix of serialization functions
#define SERIALIZATION_PREFIX        "serialization_"

// ******************** Names of enum types ********************

// Name of the enum type containing all nodes and nodesets
#define NT_ENUM_NAME                "NodeType"

// Name of the enum type containing all traversals
#define TRAV_ENUM_NAME              "TraversalType"

// ******************** Prefix of enum type values ********************

// Prefix of values of the enums of nodesets containing the possible nodes
#define NS_ENUM_PREFIX              "NS_"

// Prefix of values of the enum type containing all nodes and nodesets
#define NT_ENUM_PREFIX              "NT_"

// Prefix of values of the enum type containing all traversals
#define TRAV_ENUM_PREFIX            "TRAV_"

// ***************** Format of enum type names and functions *****************

// Format of enum types of nodesets containing all possible nodes
// arg1 = nodeset identifier
#define NS_ENUMTYPE_FORMAT          NS_ENUM_PREFIX "%s_enum"

// Format of values of enum types of nodesets containing all possible nodes
// arg1 = nodeset identifier, arg2 = identifier of one of the possible nodes
#define NS_FORMAT                   NS_ENUM_PREFIX "%s_%s"

// Format of values of the enum type of all nodes and nodesets
// arg1 = node identifier
#define NT_FORMAT                   NT_ENUM_PREFIX "%s"

// Format of values of the enum type of all traversals
// arg1 = traversal identifier
#define TRAV_FORMAT                 TRAV_ENUM_PREFIX "%s"

// Format of the node handle functions of a traversal
// arg1 = traversal identifier, arg2 = node or nodeset identifier
#define TRAVERSAL_HANDLER_FORMAT    "%s_%s"

#define TRAV_START_FORMAT           TRAV_START_FUNC_PREFIX "%s"

// Format of functions to create a new node
// arg1 = node identifier
#define CREATE_NODE_FORMAT          CREATE_FUNC_PREFIX "%s"

// Format of functions to create a new nodeset
// arg1 = nodeset identifier, arg2 = node identifier
#define CREATE_NODESET_FORMAT       CREATE_FUNC_PREFIX "%s_%s"

// Format of functions to copy a node
// arg1 = node or nodeset identifier
#define COPY_NODE_FORMAT            COPY_FUNC_PREFIX "%s"

// Format of functions to replace a node
// arg1 = node identifier
#define REPLACE_NODE_FORMAT         REPLACE_NODE_FUNC_PREFIX "%s"

// Formats of functions to free a subtree or only the node
// arg1 = node identifier
#define FREE_TREE_FORMAT            FREE_FUNC_PREFIX "%s_tree"
#define FREE_NODE_FORMAT            FREE_FUNC_PREFIX "%s_node"

// Formats of entry function of a pass
// arg1 = pass identifier
#define PASS_ENTRY_FORMAT           PASS_PREFIX "%s_entry"

// Formats for serialization functions
// arg1 = node/nodeset identifier
#define SERIALIZE_WRITE_BIN_FORMAT  SERIALIZATION_PREFIX "write_binfile_%s"
#define SERIALIZE_READ_BIN_FORMAT   SERIALIZATION_PREFIX "read_binfile_%s"
#define SERIALIZE_WRITE_TXT_FORMAT  SERIALIZATION_PREFIX "write_txtfile_%s"
#define SERIALIZE_READ_TXT_FORMAT   SERIALIZATION_PREFIX "read_txtfile_%s"
