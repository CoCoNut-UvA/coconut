static smap_t *node_index;
static smap_t *trav_index;

// Node reachability matrix
static bool **node_reachability;

static bool **pass_nodes;
static bool **Traversal_nodes;

void create_indices(Config *config);
void compute_reachable_nodes(Config *config);
void compute_traversal_nodes(Config *config);
int get_trav_index(char *id);
int get_node_index(char *id);
bool is_pass_node(Traversal *trav, Node *node);
bool is_traversal_node(Traversal *trav, Node *node);