/* -----------------------------------------------------------------------------
GLEAM/AE                      Package-Header-File
Package: hmod                    File: hmod.h                    Version: V3.1.6
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Das Package enthaelt Routinen zum Einlesen des MOD-Files, das das Handlungsmo-
dell beschreibt, zur Generierung segmentierter Ketten, zur Parametrierung und 
Parameteraenderung von Aktionen, zum Plausibilitaetstest, zum Genetic Repair und
zur Bestimmung des Hammingabstands zweier Ketten.

Module des Packages:
    segm_gen      Allgemeine AK-Generierung und -Segmentierung sowie Parametrie-
                  rung und Parameteraenderung von Aktionen. Allgemeine Formen 
                  des Genetic Repair und des Plausibilitaetstests.
    hamming       Bestimmung des Hammingabstands zweier Ketten.
    mod_data      Einlesen des MOD-Files und Zugriffsroutinen. 

--------------------------------------------------------------------------------
Status : O.K.                   Autor: W.Jakob                 Datum: 06.07.2019
-------------------------------------------------------------------------------- */


/* ========================= Exportierte Konstanten: ======================== */
#define NO_HAM                 99.0  /* Indikator fuer "Kein Hamming-Abstand" */
#define WITH_GENECODE_CHECK    TRUE  /* fuer get_act_descr()                  */
#define WITHOUT_GENECODE_CHECK FALSE /* fuer get_act_descr()                  */

/* --------------------------- Generierungscodes: --------------------------- */
#define FIX_ACT_FIX_GEN   1 /* gen_akt_mode: jede guelt.Akt. 1x in aufst.Sequ.*/
#define FIX_ACT_RAND_GEN  2 /* gen_akt_mode: jede guelt.Akt. 1x in zuf. Sequ. */
#define RAND_ACT_GEN      3 /* gen_akt_mode: Aktion zufaellig auswaehlen      */
#define FIX_LEN           1 /* gen_len_mode: AK-Laenge fix ("min_ketten_len") */
#define RAND_LEN          2 /* gen_len_mode: AK-Laenge zufaellig aus Intervall*/

#define ACT_NAME_MAX       20    /* Maximale Laenge der Aktionsnamen          */
#define PAR_NAME_MAX       20    /* Maximale Laenge der Parameternamen        */
#define EINHEIT_MAX        12    /* Maximale Laenge der Einheitsbezeichner    */
#define AFILE_MAX_ANZ       5    /* Maximale Anzahl zusaetzlicher ApplFiles   */
#define AFILE_EXT_LEN       3    /* Max.Laenge d.Extention zusaetzl.ApplFiles */
#define AFILE_BEZ_LEN      16    /* Max.Laenge d.Bezeichners f.zus. ApplFiles */

#define KEINE_KENNUNG       0    /* Die leere Kennung                         */
#define CHANGE_NEW          1    /* Wert d."mod_kind"-Param von "modify_param"*/
#define CHANGE_REL          2    /* Wert d."mod_kind"-Param von "modify_param"*/
#define CHANGE_SMALL        3    /* Wert d."mod_kind"-Param von "modify_param"*/
#define LESAK_APPL          1    /* LESAK mit int.Sim.         Kennung        */
#define GADO_APPL           2    /* DesignOpt mit ext.Sim.     der            */
#define MATLAB_APPL         3    /* MatLab-Integration         Anwen-         */
#define MATH_FKT_APPL       4    /* Math.Benchmarkfunktionen   dung           */
#define CEC_MATHFKT_APPL    5    /* Math.Benchmarkfkt von CEC  in             */
#define SIMU_SERV_APPL      6    /* Anbindung externer Simulationservices     */
#define ADD_APPL          100    /* Schnittst.f.ZusatzAnw.     "appl_code"    */
#define ELDO_SIMU           1    /* ELDO                         Kennung d.   */
#define M_MATICA_SIMU       2    /* Mathematica                  ext.Simula-  */
#define GEN_EXT_SIMU        3    /* allg.ExtSim-Schnittstelle    tors in "ext_*/
#define GEN_ML_EXT_SIMU     4    /* GenExt mit mehreren Zeilen   simu_code"   */
#define ML_MP_OPF         101    /* MATLAB: MATPOWER mit OPF                  */
#define ML_MP_PF          102    /* MATLAB: MATPOWER mit PF (ohne OPF)        */
#define MBF_SPHERE       1001    /* "ext_simu_code" einer Math-Benchmark-Fkt  */
#define MBF_WSPHERE      1002    /* "ext_simu_code" einer Math-Benchmark-Fkt  */
#define MBF_ACKLEY       1003    /* "ext_simu_code" einer Math-Benchmark-Fkt  */
#define MBF_FLETCHER     1004    /* "ext_simu_code" einer Math-Benchmark-Fkt  */
#define MBF_FOXHOLES     1005    /* "ext_simu_code" einer Math-Benchmark-Fkt  */
#define MBF_FRACTAL      1006    /* "ext_simu_code" einer Math-Benchmark-Fkt  */
#define MBF_RAST         1007    /* "ext_simu_code" einer Math-Benchmark-Fkt  */
#define MBF_RBERG        1008    /* "ext_simu_code" einer Math-Benchmark-Fkt  */
#define MBF_BRACK        1009    /* "ext_simu_code" einer Math-Benchmark-Fkt  */
#define MBF_BEALE        1010    /* "ext_simu_code" einer Math-Benchmark-Fkt  */
#define MBF_BIGG         1011    /* "ext_simu_code" einer Math-Benchmark-Fkt  */
#define MBF_HVALLEY      1012    /* "ext_simu_code" einer Math-Benchmark-Fkt  */
#define MBF_GRIEWANK     1013    /* "ext_simu_code" einer Math-Benchmark-Fkt  */
#define MBF_QUICK_DIRTY  1014    /* "ext_simu_code" einer Math-Benchmark-Fkt  */



/* ====================== Exportierte Typdeklarationen: ===================== */
typedef char  ACT_NAME_TYPE [ACT_NAME_MAX+1];
typedef char  PAR_NAME_TYPE [PAR_NAME_MAX+1];
typedef char  EINHEIT_TYPE  [EINHEIT_MAX];

typedef struct I_PAR_DESCR_TYPE
  {
    INT            par_u_grenze,
                   par_o_grenze,
                   anzSignStellen; /* nur f.AK-AehnlVgl, vgl. MIT_AEHNL_CHECK */
    DOUBLE         par_bereich;
    BOOL           big_range;
    DOUBLE         small_delta;
    PAR_NAME_TYPE  par_name;
    EINHEIT_TYPE   einheit,
                   alt_einheit;
  } I_PAR_DESCR_TYPE;

typedef struct R_PAR_DESCR_TYPE
  {
    DOUBLE         par_u_grenze,
                   par_o_grenze;
    DOUBLE         par_bereich,
                   small_delta;
    INT            anzSignStellen; /* nur f.AK-AehnlVgl, vgl. MIT_AEHNL_CHECK */
    PAR_NAME_TYPE  par_name;
    EINHEIT_TYPE   einheit,
                   alt_einheit;
  } R_PAR_DESCR_TYPE;

typedef struct SINGLE_ACT_DESCR_TYPE
  {
    INT               prio,          /* Gewicht der Aktion fuer Selektion     */
                      prio_sum,      /* Gewichtsumme der A(1) bis diese Aktion*/
                      i_par_anz,     /* Anzahl aller Integer-Parameter        */
                      r_par_anz,     /* Anzahl aller Real-Parameter           */
                      ges_par_anz,   /* Anzahl aller Integer- u.Real-Parameter*/
                      aend_ipar_anz, /* Anzahl aller aenderbaren Int-Parameter*/
                      aend_ges_par_anz; /* Anz.aller aenderb.Int-u.Real-Params*/
    ACT_NAME_TYPE     act_name;
    I_PAR_DESCR_TYPE *i_par_descr;
    R_PAR_DESCR_TYPE *r_par_descr;
  } SINGLE_ACT_DESCR_TYPE;

typedef SINGLE_ACT_DESCR_TYPE *ACT_DESCR_PTR_TYPE;


typedef char  AFILE_EXT_TYPE [AFILE_EXT_LEN + 1];
typedef char  AFILE_BEZ_TYPE [AFILE_BEZ_LEN + 1];
typedef char  AFILE_MEN_TYPE [MEN_ITEM_LEN  + 1];

typedef struct AFILE_DESCR_TYPE
  {
    BOOL            rettbar,     /* File kann (auch) geschrieben werden.      */
                    ladbar,      /* File kann (auch) geladen werden.          */
                    zu_retten,   /* Im Kontext von SaveExp zu retten.         */
                    bad_data;    /* Daten wegen Ladefehler inkonsistent.      */
    AFILE_EXT_TYPE  afile_ext;   /* Extention des Zusatz-Anwendungsfiles      */
    AFILE_BEZ_TYPE  afile_bez;   /* Bezeichnung des Zusatz-Anwendungsfiles    */
    AFILE_MEN_TYPE  afile_men;   /* Menue-Item des Zusatz-Anwendungsfiles     */
    STRING_TYPE     afile_spec;  /* Filespezifikation d.Zusatz-Anwendungsfiles*/
  } AFILE_DESCR_TYPE;



/* ========================== Exportierte Variable: ========================= */
extern AFILE_DESCR_TYPE    *appl_file_descr; /* Descript. f.zusaetzl.ApplFiles*/
extern CHAIN_ELEM_PTR_TYPE *aktionsFeld; /*orderXO/Reko: Aktionen der neuen AK*/
extern STR_TYPE             ext_sim_buf; /* Kennung des externen Simulators   */
extern char    *extSimSubDir,     /* Sub-Directory des externen Simulators    */
               *appl_name,        /* Applikations-Name (z.B. "GADO/V2")       */
               *appl_text,        /* Langform des Applikationsnamens          */
               *opt_param_name,   /* Bezeichnung der Optimierungsparameter    */
               *ext_simu_name;    /* Name des externen Simulators             */
extern DOUBLE  *simu_roh_erg_werte; /* Roh-Ergebniswerte des Simulators       */
extern INT     *i_params,         /* Integerparameter aller Aktionen          */
               *int_i_params,     /* Parameter der letzten interaktiven Simu  */
               *best_i_params,    /* Params d.BestAK seit "reset_simu_erg"    */
               *sequLen;          /* orderXO/Reko: Laenge der Elter-Sequenzen */
extern DOUBLE  *r_params,         /* Realparameter aller Aktionen             */
               *int_r_params,     /* Parameter der letzten interaktiven Simu  */
               *best_r_params;    /* Params d.BestAK seit "reset_simu_erg"    */
extern BOOL    *akt_flags;        /* Kennung generierter/kopierter Aktionen   */
extern INT      appl_file_anz,    /* Anzahl zusaetzlicher Applikations-Files  */
                appl_code,        /* Kennung der Anwendung                    */
                ext_simu_code,    /* Kennung des externen Simulators          */
                max_ketten_len,   /* Maximale Kettenlaenge                    */
                min_abschn_len,   /* Mindest-Abschnittslaenge                 */
                i_par_ges_anz,    /* akt. Gesamtzahl d.Integerparams f.ExtSimu*/
                r_par_ges_anz,    /* akt. Gesamtzahl d.Realparameter f.ExtSimu*/
                akt_anz_roh_erg_werte,/* akt.GesAnz d.Ergebniswerte d.ExtSimu */
                ak_max_segm_len,  /* Akt. AK-Segmentlaengenobergrenze         */
                save_rate_def;    /* Defaultwert fuer die Evo-Save-Rate       */
extern BOOL     mit_ext_simu,     /* TRUE, wenn Applikation ext.Simu erfordert*/
                std_ak_appl,      /* Anwendung mit Standard-AK-Generierung    */
                std_segm_gen_appl,/* Stnd-ElemModifaktion- u.AK-SegmGen-Anwndg*/
                displayableParams,/* TRUE: chr. params. displayable as result */
                extractChrParams, /* extract chromosome parameters for simu   */
                paramsExtracted,  /* TRUE: chr.params. of last simu extracted */
                sim_up,           /* TRUE, wenn Simulator verfuegbar          */
                always_sim,       /* True= Simuliere unabhaengig vom AK-Flag  */
                mbf_appl,         /* TRUE: MATH_FKT_APPL oder CEC_MATHFKT_APPL*/
                mit_cec_f07,      /* TRUE: Spezial-Init von CEC-MBF f07       */
                do_always_sim,    /* True = keine Menue-Aend. von "always_sim"*/
                dyn_aks,          /* TRUE, wenn AKs dynamischer Laenge        */
                stat_ak_order;    /* TRUE = statische AKs u. Reihenfolge egal */
extern INT      i_par_anf,        /* Index des 1.Parameters in i_params       */
                akt_ipar_anz,     /* Aktuelle Anzahl der Integerparameter     */
                all_iwerte_anz,   /* AktAnz aller Eintraege in i_params       */
                akt_rpar_anz,     /* Aktuelle Anzahl an Realparametern        */
                maxIntNumber,     /* Max.Anz.d.Integerparameter aller Aktionen*/
                maxDoubleNumber;  /* Max.Anz.d.Doubleparameter aller Aktionen */
extern size_t   dsize,            /* size of a DOUBLE                         */
                isize,            /* size of an INT                           */
                bsize;            /* size of a BOOL                           */
#ifdef MIT_LSV
  extern INT    *ug_iparams,      /* Fuer HyGLEAM: Untergrenze der Int-Params */
                *og_iparams;      /* Fuer HyGLEAM: Obergrenze  der Int-Params */
  extern DOUBLE *ug_rparams,      /* Fuer HyGLEAM: Untergrenze der Real-Params*/
                *og_rparams;      /* Fuer HyGLEAM: Obergrenze  der Real-Params*/
#endif /* MIT_LSV */



/* ========================== Exportierte Routinen: ========================= */
extern BOOL   hmodInit        (void);


/* ---------------------- Routinen aus Modul: segm_gen ---------------------- */
extern BOOL   gen_chain       (CHAIN_ELEM_PTR_TYPE   *chain_ptr,
                               GSTATE                *ret_state);

extern INT    calc_segm_len   (INT                    rest_len);

extern void   init_seq_gen    (CHAIN_ELEM_PTR_TYPE    chain_ptr);

extern GSTATE gen_act_seq     (CHAIN_ELEM_PTR_TYPE   *seq_ptr,
                               CHAIN_ELEM_PTR_TYPE   *seq_end_ptr,
                               INT                    number, 
                               SHORT                  start_act);

extern GSTATE initGene        (CHAIN_ELEM_PTR_TYPE    elemPtr,
                               SHORT                  geneCode);

extern BOOL   parameterize    (CHAIN_ELEM_PTR_TYPE    item_ptr,
                               GSTATE                *ret_state);

extern BOOL   modify_param    (CHAIN_ELEM_PTR_TYPE    item_ptr,
                               INT                    mod_kind,
                               GSTATE                *ret_state);

extern GSTATE repair_2_aks    (CHAIN_ELEM_PTR_TYPE    ak1_ptr,
                               CHAIN_ELEM_PTR_TYPE    ak2_ptr);

extern BOOL   repair_1_chain  (CHAIN_ELEM_PTR_TYPE   *ch_ptr,
                               GSTATE                *ret_state);

extern GSTATE segmentiere_ch  (CHAIN_ELEM_PTR_TYPE    ch_ptr,
                               BOOL                   komplett);


/* ---------------------- Routinen aus Modul: hamming.c --------------------- */
extern DOUBLE ham_dist        (CHAIN_ELEM_PTR_TYPE    ch1_ptr,
                               CHAIN_ELEM_PTR_TYPE    ch2_ptr);


/* ---------------------- Routinen aus Modul: mod_data ---------------------- */
extern GSTATE read_mod_file   (STR_TYPE               file_spec,
                               BOOL                  *bad_mod_data);

extern GSTATE get_act_descr   (ACT_DESCR_PTR_TYPE    *action_descr,
                               SHORT                  activity,
                               BOOL                   withCheck);

extern INT    get_len_data    (INT                   *segm_min_len);

extern BOOL   check_appl_id   (char *appl_id);
