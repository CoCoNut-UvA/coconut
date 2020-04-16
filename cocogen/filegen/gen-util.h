char *get_attr_str(Config *config, Attr *attr);
bool type_is_link(Config *config, Attr *attr);
char *strupr(char *string);
char *strlwr(char *string);
bool is_traversal_node(Config *config, FILE *fp, Traversal *trav, Node *node);
bool is_pass_node(Config *config, FILE *fp, Traversal *trav, Node *node);