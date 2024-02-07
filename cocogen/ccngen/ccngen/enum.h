#pragma once
enum nodesettype {
    NS_NULL,
    NS_VISIT_SEQUENCE_ALT,
    NS_VISIT_SEQUENCE,
    NS_SETEXPR,
    NS_LINK,
    NS_ACTION,
    _NS_SIZE,
};

enum ccn_nodetype {
    NT_NULL,
    NT_ID,
    NT_IENUM,
    NT_VISIT_SEQUENCE_DUMMY,
    NT_VISIT_SEQUENCE_VISIT,
    NT_VISIT_SEQUENCE_EVAL,
    NT_VISIT,
    NT_VISIT_ARG_LIST,
    NT_ATTRIBUTE_REFERENCE,
    NT_EQUATION_DEPENDENCY,
    NT_EQUATION,
    NT_ATTRIBUTE,
    NT_ITRAVDATA,
    NT_SETOPERATION,
    NT_SETLITERAL,
    NT_SETREFERENCE,
    NT_STE,
    NT_CHILD,
    NT_LIFETIME_RANGE,
    NT_ILIFETIME,
    NT_NODESET_CHILD_ENTRY,
    NT_INODESET,
    NT_INODE,
    NT_IPASS,
    NT_ITRAVERSAL,
    NT_IPHASE,
    NT_IACTIONS,
    NT_AST,
    _NT_SIZE,
};

enum ccn_traversal_type {
    TRAV_NULL,
    TRAV_PRT,
    TRAV_SLC,
    TRAV_SLD,
    TRAV_SLIS,
    TRAV_SLU,
    TRAV_SLI,
    TRAV_SAV,
    TRAV_RCB,
    TRAV_CPR,
    TRAV_CHA,
    TRAV_PRA,
    TRAV_USE,
    TRAV_NCT,
    TRAV_BST,
    TRAV_CEX,
    TRAV_DGVS,
    TRAV_DGVF,
    TRAV_DGEF,
    TRAV_DGEI,
    TRAV_DGNM,
    TRAV_DGNSAF,
    TRAV_DGNAF,
    TRAV_DGNS,
    TRAV_DGIF,
    TRAV_DGBU,
    TRAV_DGNC,
    TRAV_DGE,
    TRAV_DGEV,
    TRAV_DGTV,
    TRAV_DGNFTT,
    TRAV_DGPT,
    TRAV_DGTT,
    TRAV_DGST,
    TRAV_DGAH,
    TRAV_DGTA,
    TRAV_DGCT,
    TRAV_DGCC,
    TRAV_DGFT,
    TRAV_DGCHT,
    TRAV_DGTDS,
    TRAV_DGTDC,
    TRAV_DGTDCH,
    TRAV_DGTDG,
    TRAV_DGTDU,
    TRAV_DGT_F,
    TRAV_GAA,
    TRAV_GAAH,
    TRAV_GIT,
    TRAV_GGH,
    TRAV_DGTDT,
    TRAV_GD,
    TRAV_DUGE,
    TRAV_DUGT,
    TRAV_DUGP,
    TRAV_SGNS,
    TRAV_CSM,
    TRAV_MITL,
    TRAV_AITA,
    TRAV_CLT,
    TRAV_free,
    TRAV_check,
    TRAV_cpy,
    _TRAV_SIZE,
};

enum ccn_pass_type {
    PASS_NULL,
    PASS_DYNAMICSWITCHTOVISITSOURCE,
    PASS_DYNAMIC_START_VISIT_HEADER,
    PASS_DYNAMIC_START_EQUATION_HEADER,
    PASS_DYNAMICGENBASENODEINIT,
    PASS_DYNAMICSWITCHTOASTSOURCE,
    PASS_DYNAMICGENBASENODE,
    PASS_DYNAMIC_START_AST_HEADER,
    PASS_DYNAMICSWITCHTRAVDATA,
    PASS_DYNAMICSTARTTRAVDATA,
    PASS_DYNAMICSTARTTABLEGEN,
    PASS_DYNAMICSTARTENUMHEADER,
    PASS_DOOPTS,
    PASS_GENDEFINES,
    PASS_SCANANDPARSE,
    _PASS_SIZE,
};

enum setoperation_type {
    SO_NULL,
    SO_iunion,
    SO_intersect,
    SO_difference,
};

enum attribute_type {
    AT_NULL,
    AT_user,
    AT_link,
    AT_link_or_enum,
    AT_int,
    AT_string,
    AT_bool,
    AT_int8,
    AT_int16,
    AT_int32,
    AT_int64,
    AT_float,
    AT_double,
    AT_uint,
    AT_uint8,
    AT_uint16,
    AT_uint32,
    AT_uint64,
};

enum child_type {
    CT_NULL,
    CT_inode,
    CT_inodeset,
};

enum lifetime_type {
    LT_NULL,
    LT_disallowed,
    LT_mandatory,
    LT_optional,
    LT_allowed,
};

