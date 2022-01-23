#pragma once

#define GH_DEFAULT_NODE_TYPE() "ccn_node"
#define GH_ENSURE_DEFAULT_NODE_TYPE() "typedef struct " GH_DEFAULT_NODE_TYPE() " " GH_DEFAULT_NODE_TYPE()
