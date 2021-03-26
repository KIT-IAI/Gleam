/* -----------------------------------------------------------------------------
GLEAM/AE                      Internal Header-File
Package: opal_v                  File: opalInt.h              Version:    V1.0.0
Status : o.k.                  Author: W.Jakob                  Date: 09.03.2001
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
-------------------------------------------------------------------------------- */


/* ======================== Package-global Constants: ======================= */
#define MAX_VERFAHREN_ANZ      12
#define MAX_INTERVALL_ANZ      10

/* ------------------------------- Module IDs: ------------------------------ */
#define OPAL_IO        "OPAL_opal_io"      /* module ID                       */
#define OPAL_BEWERT    "OPAL_opal_bew"     /* module ID                       */
#define OPAL_FIO_IF    "OPAL_fio_if"       /* module ID                       */

/* ------------------------------ Error Codes: ------------------------------ */
#define INCOMPATIBLE_STR_FILE 160
#define NO_ERG_AK             161
#define TOO_MUCH_INTERVALLS   162
#define ERROR_IN_PROCEDURE    163


/* ==================== Package-global Type Declarations: =================== */
typedef struct VERFAHRENS_TYP 
  {
     INT     anl_idx;            /* index of the facility used for production */
     INT     intervall_anz;      /* number of processing intervals of product */
     INT     interv_dauer     [MAX_INTERVALL_ANZ];  /* duration of intervals  */
     DOUBLE  interv_ma_bedarf [MAX_INTERVALL_ANZ];  /* MA demand of intervals */
     INT     ges_dauer;           /* total time of all batches of this product*/
     INT     soll_zeit;           /* due time of all batches of this product  */
     BOOL    mit_vorprod;         /* TRUE = product requires a pre-product    */
     INT     quell_verf_idx;      /* index of the pre-product                 */
     DOUBLE  min_kap,             /* minimum quantity of the pre-product      */
	     max_kap;             /* maximum quantity of the pre-product      */
     DOUBLE  soll_menge,          /* total target qty of all batches of prod. */
	     ges_menge;           /* total quantity actually produced         */
  } VERFAHRENS_TYP; 



/* ======================== Package-global Variables: ======================= */

/* ---------------------- Variables of Module: opal_io ---------------------- */
extern VERFAHRENS_TYP  verfahren [MAX_VERFAHREN_ANZ];    /* product list      */
extern DOUBLE          schichtspitzen_ug;  /* above this value recording of   */
                                           /* shift peaks                     */
extern INT             verf_anz;           /* number of products in VL-file   */




/* ======================== Package-global Routines: ======================== */

/* ---------------------- Routines from Module: opal_io --------------------- */
extern BOOL read_verf_li_file (char *fspec,
                               BOOL *bad_data);


/* ---------------------- Routines from Module: opal_bew -------------------- */
extern BOOL opal_bew_init     (void);

