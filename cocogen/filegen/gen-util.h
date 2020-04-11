char *get_attr_str(Config *config, Attr *attr);
bool type_is_link(Config *config, Attr *attr);
char *strupr(char *string);
char *strlwr(char *string);
bool node_in_traversal(Config *config, FILE *fp, Traversal *trav, Node *node);