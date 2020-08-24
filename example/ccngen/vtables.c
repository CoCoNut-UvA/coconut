#include "ccngen/actions.h"
#include "ccn/ccn_types.h"
const ccn_trav_ft ccn_error_vtable[_NT_SIZE] = { &TRAVerror, &TRAVerror, &TRAVerror,  };

const ccn_trav_ft change_vtable[_NT_SIZE] = { &TRAVerror, &changebinop, &changenum,  };

const ccn_trav_ft trav_vtable[_NT_SIZE] = { &TRAVerror, &travbinop, &travnum,  };

const ccn_trav_ft ccn_free_vtable[_NT_SIZE] = { &TRAVerror, &DELbinop, &DELnum,  };

const ccn_trav_ft ccn_copy_vtable[_NT_SIZE] = { &TRAVerror, &CPYbinop, &CPYnum,  };

const ccn_trav_ft ccn_check_vtable[_NT_SIZE] = { &TRAVerror, &CHKbinop, &CHKnum,  };

const ccn_trav_ft *ccn_trav_vtable[_TRAV_SIZE] = { ccn_error_vtable, change_vtable, trav_vtable, ccn_free_vtable, ccn_copy_vtable, ccn_check_vtable,  };

const ccn_pass_ft ccn_pass_vtable[_PASS_SIZE] = { &PASSerror, &p,  };

