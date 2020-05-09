#include "ast/ast.h"

static smap_t *node_index;
static smap_t *trav_index;

// Node reachability matrix
static bool **node_reachability;

static bool **pass_nodes;

void compute_reachable_nodes(Config *config);
int get_trav_index(char *id);
int get_node_index(char *id);
bool is_pass_node(Traversal *trav, Node *node);