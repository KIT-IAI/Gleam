/* -----------------------------------------------------------------------------
GLEAM/AE                      Package-Header-File
Package: simu                    File: simu.h                    Version: V2.1.1
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
-------------------------------------------------------------------------------- 
 
Das Package enthaelt das Simulator-Menue, den Simulatorrahmen, die Simlatoran-
kopplung fuer externe Simulatoren und die internen Simulatoren. Die internen
Robotersimulatoren befinden sich als Subpackages in eigenen Unterverzeichnissen.

Module des Packages:
    simucmen  Enthaelt das Simulator-Menue der TEXT_MODE-Variante
    simu_gsw  Basisroutinen zur Ausgabenvorbereitung fuer den interaktiven Part 
              und die Engine-Version
    simu      Der Simulatorkern mit den Schnittstellen zur Ankopplung interner
              und externer Simulatoren.
    mbf_sim   Mathematische Benchmark-Funktionen (MBFs)
    cec_sim   MBFs der CEC'05-Sammlung (Routinehn im CEC-Unterverzeichnis)

Externe Simulator-Anbindungen:
    matlab_koppl Interface zu externen MatLab-Simulatoren. Alternativ zu
                 "ext_sim" und dessen Untermodulen

    extSimuServKoppl Interface zur Anbindung der externen Simulation Services

    ext_sim      Interface zu den externen Simulatoren
    eldokopl     Ankopplung des externen Simulators SimPilot / ELDO.
    mathkopl     Ankopplung des externen Simulators mathematica
    gen_kopl     Generelle Ankopplung externer Simulatoren

im lskp-Unterverzeichnis:
    rob_sim   Robotersimulator von LESAK (ohne Animation).
    rob_gsw   Basis- und Anzeigeroutinen des Robotersimulators (ohne Animation).
    rob_dial  Menue- und Dialog-Routinen des Robotersim. f.d. TEXT_MODE-Version.
    pasromin  Basisroutinen und Vereinbarung der geometrischen Datentypen.
    rob_int.h Package-globales h-File des Robotersimulators.
    lsk_grep  Roboterspezifisches Genetic-Repair.
    lsk_mod.h Konstantendefinitionen fuer das Roboter-Handlungsmodell.

--------------------------------------------------------------------------------
Aenderungen:
 1. "hole_obs" statt "load_obs".                                     CB 30.11.98
 2. Integration von "lsk_grep.c" u."lsk_mod.h" im "lskp"-Subpackage. WJ 05.12.98
 3. Anpassung von "zeige_start_ziel" und Integration von "joints.h". WJ 10.12.98
 4. Dekl. von hole_obs in rob_int.h                                  CB 18.04.99
 5. hole_obs und hindernismanagament von rob_int.h wieder hierher.   WJ 21.06.99
 6. Mehrere Dekls von simuInt.h wegen der GLEAM-Engine globalisiert. WJ 04.08.99
 7. Fuer HyGLEAM die Deklarationen von "do_mbf_sim", "do_ext_sim", 
    "do_rob_sim" und "behandle_neue_note" globalisiert.              WJ 23.08.00
 8. Neue Routine "get_ak_params" eingefuehrt.                        WJ 21.02.01
 9. "zeige_simu_bew" erweitert, "save_simu_erg" wird globaler Export  
    und neue Variable "res_fspec". Alles fuer die Engine.            WJ 14.06.02
10. "init_mbf_sim" fuer optionale Rotation der Koordinatenachsen.    WJ 24.04.03
11. MatLab-Koppung: "mitOnlVisu" und "mlOnlineVisu"                  WJ 24.08.07
12. Integration der MBFs der CEC05-Sammlung (CEC_MATHFKT_APPL).
    Neue globale Variable "mitMatLabSimu" und "cec_best_fval".
13. Erweiterungen fuer die Variante mit paralleler Simulation.       WJ 16.06.08
14. Erweiterungen fuer die Variante mit AK-Aehnlichkeitsvergleich.   WJ 21.07.08
15. Erweiterung der Parameterliste von "mlOnlineVisu" um "indivIdx"
    fuer die Version mit paralleler Simulation.                      WJ 28.07.08
16. AEHNLVGL_PRECMAX f. die Variante mit AK-Aehnlichkeitsvergleich.  WJ 01.08.08
17. Neue Routine " mlSaveOptRes" zur Ergebnissicherung (GLEAMKIT).   WJ 28.07.10
18. Verschiebung von "mit_simulog" nach Package "sys".               WJ 20.04.12
19. Entfernung von PAR_SIMU und den dazugehoerigen Deklarationen
    sowie von Deklarationen der Matlab-Kopplung.                     WJ 22.06.12
20. Einfuehrung der Topologiefilespezifikation "top_file_spec".      WJ 27.08.12
21. "show_simu_erg" wird Package-Export. "zeige_simu_bew" 
    und "zeige_evo_best_bew" sind package-export bei gesetztem 
    GUI_MODE und ansonsten nur package-lokal.                        WJ 05.12.14
22. Renovierung: Streichung des Zeilennummernmanagements. Aenderung
    der Aufrufschnittstelle von "ladekinematik" und "hole_obs".      WJ 13.05.16
23. "bewert_data_to_int_data", "plot_akt_wert", "restart_ext_sim", 
    "kleiner_ext_restart" und "ak_mem_neu_bewert" werden im GUI_MODE 
    zum Package-Export.                                              WJ 15.09.16
24. Neue TSK-Datenverwaltung: Streichung der "xxx_file_spec"-Vars.   WJ 07.12.16
25. Neue Variable "extSimuModelName" und "extSimuRunLimit",          WJ 03.03.17
    Export von EXT_SIMU_STARTUP_TXT, da mehrfach extern verwendet.   WJ 16.03.17
26. Anpassung der Doku an die Integration von Matlab/Matpower-Simu.  WJ 28.04.17
27. "lastSimuModelName" wird eine Package-Export.                    WJ 01.06.17
28. "best_ges_note" und bewert_data_to_???_data() werden Package-
    Export bei gesetztem EXT_SIMU_SERV.                              WJ 13.07.17
29. Neu: "extSimServDomainName" und "lastSimServDomainName".         WJ 25.09.17
30. Streichung des Includes von "joints.h".                          WJ 09.01.18
31. Anpassung des Exports an den neuen VISU_MODE.                    WJ 23.03.18
32. EXT_MATLAB_SIMU: Neue variable matlabNoConvCtr.                  WJ 26.04.18
33. Verschiebung von "doMatlabSim" aus simuInt.h fuer "rc_koppl.c".  WJ 27.06.18
34. Transfer of "mit_rz" and "statistik_pro_gen" to the
    "aufg"-package.                                                  WJ 17.07.18
35. For CLV: "extractCaseFile" becomes part of the package export.   WJ 29.10.18
36. PAR_POP_VERS: Delay of "delayMSec" msec after MBF exexution.     WJ 24.01.19
--------------------------------------------------------------------------------
Status: o.k.                    Autor: W.Jakob                 Datum: 24.01.2019
-------------------------------------------------------------------------------- */


/* ========================= Exportierte Konstanten: ======================== */
#define CALC_FITNESS     FALSE   /* nichtinteraktive Simulation = Bewertung   */
#define SIMU_SHOW        TRUE    /* interaktive Simulation                    */
#define AEHNLVGL_TABDEF  10000   /* Default-Wert von tabLen fuer AK-AehnlVgl  */
#define AEHNLVGL_PRECMIN     2   /* Min.Anz der sign. Stellen f. AK-AehnlVgl  */
#define AEHNLVGL_PRECDEF     5   /* Default der sign. Stellen f. AK-AehnlVgl  */
#define AEHNLVGL_PRECMAX    12   /* Max.Anz der sign. Stellen f. AK-AehnlVgl  */

/* --------------- Fuer die Ankopplung externer Simulatoren: ---------------- */
#define WAIT_FOR_READ      50                         
#define sec                * (1000 / WAIT_FOR_READ) 



/* ========================== Exportierte Variable: ========================= */

/* ------------------------------ aus simu: --------------------------------- */
extern char   *exp_file_spec,      /* File-Bezeichner fuer Experimentfile     */
              *res_fspec,          /* File-Bezeichner fuer Resultfile (engine)*/
              *extSimuModelName,   /* name of the simulation model of extSimu.*/
              *lastSimuModelName,  /* last loaded simulator model             */
              *extSimServDomainName, /* domain name of ext.simulation services*/
              *lastSimServDomainName;/* last domain name of extern.simu.serv. */
extern DOUBLE  cec_best_fval;      /* CEC:bester FktWert seit "reset_simu_erg"*/
extern INT     extSimuRunLimit,    /* limit of runs for reinit. of ext.Simu   */
               simuResultWaitCycles, /* wait cycles for simulator reply TMO   */
               erg_genauigkeit;    /* Genauigkeit der Evo-/Simu-Ergebniswerte */
extern BOOL    int_sim_data_ready, /* TRUE, wenn interakt. SimuDaten gueltig  */
               mit_simu_anim;      /* Mit Animation der Simu. Nur teilw.impl. */
#ifdef EXT_SIMU_SERV
  extern DOUBLE  best_ges_note;    /* Note der BestAK seit "reset_simu_erg"   */
                                   /* best_ges_note = -1: Keine guelt.Werte   */
#endif /* EXT_SIMU_SERV */


/* ------------------------------ aus mbf_sim: ------------------------------ */
#ifdef PAR_POP_VERS
  extern INT     delayMSec;        /* delay after MBF execution in msec       */
#endif /* PAR_POP_VERS */


/* ------------------------------ aus matlab_koppl: ------------------------- */
#ifdef EXT_MATLAB_SIMU
  extern INT    matlabNoConvCtr;   /* counts matlab runs without convergence  */
  extern INT    count_evals;
  extern DOUBLE best_res;
  extern BOOL   extractCaseFile;   /* TRUE: "doMatlabSim" causes the inter-  */
                                   /* face to write a matpower case file     */
#endif /* EXT_MATLAB_SIMU */


/* ------------------------------ aus vgl/aehnlvgl: ------------------------- */
#ifdef MIT_AEHNL_CHECK
  extern INT  *iParSign,           /* Anz.signifikanter Stellen d. Int-Params */
              *rParSign;           /* Anz.signifikanter Stellen d.Real-Params */
  extern INT   tabLen;             /* Laenge der Tabelle f. AK-Werte-Speicher */
#endif /* MIT_AEHNL_CHECK */




/* ========================== Exportierte Routinen: ========================= */

/* ---------------------- Routinen aus Modul: simucmen ---------------------- */
extern void   simu_cmenue        (GSTATE *ret_state);


/* ---------------------- Routinen aus Modul: simu -------------------------- */
extern GSTATE get_ak_params      (INT                 *iParams,
                                  DOUBLE              *rParams,
                                  CHAIN_ELEM_PTR_TYPE  ak_ptr,
		                  BOOL                 interaktiv);

extern GSTATE simulator          (DOUBLE              *note,
                                  BOOL                 interaktiv,
                                  CHAIN_ELEM_PTR_TYPE  ak_ptr);

extern BOOL   getSimuListEntry   (CHAIN_ELEM_PTR_TYPE *akPtr,
				  INT                 *elterIdx,
                                  DOUBLE              *note,
				  INT                  idx);
 
extern void   reset_simu_erg     (void);

extern BOOL   print_exp_data     (FILE    *out_file);

extern BOOL   simu_gsw_init      (void);

extern BOOL   simuInit           (BOOL    *ext_sim_mod_ok);

#if defined(GUI_MODE) || defined(EXT_SIMU_SERV)
  extern void bewert_data_to_int_data  (DOUBLE  gesamt_note);

  extern void bewert_data_to_best_data (DOUBLE  gesamt_note);
#endif /* GUI_MODE or EXT_SIMU_SERV */


/* ---------------------- Routinen aus Modul: mbf_sim ----------------------- */
extern BOOL   do_mbf_sim         (DOUBLE *simu_erg, 
				  INT     r_par_anz, 
				  DOUBLE *r_params);

extern void   init_mbf_sim       (void);


/* -------------------- Routinen aus Modul: cec_mbf_sim --------------------- */
extern BOOL   do_cec05_mbf_sim   (DOUBLE *result, 
                                  INT     parAmount,
                                  INT     fctNr,
                                  DOUBLE *param);


/* -------------------- Routinen aus Modul: matlab_koppl -------------------- */
#ifdef EXT_MATLAB_SIMU
  extern BOOL doMatlabSim (DOUBLE              *erg_werte,
                           CHAIN_ELEM_PTR_TYPE  chrPtr);
#endif /* EXT_MATLAB_SIMU */


/* ---------------------- Routinen aus Modul: ext_sim  ---------------------- */
extern BOOL   do_ext_sim          (DOUBLE *erg_werte,
                                   INT    *i_params,
                                   DOUBLE *r_params);

extern BOOL   ext_simu_hochfahren (BOOL   mit_fin);

extern void   update_ext_simu_tmo (INT    tmo_zeit_in_sec);

extern void   terminate_ext_simu  (void);

#ifdef GUI_MODE
  extern BOOL restart_ext_sim     (void);

  extern void kleiner_ext_restart (void);
#endif /* GUI_MODE */


/* ---------------------- Routinen aus Modul: simu_gsw ---------------------- */
extern void   behandle_neue_note  (CHAIN_ELEM_PTR_TYPE  ak_ptr,
                                   INT                  znr, 
                                   DOUBLE               neu_fit_note);

extern BOOL   get_ak_and_check_ak (BOOL  *must_simu,
                                   INT    lfd_nr,
                                   INT    guete);

extern void   perform_simu        (BOOL   simu_oben,
                                   INT    znr);

extern BOOL   save_simu_erg       (char  *fspec,
                                   BOOL   neu_file,
                                   BOOL   mitIndex);

extern INT    show_simu_erg       (INT    znr,
                                   BOOL   mitIndex);

extern void   prep_status_anzg    (void);

#ifdef GUI_MODE
  extern GSTATE ak_mem_neu_bewert (BOOL   clr_simu_flag);

  extern INT  zeige_simu_bew      (INT    znr,
                                   BOOL   mitIndex);
#endif /* GUI_MODE */
#if defined(GUI_MODE) || defined(VISU_MODE)
  extern INT  zeige_evo_best_bew  (INT    znr,
                                   BOOL   mitIndex);
#endif /* GUI_MODE oder VISU_MODE */

#if defined(GNUPLOT) && defined (GUI_MODE)
  extern void plot_akt_wert       (INT    krit_nr);
#endif /* GNUPLOT und GUI_MODE */



/* ------------------ Routinen aus dem Roboter-Subpackage: ------------------ */

/* ---------------------- Routinen aus Modul: rob_sim ----------------------- */
extern BOOL do_rob_sim         (DOUBLE              *simu_erg_werte,
                                CHAIN_ELEM_PTR_TYPE  ch_ptr,
                                BOOL                 interaktiv);

extern BOOL update_rob_task    (void);

extern BOOL ladekinematik      (const char *file_name,
    	                        INT         znr);

extern BOOL hole_obs           (const char *file_name,
	                        INT         znr);

extern void hindernismanagement (void);

extern BOOL write_code_file    (const char *fspec, 
                                INT         guete, 
                                INT         lfd_nr);


/* ---------------------- Routinen aus Modul: rob_gsw ----------------------- */
extern void zeige_start_ziel   (INT         znr,
                                BOOL        interaktiv);


/* ---------------------- Routinen aus Modul: rob_dial ---------------------- */
extern BOOL teach_in           (void);


/* ---------------------- Routinen aus Modul: lsk_grep ---------------------- */
extern BOOL lesak_p_test       (CHAIN_ELEM_PTR_TYPE *ch_ptr,
                                GSTATE              *ret_state);

