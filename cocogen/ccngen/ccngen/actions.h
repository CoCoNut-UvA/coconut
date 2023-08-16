#pragma once
typedef struct ccn_node ccn_node;
struct ccn_node *PRTid(struct ccn_node *node);
struct ccn_node *PRTienum(struct ccn_node *node);
struct ccn_node *PRTattribute_reference(struct ccn_node *node);
struct ccn_node *PRTequation_dependency(struct ccn_node *node);
struct ccn_node *PRTequation(struct ccn_node *node);
struct ccn_node *PRTattribute(struct ccn_node *node);
struct ccn_node *PRTitravdata(struct ccn_node *node);
struct ccn_node *PRTsetoperation(struct ccn_node *node);
struct ccn_node *PRTsetliteral(struct ccn_node *node);
struct ccn_node *PRTsetreference(struct ccn_node *node);
struct ccn_node *PRTste(struct ccn_node *node);
struct ccn_node *PRTchild(struct ccn_node *node);
struct ccn_node *PRTlifetime_range(struct ccn_node *node);
struct ccn_node *PRTilifetime(struct ccn_node *node);
struct ccn_node *PRTnodeset_child_entry(struct ccn_node *node);
struct ccn_node *PRTinodeset(struct ccn_node *node);
struct ccn_node *PRTinode(struct ccn_node *node);
struct ccn_node *PRTipass(struct ccn_node *node);
struct ccn_node *PRTitraversal(struct ccn_node *node);
struct ccn_node *PRTiphase(struct ccn_node *node);
struct ccn_node *PRTiactions(struct ccn_node *node);
struct ccn_node *PRTast(struct ccn_node *node);
ccn_node *SLCsetliteral(ccn_node *);
ccn_node *SLCsetliteral(ccn_node *);
ccn_node *SLDsetliteral(ccn_node *);
ccn_node *SLDsetliteral(ccn_node *);
ccn_node *SLISsetliteral(ccn_node *);
ccn_node *SLISsetliteral(ccn_node *);
ccn_node *SLUsetliteral(ccn_node *);
ccn_node *SLUsetliteral(ccn_node *);
ccn_node *SLIsetliteral(ccn_node *);
ccn_node *SLIsetliteral(ccn_node *);
ccn_node *RCBast(ccn_node *);
ccn_node *RCBast(ccn_node *);
ccn_node *RCBsetliteral(ccn_node *);
ccn_node *RCBsetliteral(ccn_node *);
ccn_node *RCBitraversal(ccn_node *);
ccn_node *RCBitraversal(ccn_node *);
ccn_node *RCBinode(ccn_node *);
ccn_node *RCBinode(ccn_node *);
ccn_node *RCBchild(ccn_node *);
ccn_node *RCBchild(ccn_node *);
ccn_node *RCBinodeset(ccn_node *);
ccn_node *RCBinodeset(ccn_node *);
ccn_node *CPRast(ccn_node *);
ccn_node *CPRast(ccn_node *);
ccn_node *CPRinode(ccn_node *);
ccn_node *CPRinode(ccn_node *);
ccn_node *CPRequation(ccn_node *);
ccn_node *CPRequation(ccn_node *);
ccn_node *CPRattribute(ccn_node *);
ccn_node *CPRattribute(ccn_node *);
ccn_node *CPRchild(ccn_node *);
ccn_node *CPRchild(ccn_node *);
ccn_node *CHAinodeset(ccn_node *);
ccn_node *CHAinodeset(ccn_node *);
ccn_node *CHAinode(ccn_node *);
ccn_node *CHAinode(ccn_node *);
ccn_node *CHAattribute(ccn_node *);
ccn_node *CHAattribute(ccn_node *);
ccn_node *PRAast(ccn_node *);
ccn_node *PRAast(ccn_node *);
ccn_node *PRAinodeset(ccn_node *);
ccn_node *PRAinodeset(ccn_node *);
ccn_node *PRAnodeset_child_entry(ccn_node *);
ccn_node *PRAnodeset_child_entry(ccn_node *);
ccn_node *PRAsetliteral(ccn_node *);
ccn_node *PRAsetliteral(ccn_node *);
ccn_node *USEsetoperation(ccn_node *);
ccn_node *USEsetoperation(ccn_node *);
ccn_node *USEsetliteral(ccn_node *);
ccn_node *USEsetliteral(ccn_node *);
ccn_node *USEast(ccn_node *);
ccn_node *USEast(ccn_node *);
ccn_node *USEsetreference(ccn_node *);
ccn_node *USEsetreference(ccn_node *);
ccn_node *NCTsetoperation(ccn_node *);
ccn_node *NCTsetoperation(ccn_node *);
ccn_node *NCTsetliteral(ccn_node *);
ccn_node *NCTsetliteral(ccn_node *);
ccn_node *NCTinodeset(ccn_node *);
ccn_node *NCTinodeset(ccn_node *);
ccn_node *NCTast(ccn_node *);
ccn_node *NCTast(ccn_node *);
ccn_node *NCTsetreference(ccn_node *);
ccn_node *NCTsetreference(ccn_node *);
struct ccn_node *BSTid(struct ccn_node *node);
struct ccn_node *BSTienum(struct ccn_node *node);
struct ccn_node *BSTattribute_reference(struct ccn_node *node);
struct ccn_node *BSTequation_dependency(struct ccn_node *node);
struct ccn_node *BSTequation(struct ccn_node *node);
struct ccn_node *BSTattribute(struct ccn_node *node);
struct ccn_node *BSTitravdata(struct ccn_node *node);
struct ccn_node *BSTsetoperation(struct ccn_node *node);
struct ccn_node *BSTsetliteral(struct ccn_node *node);
struct ccn_node *BSTsetreference(struct ccn_node *node);
struct ccn_node *BSTste(struct ccn_node *node);
struct ccn_node *BSTchild(struct ccn_node *node);
struct ccn_node *BSTlifetime_range(struct ccn_node *node);
struct ccn_node *BSTilifetime(struct ccn_node *node);
struct ccn_node *BSTnodeset_child_entry(struct ccn_node *node);
struct ccn_node *BSTinodeset(struct ccn_node *node);
struct ccn_node *BSTinode(struct ccn_node *node);
struct ccn_node *BSTipass(struct ccn_node *node);
struct ccn_node *BSTitraversal(struct ccn_node *node);
struct ccn_node *BSTiphase(struct ccn_node *node);
struct ccn_node *BSTiactions(struct ccn_node *node);
struct ccn_node *BSTast(struct ccn_node *node);
struct ccn_node *CEXid(struct ccn_node *node);
struct ccn_node *CEXienum(struct ccn_node *node);
struct ccn_node *CEXattribute_reference(struct ccn_node *node);
struct ccn_node *CEXequation_dependency(struct ccn_node *node);
struct ccn_node *CEXequation(struct ccn_node *node);
struct ccn_node *CEXattribute(struct ccn_node *node);
struct ccn_node *CEXitravdata(struct ccn_node *node);
struct ccn_node *CEXsetoperation(struct ccn_node *node);
struct ccn_node *CEXsetliteral(struct ccn_node *node);
struct ccn_node *CEXsetreference(struct ccn_node *node);
struct ccn_node *CEXste(struct ccn_node *node);
struct ccn_node *CEXchild(struct ccn_node *node);
struct ccn_node *CEXlifetime_range(struct ccn_node *node);
struct ccn_node *CEXilifetime(struct ccn_node *node);
struct ccn_node *CEXnodeset_child_entry(struct ccn_node *node);
struct ccn_node *CEXinodeset(struct ccn_node *node);
struct ccn_node *CEXinode(struct ccn_node *node);
struct ccn_node *CEXipass(struct ccn_node *node);
struct ccn_node *CEXitraversal(struct ccn_node *node);
struct ccn_node *CEXiphase(struct ccn_node *node);
struct ccn_node *CEXiactions(struct ccn_node *node);
struct ccn_node *CEXast(struct ccn_node *node);
ccn_node *DGNMast(ccn_node *);
ccn_node *DGNMast(ccn_node *);
ccn_node *DGNMinode(ccn_node *);
ccn_node *DGNMinode(ccn_node *);
ccn_node *DGNMchild(ccn_node *);
ccn_node *DGNMchild(ccn_node *);
ccn_node *DGNMattribute(ccn_node *);
ccn_node *DGNMattribute(ccn_node *);
ccn_node *DGNSast(ccn_node *);
ccn_node *DGNSast(ccn_node *);
ccn_node *DGNSinode(ccn_node *);
ccn_node *DGNSinode(ccn_node *);
ccn_node *DGNSchild(ccn_node *);
ccn_node *DGNSchild(ccn_node *);
ccn_node *DGNSattribute(ccn_node *);
ccn_node *DGNSattribute(ccn_node *);
ccn_node *DGIFast(ccn_node *);
ccn_node *DGIFast(ccn_node *);
ccn_node *DGIFinode(ccn_node *);
ccn_node *DGIFinode(ccn_node *);
ccn_node *DGIFchild(ccn_node *);
ccn_node *DGIFchild(ccn_node *);
ccn_node *DGIFattribute(ccn_node *);
ccn_node *DGIFattribute(ccn_node *);
ccn_node *DGBUast(ccn_node *);
ccn_node *DGBUast(ccn_node *);
ccn_node *DGBUinode(ccn_node *);
ccn_node *DGBUinode(ccn_node *);
ccn_node *DGNCast(ccn_node *);
ccn_node *DGNCast(ccn_node *);
ccn_node *DGNCinode(ccn_node *);
ccn_node *DGNCinode(ccn_node *);
ccn_node *DGNCchild(ccn_node *);
ccn_node *DGNCchild(ccn_node *);
ccn_node *DGNCattribute(ccn_node *);
ccn_node *DGNCattribute(ccn_node *);
ccn_node *DGEast(ccn_node *);
ccn_node *DGEast(ccn_node *);
ccn_node *DGEinode(ccn_node *);
ccn_node *DGEinode(ccn_node *);
ccn_node *DGEitraversal(ccn_node *);
ccn_node *DGEitraversal(ccn_node *);
ccn_node *DGEinodeset(ccn_node *);
ccn_node *DGEinodeset(ccn_node *);
ccn_node *DGEipass(ccn_node *);
ccn_node *DGEipass(ccn_node *);
ccn_node *DGEVienum(ccn_node *);
ccn_node *DGEVienum(ccn_node *);
ccn_node *DGEVast(ccn_node *);
ccn_node *DGEVast(ccn_node *);
ccn_node *DGEVid(ccn_node *);
ccn_node *DGEVid(ccn_node *);
ccn_node *DGTVitraversal(ccn_node *);
ccn_node *DGTVitraversal(ccn_node *);
ccn_node *DGTVast(ccn_node *);
ccn_node *DGTVast(ccn_node *);
ccn_node *DGNFTTast(ccn_node *);
ccn_node *DGNFTTast(ccn_node *);
ccn_node *DGNFTTinode(ccn_node *);
ccn_node *DGNFTTinode(ccn_node *);
ccn_node *DGPTast(ccn_node *);
ccn_node *DGPTast(ccn_node *);
ccn_node *DGPTipass(ccn_node *);
ccn_node *DGPTipass(ccn_node *);
ccn_node *DGTTast(ccn_node *);
ccn_node *DGTTast(ccn_node *);
ccn_node *DGTTitraversal(ccn_node *);
ccn_node *DGTTitraversal(ccn_node *);
ccn_node *DGSTast(ccn_node *);
ccn_node *DGSTast(ccn_node *);
ccn_node *DGSTinode(ccn_node *);
ccn_node *DGSTinode(ccn_node *);
ccn_node *DGAHitraversal(ccn_node *);
ccn_node *DGAHitraversal(ccn_node *);
ccn_node *DGAHipass(ccn_node *);
ccn_node *DGAHipass(ccn_node *);
ccn_node *DGAHast(ccn_node *);
ccn_node *DGAHast(ccn_node *);
ccn_node *DGAHinode(ccn_node *);
ccn_node *DGAHinode(ccn_node *);
ccn_node *DGAHid(ccn_node *);
ccn_node *DGAHid(ccn_node *);
ccn_node *DGAHsetliteral(ccn_node *);
ccn_node *DGAHsetliteral(ccn_node *);
ccn_node *DGTAast(ccn_node *);
ccn_node *DGTAast(ccn_node *);
ccn_node *DGTAinode(ccn_node *);
ccn_node *DGTAinode(ccn_node *);
struct ccn_node *DGCTid(struct ccn_node *node);
struct ccn_node *DGCTienum(struct ccn_node *node);
struct ccn_node *DGCTattribute_reference(struct ccn_node *node);
struct ccn_node *DGCTequation_dependency(struct ccn_node *node);
struct ccn_node *DGCTequation(struct ccn_node *node);
struct ccn_node *DGCTattribute(struct ccn_node *node);
struct ccn_node *DGCTitravdata(struct ccn_node *node);
struct ccn_node *DGCTsetoperation(struct ccn_node *node);
struct ccn_node *DGCTsetliteral(struct ccn_node *node);
struct ccn_node *DGCTsetreference(struct ccn_node *node);
struct ccn_node *DGCTste(struct ccn_node *node);
struct ccn_node *DGCTchild(struct ccn_node *node);
struct ccn_node *DGCTlifetime_range(struct ccn_node *node);
struct ccn_node *DGCTilifetime(struct ccn_node *node);
struct ccn_node *DGCTnodeset_child_entry(struct ccn_node *node);
struct ccn_node *DGCTinodeset(struct ccn_node *node);
struct ccn_node *DGCTinode(struct ccn_node *node);
struct ccn_node *DGCTipass(struct ccn_node *node);
struct ccn_node *DGCTitraversal(struct ccn_node *node);
struct ccn_node *DGCTiphase(struct ccn_node *node);
struct ccn_node *DGCTiactions(struct ccn_node *node);
struct ccn_node *DGCTast(struct ccn_node *node);
ccn_node *DGCCast(ccn_node *);
ccn_node *DGCCast(ccn_node *);
ccn_node *DGCCinode(ccn_node *);
ccn_node *DGCCinode(ccn_node *);
ccn_node *DGCCchild(ccn_node *);
ccn_node *DGCCchild(ccn_node *);
ccn_node *DGCCattribute(ccn_node *);
ccn_node *DGCCattribute(ccn_node *);
struct ccn_node *DGFTid(struct ccn_node *node);
struct ccn_node *DGFTienum(struct ccn_node *node);
struct ccn_node *DGFTattribute_reference(struct ccn_node *node);
struct ccn_node *DGFTequation_dependency(struct ccn_node *node);
struct ccn_node *DGFTequation(struct ccn_node *node);
struct ccn_node *DGFTattribute(struct ccn_node *node);
struct ccn_node *DGFTitravdata(struct ccn_node *node);
struct ccn_node *DGFTsetoperation(struct ccn_node *node);
struct ccn_node *DGFTsetliteral(struct ccn_node *node);
struct ccn_node *DGFTsetreference(struct ccn_node *node);
struct ccn_node *DGFTste(struct ccn_node *node);
struct ccn_node *DGFTchild(struct ccn_node *node);
struct ccn_node *DGFTlifetime_range(struct ccn_node *node);
struct ccn_node *DGFTilifetime(struct ccn_node *node);
struct ccn_node *DGFTnodeset_child_entry(struct ccn_node *node);
struct ccn_node *DGFTinodeset(struct ccn_node *node);
struct ccn_node *DGFTinode(struct ccn_node *node);
struct ccn_node *DGFTipass(struct ccn_node *node);
struct ccn_node *DGFTitraversal(struct ccn_node *node);
struct ccn_node *DGFTiphase(struct ccn_node *node);
struct ccn_node *DGFTiactions(struct ccn_node *node);
struct ccn_node *DGFTast(struct ccn_node *node);
struct ccn_node *DGCHTid(struct ccn_node *node);
struct ccn_node *DGCHTienum(struct ccn_node *node);
struct ccn_node *DGCHTattribute_reference(struct ccn_node *node);
struct ccn_node *DGCHTequation_dependency(struct ccn_node *node);
struct ccn_node *DGCHTequation(struct ccn_node *node);
struct ccn_node *DGCHTattribute(struct ccn_node *node);
struct ccn_node *DGCHTitravdata(struct ccn_node *node);
struct ccn_node *DGCHTsetoperation(struct ccn_node *node);
struct ccn_node *DGCHTsetliteral(struct ccn_node *node);
struct ccn_node *DGCHTsetreference(struct ccn_node *node);
struct ccn_node *DGCHTste(struct ccn_node *node);
struct ccn_node *DGCHTchild(struct ccn_node *node);
struct ccn_node *DGCHTlifetime_range(struct ccn_node *node);
struct ccn_node *DGCHTilifetime(struct ccn_node *node);
struct ccn_node *DGCHTnodeset_child_entry(struct ccn_node *node);
struct ccn_node *DGCHTinodeset(struct ccn_node *node);
struct ccn_node *DGCHTinode(struct ccn_node *node);
struct ccn_node *DGCHTipass(struct ccn_node *node);
struct ccn_node *DGCHTitraversal(struct ccn_node *node);
struct ccn_node *DGCHTiphase(struct ccn_node *node);
struct ccn_node *DGCHTiactions(struct ccn_node *node);
struct ccn_node *DGCHTast(struct ccn_node *node);
ccn_node *DGTDSast(ccn_node *);
ccn_node *DGTDSast(ccn_node *);
ccn_node *DGTDSitraversal(ccn_node *);
ccn_node *DGTDSitraversal(ccn_node *);
ccn_node *DGTDSitravdata(ccn_node *);
ccn_node *DGTDSitravdata(ccn_node *);
ccn_node *DGTDCitraversal(ccn_node *);
ccn_node *DGTDCitraversal(ccn_node *);
ccn_node *DGTDCitravdata(ccn_node *);
ccn_node *DGTDCitravdata(ccn_node *);
ccn_node *DGTDCHitraversal(ccn_node *);
ccn_node *DGTDCHitraversal(ccn_node *);
ccn_node *DGTDGitraversal(ccn_node *);
ccn_node *DGTDGitraversal(ccn_node *);
ccn_node *DGTDUast(ccn_node *);
ccn_node *DGTDUast(ccn_node *);
ccn_node *DGTDUitraversal(ccn_node *);
ccn_node *DGTDUitraversal(ccn_node *);
ccn_node *DGT_Fast(ccn_node *);
ccn_node *DGT_Fast(ccn_node *);
ccn_node *DGT_Finode(ccn_node *);
ccn_node *DGT_Finode(ccn_node *);
ccn_node *DGT_Fchild(ccn_node *);
ccn_node *DGT_Fchild(ccn_node *);
ccn_node *GAAiphase(ccn_node *);
ccn_node *GAAiphase(ccn_node *);
ccn_node *GAAipass(ccn_node *);
ccn_node *GAAipass(ccn_node *);
ccn_node *GAAast(ccn_node *);
ccn_node *GAAast(ccn_node *);
ccn_node *GAAitraversal(ccn_node *);
ccn_node *GAAitraversal(ccn_node *);
ccn_node *GAAHiphase(ccn_node *);
ccn_node *GAAHiphase(ccn_node *);
ccn_node *GAAHipass(ccn_node *);
ccn_node *GAAHipass(ccn_node *);
ccn_node *GAAHast(ccn_node *);
ccn_node *GAAHast(ccn_node *);
ccn_node *GAAHitraversal(ccn_node *);
ccn_node *GAAHitraversal(ccn_node *);
ccn_node *GITast(ccn_node *);
ccn_node *GITast(ccn_node *);
ccn_node *GITiphase(ccn_node *);
ccn_node *GITiphase(ccn_node *);
ccn_node *GITiactions(ccn_node *);
ccn_node *GITiactions(ccn_node *);
ccn_node *GGHast(ccn_node *);
ccn_node *GGHast(ccn_node *);
ccn_node *GGHiphase(ccn_node *);
ccn_node *GGHiphase(ccn_node *);
ccn_node *DGTDTast(ccn_node *);
ccn_node *DGTDTast(ccn_node *);
ccn_node *DGTDTitraversal(ccn_node *);
ccn_node *DGTDTitraversal(ccn_node *);
ccn_node *GDinode(ccn_node *);
ccn_node *GDinode(ccn_node *);
ccn_node *GDchild(ccn_node *);
ccn_node *GDchild(ccn_node *);
ccn_node *GDast(ccn_node *);
ccn_node *GDast(ccn_node *);
ccn_node *GDattribute(ccn_node *);
ccn_node *GDattribute(ccn_node *);
ccn_node *GDinodeset(ccn_node *);
ccn_node *GDinodeset(ccn_node *);
ccn_node *GDsetliteral(ccn_node *);
ccn_node *GDsetliteral(ccn_node *);
ccn_node *DUGTast(ccn_node *);
ccn_node *DUGTast(ccn_node *);
ccn_node *DUGTitraversal(ccn_node *);
ccn_node *DUGTitraversal(ccn_node *);
ccn_node *DUGTinode(ccn_node *);
ccn_node *DUGTinode(ccn_node *);
ccn_node *DUGTid(ccn_node *);
ccn_node *DUGTid(ccn_node *);
ccn_node *DUGPipass(ccn_node *);
ccn_node *DUGPipass(ccn_node *);
ccn_node *DUGPast(ccn_node *);
ccn_node *DUGPast(ccn_node *);
ccn_node *SGNSast(ccn_node *);
ccn_node *SGNSast(ccn_node *);
ccn_node *SGNSinode(ccn_node *);
ccn_node *SGNSinode(ccn_node *);
ccn_node *SGNSchild(ccn_node *);
ccn_node *SGNSchild(ccn_node *);
ccn_node *SGNSattribute(ccn_node *);
ccn_node *SGNSattribute(ccn_node *);
ccn_node *SGNSinodeset(ccn_node *);
ccn_node *SGNSinodeset(ccn_node *);
ccn_node *SGNSsetliteral(ccn_node *);
ccn_node *SGNSsetliteral(ccn_node *);
ccn_node *MITLast(ccn_node *);
ccn_node *MITLast(ccn_node *);
ccn_node *MITLinode(ccn_node *);
ccn_node *MITLinode(ccn_node *);
ccn_node *MITLiactions(ccn_node *);
ccn_node *MITLiactions(ccn_node *);
ccn_node *MITLilifetime(ccn_node *);
ccn_node *MITLilifetime(ccn_node *);
ccn_node *MITLid(ccn_node *);
ccn_node *MITLid(ccn_node *);
ccn_node *MITLlifetime_range(ccn_node *);
ccn_node *MITLlifetime_range(ccn_node *);
ccn_node *AITAast(ccn_node *);
ccn_node *AITAast(ccn_node *);
ccn_node *AITAiactions(ccn_node *);
ccn_node *AITAiactions(ccn_node *);
ccn_node *CLTast(ccn_node *);
ccn_node *CLTast(ccn_node *);
ccn_node *CLTattribute(ccn_node *);
ccn_node *CLTattribute(ccn_node *);
ccn_node *dynamicGenBaseNodeInit(ccn_node *);
ccn_node *dynamicSwitchToAstSource(ccn_node *);
ccn_node *dynamicGenBaseNode(ccn_node *);
ccn_node *dynamic_start_ast_header(ccn_node *);
ccn_node *dynamicSwitchTravData(ccn_node *);
ccn_node *dynamicStartTravData(ccn_node *);
ccn_node *dynamicStartTableGen(ccn_node *);
ccn_node *dynamicStartEnumHeader(ccn_node *);
ccn_node *doOpts(ccn_node *);
ccn_node *genDefines(ccn_node *);
ccn_node *SPscanAndParse(ccn_node *);
ccn_node *CPYid(ccn_node *);
ccn_node *DELid(ccn_node *);
ccn_node *CHKid(ccn_node *);
ccn_node *CPYienum(ccn_node *);
ccn_node *DELienum(ccn_node *);
ccn_node *CHKienum(ccn_node *);
ccn_node *CPYattribute_reference(ccn_node *);
ccn_node *DELattribute_reference(ccn_node *);
ccn_node *CHKattribute_reference(ccn_node *);
ccn_node *CPYequation_dependency(ccn_node *);
ccn_node *DELequation_dependency(ccn_node *);
ccn_node *CHKequation_dependency(ccn_node *);
ccn_node *CPYequation(ccn_node *);
ccn_node *DELequation(ccn_node *);
ccn_node *CHKequation(ccn_node *);
ccn_node *CPYattribute(ccn_node *);
ccn_node *DELattribute(ccn_node *);
ccn_node *CHKattribute(ccn_node *);
ccn_node *CPYitravdata(ccn_node *);
ccn_node *DELitravdata(ccn_node *);
ccn_node *CHKitravdata(ccn_node *);
ccn_node *CPYsetoperation(ccn_node *);
ccn_node *DELsetoperation(ccn_node *);
ccn_node *CHKsetoperation(ccn_node *);
ccn_node *CPYsetliteral(ccn_node *);
ccn_node *DELsetliteral(ccn_node *);
ccn_node *CHKsetliteral(ccn_node *);
ccn_node *CPYsetreference(ccn_node *);
ccn_node *DELsetreference(ccn_node *);
ccn_node *CHKsetreference(ccn_node *);
ccn_node *CPYste(ccn_node *);
ccn_node *DELste(ccn_node *);
ccn_node *CHKste(ccn_node *);
ccn_node *CPYchild(ccn_node *);
ccn_node *DELchild(ccn_node *);
ccn_node *CHKchild(ccn_node *);
ccn_node *CPYlifetime_range(ccn_node *);
ccn_node *DELlifetime_range(ccn_node *);
ccn_node *CHKlifetime_range(ccn_node *);
ccn_node *CPYilifetime(ccn_node *);
ccn_node *DELilifetime(ccn_node *);
ccn_node *CHKilifetime(ccn_node *);
ccn_node *CPYnodeset_child_entry(ccn_node *);
ccn_node *DELnodeset_child_entry(ccn_node *);
ccn_node *CHKnodeset_child_entry(ccn_node *);
ccn_node *CPYinodeset(ccn_node *);
ccn_node *DELinodeset(ccn_node *);
ccn_node *CHKinodeset(ccn_node *);
ccn_node *CPYinode(ccn_node *);
ccn_node *DELinode(ccn_node *);
ccn_node *CHKinode(ccn_node *);
ccn_node *CPYipass(ccn_node *);
ccn_node *DELipass(ccn_node *);
ccn_node *CHKipass(ccn_node *);
ccn_node *CPYitraversal(ccn_node *);
ccn_node *DELitraversal(ccn_node *);
ccn_node *CHKitraversal(ccn_node *);
ccn_node *CPYiphase(ccn_node *);
ccn_node *DELiphase(ccn_node *);
ccn_node *CHKiphase(ccn_node *);
ccn_node *CPYiactions(ccn_node *);
ccn_node *DELiactions(ccn_node *);
ccn_node *CHKiactions(ccn_node *);
ccn_node *CPYast(ccn_node *);
ccn_node *DELast(ccn_node *);
ccn_node *CHKast(ccn_node *);
