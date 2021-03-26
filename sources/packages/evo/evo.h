/* -----------------------------------------------------------------------------
HyGLEAM                       Package-Header-File
Package: evo                     File: evo.h                     Version: V3.5.7
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
-------------------------------------------------------------------------------- 

Das Package enthaelt die genetischen Operatoren, Routinen zur Berechnung einer
Generation bzw. eines Regenzyklusses, zur Abarbeitung eines Optimierungsjobs und
zum Einlesen der Evolutionsparameter (EVO-File). Ausserdem das Menue zur Ver-
waltung von Evo-Jobs sowie Anzeigefunktionen.

Module des Packages:
    go_gsw    Basisroutinen und Einlesen des EVO-Files.
    go_rxo    Standard-Rekombination und -Crossover-Operatoren
    go_pmut   Parameter-Mutationen
    go_amut   Aktions-Mutationen
    go_smut   Segment-Mutationen

    rc_koppl  Ankopplung der beiden ITEM-LSVs Rosenbrock und Complex.
    lsv_steu  Datenobjekte d. (adaptiven) LSV-Steuerung u. zentraler LSV-Aufruf.
    adapt_di  Adaptive LSV-Auswahl und -Parametrierung sowie allP-Adaption.

    evo       Bearbeitung einer Generation, sequentielle Bewertung       } alter-
    parSimEvo Bearbeitung einer Generation, parallele Simulation/Bewer-  } na-
              tung durch ext.Simu.Services (ESS).                        } tiv
              LSVs derzeit noch nicht implementiert.
    evo_gsw   Evolutionsgrundsoftware fuer "evo" und "parSimEvo"
    evo_steu  Bearbeitung der Jobliste und eines Optimierungsjobs

    evo_anzg  Package-lokale Routinen fuer Statistik, Logging u. Anzeigen
    evo_cmen  Evo/Opt-Menue der TEXT_MODE-Variante

-------------------------------------------------------------------------------- 
Status : o.k.                   Autor: W.Jakob                 Datum: 11.08.2020
-------------------------------------------------------------------------------- */


/* ========================= Exportierte Konstanten: ======================== */
#define MAX_EVO_TYPEN        2    /* Ausbau RK-Evolution                      */
#define MAX_EVO_ERG         60    /* Groesse des Evo-Ergebnisfeldes           */
#define ANZ_PAAR_OPS        10    /* MaxAnz gen.Operator-Sequenzen pro Paarung*/

/* ------------------- Codes der Optimierungs-Verfahren: -------------------- */
#define GLEAM                0    /* traditionelles GLEAM ohne LSV            */
#define GLEAM_SMA            1    /* Simple Memetic/Single-Meme-Alg. mit GLEAM*/
#define GLEAM_ASMA           2    /* Adaptive Single-Meme-Alg. mit GLEAM      */
#define GLEAM_AMMA           3    /* Adaptive Multi-Meme-Alg. mit GLEAM       */
#define LSV                  4    /* LSV                                      */
#define LAST_OPT_VERF      LSV

/* -------------------------- Init-Strategie-Codes: ------------------------- */
#define NEU                  0    /* Gueltig: Alle Optimierungsverfahren      */
#define BEST                 1    /* Gueltig: Alle Optimierungsverfahren      */
#define MIX                  2    /* Gueltig: Alle ausser LSV                 */
#define BEST_NEU             3    /* Gueltig: Alle Optimierungsverfahren      */
#define LSV_NEU              4    /* Gueltig: Alle ausser LSV                 */
#define LSV_BEST             5    /* Gueltig: Alle ausser LSV                 */
#define FROM_FILE            6    /* Gueltig: Alle ausser LSV                 */
#define ONE_CHR              7    /* Gueltig: Nur LSV                         */


/* ---------------------- Codes der Ueberlebensregeln: ---------------------- */
#define ALWAYS               0    /* besten Nachkommen immer akzeptieren      */
#define LOCAL_LEAST          1    /* akzept.,wenn besser als lok.schlechtestes*/
#define BETTER_PARENT        2    /* akzeptiere, wenn besser als Elter        */
#define ALWAYS_ES            3    /* elitaeres ALWAYS                         */
#define LOCAL_LEAST_ES       4    /* elitaeres LOCAL_LEAST                    */

/* --------------- Mutations-Operatoren ("mut_data"-Indices): --------------- */
#define PAR_CHANGE_NEW       0        /* Parameter neu auswuerfeln            */
#define PAR_CHANGE_REL       1        /* Parameter um ein Delta veraendern    */
#define ACT_NEW_PARAM        2        /* Aktion neu parametrieren             */
#define SEGM_CHANGE_REL      3        /* Je 1 Param d.Aktionen d.Segments aend*/
#define SEGM_NEW_PARAM       4        /* Aktionen d.Segments neu parametrieren*/
#define ACT_EXCHANGE         5        /* Aktion gegen eine neue austauschen   */
#define ADD_NEW_ACT          6        /* Neue Aktion hinzufuegen              */
#define DOUBLE_ACT           7        /* Aktion verdoppeln                    */
#define DELETE_ACT           8        /* Aktion loeschen                      */
#define ACT_TRANSLOCATION    9        /* Aktion verschieben                   */
#define SEGM_EXCHANGE       10        /* Segment gegen eine neues austauschen */
#define DOUBLE_SEGM         11        /* Segment verdoppeln                   */
#define DELETE_SEGM         12        /* Segment loeschen                     */
#define SEGM_TRANSL         13        /* Segment verschieben                  */
#define SEGM_INVERSION      14        /* Segment invertieren                  */
#define INTEGR_NACHB_SEGM   15        /* Benachbarte Segmente integrieren     */
#define INTEGR_SEGM         16        /* Beliebeige Segmente integrieren      */
#define SCHIEB_SEGM_GRENZ   17        /* Verschiebe die Segmentgrenze         */
#define TEILE_SEGM          18        /* Teile Segment                        */
#define ADD_NEW_SEGM        19        /* Neues Segment                        */
#define PAR_CHANGE_SMALL    20        /* Param um ein kleines Delta veraendern*/
#define SEGM_CHANGE_SMALL   21        /* Kl.Aend.von je 1 Param d.Akt.d.Segm. */

#define LAST_GO    SEGM_CHANGE_SMALL  /* Anz. akt. def. Mutations-Operatoren  */

#define SEGM_CROSS_OVER   (LAST_GO+1) /* 22: Cross-Over durch Segmentaustausch*/
#define CROSSOVER         (LAST_GO+2) /* 23: Single-Point Cross-Over          */
#define RECOMB            (LAST_GO+3) /* 24: Rekombination (Multipoint X-Over)*/
#define OX_XO             (LAST_GO+4) /* 25: XO als 2-Punkt Crossover         */
#define OX_RECO           (LAST_GO+5) /* 26: XO als n-Punkt Recombination     */
#define PPX_XO            (LAST_GO+6) /* 27: PPX als 2-Punkt Crossover        */
#define PPX_RECO          (LAST_GO+7) /* 28: PPX als n-Punkt Recombination    */
#define LAST_XO           PPX_RECO    /* groesster Index der XO-Operatoren    */


/* -------------------------------- LSV-Daten: ------------------------------ */
#ifdef ROS_3PARAM
  #define LSV_PAR_MAX         3     /* MaxAnz Int/RealSteuerparams eines LSVs */
#else /* Standard (kein ROS_3PARAM) */
  #define LSV_PAR_MAX         2     /* MaxAnz Int/RealSteuerparams eines LSVs */
#endif /* Standard (kein ROS_3PARAM) */
#define LSV_BEZ_LEN          30     /* BezLen f. LSV- u.Steuerparameter-Namen */
#define LSV_KBEZ_LEN         12     /* BezLen f. LSV-Menue-Namen              */
#define LSV_JLBEZ_LEN         9     /* BezLen f. LSV-/Param-Namen in JobListe */
#define LSV_LEVEL_ANZ_MAX    10     /* MaxAnz d.Level eines LSV-Steuerparams  */
#define ROSEN_DEF_STEP_SIZE   0.1   /* Default fuer "step" bei LSV Rosenbrock */

#define KEIN_LSV             -1     /* Code fuer kein LSV                     */
#ifdef MIT_ROCO
  #define LSV_ANZ             2     /* Anz.impl.LSVs muss zu folg.Liste passen*/
  #define LSV_ROSEN           0     /* Code fuer Rosenbrock-LSV               */
  #define LSV_COMPL           1     /* Code fuer Complex-LSV                  */
#endif /* MIT_ROCO */

#ifdef MIT_GORBA_LSVS
  #define LSV_ANZ             4     /* Anz.impl.LSVs muss zu folg.Liste passen*/
  #define LSV_LVKG            0     /* LSV LinksVerschiebung, Klein -> Gross  */
  #define LSV_LVGK            1     /* LSV LinksVerschiebung, Gross -> Klein  */
  #define LSV_RV              2     /* LSV RechtsVerschiebung                 */
  #define LSV_RAS             3     /* LSV RAS-veraenderung                   */
#endif /* MIT_GORBA_LSVS */

#define NKL_MAX               6     /* Maximale Anzahl der Notenklassen       */
#define LEVEL_MAX            10     /* Maximale Anzahl der Level              */

/* -------------------------- Codes fuer Adaption: ------------------------- */
#define ADAPT_SPEED_SCHNELL   0
#define ADAPT_SPEED_MITTEL    1
#define ADAPT_SPEED_LANGSAM   2



/* ====================== Exportierte Typdeklarationen: ===================== */
typedef struct 
  { DOUBLE  ziel_note,          /* Notenzielwert zur beendigung eines Jobs    */
            rank_param,         /* Ranking-Parameter (1.0 .. 2.0)             */
            lamarck_rate,       /* AK-Update-Rate bei Verbesserungen durch LSV*/
            xo_min_ham_dist;    /* Mindest-HamDist zur Anwendung von XO/Reko  */
    LONG    job_zeit,           /* Zeitlimit eines Jobs in Sekunden           */
            maxFEs,             /* CEC: Abbruchlimit f.FunktionsEvaluierungen */
            strat_par;          /* Parameter der Init-Strategie               */
    INT     ketten_art,         /* Zu optimierende Kettenart. Z.Zt. immer AK  */
            popel_size,         /* Populationsgroesse                         */
            generationen,       /* MaxAnz Generationen einer Population       */
            chrNumber,          /* used for LHC jobs only                     */
            gen_no_accept,      /* MaxAnzGen ohne Akzeptanz in allen Demes    */
            genno_deme_impr,    /* MaxAnzGen ohne Verbesserung in allen Demes */
            anz_erg_ketten,     /* Anzahl der Ergebnis-AKs                    */
            save_rate,          /* PopRettung alle save_rate Generationen     */
            init_strat,         /* Init-Strategie fuer die Population         */
            survival_rule,      /* Ueberlebensregel der Nachkommen            */
            opt_strategy,       /* Optimierungsstrategie des Jobs             */
            lsv_strategy;       /* LSV fuer Init und als Meme: Index in lsv[] */
    BOOL    z_init,             /* TRUE, wenn Init des Zufallszahlengenerators*/
            deaktiviert,        /* TRUE, wenn Job deaktiviert wurde           */
            lsv_all_offspr;     /* LSV f. alle (TRUE) oder besten Offspring   */
  } EVO_WS_TYPE;


typedef struct 
  {
    struct 
      {
         DOUBLE  note; 
	 SHORT   guete;
         INT     lfdnr;
      } evo_erg_feld [MAX_EVO_ERG];
    INT last_evo_index;
  } EVO_ERG_TYP;


typedef struct 
  { DOUBLE  best_note;
    INT     job_anz, 
            res_ch_sum, 
            popel_anz;
    LONG    jobli_indivs,
            zeit_sum,
            gen_anz;
  } GES_STAT_TYPE;


typedef struct  
  { INT     best_ch_len,
            anz_res_chs;
    DOUBLE  best_ch_note;
  } JOB_STAT_TYPE;


#if defined(MPI_PAR_VERS) || defined(PAR_POP_VERS)  /* andernfalls in "evoInt.h" */
/* ====================== Exportierte Typdeklarationen: ===================== */
typedef struct DEME_TYPE    /* Die Reihenfolge dieser Feldelemente wird durch */
  {                         /* die Sortierung von "partnerwahl" geaendert.    */
     struct INDIVID_TYPE *nbPtr;        /* Zeiger auf den Nachbarn            */
     INT                  hamDistIdx;   /* eigener oder Nachbar-deme_ham-Idx  */
  } DEME_TYPE;

typedef struct INDIVID_TYPE
  {
     CHAIN_ELEM_PTR_TYPE  chain_ptr;     /* Zeiger auf die Kette              */
     INT                  guete,         /* Guete der Kette                   */
                          index;         /* Eigener Index im pop_feld         */
     DOUBLE               note;          /* Note der Kette                    */
     BOOL                 new_chain;     /* TRUE: Neugen od.durch Evo veraend.*/
     struct DEME_TYPE     nachbarn[MAX_ANZ_NACHBARN];  /* Deme zum Sortieren  */
     DOUBLE               deme_ham [MAX_ANZ_NACHBARN]; /* Dists d.ganzen Deme */
     INT                  staticHDIdx[MAX_ANZ_NACHBARN];  /* statisches Deme  */
  } INDIVID_TYPE;

  typedef struct CHR_LIST_TYPE
  {
     CHAIN_ELEM_PTR_TYPE *offspringList;    /*pointers to all generated offspr*/
     INT                  offspringAmount;  /* amount of offspring in list    */
     DOUBLE               worstDemeFitness, /* fitness of weakest deme member */
                          bestDemeFitness;  /* fitness of best deme member    */
  } CHR_LIST_TYPE;
#endif /* eine der Parallelversionen */


typedef DOUBLE *DF_TYP;
typedef LONG   *LF_TYP;
typedef INT    *IF_TYP;
typedef DF_TYP *DFF_TYP;
typedef LF_TYP *LFF_TYP;
typedef IF_TYP *IFF_TYP;

typedef struct LSVDescrType {
   char    lsvName[LSV_BEZ_LEN];       /* LSV-Bezeichnung                     */
   char    lsvMenueName[LSV_KBEZ_LEN]; /* LSV-Menuebezeichnung,max 10 Zeichen */
   char    lsvJobLiName[LSV_JLBEZ_LEN];/* LSV-Bezeichnung f.Jobliste, max 6 Z.*/
   char    lsvParName[LSV_PAR_MAX][LSV_BEZ_LEN],      /* Namen d.LSV-Steuerpa-*/
           lsvParJL1Name[LSV_PAR_MAX][LSV_JLBEZ_LEN], /* meter in Langform u. */
           lsvParJL2Name[LSV_PAR_MAX][LSV_JLBEZ_LEN]; /* als JobListen-Koepfe */
   char    lsvCh;                      /* Menue-Character des LSVs            */
   BOOL    impl,                       /* LSV in aktueller Vers.implementiert */
           aktiv;                      /* Aktiv im aktuellen Genmodell-Kontext*/
   INT     lsvParAnz;                  /* Anzahl der LSV-Steuerparameter      */
   DOUBLE  lsvPar[LSV_PAR_MAX];        /* Steuerparameter des LSVs f. LSV/SMA */
   /* ------- Daten fuer adaptiv gesteuerten Meme-Einsatz (AMSA/AMMA): ------ */
   BOOL    nklValid[NKL_MAX];         /* LSV-Par/allP-Level dieser NKL gueltig*/
   DFF_TYP parLevelP[LSV_PAR_MAX],    /* LSV-Par: Level-Einzel-Wahrscheinl.   */
           parFgSum[LSV_PAR_MAX];     /* LSV-Par: FG-Summe des aktuellen Lev. */
   LFF_TYP parEvalSum[LSV_PAR_MAX];   /* LSV-Par: Eval-Summe d.aktuellen Lev. */
   IFF_TYP parCtr[LSV_PAR_MAX];       /* LSV-Par: Cntr d.Anw.d.aktuellen Lev. */
   DF_TYP  parLevelVal[LSV_PAR_MAX],  /* LSV-Par: Level-Werte d.Steuerparams  */
           par2ndLevPSum[LSV_PAR_MAX];/* LSV-Par: P-Summe des 1.u.2.akt.Levels*/
   IF_TYP  parAllCtr[LSV_PAR_MAX],    /* LSV-Par: Cntr d.Anw.aller aktiven L. */
           par1stActLev[LSV_PAR_MAX]; /* LSV-Par: Index des 1.aktiven Levels  */
   INT     parLastLevel[LSV_PAR_MAX], /* LSV-Par: zuletzt gewaehltes Level    */
           parStartLev[LSV_PAR_MAX],  /* LSV-Par: initiales Startlevel        */
           parLevelAnz[LSV_PAR_MAX];  /* LSV-Par: Anzahl der Level            */
   DOUBLE  parIniP1stLev[LSV_PAR_MAX],/* LSV-Par: initiale Wahrsch.d.1.Levels */
           parIniP2ndLev[LSV_PAR_MAX];/* LSV-Par: initiale Wahrsch.d.2.Levels */
   IFF_TYP statParBestLSum[LSV_PAR_MAX];/* summiert d.besten Level am Jobende */
   DF_TYP  allPLevelP[NKL_MAX],       /* all-P: Level-Einzel-Wahrscheinlichk. */
           allPFgSum[NKL_MAX];        /* all-P: FG-Summe des aktuellen Levels */
   LF_TYP  allPEvalSum[NKL_MAX];      /* all-P: Eval-Summe d.aktuellen Levels */
   IF_TYP  allPCtr[NKL_MAX];          /* all-P: Cntr d.Anw.d.aktuellen Levels */
   INT     allPAllCtr[NKL_MAX],       /* all-P: Cntr d.Anw.aller aktiven Lev. */
           allP1stActLev[NKL_MAX];    /* all-P: Index des 1.aktiven Levels    */
   INT     allPStartLevel;            /* all-P: initiales Startlevel          */
   DOUBLE  allP2ndLevPSum[NKL_MAX],   /* all-P: P-Summe des 1.u.2.akt.Levels  */
           allPIniP1stLev,            /* all-P: initiale Wahrsch.d. 1.Levels  */
           allPIniP2ndLev;            /* all-P: initiale Wahrsch.d. 2.Levels  */
   IF_TYP  statAllPBestLSum[NKL_MAX]; /* all-P: summ.d.besten Level am Jobende*/
   INT     statAllPJobAnz[NKL_MAX];   /* all-P: zaehlt allP-Jobs pro NKlasse  */
   IF_TYP  statLsvPNklHisto[NKL_MAX]; /* LSV-Ausw: fuer Histogramm von p(LSV) */
   DOUBLE  statLsvPNklSum[NKL_MAX],   /* LSV-Ausw: summiert p(LSV) am Jobende */
           lsvP[NKL_MAX],             /* LSV-Ausw: LSV-Wahrscheinlichkeit (p) */
           lsvPSel[NKL_MAX],          /* LSV-Ausw: LSV-Summen-Wahrscheinlich. */
           lsvFgSum[NKL_MAX];         /* LSV-Ausw: Fitnessgewinn-Summe d.LSVs */
   LONG    lsvEvalSum[NKL_MAX];       /* LSV-Ausw: Evaluationen-Summe d. LSVs */
   INT     lsvCtr[NKL_MAX],           /* LSV-Ausw: Cntr d.Anwendungen des LSVs*/
           lsvFinCtr[NKL_MAX];        /* LSV-Ausw: Zaehler f. p(LSV)<lsvPMin  */
   BOOL    lsvOn[NKL_MAX],            /* LSV-Ausw: p(LSV) > 0 fuer diese NKl  */
           abzuschalten[NKL_MAX];     /* LSV-Ausw: LSV nachtraegl.abschalten! */
   DOUBLE  lsvStartP,                 /* LSV-Ausw: Initiale Wahrsch. des LSVs */
           neuLsvStartP;              /* LSV-Ausw: neuer Wert, f. "evo_cmen"  */
} LSVDescrType;



/* ========================== Exportierte Variable: ========================= */

/* ---------------------------- aus Modul "evo_steu": ----------------------- */
extern EVO_WS_TYPE   *evo_ws;           /* Optimierungs-Job-Warteschlange     */
extern JOB_STAT_TYPE  ak_jobli_stat_dat,/* Gesamtstat. aller Jobs einer Jobli */
                      rk_jobli_stat_dat;/* Gesamtstat. aller Jobs einer Jobli */
extern GES_STAT_TYPE  ges_ak_stat_dat,  /* Gesamtstat. aller Jobs aller Joblis*/
                      ges_rk_stat_dat;  /* Gesamtstat. aller Jobs aller Joblis*/
extern INT            free_evo_index,   /* Erstes freies Element in d.OptJobli*/
                      first_evo_item,   /* Aktueller (erster) Job der OptJobli*/
                      last_psize;       /* Populationsgroesse des letzten Jobs*/
extern BOOL           evo_done,         /* TRUE, wenn Opt.seit letzt.EXP-Laden*/
                      evo_aktiv,        /* TRUE, wenn Optimierung laeuft      */
                      job_ende_ton;     /* TRUE = Tonmeldung bei pos. Jobende */
#if defined(GUI_MODE) || defined(VISU_MODE) 
  extern BOOL         job_aktiv;        /* TRUE, keine JobEndeBeding. erfuellt*/
#endif /* GUI_MODE oder VISU_MODE */
#if defined(MPI_PAR_VERS) || defined(GUI_MODE) || defined(VISU_MODE)
  extern DOUBLE       bestJobNote,      /* Beste Note eines OptJobs           */
                      gen_note;         /* Beste Note der Pop.nach einer Gen. */
  extern LONG         abgelaufene_zeit; /* Verbrauchte Populationszeit in Sek */
#endif /* Parallelversion oder GUI-Mode oder VISU-Mode */


/* ---------------------------- aus Modul "evo": ---------------------------- */
#ifdef CEC_MBF
  extern DOUBLE      cec_accuracy_val,  /* CEC-MBFs: Genauigkeitsgrenze       */
                     cec_fopt;          /* CEC-MBFs: Funktionsoptimum         */
#endif /* CEC_MBF */
#ifdef MPI_PAR_VERS
  extern BOOL        accepted,          /* TRUE = Offspr.wird in 1 Gen.akzept.*/
                     accept_improved;   /* TRUE = Offspr. verbessert Deme     */
#endif /* MPI_PAR_VERS */


/* -------------------------- aus Modul "evo_gsw": -------------------------- */
extern EVO_ERG_TYP  *last_evo_erg;      /* Ergebnisdaten der letzten EvoJobs  */
extern LONG          indiv_ctr,         /* Zaehlt d.Funtionsevaluat.inkl.Init */
                     evalLimit;         /* Vorgabe eines Evaluationsbudgets   */
extern DOUBLE        xo_min_ham_dist,   /* akt.min.Hamming-Abst. fuer Reko/XO */
                     def_max_fit,       /* Default f. Ranking-Param "max_fit" */
                     def_xo_min_ham_dist,/* Def. min.Hamming-Abst.f.Reko/XO   */
                     defTargetFitness,  /* default for target fitness of a job*/
                     def_lamarck_rate,  /* Def.f.Lamarck-Param lamarck_rate   */
                     oldDistribFrac;    /* Anteil d.alten LSV-Vert.an d.neuen */
extern INT           evo_tst_frequ,     /* Frequ.d.EvoAbbruchtests bei Engine */
                     anz_nachbarn,      /* aktuelle Nachbarschaftsgroesse     */
                     anz_nb_halbe,      /* Haelfte von "anz_nachbarn"         */
                     default_survival_rule, /* Default-Ueberlebensregel       */
                     defInitStrat,      /* default strategy for start popul.  */
                     defInitStratPar,   /* default for init.strategy parameter*/
                     def_psize,         /* Default-Populationsgroesse         */
                     defGenLimit,       /* default for generation limit       */
                     defGAcLimit,       /* def.: limit of gen. w/o acceptance */
                     defGDI_Limit,      /* def.: limit of gen. w/o deme impr. */
                     defTimeLimit,      /* default for job time limit in sec. */
                     defEvalLimit,      /* def.: limit of evaluations         */
                     defResultChrs,     /* default of result chromosome number*/
                     def_opt_strat,     /* Default-Optimierungs-Verfahren     */
                     def_lsv_strat,     /* Default-LSV solo und mit EVO       */
                     save_rate_def,     /* Defaultwert fuer die Evo-Save-Rate */
                     selectedChrAmount, /* PAR_POP_VERS:#initial.selected chr.*/
                     gen_counter;       /* Generationszaehler                 */
extern BOOL          with_evo_term_tst, /* TRUE: Mit EvoAbbruchtests d.Engine */
                     initZufGen,        /* TRUE: Init. des Zufallszahlengen.  */
                     withAllImprovement,/* TRUE: LHC-opt for all offspring    */
                     prepareForEvo,     /* TRUE: not PAR_POP_VERS initializer */
                     mem_out;           /* TRUE = Abbruch wegen Speichermangel*/
#if defined(GUI_MODE) || defined(VISU_MODE) 
  extern INT         no_accept_gen_counter,   /* GenAnz ohne OffsprAkzeptanz  */
                     no_deme_impr_gen_counter;/* GenAnz ohne DemeVerbesserung */
#endif /* GUI_MODE or VISU_MODE */
#if defined(MPI_PAR_VERS) || defined(PAR_POP_VERS)
  extern INDIVID_TYPE *pop_feld;        /* enthaelt die Population (evo_gsw)  */
  extern INT           init_strat,      /* Initialisierungs-Strategie         */
                       max_nb_index;    /* Groesster Index: "anz_nachbarn"-1  */
#endif /* eine der beiden Parallelversionen */
#if defined(MPI_PAR_VERS) || defined(GUI_MODE) || defined(VISU_MODE)
  extern INT           akt_indiv;       /* "pop_feld"-Index des akt. Indivs   */
#endif /* Parallelversion mit allg. Nachbarschaft od. GUI-Mode od. VISU_MODE  */



#ifdef MIT_LSV
/* ---------------------------- aus Modul "lsv_steu": ----------------------- */
  extern LSVDescrType *lsv;            /* Liste aller implementierten LSVs    */
  extern INT         lsv_anz;          /* Anzahl aktiver LSVs                 */
  extern INT         aktive_lsv[LSV_ANZ]; /* Im akt.MOD-Kontext gueltige LSVs */
  extern INT         allPLevelAnz;     /* all-P: Anzahl der Level             */
  extern DOUBLE      allPLevelVal[LEVEL_MAX]; /* all-P: LSV-Wahrsch.f. "all"  */


/* ---------------------------- aus Modul "adapt_di": ----------------------- */
  extern INT         lsv_einzel_limit[NKL_MAX], /* Mindestanw. einzelnes LSV  */
                     lsv_gesamt_limit[NKL_MAX], /* Mindestanw. aller LSVs     */
                     lsv_nix_anwendung_limit,/* MaxAnz Adapt mit nur 1 LSV    */
                     level_einzel_limit,     /* Mindestanw.eines einzeln.Level*/
                     level_gesamt_limit;     /* Mindestanw. aller Level       */
#endif /* MIT_LSV */


/* ---------------------------- aus Modul "evo_anzg": ----------------------- */
extern BOOL          initFileReloaded;  /* TRUE, if an init.file was reloaded */
#ifndef GLEAM_USR
  extern BOOL        detailed_log;  /* Mit detailierten Logfile-Eintraegen    */
#endif /* kein GLEAM_USR, also Engine */




/* ========================== Exportierte Routinen: ========================= */

/* ---------------------- Routinen aus Modul: evo_cmen ---------------------- */
extern void evo_cmenue       (void);


/* ---------------------- Routinen aus Modul: evo_steu ---------------------- */
extern BOOL do_job_end        (void);

extern void do_evo_jobs       (void);

#if defined(GUI_MODE) || defined(VISU_MODE)
  extern BOOL do_1_evo_job    (void);
#endif /* GUI_MODE oder VISU_MODE */

extern void clear_evo_results (GES_STAT_TYPE *stat_dat);


/* ---------------------- Routinen aus Modul: evo / parSimEvo --------------- */
#ifdef MPI_PAR_VERS
  extern BOOL evo_1gen          (void);
#endif /* MPI_PAR_VERS */


/* ---------------------- Routinen aus Modul: evo_gsw ----------------------- */
extern void     loesche_evo_erg   (void);

extern void     dumpPopFeld       (const char *buf,
                                   INT         popSize);

extern DOUBLE   getHamDist        (INT         indivIdx,
                                   INT         partnerIdx);

extern GSTATE   del_ep            (void);

#if defined(GUI_MODE)
  extern GSTATE keep_result_chain (INT     index, 
                                   INT     ketten_art);
#endif /* GUI_MODE */
#if defined(GUI_MODE) || defined(VISU_MODE)
  extern INT    get_guete         (DOUBLE *note,
                                   INT     index);
#endif /* GUI_MODE oder VISU_MODE */
#if defined(MPI_PAR_VERS) || defined(PAR_POP_VERS)
  extern BOOL   init_ep           (DOUBLE *best_note,
                                   INT     akt_pop_size,
                                   LONG    s_par);
#endif /* MPI_PAR_VERS oder PAR_POP_VERS */
#if defined(MPI_PAR_VERS)
  extern void   initSubPop        (INT     pSize,
                                   INT     optVerf,
		                   INT     survivalRule,
		                   INT     lsvCode,
		                   DOUBLE  rankParam,
	                           BOOL    mitAllVerb);

  extern BOOL   ch_gen            (INT     indivIdx);
#endif /* MPI_PAR_VERS */
#if defined(PAR_POP_VERS) /* Version mit parallelen Populationen */
  extern void   save_ep             (void);
#endif /* Version mit parallelen Populationen */



#ifdef MIT_LSV
/* ---------------------- Routinen aus Modul: lsvkoppl ---------------------- */
  extern BOOL   prepare_rc_koppl    (INT     i_par_ges_anz, 
                                     INT     r_par_ges_anz);

  extern GSTATE set_rc_limits       (INT     new_c_max_iter,
                                     INT     new_r_max_iter,
                                     DOUBLE  error_goal,
                                     DOUBLE  step_size);


/* ---------------------- Routinen aus Modul: lsv_steu ---------------------- */
  extern BOOL   activate_lsv        (INT     lsvIdx);

  extern BOOL   prep_active_lsvs    (void);

  extern INT    get_lsv_idx         (INT     idx);

  extern BOOL   parameterise_lsv    (INT     lsv_idx);

  extern GSTATE parameteriseAllLSVs (void);


/* ---------------------- Routinen aus Modul: adapt_di ---------------------- */
  extern INT    getActNKlAnz        (void);

  extern void   init_adi            (INT     lsvAdaptSpeed,
                                     INT     levelAdaptSpeed);

  extern void   setLSVAdaptSpeed    (INT     adaptSpeed,
                                     INT     aktNkl);

  extern void   setLevelAdaptSpeed  (INT     adaptSpeed);

  extern void   getAdaptSpeeds      (INT    *lsvAdaptSpeed,
                                     INT    *levelAdaptSpeed);

  extern void   set_std_nkl         (INT     new_nkl_anz);
#endif /* MIT_LSV */


/* ---------------------- Routinen aus Modul: evo_anzg ---------------------- */
#if defined(GLEAM_USR) && defined(GUI_MODE)
  extern void prep_evo_param_anzg (void);

  extern void prepJobListDisplay  (void);
#endif /* mit grafischem User-Interface */

extern void showActJobDataOnProcessingStart (void);

extern void showActJobDataOnContinuation    (void);

extern void showGenResults                  (void); 

extern void optJobEndStatistics             (void);

#ifdef MPI_PAR_VERS
  extern void   logOptJobData     (INT  initStrat,
		 	           INT  genCtr,
		 	           INT  gdvCtr,
			           INT  gakCtr);
#endif /* MPI_PAR_VERS */



/* ---------------------- Routinen aus Modul: go_rxo ------------------------ */
extern BOOL abschn_cross_over (CHAIN_ELEM_PTR_TYPE *res_ptr1,
                               CHAIN_ELEM_PTR_TYPE *res_ptr2,
                               CHAIN_ELEM_PTR_TYPE  org_ptr1,
                               CHAIN_ELEM_PTR_TYPE  org_ptr2,
                               GSTATE              *ret_state);

extern BOOL cross_over        (CHAIN_ELEM_PTR_TYPE *res_ptr1,
                               CHAIN_ELEM_PTR_TYPE *res_ptr2,
                               CHAIN_ELEM_PTR_TYPE  org_ptr1,
                               CHAIN_ELEM_PTR_TYPE  org_ptr2,
                               GSTATE              *ret_state);

extern BOOL rekombination     (CHAIN_ELEM_PTR_TYPE *res_ptr1,
                               CHAIN_ELEM_PTR_TYPE *res_ptr2,
                               CHAIN_ELEM_PTR_TYPE  org_ptr1,
                               CHAIN_ELEM_PTR_TYPE  org_ptr2,
                               GSTATE              *ret_state);

extern BOOL orderRekoXO       (CHAIN_ELEM_PTR_TYPE *res_ptr1,
                               CHAIN_ELEM_PTR_TYPE *res_ptr2,
                               CHAIN_ELEM_PTR_TYPE  org_ptr1,
                               CHAIN_ELEM_PTR_TYPE  org_ptr2,
                               INT                  genop,
                               GSTATE              *ret_state);

extern BOOL rxo_init          (void);


/* ---------------------- Routinen aus Modul: go_pmut ----------------------- */
extern GSTATE akt_par_aend  (BOOL                *mut_done,
                             INT                  mod_kind,
                             CHAIN_ELEM_PTR_TYPE  chain_ptr);

extern GSTATE akt_par_neu   (CHAIN_ELEM_PTR_TYPE  chain_ptr);

extern GSTATE akt_neu       (CHAIN_ELEM_PTR_TYPE  chain_ptr);

extern GSTATE segm_par_aend (BOOL                *mut_done,
                             INT                  mod_kind,
                             CHAIN_ELEM_PTR_TYPE  chain_ptr);

extern GSTATE segm_neu      (CHAIN_ELEM_PTR_TYPE  chain_ptr);


/* ---------------------- Routinen aus Modul: go_amut ----------------------- */
extern GSTATE plus_neue_aktion  (CHAIN_ELEM_PTR_TYPE  chain_ptr);

extern GSTATE aktions_austausch (CHAIN_ELEM_PTR_TYPE  chain_ptr);

extern GSTATE aktion_verdoppeln (CHAIN_ELEM_PTR_TYPE  chain_ptr);

extern GSTATE aktion_loeschen   (CHAIN_ELEM_PTR_TYPE  chain_ptr);

extern GSTATE aktions_transl    (CHAIN_ELEM_PTR_TYPE  chain_ptr);


/* ---------------------- Routinen aus Modul: go_smut ----------------------- */
extern GSTATE abschn_austausch    (CHAIN_ELEM_PTR_TYPE  chain_ptr);

extern GSTATE abschn_verdoppeln   (CHAIN_ELEM_PTR_TYPE  chain_ptr);

extern GSTATE abschn_loeschen     (CHAIN_ELEM_PTR_TYPE  chain_ptr);

extern GSTATE abschn_transl       (CHAIN_ELEM_PTR_TYPE  chain_ptr);

extern GSTATE abschn_inversion    (CHAIN_ELEM_PTR_TYPE  chain_ptr);

extern GSTATE nachb_segm_integr   (CHAIN_ELEM_PTR_TYPE  chain_ptr);

extern GSTATE segm_integr         (CHAIN_ELEM_PTR_TYPE  chain_ptr);

extern GSTATE schiebe_segm_grenze (BOOL                *mut_done,
                                   CHAIN_ELEM_PTR_TYPE  chain_ptr);

extern GSTATE segm_teilung        (BOOL                *mut_done,
                                   CHAIN_ELEM_PTR_TYPE  chain_ptr);

extern GSTATE add_new_segm        (CHAIN_ELEM_PTR_TYPE  chain_ptr);


/* ---------------------- Routinen aus Modul: go_gsw ------------------------ */
extern BOOL   evoInit          (void);

extern INT    calc_p_go        (BOOL   *one_child,
                                INT     go_index,
                                DOUBLE  note);

extern INT    mutations_zahl   (INT     mutation,
                                INT     len);

extern GSTATE loadEvoParamFile (char   *fspec,
                                BOOL   *bad_data);

extern INT    get_last_go_idx  (void);

extern void   get_noten_limits (LONG   *n_lim1,
                                LONG   *n_lim2);

extern INT    get_mz_bereich   (void);

