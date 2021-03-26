/*******************************************************************************
GLEAM/AE                                                                  V3.3.3
                               G L E A M  /  A E 
       Genetic LEarning Algorithms and Methods / Application Environment

                         Package: SIMUlator
                          Module: SIMUlatorkern
 
Package: simu                     File: simu.c 
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of this file.
--------------------------------------------------------------------------------

Der Modul realisiert den Simulatorkern zur Benutzung interner oder externe Simu-
latoren. Der Kern umfasst folgende Funktionen:
  1. Feststellung, ob simuliert werden muss, oder ob in der AK noch gueltige
     (bereits aufbereitete) Simulatordaten hinterlegt sind.
  2. Vorbereitung des Simulatoraufrufs, wobei entweder die Parameter aus der AK
     extrahiert werden, oder die AK selbst Parameter ist.
  3. Nachbereitung der Simulation, Aufbereitung der Rohdaten.
  4. Bewertung der Rohdaten und Ermittlung der Fitness (Note).
  5. Bei externen Simulatoren gegebenenfalls Restart bei Simulator-Crash.

Applikationsabhaengig wird das entsprechende Simulatormodul aufgerufen. 

Bei eingeschaltetem "statistik_pro_gen" und Fitnessverbesserung werden Daten-
zeilen ausgegeben, die das zeitabhaengige Fitnesswachstum dokumentieren. Diese 
werden durch ein "#* "-String eingeleitet und enthalten:
   Zeit in Sekunden  Fitness  Simulator-Rohwerte  [Eingangsparameter]
wobei die Eingangsparameter nur bei MBF oder externer Simulation ausgegeben 
werden.


Bewertung:
==========
Ergebnis eines "simulator"-Aufrufs ist immer eine Note, die die Qualitaet des 
Individuums (AK) bezogen auf die vorgegebenen Bewertungskriterien ausdrueckt. 
Die Funktionen zur Berechnung der Noten einzelner Kriterien aus den Ergebnis-
werten des Simulators sind im Package "bew" zusammengefasst. Dort sind auch die 
entsprechenden Daten wie z.B. das Feld "kriterien" zu finden. Noten haben einen 
Wertebereich zwischen 0.0 und 100000.0 und errechnen sich hinsichtlich der Kri-
terien wie folgt. Die Kriterien koennen in Klassen eingeteilt werden, wobei 
mehrere Kriterien in einer Klasse sein koennen. Erst wenn die Noten der Krite-
rien der hoeher prioren Vorgaenger-Klasse jeweils einen vorgegebenen Wert er-
reicht haben, werden die Resultate der Kriterien der aktuellen Klasse bei der 
Gesamtnotenbildung beruecksichtigt. 

Kriterien stehen im Feld "kriterien" im Index-bereich 0 .. "max_anz_bew_krit"-1. 
Im Intervall 'max_anz_bew_krit" .. "max_anz_kriterien"-1 stehen die Straffunk-
tionen fuer jedes Kriterium. Die Straffunktion des Kriteriums mit dem Index "x" 
hat also den Index "x + max_anz_bew_krit". Die Kriterien sind im Feld nicht nach
Klassen sortiert. Vielmehr stehen sie dort in der gleichen Reihenfolge wie in 
der BEW-Datei. Mit dem Klassenwert 0 ("KRITERIUM_AUS") laesst sich ein Kriterium
abschalten. 

Straffunktionen sind nur fuer Kriterien mit linearer oder exponentiell-linearer 
Bewertung (LIN, MIXED_MINCR und MIXED_MDECR) vorgesehen. Mit ihnen
wird die berechnete Gesamt-Note abgewertet, wenn Ergebniswerte ausserhalb der 
zulaessigen Grenzen liegen. Dazu wird anhand der in den Kriterien hinterlegten 
Bewertungsfunktion ein Faktor zwischen 0.0 und 1.0 ermittelt. Mit diesem wird 
dann die Gesamtnote multipliziert. Der Klassenwert einer Straffunktion gibt an, 
ab welchem Klassenlevel die Straffunktion beruecksichtigt wird. So kann z.B. 
die Bereichsgrenzenueberschreitung eines Klasse-3-Kriteriums immer berueck-
sichtigt werden, indem man den Klassenwert der Straffunktion auf 1 setzt. In-
wieweit die damit auch moeglichen Bestrafungen von Bereichsgrenzenueberschrei-
tungen mit niedrigerer Prioritaet als die Bewertung des Kriteriums selbst sinn-
voll ist, sei dahin gestellt. Angewandte Straffunktionen werden im oberen Teil 
von "krit_gueltig" markiert. Die Straffaktoren werden im Feld "straf_fkt" hin-
terlegt.

Bei den Applikationen MATH_FKT_APPL, GADO_APPL oder MATLAB_APPL erfolgt eine 
Parameter- statt eine AK-Bewertung. Die Abbildung der Parameter der Aktionen 
einer AK in die Felder "i_params" und "r_params" haengt vom AK-Typ ab ("dyn_aks" 
TRUE oder nicht). Die Felder haben folgenden Aufbau:

               0   1         len i_par_anf
               v   v         v   v       
              +---+---+-----+---|----+-----+----+
    i_params: |len|ac | ... |ac |a i | ... |a i |   r_params: akt_rpar_anz Werte
              |   |  1|     |  n| 1 1|     | n 1|
              +---+---+-----+---|----+-----+----+
              \                  \             //
               \                  akt_ipar_anz-/
                -- all_iwerte_anz -------------


  Fall statischer AKs:
  ====================
  Abbildung der Aktionen in der im Handlungsmodell definierten Aktionsreihen-
  folge: Zuerst alle Integer und dann alle Real-Parameter der AK. Die Reihen-
  folge der Aktionen einer AK spielt also fuer ihre Bewertung keine Rolle. 
  i_par_anf = 0

  Fall dynamischer AKs:
  =====================
  Abbildung der Aktionen in der in der AK definierten Aktionsreihenfolge: 
  Zuerst alle Integer und dann alle Real-Parameter jeder Aktion der AK.

In beiden Faellen werden die Integer- und Realparameter vollstaendig nach 
"i_params" und "r_params" abgebildet. 


Die Ergebnisse eines Simulatorlaufs stehen im Feld "simu_erg" derart, dass An-
zahl und Reihenfolge der Werte in "simu_erg" mit der der Kriterien in "krite-
rien" uebereinstimmen. Fuer eine eventuelle applikationsspezifische Aufbereitung
von Simulationsrohdaten steht das Feld "simu_roh_erg_werte"  zur Verfuegung. Es
wird vom Packages "hmod" entsprechend den Vorgaben des MOD-Files angelegt.


Simulatorschnittstelle:
=======================
Der Simulatorkern bindet applikationsabhaengig interne und externe Simulatoren 
ueber eine gemeinsame Schnittstelle ein. Bei Verwendung externer Simulatoren
wird beim MOD-File-Lesen (Routine "read_mod_file" in Package "hmod") die glo-
bale Variable "mit_ext_sim" gesetzt.

Eingangsparameter:
  entweder 
    "i_params" und "r_params"   die aus der AK extrahierten Parameter (Feld).
  oder
    "ch_ptr"                    die AK selbst
  "interaktiv"                  gibt an, ob der Simulator interaktiv oder nur 
                                zur Ergebnisdatenermittlung benutzt wird. Bei
                                externen Simulatoren ist ein interaktiver Be-
                                trieb nicht vorgesehen. Optionaler Parameter.
  "akt_anz_roh_erg_werte"       Anzahl der von der aktuellen Anwendunge erwar-
                                teten Ergebnis-Rohwerte in "simu_roh_erg_werte". 
                                DiesenWert kann der Simulator zu Pruefzwecken 
                                verwenden.

Ausgangsparameter:
  "simu_erg"                    Ergebniswerte des Simulationslaufs (Feld).
  <funktions_ergebnis>          Der Boolsche Returnparameter des Simulatorauf-
                                rufs gibt an, ob die AK bewertbar war und die
                                Ergebniswerte damit gueltig sind oder nicht.
  "sim_up"                      Ist TRUE solange der Simulator arbeitsfaehig 
                                ist. Andernfalls sind wahrscheinlich Fehlermel-
                                dungen vorhanden.
  "sim_ok"                      Ist TRUE solange die Kopplung zum externen
                                Simulator besteht.

Deklaration der Schnittstellendaten:
  "i_params" und "r_params"     im Package "hmod", Modul "
  "simu_erg"                    im Package "bew", Modul "bewert" 
  "sim_up"                      im Package "hmod", Modul "segm_gen" 
  "sim_ok"                      im Package "sys", Modul "extkoppl"
  "simu_roh_erg_werte"          im Package "hmod", Modul "segm_gen" 

Die beiden zulaessigen Funktionsprototypen fuer den Simulator haben somit fol-
gendes Aussehen ("interaktiv"-Parameter optional):
          BOOL <simu_fkt_name> (DOUBLE *simu_erg,
                                INT    *i_params,
                                DOUBLE *r_params,
                                BOOL    interaktiv);
  oder
          BOOL <simu_fkt_name> (DOUBLE              *simu_erg,
                                CHAIN_ELEM_PTR_TYPE  ch_ptr,
                                BOOL                 interaktiv);

Bisher eingebunden sind folgende Simulatoren:

          BOOL do_ext_sim      (DOUBLE   *erg_werte,
                                INT      *i_params,
                                DOUBLE   *r_params);
          Schnittstelle zum externen Simulator.

          BOOL do_rob_sim      (DOUBLE              *simu_erg_werte,
                                CHAIN_ELEM_PTR_TYPE  ch_ptr,
                                BOOL                 interaktiv);
          LESAK-Simulator aus Modul "rob_sim".

          BOOL do_appl_sim     (DOUBLE              *simu_erg_werte,
                                CHAIN_ELEM_PTR_TYPE  ch_ptr,
                                BOOL                 interaktiv,
                                GSTATE              *erg_state);
          Allgemeine Simulatoranbindung von zusaetzlichen Anwendungen ueber
          Package "appl".

          BOOL do_mbf_sim      (DOUBLE   *simu_erg_werte,
                                INT       r_par_anz,
                                DOUBLE   *r_params);
          Simulator fuer mathematische Benchmarkfunktionen (Module "mbf_sim").




Die Routinen im einzelnen:
  
GSTATE get_ak_params (INT                 *iParams,
                      DOUBLE              *rParams,
                      CHAIN_ELEM_PTR_TYPE  ak_ptr,
                      BOOL                 interaktiv);
          Die Routine extrahiert die Integer- und Realparameter einer extern zu
          simulierenden oder MBF-AK (ak_ptr) in "iParams" und "rParams". Wenn
          die AK-Laenge nicht stimmt, erfolgt eine Fehlermeldung und der Return-
          status wird bei gesetztem "interaktiv" GLEAM_ERROR sonst GLEAM_FATAL.
          Siehe auch obige Beschreibung der beiden Parameterfelder.

GSTATE simulator (DOUBLE              *note,
                  BOOL                 interaktiv,
                  CHAIN_ELEM_PTR_TYPE  ak_ptr);
          Je nach Anwendung interpretiert die Prozedur das mit "ak_ptr" bezeich-
          nete Individuum (AK) und startet mit den so gewonnen Daten den exter-
          nen Simulator bzw. ruft mit den Daten oder dem "ak_ptr" selbst den in-
          ternen Simulator auf. Der Simulatoraufruf erfolgt nur, wenn in der AK
          nicht das Flag "simulated" gesetzt oder "always_sim" TRUE ist. Anson-
          sten werden die im Kettenkopf hinterlegten Ergebniswerte benutzt.
          Andernfalls werden die Ergebnisdaten des (externen) Simulators entge-
          gengenommen, hinterlegt und bewertet. Ergebnis eines Simulatoraufrufs 
          ist immer eine "note", die, wie oben beschrieben, errechnet wird. Bei 
          Abbruch der Simulation und sonst unbewertbaren Ketten hat "note" den 
          Wert "KEINE_NOTE"; extrem schlechte Ketten haben dagegen den Notenwert 
          Null. Ausser im Falle von "KEINE_NOTE" wird die Note auch in den Ket-
          tenkopf eingetragen.
          Der Parameter "interaktiv" gibt an, ob der Simulator interaktiv als 
          Benutzerfunktion gestartet wurde, oder ob er im Kontext der Evolution 
          als Fitness-Funktion aufgerufen wurde. Wenn im nicht-interaktiven 
          Modus nach einer Simulation "sim_up" FALSE ist, erfolgt ein zyklisches 
          Recovery des externen Simulators.
          Bei auftretenden Fehlern (Nil-Zeiger, fehlerhaften AKs, usw) liefert 
          der Simulator GLEAM_ERROR oder GLEAM_FATAL (und KEINE_NOTE) ab, sonst 
          GLEAM_OK.

void bewert_data_to_best_data (DOUBLE  gesamt_note);
          Daten der besten AK des letzten Evo-Laufs festhalten.                   

void bewert_data_to_int_data (DOUBLE  gesamt_note);
          Daten der letzten interaktiven Simulation fuer die Ergebnisanzeige   
          festhalten.                                                             

void reset_simu_erg (void);
          Ruecksetzung der Ergebnisdaten der besten AK. Ist daher vor Beginn 
          eines Evolutionslaufs aufzurufen.

BOOL print_exp_data (FILE *out_file);
          Gibt die Experimentdaten (Filenamen der geladenen Files) in die Datei
          "out_file" aus und liefert TRUE, wenn alles gut ging, sonst FALSE.

BOOL simu_gsw_init (void);
          Initialisiert die Filedeskriptoren der Standard-Initialisierungsfiles 
          und ist vor "simuInit" aufzurufen!

BOOL simuInit (BOOL *ext_sim_mod_ok);
          Modulinitialisierung und Anlegen der internen Datenstrukturen. Liefert
          FALSE, wenn dazu nicht ausreichend Platz ist oder "rob_init" oder 
          "init_ext_koppl" FALSE liefern. "ext_sim_mod_ok" wird von "init_ext_
          koppl" geliefert.
          Bei Verwendung externer Simulatoren erfolgt eine Pruefung der Simula-
          tionsmodell-Namen (Environment oder Default) und eine Initialisierung 
          des Kopplungsmoduls ("init_ext_koppl"). "ext_sim_mod_ok" enthaelt das 
          Pruefergebnis bzw ist TRUE, wenn intern simuliert wird. Bei fehlerhaf-
          tem Modellpfad erfolgen (Fehler-)Meldungen. Bei interner Simulation 
          wird "sim_up" auf TRUE gesetzt.
          Diese Routime darf erst NACH dem Lesen des TSK-Files aufgerufen werden.

--------------------------------------------------------------------------------
basiert auf C-Unit: simu
--------------------------------------------------------------------------------
Known Bugs:
Fehler bei der Prio-Klassenauswertung zusammengesetzter Bewertungsfunktionen. 
Siehe Beschreibung der Routine "ak_bewerten".
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
Zusammengesetzte Bewertungsfunktionen. 
Erweiterung der Schnittstelle zu externen Simulatoren.
--------------------------------------------------------------------------------
Aenderung gegenueber Version 2.0.0:
Im Rahmen der Bereinigung des Bewertungs-Packages mit dem Ziel, alle Bewertungs-
routinen darin zusammenzufassen und einen Kern bereit zustellen, der die reine 
Algorithmik der Bewertung und das File-I/O abdeckt, wurde die Routine "ak_
bewerten" in das Package "bew" transferiert.
--------------------------------------------------------------------------------
Aenderung gegenueber Version V3.0.0:
 1. Erweiterung des Logging-Verhaltens: Bei eingeschalteten Optionen "Genera-
    tionsweise Statistik" und "Merken des Evo-Besten" werden die Daten (Note, 
    Eingangsparameter bei externer Simulation und Ergebnbiswerte) jeder Verbes-
    serung im Laufeeines Evo-Laufs geloggt. Zeilenkennung "#*".
 2. "mit_simulog" per Default auf FALSE.
 3. Verbesserte Fehlerbehandlung bei der applikationsspezifischen Simulation.
 5. "mit_simulog" wir exportiert.                                    WJ 04.08.99
 6. "i_params", "r_params" usw. werden mit an das HMod angepasster  
    Länge von Packages "hmod" exportiert. Verbesserung der Design-
    Parameter-Aufbereitung.                                          WJ 13.08.00
 7. Spezialanbindung dynamischer AKs eines Aktionstyps f.ext.Simu.   WJ 28.09.00
 8. Die Spezialanbindung von 7 f.allg.dyn. AKs u.ext.Simu. erweitert 
    und Parameterextraktionsroutine "get_ak_params" eingefuehrt.     WJ 22.02.01
 9. Fuer die Engine: Neue Variable "res_fspec" u. RES-File-Update
    am Ende von "simu".                                              WJ 14.06.02
10. "simulator" liefert bei erfolglosem Recovery GLEAM_ERROR.        WJ 18.03.03
11. Aufruf von "init_mbf_sim" wegen Achsenrotation eingefuegt.       WJ 24.04.03
12. Neu fuer MatLab-Koppung: "mitOnlVisu".                           WJ 23.08.07
13. Integration d.MBFs d.CEC05-Sammlung u.neu: "mitMatLabSimu".      
    Bei CEC_MBF Pflege von "cec_best_fval".            
14. MAXDOUBLE wird durch DBL_MAX aus "float.h" ersetzt.              WJ 31.01.08
15. Anbindung eines Aehnlichkeitsvergleichs von Simulationsauftraegen 
    mit bereits simulierten Individuen (MIT_AEHNL_CHECK).            WJ 16.05.08
16. "iParams" und "rParams" werden an "get_ak_params" uebergeben.    WJ 11.06.08
17. Verschiebung von "mit_simulog" nach Package "sys".               WJ 20.04.12
18. Explizite Klammern zur eindeutigen Zuordnung von else-Zweigen. 
    Initialisierung von "log_ok" in "print_exp_data".                WJ 27.06.12
19. Einfuehrung der Topologiefilespezifikation "top_file_spec".
    Einfuehrung von sprachspezifischen Stringkonstanten.             WJ 27.08.12
20. Fehler in "print_exp_data" beseitigt.                            WJ 24.09.12
21. Anpassungen des Aufrufs von "save_simu_erg".                     WJ 04.12.14
22. Integration von "simValOutOfEvalRange" und Abkoemmlinge.         WJ 21.07.16
23. Einfuehrung von "reset_krit_gueltig" und 
    "reserSimValOutOfEvalRange".                                     WJ 25.08.16
24. Anpassungen an die neue TSK-Datenverwaltung. Verlagerung von 
    "mit_simu_anim" nach "rob_int.h". Verbesserungen in 
    "print_exp_data".                                                WJ 21.12.16
25. Neue Variable "extSimuModelName" und "extSimuRunLimit".          WJ 03.03.17
26. Anpassungen an ext. matlab/matpower-Simulatoren.                 WJ 26.04.17
27. Uebernahme von "lastSimuModelName" aus "extSimuServKoppl" bzw.
    aus "matlab_koppl" als globale Variable wegen Init. bei der allg.
    Package-Init. ("simu_gsw_init") und "processReadTskData". Neue 
    Variable "simuResultWaitCycles".                                 WJ 01.06.17
28. Anpassung des Aufrufs von "get_ak_params" und der externen 
    Simulation an die neue Variable "extractChrParams".              WJ 14.08.17
29. Neu: "extSimServDomainName" und "lastSimServDomainName". An-
    passung an Neuordnung der zentralen Textfiles.                   WJ 06.10.17
30. "save_simu_erg" wird in der CLV so aufgerufen, dass die 
    Ergebnisdatei erweiternd gschrieben wird. Die Laenge von 
    "exp_file_spec" und "res_fspec" wird auf MAX_STRING erhoeht.     WJ 05.02.18
31. Kleinere Anpassungen am VISU_MODE-bezogenen Code in simulator(). WJ 26.03.18
32. Simulator(): Verbesserte Ausgabe der Wachstumsdaten in der 
    "#*"-Zeile. Transfer of "mit_rz" and "statistik_pro_gen" to the
    "aufg"-package.                                                  WJ 20.07.18
33. Anpassung der Aufrufe von get_act_descr().                       WJ 28.06.19
34. Include von "lgsw.h" anstelle von "chaindef.h".                  WJ 08.07.19
35. Korrektur des Aufrufs von get_act_descr() in get_ak_params() 
    bei gesetztem MIT_LSV.                                           WJ 12.08.19
36. Enlargement of simuLogBuf to avoid buffer overflow. Avoiding 
    buffer overflow of static strings in: print_exp_data().          WJ 18.07.20
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 18.07.2020
================================================================================ */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "lgsw.h"
#include "bew.h"
#include "appl.h"
#include "hmod.h"
#include "aufg.h"
#include "simu.h"
#include "simuInt.h"

#include GLOBAL_TEXTS
#include SIMU_TEXTS

#ifdef GLO_CHECKS
  #define PTR_CHECK                                    /* Parameterpruefungen */
#endif

#define LOG_GENE_VALUES



/* ========================== Exportierte Variable: ========================= */
char   *exp_file_spec,           /* File-Bezeichner fuer Experimentfile       */
       *res_fspec,               /* File-Bezeichner fuer Resultfile (engine)  */
       *extSimuModelName,        /* name of the simulation model of extSimu.  */
       *lastSimuModelName,       /* last loaded simulator model               */
       *extSimServDomainName,    /* domain name of extern.simulation services */
       *lastSimServDomainName;   /* last domain name of external simu. serv.  */
DOUBLE  cec_best_fval;           /* CEC: bester FktWert seit "reset_simu_erg" */
INT     extSimuRunLimit,         /* limit of runs for reinit. of ext.Simu     */
        simuResultWaitCycles,    /* wait cycles for simulator reply TMO       */
        erg_genauigkeit = 7;     /* Genauigkeit der Evo-Ergebniswerte         */
BOOL    int_sim_data_ready;      /* TRUE, wenn interaktive SimuDaten gueltig  */


/* ======================== Package-globale Variable: ======================= */
char    simuLogBuf[2*MAX_STRING+MAX_BUF];  /* general buffer for log messages */
DOUBLE  int_gesamt_note,    /* Ergebnis des letzten interaktiven SimuLaufs    */
        best_ges_note;      /* Note der Besten AK seit "reset_simu_erg".      */
                            /* Export bei gesetzem EXT_SIMU_SERV              */
                            /* best_ges_note = -1: Keine guelt. Werte         */
char   *akt_sim_mod_path;   /* Pfad zum aktuellen Simu-Modell                 */




/* ============================== get_ak_params ============================= */
GSTATE get_ak_params (INT                 *iParams,
                      DOUBLE              *rParams,
                      CHAIN_ELEM_PTR_TYPE  ak_ptr,
                      BOOL                 interaktiv)
{
   CHAIN_ELEM_PTR_TYPE  item_ptr;
   ACT_DESCR_PTR_TYPE   this_act_ptr;
   GSTATE               erg_status = GLEAM_OK;
   INT                  i_par_index,  /* Integer-Parameter-Index              */
                        r_par_index,  /* Real-Parameter-Index                 */
                        ak_len,       /* AK-Laenge                            */
                        ai,           /* Aktionscode                          */
                        i_idx,        /* 2.Index in i_params f.dyn.ext.sim.AK */
                        i_anz, ii;

   /* get_ak_params */
   r_par_index = 0;
   ak_len      = ak_ptr->u_el.u_xel.u_hdr.chain_length;
   if (dyn_aks && mit_ext_simu)
   { /* extern zu simulierende dyn.AK */
      i_par_anf         = ak_len + 1;
      i_par_index       = i_par_anf;
      i_idx             = 0;
      iParams[i_idx++]  = ak_len;
      item_ptr          = ak_ptr->u_el.next_action;
      while (item_ptr != NULL)
      { /* AK abklappern */
         ai = item_ptr->u_el.activity;
         iParams[i_idx++] = ai;
         i_anz = int_pro_aktion[ai];
#ifdef MIT_LSV
         if ((erg_status |= 
              get_act_descr(&this_act_ptr, (SHORT)ai, WITHOUT_GENECODE_CHECK)) != GLEAM_OK)
         { /* Aktionsdescriptor nicht erhalten */
            if (!interaktiv)
               erg_status = fatal (SIMU_SIMU, 1, ai, NO_INFO, "", BAD_ACTION);
            break;
         } /* Aktionsdescriptor erhalten */
#endif /* MIT_LSV */
         for (ii = 0;  ii < i_anz;  ii++)
         { /* alle I-Parameter */
            if (i_par_index < i_par_ges_anz)                   /* Noch Platz? */
            { /* Platz ok */
               iParams[i_par_index]    = item_ptr->u_el.u_xel.u_act.i_params[ii];
#ifdef MIT_LSV
               ug_iparams[i_par_index] = this_act_ptr->i_par_descr[ii].par_u_grenze;
               og_iparams[i_par_index] = this_act_ptr->i_par_descr[ii].par_o_grenze;
#endif /* MIT_LSV */
            } /* Platz ok */
            i_par_index++;        /* weiterzaehlen, damit Fehler bemerkt wird */
         } /* alle I-Parameter */
         for (ii = 0;  ii < real_pro_aktion[ai];  ii++)
         { /* alle R-Parameter */
            if (r_par_index < r_par_ges_anz)                   /* Noch Platz? */
            { /* Platz ok */
               rParams[r_par_index]    = item_ptr->u_el.u_xel.u_act.r_params[ii];
#ifdef MIT_LSV
               ug_rparams[r_par_index] = this_act_ptr->r_par_descr[ii].par_u_grenze;
               og_rparams[r_par_index] = this_act_ptr->r_par_descr[ii].par_o_grenze;
#endif /* MIT_LSV */
            } /* Platz ok */
            r_par_index++;        /* weiterzaehlen, damit Fehler bemerkt wird */
         } /* alle R-Parameter */
         item_ptr = item_ptr->u_el.next_action;       /* zur naechsten Aktion */
      } /* AK abklappern */
      all_iwerte_anz = i_par_index;
      akt_ipar_anz   = i_par_index - i_par_anf;
      akt_rpar_anz   = r_par_index;
      if (i_par_index > i_par_ges_anz) 
      {
         if (interaktiv)
            erg_status = fehler (SIMU_SIMU, 2, i_par_index, i_par_ges_anz, "", BAD_CH_LEN);
         else
            erg_status = fatal  (SIMU_SIMU, 2, i_par_index, i_par_ges_anz, "", BAD_CH_LEN);
      }
      if (r_par_index > r_par_ges_anz)
      {
         if (interaktiv)
            erg_status = fehler (SIMU_SIMU, 3, r_par_index, r_par_ges_anz, "", BAD_CH_LEN);
         else
            erg_status = fatal  (SIMU_SIMU, 3, r_par_index, r_par_ges_anz, "", BAD_CH_LEN);
      }
   } /* extern zu simulierende dyn. AK */
   else
   { /* statische AK */
      i_par_anf   = 0;
      i_par_index = 0;
      for (ai = 1;  ai <= ak_len;  ai++)
      { /* Parameterwerte ermitteln */
         item_ptr = ak_ptr->u_el.next_action;
         while ((item_ptr->u_el.activity != (SHORT)ai) && 
                (item_ptr->u_el.next_action != NULL))
            item_ptr = item_ptr->u_el.next_action;    /* zur naechsten Aktion */
         if (item_ptr->u_el.activity == (SHORT)ai)
         { /* Item gefunden */
            for (ii = 0;  ii < int_pro_aktion[ai];  ii++)
            { /* alle I-Parameter */
               if (i_par_index < i_par_ges_anz)                /* Noch Platz? */
                  iParams[i_par_index]=item_ptr->u_el.u_xel.u_act.i_params[ii];
                  i_par_index++;  /* weiterzaehlen, damit Fehler bemerkt wird */
            } /* alle I-Parameter */
            for (ii = 0;  ii < real_pro_aktion[ai];  ii++)
            { /* alle R-Parameter */
               if (r_par_index < r_par_ges_anz)
                  rParams[r_par_index]=item_ptr->u_el.u_xel.u_act.r_params[ii];
               r_par_index++;     /* weiterzaehlen, damit Fehler bemerkt wird */
            } /* alle R-Parameter */
         } /* Item gefunden */
      } /* Parameterwerte ermitteln */
      all_iwerte_anz = i_par_index;
      akt_ipar_anz   = i_par_index;
      akt_rpar_anz   = r_par_index;
      if (i_par_index != i_par_ges_anz) 
      {
         if (interaktiv)
            erg_status = fehler (SIMU_SIMU, 4, i_par_index, i_par_ges_anz, "", BAD_CH_LEN);
         else
            erg_status = fatal  (SIMU_SIMU, 4, i_par_index, i_par_ges_anz, "", BAD_CH_LEN);
      }
      if (r_par_index != r_par_ges_anz)
      {
         if (interaktiv)
            erg_status = fehler (SIMU_SIMU, 5, r_par_index, r_par_ges_anz, "", BAD_CH_LEN);
         else
            erg_status = fatal  (SIMU_SIMU, 5, r_par_index, r_par_ges_anz, "", BAD_CH_LEN);
      }
   } /* statische AK */
   return (erg_status);
} /* get_ak_params */



/* ================================ simulator =============================== */
GSTATE simulator (DOUBLE              *note,
                  BOOL                 interaktiv,
                  CHAIN_ELEM_PTR_TYPE  ak_ptr)
{ 
   CHAIN_ELEM_PTR_TYPE  item_ptr;
   INT                  ii;
   GSTATE               ret_state    = GLEAM_OK;
   BOOL                 bewerten     = FALSE, 
                        ext_sim_done = FALSE,
                        call_simu;

   /* simulator */
   *note = KEINE_NOTE;
#ifdef PTR_CHECK
   if (ak_ptr == NULL)
      return (fehler (SIMU_SIMU, 6, NO_INFO, NO_INFO, "", NIL_PTR));
   if (!dyn_aks && (ak_ptr->u_el.u_xel.u_hdr.chain_length != last_act))
   { /* Kettenlaenge bei fixierter Laenge nicht ok */
      if (interaktiv)
         return (fehler (SIMU_SIMU, 7, ak_ptr->u_el.u_xel.u_hdr.chain_length, 
                          last_act, "", BAD_CH_LEN));
      else
         return (fatal  (SIMU_SIMU, 7, ak_ptr->u_el.u_xel.u_hdr.chain_length, 
                         last_act, "", BAD_CH_LEN));
   } /* Kettenlaenge bei fixierter Laenge nicht ok */
#endif /* PTR_CHECK */

   if (extractChrParams || (interaktiv && displayableParams))
   { /* extract parameters of static chromomosomes or dynamic ones for ext.simu */
      ret_state = get_ak_params(i_params, r_params, ak_ptr, interaktiv);
      paramsExtracted = TRUE;                           /* for result display */
   } /* extract parameters of static chromomosomes or dynamic ones for ext.simu */
   else
      paramsExtracted = FALSE;                          /* for result display */

   /* ---------------------- Simulator oder alte Werte: --------------------- */
   item_ptr  = ak_ptr->u_el.u_xel.u_hdr.header_par_ptr;
   call_simu = TRUE;
#ifdef MIT_AEHNL_CHECK
   if (!always_sim && std_simu_interface) 
   { /* AK soll simuliert werden */
      bewerten = TRUE;
      if (ak_ptr->u_el.u_xel.u_hdr.chain_status & SIMULATED)
      { /* alte Simulator-Werte nehmen */
	 for (ii = 0;  ii < akt_anz_krit;  ii++)
	    simu_erg[ii] = item_ptr->sim_erg_werte[ii];
	 call_simu = FALSE;
      } /* alte Simulator-Werte nehmen */
      else
	 call_simu = unknownIndiv (simu_erg, i_params, r_params);
   } /* AK soll simuliert werden */
#else /* kein MIT_AEHNL_CHECK */
   if (!always_sim && std_simu_interface && 
       (ak_ptr->u_el.u_xel.u_hdr.chain_status & SIMULATED))
   { /* alte Simulator-Werte nehmen */
      for (ii = 0;  ii < akt_anz_krit;  ii++)
         simu_erg[ii] = item_ptr->sim_erg_werte[ii];
      bewerten  = TRUE;
      call_simu = FALSE;
   } /* alte Simulator-Werte nehmen */
#endif /* kein MIT_AEHNL_CHECK */
   
   if (call_simu && (ret_state == GLEAM_OK))
   { /* call simulator and save valid results */
      if (mit_ext_simu) 
      { /* Aufruf des externen Simulators */
	 if (extractChrParams)
	    bewerten = do_ext_sim (simu_erg, i_params, r_params);
#if defined(EXT_MATLAB_SIMU) || defined(EXT_SIMU_SERV)
	 else
  #ifdef EXT_MATLAB_SIMU
	    bewerten = doMatlabSim(simu_erg, ak_ptr);
  #endif /* EXT_MATLAB_SIMU */
  #ifdef EXT_SIMU_SERV
	    bewerten = do1ExtSimServRun(simu_erg, ak_ptr);
  #endif /* EXT_SIMU_SERV */
#endif /* EXT_MATLAB_SIMU oder defined(EXT_SIMU_SERV */
	 ext_sim_done = TRUE;
      } /* Aufruf des externen Simulators */
      else 
	 if (appl_code == LESAK_APPL)
	    bewerten = do_rob_sim (simu_erg, ak_ptr, interaktiv);
	 else
	    if (appl_code == MATH_FKT_APPL)
	       bewerten = do_mbf_sim (simu_erg, akt_rpar_anz, r_params);
	    else
	       if (appl_code == CEC_MATHFKT_APPL)
		  bewerten = do_cec05_mbf_sim (simu_erg, akt_rpar_anz, ext_simu_code, 
					       r_params);
	       else  /* applikationsspezifische Simulation */
		  bewerten = do_appl_sim (simu_erg, ak_ptr, interaktiv, &ret_state);
      if (bewerten && !do_always_sim) { /* Daten ok und nicht zuviele ? */
	 for (ii = 0;  ii < akt_anz_krit;  ii++) 
	    item_ptr->sim_erg_werte[ii] = simu_erg[ii];
#ifdef MIT_AEHNL_CHECK
	 if (!update(simu_erg, i_params, r_params))
	    meldung (SIMU_SIMU, NO_INFO, NO_INFO, "Kein Update der Simu-Datenbasis!", 
		     EXT_KOPPL_ERR);
#endif /* MIT_AEHNL_CHECK */
      } /* Daten ok und nicht zuviele ? */
   } /* call simulator and save valid results */

   /* -------------------- Simu-Ergebnisdaten bewerten: --------------------- */
   if (bewerten && (ret_state == GLEAM_OK))
   { /* Bewertung einer Aktionskette */
      *note = ak_bewerten (interaktiv);
      ak_ptr->u_el.u_xel.u_hdr.fitness_note = *note;
      ak_ptr->u_el.u_xel.u_hdr.chain_status |= SIMULATED;

      if ((*note > best_ges_note) && statistik_pro_gen)
      { /* mit generationsweiser Statistik u. es gab eine Verbesserung */
	 if (!logfile_open) {
	    logfile = fopen (logfilename, "at");
	    logfile_open = TRUE;
	 }
	 fprintf (logfile, "#* %2d %e %13.6f ", akt_anz_krit, calcElapsedTimeMS(), *note);
	 for (ii = 0;  ii < akt_anz_krit;  ii++)
	    fprintf (logfile, " %.*g", erg_genauigkeit, simu_erg[ii]);
#if defined(LOG_GENE_VALUES) && !defined(EXT_MATLAB_SIMU)
	 if (mit_ext_simu || mbf_appl)
         { /* Eingangsparameter loggen */
	    for (ii = 0;  ii < i_par_ges_anz;  ii++)
	       fprintf (logfile, " %d", i_params[ii]);
	    fprintf (logfile, " ");
	    for (ii = 0;  ii < r_par_ges_anz;  ii++)
	       fprintf (logfile, " %.*G", erg_genauigkeit, r_params[ii]);
	    fprintf (logfile, " ");
	 } /* Eingangsparameter loggen */
#endif /* LOG_GENE_VALUES and not EXT_MATLAB_SIMU */
         fprintf (logfile, "\n");
	 fflush (logfile);
	 fclose (logfile);
	 logfile_open = FALSE;
      } /* mit generationsweiser Statistik u. es gab eine Verbesserung */
      if (interaktiv) 
      { /* interaktiv oder finale Simu der CLV */
         bewert_data_to_int_data(*note);       /* Daten f.interaktive Anzeige */
#ifdef VISU_MODE
	 save_simu_erg(res_fspec, FALSE, TRUE);    /* ..., neu_file, mitIndex */
#endif /* VISU_MODE */
      } /* interaktiv oder finale Simu der CLV */
      else
         if (record_evo_best && (*note > best_ges_note))
	    /* nicht interaktiv, EvoBest merken u. es gab eine Verbesserung: */
            bewert_data_to_best_data (*note);
#if !defined(GLEAM_USR) && !defined(VISU_MODE)
      if (record_evo_best && (*note > best_ges_note))
      { /* EvoBest merken u. es gab eine Verbesserung */
         best_ges_note = *note;
         bewert_data_to_int_data (*note);  /* Daten fuer Ergebnis im RES-File */
         save_simu_erg (res_fspec, FALSE, TRUE);   /* ..., neu_file, mitIndex */
      } /* EvoBest merken u. es gab eine Verbesserung */
#endif /* neither GLEAM_USR nor VISU_MODE */
#ifdef CEC_MBF
      if (simu_erg[0] < cec_best_fval) 
         cec_best_fval = simu_erg[0];
#endif /* CEC_MBF */
   } /* Bewertung einer Aktionskette */

   /* ---------------- Eventuell Ext.Simulator wiederbeleben: --------------- */
   if (!sim_up && !interaktiv && ext_sim_done)
   { /* Recovery des externen Simulators */
      ext_simu_recovery (); 
      if (!sim_up)
         ret_state |= GLEAM_ERROR;
   } /* Recovery des externen Simulators */             
   return (ret_state);
} /* simulator */



/* ========================= bewert_data_to_int_data ======================== */
void bewert_data_to_int_data (DOUBLE  gesamt_note)
{
   INT  ii;

   /* bewert_data_to_int_data */
   int_gesamt_note = gesamt_note;
   for (ii = 0;  ii < akt_anz_krit;  ii++)
      int_simu_erg[ii] = simu_erg[ii];
   for (ii = 0;  ii < akt_anz_krit;  ii++)
   {
      int_ungew_note[ii] = ungew_note[ii];
      int_gew_note[ii]   = gew_note[ii];
      int_straf_fkt[ii]  = straf_fkt[ii];
      int_simValOutOfEvalRange[ii] = simValOutOfEvalRange[ii];
   }
   for (ii = 0;  ii < krit_og;  ii++)
      int_krit_gueltig[ii] = krit_gueltig[ii];
   for (ii = 0;  ii < i_par_ges_anz;  ii++)
      int_i_params[ii] = i_params[ii];
   for (ii = 0;  ii < r_par_ges_anz;  ii++)
      int_r_params[ii] = r_params[ii];
   int_sim_data_ready = TRUE;
} /* bewert_data_to_int_data */



/* ======================== bewert_data_to_best_data ======================== */
void bewert_data_to_best_data (DOUBLE  gesamt_note)
{
   INT  ii;

   /* bewert_data_to_best_data */
   best_ges_note = gesamt_note;
   for (ii = 0;  ii < akt_anz_krit;  ii++)
      best_simu_erg[ii] = simu_erg[ii];
   for (ii = 0;  ii < akt_anz_krit;  ii++)
   {
      best_ug_note[ii] = ungew_note[ii];
      best_g_note[ii]  = gew_note[ii];
      best_str_fkt[ii] = straf_fkt[ii];
      best_simValOutOfEvalRange[ii] = simValOutOfEvalRange[ii];
   }
   for (ii = 0;  ii < krit_og;  ii++)
      best_krit_g[ii] = krit_gueltig[ii];
   for (ii = 0;  ii < i_par_ges_anz;  ii++)
      best_i_params[ii] = i_params[ii];
   for (ii = 0;  ii < r_par_ges_anz;  ii++)
      best_r_params[ii] = r_params[ii];
} /* bewert_data_to_best_data */



/* ============================= reset_simu_erg ============================= */
void reset_simu_erg (void)
{
   /* reset_simu_erg */
   best_ges_note = -1;            /* d.h., dass keine Ergebniswerte vorliegen */
   cec_best_fval = DBL_MAX;
} /* reset_simu_erg */



/* ============================= print_exp_data ============================= */
BOOL print_exp_data (FILE *outfile)
{
   STRING_TYPE  buffer;
   INT          ii;
   BOOL         log_ok;

   /* print_exp_data */
   if (strcmp (exp_file_spec, ""))
   {
      sprintf (simuLogBuf, "%s%s\n", EXP_FILE_ANZG_TXT, exp_file_spec);
      log_ok = print_and_check (outfile, simuLogBuf);
   }
   else
      log_ok = TRUE;
   if (!getStringVal(buffer, MOD_FILE_SPEC_IDX, GLEAM_ERROR))
      strcpy (buffer, "");
   sprintf (simuLogBuf, "%s%s\n", MOD_FILE_ANZG_TXT, buffer);
   log_ok &= print_and_check (outfile, simuLogBuf);
   if (!getStringVal(buffer, BEW_FILE_SPEC_IDX, GLEAM_ERROR))
      strcpy (buffer, "");
   sprintf (simuLogBuf, "%s%s\n", BEW_FILE_ANZG_TXT, buffer);
   log_ok &= print_and_check (outfile, simuLogBuf);
   if (!getStringVal(buffer, TSK_FILE_SPEC_IDX, GLEAM_ERROR))
      strcpy (buffer, "");
   sprintf (simuLogBuf, "%s%s\n", TSK_FILE_ANZG_TXT, buffer);
   log_ok &= print_and_check (outfile, simuLogBuf);
   if (!getStringVal(buffer, EVO_FILE_SPEC_IDX, GLEAM_ERROR))
      strcpy (buffer, "");
   sprintf (simuLogBuf, "%s%s\n", EVO_FILE_ANZG_TXT, buffer);
   log_ok &= print_and_check (outfile, simuLogBuf);
   if (getStringVal(buffer, MEM_FILE_SPEC_IDX, GLEAM_OK))
   {
      if (strlen(buffer) == 0)
	 sprintf(simuLogBuf, "%s\n", NO_CH_FILE_TXT);
      else
	 sprintf(simuLogBuf, "%s%s\n", CH_FILE_ANZG_TXT, buffer);
      log_ok &= print_and_check(outfile, simuLogBuf);
   }
   if (getStringVal(buffer, TOP_FILE_SPEC_IDX, GLEAM_OK))
   {
      if (strlen(buffer) > 0) {
	 sprintf(simuLogBuf, "%s%s\n", TOP_FILE_ANZG_TXT, buffer);
	 log_ok &= print_and_check(outfile, simuLogBuf);
      }
   }
   if (appl_file_anz > 0)
   { /* Es gibt zusaetzl. Anwendungsfiles */
      sprintf (simuLogBuf, "%s %d\n", ADD_FILE_ANZG_TXT, appl_file_anz);
      log_ok &= print_and_check (outfile, simuLogBuf);
      for (ii = 0;  ii < appl_file_anz;  ii++)
      {
         sprintf (simuLogBuf, "  %-*s: %s\n", AFILE_BEZ_LEN, 
                  appl_file_descr[ii].afile_bez, 
                  appl_file_descr[ii].afile_spec);
         log_ok &= print_and_check (outfile, simuLogBuf);
      }
   } /* Es gibt zusaetzl. Anwendungsfiles */
   sprintf (simuLogBuf, "\n");
   log_ok &= print_and_check (outfile, simuLogBuf);
   return (log_ok);
} /* print_exp_data */



/* ============================== simu_gsw_init ============================= */
BOOL simu_gsw_init (void)
{
   if (((exp_file_spec        = (char*)malloc(MAX_STRING * sizeof(char))) != NULL) &&
       ((res_fspec            = (char*)malloc(MAX_STRING * sizeof(char))) != NULL) &&
       ((extSimuModelName     = (char*)malloc(MAX_STRING * sizeof(char))) != NULL) &&
       ((lastSimuModelName    = (char*)malloc(MAX_STRING * sizeof(char))) != NULL) &&
       ((extSimServDomainName = (char*)malloc(MAX_STRING * sizeof(char))) != NULL) &&
       ((lastSimServDomainName= (char*)malloc(MAX_STRING * sizeof(char))) != NULL))
   {
      strcpy (exp_file_spec,        "");
      strcpy (res_fspec,            "");
      strcpy (extSimuModelName,     "unspecified");
      strcpy (lastSimuModelName,    "");  /*  MUST be empty on program start! */
      strcpy (extSimServDomainName, "undefined.iai.kit.edu");
      strcpy (lastSimServDomainName,"");
      extSimuRunLimit = 10000000;  /* i.e., no re-init. of external simulator */
      return (TRUE);
   }
   else
      return (FALSE);
} /* simu_gsw_init */



/* ================================ simuInit ================================ */
BOOL simuInit (BOOL *ext_sim_mod_ok)
{
   INT     ii;
   size_t  csize = sizeof (char);
   BOOL    erg;

   /* simuInit */
   if (((akt_sim_mod_path = (char*)malloc((MAX_STR+1)*csize)) != NULL) && rob_init ())
   { /* Daten und "rob_init" ok */
      reset_krit_gueltig ();                               /* Initialisierung */
      resetSimValOutOfEvalRange ();                        /* Initialisierung */
      int_sim_data_ready = FALSE;
      best_ges_note      = -1;    /* d.h., dass keine Ergebniswerte vorliegen */
      init_mbf_sim ();
      erg = init_ext_koppl (ext_sim_mod_ok);
#ifdef MIT_AEHNL_CHECK
      erg &= initAehnlVgl ();
#endif /* MIT_AEHNL_CHECK */
      return (erg);
   } /* Daten und "rob_init" ok */
   else
      return (FALSE);
} /* simuInit */
