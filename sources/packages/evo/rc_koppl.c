/*******************************************************************************
HyGLEAM                                                                   V2.2.0

                                 H y G L E A M
       Hybrid GeneraL purpose Evolutionary learning Algorithms and Methods

                       Package: EVOlution
                        Module: Rosenbrock-Complex-KOPPL

Package: evo                   File: rc_koppl.c 
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Der Modul dient der Ankopplung von den beiden durch das ITEM zur Verfuegung ge-
stellten lokalen Suchverfahren (LSV) "Rosenbrock" und "Complex".

Zur Ankopplung werden 2 Schnittstellen benoetigt:
1. Aufruf des LSVs aus der Evo heraus
2. Aufruf von Simulator und Bewertung aus dem LSV heraus.

Bei der ersten Schnittstelle wird eine AK uebergeben und eine lokal optimierte 
Bewertung erwartet. Bei gesetztem "lamarck"-Parameter erfolgt auch ein Update 
der Aktionen der AK. Da die beiden LSVs Real-Vektoren erwarten, erfolgt eine
Abbildung der Parameter der Aktionen der AK in einen Real-Vektor ausgehend 
vom "appl_code" und dem Typ der AK:

Fall MATH_FKT_APPL, CEC_MATHFKT_APPL, GADO_APPL oder GAMA_APPL ("param_bew" = TRUE):
====================================================================================
  Fall statische AKs:
  ===================
  Abbildung der Aktionen in der im Genmodell definierten Aktions-
  reihenfolge: Zuerst alle Integer und dann alle Real-Parameter der AK.
  i_par_anf = 0

  Fall dynamische AKs ("ext_sim_und_dyn_ak" = TRUE):
  ==================================================
  Abbildung der Aktionen in der in der AK definierten Aktionsreihenfolge: 
  Zuerst alle Integer und dann alle Real-Parameter der AK.

  In beiden Faellen werden die Integer- und Realparameter vollstaendig
  nach i_params und r_params abgebildet. i_params enthaelt auch Integer-
  Parameter mit so kleinem Wertebereich, dass sie nicht an den Rosenbrock
  weitergeleitet werden. Diese Filterung geschieht beim Schreiben des 
  INI-Files. Rosenbrock sieht also nur die Integerwerte mit ausreichend
  grossem Wertebereich, sodass "param" weniger Integerparameter enthalten 
  kann, als in i_params stecken. Bei der bei AK-Update notwendigen Rueck-
  abbildung wird dies beruecksichtigt, indem auf die in "ipar_fuer_lsv"
  hinterlegte Information (TRUE=grosser Wertebereich) zurueckgegriffen 
  wird.

               0   1         len i_par_anf
               v   v         v   v       
              +---+---+-----+---|----+-----+----+
    i_params: |len|ac | ... |ac |a i | ... |a i |
              |   |  1|     |  n| 1 1|     | n 1|
              +---+---+-----+---|----+-----+----+
              \                  \             //
               \                  akt_ipar_anz-/
                -- all_iwerte_anz -------------


    r_params: akt_rpar_anz Werte


               0               rosen_ipar_anz
               v               v
              +---------------|----------------+
    param:    | int-parameter | real-parameter |
              +---------------|----------------+

  
Fall LESAK_APPL oder ADD_APPL ("param_bew" = FALSE) oder MATLAB_APPL:
=====================================================================
  Abbildung der Aktionen in der in der AK vorgefundenen Aktionsreihenfolge:
  zuerst die Integer- und dann die Real-Parameter einer Aktion. Auch hier 
  werden nur Integerwerte mit ausreichend grossem Wertebereich an den Rosenbrock
  weitergegeben, was beim abschliessenden AK-Update beruecksichtigt wird.

Die zweite Schnittstelle dient der Bewertung fuer das LSV. Dabei erfolgt je nach
benutzter Simulation eine Rueckabbildung der zu testenden Werte in die AK ("do_
rob_sim", "do_appl_sim") oder es koennen die Paramter direkt weitergereicht wer-
den ("do_mbf_sim", "do_ext_sim"). Bei Integer-Parametern wird dabei gerundet.


Integration der MATLAB_APPL:
============================
Die aktuelle (2018) Integration des matlab-Simulators ist fuer die auf MatPower 
zugeschnittene Ankopplung des externen matlab-Simulators gedacht. Diese Kopplung 
beruht auf der Simulation eines Chromosoms, aus dessen Gene Name-Value-Paare ge-
bildet und an den MatPower-Simulator weitergegeben werden. Aufrufe der Bewertung 
durch LSVs basierend auf einem Wertevektor sind so nicht moeglich. Entweder wird
den Werte der passende Parameternamen entsprechend einem Muster-Chromosom zugeord-
net oder man nutzt die vorhandene Rueckuebersetzung in ein Chromosom und simuliert 
dann damit. Letzteres wurde durchgefuehrt. Dieses Vorgehen funktioniert auch bei
gemischt-ganzzahligen Werten und bei kleinen Integer-Bereichen, siehe die ent-
sprechende Doku. Ausserdem war es sehr einfach umzusetzen.



Die Routinen im einzelnen:

DOUBLE do_rc_1start (CHAIN_ELEM_PTR_TYPE *ak_ptr,
                     BOOL                 lamarck,
                     BOOL                 rosen,
                     GSTATE              *erg_status);
          Auf die im "ak_ptr" uebergebene AK wird bei gesetztem "rosen" das 
          Rosenbrock-Verfahren und bei nicht gesetztem das COMPLEX-Verfahren
          aus MODOS (ITEM, Uni Bremen) angewandt und die Note als Funktions-
          ergebnis abgeliefert. Wenn die Note durch das LSV verbessert wurde,
          wird die neue Note in die AK eingetragen. Wenn "lamarck" TRUE ist,
          wird die AK ausserdem an das gefundene lokale Optimum angepasst.
          Der Startvektor stammt aus der durch "ak_ptr" vorgegebenen AK.
          Rosenbrock:
          Der Startvektor wird in die Datei "objfct.ini" zusammen mit den 
          Grenzwerten geschrieben.
          COMPLEX:
          Der Startvektor wird in die Datei "objfct_complex.ini" zusammen mit 
          den Grenzwerten geschrieben. Die Datei "complex.ini" enthaelt nur den
          Eintrag "start_complex_method 0", der besagt, dass ein Startpunkt in
          der Ini-Datei steht und die restlichen Vertices durch die COMPLEX-SW
          zufaellig bestimmt werden.
          Bei Fehlern wird KEINE_NOTE und ein entsprechend gesetzter Return-
          status "erg_status" abgeliefert.

DOUBLE do_compl_start (INT    *ini_idx,
                       BOOL    lamarck,
                       GSTATE *erg_status);
          Fuehrt das COMPLEX-Verfahren mit mehreren Startpunkten durch und die 
          Note als Funktionsergebnis ab. In "ini_idx" wird der "ini_ptr"-Feld-
          index der AK abgeliefert, die das Ergebnis darstellt. Wenn die Note 
          durch das LSV verbessert wurde, wird die neue Note in die AK eingetra-
          gen. Wenn "lamarck" TRUE ist, wird die AK ausserdem an das gefundene 
          lokale Optimum angepasst. Bei Fehlern wird KEINE_NOTE und ein ent-
          sprechend gesetzter Returnstatus "erg_status" abgeliefert.
          Die in der package-globalen Variable "ini_ptr" hinterlegten AKs bilden
          den Start-Complex, der in der Datei "complex.ini" geschrieben wird.
          Der darin noch enthaltene Eintrag "start_complex_method 1" besagt,
          dass das Verfahren mit einem kompletten Complex statt mit nur einem 
          Startpunkt gestartet wird. Daher spielen die Startwerte der Datei 
          "objfct_complex.ini" keine Rolle. Aus ihr werden aber die Grenzwerte 
          benutzt.
          Anmerkung: Die Funktion wird derzeit nicht benutzt.

double objfct_complex (double* param);
          Die Routine wird (trotz ihres Namens) von beiden LSVs zur Bewertung 
          eines veraenderten Wertevektors aufgerufen.
          Die "akt_par_anz" (modul-lokale Variable) Parameter "param" werden
          bei nicht gesetztem "param_bew" in eine Kopie der AK zurueckabgebildet.
          Die AK bzw. der Parametervektor wird simuliert und mit "ak_bewerten" 
          bewertet. Das an eine Minimumsuche angepasste Ergebnis wird abge-
          liefert.
          Wenn der globale Fehlerstatus GLEAM_FATAL ist, wird die Routine nicht 
          ausgefuehrt und ab RNOTEN_MAX mit der Anzahl fehlerhafter Aufrufe hin-
          tereinander steigende Notenwerte abgeliefert, damit das LSV abbricht.
          Bei auftretenden Fehlern werden ab RNOTEN_MAX steigende Notenwerte 
          abgeliefert. Ausserdem wird die globale Fehler-Variable "err_state"
          bei fatalem Fehler gesetzt.

BOOL   prepare_rc_koppl (INT  i_par_ges_anz, 
                         INT  r_par_ges_anz);
          Legt entsprechend den Groessenvorgaben des Handlungsmodells bei sta-
          tischen AKs folgende Datenobjekte an: "best_params", 
          Ausserdem wird mit Hilfe der statischen Routine "write_lsv_ini_file" 
          die Datei "objfct_complex.ini" fuer das COMPLEX-Verfahren angelegt. 
          "minIParRange" wird auf MIN_IPAR_BEREICH obzw bei Zusatz-Applikation
          GORBA_PERM_APPL auf MIN_IPAR_GORBA gesetzt.
          Liefert TRUE ab, wenn alles gut ging. 

GSTATE set_rc_limits (INT     new_c_max_iter,
                      INT     new_r_max_iter,
                      DOUBLE  error_goal,
                      DOUBLE  step_size);
          Speichert "new_c_max_iter" in der modul-lokalen Variable "max_iterc" 
          zur weiteren Verwendung fuer das "complex.ini"-File, wenn "new_c_max_
          iter" groesser als 0 ist. Wenn "new_r_max_iter" groesser als 0 ist, 
          wird die Datei "rosenbrock.ini" mit "new_r_max_iter", "error_goal" 
          und "step_size" neu angelegt.
          Bei Fehlern beim Anlegen der Datei werden die entsprechenden Fehler-
          meldungen erzeugt und GLEAM_ERROR abgeliefert.
          
--------------------------------------------------------------------------------
Stand der Implementierung:
Complex-Verfahren mit einem Start-Complex: 
Wenn das Handlungsmodell Parameter mit zu kleinem Integer-Wertebereich oder 
fixierte Paramter vorsieht, werden diese zwar ausgefiltert, aber der Start-
complex enthaelt zu viele Vertices. Das fuehrt zum Crash der ITEM-SW.    28.4.01
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. Neue LSV-Praezisionen fuer "rosen_ini".                          WJ 11.10.00
 2. Startwertlimitierung auf die Grenzen.                            WJ 10.11.00
 3. Umbenennung Baldwin -> Lamarck.                                  WJ 06.12.00
 4. Vom Rosembrock zurueckerhaltene Int- u. Realparameter werden auf 
    die Parameterbereichsgrenzen begrenzt. IntParams werden nur ab 
    einem ParamBereich ab MIN_IPAR_BEREICH mit dem Rosenbrock opti-
    miert.                                                           WJ 30.01.01
 5. Auch bei Parameter-Bewertung wird obige Begrenzung durchgefuehrt.
    Integration dyn. extern zu simulierender AKs.                    WJ 21.02.01
 6. Integration des COMPLEX-Algorithmus.                             WJ 02.05.01
 7. Integration des Updates fuer Complex und Rosenbrock.             WJ 14.05.01
 8. Fall "ADD_APPL" fuer ITEM richtig auf "param_bew" abgebildet.    WJ 03.07.01
 9. "check_and_copy_rpar" wird statt nur beim Rueckkopieren in die 
    AK auch beim INI-File-Schreiben aufgerufen.                      WJ 10.07.01
10. "check_and_copy_rpar" korrigiert und Formatanpassung beim 
    Schreiben der Grenzen.                                           WJ 25.04.03
11. Include von "rosencon.h" und "complbox.h" bei gesetztem Schalter
    CALL_LSV.                                                        WJ 08.04.04
12. Erweiterung fuer die adaptive direkte Integration, u.a. neue
    Routine "set_lsv_limits". Der Modul bleibt fuer altes HyGLEAM 
    (Dis-Version) verwendbar.                                        WJ 22.07.04
13. "allg_max_iter" von "adapt_di" wegen Compl-max_iter uebernommen. WJ 06.10.04
14. Erweiterung der Routine "set_lsv_limits" um den Parameter 
    "step_size" fuer OPAL/V.                                         WJ 05.01.05
15. Linux-Portierung: Aus "round" wird "i_round".                    WJ 12.05.05
16. GORBA: Um auch kleine Integerparameterbereiche verarbeiten zu 
    koennen, wird statt MIN_IPAR_BEREICH die Variable "minIParRange"
    eingefuehrt, die bei GORBA_PERM_APPL den Wert von MIN_IPAR_GORBA
    erhaelt und sonst MIN_IPAR_BEREICH. Bei GORBA-Anwendungen er-
    folgt eine Warnungsmeldung. Alles in "prepare_lsv_koppl".        WJ 02.02.06
17. In "objfct_complex" wird die Einhaltung der Grenzen der Integer-
    parameter erzwungen. Relevant fuer GORBA:                        WJ 02.02.06
18. Erweiterung fuer n LSVs. Das Rosenbrock-Ini-File wird neu ange-
    legt und es gibt keine vordefinierten mehr. Umbenennung des 
    Moduls in "rc_koppl" und der Routinen "prepare_lsv_koppl" in
    "prepare_rc_koppl" und "do_lsv_1start" in "do_rc_1start".        WJ 28.06.06
19. Eine Reihe auskommentierter Testausgaben auf Schalter umgestellt.WJ 24.08.06
20. Integration der MBFs der CEC05-Sammlung (CEC_MATHFKT_APPL).      WJ 26.10.07
21. Bei CEC_MBF Pflege von "cec_best_fval".                          WJ 06.11.07
22. Anpassung an neue Datentypen fuer Int- und Real-Parameter
    (neues "chaindef.h" fuer 64 bit)                                 WJ 03.01.08
23. Streichung des Includes von "evoIntg.h", da dies bereits durch 
    "evoInts.h" includiert wird.                                     WJ 05.06.08
24. "i_params" und "r_arams" werden an "get_ak_params" uebergeben.   WJ 11.06.08
25. "write_lsv_ini_file": Deklaration von "fspec", als "const".      WJ 20.04.12
26. Vereinfachung der Aufrufschnittstelle der beiden LSVs durch
    Verlagerung der Filenamendefinitionen in die jeweiligen .h-Files.
    Dadurch wird ein parameterloser Aufruf ermoeglicht.              WJ 20.04.12
27. Renovierung: Ungenutzte Returnvalues in "read_sim_anz_from_dat"
    und "dat_killer" beseitigt.                                      WJ 20.04.16
28. Anpassung des "write_to_log"-Aufrufs.                            WJ 19.12.16
29. Fehlermeldung NOT_ENOUGH_MEM korrigiert.  Reunion of "evoIntg.h" 
    and "evoInts.h" in "evoInt.h".                                   WJ 30.06.17
30. MATLAB_APPL: In do_rc_1start() und objfct_complex() wird eine
    Chromosomenkopie angelegt und ueber die Chromsoomenschnittstelle 
    simuliert.                                                       WJ 29.06.18
31. Anpassung der Aufrufe von get_act_descr().                       WJ 28.06.19
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 28.06.2019
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "lgsw.h"
#include "bew.h"
#include "hmod.h"
#include "appl.h"
#include "simu.h"
#include "evo.h"
#include "evoInt.h"

#include EVO_TEXTS


#define PFILE  0       /* Modos-Logfile: O: keins, 1: anlegen                 */

#undef SS_TST
#define CALL_LSV       /* Gesetzt: Aufruf der MODOS-Routinen, sonst wird die  */
                       /* Schnittstelle intern wegen Debug befriedigt.        */
#undef PARAM_KOR       /* Korrektur bei Parametergrenzueberschreitungen       */
#undef SS_LOG          /* Logging der Rosenbrock/Complex-Aufrufe              */
#undef COMPL_LOG       /* Logging bei "objfct_complex"                        */

#undef HY_DIS          /* Gesetzt: HyGLEAM und Dis.: ALL1COMPL als Kombi-     */
                       /* Nachopt. (2 Laeufe pro Opt/Nachopt) Jetzt unbenutzt.*/

#ifdef CALL_LSV
  #include "rosencon.h"
  #include "complbox.h"
#else /* kein CALL_LSV: wird sonst aus "rosencon.h" u."complbox.h" importiert:*/
  #define ROSEN_INI_FNAME       "rosenbrock.ini"
  #define ROSEN_STARTW_FNAME    "objfct.ini"
  #define COMPL_STARTW_FNAME    "complex.ini"
  #define QF_FKT_NAME           "objfct"
  static int ComplexIndex;       /* sonst aus complbox.h importierte Variable */
#endif /* kein CALL_LSV */



/* =================== vom lsv-Package (MODOS) aufgerufen: ================== */
double objfct_complex (double* punkt);



/* ======================== Package-globale Variable: ======================= */
INT     anz_compl_pkte;        /* Anzahl der Complex-Startpunkte              */
CHAIN_ELEM_PTR_TYPE *ini_ptr;  /* Complex-Startpunkte                         */



/* ============================ Lokale Konstante: =========================== */
#define MIN_IPAR_BEREICH      470.0
#define MIN_IPAR_GORBA         10.0
#define ERG_DAT_FSPEC         "ergebnis.dat"
#define COMPL_INI_FNAME       "objfct_complex.ini"
#define INIF_HDR_LINE         "function_name "QF_FKT_NAME"\n\n"
#define INIF_DEF_LINE         "def X%d with\n"
#define INIF_START_LINE       "   start = %.8g\n"
#define INIF_LB_LINE          "   lowerbound = %g\n"
#define INIF_UB_LINE          "   upperbound = %g\n"
#define INIF_ISTART_LINE      "   start = %d\n"
#define INIF_ILB_LINE         "   lowerbound = %d\n"
#define INIF_IUB_LINE         "   upperbound = %d\n"
#define INIF_END_LINE         "enddef\n\n"
#define START_COMPL_LINE1     "start_complex_method %d\n"
#define START_COMPL_LINE2     "norm 0\n"
#define START_COMPL_LINE3     "pfile %d\n"        /* kontrolliert LSV-Log-File */
#define START_COMPL_LINE4     "max_iter %d\n\n"
#define START_COMPL_ANF       "startcomplex\n"
#define START_COMPL_END       "end\n"
#define OHNE_START_WERTE      FALSE
#define MIT_START_WERTE       TRUE



/* ======================== Lokale Typdeklarationen: ======================== */



/* ============================ Lokale Variable: ============================ */
static CHAIN_ELEM_PTR_TYPE *ak_copy_ptr = NULL,
                            best_ak_ptr = NULL;
static FILE    *ini_file,          /* ini-File fuer MODOS                     */
               *erg_file;          /* Ergebnisfile fuer MODOS                 */
static DOUBLE   best_note,         /* Beste Note einer lokalen Optimierung    */
                minIParRange;      /* Mindestwertebereich f. Integerparameter */
static LONG     sim_anz;           /* Anzahl der Simulationen eines LSV-Calls */
static INT      akt_par_anz,       /* Aktuelle Laenge des "param"-Vektors     */
                rosen_ipar_anz,    /* IntParAnz mit ausr.grossem Wertebereich */
                bad_sim_ctr,       /* Zaehlt konsekutive Simu-Abbrueche       */
                ak_idx,            /* AK-Index in "ini_ptr" u. "ak_copy_ptr"  */
                best_ak_idx;       /* Index der besten AK des LSV-Laufs       */
static INT      max_iterc = 10000; /* Iterationslimit fuer Complex-Verfahren  */
static BOOL     param_bew,         /* TRUE, wenn Param- statt AK-Bewertung    */ 
                mit_ak_update,     /* gibt lok.Routinen den Wert von "lamarck"*/
                ext_sim_und_dyn_ak,/* TRUE, wenn dyn.AK extern zu simulieren  */
                mit_start_compl;   /* TRUE, wenn ein Init-Startcomplex vorgeg.*/

static DOUBLE  *best_params = NULL;/* Parameter der BestAK eines LSV-Laufs    */
static BOOL    *ipar_fuer_lsv;     /* TRUE: IParam mit ausr.gross.Wertebereich*/

#if defined (HY_DIS)
  static INT   c1all_qfa_anz=0;  /* BewAnz d.letzten Complex mit 1 StartCompl */
#endif /*  HY_DIS */             /* vorher package-global fuer "evo_anzg".    */



/* ============================ Lokale Routinen: ============================ */
static BOOL   write_lsv_ini_file    (const char          *fspec,
                                     BOOL                 mit_start_werte,
                                     CHAIN_ELEM_PTR_TYPE  ak_ptr);

static GSTATE lsv_erg_auswertung    (CHAIN_ELEM_PTR_TYPE *ak_ptr);

static void   check_and_copy_ipar   (INT    *erg_wert,
                                     INT     in_wert,
                                     INT     ug,
                                     INT     og);

static void   check_and_copy_rpar   (DOUBLE *erg_wert,
                                     DOUBLE  in_wert,
                                     DOUBLE  ug,
                                     DOUBLE  og);

static INT    read_sim_anz_from_dat (void);




/* =============================== do_rc_1start ============================= */
DOUBLE do_rc_1start (CHAIN_ELEM_PTR_TYPE *ak_ptr,
                     BOOL                 lamarck,
                     BOOL                 rosen,
                     GSTATE              *erg_status)
{
   CHAIN_ELEM_PTR_TYPE  item_ptr;
   ACT_DESCR_PTR_TYPE   this_act_ptr;
   INT                  ai,           /* Aktionscode                          */
                        par_ctr,      /* Parameter-Zaehler fuer Test ohne LSV */
                        ii;
   BOOL                 ok;
 
   /* do_rc_1start */
   ext_sim_und_dyn_ak = mit_ext_simu && dyn_aks;
   mit_start_compl    = FALSE;
   ak_idx             = 0;

   if (rosen)
   { /* Rosenbrock-Ini-File anlegen */
      /* ------------- Ini-File (Grenz- und Startwerte) anlegen: ------------ */
      if (!write_lsv_ini_file (ROSEN_STARTW_FNAME, MIT_START_WERTE, *ak_ptr))
      { /* File nicht geschrieben */
         *erg_status = fatal (EVO_RC_KOPPL, 1, NO_INFO, NO_INFO, ROSEN_STARTW_FNAME, 
                              WRITE_ERR);
         return (KEINE_NOTE);
      } /* File nicht geschrieben */
   } /* Rosenbrock-Ini-File anlegen */
   else
   { /* Complex-Ini-Files schreiben */
      /* ------------- Ini-File (Grenz- und Startwerte) anlegen: ------------ */
      if (!write_lsv_ini_file (COMPL_INI_FNAME, MIT_START_WERTE, *ak_ptr))
      { /* File nicht geschrieben */
         *erg_status = fatal (EVO_RC_KOPPL, 2, NO_INFO, NO_INFO, COMPL_INI_FNAME, 
                              WRITE_ERR);
         return (KEINE_NOTE);
      } /* File nicht geschrieben */

      /* --------------- Leeres Start-Complex-File schreiben: --------------- */
      if (!open_file (&ini_file, COMPL_STARTW_FNAME, "wt"))
      { /* File nicht zu oeffnen */
         *erg_status = fatal (EVO_RC_KOPPL, 3, NO_INFO, NO_INFO, COMPL_STARTW_FNAME, 
                              FILE_NOT_OPEN);
         return (KEINE_NOTE);
      } /* File nicht zu oeffnen */
      fprintf (ini_file, START_COMPL_LINE1, 0);     /* 0 = kein Start-Complex */
      fprintf (ini_file, START_COMPL_LINE2);
      fprintf (ini_file, START_COMPL_LINE3, PFILE);
      fprintf (ini_file, START_COMPL_LINE4, max_iterc);
      fclose (ini_file);
   } /* Complex-Ini-Files schreiben */
   
   /* --------------- LSV (Complex oder Rosenbrock) aufrufen: --------------- */
   if (*erg_status == GLEAM_OK)
   { /* Ini-File erfolgreich geschrieben */
      if (param_bew && (appl_code != MATLAB_APPL)) // AK-Copy, wenn  (!param_bew || (appl_code == MATLAB_APPL))
         ok = TRUE;
      else
      { /* Kopie der AK fuer objfct anlegen */
         if (ak_copy_ptr[0] != NULL)      /* Zur Sicherheit: sollte schon ge- */
            *erg_status = delete_chain (&(ak_copy_ptr[0]));   /* loescht sein */
         ok = copy_chain (&(ak_copy_ptr[0]), *ak_ptr, KEINE_URKETTE, WITH_MEM_CHECK,
                          erg_status);
      } /* Kopie der AK fuer objfct anlegen */
      if (ok)
      { /* Alles fertig fuer LSV */
         best_note     = -1.0;                                 /* Initialwert */
         sim_anz       = 0;
         bad_sim_ctr   = 0;
         mit_ak_update = lamarck;
#ifdef CALL_LSV
  #ifdef SS_LOG
         sprintf (buf1, "do_rc_1start: Vor  dem Aufruf vom LSV. rosen=%d", rosen);
         write_to_log (buf1, CLOSE_LOG);
  #endif /* SS_LOG */
         if (rosen)
            rosencon_original ();
         else
            complex_box (); 
  #ifdef SS_LOG
         write_to_log ("do_rc_1start: Nach dem Aufruf vom LSV.", CLOSE_LOG);
  #endif /* SS_LOG */
#else /* kein CALL_LSV */
         par_ctr = 0;
         if (param_bew)
         { /* Param-Bew */
            for (ii = i_par_anf;  ii < all_iwerte_anz;  ii++)
               if (ipar_fuer_lsv[ii])           /* Wertebereich gross genug ? */
                  best_params[par_ctr++] = i_params[ii];
            for (ii = 0;  ii < akt_rpar_anz;  ii++)
               best_params[par_ctr++] = r_params[ii];
         } /* Param-Bew */
         else
         { /* AK-Bewertung */
            item_ptr = (*ak_ptr)->u_el.next_action;
            while (item_ptr != NULL)
            { /* all along the chain */
               ai = item_ptr->u_el.activity;
               for (ii = 0;  ii < int_pro_aktion[ai];  ii++)
               { /* Int-Parameter */
                  get_act_descr (&this_act_ptr, (SHORT)ai, WITHOUT_GENECODE_CHECK);
                  if (this_act_ptr->i_par_descr[ii].par_bereich >= minIParRange)
                     best_params[par_ctr++] =               
                        item_ptr->u_el.u_xel.u_act.i_params[ii];
               } /* Int-Parameter */
               for (ii = 0;  ii < real_pro_aktion[ai];  ii++)
                  best_params[par_ctr++] =                  /* Real-Parameter */
                      item_ptr->u_el.u_xel.u_act.r_params[ii];
               item_ptr = item_ptr->u_el.next_action; /* zur naechsten Aktion */
            } /* all along the chain */
         } /* AK-Bewertung */
         objfct_complex (best_params);                       /* Test fuer LSV */
         best_note += 1.0;
#endif /* kein CALL_LSV */
         if (err_state == GLEAM_OK)
            *erg_status |= lsv_erg_auswertung (ak_ptr);     /* LSV fehlerfrei */
         else
         { /* Fehler beim LSV-Aufruf */
            best_note    = KEINE_NOTE;
             *erg_status |= err_state;
         } /* Fehler beim LSV-Aufruf */
      } /* Alles fertig fuer LSV */
      else
         *erg_status = fatal (EVO_RC_KOPPL, 12, NO_INFO, NO_INFO, CHROMOSOM_TXT, NOT_ENOUGH_MEM);
      if (ak_copy_ptr[0] != NULL)
         *erg_status = delete_chain (&(ak_copy_ptr[0]));
   } /* Ini-File erfolgreich geschrieben */

   if (*erg_status == GLEAM_OK)
      return (best_note);
   else
      return (KEINE_NOTE);
} /* do_rc_1start */



/* ============================= do_compl_start ============================= */
DOUBLE do_compl_start (INT    *ini_idx,
                       BOOL    lamarck,
                       GSTATE *erg_status)
{
   CHAIN_ELEM_PTR_TYPE  item_ptr;
   ACT_DESCR_PTR_TYPE   this_act_ptr;
   DOUBLE               rwert;
   INT                  ai,                              /* Aktionscode       */
                        par_ctr, 
                        ii, ak_ii;
   BOOL                 ok = TRUE;

   /* do_compl_start */
   if (*erg_status != GLEAM_OK)
      return (KEINE_NOTE);
   if (dyn_aks)
   { /* Dynamische AK = unbewertbar */
      fatal (EVO_RC_KOPPL, 20, NO_INFO, NO_INFO, "", DYN_AKS_NIX_1COMPL);
      return (KEINE_NOTE);
   } /* Dynamische AK = unbewertbar */

   ext_sim_und_dyn_ak = FALSE;
   mit_start_compl    = TRUE;
   ak_idx             = 0;                    /* Defaultwert fuer "param_bew" */
   best_ak_idx        = 0;                    /* Defaultwert fuer "param_bew" */

   /* -------------------- Start-Complex-File schreiben: -------------------- */
   if (!open_file (&ini_file, COMPL_STARTW_FNAME, "wt"))
   { /* File nicht zu oeffnen */
      *erg_status = fatal (EVO_RC_KOPPL, 21, NO_INFO, NO_INFO, COMPL_STARTW_FNAME, 
                           FILE_NOT_OPEN);
      return (KEINE_NOTE);
   } /* File nicht zu oeffnen */
   fprintf (ini_file, START_COMPL_LINE1, 1);    /* 1 = enthaelt Start-Complex */
   fprintf (ini_file, START_COMPL_LINE2);
   fprintf (ini_file, START_COMPL_LINE3, PFILE);
   fprintf (ini_file, START_COMPL_LINE4, max_iterc);
   fprintf (ini_file, START_COMPL_ANF);
   for (ak_ii = 0;  (ak_ii < anz_compl_pkte) && ok;  ak_ii++)
   { /* Alle Startvertices */
      if (ini_ptr[ak_ii] == NULL)
      { /* Start-AK fehlt! */
         *erg_status = fatal (EVO_RC_KOPPL, 22, NO_INFO, NO_INFO, "", AK_EXPECTED);
         ok = FALSE;
      } /* Start-AK fehlt! */
      else
      { /* Start-AKs rausschreiben und bei AK-Bewertung Kopien anlegen */
         if (param_bew)
         { /* --------- Parameter-Bewertung: INI-File schreiben: ------------ */
            par_ctr = 0;
	    ok = (get_ak_params (i_params, r_params, ini_ptr[ak_ii], FALSE) == GLEAM_OK);
            if (ok)
            { /* Param ok */
               for (ii = i_par_anf;  (ii < all_iwerte_anz) && ok;  ii++)
                  if ((og_iparams[ii] - ug_iparams[ii]) >= minIParRange)
                  { /* Int-Param mit ausreich. grossem Wertebereich schreiben */
                     ipar_fuer_lsv[ii] = TRUE;
                     ok = fprintf (ini_file, " %d", i_params[ii])> 0;
                     par_ctr++;
                  } /* Int-Param mit ausreich. grossem Wertebereich schreiben */
                  else
                     ipar_fuer_lsv[ii] = FALSE;
               rosen_ipar_anz = par_ctr;
               for (ii = 0;  (ii < akt_rpar_anz) && ok;  ii++)
               { /* Real-Parameter rausschreiben */
                  rwert = r_params[ii];
                  if (rwert > og_rparams[ii] - 0.000001)
                     rwert = og_rparams[ii] - 0.000001;
                  if (rwert < ug_rparams[ii] + 0.000001)
                     rwert = ug_rparams[ii] + 0.000001;
                  ok = fprintf (ini_file, " %.8g", rwert) > 0;
                  par_ctr++;
               } /* Real-Parameter rausschreiben */
            } /* Param ok */
            akt_par_anz = par_ctr;
            fprintf (ini_file, "\n");
         } /* Parameter-Bewertung: INI-File schreiben */
         else
          { /* -------------- AK-Bewertung: INI-File schreiben: -------------- */
            item_ptr = ini_ptr[ak_ii]->u_el.next_action;
            while (item_ptr != NULL)
            { /* all along the chain */
               ai = item_ptr->u_el.activity;
               if (get_act_descr (&this_act_ptr,(SHORT)ai, WITHOUT_GENECODE_CHECK) == GLEAM_OK)
               { /* Aktionsdescriptor erhalten */
                  for (ii = 0;  (ii < this_act_ptr->i_par_anz) && ok;  ii++)
                     if (this_act_ptr->i_par_descr[ii].par_bereich >= minIParRange)
                        ok = fprintf (ini_file, " %d", 
                                      item_ptr->u_el.u_xel.u_act.i_params[ii])>0;
                  for (ii = 0;  (ii < this_act_ptr->r_par_anz) && ok;  ii++)
                     ok = fprintf (ini_file, " %.8g", 
                                   item_ptr->u_el.u_xel.u_act.r_params[ii]) > 0;
               } /* Aktionsdescriptor erhalten */
               item_ptr = item_ptr->u_el.next_action; /* zur naechsten Aktion */
            } /* all along the chain */
            fprintf (ini_file, "\n");

            /* -------------- AK-Bewertung: AK-Kopien anlegen: -------------- */
            if (ak_copy_ptr[ak_ii] != NULL) /* sollte eigentl.schon geloescht */
               *erg_status = delete_chain (&(ak_copy_ptr[ak_ii]));    /* sein */
            ok = copy_chain (&(ak_copy_ptr[ak_ii]), ini_ptr[ak_ii], 
                              KEINE_URKETTE, WITH_MEM_CHECK, erg_status);
            if (!ok)
               *erg_status = fatal (EVO_RC_KOPPL, 23, NO_INFO, NO_INFO, CHROMOSOM_TXT, NOT_ENOUGH_MEM);
            ok &= *erg_status == GLEAM_OK;
         } /* AK-Bewertung: INI-File schreiben */
      } /* Start-AKs rausschreiben und bei AK-Bewertung Kopien anlegen */
   } /* Alle Startvertices */
   fprintf (ini_file, START_COMPL_END);
   fclose (ini_file);

   /* -------------------------- Complex aufrufen: -------------------------- */
   if (ok)
   { /* Ini-File erfolgreich geschrieben */
      best_note     = -1.0;                                    /* Initialwert */
      sim_anz       = 0;
      bad_sim_ctr   = 0;
      mit_ak_update = lamarck;
#ifdef CALL_LSV
      /* sprintf (buf1, "do_compl_start: Vor  COMPLEX-Aufruf. erg_status=%d", *erg_status);
         write_to_log (buf1, CLOSE_LOG); */
      complex_box (); 
      /* sprintf (buf1, "do_compl_start: Nach COMPLEX-Aufruf. err_state =%d  best_note=%8.1f",
         err_state, best_note);
         write_to_log (buf1, CLOSE_LOG); */
#else /* kein CALL_LSV */
         par_ctr = 0;
         if (param_bew)
         { /* Param-Bew */
            for (ii = i_par_anf;  ii < all_iwerte_anz;  ii++)
               if (ipar_fuer_lsv[ii])           /* Wertebereich gross genug ? */
                  best_params[par_ctr++] = i_params[ii];
            for (ii = 0;  ii < akt_rpar_anz;  ii++)
               best_params[par_ctr++] = r_params[ii];
         } /* Param-Bew */
         else
         { /* AK-Bewertung */
            ComplexIndex = 0;
            item_ptr = ini_ptr[0]->u_el.next_action;
            while (item_ptr != NULL)
            { /* all along the chain */
               ai = item_ptr->u_el.activity;
               for (ii = 0;  ii < int_pro_aktion[ai];  ii++)
               { /* Int-Parameter */
                  get_act_descr (&this_act_ptr, (SHORT)ai, WITHOUT_GENECODE_CHECK);
                  if (this_act_ptr->i_par_descr[ii].par_bereich >= minIParRange)
                     best_params[par_ctr++] =               
                        item_ptr->u_el.u_xel.u_act.i_params[ii];
               } /* Int-Parameter */
               for (ii = 0;  ii < real_pro_aktion[ai];  ii++)
                  best_params[par_ctr++] =                  /* Real-Parameter */
                      item_ptr->u_el.u_xel.u_act.r_params[ii];
               item_ptr = item_ptr->u_el.next_action; /* zur naechsten Aktion */
            } /* all along the chain */
         } /* AK-Bewertung */
         objfct_complex (best_params);                       /* Test fuer LSV */
         best_note += 1.0;
#endif /* kein CALL_LSV */

      if (err_state == GLEAM_OK)
      { /* LSV fehlerfrei */
         *erg_status |= lsv_erg_auswertung (&(ini_ptr[best_ak_idx])); 
         *ini_idx     = best_ak_idx;                 /* Index der Ergebnis-AK */
      } /* LSV fehlerfrei */
      else
      { /* Fehler beim LSV-Aufruf */
         best_note    = KEINE_NOTE;
         *erg_status |= err_state;
         ok           = FALSE;
      } /* Fehler beim LSV-Aufruf */
   } /* Ini-File erfolgreich geschrieben */
   if (!param_bew)
      for (ak_ii = 0;  ak_ii < anz_compl_pkte;  ak_ii++)
         if (ak_copy_ptr[ak_ii] != NULL) 
            *erg_status = delete_chain (&(ak_copy_ptr[ak_ii])); 

   if (ok)
      return (best_note);
   else
      return (KEINE_NOTE);
} /* do_compl_start */



/* ============================= objfct_complex ============================= */
double objfct_complex (double* param)
{
   CHAIN_ELEM_PTR_TYPE  item_ptr;
   ACT_DESCR_PTR_TYPE   this_act_ptr;
   GSTATE  ret_state;
   DOUBLE  note;                          /* Durch Simu u.Bew ermittelte Note */
   INT     wert;
   INT     ai,                            /* Aktionscode                      */
           int_der_aktion,                /* Anzahl der Ints dieser Aktion    */
           idx, ii;
   BOOL    bewerten;                      /* Simu ok, also bewerten           */

   /* objfct_complex */
#ifdef COMPL_LOG
   sprintf (buf1, "objfct_complex: param_bew=%d ", param_bew);
   write_to_log (buf1, CLOSE_LOG);
#endif /* COMPL_LOG */
   get_fbuf_data (&ret_state);
   if (ret_state == GLEAM_FATAL)
   { /* Immer schlechtere Bewertungen abliefern, damit LSV terminiert */
      bad_sim_ctr++;
#ifdef COMPL_LOG
      write_to_log ("objfct_complex: schlechtes Ende", CLOSE_LOG);
#endif /* COMPL_LOG */
      return (RNOTEN_MAX + 10.0 * bad_sim_ctr);
   } /* Immer schlechtere Bewertungen abliefern, damit LSV terminiert */
   if (!param_bew || (appl_code == MATLAB_APPL))
   { /* Parameter in die AK-Kopie eintragen */
      if (mit_start_compl)
         ak_idx = ComplexIndex;
      item_ptr = ak_copy_ptr[ak_idx]->u_el.next_action;
      idx      = 0;
      while (item_ptr != NULL)
      { /* all along the chain */
         ai             = item_ptr->u_el.activity;
         int_der_aktion = int_pro_aktion[ai];
         if (int_der_aktion > 0)
         { /* Die Aktion hat ueberhaupt Ints */
            get_act_descr (&this_act_ptr, (SHORT)ai, WITHOUT_GENECODE_CHECK);
            for (ii = 0;  ii < int_der_aktion;  ii++)
               if (this_act_ptr->i_par_descr[ii].par_bereich >= minIParRange)
               { /* alle I-Parameter mit genuegend grossem Wertebereich */
                  if (param[idx] < this_act_ptr->i_par_descr[ii].par_u_grenze)
                     item_ptr->u_el.u_xel.u_act.i_params[ii] = 
                        this_act_ptr->i_par_descr[ii].par_u_grenze;
                  else if (this_act_ptr->i_par_descr[ii].par_o_grenze < param[idx])
                     item_ptr->u_el.u_xel.u_act.i_params[ii] = 
                        this_act_ptr->i_par_descr[ii].par_o_grenze;
                  else
                     item_ptr->u_el.u_xel.u_act.i_params[ii] = i_round(param[idx]);
                  idx++;
               } /* alle I-Parameter mit genuegend grossem Wertebereich*/
         } /* Die Aktion hat ueberhaupt Ints */
         for (ii = 0;  ii < real_pro_aktion[ai];  ii++)
         { /* alle R-Parameter */
            item_ptr->u_el.u_xel.u_act.r_params[ii] = param[idx];
            idx++;
         } /* alle R-Parameter */
         item_ptr = item_ptr->u_el.next_action;       /* zur naechsten Aktion */
      } /* all along the chain */
   } /* Parameter in die AK-Kopie eintragen */

   /* ----------------------------- simulieren: ----------------------------- */
#ifdef COMPL_LOG
   sprintf (buf1, "\nobjfct_complex: Vor der %ld.Simu. mit_ext_simu=%d akt_ipar_anz=%d appl_code=%d", 
	    sim_anz, mit_ext_simu, akt_ipar_anz, appl_code);
   write_to_log (buf1, CLOSE_LOG);
#endif /* COMPL_LOG */
   sim_anz++;                                      /* Simu-Aufrufe mitzaehlen */
   if (mit_ext_simu)
   { /* Aufruf des externen Simulators */
#ifdef EXT_MATLAB_SIMU
      if (appl_code == MATLAB_APPL)
	 bewerten = doMatlabSim(simu_erg, ak_copy_ptr[ak_idx]);  /* matlab/MatPower interface */
      else
#endif /* EXT_MATLAB_SIMU */
      { /* no matlab/MatPower simulator interface */
	 if (akt_ipar_anz > 0)
         { /* Integer- und Real-Parameter trennen */
	    idx = 0;
	    for (ii = i_par_anf;  ii < all_iwerte_anz;  ii++)
	       if (ipar_fuer_lsv[ii])
               { /* Int-Param mit ausreichend grossem Wertebereich kopieren */
		  i_params [ii] = i_round (param[idx]);
		  idx++;
	       } /* Int-Param mit ausreichend grossem Wertebereich kopieren */
	    bewerten = do_ext_sim (simu_erg, i_params, &(param[rosen_ipar_anz]));
	 } /* Integer- und Real-Parameter trennen */
	 else
	    bewerten = do_ext_sim (simu_erg, i_params, param);
      } /* no matlab/MatPower simulator interface */
   } /* Aufruf des externen Simulators */
   else 
     if (appl_code == LESAK_APPL)
        bewerten = do_rob_sim (simu_erg, ak_copy_ptr[ak_idx], CALC_FITNESS);
     else
        if (appl_code == MATH_FKT_APPL)
           bewerten = do_mbf_sim (simu_erg, r_par_ges_anz, param);
        else
           if (appl_code == CEC_MATHFKT_APPL)
              bewerten = do_cec05_mbf_sim (simu_erg, r_par_ges_anz, ext_simu_code, 
                                           param);
           else  /* applikationsspezifische Simulation */
              bewerten = do_appl_sim (simu_erg, ak_copy_ptr[ak_idx], CALC_FITNESS, 
                                      &ret_state);
   
   /* ------------------------------ bewerten: ------------------------------ */
#ifdef COMPL_LOG
   sprintf (buf1, "objfct_complex: Nach Simu. bewerten=%d", bewerten);
   write_to_log (buf1, CLOSE_LOG);
#endif /* COMPL_LOG */
   if (bewerten)
   { /* Bewertung einer Aktionskette */
      note = ak_bewerten (CALC_FITNESS);
#ifdef CEC_MBF
      if (simu_erg[0] < cec_best_fval)
         cec_best_fval = simu_erg[0];
#endif /* CEC_MBF */
     
      /* if (sim_anz < 15) note += 10.0*sim_anz; else note -= 10.0*sim_anz; */
      if (note > best_note)
      { /* Werte merken */
         best_note = note;
         for (ii = 0;  ii < akt_anz_krit;  ii++)
            int_simu_erg[ii] = simu_erg[ii];
         if (mit_ak_update)
            if (param_bew)
            { /* X-Werte merken */
               for (ii = 0;  ii < akt_par_anz;  ii++)
                  best_params[ii] = param[ii];
            } /* X-Werte merken */
            else
            { /* AK merken */
               best_ak_idx = ak_idx;
               if (best_ak_ptr != NULL)
                  err_state = delete_chain (&best_ak_ptr);
               if (!copy_chain (&best_ak_ptr, ak_copy_ptr[ak_idx], 
                                KEINE_URKETTE, WITH_MEM_CHECK, &err_state))
               { /* Mist! */
                  bad_sim_ctr++;
                  note = -RNOTEN_MAX - bad_sim_ctr;  /*damit das LSV abbricht */
                  err_state = fatal (EVO_RC_KOPPL, 30, NO_INFO, NO_INFO, CHROMOSOM_TXT, 
                                     NOT_ENOUGH_MEM);
               } /* Mist! */
            } /* AK merken */
      } /* Werte merken */
      note = RNOTEN_MAX - note;                   /* Rosenbrock sucht Minimum */
   } /* Bewertung einer Aktionskette */
   else
   { /* Fehler */
      if (err_state != GLEAM_FATAL)
      { /* tolerieren! */
         meldung (EVO_RC_KOPPL, NO_INFO, NO_INFO, "-LSV", EVO_WUERG_SIMU_MELD);
         clear_err_state ();
      } /* tolerieren! */
      else /* FATAL: nicht tolerieren! */
         fehler (EVO_RC_KOPPL, 31, NO_INFO, NO_INFO, "-LSV", EVO_WUERG_SIMU_MELD);
      bad_sim_ctr++;
      note = RNOTEN_MAX + bad_sim_ctr;              /* damit das LSV abbricht */
   } /* Fehler */
#ifdef COMPL_LOG
   write_to_log ("objfct_complex: normales Ende", CLOSE_LOG);
#endif /* COMPL_LOG */
   return (note);
} /* objfct_complex */



/* =========================== prepare_rc_koppl ============================= */
BOOL prepare_rc_koppl (INT  i_par_ges_anz, 
                       INT  r_par_ges_anz)
{
   size_t  bsize = sizeof (BOOL);
   size_t  dsize = sizeof (DOUBLE);
   size_t  psize = sizeof (CHAIN_ELEM_PTR_TYPE);
   INT     ptr_feld_len, ii;
   BOOL    ok = TRUE;

   /* prepare_rc_koppl */
   if (best_params != NULL)
   { /* alte Felder freigeben */
      free (best_params);
      free (ini_ptr);
      free (ak_copy_ptr);
      best_params = NULL;
      ini_ptr     = NULL;
      ak_copy_ptr = NULL;
   } /* alte Felder freigeben */

   param_bew = (stat_ak_order ||    /* AK fester Laenge u. irrelev.Reihenfolge */
     (mit_ext_simu && dyn_aks)) &&  /* extern zu simulierende dynamische AKs   */
      (appl_code != ADD_APPL);      /* aber keine ADD_APPL                     */
   if (param_bew)
      anz_compl_pkte = i_par_ges_anz + r_par_ges_anz + 1;
   else
      anz_compl_pkte = last_act + 1;            /* Spezialloesung fuer OPAL/V */
   if (param_bew)
      ptr_feld_len = 1;
   else
      ptr_feld_len = anz_compl_pkte;
   if (anz_compl_pkte > MAX_POPEL_SIZE)
   { /* Zu viele Complex-Startpunkte/Dimensionen */
      meldung (EVO_RC_KOPPL, anz_compl_pkte, MAX_POPEL_SIZE, "", BAD_PAR_AMOUNT);
      anz_compl_pkte = MAX_POPEL_SIZE;
   } /* Zu viele Complex-Startpunkte/Dimensionen */
   if (best_params == NULL)
   { /* neue Felder anlegen */
      ok=((best_params    = (DOUBLE*)malloc(anz_compl_pkte * dsize)) != NULL) &&
         ((ipar_fuer_lsv  = (BOOL*)malloc(i_par_ges_anz * bsize))    != NULL) &&
         ((ini_ptr        = (CHAIN_ELEM_PTR_TYPE*)malloc(anz_compl_pkte*psize)) !=NULL) &&
         ((ak_copy_ptr    = (CHAIN_ELEM_PTR_TYPE*)malloc(ptr_feld_len*psize))   !=NULL);
   } /* neue Felder anlegen */

   minIParRange = MIN_IPAR_BEREICH;
   if (appl_code == ADD_APPL)
      if (add_appl_code == GORBA_PERM_APPL) {
         minIParRange = MIN_IPAR_GORBA;
         sprintf (buf1, "Perm.HMod: Mind. %g GridJobs fuer Rosen/Compl!", minIParRange);
         meldung (EVO_RC_KOPPL, NO_INFO, NO_INFO, buf1, SIMU_RZ_MELD);
      }
      else
         if (((add_appl_code == GORBA_KOMB_APPL) || (add_appl_code == GORBA_KOMB2_APPL)) &&
             (lsv[LSV_ROSEN].aktiv || lsv[LSV_COMPL].aktiv))
            meldung (EVO_RC_KOPPL, NO_INFO, NO_INFO, 
                     "GORBA: Beim aktuellen Genmodel Rosen/Compl nicht sinnvoll!", 
                     SIMU_RZ_MELD);

   if (ok)
   { /* Datenobjekte angelegt */
      ok = write_lsv_ini_file (COMPL_INI_FNAME, OHNE_START_WERTE, NULL);
      for (ii = 0;  ii < ptr_feld_len;  ii++)
         ak_copy_ptr[ii] = NULL; 
   } /* Datenobjekte angelegt */

   return (ok);
} /* prepare_rc_koppl */



/* =========================== write_lsv_ini_file =========================== */
static BOOL write_lsv_ini_file (const char          *fspec,
                                BOOL                 mit_start_werte,
                                CHAIN_ELEM_PTR_TYPE  ak_ptr)
/*----------------------------------------------------------------------------*/
/*  Legt die durch "fspec" bezeichnete Initialisierungsdatei fuer das LSV an, */
/*  also "objfct.ini" beim Rosenbrock und "objfct_complex.ini" fuer das COM-  */
/*  PLEX-Verfahren. Startwerte werden bei gesetztem "mit_start_werte" je nach */
/*  AK-Typ aus "i_params" und "r_params" bzw. aus dem "ak_ptr" genommen und   */
/*  in das File geschriben. Bei nicht gesetztem "mit_start_werte" sind bei    */
/*  "param_bew" alle Startwerte 0, waehrend bei AK-Bewertung die Datei nicht  */
/*  vollstaendig geschrieben wird.                                            */
/*  Liefert TRUE ab, wenn alles gut ging.                                     */
/*----------------------------------------------------------------------------*/
{
   CHAIN_ELEM_PTR_TYPE  item_ptr;
   ACT_DESCR_PTR_TYPE   this_act_ptr;
   DOUBLE               rwert;
   INT                  par_ctr,      /* Parameter-Zaehler fuer "def X<nr>"   */
                        ai,           /* Aktionscode                          */
                        ii;
   BOOL                 ok;

   /* write_lsv_ini_file */
   if (!open_file (&ini_file, fspec, "wt"))
   { /* File nicht zu oeffnen */
      fatal (EVO_RC_KOPPL, 50, NO_INFO, NO_INFO, fspec, FILE_NOT_OPEN);
      return (FALSE);
   } /* File nicht zu oeffnen */

   par_ctr = 1;
   ok = fprintf (ini_file, INIF_HDR_LINE) > 0;
   if (param_bew)
   { /* ------------- Parameter-Bewertung: INI-File schreiben: -------------- */
      if (mit_start_werte)                          /* Parameter aufbereiten: */
         ok = (get_ak_params (i_params, r_params, ak_ptr, FALSE) == GLEAM_OK);
      else
      { /* Keine Parameter, also Felder auf null setzen */
         ii = (INT)GLEAM_OK;
         ok = gen_chain(&ak_ptr, (GSTATE*)(&ii));
         if (ok)
         { /* setzt "i_par_anf", "all_iwerte_anz", "akt_rpar_anz" */
            ok = (get_ak_params (i_params, r_params, ak_ptr, FALSE) == GLEAM_OK);
            delete_chain (&ak_ptr);
         } /* setzt "i_par_anf", "all_iwerte_anz", "akt_rpar_anz" */
         memset (i_params, 0, i_par_ges_anz * sizeof(INT));
         memset (r_params, 0, r_par_ges_anz * sizeof(DOUBLE));
      } /* Keine Parameter, also Felder auf null setzen */
      if (ok)
      { /* Param ok */
         for (ii = i_par_anf;  (ii < all_iwerte_anz) && ok;  ii++)
            if ((og_iparams[ii] - ug_iparams[ii]) >= minIParRange)
            { /* Int-Param mit ausreichend grossem Wertebereich schreiben */
               ipar_fuer_lsv[ii] = TRUE;
               ok  = fprintf (ini_file, INIF_DEF_LINE, par_ctr++) > 0;
               ok &= fprintf (ini_file, INIF_ISTART_LINE, i_params[ii])   > 0;
               ok &= fprintf (ini_file, INIF_ILB_LINE,    ug_iparams[ii]) > 0;
               ok &= fprintf (ini_file, INIF_IUB_LINE,    og_iparams[ii]) > 0;
               ok &= fprintf (ini_file, INIF_END_LINE) > 0;
            } /* Int-Param mit ausreichend grossem Wertebereich schreiben */
            else
               ipar_fuer_lsv[ii] = FALSE;
         rosen_ipar_anz = par_ctr - 1;
         for (ii = 0;  (ii < akt_rpar_anz) && ok;  ii++)
         { /* Real-Parameter rausschreiben */
            check_and_copy_rpar (&rwert, r_params[ii],
                                 ug_rparams[ii], og_rparams[ii]);
#ifdef PARAM_KOR
            rwert = r_params[ii];
            if (rwert > og_rparams[ii] - 0.000001)
               rwert = og_rparams[ii] - 0.000001;
            if (rwert < ug_rparams[ii] + 0.000001)
               rwert = ug_rparams[ii] + 0.000001;
            write_to_log ("write_lsv_ini_file: Mit Korrekturen der Parametergrenzen um 1.0e-6", CLOSE_LOG);
#endif /* PARAM_KOR */
            ok  = fprintf (ini_file, INIF_DEF_LINE, par_ctr++) > 0;
            ok &= fprintf (ini_file, INIF_START_LINE, rwert)   > 0;
            ok &= fprintf (ini_file, INIF_LB_LINE,    ug_rparams[ii]) > 0;
            ok &= fprintf (ini_file, INIF_UB_LINE,    og_rparams[ii]) > 0;
            ok &= fprintf (ini_file, INIF_END_LINE) > 0;
         } /* Real-Parameter rausschreiben */
      } /* Param ok */
   } /* Parameter-Bewertung: INI-File schreiben */
   else
      if (mit_start_werte)
      { /* -------- AK-Bewertung: INI-File mit Startwerten schreiben: ------- */
         item_ptr = ak_ptr->u_el.next_action;
         while (item_ptr != NULL)
         { /* all along the chain */
            ai = item_ptr->u_el.activity;
            if (get_act_descr (&this_act_ptr,(SHORT)ai, WITHOUT_GENECODE_CHECK) == GLEAM_OK)
            { /* Aktionsdescriptor erhalten */
               for (ii = 0;  (ii < this_act_ptr->i_par_anz) && ok;  ii++)
                  if (this_act_ptr->i_par_descr[ii].par_bereich >= minIParRange)
                  { /* Int-Parameter rausschreiben */
                     ok  = fprintf (ini_file, INIF_DEF_LINE, par_ctr++) > 0;
                     ok &= fprintf (ini_file, INIF_ISTART_LINE, 
                                    item_ptr->u_el.u_xel.u_act.i_params[ii]) > 0;
                     ok &= fprintf (ini_file, INIF_ILB_LINE, 
                                    this_act_ptr->i_par_descr[ii].par_u_grenze) > 0;
                     ok &= fprintf (ini_file, INIF_IUB_LINE, 
                                    this_act_ptr->i_par_descr[ii].par_o_grenze) > 0;
                     ok &= fprintf (ini_file, INIF_END_LINE) > 0;
                  } /* Int-Parameter rausschreiben */
               for (ii = 0;  (ii < this_act_ptr->r_par_anz) && ok;  ii++)
               { /* Real-Parameter rausschreiben */
                 ok  = fprintf (ini_file, INIF_DEF_LINE, par_ctr++) > 0;
                 check_and_copy_rpar (&rwert, item_ptr->u_el.u_xel.u_act.r_params[ii],
                                      this_act_ptr->r_par_descr[ii].par_u_grenze,
                                      this_act_ptr->r_par_descr[ii].par_o_grenze);
                 ok &= fprintf (ini_file, INIF_START_LINE, rwert) > 0;
                 ok &= fprintf (ini_file, INIF_LB_LINE, 
                                this_act_ptr->r_par_descr[ii].par_u_grenze) > 0;
                 ok &= fprintf (ini_file, INIF_UB_LINE, 
                                this_act_ptr->r_par_descr[ii].par_o_grenze) > 0;
                 ok &= fprintf (ini_file, INIF_END_LINE) > 0;
               } /* Real-Parameter rausschreiben */
            } /* Aktionsdescriptor erhalten */
            item_ptr = item_ptr->u_el.next_action;    /* zur naechsten Aktion */
         } /* all along the chain */
      } /* AK-Bewertung: INI-File mit Startwerten schreiben */
      else
      { /* AK-Bewertung: INI-File ohne Startwerte fuer OPAL/V u.GORBA schreiben */
        for (ii = 0;  ii < last_act;  ii++)
        { /* Keine allg. Loesung! Nur fuer OPAL/V und GORBA */
          ok  = fprintf (ini_file, INIF_DEF_LINE, ii+1)  > 0;
          ok &= fprintf (ini_file, INIF_START_LINE, 0.0) > 0;
          ok &= fprintf (ini_file, INIF_LB_LINE, 0.0)    > 0;
          ok &= fprintf (ini_file, INIF_UB_LINE, 1656.0) > 0;
          ok &= fprintf (ini_file, INIF_END_LINE)        > 0;
        } /* Keine allg. Loesung! Nur fuer OPAL/V und GORBA */
      } /* AK-Bewertung: INI-File ohne Startwerte fuer OPAL/V u.GORBA schreiben */
   akt_par_anz = par_ctr - 1;
   fclose (ini_file);

   return (ok);
} /* write_lsv_ini_file */



/* =========================== lsv_erg_auswertung =========================== */
static GSTATE lsv_erg_auswertung (CHAIN_ELEM_PTR_TYPE *ak_ptr)
/*----------------------------------------------------------------------------*/
/*   Ermittelt die Anzahl der Bewertungen und zaehlt sie zum "indiv_ctr" hin- */
/*   zu. Wenn die AK verbessert wurde, werden die Note und die SimuErgWerte   */
/*   in die AK eingetragen; bei gesetztem "mit_ak_update" wird sie darueber-  */
/*   hinaus auch noch upgedated.                                              */
/*----------------------------------------------------------------------------*/
{
   CHAIN_ELEM_PTR_TYPE  item_ptr, best_item_ptr;
   ACT_DESCR_PTR_TYPE   this_act_ptr;
   DOUBLE               rwert;
   INT                  i_par_index,  /* Idx im reduzierten "best_params"-Feld*/
                        i_check_idx,  /* Index in den vollstaend. IParFeldern */
                        r_par_index,  /* Real-Parameter-Index                 */
                        ak_len,       /* AK-Laenge                            */
                        ai,           /* Aktionscode                          */
                        qfa_anz,      /* Anzahl der QF-Anforderungen          */
                        bew_anz,      /* max (qfa_anz, sim_anz)               */
                        r_idx,        /* Index                                */
                        ii, jj;
   GSTATE               erg_state = GLEAM_OK;

   /* lsv_erg_auswertung */
   qfa_anz = read_sim_anz_from_dat ();
   if (qfa_anz > sim_anz)
      bew_anz = qfa_anz;
   else
      bew_anz = sim_anz;
   indiv_ctr += bew_anz;                  /* Statistik: Bewertungen erfassen  */

#ifdef SS_TST
   sprintf (buf3, "lsv_koppl: qfa_anz=%d sim_anz=%d bew_anz=%d  indiv_ctr=%ld\n", 
            qfa_anz, sim_anz, bew_anz, indiv_ctr);
   write_to_log (buf3, CLOSE_LOG);
#endif /* SS_TST */

#ifdef HY_DIS
   if (mit_start_compl)
      c1all_qfa_anz = bew_anz;
#endif /* HY_DIS */        

   if (best_note > (*ak_ptr)->u_el.u_xel.u_hdr.fitness_note)
   { /* AK verbessert */
     if (mit_ak_update)
     { /* --------------------------- AK-Update ----------------------------- */
       if (param_bew)
       { /* Parameter-Bewertung: Besten ParamVektor uebernehmen */
         ak_len      = (*ak_ptr)->u_el.u_xel.u_hdr.chain_length;
         i_par_index = 0;
         i_check_idx = i_par_anf;
         r_par_index = rosen_ipar_anz;
         r_idx       = 0;
         item_ptr    = *ak_ptr;
         for (jj = 1;  jj <= ak_len;  jj++)
         { /* Parameterwerte uebernehmen */
           if (ext_sim_und_dyn_ak)
           { /* dyn.AK: zur naechsten Aktion */
             item_ptr = item_ptr->u_el.next_action;
             ai       = item_ptr->u_el.activity;
           } /* dyn.AK: zur naechsten Aktion */
           else
           { /* statische AK: Aktion suchen */
             ai = jj;
             item_ptr = (*ak_ptr)->u_el.next_action;
             while ((item_ptr->u_el.activity != (SHORT)ai) && 
                    (item_ptr->u_el.next_action != NULL))
               item_ptr = item_ptr->u_el.next_action;
           } /* statische AK: Aktion suchen */

           for (ii = 0;  ii < int_pro_aktion[ai];  ii++)
           { /* Alle I-Params dieser Aktion */
             if (ipar_fuer_lsv[i_check_idx])
             { /* IntPar mit ausr.grossem Werteber.kopieren */
               check_and_copy_ipar 
                 (&(item_ptr->u_el.u_xel.u_act.i_params[ii]), 
                  i_round(best_params[i_par_index]),
                  ug_iparams[i_check_idx], og_iparams[i_check_idx]);
               i_par_index++;
             } /* IntPar mit ausr.grossem Werteber.kopieren */
             i_check_idx++;
           } /* Alle I-Params dieser Aktion */
           for (ii = 0;  ii < real_pro_aktion[ai];  ii++)
           { /* alle R-Parameter */
             check_and_copy_rpar 
               (&rwert, best_params[r_par_index],
                ug_rparams[r_idx], og_rparams[r_idx]);
             item_ptr->u_el.u_xel.u_act.r_params[ii] = rwert;
             r_idx++;
             r_par_index++;
           } /* alle R-Parameter */
         } /* Parameterwerte uebernehmen */
       } /* Parameter-Bewertung: Besten ParamVektor uebernehmen */
       else
       { /* AK-Bewertung: Beste AK nehmen */
         item_ptr      = (*ak_ptr)->u_el.next_action;
         best_item_ptr = best_ak_ptr->u_el.next_action;
         while (item_ptr != NULL)
         { /* all along the chain */
           ai = item_ptr->u_el.activity;
           get_act_descr (&this_act_ptr, (SHORT)ai, WITHOUT_GENECODE_CHECK);
           for (ii = 0;  ii < int_pro_aktion[ai];  ii++)
             if (this_act_ptr->i_par_descr[ii].par_bereich >= minIParRange)
               check_and_copy_ipar                  /* IntParam aktualisieren */
                 (&(item_ptr->u_el.u_xel.u_act.i_params[ii]), 
                  best_item_ptr->u_el.u_xel.u_act.i_params[ii],
                  this_act_ptr->i_par_descr[ii].par_u_grenze,
                  this_act_ptr->i_par_descr[ii].par_o_grenze);
           for (ii = 0;  ii < real_pro_aktion[ai];  ii++)
           { /* RealParam aktualisieren */
             check_and_copy_rpar            
               (&rwert, best_item_ptr->u_el.u_xel.u_act.r_params[ii],
                this_act_ptr->r_par_descr[ii].par_u_grenze,
                this_act_ptr->r_par_descr[ii].par_o_grenze);
             item_ptr->u_el.u_xel.u_act.r_params[ii] = rwert;
           } /* RealParam aktualisieren */
           item_ptr      = item_ptr->u_el.next_action;      
           best_item_ptr = best_item_ptr->u_el.next_action; 
         } /* all along the chain */
         erg_state = delete_chain (&best_ak_ptr);
       } /* AK-Bewertung: Beste AK nehmen */
     } /* AK-Update */
#ifdef HY_DIS
     if (!mit_start_compl)
#endif /* HY_DIS */                     
        (*ak_ptr)->u_el.u_xel.u_hdr.fitness_note  = best_note;
     (*ak_ptr)->u_el.u_xel.u_hdr.chain_status |= SIMULATED;
     if (!do_always_sim)                             /* Daten nicht zuviele ? */
     { /* Simu-Werte in die AK eintragen */
       item_ptr = (*ak_ptr)->u_el.u_xel.u_hdr.header_par_ptr;
       for (ii = 0;  ii < akt_anz_krit;  ii++)
         item_ptr->sim_erg_werte[ii] = int_simu_erg[ii];
     } /* Simu-Werte in die AK eintragen */
   } /* AK verbessert */
   return (erg_state);
} /* lsv_erg_auswertung */



/* =========================== check_and_copy_ipar ========================== */
static void check_and_copy_ipar (INT *erg_wert,
                                 INT  in_wert,
                                 INT  ug,
                                 INT  og)
/*----------------------------------------------------------------------------*/
/*  Prueft den "in_wert auf die Einhaltung der Grenzen "ug" und "og" und      */
/*  liefert den begrenzten Wert in "erg_wert" ab. Auftretende Grenzver-       */
/*  letzungen werden bei gesetztem SS_TST ins Logfile geschrieben.            */
/*----------------------------------------------------------------------------*/
{
   if (in_wert > og)
   { /* I-Wert zu gross */
#ifdef SS_TST
      sprintf (buf1, IPAR_ZU_GROSS, og, in_wert);
      write_to_log (buf1, CLOSE_LOG);
#endif /* SS_TST */
      *erg_wert = og;
   } /* I-Wert zu gross */
   else
      if (in_wert < ug)
      { /* I-Wert zu klein */
#ifdef SS_TST
         sprintf (buf1, IPAR_ZU_KLEIN, ug, in_wert);
         write_to_log (buf1, CLOSE_LOG);
#endif /* SS_TST */
         *erg_wert = ug;
      } /* I-Wert zu klein */
      else
         *erg_wert = in_wert;
} /* check_and_copy_ipar */



/* =========================== check_and_copy_rpar ========================== */
static void check_and_copy_rpar (DOUBLE *erg_wert,
                                 DOUBLE  in_wert,
                                 DOUBLE  ug,
                                 DOUBLE  og)
/*----------------------------------------------------------------------------*/
/*  Prueft den "in_wert auf die Einhaltung der Grenzen "ug" und "og" und      */
/*  liefert den begrenzten Wert in "erg_wert" ab. Auftretende Grenzver-       */
/*  letzungen werden bei gesetztem SS_TST ins Logfile geschrieben.            */
/*----------------------------------------------------------------------------*/
{
   if (og > 0.0)
      og = og - og/1000000.0;
   else
      og = og + og/1000000.0;
   if (ug > 0.0)
      ug = ug + ug/1000000.0;
   else
      ug = ug - ug/1000000.0;

   if (in_wert > og)
   { /* R-Wert zu gross */
#ifdef SS_TST
      sprintf (buf1, RPAR_ZU_GROSS, og, in_wert);
      write_to_log (buf1, CLOSE_LOG);
#endif /* SS_TST */
      *erg_wert = og;
   } /* R-Wert zu gross */
   else
      if (in_wert < ug)
      { /* R-Wert zu klein */
#ifdef SS_TST
         sprintf (buf1, RPAR_ZU_KLEIN, ug, in_wert);
         write_to_log (buf1, CLOSE_LOG);
#endif /* SS_TST */
         *erg_wert = ug;
      } /* R-Wert zu klein */
      else
         *erg_wert = in_wert;
} /* check_and_copy_rpar */



/* ========================== read_sim_anz_from_dat ========================= */
static INT read_sim_anz_from_dat (void)
/*----------------------------------------------------------------------------*/
/*  Liest die Anzahl der angeforderten QF Werte aus dem Ergebnis-DAT-File     */
/*  des Rosenbrocks und liefert sie ab.                                       */
/*----------------------------------------------------------------------------*/

{
   FILE        *datfile;
   STRING_TYPE  buffer;
   INT          wert, anz = 0;
   char        *ch_dummy;

   /* read_sim_anz_from_dat */
   if (open_file(&datfile, ERG_DAT_FSPEC, "rt"))
   { /* DAT-File offen */
      ch_dummy = fgets (buffer, MAX_STRING, datfile);          /* skip header */
      if (fscanf (datfile, "Anzahl der angeforderten QF Werte betraegt:%d", 
                  &wert) == 1)
         anz = wert;
      fclose (datfile);
   } /* DAT-File offen */
   return (anz);
} /* read_sim_anz_from_dat */



/* =============================== dat_killer =============================== */
void dat_killer (void)
{
   STR_TYPE  buf;
   INT       dummy;
   
   /* dat_killer */
   sprintf (buf, "rm -f *.dat");
   dummy = system (buf);
   return;
} /* dat_killer */



/* ============================== set_rc_limits ============================= */
GSTATE set_rc_limits (INT     new_c_max_iter,
                      INT     new_r_max_iter,
                      DOUBLE  error_goal,
                      DOUBLE  step_size)
{
   BOOL    ok        = TRUE;
   GSTATE  ret_state = GLEAM_OK;
   
   /* set_rc_limits */
   if (new_c_max_iter > 0)
      max_iterc = new_c_max_iter;         /* Zuweisung fuer Complex-Ini-Files */
   if (new_r_max_iter > 0)
   { /* Es ist ein neues "rosenbrock.ini" zu schreiben */
      if (!open_file (&ini_file, ROSEN_INI_FNAME, "wt"))
         return (fehler (EVO_RC_KOPPL, 60, NO_INFO, NO_INFO, ROSEN_INI_FNAME, 
                         FILE_NOT_OPEN));            /* File nicht zu oeffnen */

      /* Anlegen der Datei "rosenbrock.ini": */
      ok  = fprintf (ini_file, "error_goal = %g\n", error_goal)  > 0;
      ok &= fprintf (ini_file, "hits = 1\n") > 0;
      ok &= fprintf (ini_file, "delta = 1.0e-4\n") > 0;
      ok &= fprintf (ini_file, "step = %g\n", step_size) > 0;
      ok &= fprintf (ini_file, "norm = 2\n") > 0;
      ok &= fprintf (ini_file, "interpolate = 0\n") > 0;
      ok &= fprintf (ini_file, "avoid_reevaluations = 1\n") > 0;
      ok &= fprintf (ini_file, "restart = 0\n") > 0;
      ok &= fprintf (ini_file, "max_iter = %d\n", new_r_max_iter) > 0;
      ok &= fprintf (ini_file, "pfile = %d\n", PFILE) > 0;
      fclose (ini_file);
      if (!ok)
         ret_state = fehler (EVO_RC_KOPPL, 61, NO_INFO, NO_INFO, ROSEN_INI_FNAME, 
                             WRITE_ERR);                     /* Schreibfehler */
   } /* Es ist ein neues "rosenbrock.ini" zu schreiben */
   
   return (ret_state);
} /* set_rc_limits */
