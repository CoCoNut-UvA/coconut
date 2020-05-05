char *get_attr_str(Config *config, enum AttrType type, char *type_id);
bool type_is_link(Config *config, char *type_id);
char *strupr(char *string);
char *strlwr(char *string);
bool is_traversal_node(Traversal *trav, Node *node);
