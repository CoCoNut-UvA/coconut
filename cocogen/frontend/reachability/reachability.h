#pragma once


struct ast *RBdoTrav(struct ast *ast);
int RBgetTravIndex(struct traversal *trav);
int RBgetNodeIndex(struct node *node);
bool RBisTraversalNode(struct traversal *trav, struct node *node);
bool RBshouldPassNode(struct traversal *trav, struct node *node);