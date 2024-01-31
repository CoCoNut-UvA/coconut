#include "frontend/ctihelp.h"
#include "globals.h"

void id_to_info(node_st *ID, struct ctinfo *info) {
   struct ctinfo tmp = {.first_line = ID_ROW(ID), .filename = globals.filename,
           .line = HTlookup(globals.line_map, &ID_ROW(ID)),
           .first_column = ID_COL_BEGIN(ID), .last_column = ID_COL_END(ID)};
   *info = tmp;
}
