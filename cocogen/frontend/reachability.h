#pragma once


#define RCB_NODE_NOT_HANDLED (1 << 0) // Becomes a TRAVnop;
#define RCB_NODE_HANDLED_BY_TRAV (1 << 1) // Becomes a TRAVchildren call
#define RCB_NODE_HANDLED_BY_USER (1 << 2) // Becomes a user defined trav call
