/*******************************************************************************
GLEAM/AE                                                                  V2.2.2

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: HAndlungsMODell spezifische software
                        Module: MODell-DATA
 
Package: hmod                   File: mod_data.c                  
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Der Modul enthaelt neben der Leseroutine fuer das MOD-File die Datenstruktur
"act_descr" und alle damit zusammenhaengenden Allokations- und Verwaltungs-
routinen.

Am Anfang des MOD-Filekopfs stehen die Generierungsangaben fuer die AKs:
gen_len_mode:  AK-Laenge bei der Generierung:  
               1=FIX_LEN (Laenge="min_ketten_len")  2=RAND_LEN
gen_akt_mode:  1=FIX_ACT_FIX_GEN:  jede gueltige Aktion wird in aufsteigender 
                                   Reihenfolge genau 1x generiert.
               2=FIX_ACT_RAND_GEN: jede gueltige Aktion wird genau 1x in be-
                                   liebiger Reihenfolge generiert.
               3=RAND_ACT_GEN:     jede gueltige Aktion wird entsprechend den
                                   MOD-Fileangaben in beliebiger Reihenfolge 
                                   generiert.
gen_segm_vert: 1=GLEICH_VERT   2=FALLEND_VERT  (siehe auch "segm_gen.c")

Der MOD-Filekopf enthaelt in der 4. Datenzeile nach der obligatorischen Angabe
ueber die aktuellen RohErgWerte des (externen) Simulators eine optionale Angabe
fuer die Mutation "kleine Parameteraenderung": "small_change_frac".
Die folgende Datenzeile enthaelt nach der obligatorischen Anzahl externer Files
die optionale Laenge der Tabelle fuer den AK-Aehnlichkeitsvergleich (vgl.
Schalter MIT_AEHNL_CHECK). Wenn diese Laenge angegeben ist, wird fuer jeden 
Aktionsparameter eine zusaetzliche Angabe ueber die Anzahl der signifikanten 
Stellen fuer den AK-Vergleich werwartet.

Der Kopf des MOD-Files enthaelt auch die Beschreibung der anwendungsspezifischen 
Zusatzfiles, die in der Datenstruktur "appl_file_descr" abgelegt werden. Die
Beschreibung hat die Form:
<io-code> <datei-bezeichnung> <menue-item>
mit: <io-code>           0=lesen, 1=lesen/schreiben, 2=schreiben
     <datei-bezeichnung> Dateibezeichnung fuer Dialoge 
                         (max. 15 Zeichen (AFILE_BEZ_LEN))
     <menue-item>        Eintrag im entsprechenden Menue 
                         (max. 10 Zeichen (MEN_ITEM_LEN))

--------------------------------------------------------------------------------
Verbesserte Integration der Angaben zu LSVs und Heuristiken:
============================================================
Die Verwendung von LSVs und/oder Heuristiken als eigenständige Optimierer, als 
Meme oder zur Verbesserung der Startpopulation wird durch die Kennung "withLS"
in der 2.Kopfzeile nach den obligatorischen Eintraegen angegeben. Fehlt diese,
sind keine LSVs und/oder Heuristiken aktiviert und es darf auch keinen ent-
sprechenden Parameterteil geben. Dieser wird durch Zeilen mit den Kennungen
"BEGIN_LS" und "END_LS" begrenzt.

Stand vor dem 9.4.2018:
=======================
Wenn in der 2.Kopfzeile ein "new" steht, folgt nach den Angaben zu den anwen-
dungsspezifischen Zusatzdateien ein Teil zu Parametrierung der LSVs, der mit
der Zeile END_LSV endet.
--------------------------------------------------------------------------------

Der Module verwaltet die package-globale Datenstruktur "act_descr", ein Pointer-
Feld fuer Aktionsdeskriptoren (SINGLE_ACT_DESCR_TYPE) zur Beschreibung des 
Handlungsmodells. Diese enthalten Name, Prioritaet und Parameteranzahl jeder 
Aktion sowie pro Parameter dessen Namen, seinen Einheitsbezeichner nebst dem 
alternativen Einheitsbezeichner sowie seine Ober- und Untergrenzen. Diese Daten
werden dem MOD-File entnommen. Das Feld wird dynamisch entsprechend der durchs 
MOD-File vorgegenen Anzahl der Aktionen angelegt.

Alle Routinen sind so implementiert, dass ein schlechter uebergebener Return-
status die Nichtausfuehrung der Routine bewirkt. 


Die Routinen im einzelnen:

GSTATE read_mod_file (STR_TYPE  file_spec,
                      BOOL     *bad_mod_data);
          Initialisiert die die Kettengenerierung steuernde Datenstruktur "act_
          descr" aus dem durch "file_spec" bezeichneten File. Wenn dabei ein 
          Fehler (I/O-Error, unkompatible Daten) auftritt, erfolgt eine Fehler-
          meldung und die Routine wird abgebrochen. "bad_mod_data" gibt dann an,
          ob die internen Modelldaten bereits inkonsistent geworden sind (TRUE)
          oder ob mit dem alten Modell weitergearbeitet werden kann (FALSE).
          Die modul-globalen Variablen "gen_len_mode", "gen_akt_mode" und "new_
          last_act" muessen einer Konsistenzpruefung genuegen. Dabei wird wie
          folgt geprueft:
              bei "FIX_LEN":   "min_ketten_len" <= "new_last_act" 
              bei "RAND_LEN":  "max_ketten_len" <= "new_last_act" 

GSTATE get_act_descr (ACT_DESCR_PTR_TYPE *action_descr,
                      SHORT               activity,
                      BOOL                withCheck);
          Die Routine liefert einen Pointer auf den zur "activity" gehoerigen 
          Eintrag im Feld "act_descr" ab. Bei gesetztem Compilerschalter CHECKS 
          oder bei gesetztem "withCheck" wird die Gueltigkeit des Gencodes in
          "activity" geprueft. Im Fehlerfalle erfolgt eine Fehlermeldung, es 
          wird GLEAM_ERROR zurueck geliefert und "action_descr" zeigt auf den 
          Gentyp von ACTIVITY_ACT.

INT    get_len_data (INT *segm_min_len);
          Liefert die maximale Kettenlaenge und die minimale Segmenlaenge fuer
          die Generierung ab.

BOOL   check_appl_id (char *appl_id);
          Prueft, ob "appl_id" zur aktuell geladenen Applikation passt und lie-
          fert positivenfalls TRUE, sonst FALSE ab.

BOOL   hmodInit (void);
          Initialsiert die modul-internen Datenstrukturen und liefert FALSE ab,
          wenn dazu nicht genuegend Speicher zur Verfuegung steht. Sonst TRUE.


Package-globale Routinen:

BOOL   aktion_gueltig (SHORT  activity);
          Die Funktion liefert TRUE, wenn die zu "activity" gehoerige Aktivitaet
          eine Prioritaet > 0 hat und "activity" einen zulaessigen Wert hat.


--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
1.  Erweiterung f.die neuen gen.Operatoren PAR_CHANGE_SMALL u.SEGM_CHANGE_SMALL.
2.  Zur besseren Ausnutzung des Speicherplatzes werden die Parameterdeskriptoren
    d. Aktionsdescriptoren dynamisch entsprechend dem aktuellen Bedarf angelegt.
3.  Fehler in "alloc_descr_data" korrigiert.                         WJ 12.12.98
4.  "gen_akt_mode" wird zusammen mit den Codes exportiert. "i_params" 
    und "r_params" werden mit an das HMod angepasster Länge expor-
    tiert. Neue exportierte Variable "int_pro_aktion" und 
    "real_pro_aktion".                                               WJ 13.08.00
5.  Neue exportierte Variable f. HyGLEAM (gesetztes MIT_LSV)         WJ 18.08.00
6.  Neuen ExtSim-Schnittstellencode GEN_ML_EXT_SIMU eingefuehrt.     WJ 11.09.00
7.  Spezialanbindung dynamischer AKs eines Aktionstyps f.ext.Simu.   WJ 04.10.00
8.  Die Spezialanbindung von 7 f.allg.dyn. AKs u.ext.Simu.erweitert. WJ 21.02.01
9.  Integration des COMPLEX-Alg.: "best_params" nach "lsvkoppl" u.
    Aufruf der Routine "prepare_lsv_koppl" aus "lsvkoppl". Neue 
    Variable "dyn_aks" und "stat_ak_order". Neue Defaults.           WJ 27.04.01
10. "save_rate_def" an Borland-C angepasst.                          WJ 12.05.01
11. "opt_param_name" wird bei MATH_FKT_APPL mit MBF_PAR_TXT besetzt. WJ 07.05.02
12. Korrektur der Berechnung des "par_bereich" von Integer-Parameter
    fuer grosse Wertebereiche. Variable "big_range" im I_PAR_DESCR_
    TYPE zur Steuerung des Wert-Auswuerfelns eingefuehrt.            WJ 12.02.03
13. Einfuehrung der MBF MBF_QUICK_DIRTY fuer schnelles Testen.       WJ 13.11.03
14. "std_ak_appl" und "std_segm_gen_appl" werden Package-Export.     WJ 10.05.06
15. Erweiterung des MOD-Files fuer LSVs und Heuristiken.             WJ 28.08.06
16. Erweiterung von "alloc_descr_data" fuer neue gen. Operatoren 
    "order_xo" und "order_reko".                                     WJ 24.01.07
17. Integration der Griewank-Funktion (F22).                         WJ 10.05.07
18. Erweiterung fuer die MatLab-Koppung: Aus GAMA wird MatLab.       WJ 23.08.07
19. PhaenoRep Sache von Matlab: Schalter auf FALSE f.MatLab-Anw.     WJ 28.08.07
20. Integration der MBFs der CEC05-Sammlung (CEC_MATHFKT_APPL).      WJ 26.10.07
21. Anpassung an neue Datentypen fuer Int- und Real-Parameter
    (neues "chaindef.h" fuer 64 bit)                                 WJ 03.01.08
22. SMALL_CHANGE_FRAC_DEF von .0001 auf .001 geaendert. Entspricht 
    f. kl.Aenderungen einem Aenderungsbereich von (og-ug)/100.       WJ 29.04.08
23. "always_sim" fuer MBFs auf FALSE gesetzt.                        WJ 16.05.08
24. "mit_cec_f07" fuer Sonderinitialisierung dieser MBF eingefuehrt. WJ 02.06.08
25. Erweiterungen fuer die Variante mit AK-Aehnlichkeitsvergleich;
    Begrenzung d. Praezision auf AEHNLVGL_PRECMIN..AEHNLVGL_PRECMAX. WJ 04.08.08
26. "min_abschn_len" und "max_ketten_len" werden Package-Export 
    wegen Gorba-Umplanung.                                           WJ 09.09.08
27. Anpassung an parameterlose LSVs ausgeloest durch die GORBA-LSVs. WJ 04.11.09
28. Entfernung der Routine "update_bgi_hdr" sowie der Schalter und 
    des dazugehoerigen Codes:BS_DOS, BS_WINXP_GNU.                   WJ 21.06.12
29. Renovierung: Korrektur der fgets-Aufrufe. Verbesserung von 
    "read_name".                                                     WJ 11.03.16
30. "read_mod_file": Ohne MOD-File-Angaben gilt: lsv_anz = 0.        WJ 05.07.16
31. Integration der neuen TSK-Datenverwaltung. Reparatur von 
    "read_name". Verschiebung von "save_rate_def" nach "evo_gsw.c".  WJ 13.12.16
32. Ersetzung von "sim_mod_art_name" durch "extSimSubDir". Der kann
    leer sein, was im Gegensatz zu GADO keine indirekte Steuerungs-
    funktion mehr hat. "read_mod_file" setzt auch die Editierbarkeit 
    von OPAL/V-relevanten TSK-File-Parametern.                       WJ 04.04.17
33. Vorbereitungen zur Integration des Matlab/Matpower-Simulators.
    Einfuehrung von "simulateChromosome", das TRUE ist, wenn keine 
    Parameteraufbereitung erfolgen soll und das Chromosom direkt an 
    die anwendungsspezifische Simu-Routine uebergeben wird. Einfueh-
    rung von "extractChrParam" zur einfacheren Steuerung der Parame-
    teraufbereitung. Verbesserte Fehlerbehandlung beim Modellladen.  WJ 28.04.17
34. Webbasierte Anbindung externer Simulatoren: Neue Variable
    "maxIntNumber" und "maxDoubleNumber". Neu: SIMU_SERV_KENNUNG. 
    Ersetzung von "akt_roh_erg_werte" durch "akt_anz_roh_erg_werte". 
    Fehlermeldung NOT_ENOUGH_MEM korrigiert.                         WJ 31.05.17
35. "extractChrParams" wird in "read_mod_file" oder "set_appl_texts" 
    direkt gesetzt, wenn die Parameter fuer die Simulation eines 
    OptJobs zu extrahieren sind. "displayableParams" gibt an, ob die
    Parameter eines Chromosoms bei der Ergebnisdarstellung eines 
    OptJobs angezeigt werden koennen.                                WJ 14.08.17
36. Neuer Programmparameter SIMU_DOMAIN_NAME_TXT, der am Ende von 
    read_mod_file() ggf. auf editierbar gesetzt wird.                WJ 25.09.17
37. Streichung des Includes von "simu.h".                            WJ 09.01.18
38. EXT_MATLAB_SIMU: Neue exportierte Variable "matlabApplName", die 
    bei der "Matlab"-Anwendungskennung gelesen wird.                 WJ 29.03.18
39. Verbesserte LSV-Kennung im MOD-File, siehe Doku.                 WJ 09.04.18
40. ESS_EVAL_SLAVE: Keine Aufrufe von setParamEditable().            WJ 20.04.18
41. Aufruf von set_appl_texts() in read_mod_file() angepasst.        WJ 06.06.18
42. get_act_descr() prueft auch bei gesetzem ESS_EVAL_SLAVE auf die
    Zulaessigkeit der Gene-ID. "last_act" wird mit 0 initialisiert. 
    Streichung von "matlabApplName". Dessen Rolle uebernimmt 
    "ext_sim_buf" und die daraus abgeleitete Variable 
    "ext_simu_code". Einfuehrung entsprechender neuer Kennungen.     WJ 22.10.18
43. Erweiterung von get_act_descr() um einen Parameter, der die 
    Pruefung des Gencodes "activity" veranlasst. Daher spielt der 
    Schalter ESS_EVAL_SLAVE keine Rolle mehr. Neue Variable 
    "modifyGeneParams", die durch alloc_param_data() angelegt und 
    angepasst wird. Neue Konstante MAX_GENE_CODE, die auch zur 
    Pruefung der Angaben des MOD-Files verwendet wird.               WJ 01.07.19
44. Auslagerung der Variablen "last_act", "int_pro_aktion" und 
    "real_pro_aktion" in das package "lgsw", damit diese dort nach 
    Laden eines Genmodells bei gesetztem WITH_DYN_GENES zur 
    Verfuegung stehen. 
    WITH_DYN_GENES: Keine Pruefung der Obergrenze der Integer- oder 
    Realparameter eines Gens bzw. einer Aktion.                      WJ 10.07.19
45. read_mod_file(): Update of program parameter data base, if 
    "mitOptionPhaenoRep" is set. Removal of program parameter 
    OPAL_MAX_PRODTIME. Some corrections in error handling.           WJ 29.08.20
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 29.08.2020
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "lgsw.h"
#include "bew.h"
#include "appl.h"
#include "aufg.h"
#include "evo.h"
#include "hmod.h"
#include "hmodInt.h"

#include HMOD_TEXTS

#ifdef GLO_CHECKS
  #define CHECKS
#endif



/* ========================== Exportierte Variable: ========================= */
AFILE_DESCR_TYPE    *appl_file_descr; /* Descriptoren f.zusaetzliche ApplFiles*/
CHAIN_ELEM_PTR_TYPE *aktionsFeld;     /* orderXO/Reko: Aktionen der neuen AK  */
STR_TYPE             ext_sim_buf;     /* Kennung des externen Simulators      */
char    *extSimSubDir,          /* Sub-Directory des externen Simulators      */
        *appl_name,             /* Applikations-Name (z.B. "GADO/V2")         */
        *appl_text,             /* Langform des Applikationsnamens            */
        *opt_param_name,        /* Bezeichnung der Optimierungsparameter      */
        *ext_simu_name;         /* Name des externen Simulators               */
DOUBLE  *simu_roh_erg_werte;    /* Roh-Ergebniswerte des Simulators           */
INT     *i_params,              /* Integerparameter aller Aktionen            */
        *int_i_params,          /* Parameter der letzten interaktiven Simu    */
        *best_i_params,         /* Params des BestAK seit "reset_simu_erg"    */
        *sequLen;               /* orderXO/Reko: Laenge der Elter-Sequenzen   */
DOUBLE  *r_params,              /* Realparameter aller Aktionen               */
        *int_r_params,          /* Parameter der letzten interaktiven Simu    */
        *best_r_params;         /* Params des BestAK seit "reset_simu_erg"    */
BOOL    *akt_flags;             /* Kennung generierter/kopierter Aktionen     */
INT      appl_file_anz,         /* Anzahl zusaetzlicher Applikations-Files    */
         appl_code,             /* Kennung der Anwendung                      */
         ext_simu_code,         /* Kennung des externen Simulators            */
         max_ketten_len,        /* Maximale Kettenlaenge                      */
         min_abschn_len,        /* Mindest-Abschnittslaenge                   */
         i_par_ges_anz,         /* aktuelle Gesamtzahl der Integerparameter   */
         r_par_ges_anz,         /* aktuelle Gesamtzahl der Realparameter      */
         akt_anz_roh_erg_werte, /* akt. Gesamtzahl d. Ergebniswerte d.ExtSimu */
         ak_max_segm_len;       /* akt. AK-Segmentlaengenobergrenze           */
BOOL     mit_ext_simu,          /* TRUE, wenn Applikation ext. Simu erfordert */
         std_ak_appl,           /* Anwendung mit Standard-AK-Generierung      */
         std_segm_gen_appl,     /* Stnd-ElementModifaktion- u.AK-SegmGen-Anwndg*/
         displayableParams,     /* TRUE: chr. params. displayable as result   */
         extractChrParams,      /* extract chromosome parameters for simu     */
         paramsExtracted;       /* TRUE: chr. params. of last simu extracted  */
BOOL     sim_up        = FALSE; /* TRUE, wenn Simulator verfuegbar            */
BOOL     always_sim    = FALSE, /* True = Simuliere unabhaengig vom AK-Flag   */
         do_always_sim = FALSE, /* True = keine Menue-Aend. von "always_sim"  */
         mbf_appl,              /* TRUE: MATH_FKT_APPL oder CEC_MATHFKT_APPL  */
         mit_cec_f07,           /* TRUE: Spezial-Init von CEC-MBF f07         */
         dyn_aks       = FALSE, /* TRUE, wenn AKs dynamischer Laenge          */
         stat_ak_order =  TRUE; /* TRUE = statische AKs u. Reihenfolge egal   */
INT      i_par_anf,             /* Index des 1.Parameters in i_params         */
         akt_ipar_anz,          /* Aktuelle Anzahl an Integerparametern       */
         all_iwerte_anz,        /* AktAnz aller Eintraege in i_params         */
         akt_rpar_anz,          /* Aktuelle Anzahl an Realparametern          */
         maxIntNumber,          /* Max.Anz.der Integerparameter aller Aktionen*/
         maxDoubleNumber;       /* Max.Anz.der Doubleparameter aller Aktionen */
size_t   dsize,                 /* size of a DOUBLE                           */
         isize,                 /* size of an INT                             */
         bsize;                 /* size of a BOOL                             */

#ifdef MIT_LSV
  INT    *ug_iparams,           /* Fuer HyGLEAM: Untergrenze der Int-Params   */
         *og_iparams;           /* Fuer HyGLEAM: Obergrenze  der Int-Params   */
  DOUBLE *ug_rparams,           /* Fuer HyGLEAM: Untergrenze der Real-Params  */
         *og_rparams;           /* Fuer HyGLEAM: Obergrenze  der Real-Params  */
#endif /* MIT_LSV */



/* ======================== Package-globale Variable: ======================= */
ACT_DESCR_PTR_TYPE *act_descr; /* Pointer-Feld fuer die Aktions-Deskriptoren  */
TAB_ELEM_TYPE      *ak1_tab,   /* Tabellen fuer "repair_2_aks"                */
                   *ak2_tab;   /* Tabellen fuer "repair_2_aks"                */
INT      *ak_segm_laengen;     /* Segmlen-Wahrsch.f."segmentiere_ch"          */
INT       min_ketten_len,      /* Mindest-Kettenlaenge                        */
          act_prio_sum,        /* Aktuelle Prioritaets-Summe                  */
          act_anz_haelfte,     /* Abgerundete Hälfte der Aktionsanz           */
          ak_slen_sum,         /* WahrschSumme f. "segmentiere_ak"            */
          segm_werte_anz,      /* Anzahl gueltiger Segmentlaengen             */
          gen_akt_mode,        /* Akt. Gen-Modus: Aktionen u.Aktionssequnzen  */
          gen_len_mode;        /* Akt. Gen-Modus: Kettenlaenge                */
DOUBLE    min_delta = DBL_MIN * 20;  /* fuer "change_r_param"                 */
DOUBLE    ham_max_abst;        /* MaxAktAbstand bei AKs fester Laenge(HAMMING)*/
char     *appl_ident;          /* Ident-Text der geladenen Applikation        */
BOOL     *modifyGeneParams;    /* Markierung zu aendernder Genparameter       */



/* ============================ Lokale Konstante: =========================== */
#define LESAK_KENNUNG        "LESAK/Plus"   /* Kennung d.Anwendung im MOD-File*/
#define GADO_KENNUNG         "GADO-V2"      /* Kennung d.Anwendung im MOD-File*/
#define MATLAB_KENNUNG       "Matlab"       /* Kennung d.Anwendung im MOD-File*/
#define MFKT_KENNUNG         "MathFkt"      /* Kennung d.Anwendung im MOD-File*/
#define CEC_MBF_KENNUNG      "CEC-MathFkt"  /* Kennung d.Anwendung im MOD-File*/
#define ELDO_KENNUNG         "Eldo"         /* Kennung d.ext.Simu. im MOD-File*/
#define MATHICA_KENNUNG      "Mathematica"  /* Kennung d.ext.Simu. im MOD-File*/
#define GEN_SIM_KENNUNG      "GenExtSim"    /* Kennung d.ext.Simu. im MOD-File*/
#define GEN_ML_SIM_KENNUNG   "GenMLExtSim"  /* Kennung d.ext.Simu. im MOD-File*/
#define SIMU_SERV_KENNUNG    "ExtSimuServ"  /* Kennung d.ext.Simu. im MOD-File*/
#define MBF_SPHERE_KENNUNG   "MBF-Sphere"   /* MathBenchmFkt-Kennung im MOD-F.*/
#define MBF_WSPHERE_KENNUNG  "MBF-WSphere"  /* MathBenchmFkt-Kennung im MOD-F.*/
#define MBF_ACKLEY_KENNUNG   "MBF-Ackley"   /* MathBenchmFkt-Kennung im MOD-F.*/
#define MBF_FLETCHER_KENNUNG "MBF-Fletcher" /* MathBenchmFkt-Kennung im MOD-F.*/
#define MBF_FOX_KENNUNG      "MBF-Foxholes" /* MathBenchmFkt-Kennung im MOD-F.*/
#define MBF_FRACTAL_KENNUNG  "MBF-Fractal"  /* MathBenchmFkt-Kennung im MOD-F.*/
#define MBF_RAST_KENNUNG     "MBF-Rast"     /* MathBenchmFkt-Kennung im MOD-F.*/
#define MBF_RBERG_KENNUNG    "MBF-Rberg"    /* MathBenchmFkt-Kennung im MOD-F.*/
#define MBF_BRACK_KENNUNG    "MBF-Bracken"  /* MathBenchmFkt-Kennung im MOD-F.*/
#define MBF_BEALE_KENNUNG    "MBF-Beale"    /* MathBenchmFkt-Kennung im MOD-F.*/
#define MBF_BIGG_KENNUNG     "MBF-Bigg"     /* MathBenchmFkt-Kennung im MOD-F.*/
#define MBF_HVALLEY_KENNUNG  "MBF-HValley"  /* MathBenchmFkt-Kennung im MOD-F.*/
#define MBF_GRIEWANK_KENNUNG "MBF-Griewank" /* MathBenchmFkt-Kennung im MOD-F.*/
#define MBF_Q_AND_D_KENNUNG  "MBF-Q-and-D"  /* MathBenchmFkt-Kennung im MOD-F.*/
#define ML_MP_OPF_KENNUNG    "MATPOWER_OPF" /* Matlab: MatPower mit OPF       */
#define ML_MP_PF_KENNUNG     "MATPOWER_PF"  /* Matlab: MatPower mit PF        */
#define LSV_KENNUNG          "withLS"       /* mit Parameterteil f.LSVs/Heuris*/
#define LSV_ANFANG_KENNUNG   "BEGIN_LS"     /* Beginn der LSV/Heuri-Parameter */
#define LSV_ENDE_KENNUNG     "END_LS"       /* Ende der LSV/Heuri-Parameter   */
#define GLEICH_VERT           1             /* Code fuer "gen_segm_vert"      */
#define FALLEND_VERT          2             /* Code fuer "gen_segm_vert"      */
#define SMALL_CHANGE_FRAC_DEF 0.001         /* 1/100  d.Intervallanteils f.kl.*/
                                            /* ParAend (=Klassenwert) Default */




/* ============================ Lokale Variable: ============================ */
static INT     abschn_delta,      /* Schwankungsbreite der Abschn-Laenge      */
               old_akt_typen_anz, /* Aktuelle Groesse von "act_descr"         */
               old_roh_erg_anz,   /* Akt.Anz.d.SimuRohErgWerte Datenobj       */
               old_segm_anz,      /* Akt. Anz. verschiedener Segmentlaengen   */
               old_i_par_ges_anz, /* Akt. Anz. aller Intergerparameter        */
               old_r_par_ges_anz, /* Akt. Anz. aller Realparameter            */
               oldMaxParPerGene,  /* Aktuelle Laenge von "modifyGeneParams"   */
               ires;              /* IO-Ergebnis fuer "read_mod_file"         */
static DOUBLE  small_change_frac; /* Klassenwert f.kleine ParamAenderungen    */ 
static FILE   *mod_file;          /* MOD-File fuer "read_mod_file"            */



/* ============================ Lokale Routinen: ============================ */
static void   read_name        (char  *obj_name,
                                INT    max_len,
                                INT    act_code,
                                INT    callInfo);

static BOOL   alloc_descr_data (INT    akt_typen_anz);

static BOOL   alloc_simu_data  (INT    roh_erg_anz);

static BOOL   alloc_segm_data  (INT    len);

static BOOL   alloc_param_data (INT    i_par_ges_anz, 
                                INT    r_par_ges_anz);




/* ============================== read_mod_file ============================= */
GSTATE read_mod_file (STR_TYPE  file_spec,
                      BOOL     *bad_mod_data)
{
   ACT_NAME_TYPE  name_buf,               /* f. MOD-Ident u weitere Kennungen */
                  tmp_buf;
   GSTATE         ret_state = GLEAM_OK;   /* Return-Status                    */
   INT            gen_segm_vert;          /* Akt. Wahrsch.Vert.f. SegmLaengen */
   INT            new_len_mode, new_akt_mode,            /* Neue Modellwerte, */
                  new_min_ch_len, new_max_ch_len,        /* die bis zum posi- */
                  new_min_abschn_len, new_abschn_delta,  /* tiven Check zwi-  */
                  new_last_act, new_akt_anz_roh_erg_werte,/* schengespeichert */
                  new_ak_max_segm_len, new_appl_file_anz;/* werden.           */
   INT            prio, i_param_anz, r_param_anz, 
                  heuri_anz,     /* muss spaeter aus Heuristik-Modulen kommen */
                  wert, idx, len, ii, jj, kk;
   SHORT          ai;
   DOUBLE         delta, p_sum;
   STRING_TYPE    buffer;             /* (grosser) Skip-Puffer                */
   BOOL           ok, ok2,            /* Fuer Zwischenpruefungen              */
                  mit_lsv_heuri;      /* Mit "withLS"-Kennung f.LSV u.Heuri   */
   char          *ch_tmp;
#ifdef MIT_LSV
   INT            ip_idx = 0;         /* Int-Param-Index                      */
   INT            rp_idx = 0;         /* Real-Param-Index                     */
#else
   INT            lsv_anz;            /* wg. Ueberlesen von LSV/Heuri-Angaben */
#endif /* MIT_LSV */
#ifdef MIT_AEHNL_CHECK
   BOOL           mitAehnlPrec;       /* TRUE: Daten f.AK-AehnlVgl erwartet   */
#endif /* MIT_AEHNL_CHECK */
  
   /* read_mod_file */
   *bad_mod_data = FALSE;
   mit_lsv_heuri = FALSE;
   /* ---------------- MOD-File oeffnen und Kopf einlesen: ------------------ */
   if (!open_file (&mod_file, file_spec, "rt"))
      return (GLEAM_ERROR);
   ch_tmp = fgets (buffer, MAX_STRING, mod_file);      /* skip header line    */
   read_name (name_buf, strlen (PROG_IDENT), -1, 0);           /* Programm-ID */
   if (strcmp (name_buf, PROG_IDENT) != 0)
   { /* falsche Programm-ID */
      fclose (mod_file);
      sprintf (buffer, "%s ", name_buf);             /* never an empty string */
      return (fehler (HMOD_DATA, -2, NO_INFO, NO_INFO, buffer, BAD_MOD_FILE));
   } /* falsche Programm-ID */

   strcpy (ext_sim_buf, UNDEF_TXT);
   read_name (name_buf, ACT_NAME_MAX, -1, 0);    /* read Applikations-Kennung */
   sprintf (buffer, "%s ", name_buf); /*for error msgs: never an empty string */
   if (strcmp (name_buf, MATLAB_KENNUNG) == 0)
   { /* matlab-Anwendung */
#ifdef EXT_MATLAB_SIMU
      appl_code = MATLAB_APPL;
      read_name(ext_sim_buf, BEZ_LEN, -2, 0);
#else /* kein EXT_MATLAB_SIMU */
      ret_state = fehler (HMOD_DATA, -30, NO_INFO, NO_INFO, buffer, APPL_NOT_AVAILABLE);
#endif /* kein EXT_MATLAB_SIMU */
   } /* matlab-Anwendung */
   else 
      if (strcmp (name_buf, GADO_KENNUNG) == 0)
      { /* GADO */
#ifdef EXT_MATLAB_SIMU
	 ret_state = fehler (HMOD_DATA, -31, NO_INFO, NO_INFO, buffer, APPL_NOT_AVAILABLE);
#else /* kein EXT_MATLAB_SIMU */
         read_name (ext_sim_buf, ACT_NAME_MAX, -2, 0);    /* ext.Simu-Kennung */
         appl_code = GADO_APPL;
#endif /* kein EXT_MATLAB_SIMU */
      } /* GADO */
      else 
         if (strcmp (name_buf, LESAK_KENNUNG) == 0)
            appl_code = LESAK_APPL;
         else 
            if (strcmp (name_buf, MFKT_KENNUNG) == 0)
            { /* Math. Benchmark-Funktionen */
               read_name (ext_sim_buf, ACT_NAME_MAX, -3, 0);   /* MBF-Kennung */
               appl_code = MATH_FKT_APPL;
            } /* Math. Benchmark-Funktionen */
            else
	       if (strcmp (name_buf, SIMU_SERV_KENNUNG) == 0)
#ifdef EXT_SIMU_SERV
		  appl_code = SIMU_SERV_APPL;
#else /* kein EXT_SIMU_SERV */
                  ret_state = fehler (HMOD_DATA, -32, NO_INFO, NO_INFO, buffer, APPL_NOT_AVAILABLE);
#endif /* kein EXT_SIMU_SERV */
	       else
		  if (strcmp (name_buf, CEC_MBF_KENNUNG) == 0)
                  { /* Math. Benchmark-Funktionen */
		     read_name (ext_sim_buf, ACT_NAME_MAX, -4, 0); /*MBF-Kennung */
		     appl_code = CEC_MATHFKT_APPL;
		  } /* Math. Benchmark-Funktionen */
		  else
		     if (!read_add_appl (&appl_code, name_buf)) 
			ret_state = fehler (HMOD_DATA, -3, NO_INFO, NO_INFO, 
					    buffer, BAD_APPLICATION);
   read_name (tmp_buf, ACT_NAME_MAX, -9999, 0);  /* read Kennung f.erw.Format */
   if (strlen (tmp_buf) > 0) { /* moeglicherweise neues Format f.LSV u.Heuri */
      mit_lsv_heuri = (strcasecmp(tmp_buf, LSV_KENNUNG) == 0);
      ch_tmp = fgets (buffer, MAX_STRING, mod_file);     /* skip rest of line */
   } /* moeglicherweise neues Format f.LSV u.Heuri */

   /* -------------------- Allgemeine Daten einlesen: ----------------------- */
   ires = fscanf (mod_file, "%d%d%d", &new_len_mode, &new_akt_mode, 
          &gen_segm_vert);
   if (ires != 3)
      ret_state = fehler (HMOD_DATA, -4, ires, 3, "", MOD_IO_ERROR);
   ch_tmp = fgets (buffer, MAX_STRING, mod_file);        /* skip rest of line */
   ires = fscanf (mod_file,  "%d%d%d%d", &new_min_ch_len, &new_max_ch_len, 
          &new_min_abschn_len, &new_abschn_delta);
   if (ires != 4)
      ret_state = fehler (HMOD_DATA, -5, ires, 4, "", MOD_IO_ERROR);
   ch_tmp = fgets (buffer, MAX_STRING, mod_file);        /* skip rest of line */
   ires = fscanf (mod_file, "%d", &new_last_act);
   if (ires != 1)
      ret_state = fehler (HMOD_DATA, -6, ires, 1, "", MOD_IO_ERROR);
   ch_tmp = fgets (buffer, MAX_STRING, mod_file);        /* skip rest of line */
   ires = fscanf (mod_file, "%d", &new_akt_anz_roh_erg_werte);
   if (ires != 1)
      ret_state = fehler (HMOD_DATA, -7, ires, 1, "", MOD_IO_ERROR);
   ires = fscanf (mod_file, "%lf", &delta);
   if (ires == 1)
      small_change_frac = delta;
   else
      small_change_frac = SMALL_CHANGE_FRAC_DEF;
   ch_tmp = fgets (buffer, MAX_STRING, mod_file);        /* skip rest of line */
   ires = fscanf (mod_file, "%d", &new_appl_file_anz);
   if (ires != 1)
      ret_state = fehler (HMOD_DATA, -8, ires, 1, "", MOD_IO_ERROR);
#ifdef MIT_AEHNL_CHECK
   ires = fscanf (mod_file, "%d", &wert);
   if (ires == 1) {
      tabLen       = wert;
      mitAehnlPrec = TRUE;
   }
   else {
      tabLen       = AEHNLVGL_TABDEF;
      mitAehnlPrec = FALSE;
   }
#endif /* MIT_AEHNL_CHECK */
   ch_tmp = fgets (buffer, MAX_STRING, mod_file);        /* skip rest of line */

   /* ---------- Allgemeine Daten pruefen und ggf. uebernehmen:-------------- */
   if (new_min_ch_len > new_max_ch_len) 
      ret_state = fehler (HMOD_DATA, -9, NO_INFO, NO_INFO, CH_LEN_ERRTXT,
			  MOD_DATA_ERROR);               /* data check failed */
   new_ak_max_segm_len = new_min_abschn_len + new_abschn_delta;
   if (new_appl_file_anz > AFILE_MAX_ANZ) 
      ret_state = fehler (HMOD_DATA, -10, new_appl_file_anz, AFILE_MAX_ANZ, 
			  "", BAD_AFILE_AMOUNT);         /* data check failed */

   /* --- Pruefung der Aktionsanzahl, Generierungscodes und Kettenlaengen: -- */
   if (MAX_GENE_CODE < new_last_act) { /* too many gene types */
      ok2 = FALSE;
      meldung(HMOD_DATA, new_last_act, MAX_GENE_CODE, "", FALSCHE_AKT_ANZ);
   } /* too many gene types */
   else
      ok2 = TRUE;
   if ((0 < new_akt_mode) && (new_akt_mode <= FIX_ACT_RAND_GEN))
   { /* Aktionsauswahl u.-sequenzierung: FIX_ACT_RAND_GEN oder FIX_ACT_FIX_GEN*/
      if (new_len_mode == FIX_LEN)
      { /* AKs fester Länge bei FIX_ACT_RAND_GEN oder FIX_ACT_FIX_GEN */
         ok = new_min_ch_len <= new_last_act;
         if (new_min_ch_len < new_last_act)
             meldung (HMOD_DATA, new_min_ch_len, new_last_act, "", CH_LEN_ZU_KLEIN);
      } /* AKs fester Länge bei FIX_ACT_RAND_GEN oder FIX_ACT_FIX_GEN */
      else
         if (new_len_mode == RAND_LEN)
            ok = new_max_ch_len <= new_last_act;
         else
            ok = FALSE;
   } /* Aktionsauswahl u.-sequenzierung: FIX_ACT_RAND_GEN oder FIX_ACT_FIX_GEN*/
   else 
      ok = ((0 < new_akt_mode) && (new_akt_mode <= RAND_ACT_GEN) && 
            (0 < new_len_mode) && (new_len_mode <= RAND_LEN));
   if (!ok || !ok2)
      ret_state = fehler (HMOD_DATA, -11, NO_INFO, NO_INFO, 
			  GEN_MODE_ERR, MOD_DATA_ERROR);
   if (ret_state != GLEAM_OK)
   { /* Es trat ein Fehler auf */
      fclose (mod_file);
      return (GLEAM_ERROR);
   } /* Es trat ein Fehler auf */

   /* ---------- Ab jetzt werden die internen MOD-Daten veraendert: --------- */
   *bad_mod_data  = TRUE;    /* Wenn ab jetzt Fehler, sind die MOD-Daten hin! */
   if (!alloc_descr_data (new_last_act))
      ret_state = fatal (HMOD_DATA, -12, new_last_act, NO_INFO,
			 "", BAD_ACT_AMOUNT);      /* data acquisition failed */
   abschn_delta   = new_abschn_delta + 1;  /* da irand Werte [0..max) liefert */
   segm_werte_anz = abschn_delta;
   if (!alloc_segm_data (segm_werte_anz))
      ret_state = fatal (HMOD_DATA, -13, new_ak_max_segm_len, NO_INFO, "", 
			 BAD_SEGM_LEN);            /* data acquisition failed */
   min_ketten_len = new_min_ch_len;
   max_ketten_len = new_max_ch_len;
   min_abschn_len = new_min_abschn_len;
   gen_len_mode   = new_len_mode;
   gen_akt_mode   = new_akt_mode;
   last_act       = new_last_act;
   appl_file_anz  = new_appl_file_anz;
   stat_ak_order  = gen_akt_mode == FIX_ACT_FIX_GEN;
   dyn_aks        = gen_akt_mode == RAND_ACT_GEN;
   act_prio_sum   = 0;
   i_par_ges_anz  = 0;
   r_par_ges_anz  = 0;
   if (gen_len_mode == FIX_LEN)
      ham_max_abst = calc_max_abst (min_ketten_len); /*Abst.b.AKs fester Länge*/
   strcpy (appl_ident, name_buf);                        /* Appl-Ident merken */
   if (!alloc_simu_data (new_akt_anz_roh_erg_werte))
      ret_state = fatal (HMOD_DATA, -14, new_akt_anz_roh_erg_werte, NO_INFO, "", 
                         BAD_SIMU_ROH_ERG_ANZ);    /* data acquisition failed */
   akt_anz_roh_erg_werte = new_akt_anz_roh_erg_werte;
   do_always_sim         = akt_anz_roh_erg_werte > AK_ERG_WERTE_MAX;
   ak_max_segm_len       = new_ak_max_segm_len;

   /* ------------------- Einlesen der Zusatzfile-Angaben: ------------------ */
   for (ii = 0;  (ii < appl_file_anz) && (ret_state == GLEAM_OK);  ii++)
   { /* Daten eines Zusatzfiles lesen */
      ires = fscanf (mod_file, "%d", &wert);
      if (ires != 1)
         ret_state = fehler (HMOD_DATA, -100-ii, ires, 1, "", MOD_IO_ERROR);
      if (ret_state == GLEAM_OK)
      { /* I/0-Steuer-Wert ok, Rest lesen */
         appl_file_descr[ii].ladbar  = wert < 2;
         appl_file_descr[ii].rettbar = wert > 0;
         read_name (appl_file_descr[ii].afile_ext, AFILE_EXT_LEN, -(ii*10 + 1), 1);
         read_name (appl_file_descr[ii].afile_bez, AFILE_BEZ_LEN, -(ii*10 + 2), 2);
         read_name (appl_file_descr[ii].afile_men, MEN_ITEM_LEN,  -(ii*10 + 3), 3);
         ch_tmp = fgets (buffer, MAX_STRING, mod_file);  /* skip rest of line */
      } /* I/0-Steuer-Wert ok, Rest lesen */
      appl_file_descr[ii].zu_retten = FALSE;
      if (!appl_file_descr[ii].ladbar)
         appl_file_descr[ii].bad_data = FALSE;
   } /* Daten eines Zusatzfiles lesen */

   /* ------------- Einlesen der Angaben zu LSVs und Heuristiken: ----------- */
   if (mit_lsv_heuri)
   { /* LSV/Heuri-Angaben auswerten */
      ch_tmp = fgets(buffer, MAX_STRING, mod_file);     /* read and skip line */
      len    = strlen(LSV_ANFANG_KENNUNG);
      if (strlen(buffer) >= len)
	 if (strncmp(buffer, LSV_ANFANG_KENNUNG, len) != 0) /*Strings ungleich?*/
	    ret_state = fatal(HMOD_DATA, -33, NO_INFO, NO_INFO, LSV_ANFANG_KENNUNG,
			      BAD_OR_MISSING_LS_DELIMITER);
      ires = fscanf (mod_file, "%d%d", &lsv_anz, &heuri_anz);
      if (ires != 2)
         ret_state = fehler (HMOD_DATA, -15, ires, 3, "", MOD_IO_ERROR);
      ch_tmp = fgets (buffer, MAX_STRING, mod_file);     /* skip rest of line */
#ifdef MIT_LSV
      if (lsv_anz > LSV_ANZ)
         ret_state = fatal (HMOD_DATA, -16, lsv_anz, LSV_ANZ, "", BAD_LSV_AMOUNT);
      else
      { /* LSV-Daten einlesen */
         ok    = TRUE;
         p_sum = 0.0;
         for (ii = 0;  (ii < lsv_anz) && ok;  ii++)   /* aktive LSVs einlesen */
            if (fscanf (mod_file, "%d", &wert) == 1)
               ok = activate_lsv (wert);
         ch_tmp = fgets (buffer, MAX_STRING, mod_file);  /* skip rest of line */
         ok &= prep_active_lsvs();
         ok2 = TRUE;
         for (ii = 0;  (ii < lsv_anz) && ok2;  ii++)  /* LSV-Param f. LSV/SMA */
         { /* Schleife ueber alle LSVs */
            idx = get_lsv_idx (ii);
            for (jj = 0;  (jj < lsv[idx].lsvParAnz) && ok2;  jj++)
               ok2 = (fscanf (mod_file, "%lf", &(lsv[idx].lsvPar[jj])) == 1);
            if (lsv[idx].lsvParAnz > 0)
	       ch_tmp = fgets (buffer, MAX_STRING, mod_file); /* skip rest of line */
         } /* Schleife ueber alle LSVs */
         for (ii = 0;  (ii < lsv_anz) && ok2;  ii++)  /* LSV-Param f.ASMA/AMMA*/
         { /* Schleife ueber alle LSVs */
            idx = get_lsv_idx (ii);
            for (jj = 0;  (jj < lsv[idx].lsvParAnz) && ok2;  jj++) { /* ein Par */
               ok2 = (fscanf (mod_file, "%d", &(lsv[idx].parLevelAnz[jj])) == 1);
               if (ok2 && ((lsv[idx].parLevelAnz[jj] < 3) || 
                           (LEVEL_MAX < lsv[idx].parLevelAnz[jj]))) { 
                  ret_state = fatal (HMOD_DATA, -17, lsv[idx].parLevelAnz[jj], 
                                     LEVEL_MAX, "", BAD_PAR_AMOUNT);
                  ok2 = FALSE;
               }
               for (kk = 0;  (kk < lsv[idx].parLevelAnz[jj]) && ok2;  kk++) 
                  ok2 = (fscanf (mod_file, "%lf",              /* Level-Werte */
                                 &(lsv[idx].parLevelVal[jj][kk])) == 1);
               if (lsv[idx].lsvParAnz > 0)
                  ch_tmp = fgets (buffer, MAX_STRING, mod_file);  /* skip rest of line */
            } /* ein Par */
            for (jj = 0;  (jj < lsv[idx].lsvParAnz) && ok2;  jj++)  /* StartLevel */
               ok2 = (fscanf (mod_file, "%d", &(lsv[idx].parStartLev[jj])) == 1);
            if (lsv[idx].lsvParAnz > 0)
               ch_tmp = fgets (buffer, MAX_STRING, mod_file);     /* skip rest of line */
            for (jj = 0;  (jj < lsv[idx].lsvParAnz) && ok2;  jj++) /* initiale Ps */
               ok2 = (fscanf (mod_file, "%lf%lf", &(lsv[idx].parIniP1stLev[jj]),
                              &(lsv[idx].parIniP2ndLev[jj])) == 2);/* der Levels  */
            if (lsv[idx].lsvParAnz > 0)
               ch_tmp = fgets (buffer, MAX_STRING, mod_file);     /* skip rest of line */
            if (ok2) { /* initiale LSV-Wahrsch, initiale allP-Level-Werte */
               ok2 = (fscanf (mod_file, "%lf%d%lf%lf", &(lsv[idx].lsvStartP),
                              &(lsv[idx].allPStartLevel), &(lsv[idx].allPIniP1stLev), 
                              &(lsv[idx].allPIniP2ndLev)) == 4);
               ch_tmp = fgets (buffer, MAX_STRING, mod_file);     /* skip rest of line */
            } /* initiale LSV-Wahrsch, initiale allP-Level-Werte */
            p_sum += lsv[idx].lsvStartP;
         } /* Schleife ueber alle LSVs */
         ok2 &= (fscanf (mod_file, "%d", &allPLevelAnz) == 1);
         if (ok2 && ((allPLevelAnz < 2) || (LEVEL_MAX < allPLevelAnz))) { 
            ret_state = fatal (HMOD_DATA, -18, allPLevelAnz, LEVEL_MAX, "", BAD_PAR_AMOUNT);
            ok2 = FALSE;
         }
         for (kk = 0;  (kk < allPLevelAnz) && ok2;  kk++) /*allP-Level-Werte: */
            ok2 = (fscanf (mod_file, "%lf", &(allPLevelVal[kk])) == 1);
         ch_tmp = fgets (buffer, MAX_STRING, mod_file);  /* skip rest of line */
         if (p_sum > 1.0) { /* Summe der LSV-Wahrscheinlichkeiten != 1.0 */
            ret_state = fatal (HMOD_DATA, -19, (LONG)p_sum, NO_INFO, "", BAD_LSV_P_SUM);
            ok2 = FALSE;
         } /* Summe der LSV-Wahrscheinlichkeiten != 1.0 */
         ok &= ok2;
         if (ok)
            init_adi (ADAPT_SPEED_SCHNELL, ADAPT_SPEED_SCHNELL);
      } /* LSV-Daten einlesen */
      if (lsv_anz == 0)
         meldung (HMOD_DATA, NO_INFO, NO_INFO, "", NO_LSV_ACTIVATED);
      ch_tmp = fgets(buffer, MAX_STRING, mod_file);     /* read and skip line */
      len    = strlen(LSV_ENDE_KENNUNG);
      if (strlen(buffer) >= len)
	 if (strncmp(buffer, LSV_ENDE_KENNUNG, len) != 0)  /*Strings ungleich?*/
	    ret_state = fatal(HMOD_DATA, -34, NO_INFO, NO_INFO, LSV_ENDE_KENNUNG,
			      BAD_OR_MISSING_LS_DELIMITER);
#else /* ohne LSV */
      meldung (HMOD_DATA, NO_INFO, NO_INFO, "", NO_LSV_HEURI_IMPL);
      do { /* Ueberlesen bis END_LSV" */
         ch_tmp = fgets(buffer, MAX_STRING, mod_file);  /* read and skip line */
	 len    = strlen(LSV_ENDE_KENNUNG);
         if (strlen(buffer) >= len)
            ok2 = strncmp(buffer, LSV_ENDE_KENNUNG, len) == 0; /*strings gleich?*/
         else
            ok2 = FALSE;
      } /* Ueberlesen bis END_LSV" */
      while (!ok2 && !feof(mod_file));
      if (!ok2)                           /* LSV_ENDE_KENNUNG nicht gefunden? */
	 ret_state = fatal(HMOD_DATA, -35, NO_INFO, NO_INFO, LSV_ENDE_KENNUNG,
			   BAD_OR_MISSING_LS_DELIMITER);
#endif /* MIT_LSV */
   } /* LSV/Heuri-Angaben auswerten */
   else
      lsv_anz = 0;            /* ohne MOD-File-Angaben sind keine LSV aktiv ! */

   /* ------------------ Segmentierungsdaten vorbereiten: ------------------- */
   if ((gen_segm_vert == GLEICH_VERT) || (segm_werte_anz < 2))
   { /* Alle zulaessigen Segmentlaengen sind gleich wahrscheinlich */
      ak_slen_sum = 0;
      for (ii = 0;  ii < segm_werte_anz;  ii++)
      {
         ak_slen_sum++;
         ak_segm_laengen[ii] = ak_slen_sum;
      }
   } /* Alle zulaessigen Segmentlaengen sind gleich wahrscheinlich */
   else if (gen_segm_vert == FALLEND_VERT)
   { /* fallende Wahrscheinlichkeit der gueltigen Segment-Laengen */ 
      wert           = segm_werte_anz;
      ak_slen_sum    = segm_werte_anz * (segm_werte_anz + 1) / 2;
      for (ii = 0;  ii < segm_werte_anz;  ii++)
      {
         ak_segm_laengen[ii] = wert;
         wert += segm_werte_anz - ii - 1;
      }
   } /* fallende Wahrscheinlichkeit der gueltigen Segment-Laengen */
   else
      ret_state |= fehler (HMOD_DATA, -20, NO_INFO, NO_INFO, SEGM_VERT_ERR,
                           MOD_DATA_ERROR);

   /* ------------------- Aktions-Deskriptoren lesen: ----------------------- */
   ch_tmp = fgets (buffer, MAX_STRING, mod_file);       /* skip 1st docu line */
   len    = strlen(LSV_ANFANG_KENNUNG);
   if (strlen(buffer) >= len)      /* check for unexpected LS-part delimiter: */
      if (strncmp(buffer, LSV_ANFANG_KENNUNG, len) == 0)   /* Strings gleich? */
	 ret_state = fatal(HMOD_DATA, -36, NO_INFO, NO_INFO, LSV_ANFANG_KENNUNG,
			   UNEXPECTED_LS_DELIMITER_FOUND);
   for (ii = 1;  ii <= 6;  ii++) 
      ch_tmp = fgets (buffer, MAX_STRING, mod_file);       /* skip docu lines */
   if (feof(mod_file))
      ret_state |= fehler (HMOD_DATA, -21, last_act+1, 0, "", ZU_WENIG_AKTIONEN);
   for (ai = ACTIVITY_ACT;  (ai <= last_act) && (ret_state == GLEAM_OK);  ai++) 
   { /* Initialisierung aller Aktionen in "act_descr" */
      ires = fscanf (mod_file, "%d%d%d", &prio, &i_param_anz, &r_param_anz);
      if (ires != 3)
         ret_state = fehler (HMOD_DATA, ai, ires, 3, "", MOD_IO_ERROR);
      i_par_ges_anz += i_param_anz;
      r_par_ges_anz += r_param_anz;
      if (maxIntNumber < i_param_anz)      /* determine the maximum number of */
	 maxIntNumber = i_param_anz;       /* integer and                     */
      if (maxDoubleNumber < r_param_anz)   /* double  parameters              */
	 maxDoubleNumber = r_param_anz;    /* for all actions                 */
      read_name (act_descr[ai]->act_name, ACT_NAME_MAX, ai, 10);
      ch_tmp = fgets (buffer, MAX_STRING, mod_file);     /* skip rest of line */
      act_descr[ai]->prio     = prio;
      act_prio_sum           += prio;
      act_descr[ai]->prio_sum = act_prio_sum;
#ifndef WITH_DYN_GENES
      if (i_param_anz > I_PAR_ANZ_MAX) 
         ret_state = fehler (HMOD_DATA, ai, i_param_anz, I_PAR_ANZ_MAX, "", BAD_PAR_AMOUNT);
#endif /* no WITH_DYN_GENES */
      act_descr[ai]->i_par_anz   = i_param_anz;
      int_pro_aktion[ai]         = i_param_anz;
      real_pro_aktion[ai]        = r_param_anz;
      act_descr[ai]->ges_par_anz = i_param_anz + r_param_anz;
      if (i_param_anz > 0)
         if ((act_descr[ai]->i_par_descr = (I_PAR_DESCR_TYPE*)
              malloc(i_param_anz * sizeof(I_PAR_DESCR_TYPE))) == NULL)
            ret_state = fatal (HMOD_DATA, ai, new_last_act, NO_INFO, "", 
                               BAD_ACT_AMOUNT);    /* data acquisition failed */
      if (r_param_anz > 0)
         if ((act_descr[ai]->r_par_descr = (R_PAR_DESCR_TYPE*)
              malloc(r_param_anz * sizeof(R_PAR_DESCR_TYPE))) == NULL)
            ret_state = fatal (HMOD_DATA, ai, new_last_act, NO_INFO, "", 
                               BAD_ACT_AMOUNT);    /* data acquisition failed */
      for (ii = 0;  (ii < i_param_anz) && (ret_state == GLEAM_OK);  ii++) 
      { /* Lies I-Parameter-Descriptoren der Aktion */
         ires = fscanf (mod_file, "%d%d",
                        &(act_descr[ai]->i_par_descr[ii].par_u_grenze),
                        &(act_descr[ai]->i_par_descr[ii].par_o_grenze));
         if (ires != 2)
            ret_state = fehler (HMOD_DATA, ai, ires, 2, "", MOD_IO_ERROR);
         if (act_descr[ai]->i_par_descr[ii].par_u_grenze > 
             act_descr[ai]->i_par_descr[ii].par_o_grenze)
            ret_state = fehler (HMOD_DATA, -22, ai, ii+1, "", 
                                BAD_PAR_LIMITS);
         read_name (act_descr[ai]->i_par_descr[ii].par_name, PAR_NAME_MAX, ai, 11);
#ifdef MIT_AEHNL_CHECK
	 act_descr[ai]->i_par_descr[ii].anzSignStellen = AEHNLVGL_PRECDEF;
	 if (mitAehnlPrec) {
	    ires = fscanf (mod_file, "%d", &wert);
	    if (ires == 1) { /* gelesen */
	       if (AEHNLVGL_PRECMAX < wert) {
		  meldung (HMOD_DATA, ai, AEHNLVGL_PRECMAX, "", PREC_UNGEEIGNET);
		  wert = AEHNLVGL_PRECMAX;
	       }
	       else
		  if (wert < AEHNLVGL_PRECMIN) {
		     meldung (HMOD_DATA, ai, AEHNLVGL_PRECMIN, "", 
			      PREC_UNGEEIGNET);
		     wert = AEHNLVGL_PRECMIN;
		  }
	       act_descr[ai]->i_par_descr[ii].anzSignStellen = wert;
	    } /* gelesen */
	 }
#endif /* MIT_AEHNL_CHECK */
         ch_tmp = fgets (buffer, MAX_STRING, mod_file);  /* skip rest of line */
         read_name (act_descr[ai]->i_par_descr[ii].einheit, EINHEIT_MAX, ai, 12);
         if (act_descr[ai]->i_par_descr[ii].einheit[0] == '!')
            strcpy (act_descr[ai]->i_par_descr[ii].einheit, "");
         read_name (act_descr[ai]->i_par_descr[ii].alt_einheit, 1, ai, 13);
         act_descr[ai]->i_par_descr[ii].alt_einheit[1] = ' ';
         read_name (&(act_descr[ai]->i_par_descr[ii].alt_einheit[2]), 
                    EINHEIT_MAX - 2, ai, 14);
         ch_tmp = fgets (buffer, MAX_STRING, mod_file);   /* skip rest of line*/
         delta  = act_descr[ai]->i_par_descr[ii].par_o_grenze; /* Vorbereitng */
         delta -= act_descr[ai]->i_par_descr[ii].par_u_grenze; /*d.Berechng d.*/
         act_descr[ai]->i_par_descr[ii].par_bereich = delta;   /*Hamming-Abst.*/
         act_descr[ai]->i_par_descr[ii].big_range = delta > (DOUBLE)INT_MAX;
         delta = delta * small_change_frac;
         if (delta < 2.0)
            act_descr[ai]->i_par_descr[ii].small_delta = 0.0;
         else
            act_descr[ai]->i_par_descr[ii].small_delta = delta;
      } /* Lies I-Parameter-Descriptoren der Aktion */
#ifndef WITH_DYN_GENES
      if (r_param_anz > R_PAR_ANZ_MAX) 
         ret_state |= fehler (HMOD_DATA, ai, r_param_anz, R_PAR_ANZ_MAX, "", BAD_PAR_AMOUNT);
#endif /* no WITH_DYN_GENES */
      act_descr[ai]->r_par_anz = r_param_anz;
      for (ii = 0; (ii < r_param_anz) && (ret_state == GLEAM_OK); ii++) 
      { /* Lies R-Parameter-Descriptoren der Aktion */
         ires = fscanf (mod_file, "%lf%lf",
                        &(act_descr[ai]->r_par_descr[ii].par_u_grenze),
                        &(act_descr[ai]->r_par_descr[ii].par_o_grenze));
         if (ires != 2)
            ret_state = fehler (HMOD_DATA, ai, ires, 2, "", MOD_IO_ERROR);
         if (act_descr[ai]->r_par_descr[ii].par_u_grenze > 
             act_descr[ai]->r_par_descr[ii].par_o_grenze)
            ret_state = fehler (HMOD_DATA, -23, ai, ii+1, "", 
                                BAD_PAR_LIMITS);
         read_name (act_descr[ai]->r_par_descr[ii].par_name, PAR_NAME_MAX, ai, 15);
#ifdef MIT_AEHNL_CHECK
	 act_descr[ai]->r_par_descr[ii].anzSignStellen = AEHNLVGL_PRECDEF;
	 if (mitAehnlPrec) {
	    ires = fscanf (mod_file, "%d", &wert);
	    if (ires == 1) { /* gelesen */
	       if (AEHNLVGL_PRECMAX < wert) {
		  meldung (HMOD_DATA, ai, AEHNLVGL_PRECMAX, "", PREC_UNGEEIGNET);
		  wert = AEHNLVGL_PRECMAX;
	       }
	       else
		  if (wert < AEHNLVGL_PRECMIN) {
		     meldung (HMOD_DATA, ai, AEHNLVGL_PRECMIN, "", 
			      PREC_UNGEEIGNET);
		     wert = AEHNLVGL_PRECMIN;
		  }
	       act_descr[ai]->r_par_descr[ii].anzSignStellen = wert;
	    } /* gelesen */
	 }
#endif /* MIT_AEHNL_CHECK */
         ch_tmp = fgets (buffer, MAX_STRING, mod_file);  /* skip rest of line */
         read_name (act_descr[ai]->r_par_descr[ii].einheit, EINHEIT_MAX, ai, 16);
         if (act_descr[ai]->r_par_descr[ii].einheit[0] == '!')
            strcpy (act_descr[ai]->r_par_descr[ii].einheit, "");
         read_name (act_descr[ai]->r_par_descr[ii].alt_einheit, 1, ai, 17);
         act_descr[ai]->r_par_descr[ii].alt_einheit[1] = ' ';
         read_name (&(act_descr[ai]->r_par_descr[ii].alt_einheit[2]), 
                    EINHEIT_MAX - 2, ai, 18);
         ch_tmp = fgets (buffer, MAX_STRING, mod_file);   /* skip rest of line*/
         delta = act_descr[ai]->r_par_descr[ii].par_o_grenze - /* Vorbereitng */
            act_descr[ai]->r_par_descr[ii].par_u_grenze;       /* d. Berechng */
         act_descr[ai]->r_par_descr[ii].par_bereich = delta;   /*d.Hammingabst*/
         delta = delta * small_change_frac;
         if (delta < (DBL_MIN * 20.0))
            act_descr[ai]->r_par_descr[ii].small_delta = 0.0;
         else
            act_descr[ai]->r_par_descr[ii].small_delta = delta;
      } /* Lies R-Parameter-Descriptoren der Aktion */
      if (feof (mod_file) && (ai < last_act))
         ret_state |= fehler (HMOD_DATA, -24, last_act+1, ai, "", ZU_WENIG_AKTIONEN);
      act_descr[ai]->aend_ipar_anz    = i_param_anz;
      act_descr[ai]->aend_ges_par_anz = act_descr[ai]->ges_par_anz;
      ok = ret_state != GLEAM_FATAL;
      for (ii = i_param_anz - 1;  (ii >= 0) && ok;  ii--) 
      { /* check Param */
         if (act_descr[ai]->i_par_descr[ii].par_bereich == 0)
         { /* fixierten I-Param gefunden */
            (act_descr[ai]->aend_ipar_anz)--;
            (act_descr[ai]->aend_ges_par_anz)--;
         } /* fixierten I-Param gefunden */
         else
            ok = FALSE;
      } /* check Param */
      ok = ret_state != GLEAM_FATAL;
      for (ii = r_param_anz - 1;  (ii >= 0) && ok;  ii--) 
      { /* check Param */
         if (act_descr[ai]->r_par_descr[ii].par_bereich <= min_delta)
            (act_descr[ai]->aend_ges_par_anz)--;  /* fixierten R-Param gefunden */
         else
            ok = FALSE;
      } /* check Param */
   } /* Initialisierung aller Aktionen in "act_descr" */
   fclose (mod_file);

   /* ---------------- Abschlusspruefungen u. Gesamtcheck: ------------------ */
   act_anz_haelfte = last_act / 2;
   ok = TRUE;
   if (appl_code == GADO_APPL)
   { /* "ext_sim_buf" fuer GADO auswerten */
      if (strcmp (ext_sim_buf, ELDO_KENNUNG) == 0)
         ext_simu_code = ELDO_SIMU;
      else
         if (strcmp (ext_sim_buf, MATHICA_KENNUNG) == 0)
            ext_simu_code = M_MATICA_SIMU;
         else
            if (strcmp (ext_sim_buf, GEN_SIM_KENNUNG) == 0)
               ext_simu_code = GEN_EXT_SIMU;
            else
               if (strcmp (ext_sim_buf, GEN_ML_SIM_KENNUNG) == 0)
                  ext_simu_code = GEN_ML_EXT_SIMU;
               else
                  ok = FALSE;
   } /* "ext_sim_buf" fuer GADO auswerten */
   else
      if (appl_code == MATH_FKT_APPL)
      { /* "ext_sim_buf" fuer MBF auswerten */
         if (strcmp (ext_sim_buf, MBF_SPHERE_KENNUNG) == 0)
            ext_simu_code = MBF_SPHERE;
         else
            if (strcmp (ext_sim_buf, MBF_WSPHERE_KENNUNG) == 0)
               ext_simu_code = MBF_WSPHERE;
            else
               if (strcmp (ext_sim_buf, MBF_ACKLEY_KENNUNG) == 0)
                  ext_simu_code = MBF_ACKLEY;
               else
                  if (strcmp (ext_sim_buf, MBF_FLETCHER_KENNUNG) == 0)
                     ext_simu_code = MBF_FLETCHER;
                  else
                     if (strcmp (ext_sim_buf, MBF_FOX_KENNUNG) == 0)
                        ext_simu_code = MBF_FOXHOLES;
                     else
                        if (strcmp (ext_sim_buf, MBF_FRACTAL_KENNUNG) == 0)
                           ext_simu_code = MBF_FRACTAL;
                        else
                           if (strcmp (ext_sim_buf, MBF_RAST_KENNUNG) == 0)
                              ext_simu_code = MBF_RAST;
                           else
                              if (strcmp (ext_sim_buf, MBF_RBERG_KENNUNG) == 0)
                                 ext_simu_code = MBF_RBERG;
                              else
                                 if (strcmp (ext_sim_buf,  MBF_BRACK_KENNUNG) == 0)
                                    ext_simu_code = MBF_BRACK;
                                 else
                                    if (strcmp (ext_sim_buf, MBF_BEALE_KENNUNG) == 0)
                                       ext_simu_code = MBF_BEALE;
                                    else
                                       if (strcmp (ext_sim_buf, MBF_BIGG_KENNUNG) == 0)
                                          ext_simu_code = MBF_BIGG;
                                       else
                                          if (strcmp(ext_sim_buf,MBF_HVALLEY_KENNUNG)==0)
                                             ext_simu_code = MBF_HVALLEY;
                                          else
                                             if (strcmp (ext_sim_buf, 
                                                         MBF_GRIEWANK_KENNUNG) == 0)
                                                ext_simu_code = MBF_GRIEWANK;
                                             else
                                                if (strcmp (ext_sim_buf, 
                                                            MBF_Q_AND_D_KENNUNG) == 0)
                                                   ext_simu_code = MBF_QUICK_DIRTY;
                                                else
                                                   ok = FALSE;
      } /* "ext_sim_buf" fuer MBF auswerten */
      else
         if (appl_code == CEC_MATHFKT_APPL) 
         { /* "ext_sim_buf" fuer CEC-MBFs auswerten */
            ext_simu_code = (INT)strtol(&(ext_sim_buf[1]), NULL, 10);
            if ((ext_simu_code < 1) || (25 < ext_simu_code))
               ok = FALSE;
	    else
	       mit_cec_f07 = ext_simu_code == 7;
         } /* "ext_sim_buf" fuer CEC-MBFs auswerten */
	 else
	    if (appl_code == MATLAB_APPL)
	    { /* "ext_sim_buf" fuer Matlab-Anwendungen auswerten */
	       if ((strcmp (ext_sim_buf, ML_MP_OPF_KENNUNG) == 0) || 
		   (strcmp (ext_sim_buf, "OPF")             == 0)   )
		  ext_simu_code = ML_MP_OPF;
	       else
		  if ((strcmp (ext_sim_buf, ML_MP_PF_KENNUNG) == 0) || 
		      (strcmp (ext_sim_buf, "PF")             == 0)   )
		     ext_simu_code = ML_MP_OPF;
		  else
		     ok = FALSE;
	    } /* "ext_sim_buf" fuer Matlab-Anwendungen auswerten */

   if (!ok)
      ret_state |= fehler (HMOD_DATA, -25, NO_INFO, NO_INFO, ext_sim_buf, BAD_EXT_SIMU_KENNUNG);

   mbf_appl = FALSE;                                               /* Default */
   switch (appl_code)
   {
      case GADO_APPL:
      strcpy (appl_name,        GADO_NAME);
      strcpy (appl_text,        GADO_TEXT);
      strcpy (extSimSubDir,     GADO_SUB_DIR);
      strcpy (opt_param_name,   GADO_PARAM_TXT);
      strcpy (ext_simu_name,    ext_sim_buf);
      mit_ext_simu       = TRUE;
      displayableParams  = TRUE;
      extractChrParams   = TRUE;
      sim_up             = FALSE;
      std_simu_interface = TRUE;
      always_sim         = FALSE;
      std_ak_appl        = TRUE;
      std_segm_gen_appl  = TRUE;
      mit_gen_rep_par    = FALSE;         /* Anwendung mit GenRep-SteuerParam */
      mitOptionPhaenoRep = FALSE;         /* Mit Option fuer phaenotyp.Repair */
      mitPhaenoRepair    = FALSE;         /* Schalter f.phaenotypisches Repair*/
      break; /* GADO_APPL */

      case MATLAB_APPL: 
      strcpy (appl_name,        MATLAB_NAME);
      strcpy (appl_text,        MATLAB_TEXT);
      strcpy (extSimSubDir,     MLAB_SUB_DIR);
      strcpy (opt_param_name,   MLAB_PARAM_TXT);
      strcpy (ext_simu_name,    "Matlab/Matpower");
      mit_ext_simu       = TRUE;
      displayableParams  = TRUE;
      extractChrParams   = FALSE;            /* simulates chromosome directly */
      sim_up             = FALSE;
      std_simu_interface = TRUE;
      always_sim         = FALSE;
      std_ak_appl        = TRUE;
      std_segm_gen_appl  = TRUE;
      mit_gen_rep_par    = FALSE;     /* Anwendung mit GenRepair-SteuerParam  */
      mitOptionPhaenoRep = FALSE;     /* Option u.Schalter f.phaenotyp.Repair */
      mitPhaenoRepair    = FALSE;     /* FALSE, da PhaenoRep Sache von MatLab */
      break; /* MATLAB_APPL */

      case SIMU_SERV_APPL: 
      strcpy (appl_name,        SIMU_SERV_NAME);
      strcpy (appl_text,        SIMU_SERV_TEXT);
      strcpy (extSimSubDir,     "");                   /* is empty and unused */
      strcpy (opt_param_name,   UNDEF_TXT);
      strcpy (ext_simu_name,    SIMU_SERV_NAME);
      mit_ext_simu       = TRUE;
      displayableParams  = FALSE;
      extractChrParams   = FALSE;            /* simulates chromosome directly */
      sim_up             = FALSE;
      std_simu_interface = TRUE;
      always_sim         = FALSE;
      std_ak_appl        = TRUE;
      std_segm_gen_appl  = TRUE;
      mit_gen_rep_par    = FALSE;     /* Anwendung mit GenRepair-SteuerParam  */
      mitOptionPhaenoRep = FALSE;     /* Option u.Schalter f.phaenotyp.Repair */
      mitPhaenoRepair    = FALSE;     /* FALSE, da PhaenoRep Sache von MatLab */
      break; /* SIMU_SERV_APPL */

      case LESAK_APPL: 
      strcpy (appl_name,        LESAK_NAME);
      strcpy (appl_text,        LESAK_TEXT);
      strcpy (extSimSubDir,     "");
      strcpy (opt_param_name,   "");
      mit_ext_simu       = FALSE;
      displayableParams  = FALSE;
      extractChrParams   = FALSE;            /* simulates chromosome directly */
      sim_up             = TRUE;          /* Benutzende SW kann simulieren    */
      std_simu_interface = FALSE;
      always_sim         = TRUE;
      std_ak_appl        = TRUE;
      std_segm_gen_appl  = TRUE;
      mit_gen_rep_par    = FALSE;         /* Anwendung mit GenRep-SteuerParam */
      mitOptionPhaenoRep = FALSE;         /* Mit Option fuer phaenotyp.Repair */
      mitPhaenoRepair    = FALSE;         /* Schalter f.phaenotypisches Repair*/
      break; /* LESAK_APPL*/

      case CEC_MATHFKT_APPL:
      case MATH_FKT_APPL: 
      if (appl_code == CEC_MATHFKT_APPL)
	 strcpy (appl_name,     CEC_MBF_NAME);
      else
	 strcpy (appl_name,     MBF_NAME);
      strcpy (appl_text,        MBF_TEXT);
      strcpy (extSimSubDir,     "");
      strcpy (opt_param_name,   MBF_PARAM_TXT);
      strcpy (ext_simu_name,    ext_sim_buf);
      mbf_appl           = TRUE;
      mit_ext_simu       = FALSE;
      displayableParams  = TRUE;
      extractChrParams   = TRUE;
      sim_up             = TRUE;          /* Benutzende SW kann simulieren    */
      std_simu_interface = TRUE;
      always_sim         = FALSE;
      std_ak_appl        = TRUE;
      std_segm_gen_appl  = TRUE;
      mit_gen_rep_par    = FALSE;         /* Anwendung mit GenRep-SteuerParam */
      mitOptionPhaenoRep = FALSE;         /* Mit Option fuer phaenotyp.Repair */
      mitPhaenoRepair    = FALSE;         /* Schalter f.phaenotypisches Repair*/
      break; /* MATH_FKT_APPL*/

      case ADD_APPL: 
      ret_state |= set_appl_texts(&std_ak_appl, &std_segm_gen_appl);
      break;

      default:
      strcpy (appl_name,        UNDEF_TXT);
      strcpy (appl_text,        "");
      strcpy (extSimSubDir,     "");
      strcpy (opt_param_name,   "");
      mit_ext_simu       = FALSE;
      displayableParams  = FALSE;
      extractChrParams   = FALSE;
      sim_up             = TRUE;          /* Benutzende SW kann simulieren    */
      std_simu_interface = TRUE;
      always_sim         = FALSE;
      std_ak_appl        = FALSE;
      std_segm_gen_appl  = FALSE;
      mit_gen_rep_par    = FALSE;         /* Anwendung mit GenRep-SteuerParam */
      mitOptionPhaenoRep = FALSE;         /* Mit Option fuer phaenotyp.Repair */
      mitPhaenoRepair    = FALSE;         /* Schalter f.phaenotypisches Repair*/
   } /* case */

   if (mit_ext_simu && dyn_aks)
   { /* dyn.AKs u.ext.Simu: angenommene MaxLen: 5*max_ketten_len */
      i_par_ges_anz = i_par_ges_anz * max_ketten_len * 5 + 1 +
	              max_ketten_len * 5;                /* fuer Aktionscodes */
      r_par_ges_anz = r_par_ges_anz * max_ketten_len * 5 + 1;
   } /* dyn.AKs u.ext.Simu: angenommene MaxLen: 5*max_ketten_len */
   if (!alloc_param_data (i_par_ges_anz, r_par_ges_anz))
      ret_state = fatal (HMOD_DATA, -26, NO_INFO, NO_INFO, " ", NOT_ENOUGH_MEM);
#ifdef MIT_LSV
   else
   { /* Unter- und Obergrenzen fuer HyGLEAM separat abspeichern */
      for (ai = ACTIVITY_ACT;  ai <= last_act;  ai++) 
      { /* jeder Aktionsdescriptor */
/* Testausgaben fuer Locale-Probleme (3,14 satt 3.14)
printf ("read_mod_file: ai=%d: i_par_anz=%d r_par_anz=%d\n", ai, 
act_descr[ai]->i_par_anz, act_descr[ai]->r_par_anz);
*/
         for (ii = 0;  ii < act_descr[ai]->i_par_anz;  ii++) 
         { /* Alle Int-Param dieser Aktion */
            ug_iparams [ip_idx]   = act_descr[ai]->i_par_descr[ii].par_u_grenze;
            og_iparams [ip_idx++] = act_descr[ai]->i_par_descr[ii].par_o_grenze;
         } /* Alle Int-Param dieser Aktion */
         for (ii = 0;  ii < act_descr[ai]->r_par_anz;  ii++) 
         { /* Alle Real-Param dieser Aktion */
            ug_rparams [rp_idx]   = act_descr[ai]->r_par_descr[ii].par_u_grenze;
            og_rparams [rp_idx++] = act_descr[ai]->r_par_descr[ii].par_o_grenze;
         } /* Alle Real-Param dieser Aktion */
      } /* jeder Aktionsdescriptor */
#ifdef MIT_ROCO
      if (!prepare_rc_koppl (i_par_ges_anz, r_par_ges_anz))
	 ret_state = fatal (HMOD_DATA, -27, NO_INFO, NO_INFO, " ", NOT_ENOUGH_MEM);
#endif /* MIT_ROCO */
   } /* Unter- und Obergrenzen fuer HyGLEAM separat abspeichern */
#endif /* MIT_LSV */

   if (ret_state == GLEAM_OK)
   { /* Alles ok */
      *bad_mod_data = FALSE;
#ifdef MIT_AEHNL_CHECK
      if (stat_ak_order)
      { /* statische AKs ohne bedeutungstragende Aktionsreihenfolge */
	 idx = 0;
	 kk  = 0;
	 for (ai = ACTIVITY_ACT;  ai <= last_act;  ai++) 
	 { /* jeder Aktionsdescriptor */
	    for (ii = 0;  ii < act_descr[ai]->i_par_anz;  ii++)
	       iParSign[idx++] = act_descr[ai]->i_par_descr[ii].anzSignStellen;
	    for (ii = 0;  ii < act_descr[ai]->r_par_anz;  ii++)
	       rParSign[kk++] = act_descr[ai]->r_par_descr[ii].anzSignStellen;
	 } /* jeder Aktionsdescriptor */
      } /* statische AKs ohne bedeutungstragende Aktionsreihenfolge */
      else
      { /* AKs mit bedeutungstragender Aktionsreihenfolge */
	 for (ii = 0;  ii < i_par_ges_anz;  ii++)
	    iParSign[ii] = AEHNLVGL_PRECDEF;
	 for (ii = 0;  ii < r_par_ges_anz;  ii++)
	    rParSign[ii] = AEHNLVGL_PRECDEF;
      } /* AKs mit bedeutungstragender Aktionsreihenfolge */
#endif /* MIT_AEHNL_CHECK */
#ifndef ESS_EVAL_SLAVE
  #ifdef MIT_LSV
      ret_state |= parameteriseAllLSVs();
  #endif /* MIT_LSV */
      setParamEditable(lsv_anz > 0,                  MA_WITH_ALL_IMPR_IDX);
      setParamEditable(mit_ext_simu,                 SIMU_MAX_TIME_IDX);
      setParamEditable(mit_ext_simu,                 SIMU_MOD_NAME_IDX);
      setParamEditable(mit_ext_simu,                 SIMU_RUNS_PER_INIT_IDX);
  #if defined(EXT_SIMU_SERV)
      setParamEditable(mit_ext_simu,                 SIMU_DOMAIN_NAME_IDX);
  #endif /* EXT_SIMU_SERV */
  #if !defined(EXT_SIMU_SERV) && !defined(EXT_MATLAB_SIMU)
      setParamEditable(mit_ext_simu,                 WITH_SIMU_TIME_IDX);
  #endif /* weder EXT_SIMU_SERV noch EXT_MATLAB_SIMU */
      setParamEditable(appl_code == MATH_FKT_APPL,   MBF_ROT_ANGLE_IDX);
      setParamEditable(appl_code == LESAK_APPL,      LSKP_R500_IDX);
      setParamEditable(appl_code == LESAK_APPL,      LSKP_COLLI_TEST_IDX);
      setParamEditable(appl_code == LESAK_APPL,      LSKP_STOP_TEST_IDX);
      setParamEditable(appl_code == LESAK_APPL,      LSKP_STOP_DEPREC_IDX);
      setParamEditable(appl_code == LESAK_APPL,      LSKP_AXES_NUMBER_IDX);
      setParamEditable(appl_code == LESAK_APPL,      LSKP_CYCLE_LENGTH_IDX);
      setParamEditable((appl_code == ADD_APPL) && 
		       (add_appl_code == OPAL_APPL), OPAL_MAX_SHIFTS_IDX);
      if (mitOptionPhaenoRep) 
	 if (setBoolVal(mitPhaenoRepair, WITH_PHAENO_REP_IDX))
	    setParamEditable(TRUE, WITH_PHAENO_REP_IDX);
#endif /* ESS_EVAL_SLAVE */

/*  aktivieren, wenn Routine reinitialisieren kann. Dann in simu/simuInit
    entfernen.                                                            
#ifdef MIT_AEHNL_CHECK
      if (!initAehnlVgl())
	 ret_state = GLEAM_FATAL;
#endif /* MIT_AEHNL_CHECK */
   } /* Alles ok */
   return (ret_state);
} /* read_mod_file */



/* ================================ read_name =============================== */
static void read_name (char  *obj_name,
                       INT    max_len,
                       INT    act_code,
                       INT    callInfo)
/*----------------------------------------------------------------------------*/
/*  Liest aus der geoeffneten Datei "mod_file" einen blankfreien String ein   */
/*  und liefert ihn in "obj_name" ab. Der String wird im File durch ein Blank */
/*  oder das Zeilenende terminiert. Seine maximale Laenge ist durch "max_len" */
/*  vorgegeben. Linksstehende Blanks werden ueberlesen. "act_code" gibt die   */
/*  Aktion fuer eventuelle Meldungen (kein Fehler) bei fehlerhaften Namen an, */
/*  wobei bei -9999 keine Meldung erfolgt. "callInfo" gibt den Aufruf der     */
/*  Routine genauer an.                                                       */
/*----------------------------------------------------------------------------*/
{
   STR_TYPE  buf;
   INT       ii;
   char      ch;
  
   /* read_name */
   strcpy (obj_name, "");
   ii   = 0;
   ch   = fgetc (mod_file);
   if ((ch == '\n') || feof (mod_file))
   { /* nichts mehr */
      if (act_code != -9999)
	 meldung (HMOD_DATA, act_code, callInfo, "", NO_ACT_NAME);
      ungetc (ch, mod_file);
   } /* nichts mehr */
   else
   { /* Es steht noch was in der Zeile */
      while ((ch != '\n') && (ch == ' '))
	 ch = fgetc (mod_file);                                /* skip blanks */
      if (ch == '\n') 
      {
	 if (act_code != -9999)
	    meldung (HMOD_DATA, act_code, callInfo, "", NO_ACT_NAME);
	 ungetc (ch, mod_file);
      }
      else
      { /* Name lesen */
	 while ((ch != '\n') && (ch != ' ') && (ii < max_len))
         { /* read name */
	    sprintf (buf, "%c", ch);
	    strcat (obj_name, buf);
	    ch = fgetc (mod_file);
	    ii++;
	 } /* read name */
	 if (ch == '\n') 
	    ungetc (ch, mod_file);
	 if ((ii == max_len) && (ch != ' ') && (ch != '\n'))
	    meldung (HMOD_DATA, act_code, max_len, "",  ACT_NAME_TO_LONG);
      } /* Name lesen */
   } /* Es steht noch was in der Zeile */
} /* read_name */



/* ============================= get_act_descr ============================== */
GSTATE get_act_descr (ACT_DESCR_PTR_TYPE *action_descr_ptr,
                      SHORT               activity,
                      BOOL                withCheck)
{
   GSTATE  ret_state = GLEAM_OK;

   /* get_act_descr */
#if defined(CHECKS) 
   withCheck = TRUE;
#endif /* Routine mit Pruefungen */
   if (withCheck)
   { /* with check of validity of "activity" */
      if ((0 <= activity) && (activity <= last_act))
	 *action_descr_ptr = act_descr[activity];    /* valid "activity" code */
      else
      { /* invalid "activity"code */
	 *action_descr_ptr = act_descr[0]; /* damit es oben keinen Crash gibt */
	 ret_state = fehler (HMOD_DATA, -28, activity, NO_INFO, "", BAD_ACTION);
      } /* invalid "activity" code */
   } /* with check of validity of "activity" */
   else /* without check of validity of "activity" */
      *action_descr_ptr = act_descr[activity];
   return (ret_state);
} /* get_act_descr */



/* ============================== get_len_data ============================== */
INT get_len_data (INT *segm_min_len)
{
   *segm_min_len = min_abschn_len;
   return (max_ketten_len);
} /* get_len_data */



/* ============================== check_appl_id ============================= */
BOOL check_appl_id (char *appl_id)
{
   return (strcmp (appl_id, appl_ident) == 0);
} /* check_appl_id */



/* ============================= aktion_gueltig ============================= */
BOOL aktion_gueltig (SHORT  activity)
{
   if ((activity <= 0) || (last_act < activity))
      return (FALSE);
   else 
      return (act_descr[activity]->prio > 0);
} /* aktion_gueltig */



/* ============================ alloc_descr_data ============================ */
static BOOL alloc_descr_data (INT  akt_typen_anz)
/*----------------------------------------------------------------------------*/
/*  Prueft, ob die Datenobjekte "act_descr", "akt_flags", "ak1_tab","ak2_tab",*/
/*  "int_pro_aktion", "real_pro_aktion", "aktionsFeld" und "sequenzEnde" be-  */
/*  reits existieren, indem geprueft wird, ob "act_descr" kein NULL-Pointer   */
/*  ist. Wenn ja, werden die Datenobjekte freigegeben und gemaess der durch   */
/*  "akt_typen_anz" vorgegebenen Groesse neu angelegt. Die Funktion liefert   */
/*  TRUE, wenn dies gut ging, sonst FALSE.                                    */
/*----------------------------------------------------------------------------*/
{
   size_t  asize = sizeof (ACT_DESCR_PTR_TYPE);
   size_t  tsize = sizeof (TAB_ELEM_TYPE);
   INT     anz, ii;
   BOOL    ok    = TRUE;

   /* alloc_descr_data */
   if (act_descr != NULL)
   { /* Datenobjekte deallokieren */
      free (akt_flags);
      free (ak1_tab);
      free (ak2_tab);
      free (int_pro_aktion);
      free (real_pro_aktion);
      free (aktionsFeld);
      free (sequLen);
      for (ii = 0;  ii <= old_akt_typen_anz;  ii++)
      { /* "act_descr"-Komponenten freigeben */
	 if (act_descr[ii]->i_par_descr != NULL)
            free (act_descr[ii]->i_par_descr);
	 if (act_descr[ii]->r_par_descr != NULL)
            free (act_descr[ii]->r_par_descr);
	 free (act_descr[ii]);
      } /* "act_descr"-Komponenten freigeben */
      free (act_descr);
      act_descr = NULL;
   } /* Datenobjekte deallokieren */

   anz = akt_typen_anz + 1;
   if (act_descr == NULL)
   { /* Datenobjekte neu anlegen */
      ok=((akt_flags   = (BOOL*)               malloc((anz+1) * bsize)) != NULL) &&
         ((ak1_tab     = (TAB_ELEM_TYPE*)      malloc((anz+1) * tsize)) != NULL) &&
         ((ak2_tab     = (TAB_ELEM_TYPE*)      malloc((anz+1) * tsize)) != NULL) &&
         ((act_descr   = (ACT_DESCR_PTR_TYPE*) malloc( anz    * asize)) != NULL) &&
	 ((aktionsFeld = (CHAIN_ELEM_PTR_TYPE*)malloc( anz    * asize)) != NULL) &&
	 ((sequLen     = (INT*)                malloc( anz    * isize)) != NULL) &&
         ((int_pro_aktion  = (INT*)            malloc( anz    * isize)) != NULL) &&
         ((real_pro_aktion = (INT*)            malloc( anz    * isize)) != NULL) ;
      for (ii = 0;  (ii <= akt_typen_anz) && ok;  ii++)
      { /* Einen "act_descr" anlegen */
         ok = (act_descr[ii] = 
               (SINGLE_ACT_DESCR_TYPE*)malloc(sizeof(SINGLE_ACT_DESCR_TYPE))) != NULL;
	 if (ok)
	 {
	    act_descr[ii]->i_par_descr = NULL;
	    act_descr[ii]->r_par_descr = NULL;
	 }
      } /* Einen "act_descr" anlegen */
      old_akt_typen_anz = akt_typen_anz;
   } /* Datenobjekte neu anlegen */
   return (ok);
} /* alloc_descr_data */



/* ============================ alloc_simu_data ============================= */
static BOOL alloc_simu_data (INT  roh_erg_anz)
/*----------------------------------------------------------------------------*/
/*  Prueft, ob das Datenobjekt "simu_roh_erg_werte" bereits existiert, indem  */
/*  geprueft wird, ob "simu_roh_erg_werte" kein NULL-Pointer ist. Wenn ja,    */
/*  wird das Datenobjekt nur freigegeben und neu angelegt, wenn das vor-      */
/*  handene gemaess "roh_erg_anz" zu klein ist. Wenn nein, wird es gemaess    */
/*  der durch "roh_erg_anz" vorgegebenen Groesse neu angelegt. Die Funktion   */
/*  liefert TRUE, wenn dies gut ging, sonst FALSE.                            */
/*----------------------------------------------------------------------------*/
{
   BOOL  ok = TRUE;

   /* alloc_simu_data */
   if ((simu_roh_erg_werte != NULL) && (old_roh_erg_anz < roh_erg_anz))
   { /* Datenobjekte deallokieren */
      free (simu_roh_erg_werte);
      simu_roh_erg_werte = NULL;
   }  /* Datenobjekte existieren */
   if (simu_roh_erg_werte == NULL)
   { /* Datenobjekte neu anlegen */
      ok = (simu_roh_erg_werte = (DOUBLE*)malloc(roh_erg_anz * dsize)) != NULL;
      old_roh_erg_anz = roh_erg_anz;
   } /* Datenobjekte neu anlegen */
   return (ok);
} /* alloc_descr_data */



/* ============================= alloc_segm_data ============================ */
static BOOL alloc_segm_data (INT  len)
/*----------------------------------------------------------------------------*/
/*  Prueft, ob das Datenobjekt "ak_segm_laengen" bereits existiert. Wenn ja,  */
/*  wird das Datenobjekt nur freigegeben und neu angelegt, wenn es gemaess    */
/*  "len" zu klein ist. Wenn nein, wird es mit der Laenge "len" neu angelegt. */
/*  Die Funktion liefert TRUE, wenn dies gut ging, sonst FALSE.               */
/*----------------------------------------------------------------------------*/
{
   BOOL  ok = TRUE;

   /* alloc_segm_data */
   if ((ak_segm_laengen != NULL) && (old_segm_anz < len))
   { /* "ak_segm_laengen" deallokieren */
      free (ak_segm_laengen);                     /* Weg mit dem alten Schiet */
      ak_segm_laengen = NULL;
   }  /* "ak_segm_laengen" deallokieren */
   if (ak_segm_laengen == NULL)
   { /* "ak_segm_laengen" neu anlegen */
      ok = (ak_segm_laengen = (INT*)malloc((len * isize))) != NULL;
      old_segm_anz = len;
   } /* "ak_segm_laengen" neu anlegen */
   return (ok);
} /* alloc_segm_data */



/* ============================ alloc_param_data ============================ */
static BOOL alloc_param_data (INT  i_par_ges_anz, 
                              INT  r_par_ges_anz)
/*----------------------------------------------------------------------------*/
/*  Prueft, ob die Datenobjekte "i_params", "int_i_params", "best_i_params",  */
/*  "r_params", "int_r_params" und "best_r_params" bereits existieren, indem  */
/*  geprueft wird, ob "i_params" bzw. "r_params" kein NULL-Pointer ist oder   */
/*  die jeweilige Groesse nicht mehr ausreicht. Wenn ja, werden die entspre-  */
/*  chenden Datenobjekte freigegeben und gemaess der durch "i_par_ges_anz"    */
/*  bzw. "r_par_ges_anz" vorgegebenen Groesse neu angelegt. Desgleichen mit   */
/*  "modifyGeneParams", welches jedoch mit der Groesse von maxIntNumber+      */
/*  maxDoubleNumber angelegt wird.                                            */
/*  Die Funktion liefert TRUE, wenn dies gut ging, sonst FALSE.               */
/*----------------------------------------------------------------------------*/
{
   BOOL  ok = TRUE;

   /* alloc_param_data */
   if ((i_params != NULL) && (i_par_ges_anz > old_i_par_ges_anz))
   { /* alte Felder freigeben */
      free (i_params);
      free (int_i_params);
      free (best_i_params);
      i_params      = NULL;
      int_i_params  = NULL;
      best_i_params = NULL;
#ifdef MIT_AEHNL_CHECK
      free (iParSign);
      iParSign      = NULL;
#endif /* MIT_AEHNL_CHECK */
#ifdef MIT_LSV
      free (ug_iparams);
      free (og_iparams);
      ug_iparams    = NULL;
      og_iparams    = NULL;
#endif /* MIT_LSV */
   } /* alte Felder freigeben */
   if ((i_params == NULL) && (i_par_ges_anz > 0))
   { /* neue Felder anlegen */
      ok = ((i_params      = (INT*)malloc(i_par_ges_anz * isize)) != NULL) &&
           ((int_i_params  = (INT*)malloc(i_par_ges_anz * isize)) != NULL) &&
#ifdef MIT_AEHNL_CHECK
           ((iParSign      = (INT*)malloc(i_par_ges_anz * isize)) != NULL) &&
#endif /* MIT_AEHNL_CHECK */
#ifdef MIT_LSV
           ((ug_iparams    = (INT*)malloc(i_par_ges_anz * isize)) != NULL) &&
           ((og_iparams    = (INT*)malloc(i_par_ges_anz * isize)) != NULL) &&
#endif /* MIT_LSV */
           ((best_i_params = (INT*)malloc(i_par_ges_anz * isize)) != NULL);
      old_i_par_ges_anz = i_par_ges_anz;
   } /* neue Felder anlegen */

   if ((r_params != NULL) && (r_par_ges_anz > old_r_par_ges_anz) && ok)
   { /* alte Felder freigeben */
      free (r_params);
      free (int_r_params);
      free (best_r_params);
      r_params      = NULL;
      int_r_params  = NULL;
      best_r_params = NULL;
#ifdef MIT_AEHNL_CHECK
      free (rParSign);
      rParSign      = NULL;
#endif /* MIT_AEHNL_CHECK */
#ifdef MIT_LSV
      free (ug_rparams);
      free (og_rparams);
      ug_rparams  = NULL;
      og_rparams  = NULL;
#endif /* MIT_LSV */
   } /* alte Felder freigeben */
   if ((r_params == NULL) && (r_par_ges_anz > 0) && ok)
   { /* neue Felder anlegen */
      ok = ((r_params     = (DOUBLE*)malloc(r_par_ges_anz * dsize)) != NULL) &&
           ((int_r_params = (DOUBLE*)malloc(r_par_ges_anz * dsize)) != NULL) &&
#ifdef MIT_AEHNL_CHECK
           ((rParSign     = (INT*)   malloc(r_par_ges_anz * isize)) != NULL) &&
#endif /* MIT_AEHNL_CHECK */
#ifdef MIT_LSV
           ((ug_rparams   = (DOUBLE*)malloc(r_par_ges_anz * dsize)) != NULL) &&
           ((og_rparams   = (DOUBLE*)malloc(r_par_ges_anz * dsize)) != NULL) &&
#endif /* MIT_LSV */
           ((best_r_params= (DOUBLE*)malloc(r_par_ges_anz * dsize)) != NULL);
      old_r_par_ges_anz = r_par_ges_anz;
   } /* neue Felder anlegen */

   if ((modifyGeneParams != NULL) && ((maxIntNumber + maxDoubleNumber) > oldMaxParPerGene))
      free(modifyGeneParams);                         /* altes Feld freigeben */
   if ((modifyGeneParams == NULL) && ((maxIntNumber + maxDoubleNumber) > 0) && ok)
   { /* neues Feld anlegen */
      ok = ((modifyGeneParams = (BOOL*)malloc((maxIntNumber + maxDoubleNumber) * bsize)) != NULL);
      oldMaxParPerGene = maxIntNumber + maxDoubleNumber;
   } /* neues Feld anlegen */

   return (ok);
} /* alloc_param_data */



/* ================================ hmodInit ================================ */
BOOL hmodInit (void)
{
   INT  ii;

   maxIntNumber       = 0;
   maxDoubleNumber    = 0;
   appl_file_anz      = 0;
   std_ak_appl        = FALSE;
   std_segm_gen_appl  = FALSE;
   mit_cec_f07        = FALSE;
   paramsExtracted    = FALSE;                          /* for result display */
   ext_simu_code      = KEINE_KENNUNG;
   appl_code          = KEINE_KENNUNG;       /* Bewirkt jede Menge (fataler)  */
   gen_len_mode       = 0;                   /*    Fehler, wenn nicht bald    */
   gen_akt_mode       = 0;                   /*    ein MOD-File geladen wird! */
   act_descr          = NULL;                /* Datenstruktur                 */
   akt_flags          = NULL;                /*    noch                       */
   ak1_tab            = NULL;                /*                               */
   ak1_tab            = NULL;                /*                               */
   ak_segm_laengen    = NULL;                /*    nicht                      */
   simu_roh_erg_werte = NULL;                /*                               */
   i_params           = NULL;                /*                               */
   int_i_params       = NULL;                /*    an-                        */
   i_params           = NULL;                /*                               */
   best_r_params      = NULL;                /*                               */
   int_r_params       = NULL;                /*    ge-                        */
   best_r_params      = NULL;                /*                               */
   modifyGeneParams   = NULL;                /*    legt.                      */
#ifdef MIT_LSV
   ug_iparams         = NULL;                /* Datenstruktur                 */
   og_iparams         = NULL;                /*    noch                       */
   ug_rparams         = NULL;                /*    nicht                      */
   og_rparams         = NULL;                /*    angelegt                   */
#endif /* MIT_LSV */
   dsize = sizeof (DOUBLE);
   isize = sizeof (INT);
   bsize = sizeof (BOOL);

   if (((extSimSubDir    = (char*)malloc((BEZ_LEN+1)*sizeof(char))) == NULL) ||
       ((appl_name       = (char*)malloc((BEZ_LEN+1)*sizeof(char))) == NULL) ||
       ((appl_ident      = (char*)malloc((BEZ_LEN+1)*sizeof(char))) == NULL) ||
       ((appl_text       = (char*)malloc((MAX_STR+1)*sizeof(char))) == NULL) ||
       ((opt_param_name  = (char*)malloc((BEZ_LEN+1)*sizeof(char))) == NULL) ||
       ((ext_simu_name   = (char*)malloc((MAX_STR  )*sizeof(char))) == NULL) ||
       ((appl_file_descr = (AFILE_DESCR_TYPE*)malloc(AFILE_MAX_ANZ *
                            sizeof(AFILE_DESCR_TYPE))) == NULL))
      return (FALSE);
   strcpy (ext_simu_name, "");
   strcpy (extSimSubDir, "");
   strcpy (appl_ident, "");
   for (ii = 0;  ii < AFILE_MAX_ANZ;  ii++)
   {
      appl_file_descr[ii].bad_data = TRUE;
      strcpy (appl_file_descr[ii].afile_spec, "");
   }
   return (TRUE);   
} /* hmodInit */

