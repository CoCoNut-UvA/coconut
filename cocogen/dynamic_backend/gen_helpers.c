#include "dynamic_backend/gen_helpers.h"
#include "palm/str.h"


char *DGHpassFuncName(node_st *pass)
{
    if (IPASS_TARGET_FUNC(pass)) {
        return STRcpy(ID_ORIG(IPASS_TARGET_FUNC(pass)));
    }

    if (IPASS_IPREFIX(pass)) {
        return STRcat(ID_UPR(IPASS_IPREFIX(pass)), ID_ORIG(IPASS_NAME(pass)));
    }

    return STRcpy(ID_ORIG(IPASS_NAME(pass)));
}

char *DGHattributeField(node_st *attr)
{
    return ID_ORIG(ATTRIBUTE_NAME(attr));
}

char *DGHattributeAccess(node_st *node, node_st *attr, char *arg)
{
    return STRcatn(6, ID_UPR(INODE_NAME(node)), "_", ID_UPR(ATTRIBUTE_NAME(attr)), "(", arg, ")");
}
