/* -----------------------------------------------------------------------------
HyGLEAM                       Internal Header-File
Package: evo                     File: evoInt.h               Version:    V1.0.1
Status : o.k.                   Autor: W.Jakob                 Datum: 10.07.2019
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------
Reunion of "evoIntg.h" and "evoInts.h".
-------------------------------------------------------------------------------- */


/* ======================= Package-globale Konstanten: ====================== */
#define SHOW_START       0               /* for "showOptRunHdr"               */
#define SHOW_RESTART     1               /* for "showOptRunHdr"               */
#define SHOW_RUNNING     2               /* for "showOptRunHdr"               */
#define SHOW_END         3               /* for "showOptRunHdr"               */

#define DEL_ALL_CHR_ENTRIES   TRUE       /* for the call of "clearChrList"    */
#define REPORT_CHR_ENTRIES   FALSE       /* for the call of "clearChrList"    */

/* --- values of job phase execution control variable "jobInterruptPhase" --- */
/* --- for external simualtion services (switch EXT_SIMU_SERV is set):    --- */
#define NULL_PHASE                   0         /* phase of no job execution   */
#define POP_INIT_GEN_PHASE          10         /* } phases of                 */
#define POP_INIT_EVAL_PHASE         11         /* } optimization              */
#define OFFSPRING_GEN_PHASE         20         /* } job                       */
#define GEN_OFFSPRING_EVAL_PHASE    21         /* } execution,                */
#define LHC_OF_POP_INIT_PHASE       30         /* } which may be              */
#define MEMETIC_PHASE               40         /* } interrupted               */


/* --------------------------- Allgemeine Limits: --------------------------- */
#define MAX_NB_HALBE  (MAX_ANZ_NACHBARN/2)    /* Haelfte einer Nachbarschaft  */
#define P_MIN            0.04        /* Mindestwahrscheinlickeit von p(LSV)   */

#define MAX_JOB_ZEIT  8553600L                              /* 99 Tage in sec */


/* ----------------------- Codes fuer Standard-LSVs: ----------------------- */
#define LSV_ROSENBROCK        TRUE
#define LSV_COMPLEX           FALSE

/* -------------------------- Codes fuer Adaption: ------------------------- */
#define KEINE_NKL             -1



/* ====================== Exportierte Typdeklarationen: ===================== */
#if !defined(MPI_PAR_VERS) && !defined(PAR_POP_VERS)  /*andernfalls in "evo.h"*/
  typedef struct DEME_TYPE
  {
     struct INDIVID_TYPE *nbPtr;        /* Zeiger auf den Nachbarn            */
  } DEME_TYPE;

  typedef struct INDIVID_TYPE
  {
    CHAIN_ELEM_PTR_TYPE  chain_ptr;     /* Zeiger auf die Kette               */
    INT                  guete,         /* Guete der Kette                    */
                         index;         /* Eigener Index im pop_feld          */
    DOUBLE               note;          /* Note der Kette                     */
    BOOL                 new_chain;     /* TRUE: Neugen od. durch Evo veränd. */
    struct DEME_TYPE     nachbarn[MAX_ANZ_NACHBARN];         /* Nachbarschaft */
    DOUBLE               deme_ham [MAX_NB_HALBE]; /*Dists d.linken Demehaelfte*/
  } INDIVID_TYPE;

  typedef struct CHR_LIST_TYPE
  {
     CHAIN_ELEM_PTR_TYPE *offspringList;    /*pointers to all generated offspr*/
     INT                  offspringAmount;  /* amount of offspring in list    */
     DOUBLE               worstDemeFitness, /* fitness of weakest deme member */
                          bestDemeFitness;  /* fitness of best deme member    */
  } CHR_LIST_TYPE;
#endif /* sequentielle Version mit sym. Ringnachbarschaft */



/* ======================== Package-globale Variable: ======================= */

/* ---------------------------- aus Modul "evo_steu": ----------------------- */
extern JOB_STAT_TYPE akjob_stat_dat,    /* Statistik eines AK-Jobs            */
                     rkjob_stat_dat;    /* Statistik eines RK-Jobs            */
extern INT           gen_len;           /* Laenge der besten AK eines OptJobs */

#if !defined(MPI_PAR_VERS) && !defined(GUI_MODE) && !defined(VISU_MODE)
  extern DOUBLE      bestJobNote,       /* Beste Note eines OptJobs           */
                     gen_note;          /* Beste Note der Pop.nach einer Gen. */
  extern LONG        abgelaufene_zeit;  /* Verbrauchte Populationszeit in Sek */
#endif /* weder Parallelversion noch GUI- oder VISU-Mode */


/* ---------------------- aus Modul "evo"/"parSimEvo": ---------------------- */
#ifndef MPI_PAR_VERS
  extern BOOL        accepted,          /* TRUE = Offspr.wird in 1 Gen.akzept.*/
                     accept_improved;   /* TRUE = Offspr. verbessert Deme     */
#endif /* no MPI_PAR_VERS */
#ifdef CEC_MBF
  extern LONG        aktFEsLim;         /* aktuelle FEs-UG zum Schreiben      */
  extern BOOL        fesT,              /* #F-Zeile f.Tausend ungeschrieben   */
                     fesZT,             /* #F-Zeile f.ZehnTsd. ungeschrieben  */
                     fesHT,             /* #F-Zeile f.HundertT. ungeschrieben */
                     fesMax,            /* CEC: #F-Zeile f.maxFEs ungeschr.   */
                     sZeile;            /* CEC: #S-Zeile ungeschrieben        */
#endif /* CEC_MBF */
#ifdef EXT_SIMU_SERV
  extern CHR_LIST_TYPE *chrList;        /*list of all offspring per generation*/
#endif /* EXT_SIMU_SERV */


/* -------------------------- aus Modul "evo_gsw": -------------------------- */
extern CHAIN_ELEM_PTR_TYPE  akt_ptr,         /* aktuelles Individuum          */
                            best_child_ptr,  /* bester Nachkomme pro Paarung  */
                            partner_ptr,     /* Partner des akt. Individuums  */
                            child_ptr,       /* 1. Offspring                  */
                            child2_ptr;      /* 2. Offspring (nur XO/Reko)    */
extern INT           mut_op_ctr [ANZ_PAAR_OPS]; /* Statistik: akzept. Offspr. */
extern DOUBLE        best_child_note,     /* Note dieses Nachkommens          */
                     max_fit,             /* aktueller Ranking Parameter      */
                     min_fitness,         /* f.Partnerwahl:  2.0-"max_fit"    */
                     fitness_delta,       /*        "max_fit" - "min_fitness" */
                     zielnote,            /* Vorgegebene Zielnote             */
                     lamarck_rate;        /* AK-Update-Rate fuer LSV          */
extern LONG          maxFEs;              /* Abbruchlimit f. Funktionsberechn.*/
extern INT           akt_gen_op,          /* Index der akt. gen. Operation    */
                     pop_size,            /* akt. Groesse des "pop_feld"      */
                     act_opt_strategy,    /* aktuelles Optimierungsverfahren  */
                     act_survival_rule,   /* aktuelle Ueberlebensregel        */
                     max_pop_index,       /* max.akt.Index im "pop_feld"      */
                     gut_anz,             /* Anz guter AKS in "gen0_neu"      */
                     toBeEvalCount,       /*counts newly generated chromosomes*/
                     jobInterruptPhase,   /*shows interrupted phase of jobExec*/
                     mut_op,              /* Init. nur f.NichtEvo relevant    */
                     lsv_strat,           /* LSV-Strategie allein und mit Evo */
                     anz_children;        /* Anzahl der offspring             */
#if !defined(GUI_MODE) && !defined(VISU_MODE)
  extern INT         no_accept_gen_counter,   /* GenAnz ohne OffsprAkzeptanz  */
                     no_deme_impr_gen_counter;/* GenAnz ohne DemeVerbesserung */
#endif /* weder GUI_MODE noch VISU_MODE */
extern BOOL          do_xo,               /* TRUE = HamAbst erlaubt XO/Reko   */
                     intrpt_at_gen_end,   /* TRUE, wenn Taste b.Generat.-Ende */
                     lsv_best_child,      /* TRUE, wenn LSV f.bestes Kind     */
                     lsv_all_children;    /* TRUE, wenn LSV f.alle Kinder     */
#ifdef MIT_LSV
  extern BOOL        gleamPur,            /* TRUE: reiner GLEAM-Job           */
                     gleam_adi,           /* TRUE: GLEAM_ASMA- od. _AMMA-Job  */
                     gleam_asma,          /* TRUE: GLEAM_ASMA-Job             */
                     gleam_amma;          /* TRUE: GLEAM_AMMA-Job             */
  extern CHAIN_ELEM_PTR_TYPE *child_ak_list;/* Liste der offspring            */
#endif /* MIT_LSV */
#if !defined(MPI_PAR_VERS)                          /* andernfalls in "evo.h" */
  extern INDIVID_TYPE *pop_feld;        /* enthaelt die Population (evo_gsw)  */
  extern INT         init_strat,        /* Initialisierungs-Strategie         */
                     max_nb_index;      /* Groesster Index: "anz_nachbarn"-1  */
#endif /* sequentielle Version mit sym. Ringnachbarschaft */
#if !defined(MPI_PAR_VERS) && !defined(GUI_MODE) && !defined(VISU_MODE)
  extern INT         akt_indiv;         /* "pop_feld"-Index des akt. Indivs   */
                                        /* andernfalls in "evo.h"             */
#endif /* sequentielle Version mit Ring UND kein GUI- und kein VISU-Mode */


/* ---------------------------- aus Modul "evo_anzg": ----------------------- */
extern char *logBuffer;                 /* big buffer for logfile output      */


#ifdef MIT_LSV
/* ---------------------------- aus Modul "rc_koppl": ----------------------- */
  extern INT     anz_compl_pkte;        /* Anzahl der Complex-Startpunkte     */
  extern CHAIN_ELEM_PTR_TYPE *ini_ptr;  /* Complex-Startpunkte                */


/* ------------------------- aus Modul "adapt_di": -------------------------- */
  extern char    lsv_adapt_speed_txt[10],      /*Text zur Adaptionsgeschw.    */
                 level_adapt_speed_txt[10];    /*Text zur Adaptionsgeschw.    */
  extern INT     nklassen_anz,                 /* Anzahl der Notenklassen     */
                 aktLSVAdaptSpeed,             /*aktuelle LSV-Adaptionsgeschw.*/
                 aktLevelAdaptSpeed,           /* akt. Level-Adaptionsgeschw. */
                 singleLsv          [NKL_MAX], /*nach Wettbew.verbl/einziges LSV*/
                 aktuellBeteilLsvAnz[NKL_MAX]; /*Anz.akt.noch beteiligter LSVs*/
  extern DOUBLE  nklassen_feld      [NKL_MAX], /* Notenklassen-Feld           */
                 nklassen_feld_neu  [NKL_MAX], /* Notenklassen-Feld neu       */
                 startLsvPMin;                 /*Initiale P-UG f.LSV-Abschaltg*/
  extern BOOL    lsv_adapt_aktiv    [NKL_MAX]; /* aktive LSV konkurrieren noch*/
#endif /* MIT_LSV */


/* -------------------------- aus Modul "go_gsw" : -------------------------- */
extern INT   go_anz,       /* Anzahl d.Eintraege gen. Operatoren in "go_list" */
             maxOffsprAmount;   /* maximal number of offspring per mating     */
extern BOOL  do_evo;       /* TRUE, wenn Evo und keine anderen OptStrat.      */
extern char *buf1,         /* Hilfspuffer, nur f. EvoOpt-Kontext (thread!)    */
            *buf2,         /* Hilfspuffer, nur f. EvoOpt-Kontext (thread!)    */
            *buf3;         /* Hilfspuffer, nur f. EvoOpt-Kontext (thread!)    */




/* ======================== Package-globale Routinen: ======================= */

/* ---------------------- Routinen aus Modul: evo_steu ---------------------- */
extern void   do_evo_jobs          (void);

extern void   clearEvoJobIntrpt    (void);

extern BOOL   init_evo_steu        (void);


/* ---------------------- Routinen aus Modul: evo / parSimEvo --------------- */
#ifndef MPI_PAR_VERS
  extern BOOL evo_1gen          (void);
#endif /* kein MPI_PAR_VERS */

extern BOOL pruef_und_speichere (CHAIN_ELEM_PTR_TYPE *c_ptr, 
                                 INT                  mut_index);

#ifdef MIT_LSV
  extern BOOL do_lsv            (BOOL  lamarck,
                                 INT   znr);
#endif /* MIT_LSV */

extern BOOL allocChrList        (void);

extern void clearInitialChrList (void);

extern BOOL clearChrList        (BOOL  delChr);

extern BOOL evaluateStartPop    (void);


/* -------------------- Routinen aus Modul: evo_gsw ------------------------- */
#ifndef GUI_MODE
  extern GSTATE keep_result_chain   (INT     index, 
                                     INT     ketten_art);
#endif /* kein GUI-Mode */
#if !defined(GUI_MODE) && !defined(VISU_MODE)
  extern INT    get_guete           (DOUBLE *note,
                                     INT     index);
#endif /* kein GUI- und kein VISU-Mode */
#if !defined(MPI_PAR_VERS) && !defined(PAR_POP_VERS) /* keine Parallelversion */
  extern BOOL   init_ep             (DOUBLE *best_note,
                                     INT     akt_pop_size,
                                     LONG    s_par);
#endif /* keine Parallelversion */
#ifndef MPI_PAR_VERS /* keine Parallelversion */
  extern void   loe_deme_ham_werte  (INT     indiv);
#endif /* sequentielle Version mit symmetrischer Ringnachbarschaft */

extern BOOL     do_g_operationen    (void);

#if !defined(PAR_POP_VERS) /* keine Version mit parallelen Populationen */
  extern void   save_ep             (void);
#endif /* keine Version mit parallelen Populationen */

extern GSTATE   store_res_ch        (INT    *lfd_nr,
                                     INT     idx,
                                     INT     ketten_art);

extern INT      get_best            (INT     anz_erg_ketten,
                                     BOOL    mit_anzg,
                                     GSTATE *ret_state);

extern void     store_in_evo_erg    (INT     number, 
                                     INT     guete, 
                                     DOUBLE  note,
                                     INT     ketten_art);

extern BOOL     check_for_lamarck   (DOUBLE  lamarck_rate);

extern DOUBLE  *partnerwahl         (INT     no,
                                     INT    *partnerIdx,
                                     DOUBLE *weakest_note,
                                     DOUBLE *best_note);

extern void     clear_mut_op_ctrs   (void);

extern void     meld_evo_crash      (INT         meld_nr,
                                     const char *mod_id,
                                     LONG        info);

extern BOOL     externalTermRequest (void);

extern BOOL     evo_init            (void);


/* ---------------------- Routinen aus Modul: evo_anzg ---------------------- */
#if defined(GLEAM_USR) && defined(TEXT_MODE)
  extern void prep_evo_param_anzg (void);

  extern void prepJobListDisplay  (void); 
#endif /* mit textuellem User-Interface */

extern void showOptRunHdr         (INT  showMode);

extern void jobLiLog              (void);

extern BOOL init_evo_anzg         (void);


/* ---------------------- Routinen aus Modul: lsv_steu ---------------------- */
#ifdef MIT_LSV
extern DOUBLE do_lsv_start      (CHAIN_ELEM_PTR_TYPE *ak_ptr,
                                 BOOL                 lamarck,
                                 GSTATE              *erg_status);

extern BOOL   init_lsv_steu     (void);


/* ---------------------- Routinen aus Modul: lsvkoppl ---------------------- */
extern DOUBLE do_rc_1start   (CHAIN_ELEM_PTR_TYPE *ak_ptr,
                              BOOL                 lamarck,
                              BOOL                 rosen,
                              GSTATE              *erg_status);

extern DOUBLE do_compl_start (INT                 *ini_idx,
                              BOOL                 lamarck,
                              GSTATE              *erg_status);

extern void   dat_killer     (void);


/* ---------------------- Routinen aus Modul: adapt_di ---------------------- */
extern void   reset_adi                (void);

extern void   set_actual_adi_lsv       (INT    *this_nkl,
                                        DOUBLE  akt_note);

extern INT    get_nklasse              (DOUBLE  note);

extern GSTATE parameterise_lsv_for_all (INT    *this_nkl,
                                        DOUBLE  akt_note);

extern GSTATE parameterise_lsv_for_best(INT     akt_nkl);

extern void   lsv_erg_fuer_adi         (INT     akt_nkl,
                                        DOUBLE  fg,
                                        DOUBLE  f_alt,
                                        LONG    aufwand);

extern DOUBLE get_lsv_p                (INT     akt_nkl);

extern void   all_erg_fuer_adi         (INT     akt_nkl,
                                        DOUBLE  fg,
                                        DOUBLE  f_alt,
                                        LONG    aufwand,
                                        DOUBLE  lsv_ist_rate);

extern void   get_adi_statistik_string (char   *str);

extern void   log_adi_ges_statistik    (void);

extern BOOL   init_adapt_di            (void);
#endif /* MIT_LSV */


/* ---------------------- Routinen aus Modul: go_gsw ------------------------ */
extern BOOL   calc_p_op           (INT    *gen_op_id,
                                   INT     op_index);

