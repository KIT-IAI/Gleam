/* -----------------------------------------------------------------------------
GLEAM/AE                      Package-Header-File
Package: appl                    File: appl.h                    Version: V1.2.0
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
-------------------------------------------------------------------------------- 

The package contains the interface for connecting other applications than those 
included in the GLEAM or HyGLEAM standard.

Modules of the Package:
    appl_if   interface module
    appl_sim  application specific simulator and package initialization
    appl_go   application specific genetic operators, if any
    appl_rep  application specific genetic repair, if any
    appl_gen  application specific generation of chromosomes, if any
    appl_fio  application specific I/O of additioal files, if any

-------------------------------------------------------------------------------- 
State : o.k.                   Author: W.Jakob                  Date: 23.10.2020
-------------------------------------------------------------------------------- */


/* =========================== Exported Constants: ========================== */
#define TSP_KENNUNG      "TSP/V1"   /* application class ID in the MOD file   */
#define OPAL_KENNUNG     "OPAL/V"   /* application class ID in the MOD file   */
#define ITEM_KENNUNG     "ITEM"     /* application class ID in the MOD file   */
#define GORBA_K_KENNUNG  "GORBA/K"  /* application class ID in the MOD file   */
#define GORBA_K2_KENNUNG "GORBA/K2" /* application class ID in the MOD file   */
#define GORBA_P_KENNUNG  "GORBA/P"  /* application class ID in the MOD file   */
#define SGS_KENNUNG      "SGS"      /* application class ID in the MOD file   */
#define PMS_KENNUNG      "PMS"      /* application class ID in the MOD file   */
#define TSP_APPL         201        /* application ID of "add_appl_code"      */
#define OPAL_APPL        301        /* application ID of "add_appl_code"      */
#define ITEM_APPL        401        /* application ID of "add_appl_code"      */
#define GORBA_KOMB_APPL  501        /* application ID of "add_appl_code"      */
#define GORBA_PERM_APPL  502        /* application ID of "add_appl_code"      */
#define GORBA_KOMB2_APPL 503        /* application ID of "add_appl_code"      */
#define SGS_APPL         601        /* application ID of "add_appl_code"      */
#define PMS_APPL         701        /* application ID of "add_appl_code"      */



/* ======================= Exported Type Declarations: ====================== */



/* =========================== Exported Variables: ========================== */
extern INT     add_appl_code;       /* ID of additional application           */
extern BOOL    mit_gen_rep_par,     /* TRUE: with control param.f.genetic rep.*/
               mitOptionPhaenoRep,  /* TRUE: with option of phenotypic repair */
               mitPhaenoRepair;     /* TRUE: with phenotypic repair           */
extern DOUBLE  gen_rep_par,         /* control parameter for genetic repair   */
               gen_rep_par_ug,      /* lower limit of gen.rep.control paramm. */
               gen_rep_par_og;      /* upper limit of gen.rep.control paramm. */
extern char   *gen_rep_par_query,   /* query text for gen.rep.control paramm. */
              *gen_rep_par_anzg,    /* output text of gen.rep.control paramm. */
              *gen_rep_par_kurzanzg;/*logfile output of gen.rep.control paramm*/


/* ============================ Exported Routines: ========================== */

/* ---------------------- Routines of Module: appl_if ----------------------- */
extern BOOL   read_add_appl         (INT                 *appl_id_code,
                                     char                *appl_id_str);

extern GSTATE set_appl_texts        (BOOL                *std_ak_anwndg,
                                     BOOL                *std_segm_gen_appl);
 

/* ---------------------- Routines of Module: appl_sim ---------------------- */
extern BOOL   do_appl_sim           (DOUBLE              *simu_erg_werte,
                                     CHAIN_ELEM_PTR_TYPE  ch_ptr,
                                     BOOL                 interaktiv,
                                     GSTATE              *erg_state);

extern GSTATE initApplSim           (void);

extern BOOL   appl_init             (BOOL                 steu_par);


/* ---------------------- Routines of Module: appl_go ----------------------- */
extern BOOL   do_appl_xo            (CHAIN_ELEM_PTR_TYPE *child1_ptr,
                                     CHAIN_ELEM_PTR_TYPE *child2_ptr,
                                     GSTATE              *ret_state,
                                     CHAIN_ELEM_PTR_TYPE  indiv_ptr,
                                     CHAIN_ELEM_PTR_TYPE  mate_ptr,
                                     INT                  op_code,
                                     BOOL                 do_xo);

extern BOOL   do_appl_mut           (CHAIN_ELEM_PTR_TYPE  child_ptr,
                                     INT                  op_code,
                                     GSTATE              *ret_state);


/* ---------------------- Routines of Module: appl_rep ---------------------- */
extern BOOL   add_appl_repair_1ch   (CHAIN_ELEM_PTR_TYPE *ch_ptr,
                                     GSTATE              *ret_state);

extern GSTATE add_repair_2_chs      (CHAIN_ELEM_PTR_TYPE  ch1_ptr,
                                     CHAIN_ELEM_PTR_TYPE  ch2_ptr);


/* ---------------------- Routines of Module: appl_gen ---------------------- */
extern BOOL   add_anw_ch_gen        (CHAIN_ELEM_PTR_TYPE *ch_ptr,
                                     GSTATE              *ret_state);

extern GSTATE add_gen_act_seq       (CHAIN_ELEM_PTR_TYPE *seq_ptr,
                                     CHAIN_ELEM_PTR_TYPE *seq_end_ptr,
                                     INT                  number);


/* ---------------------- Routines of Module: appl_fio ---------------------- */
extern BOOL   add_appl_load_afiles  (char *tmp_afspec[]);

extern BOOL   add_appl_load_1_afile (INT   index,
                                     char *fspec);

extern BOOL   add_appl_save_afiles  (INT  *znr,
                                     char *tmp_afspec[]);

extern BOOL   add_appl_save_1_afile (INT   index,
                                     char *fspec);

extern BOOL   add_appl_save_erg     (FILE *fdescr);

