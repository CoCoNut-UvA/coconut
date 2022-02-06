/**
 * @file
 *
 * This file contains the code for the dynamicGenTravFunctions traversal.
 * The traversal has the uid: DGT_F
 *
 *
 */
#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav_data.h"
#include "palm/hash_table.h"
#include "filesystem/gen_files.h"
#include "gen_helpers/out_macros.h"
#include "palm/str.h"
#include "palm/memory.h"

static htable_st *lookup;
static FILE *fp;
static int indent = 0;

void DGT_Finit() { return; }
void DGT_Ffini() { return; }

static void *MapChildrenSource(void *key, void *ids)
{
    node_st *curr= (node_st*)ids;
    char *key_upr = STRupper((char*)key);
    OUT_START_FUNC("node_st *TRAV%s(node_st *node)", (char*)key);
    OUT_BEGIN_SWITCH("NODE_TYPE(node)");
    while (curr) {
        OUT_BEGIN_CASE("NT_%s", ID_UPR(curr));
        OUT_FIELD("%s_%s(node) = TRAVopt(%s_%s(node))", ID_UPR(curr), key_upr, ID_UPR(curr), key_upr);
        OUT_FIELD("break");
        OUT_END_CASE();
        curr = ID_NEXT(curr);
    }
    OUT("default:\n");
    indent++;
    OUT_FIELD("DBUG_ASSERT(false, \"Current target has no child %s\")", (char*)key);
    OUT_FIELD("break");
    OUT_END_CASE();
    OUT_END_SWITCH();

    OUT_FIELD("return node");
    OUT_END_FUNC();
    MEMfree(key_upr);
    return ids;
}

static void *MapChildrenInclude(void *key, void *ids)
{
    (void)ids;
    OUT_FIELD("node_st *TRAV%s(node_st *node)", (char*)key);

    return ids;
}

/**
 * @fn DGT_Fast
 */
node_st *DGT_Fast(node_st *node)
{
    lookup = HTnew_String(AST_NUM_NODES(node));
    TRAVopt(AST_INODES(node));
    fp = FSgetSourceFile("trav.c");
    OUT("#include <stdbool.h>\n");
    OUT("#include \"palm/dbug.h\"\n");
    OUT("#include \"ccn/ccn.h\"\n");
    OUT("#include \"ccngen/ast.h\"\n");
    HTmapWithKey(lookup, MapChildrenSource);
    fclose(fp);

    fp = FSgetIncludeFile("trav.h");
    OUT("#include \"ast.h\"\n");
    HTmapWithKey(lookup, MapChildrenInclude);
    fclose(fp);
    return node;
}

/**
 * @fn DGT_Finode
 */
node_st *DGT_Finode(node_st *node)
{
    struct data_dgt_f *data = DATA_DGT_F_GET();
    data->curr_node = node;
    TRAVopt(INODE_ICHILDREN(node));
    TRAVopt(INODE_NEXT(node));
    return node;
}

/**
 * @fn DGT_Fchild
 */
node_st *DGT_Fchild(node_st *node)
{
    struct data_dgt_f *data = DATA_DGT_F_GET();
    node_st *entry = HTlookup(lookup, ID_ORIG(CHILD_NAME(node)));
    node_st *node_name = CCNcopy(INODE_NAME(data->curr_node));
    if (entry == NULL) {
        HTinsert(lookup, ID_ORIG(CHILD_NAME(node)), node_name);
    } else {
        ID_NEXT(node_name) = entry;
        HTinsert(lookup, ID_ORIG(CHILD_NAME(node)), node_name);
    }

    TRAVopt(CHILD_NEXT(node));
    return node;
}
