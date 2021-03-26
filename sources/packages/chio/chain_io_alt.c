/*******************************************************************************
GLEAM/AE                                                                  V1.0.5

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: CHain Input/Output
                        Module: CHAIN-Input/Output
 
Package: chio                   File: chain_io.c                                
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Das Unit enthaelt Routinen zum Retten und Restaurieren des Kettenspeichers kom-
plett oder teilweise und von einzelnen Ketten. Letztere jedoch mit der Ein-
schraenkung, dass die Restaurierung einzelner Ketten nur moeglich ist, wenn 
eventuelle AK-Referenzen AKs betreffen, die an gleicher Stelle im AK-Speicher 
stehen, wie bei der Rettung. Vorgesehen ist dieser Fall fuer die zyklische Ret-
tung der Evo-Population bzw. der Initialisierung vom Evo-Scratchfile.

Es gibt drei I/O-Modi, die durch "open_ch_file" eingestellt werden:
Binary-I/O:   Plattformspezifische Files. Schnell und kompakt.
Textual-I/O:  Plattformunabhaengige Files. Langsam und speicherplatzintensiv.
PVM-I/O:      Plattformunabhaengige I/O- der Parallelversion basierend auf PVM.


Definitionen:
=============
Unter-AKs      sind lokale Unter-AKs einer AK, die ohne ihre Haupt-AK ueber kei-
               nerlei eigenstaendige Existenz verfuegen. Eine Unter-AK kann wie-
               derum Unter-AKs enthalten, so dass hierarchische Kettenstrukturen
               ermoeglicht werden.
AK-Referenzen  sind Verweise auf selbststaendig existierende AKs. Diese koennen
               - muessen aber nicht - Basis-AK sein ausser bei PVM-I/O, wo sie 
               Basis-AK sein muessen.
In beiden Faellen erfolgt die Speicherung des Kettenverweises innerhalb einer
speziellen Aktion vom Typ ACTIVITY_ACT.
 
Die Routinen zur Kettenrettung sollen sowohl die Rettung des ganzen Kettenspei-
chers als auch einzelner Ketten (z.B. per Dialog ausgewaehlte AKs oder alle 
Basis-AKs) gestatten. Alle Kettenarten werden im einheitlichen Kettenfile 
"chain_file" gespeichert. Dieses hat je nach Speicherungsart folgende Extension
     ".MEM":  wenn es sich um einen kompletten Speicherabzug handelt, der in 
              einen LEEREN Speicher geladen wird.
     ".CHS":  wenn es sich um einen kompletten Speicherabzug handelt, der in 
              einen evtl. bereits gefuellten Speicher hinzugeladen wird.
     ".AKS":  wenn es sich um einzelne AKs handelt.
 
Diese Konvention dient ausser bei "MEM" nur der besseren Handhabung, denn die 
Routine "restore_chs" kann alle Kettenarten verarbeiten. Ein MEM-File unter-
scheidet sich von den anderen dadurch, dass die schnellere Restaurierung der 
AK-Referenzen mittels AK-Adressen benutzt wird (globale Speicherung, siehe auch 
weiter unten).

Die Rettung der Evo-Population erfolgt als globale Rettung, wobei jedoch nur die
AKs der Population ohne eventuell referenzierte AKs gerettet werden. Beim Re-
staurieren wird daher vorausgesetzt, dass der Kettenspeicher mit dem der Ret-
tung (zumindest was die Adressen der refenzierten AKs betrifft) uebereinstimmt.
Rettung und Restaurierung erfolgen nicht ueber den Kettenspeicher sondern direkt 
vom bzw. in das Populationsfeld mit Hilfe der Routinen "save_chain" einerseits
und "read_chain" in Verbindung mit "restore_ch_ref" andererseits.
 
Das Retten und Restaurieren erfolgt folgendermassen:
   AKs: Segm:    Die Segment-Descriptoren werden gerettet und die Zeiger anhand 
                 der Segmentlaengen restauriert.
        AK-Refs: Im "activity_act"-Item wird bei selektiver Speicherung in 
                 "sub_ak_index" der "ak_index" und in "sub_ak_guete" die "guete"
                 der Unter-AK und bei globaler Speicherung in "sub_ak_guete" und
                 "sub_ak_lfd_nr" die AK-Adresse gespeichert. Beim Restaurieren 
                 werden die Pointer in einem 2.Pass, wenn alle AKs wieder im 
                 Speicher stehen, eingerichtet, indem die AK mit gleichem Index 
                 bzw. gleicher Adresse gesucht wird.
 
Beim selektiven Retten wird der "ak_index" von AKs durch "save_chain" folgender-
massen vergeben: Wenn ein "activity_act"-Item gerettet werden soll, wird ge-
prueft, ob die zugehoerige AK bereits einen "ak_index"-Eintrag im Header hat. 
Wenn ja, wird dieser in "sub_ak_index" im "activity_act"-Item eingetragen. An-
sonsten wird ein neuer vergeben und ebenfalls eingetragen. Ferner wird in 
"sub_ak_guete" die AK-Guete gespeichert. Nach dem Restaurieren muessen diese 
AK-Indices wieder geloescht, d.h. auf 0 gesetzt werden. Bei globaler Rettung 
ist der "ak_index" immer NO_INDEX und es wird ueber die Kettenadresse restau-
riert. D.h., dass die Restaurierung durch die "ak_index"-Angabe gesteuert wird. 
Die Unterscheidung erfolgt deswegen, weil der Zugriff ueber die Kettenadresse 
nur bei globaler Rettung moeglich und schneller als das Suchen nach dem "ak_
index" ist. Gesteuert wird das Ganze ueber die globale Variable "global_save", 
die zusammen mit "ak_ind_ctr" von der Funktion "open_ch_file" initialisiert 
wird.
 
Wenn nur einzelne Ketten gerettet werden, so macht das nur bei AKs Probleme: Es 
muessen naemlich auch die referenzierten AKs mitgerettet werden. Dazu werden 
sie mit dem Bit "to_be_saved" im "chain_ status" im Header markiert. Wenn alles 
markiert ist, beginnt erst die eigentliche Rettung unter Ruecksetzten des Mar-
ker-Bits.

Das Kettenfile besteht aus Records, die entweder ein Kettenelement, ein Header-
Parameter oder ein Segmentdescriptor enthalten:
 
    AK:
    +--------+--------+--------+     +--------+--------+     +--------+
    | Hdr-   | Hdr-   | Segm-  | ... | Segm-  | Chain- | ... | Chain- |
    | Elem   | Params | Descr 1|     | Descr n| Elem 1 |     | Elem n |
    +--------+--------+--------+     +--------+--------+     +--------+
                      \__________  __________/
                                 \/
                              optional
 
Die Anzahl der Segmentdescriptor-Records bei AKs ergibt sich aus der "segment_
anz" im Header. Zum Restaurieren wird der "ak_index" in doppelter Hinsicht ge-
nutzt: Erstens zum Zugriff auf die AK wie oben beschrieben. Zweitens markiert 
er alle geladenen AKs, sodass diese bei einem Fehler identifiziert und geloescht
werden koennen. Dies ist notwendig, da wegen der Sub-AK-Verweise auf AKs alle 
AKs geladen sein muessen oder gar keine. D.h., dass Ladefehler die Loeschung der
geladenen AKs bewirken. Ein Restaurierungsfehler von AK-Verweisen bewirkt die 
Loeschung aller geladenen AKs.

Die Restaurierung von Unter-AKs wird durch folgenden Automaten gesteuert:

             AK-Hdr              AK-Hdr                  AK-Hdr / loc_ch_ctr++
    NOTHING -------> AK_RESTORE -------> LOC_AK_RESTORE ----------------------+  
       ^                |  ^             | ^        | ^                       | 
       |                |  |             | |        | +-----------------------+
       +---------------+   +-------------+ +--------+ 
           AK fertig       AK fertig UND    AK fertig UND loc_ch_ctr > 0 /
                           loc_ch_ctr == 0  loc_ch_ctr--

PVM-I/O:
Bei PVM-I/O werden Ketten zwischen Master und Slave sowie zwischen den Slaves 
uebertragen. Slave und Master koennen im wesentlichen den gleichen "chain_io"-
Code benutzten; die Differenzen regelt der Compilerschalter SLAVE. BAKs stehen 
einerseits wie alle AKs im AKMem und sind andererseits nochmals in der BAK-VK 
aufgelistet. Die dadurch gegebene Reihenfolge (modifizierte Vergabe von "ak_
index"-Werten) wird fuer die Identifizierung bei der Restaurierung der Unter-AK-
Referenzen genutzt. Daher duerfen bei der Pararllelversion waehrend eines Jobs 
BAKs nicht in normale AKs umgewandelt werden. 
Die gesamte PVM-Ketten-I/O erfolgt mit SELECTIVE_SAVE und wird nachfolgend er-
laeutert. Gesteuert durch den eingestellten I/O-Mode interpretiert "restore_ch_
ref" die "ak_index"-Werte von AK-Referenzen als Indices in der BAK-VK.

Init der Slaves:
   Im Ergebnis der Initialisierung der Slaves haben alle Slaves alle BAKS 
   unter der gleichen AK-Adresse und "ak_index"-Wert im AKMem stehen.

   Master (Sender):
   ================
   0. Vorbereitung der I/O durch "open_ch_file". Dabei wird der AK-Index-Counter
      zurueckgesetzt und der IO-Mode eingestellt. 
          open_ch_file ("", 1, CH_WRITE, SELECTIVE_SAVE, MPI2_IO);

   1. Sende BAKs mit "save_baks" als ein Telegramm. Die dabei angelegten
      "ak_index"-Werte bleiben erhalten und entsprechen der Reihenfolge der 
      BAKs in der BAK-VK. Dabei werden keine AKs markiert.
          save_baks ();

   2. Sende der Reihe nach die AKs eines Slaves ("save_chain") mit vorange-
      stelltem PopIndex in jeweils einem Telegramm. "save_chain" vergibt dabei
      neue "ak_index"-Werte (die nicht weiter interessieren).
          save_chain (ak_ptr, FALSE);

   3. Loesche die angelegten "ak_index"-Werte: BAKs per "clr_bak_indices" und
      bei den AKs eines Slaves zu Fuss.
          clr_bak_indices ();
          ak_ptr->u_el.u_xel.u_hdr.ak_index = 0;

   Slave (Empfaenger):
   ===================
   0. Vorbereitung der I/O durch "open_ch_file". Dabei wird der IO-Mode 
      eingestellt.
          open_ch_file ("", 1, CH_READ, SELECTIVE_SAVE, MPI2_IO);

   1. Restauriere alle AKs des ersten Telegramms mit "restore_chs", wobei die
      "ak_index"-Werte erhalten bleiben. -> BAKs stehen im AKMem.
          restore_chs (TRUE, FALSE);

   2. Empfange der Reihe nach die Telegramme und restauriere die darin ent-
      haltene AK mit "read_ch" und "restore_ch_ref". Speichere sie in das 
      PopFeld des Slaves unter PopIndex. -> Diese AKs stehen nicht im AKMem.
      Im chain_status der Kette muss ausserdem das BASIS_AK-Flag 
      zurueckgesetzt werden.
          read_ch (&ak_ptr, CH_START);
          ak_ptr->u_el.u_xel.u_hdr.chain_status &= ~BASIS_AK;
          restore_ch_ref (ak_ptr);
      
   3. Die "ak_index"-Werte der AKs des Slaves werden zu Fuss geloescht.
          ak_ptr->u_el.u_xel.u_hdr.ak_index = 0;

AK-Update und ErgAKs:
   Einzelne AKs werden zum Nachbar-Update an Slaves und als Ergebnis-AKs an 
   den Master verschickt. Per Definitionem koennen sie nur BAKs als referen-
   zierte UnterAKs haben. 

   Slave (Sender):
   ===============
   0. Vorbereitung der I/O durch "open_ch_file". Dabei wird der AK-Index-
      Counter zurueckgesetzt und der IO-Mode eingestellt.
          open_ch_file ("", 1, CH_WRITE, SELECTIVE_SAVE, MPI2_IO);

   1. Sende die AKs des Slaves ("save_chain") mit vorangestelltem PopIndex in 
      einem Telegramm. "save_chain" vergibt dabei neue "ak_index"-Werte (die 
      nicht weiter interessieren).
          save_chain (ak_ptr, FALSE);

   3. Loesche den angelegten "ak_index"-Wert der AK des Slaves zu Fuss.
          ak_ptr->u_el.u_xel.u_hdr.ak_index = 0;

   Slave (Empfaenger):
   ===================
   0. Vorbereitung der I/O durch "open_ch_file". Dabei wird der IO-Mode einge-
      stellt.
          open_ch_file ("", 1, CH_READ, SELECTIVE_SAVE, MPI2_IO);

   1. Empfange das Telegramm und restauriere die darin enthaltene AK mit 
      "read_ch" und "restore_ch_ref". Speichere sie in das PopFeld des Slaves
      unter PopIndex. -> Diese AK steht nicht im AKMem.
          read_ch (&ak_ptr, CH_START);
          restore_ch_ref (ak_ptr);
      
   2. Der "ak_index"-Wert der AK des Slaves wird zu Fuss geloescht. 
          ak_ptr->u_el.u_xel.u_hdr.ak_index = 0;

   Master (Empfaenger):
   ====================
   0. Vorbereitung der I/O durch "open_ch_file". Dabei wird der IO-Mode einge-
      stellt.
          open_ch_file ("", 1, CH_READ, SELECTIVE_SAVE, MPI2_IO);

   1. Empfange das Telegramm und restauriere die darin enthaltene AK mit 
      "read_ch" und "restore_ch_ref". Speichere sie in den AKMem-Speicher und
      die Daten der AK in das "evo_erg"-Feld mit Hilfe von "store_in_evo_erg".
      Der "ak_index"-Wert der empfangenen AK des Slaves wird zu Fuss geloescht.
          read_ch (&ak_ptr, CH_START);
          restore_ch_ref (ak_ptr);
          ak_ptr->u_el.u_xel.u_hdr.ak_index = 0;
          berechne_guete (&guete, ak_ptr->u_el.u_xel.u_hdr.fitness_note);
          store_chain (&lfd_nr, ak_ptr, AK, guete);
          store_in_evo_erg (lfd_nr, guete, 
                            ak_ptr->u_el.u_xel.u_hdr.fitness_note, AK);


Die Routinen im einzelnen:

void   set_log_state (BOOL mit_logging);
          Setzt die interne Variable "chio_logging", die die Protokollierung 
          der Kettenfile-I/O kontrolliert, auf "mit_logging".

BOOL   open_ch_file (const char *fspec,
                     INT         start_znr,
                     BOOL        lesen,
                     BOOL        glo_sav,
                     INT         io_mode);
          Vorbereitung zu Ketten-I/O entsprechend "io_mode":
          io_mode == FILE_BIN_IO, FILE_TXT_IO:
                  Das durch "fspec" (komplette File-Spezifikation) bezeichnete 
                  Kettenfile "chain_file" wird entsprechend "lesen" geoeffnet. 
                  Protokollausgaben werden ab "start_znr" in den Textpuffer von
                  "fbhm" geschrieben. Diese Zeilennummer wird bei beiden I/O-Modi
                  auch von "save_marked_aks", "save_ch_mem" und "restore_chs" 
                  zu Grunde gelegt.
          io_mode == MPI2_IO:
                  Die Ketten-I/O wird entsprechend "lesen" vorbereitet.
          Wenn alles gut ging, wird TRUE abgeliefert, sonst FALSE. 
          "glo_sav" hat nur beim Oeffnen zum Schreiben Bedeutung und unterschei-
          det zwischen globaler und selektiver Rettung.

void   close_ch_file (void);
          Schliesst das Kettenfile.
 
GSTATE markiere_ak (INT  guete,
                    INT  lfd_nr);
          Markiert die mit "guete" und "lfd_nr" angegebene AK und alle referen-
          zierten AKs durch Setzen des Bits "to_be_saved" im "chain_status" im 
          AK-Header. Wenn die AK nicht verfuegbar ist, liegt ein Fehler vor, der
          gemeldet und als Funktionsergebnis abgeliefert wird.

GSTATE markiere_aks (INT  guete);
          Alle Aks der angegebenen "guete" werden zum Retten markiert. Dabei 
          auftretende Fehler werden im Funktionsergebnis zurueckgeliefert.
 
BOOL   save_marked_aks (BOOL  als_bak);
          Rettet alle markierten AKs mit Hilfe von "save_chain" in das Ketten-
          file "chain_file". Zuvor wird das Bit "to_be_saved" im "chain_status"
          des Kettenkopfes zurueckgesetzt. Nach der Rettung werden alle "ak_
          index"-Eintraege in den AKs auf null gesetzt. "als_bak" wird an "save_
          chain" weitergereicht. Im Textpuffer von "fbhm" wird die Anzahl gerette-
          ter AKs pro Guete und deren Gesamtzahl samt dem Rettungsergebnis hinter-
          lassen, sofern "MPI_PAR_VERS" nicht gesetzt ist. Die Gesamtzahl mit dem 
          Rettungsergebnis wird bei gesetztem "chio_logging" auch ins LOG-File 
          geschrieben. Im (fatalen) Fehlerfalle bricht die Routine ab und liefert 
          FALSE ab, sonst TRUE.

void   clear_save_marks (void);
          Setzt in allen AKs des Kettenspeichers das Bit "to_be_saved" im 
          "chain_status" des Kettenkopfes zurueck.

BOOL   save_chain (CHAIN_ELEM_PTR_TYPE  ch_ptr, 
                   BOOL                 als_bak);
          Die durch "ch_ptr" bezeichntete Kette wird samt eventuellen Substruk-
          turen wie oben beschrieben in das Kettenfile "chain_file" gerettet.
          Wenn "als_bak" gesetzt ist, wird die AK (sonst fataler Fehler) als Ba-
          sis-AK markiert gerettet. Basis-AKs werden immer als solche gerettet. 
          Im Fehlerfalle bricht die Routine ab und liefert FALSE ab, sonst TRUE.

BOOL   save_ch_mem (void);
          Der ganze Kettenspeicher wird unter Benutzung von "save_chain" geret-
          tet (globales Retten). Im Textpuffer von "fbhm" wird die Anzahl gerette-
          ter AKs pro Guete und deren Gesamtzahl samt dem Rettungsergebnis hinter-
          lassen, sofern "MPI_PAR_VERS" nicht gesetzt ist. Die Gesamtzahl mit dem 
          Rettungsergebnis wird bei gesetztem "chio_logging" auch ins LOG-File 
          geschrieben. Im Fehlerfalle bricht die Routine ab und liefert FALSE ab, 
          sonst TRUE.

BOOL   save_baks (void);
          Rettet alle BAKs, wobei vorausgesetzt wird, dass "open_ch_file" zuvor
          mit SLECTIVE_SAVE aufgerufen wurde. 
          Die Routine ist fuer PVM-I/O vorgesehen und arbeitet ohne AK-Markie-
          rung. Daher muessen alle AK-Referenzen BAKs sein!

void   clr_bak_indices (void);
          Loescht alle "ak_index"-Eintraege in den BAK-Koepfen.
          Die Routine ist fuer PVM-I/O vorgesehen.
 
BOOL   restore_chs (BOOL  basis_aks, 
                    BOOL  anzeige);
          Alle Ketten im Kettenfile "chain_file" werden eingelesen u. im Ketten-
          speicher gespeichert. Wenn dazu nicht mehr genuegend Speicherplatz 
          vorhanden ist oder anderweitig ein Fehler auftritt, wird das Einlesen 
          abgebrochen und eine Fehlermeldung ausgegeben. Wenn "basis_ak" gesetzt
          ist, werden alle AKs mit gesetztem Bit "basis_ak" im "chain_status" 
          des AK-Headers auch als Basis-AK gespeichert. Andernfalls werden etwa-
          ige gesetzte Bits BASIS_AK zurueckgesetzt. Bei jeder Kette wird der
          Kopf, die Kettenstruktur und das Kettenende geprueft. Im Fehlerfalle 
          wird nach einer Fehlermeldung das Einlesen ebenfalls abgebrochen. Bei
          segmentierten AKs wird die Segmentierung der AK restauriert. 
          Nach erfolgreichem Abschluss des Einlesens werden bei AKs alle Sub-AK-
          Referenzen gesucht u. restauriert. Nach erfolgter Restaurierung werden
          alle "ak_index"-Eintraege der AKs auf 0 gesetzt. Wenn alles gut ging, 
          liefert die Funktion TRUE.
          Im Fehlerfalle werden wie oben beschrieben alle AKs mit einem "ak_in-
          dex"-Eintrag <> 0 geloescht, um einen konsistenten Zustand wiederher-
          zustellen. Die Funktion liefert dann FALSE.
          Bei nicht gesetztem "MPI_PAR_VERS" und gesetztem Parameter "anzeige" 
          werden die Adressen, unter denen die restaurierten Ketten abgespeichert 
          wurden, im Textpuffer ("fbhm") angezeigt.

BOOL   ch_file_ok (void);
          Prueft, ob das File offen ist und auf "feof". Liefert TRUE, wenn das 
          File offen und EOF noch nicht erreicht wurde.

BOOL   read_chain (CHAIN_ELEM_PTR_TYPE *ch_ptr,
                     BOOL                 reset_state);
          Die Funktion liefert nach erfolgreichem Einlesen einer Kette TRUE und 
          in "ch_ptr" die Kette ab, sonst FALSE und einen leeren Zeiger. Bei 
          gesetztem "reset_state" wird davon ausgegangen, dass die Restaurierung
          einer eigenstaendigen Kette beginnt und daher der interne Restaurie-
          rungsstatus zurueckzusetzen ist.
          Die Routine ueberprueft die Kettenlaenge und -struktur, die Anzahl 
          eventueller Segmentdescriptoren sowie die Zulaessigkeit von AK-Refe-
          renzen und lokalen Unterketten. Bei einem Fehler wird die bereits 
          angelegte Teilkette vernichtet. 
 
BOOL   restore_ch_ref (CHAIN_ELEM_PTR_TYPE  ch_ptr);
          Stellt die Sub-AK-Verweise auf globale AKs wieder her. Wenn ein Ver-
          weis nicht restauriert werden kann, d.h. dass die referenzierte Kette
          nicht gefunden wurde, bricht die Funktion ab und liefert FALSE, sonst
          TRUE. Bei Fehlerfreiheit und gesetztem "global_save" wird auch der 
          "ak_index"-Eintrag im Kettenkopf geloescht.

BOOL   chain_io_init (BOOL  mit_logging);
          Etabliert den Exithandler u. setzt die lokale Variable "chio_logging",
          die die Log-File-I/O-Prokollierung steuert, auf den Wert von "mit_
          logging".
          In der MPI-Version wird "initMPI_Chio" aufgerufen und dessen Returnwert 
          zurueckgeliefert. Andernfalls ist das Ergebnis immer TRUE.

--------------------------------------------------------------------------------
basiert auf C-Unit: chain_io.
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. FileSpecParameter von "open_ch_file" wird *char statt Puffer.    WJ 14.08.07
 2. Bei nicht gesetztem GORBA_KP_BEW: Simulation restaurierte und 
    unsimulierter AKs in "restore_chs".                              WJ 16.08.07
 3. Anpassung an neue Datentypen fuer Int- und Real-Parameter
    (neues "chaindef.h" fuer 64 bit)                                 WJ 22.01.08
 4. Korrektur einiger Fehlermeldungsaufrufe wg. Umstellung in fbhm.  WJ 13.08.08
 5. Abschlussmeldung der Restauration mit Angabe der besten Note.    WJ 18.12.08
 6. "open_ch_file": Deklaration von "fspec", als "const".            WJ 20.04.12
 7. Entfernung ueberfluessiger Variable. Initialisierung von 
    uninitialisierten Variablen.                                     WJ 26.06.12
 8. "restore_chs": Bei Fehlerabbruch wird "ret_state" zurueckgesetzt.WJ 11.09.12
 9. Anpassungen an die MPI-Parallelversion. Neuer Kopfeintrag 
    "parIndIdx", der den "indIdx" der Individuums enthaelt.          WJ 18.10.12
    Package "pcom" geht in "mPar" auf.                               WJ 09.11.12
10. Renovierung: Streichung des "link"-Includes.                     WJ 18.04.16
11. Anpassung des Aufrufs von "write_to_log".                        WJ 19.12.16
12. "restore_chs" kann jetzt auch unbewertete AKs mit dem externen 
    Simulator bewerten.                                              WJ 16.03.17
13. "open_ch_file" oeffnet bei nicht gesetztem GLEAM_USR fuer CLVs
    eine zu schreibende Kettendatei im "append"-Modus. Die Laenge 
    von "buffer" in "open_ch_file" wird auf MAX_STRING erhoeht.      WJ 05.02.18
14. "restore_chs": Alle restaurierten Ketten, die unsimuliert sind, 
    werden bewertet und nicht nur die erste. Export der neuen Varia-
    ble "initialChrFileMode".                                        WJ 18.04.18
15. write_elem(): schreibt bei FILE_TXT_IO eine "0" bei entsprechen-
    den reellen Werten.                                              WJ 15.06.18
16. WarnungKeine Restaurierung unbewerteter Chromosome, wenn mehr 
    Bewertungskriterien als Simulationsergebnisse spezifiziert 
    wurden.                                                          WJ 14.02.19
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 14.02.2019
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "lgsw.h"
#include "hmod.h"
#include "bew.h"
#ifdef MPI_PAR_VERS
  #include "mPar.h"
  #include "chioInt.h"
#endif /* MPI_PAR_VERS */
#ifndef GORBA_KP_BEW
  #include "simu.h"
#endif /* kein GORBA_KP_BEW */
#include "chio.h"

#include CHIO_TEXTS
#include GLOBAL_TEXTS


#ifdef GLO_CHECKS
  #define CHECKS
#endif



/* ========================== Exportierte Variable: ========================= */
INT  initialChrFileMode;      /* Fileformat der initialen Chromosomendatei    */



/* ============================ Lokale Konstante: =========================== */
#define NO_INDEX     -9999    /* Restaurierung erfolgt nicht ueber "ak_index" */
#define NULL_KENNUNG 10000    /* Kennung eines NULL-Pointers                  */
#define PTR_KENNUNG  12345    /* Kennung eines von NULL verschiedenen Pointers*/
#define NULL_PTR_IND 0.543    /* NULL-Pointer-Indikator fuer die MPI-Version  */
#define ELEM_REC     TRUE
#define AK_HDR_REC   FALSE



/* ======================== Lokale Typdeklarationen: ======================== */
typedef enum 
  { CHAIN_ITEM, DESCR_ITEM
  } SAVE_CAST_ELEM_TYPE;

typedef union 
  { CHAIN_ELEM_TYPE  chain_elem;
    REF_DESCR_TYPE   descr_elem;
  } SAVE_CAST_TYPE;

typedef enum 
  { NOTHING, AK_RESTORE, LOC_AK_RESTORE
  } CH_STATE_TYPE;



/* ============================ Lokale Variable: ============================ */
static FILE     *chain_file;     /* Kettenfile                                */
static char      offs_buf [3];   /* Offsetbuffer fuer Protokollierung.        */
static INT       ak_ind_ctr,     /* "ak_index"-Counter                        */
                 znr;            /* Zeilen-Nr fuer Ausgabe in den Textpuffer  */
static INT       ch_io_mode;     /* Chain-I/O-Mode (FILE_BIN/TXT_IO, MPI2_IO) */
static BOOL      chainfile_open, /* TRUE, wenn das "chain_file" offen ist     */
                 global_save,    /* Rettung: TRUE = global,  FALSE = selektiv */
                 chio_logging,   /* I/O-Protokollierung ins Log-File          */
                 save_ok,        /* TRUE, solange kein Fehler bei Kettenrettg.*/
                 stnd_io;        /* TRUE, solange kein PVM-I/O                */

static CH_STATE_TYPE   ch_state = NOTHING;  /* fuer "restore_chs"             */
static BOOL            eof_found;           /* fuer "restore_chs"             */
static int             loc_ch_ctr;          /* fuer "read_chain"              */
static BOOL            ch_ok;               /* fuer "read_chain"              */



/* ============================ Lokale Routinen: ============================ */
static BOOL   write_elem            (CHAIN_ELEM_PTR_TYPE  elem_ptr,
                                     INT                  item_nr,
                                     BOOL                 elem_rec);

static BOOL   write_descr           (REF_DESCR_PTR_TYPE   descr_ptr,
                                     INT                  item_nr);

static GSTATE mark_ak               (CHAIN_ELEM_PTR_TYPE  ak_ptr);

static INT    save_marked_chs_of_vk (CHAIN_ELEM_PTR_TYPE  vk_ptr,
                                     BOOL                 als_bak);

static INT    save_vk_contents      (CHAIN_ELEM_PTR_TYPE  vk_ptr);

static BOOL   read_elem             (CHAIN_ELEM_PTR_TYPE  elem_ptr,
                                     INT                  item_nr,
                                     BOOL                 elem_rec);

static BOOL   read_descr            (REF_DESCR_PTR_TYPE   descr_ptr,
                                     INT                  item_nr);

static VOID   state_to_txt          (STR_TYPE             buf);

static VOID   fmeld                 (INT                  fnr,
                                     LONG                 info,
                                     INT                  ident);
  
static BOOL   search_ak             (CHAIN_ELEM_PTR_TYPE *ak_ptr, 
                                     INT                  ak_ind, 
                                     INT                  guete, 
                                     INT                  lfd_nr);

static INT    del_restored_chs      (CHAIN_ELEM_PTR_TYPE  vk_ptr,
                                     GSTATE              *ret_state);

static VOID   chain_io_exit         (VOID);





/* ============================== set_log_state ============================= */
void set_log_state (BOOL mit_logging)
{
   chio_logging = mit_logging;
} /* set_log_state */



/* ============================== open_ch_file ============================== */
BOOL open_ch_file (const char *fspec,
                   INT         start_znr,
                   BOOL        lesen,
                   BOOL        glo_sav,
                   INT         io_mode)
{ 
   STRING_TYPE  buffer;
   BOOL         ready_for_io;

   /* open_ch_file */
   stnd_io = io_mode != MPI2_IO;
#ifdef MPI_PAR_VERS
   if (master)
#endif /* MPI_PAR_VERS */
      if (stnd_io)
      { /* Sequentielle Versionen ODER MPI-Master bei File-I/O  */
	 znr = start_znr;            /* Init der Zeilennummer fuer I/O-Protokoll */
	 strcpy (offs_buf, "  ");
	 if (znr == 1) {
	    reset_text_buf ();
	    strcpy (offs_buf, "");
	 }
	 if (lesen)
	    sprintf (buffer, CHF_IN_TXT, offs_buf, fspec);
	 else
	    sprintf (buffer, CHF_OUT_TXT, offs_buf, fspec);
	 write_buf (znr++, buffer);
      } /* Sequentielle Versionen ODER MPI-Master bei File-I/O */
   switch (io_mode)
   {
      case FILE_BIN_IO:
         if (lesen) 
            chain_file = fopen (fspec, "rb");
         else 
            chain_file = fopen (fspec, "wb");
      break; /* FILE_BIN_IO */

      case FILE_TXT_IO:
         if (lesen) 
            chain_file = fopen (fspec, "rt");
         else 
#if defined(GLEAM_USR) || defined(PAR_POP_VERS)
            chain_file = fopen (fspec, "wt");
#else /* weder GLEAM_USR noch PAR_POP_VERS */
            chain_file = fopen (fspec, "at");
#endif /* weder GLEAM_USR noch PAR_POP_VERS */
      break; /* FILE_BIN_IO */

      case MPI2_IO:
#ifdef MPI_PAR_VERS
	 akPufferIdx  = 0;
	 ready_for_io = TRUE;
#else /* keine MPI_PAR_VERS */
	 ready_for_io = FALSE;
#endif /* keine MPI_PAR_VERS */
      break; /* MPI2_IO */
   } /* case */

   if (io_mode != MPI2_IO)
   { /* File-I/O */
      if (chain_file != NULL) 
         chainfile_open = TRUE;
      else 
      { /* mist file */
         if (strlen (fspec) == 0)
            strcpy (buffer, " ");                    /* never an empty string */
         else
            strcpy (buffer, fspec);
         chainfile_open = FALSE;
         fehler (CHIO_CHAIN_IO, 1, NO_INFO, NO_INFO, buffer, FILE_NOT_EXISTS);
      } /* mist file */
      ready_for_io = chainfile_open;
   } /* File-I/O */
   ak_ind_ctr  = 1;    /* Initialisierung f.eine evtl. Vergabe von AK-Indizes */
   global_save = glo_sav;
   ch_io_mode  = io_mode;
   eof_found   = FALSE;
   ch_state    = NOTHING;
   loc_ch_ctr  = 0;
   if (chio_logging)
   { /* Log-Protokollierung */
      sprintf (buffer, CHF_OPEN_TXT, (io_mode == MPI2_IO)?"MPI-Com":fspec,
               (io_mode == MPI2_IO)?ready_for_io:chainfile_open, lesen, global_save);
      write_to_log (buffer, CLOSE_LOG);
   } /* Log-Protokollierung */
   return (ready_for_io);
} /* open_ch_file */



/* ============================== close_ch_file ============================= */
void close_ch_file (void)
{ 
   if (chainfile_open) 
      fclose (chain_file);
   chainfile_open = FALSE;
} /* close_ch_file */



/* =============================== write_elem =============================== */
static BOOL write_elem (CHAIN_ELEM_PTR_TYPE  elem_ptr,
                        INT                  item_nr,
                        BOOL                 elem_rec)
/*----------------------------------------------------------------------------*/
/*  Liefert TRUE, wenn das Element, auf das "elem_ptr" zeigt, erfolgreich ge- */
/*  schrieben werden konnte. Sonst FALSE u. eine Fehlermeldung unter Verwen-  */
/*  dung der "item_nr" (==0:Kopf, ==-1:HdrParam, >0:Elem).                    */
/*----------------------------------------------------------------------------*/
{ 
   SAVE_CAST_TYPE  save_rec;
   INT             ai, ii;
   BOOL            ok = FALSE;

   /* write_elem */
   switch (ch_io_mode)
   {
      case FILE_BIN_IO:
         save_rec.chain_elem = *elem_ptr;
         ok = (fwrite (&save_rec, sizeof(SAVE_CAST_TYPE), 1, chain_file) == 1);
      break; /* FILE_BIN_IO */

      case FILE_TXT_IO:
         if (elem_rec)
         { /* Elem */
            ok = fprintf (chain_file, "%5hd  ", elem_ptr->u_el.activity) > 0;
            switch (elem_ptr->u_el.activity)
            {
               case AK_HEADER:
                  ok = fprintf (chain_file, 
                                "%.12le %hd %hd %hd %hd %hu %hd %hd", 
                                elem_ptr->u_el.u_xel.u_hdr.fitness_note,
                                elem_ptr->u_el.u_xel.u_hdr.ref_counter,
                                elem_ptr->u_el.u_xel.u_hdr.chain_length,
                                elem_ptr->u_el.u_xel.u_hdr.guete,
                                elem_ptr->u_el.u_xel.u_hdr.lfd_nr,
                                elem_ptr->u_el.u_xel.u_hdr.chain_status,
                                elem_ptr->u_el.u_xel.u_hdr.segment_anz,
                                elem_ptr->u_el.u_xel.u_hdr.ak_index) > 0;
               break; /* AK_HEADER */

               case ACTIVITY_ACT:
                  ok = fprintf (chain_file, "%hd %hd %hd %hd", 
                                elem_ptr->u_el.u_xel.u_sub.ref_typ,
                                elem_ptr->u_el.u_xel.u_sub.sub_ak_guete,
                                elem_ptr->u_el.u_xel.u_sub.sub_ak_lfd_nr,
                                elem_ptr->u_el.u_xel.u_sub.sub_ak_index) > 0;
               break; /* ACTIVITY_ACT */

               default: /* Aktion */
                  for (ii = 0;  (ii < I_PAR_ANZ_MAX) && ok;  ii++)
                     ok = fprintf (chain_file, "%d ", elem_ptr->u_el.u_xel.u_act.i_params[ii]) > 0;
                  if (ok)
                     ok = fprintf (chain_file, "  ") > 0;
                  for (ii = 0;  (ii < R_PAR_ANZ_MAX) && ok;  ii++)
		     if (elem_ptr->u_el.u_xel.u_act.r_params[ii] == 0)
			ok = fprintf (chain_file, "0 ") > 0;
		     else
			ok = fprintf (chain_file, "%.12le ", 
				   elem_ptr->u_el.u_xel.u_act.r_params[ii]) > 0;
            } /* CASE "activity" */
            if (ok)
            {
               if (elem_ptr->u_el.next_action == NULL)
                  ii = NULL_KENNUNG;
               else
                  ii = PTR_KENNUNG;
               ok = fprintf (chain_file, "  %d\n", ii) > 0;
            }
         } /* Elem */
         else
         { /* AK-Header-Param */
            ok = TRUE;
            for (ii = 0;  (ii < AK_ERG_WERTE_MAX) && ok;  ii++)
	       if (elem_ptr->sim_erg_werte[ii] == 0)
		  ok = fprintf(chain_file, "0 ") > 0;
	       else
		  ok = fprintf(chain_file, "%.12le ",elem_ptr->sim_erg_werte[ii]) > 0;
            if (ok)
               ok = fprintf (chain_file, "\n") > 0;
         } /* AK-Header-Param */
      break; /* FILE_TXT_IO */

      case MPI2_IO:
#ifdef MPI_PAR_VERS
#ifdef CHECKS
	 if (akPufferIdx >= akPufferLen) {
	    fatal (CHIO_CHAIN_IO, 300, NO_INFO, NO_INFO, "", NO_MEM_ERR);
	    return (FALSE);
	 }
#endif /* CHECKS */
         if (elem_rec)
         { /* Elem */
	    ai= elem_ptr->u_el.activity;
            akPuffer[akPufferIdx] = ai;
	    if (elem_ptr->u_el.next_action == NULL)
	       akPuffer[akPufferIdx] += NULL_PTR_IND;
	    akPufferIdx++;
            switch (ai)
            {
	       case AK_HEADER:
		  akPuffer[akPufferIdx++] = elem_ptr->u_el.u_xel.u_hdr.fitness_note;
		  akPuffer[akPufferIdx++] = elem_ptr->u_el.u_xel.u_hdr.ref_counter;
		  akPuffer[akPufferIdx++] = elem_ptr->u_el.u_xel.u_hdr.chain_length;
		  akPuffer[akPufferIdx++] = elem_ptr->u_el.u_xel.u_hdr.guete;
		  akPuffer[akPufferIdx++] = elem_ptr->u_el.u_xel.u_hdr.lfd_nr;
		  akPuffer[akPufferIdx++] = elem_ptr->u_el.u_xel.u_hdr.chain_status;
		  akPuffer[akPufferIdx++] = elem_ptr->u_el.u_xel.u_hdr.segment_anz;
		  akPuffer[akPufferIdx++] = elem_ptr->u_el.u_xel.u_hdr.ak_index;
		  akPuffer[akPufferIdx++] = elem_ptr->u_el.u_xel.u_hdr.parIndIdx;
	       break; /* AK_HEADER */

	       case ACTIVITY_ACT:
		  akPuffer[akPufferIdx++] = elem_ptr->u_el.u_xel.u_sub.ref_typ;
		  akPuffer[akPufferIdx++] = elem_ptr->u_el.u_xel.u_sub.sub_ak_guete;
		  akPuffer[akPufferIdx++] = elem_ptr->u_el.u_xel.u_sub.sub_ak_lfd_nr;
		  akPuffer[akPufferIdx++] = elem_ptr->u_el.u_xel.u_sub.sub_ak_index;
               break; /* ACTIVITY_ACT */

               default: /* Aktion */
		  for (ii = 0;  ii < int_pro_aktion[ai];  ii++)
		     akPuffer[akPufferIdx++] = elem_ptr->u_el.u_xel.u_act.i_params[ii];
		  memcpy (&(akPuffer[akPufferIdx]), elem_ptr->u_el.u_xel.u_act.r_params,
			  real_pro_aktion[ai] * sizeof(DOUBLE));
		  akPufferIdx += real_pro_aktion[ai];
            } /* CASE "activity" */
	 } /* Elem */
         else
         { /* AK-Header-Param */
	    memcpy (&(akPuffer[akPufferIdx]), &(elem_ptr->sim_erg_werte), 
		    AK_ERG_WERTE_MAX * sizeof(DOUBLE));
	    akPufferIdx += AK_ERG_WERTE_MAX;
         } /* AK-Header-Param */
	 ok = akPufferIdx <= akPufferLen;
#endif /* MPI_PAR_VERS */
     break; /* MPI2_IO */
   } /* CASE "ch_io_mode" */

   if (!ok) {
      if (ch_io_mode == MPI2_IO)
#ifdef MPI_PAR_VERS
         fehler (CHIO_CHAIN_IO, 51, item_nr, NO_INFO, "", CHIO_SEND_FAIL);
#else
         fehler (CHIO_CHAIN_IO, 51, item_nr, NO_INFO, "", CH_FILE_IO_ERR);
#endif /* MPI_PAR_VERS */
      else
         fehler (CHIO_CHAIN_IO, 52, item_nr, NO_INFO, "", CH_FILE_IO_ERR);
   } /* end if (!ok) */
   return (ok);
} /* write_elem */



/* =============================== write_descr ============================== */
static BOOL write_descr (REF_DESCR_PTR_TYPE  descr_ptr,
                         INT                 item_nr)
/*----------------------------------------------------------------------------*/
/*  Liefert TRUE, wenn das Element, auf das "descr_ptr" zeigt, erfolgreich    */
/*  geschrieben werden konnte. Sonst FALSE u. eine Fehlermeldung unter Ver-   */
/*  wendung der "item_nr" (<-1:Descr).                                        */
/*----------------------------------------------------------------------------*/
{ 
   SAVE_CAST_TYPE  save_rec;
   INT             ii;
   SHORT           wert;
   BOOL            ok = FALSE;

   /* write_descr */
   switch (ch_io_mode)
   {
      case FILE_BIN_IO:
         save_rec.descr_elem = *descr_ptr;
         ok = (fwrite (&save_rec, sizeof(SAVE_CAST_TYPE), 1, chain_file) == 1);
      break; /* FILE_BIN_IO */

      case FILE_TXT_IO:
         ok = TRUE;
         for (ii = 0;  (ii < SEGM_PTR_ANZ) && ok;  ii++)
            ok = fprintf (chain_file, "%hd ",
                          descr_ptr->segm_length[ii]) > 0;
         if (ok)
            ok = fprintf (chain_file, "\n");
      break; /* FILE_TXT_IO */

      case MPI2_IO:
#ifdef MPI_PAR_VERS
         for (ii = 0;  ii < SEGM_PTR_ANZ;  ii++) 
            akPuffer[akPufferIdx++] = descr_ptr->segm_length[ii];
	 ok = akPufferIdx <= akPufferLen;
#endif /* MPI_PAR_VERS */
      break; /* MPI2_IO */
   } /* CASE "ch_io_mode" */

   if (!ok) {
      if (ch_io_mode == MPI2_IO) {
#ifdef MPI_PAR_VERS
         fehler (CHIO_CHAIN_IO, 53, item_nr, NO_INFO, "", CHIO_SEND_FAIL);
#else
         fehler (CHIO_CHAIN_IO, 53, item_nr, NO_INFO, "", CH_FILE_IO_ERR);
#endif /* MPI_PAR_VERS */
      } else
         fehler (CHIO_CHAIN_IO, 54, item_nr, NO_INFO, "", CH_FILE_IO_ERR);
   } /* end if (!ok) */
   return (ok);
} /* write_descr */



/* ============================== mark_ak =================================== */
static GSTATE mark_ak (CHAIN_ELEM_PTR_TYPE  ak_ptr)
/*----------------------------------------------------------------------------*/
/*   Markiert die mit "ak_ptr" angegebene AK mit allen Unter-AKs durch Setzen */
/*   des Bits "to_be_saved" im "chain_status" im AK-Header. Wenn "ak_ptr"     */
/*   nicht auf eine AK zeigt, liegt ein fataler Fehler vor.                   */
/*----------------------------------------------------------------------------*/
{
   CHAIN_ELEM_PTR_TYPE  ptr;
   GSTATE               ret_state = GLEAM_OK;
 
   /* mark_ak */ 
#ifdef CHECKS
   if (ak_ptr == NULL) 
      return (fatal (CHIO_CHAIN_IO, 4, NO_INFO, NO_INFO, "", NIL_PTR));
   if (ak_ptr->u_el.activity != AK_HEADER) 
      return (fatal (CHIO_CHAIN_IO, 5, ak_ptr->u_el.activity, NO_INFO, 
                     "", BAD_CHAIN_KIND));
#endif /* CHECKS */
   ak_ptr->u_el.u_xel.u_hdr.chain_status |= TO_BE_SAVED;
   ptr = ak_ptr->u_el.next_action;
   while (ptr != NULL) 
   { /* all along the chain */
      if (ptr->u_el.activity == ACTIVITY_ACT) 
         if (ptr->u_el.u_xel.u_sub.sub_chain != NULL)
            ret_state |= mark_ak (ptr->u_el.u_xel.u_sub.sub_chain);
      ptr = ptr->u_el.next_action;
   } /* all along the chain */
   return (ret_state);
} /* mark_ak */



/* =============================== markiere_ak ============================== */
GSTATE markiere_ak (INT  guete,
                    INT  lfd_nr)
{
   CHAIN_ELEM_PTR_TYPE  ak_ptr;
   STR_TYPE             buf;
   GSTATE               ret_state = GLEAM_OK;

   /* markiere_ak */
   if (get_chain (&ak_ptr, &lfd_nr, AK, guete, &guete, &ret_state))
      ret_state = mark_ak (ak_ptr);                                  /* AK ok */
   else
   { /* AK nicht verfuegbar */
      adr_to_text (buf, guete, lfd_nr, NO_ADJUST);
      ret_state = fehler (CHIO_CHAIN_IO, 6, NO_INFO, NO_INFO, buf, NO_CHAIN);
   } /* AK nicht verfuegbar */
   return (ret_state);
} /* markiere_ak */



/* ============================== markiere_aks ============================== */
GSTATE markiere_aks (INT  guete)
{
   CHAIN_ELEM_PTR_TYPE  ptr;
   GSTATE               ret_state = GLEAM_OK;

   /* markiere_aks */
   ptr = get_vk (AK, guete, &ret_state)->u_el.next_action;
   while ((ptr != NULL)  && (ret_state == GLEAM_OK))
   { /* VK abarbeiten */
      ret_state = mark_ak (ptr->u_el.u_xel.u_sub.sub_chain);
      ptr = ptr->u_el.next_action;
   } /* VK abarbeiten */
   return (ret_state);
} /* markiere_aks */



/* ============================== save_marked_aks =========================== */
BOOL save_marked_aks (BOOL  als_bak)
{
   CHAIN_ELEM_PTR_TYPE  ptr;
   STR_TYPE             buffer;
   INT                  ak_i, ak_sum, gi;
   GSTATE               ret_state = GLEAM_OK;
 
   /* save_marked_aks */ 
   ak_i    = 0;
   ak_sum  = 0;
   save_ok = TRUE;
   for (gi = 1;  gi <= MAX_GUETE;  gi++) 
      if (save_ok) 
      { /* Rettung ok */
         ak_i = save_marked_chs_of_vk (get_vk (AK, gi, &ret_state), als_bak);
         if (ak_i > 0)
         { /* Meldung */
#ifndef SLAVE
            if (stnd_io)
            { /* Nur der Master bei File-I/O */
               sprintf (buffer, SAVED_AKS_TXT, offs_buf, ak_i, gi);
               write_buf (znr++, buffer);
            } /* Nur der Master bei File-I/O */
#endif /* Kein Slave */
            ak_sum += ak_i;
         } /* Meldung */
      } /* Rettung ok */
   if (save_ok)
      sprintf (buffer, SAVED_OK_TXT, offs_buf, ak_sum);
   else
      sprintf (buffer, BAD_SAVE_TXT, offs_buf, ak_sum);
#ifndef SLAVE
   if (stnd_io)
      write_buf (znr, buffer);                 /* Nur der Master bei File-I/O */
#endif
   if (chio_logging)
      write_to_log (buffer, CLOSE_LOG);

   /* AK-Indices in AKs auf Null setzen: */
   for (gi = 0;  gi <= MAX_GUETE;  gi++) 
   { /* In allen AKs "ak_index" auf null setzen */
      ptr = get_vk (AK, gi, &ret_state)->u_el.next_action;
      while (ptr != NULL) 
      { /* VK abarbeiten */
         ptr->u_el.u_xel.u_sub.sub_chain->u_el.u_xel.u_hdr.ak_index = 0;
         ptr = ptr->u_el.next_action;
      } /* VK abarbeiten */
   } /* In allen AKs "ak_index" auf null setzen */
   return (save_ok);
} /* save_marked_aks */



/* ============================ clear_save_marks ============================ */
void clear_save_marks (void)
{
   CHAIN_ELEM_PTR_TYPE  ptr;
   INT                  gi;
   GSTATE               ret_state = GLEAM_OK;

   /* clear_save_marks */
   for (gi = 1;  gi <= MAX_GUETE;  gi++)
   { /* Alle Gueteklassen */
      ptr = get_vk (AK, gi, &ret_state)->u_el.next_action;
      while ((ptr != NULL)  && (ret_state == GLEAM_OK))
      { /* VK abarbeiten */
         ptr->u_el.u_xel.u_sub.sub_chain->u_el.u_xel.u_hdr.chain_status &= 
             ~TO_BE_SAVED;
         ptr = ptr->u_el.next_action;
      } /* VK abarbeiten */
   } /* Alle Gueteklassen */
} /* clear_save_marks */



/* ========================= save_marked_chs_of_vk ========================== */
static INT save_marked_chs_of_vk (CHAIN_ELEM_PTR_TYPE  vk_ptr,
                                  BOOL                 als_bak)
/*----------------------------------------------------------------------------*/
/*  Rettet die an der durch "vk_ptr" bezeichneten Verwaltungskette haengenden */
/*  AKs als  Basis-AKs, wenn "als_bak" gesetzt ist. Liefert die Anzahl ge-    */
/*  retteter Ketten als Funktionsergebnis ab. Bei (fatalen) Fehlern wird      */
/*  "save_ok" auf FALSE gesetzt.                                              */
/*----------------------------------------------------------------------------*/
{
   CHAIN_ELEM_PTR_TYPE  ptr;
   INT                  ctr;
  
   /* save_marked_chs_of_vk */
   ctr = 0;
   ptr = vk_ptr->u_el.next_action;
   while ((ptr != NULL) && save_ok) 
   { /* all along the VK */
#ifdef CHECKS
      if (ptr->u_el.activity != ACTIVITY_ACT) 
         save_ok = fatal (CHIO_CHAIN_IO, 7, ptr->u_el.activity, NO_INFO, "", 
                           NO_ACTIVITY_ACT) == GLEAM_OK;
      if (ptr->u_el.u_xel.u_sub.sub_chain == NULL)
         save_ok = fatal (CHIO_CHAIN_IO, 8, NO_INFO, NO_INFO, "", NIL_PTR) == 
                   GLEAM_OK;
#endif /* CHECKS */
      if (save_ok)
      { /* ok */
         if (ptr->u_el.u_xel.u_sub.sub_chain->u_el.u_xel.u_hdr.chain_status &
             TO_BE_SAVED) 
         { /* markierte Kette */
            ptr->u_el.u_xel.u_sub.sub_chain->u_el.u_xel.u_hdr.chain_status &= 
               ~TO_BE_SAVED;
            save_ok = save_chain (ptr->u_el.u_xel.u_sub.sub_chain, als_bak);
            ctr++;
         } /* markierte Kette */
         ptr = ptr->u_el.next_action;
      } /* ok */
   } /* all along the VK */
   return (ctr);
} /* save_marked_chs_of_vk */



/* =============================== save_chain =============================== */
BOOL save_chain (CHAIN_ELEM_PTR_TYPE  ch_ptr, 
                 BOOL                 als_bak)
{
   CHAIN_ELEM_PTR_TYPE  ptr, sub_ak_ptr;
   REF_DESCR_PTR_TYPE   sd_ptr;
   INT                  elem_nr;
   INT                  sd_anz, ii;
   USHORT               old_status;
   BOOL                 io_ok, ak_save;
  
   /* save_chain */
#ifdef CHECKS
   if (ch_ptr == NULL) 
   {
      fatal (CHIO_CHAIN_IO, 9, NO_INFO, NO_INFO, "", NIL_PTR);
      return (FALSE);
   }
#endif /* CHECKS */
   ak_save = ch_ptr->u_el.activity == AK_HEADER;
   io_ok   = TRUE;         /* fuer die Rettung von Ketten, die keine AKs sind */
#ifdef CHECKS
   if (als_bak && !ak_save) 
   {
      fatal (CHIO_CHAIN_IO, 10, NO_INFO, NO_INFO, "", AK_EXPECTED);
      return (FALSE);
   } /* CHECKS */
#endif
   ptr         = ch_ptr->u_el.next_action;
   elem_nr     = 0;
   if (als_bak)
   {
      old_status = ch_ptr->u_el.u_xel.u_hdr.chain_status;
      ch_ptr->u_el.u_xel.u_hdr.chain_status |= BASIS_AK;
   }
   if (global_save)
      ch_ptr->u_el.u_xel.u_hdr.ak_index = NO_INDEX;
   else 
   { /* selektive Rettung */
      if (ch_ptr->u_el.u_xel.u_hdr.ak_index == 0) 
      { /* Neuen "ak_index" vergeben */
         ch_ptr->u_el.u_xel.u_hdr.ak_index = ak_ind_ctr;
         ak_ind_ctr++;
      } /* Neuen "ak_index" vergeben */
   } /* selektive Rettung */

   if (write_elem (ch_ptr, elem_nr, ELEM_REC))      /* ---- Write Header ---- */
   { /* Hdr erfolgreich gerettet */
      if (als_bak)
         ch_ptr->u_el.u_xel.u_hdr.chain_status = old_status;
      if (global_save)
         ch_ptr->u_el.u_xel.u_hdr.ak_index = 0;
      if (ak_save) 
      { /* AK */
         elem_nr--;
         io_ok = write_elem (ch_ptr->u_el.u_xel.u_hdr.header_par_ptr, 
                             elem_nr, AK_HDR_REC);    /* --- Write HdrPar --- */
         if (ak_save && (ch_ptr->u_el.u_xel.u_hdr.segment_anz > 0)) 
         { /* segmentierte AK */
            elem_nr--;
            sd_ptr = ch_ptr->u_el.u_xel.u_hdr.segm_descr_ptr;
            sd_anz = (ch_ptr->u_el.u_xel.u_hdr.segment_anz / SEGM_PTR_ANZ) + 1;
            for (ii = 0;  (ii < sd_anz) && io_ok;  ii++)
            { /* Segmentdescriptoren retten */
               io_ok  = write_descr (sd_ptr, elem_nr); /* - Write SegmDescr - */
               sd_ptr = sd_ptr->next_ref_descr_ptr;
               elem_nr--;
            } /* Segmentdescriptoren retten */
         } /* segmentierte AK */
      } /* AK */

      /* ---------- Rettung der Ketten-Sequenz: ---------- */
      elem_nr = 1;
      while ((ptr != NULL) && io_ok) 
      { /* all along the chain */
         if (ptr->u_el.activity == ACTIVITY_ACT) 
         { /* activity_act */
            sub_ak_ptr = ptr->u_el.u_xel.u_sub.sub_chain;
            if (sub_ak_ptr != NULL) 
            { /* es gibt eine Unterkette */
               if (ak_save) 
               { /* activity_act in AK */
                  if (ptr->u_el.u_xel.u_sub.ref_typ == SUB_AK_REF)
                  { /* AK-Referenz */
                     if (global_save) 
                     { /* globale Rettung */
                        ptr->u_el.u_xel.u_sub.sub_ak_guete  = sub_ak_ptr->u_el.
                           u_xel.u_hdr.guete;
                        ptr->u_el.u_xel.u_sub.sub_ak_lfd_nr = sub_ak_ptr->u_el.
                           u_xel.u_hdr.lfd_nr;
                        ptr->u_el.u_xel.u_sub.sub_ak_index  = NO_INDEX;
                     } /* globale Rettung */
                     else 
                     { /* selektive Rettung */
                        if (sub_ak_ptr->u_el.u_xel.u_hdr.ak_index == 0)
                        { /* Neuen "ak_index" vergeben */
                           sub_ak_ptr->u_el.u_xel.u_hdr.ak_index = ak_ind_ctr;
                           ak_ind_ctr++;
                        } /* Neuen "ak_index" vergeben */
                        ptr->u_el.u_xel.u_sub.sub_ak_index = sub_ak_ptr->u_el.
                           u_xel.u_hdr.ak_index;
                        ptr->u_el.u_xel.u_sub.sub_ak_guete = sub_ak_ptr->u_el.
                           u_xel.u_hdr.guete;
                     } /* selektive Rettung */
                  } /* activity_act in AK */
               } /* AK-Referenz */
               io_ok = write_elem (ptr, elem_nr, ELEM_REC); /* - Write Elem - */
               if (global_save) 
                  ptr->u_el.u_xel.u_sub.sub_ak_index = 0;
               if ((ptr->u_el.u_xel.u_sub.ref_typ == LOC_SUB_AK) && io_ok) 
               { /* Save SubCh */
                  ptr->u_el.u_xel.u_sub.sub_chain->u_el.u_xel.u_hdr.
                     chain_status &= ~TO_BE_SAVED;
                  io_ok = save_chain (ptr->u_el.u_xel.u_sub.sub_chain, 
                                      FALSE);               /* - Save SubCh - */
               } /* Save SubCh */
            } /* es gibt eine Unterkette */
            else 
            { /* keine Unterkette */
               if (ak_save) 
               { /* activity_act ohne Sub-AK in AK */
                  ptr->u_el.u_xel.u_sub.sub_ak_index = 0;
                  io_ok = write_elem (ptr, elem_nr, ELEM_REC);  /* Write Elem */
               } /* activity_act ohne Sub-AK in AK */
               else 
                  io_ok = fatal (CHIO_CHAIN_IO, 11, NO_INFO, NO_INFO, "", 
                                 NO_SUB_CHAIN) == GLEAM_OK;
            } /* keine Unterkette */
         } /* activity_act */
         else 
         { /* Normales Element retten */
            io_ok = write_elem (ptr, elem_nr, ELEM_REC);  /* -- Write Elem -- */
         } /* Normales Element retten */
         ptr = ptr->u_el.next_action;
         elem_nr++;
      } /* all along the chain */
   } /* Hdr erfolgreich gerettet */
   else 
      io_ok = FALSE;
   return (io_ok);
} /* save_chain */



/* =============================== save_ch_mem ============================= */
BOOL save_ch_mem (VOID)
{
   STR_TYPE  buffer;
   INT       g_anz, ak_sum, gi;
   GSTATE    ret_state = GLEAM_OK;

   /* save_ch_mem */
   ak_sum  = 0;
   save_ok = TRUE;
   for (gi = 1;  (gi <= MAX_GUETE) && save_ok;  gi++) 
   { /* Ueber alle Guete-Klassen */
      g_anz = save_vk_contents (get_vk (AK, gi, &ret_state));
#ifndef SLAVE
      if ((g_anz > 0) && stnd_io)
      { /* Es wurde etwas gerettet und nur wenn der Master bei File-I/O */
         sprintf (buffer, SAVED_AKS_TXT, offs_buf, g_anz, gi);
         write_buf (znr++, buffer);
      } /* Es wurde etwas gerettet und nur wenn der Master bei File-I/O */
#endif /* Kein Slave */
      ak_sum += g_anz;
   } /* Ueber alle Guete-Klassen */
   if (save_ok)
      sprintf (buffer, SAVED_OK_TXT, offs_buf, ak_sum);
   else
      sprintf (buffer, BAD_SAVE_TXT, offs_buf, ak_sum);
#ifndef SLAVE
   if (stnd_io)
      write_buf (znr, buffer);                 /* Nur der Master bei File-I/O */
#endif
   if (chio_logging)
      write_to_log (buffer, CLOSE_LOG);
   return (save_ok);
} /* save_ch_mem */



/* ============================ save_vk_contents ============================ */
static INT save_vk_contents (CHAIN_ELEM_PTR_TYPE  vk_ptr)
/*----------------------------------------------------------------------------*/
/*  Rettet alle Ketten der Verwaltungskette "vk_ptr" und liefert die Anzahl   */
/*  gerettetr Ketten als Funktionsergebnis ab. Bei auftretenden Fehlern wird  */
/*  "save_ok" auf FALSE gesetzt.                                              */
/*----------------------------------------------------------------------------*/
{
   CHAIN_ELEM_PTR_TYPE  ptr;
   GSTATE               erg_state = GLEAM_OK;
 
   /* save_vk_contents */ 
   ptr = vk_ptr->u_el.next_action;
   while ((ptr != NULL) && save_ok)
   { /* VK abarbeiten */
#ifdef CHECKS
      if (ptr->u_el.activity != ACTIVITY_ACT) 
         erg_state = fatal (CHIO_CHAIN_IO, 12, ptr->u_el.activity, NO_INFO, 
                            "", NO_ACTIVITY_ACT);
      if (ptr->u_el.u_xel.u_sub.sub_chain == NULL)
         erg_state |= fatal (CHIO_CHAIN_IO, 13, NO_INFO, NO_INFO, "", NIL_PTR);
      if (erg_state != GLEAM_OK)
      { /* Mist */
         save_ok = FALSE;
         return (0);
      } /* Mist */
#endif /* CHECKS */
      save_ok = save_chain (ptr->u_el.u_xel.u_sub.sub_chain, FALSE);
      ptr     = ptr->u_el.next_action;
   } /* VK abarbeiten */
   return (vk_ptr->u_el.u_xel.u_hdr.chain_length);
} /* save_vk_contents */



/* =============================== save_baks ================================ */
BOOL save_baks (void)
{
   CHAIN_ELEM_PTR_TYPE  vk_ptr, el_ptr;
   GSTATE               ret_state = GLEAM_OK;

   /* save_baks */
   vk_ptr = get_vk (AK, BASIS_AK_GUETE, &ret_state);
   el_ptr = vk_ptr->u_el.next_action;
   while (el_ptr != NULL)
   { /* VK abarbeiten: AK-Indices in aufsteigender Reihenfolge vergeben */
#ifdef CHECKS
      if (el_ptr->u_el.activity != ACTIVITY_ACT) 
         ret_state = fatal (CHIO_CHAIN_IO, 16, el_ptr->u_el.activity, NO_INFO, 
                            "", NO_ACTIVITY_ACT);
      if (el_ptr->u_el.u_xel.u_sub.sub_chain == NULL)
         ret_state |= fatal (CHIO_CHAIN_IO, 17, NO_INFO, NO_INFO, "", NIL_PTR);
      if (ret_state != GLEAM_OK)
         return (FALSE);                                           /* Abbruch */
#endif /* CHECKS */
      el_ptr->u_el.u_xel.u_sub.sub_chain->u_el.u_xel.u_hdr.ak_index= ak_ind_ctr;
      ak_ind_ctr++;
      el_ptr = el_ptr->u_el.next_action;
   } /* VK abarbeiten: AK-Indices in aufsteigender Reihenfolge vergeben */
   save_ok = TRUE;
   save_vk_contents (vk_ptr);              /* "save_ok" wird bei Fehler FALSE */
   return (save_ok);
} /* save_baks */



/* ============================= clr_bak_indices ============================ */
void clr_bak_indices (void)
{
   CHAIN_ELEM_PTR_TYPE  vk_ptr, el_ptr;
   GSTATE               ret_state = GLEAM_OK;

   /* clr_bak_indices */
   vk_ptr = get_vk (AK, BASIS_AK_GUETE, &ret_state);
   el_ptr = vk_ptr->u_el.next_action;
   while (el_ptr != NULL)
   { /* VK abarbeiten: AK-Indices loeschen */
      el_ptr->u_el.u_xel.u_sub.sub_chain->u_el.u_xel.u_hdr.ak_index = 0;
      el_ptr = el_ptr->u_el.next_action;
   } /* VK abarbeiten: AK-Indices loeschen */
} /* clr_bak_indices */



/* =============================== restore_chs ============================== */
BOOL restore_chs (BOOL  basis_aks, 
                  BOOL  anzeige)
{
   CHAIN_ELEM_PTR_TYPE  vk_s_ptr, ch_ptr;
   STR_TYPE             buf1, buf2, buf3, buffer;
   DOUBLE               simu_note, best_note;
   INT                  guete , lfd_nr, ch_kind, del_aks, 
                        ak_ctr, gi;
   BOOL                 restore_continue, rest_err, restore_ok, bak,
                        notTriedToStartSimu; /*not tried to start ext.simu yet*/
   GSTATE               ret_state = GLEAM_OK; 

   /* restore_chs */
   ak_ctr              = 0;
   restore_ok          = TRUE;
   restore_continue    = TRUE;
   rest_err            = FALSE;
   notTriedToStartSimu = TRUE;
   best_note           = 0.0;
   if (ch_file_ok ()) 
   { /* Datei nicht leer */
      while (restore_continue && (!eof_found)) 
      { /* Ketten lesen */
         bak = FALSE;
         if (read_chain (&ch_ptr, CH_START)) 
         { /* Kette gelesen */
            switch (ch_state) 
            {
               case AK_RESTORE:  /* AK */
               ak_ctr++;
               ch_kind = AK;
#ifndef GORBA_KP_BEW
               if (!(ch_ptr->u_el.u_xel.u_hdr.chain_status & SIMULATED))
               { /* unsimulierte AK restauriert */
		  if (akt_anz_krit > akt_anz_roh_erg_werte)
                  { /* mehr Bewertungskriterien als Simulationsergebnisse */
		     err_state |= fehler(CHIO_CHAIN_IO, 2, akt_anz_krit, akt_anz_roh_erg_werte, "", 
					 MORE_CRITERIA_THAN_RESULTS);
		     meldung(CHIO_CHAIN_IO, NO_INFO, NO_INFO, "", REST_AK_NOT_SIMULATED);
		     restore_continue = FALSE;
		     ch_ptr->u_el.u_xel.u_hdr.fitness_note = KEINE_NOTE;
		  } /* mehr Bewertungskriterien als Simulationsergebnisse */
		  else
		  { /* hoechstens soviel Bewertungskriterien wie Simulationsergebnisse */
		     if (mit_ext_simu && !sim_up && notTriedToStartSimu)
		     { /* check/init Simu u. Modell */
			printf ("\n%s\n", EXT_SIMU_STARTUP_TXT);
			ext_simu_hochfahren (FALSE);
			notTriedToStartSimu = FALSE;
		     } /* check/init Simu u. Modell */
		     if (sim_up)
		     { /* simulator is available */
			ret_state = simulator (&simu_note, CALC_FITNESS, ch_ptr);
			if ((ret_state != GLEAM_OK) || (simu_note == KEINE_NOTE))
			   meldung (CHIO_CHAIN_IO, NO_INFO, NO_INFO, "", REST_AK_NOT_SIMULATED);
		     } /* simulator is available */
		     else
			meldung (CHIO_CHAIN_IO, NO_INFO, NO_INFO, "", REST_AK_NOT_SIMULATED);
		  } /* hoechstens soviel Bewertungskriterien wie Simulationsergebnisse */
               }  /* unsimulierte AK restauriert */
#endif /* kein GORBA_KP_BEW */
               if (berechne_guete (&guete, ch_ptr->u_el.u_xel.u_hdr.fitness_note))
               { /* Note und damit Guete OK */
                  if (basis_aks) 
                  { /* Basis-AK Behandlung */
                     if (ch_ptr->u_el.u_xel.u_hdr.chain_status & BASIS_AK)
                     { /* Basis-AK */
                        if (store_chain (&lfd_nr, ch_ptr, ch_kind, 
                                         BASIS_AK_GUETE) != GLEAM_OK)
                        { /* Abspeicherung fehlgeschlagen */
                           restore_ok       = FALSE;
                           restore_continue = FALSE;
                        } /* Abspeicherung fehlgeschlagen */
                        bak = TRUE;
                     } /* Basis-AK */
                  } /* Basis-AK Behandlung */
                  else  /* Keine Basis-AK */
                     ch_ptr->u_el.u_xel.u_hdr.chain_status &= ~BASIS_AK;
               } /* Note und damit Guete OK */
               else
                  restore_ok = FALSE;
               break; /* AK */

               default:
                  fehler (CHIO_CHAIN_IO, 18, ch_state, NO_INFO, "", 
                          RESTORE_ERR);
                  restore_ok = FALSE;
               break;
            } /* CASE */

            if (restore_ok) 
            { /* Speicherung und Speicherungsanzeige */
               if (ch_state == AK_RESTORE) 
               { /* AK */
                  if (store_chain (&lfd_nr, ch_ptr, ch_kind, guete) == GLEAM_OK)
                  { /* Abspeicherung ok */
#ifndef SLAVE
                     if (anzeige && stnd_io) 
                     { /* optionale Anzeige bei File-I/O */
                        state_to_txt (buf1);
                        adr_to_text (buf2, guete, lfd_nr, ADJUST);
                        if (bak) 
                           strcpy (buf3, BAK_TXT);
                        else 
                           strcpy (buf3, "");
                        sprintf (buffer, AK_OK_TXT, offs_buf, buf1, buf2, buf3);
                        write_buf (znr++, buffer);
                        anzeige = znr < MAX_BUF_ANZ-1;
                     } /* optionale Anzeige bei File-I/O */
                     if (ch_ptr->u_el.u_xel.u_hdr.fitness_note > best_note)
			best_note = ch_ptr->u_el.u_xel.u_hdr.fitness_note;
#endif /* Kein Slave */
                  } /* Abspeicherung ok */
                  else
                  { /* Abspeicherung fehlgeschlagen */
                     restore_ok       = FALSE;
                     restore_continue = FALSE;
                  } /* Abspeicherung fehlgeschlagen */
               } /* AK */
            } /* Speicherung und Speicherungsanzeige */
            if (!restore_ok)
               delete_chain (&ch_ptr);
         } /* Kette gelesen */
         else 
            if (!eof_found) 
            { /* Kette nicht OK */
               restore_continue = FALSE;
               restore_ok       = FALSE;
               state_to_txt (buf1);
               meldung (CHIO_CHAIN_IO, NO_INFO, NO_INFO, buf1, CH_ERR_LOE_MLDG);
            } /* Kette nicht OK */
      } /* Ketten lesen */
   } /* Datei nicht leer */

   /* -- 2.Pass: Sub-AK-Restaurierung, AK-Indices loe u. Fehlerbehandlung: -- */

   if (restore_ok && (ak_ctr > 0)) 
   { /* Restaurierung der Sub-AKs */
      gi = 0;
      do 
      { /* Sub-AK-Rest. in allen Guete-Klassen */
         vk_s_ptr = get_vk (AK, gi, &ret_state)->u_el.next_action;
         while ((vk_s_ptr != NULL) && restore_ok) 
         { /* VK nach restaurierten AKs absuchen */
            if (vk_s_ptr->u_el.u_xel.u_sub.sub_chain->u_el.u_xel.u_hdr.
                ak_index != 0) 
            { /* Restaurierte AK gefunden */
               if (!restore_ch_ref (vk_s_ptr->u_el.u_xel.u_sub.sub_chain))
               {
                  restore_ok = FALSE;
                  rest_err   = TRUE;
               }
            } /* Restaurierte AK gefunden */
            vk_s_ptr = vk_s_ptr->u_el.next_action;
         } /* VK nach restaurierten AKs absuchen */
         gi++;
      } /* Sub-AK-Rest. in allen Guete-Klassen */
      while (!(!restore_ok || (gi > MAX_GUETE)));

#ifndef SLAVE
      if (restore_ok) 
      { /* AK-Indices loeschen */
         for (gi = 0;  gi <= MAX_GUETE;  gi++) 
         { /* Alle Gueteklassen */
            vk_s_ptr = get_vk (AK, gi, &ret_state)->u_el.next_action;
            while ((vk_s_ptr != NULL)) 
            { /* "ak_index" der AKs dieser VK ruecksetzten */
               vk_s_ptr->u_el.u_xel.u_sub.sub_chain->u_el.u_xel.u_hdr.
                  ak_index = 0;
               vk_s_ptr = vk_s_ptr->u_el.next_action;
            } /* "ak_index" der AKs dieser VK ruecksetzten */
         } /* Alle Gueteklassen */
      } /* AK-Indices loeschen */
#endif /* Kein Slave */
   } /* Restaurierung der Sub-AKs */

   if (restore_ok)
   { /* Jubel-Meldung */
      sprintf (buffer, REST_OK_TXT, offs_buf, ak_ctr, best_note);
      if (chio_logging)
         write_to_log (buffer, CLOSE_LOG);             /* Log-Protokollierung */
#ifndef SLAVE
      if (stnd_io)
      { /* Nur der Master bei File-I/O */
         if (anzeige)
         {
            if (znr >= (MAX_BUF_ANZ - 2))
               write_buf (MAX_BUF_ANZ, buffer);
            else
               write_buf (znr, buffer);
         }
         else
            write_buf (znr, buffer);
      } /* Nur der Master bei File-I/O */
#endif /* Kein Slave */
   } /* Jubel-Meldung */
   else
   { /* Fehlerabbruch */
      if ((ak_ctr > 0) || rest_err) 
      { /* Alle geladenen AKs loeschen */
	 ret_state = GLEAM_OK;     /* sonst funktioniert z.B. "get_vk" nicht! */
         del_restored_chs (get_vk (AK, BASIS_AK_GUETE, &ret_state), &ret_state);
         del_aks = 0;                                /* BAKs nicht mitzaehlen */
         for (gi = 1;  (gi <= MAX_GUETE) && (ret_state == GLEAM_OK);  gi++) 
            del_aks += del_restored_chs (get_vk (AK, gi, &ret_state), &ret_state);
         meldung (CHIO_CHAIN_IO, del_aks, NO_INFO, "", CH_ERR_LOE_SUM_MLDG);
         if (ret_state != GLEAM_OK)
            fatal (CHIO_CHAIN_IO, 19, NO_INFO, NO_INFO, "", CH_REST_ERR_BAD_RECOV);
      } /* Alle geladenen AKs loeschen */
   } /* Fehlerabbruch */
   return (restore_ok && restore_continue);
} /* restore_chs */



/* ============================== state_to_txt ============================== */
static VOID state_to_txt (STR_TYPE  buf)
/*----------------------------------------------------------------------------*/
/*  Liefert in "buf" die zu "ch_state" gehoerige Kettenbezeichnung ab.        */
/*----------------------------------------------------------------------------*/
{ 
   switch (ch_state) 
   {
      case NOTHING:
         strcpy (buf, "??");
      break;
      case AK_RESTORE:
         strcpy (buf, "AK");
      break;
      case LOC_AK_RESTORE:
         strcpy (buf, "SubAK");
      break;
      default:
         strcpy (buf, "???");
   } /* CASE */
} /* state_to_txt */



/* =============================== ch_file_ok =============================== */
BOOL ch_file_ok (void)
{
   if (ch_io_mode == MPI2_IO)
      return TRUE;
   else
      return (chainfile_open && !(feof (chain_file)));
} /* ch_file_ok */



/* =============================== read_elem ================================ */
static BOOL read_elem (CHAIN_ELEM_PTR_TYPE  elem_ptr,
                       INT                  item_nr,
                       BOOL                 elem_rec)
/*----------------------------------------------------------------------------*/
/*  Liefert TRUE, wenn Kettenkopf, -element oder HeaderParam erfolgreich ge-  */
/*  lesen werden konnte. Die Daten werden dann in einem Element, auf das      */
/*  "elem_ptr" zeigt, abgeliefert und die Funktion liefert TRUE. Andernfalls  */
/*  liefert sie FALSE u. eine Fehlermeldung unter Verwendung der "item_nr"    */
/*  (==0:Kopf, ==-1:HdrParam, >0:Elem). Bei EOF wird "eof_found" TRUE.        */
/*----------------------------------------------------------------------------*/
{ 
   SAVE_CAST_TYPE  save_rec;
   STRING_TYPE     skip_buffer;
   INT             ai, ii;
   BOOL            ok = FALSE;

   /* read_elem */
   switch (ch_io_mode)
   {
      case FILE_BIN_IO:
         ok = (fread (&save_rec, sizeof(SAVE_CAST_TYPE), 1, chain_file) == 1);       
         *elem_ptr = save_rec.chain_elem;
         if (!eof_found)
            eof_found = feof (chain_file);
      break; /* FILE_BIN_IO */

      case FILE_TXT_IO:
         if (elem_rec)
         { /* Elem */
            ok = fscanf (chain_file, "%hd", &(elem_ptr->u_el.activity)) == 1;
            if (ok)
               switch (elem_ptr->u_el.activity)
               {
                  case AK_HEADER:
                  ok = fscanf (chain_file, 
                               "%le %hd %hd %hd %hd %hu %hd %hd", 
                               &(elem_ptr->u_el.u_xel.u_hdr.fitness_note),
                               &(elem_ptr->u_el.u_xel.u_hdr.ref_counter),
                               &(elem_ptr->u_el.u_xel.u_hdr.chain_length),
                               &(elem_ptr->u_el.u_xel.u_hdr.guete),
                               &(elem_ptr->u_el.u_xel.u_hdr.lfd_nr),
                               &(elem_ptr->u_el.u_xel.u_hdr.chain_status),
                               &(elem_ptr->u_el.u_xel.u_hdr.segment_anz),
                               &(elem_ptr->u_el.u_xel.u_hdr.ak_index)) == 8;
                  break; /* AK_HEADER */

                  case ACTIVITY_ACT:
                  ok = fscanf (chain_file, "%hd %hd %hd %hd", 
                               &(elem_ptr->u_el.u_xel.u_sub.ref_typ),
                               &(elem_ptr->u_el.u_xel.u_sub.sub_ak_guete),
                               &(elem_ptr->u_el.u_xel.u_sub.sub_ak_lfd_nr),
                               &(elem_ptr->u_el.u_xel.u_sub.sub_ak_index)) == 4;
                  break; /* ACTIVITY_ACT */

                  default: /* Aktion */
                  for (ii = 0;  (ii < I_PAR_ANZ_MAX) && ok;  ii++)
                     ok = fscanf (chain_file, "%d",
                                  &(elem_ptr->u_el.u_xel.u_act.i_params[ii])) == 1;
                  for (ii = 0;  (ii < R_PAR_ANZ_MAX) && ok;  ii++)
                     ok = fscanf (chain_file, "%le",
                                  &(elem_ptr->u_el.u_xel.u_act.r_params[ii])) == 1;
               } /* CASE "activity" */
            if (ok)
               ok = fscanf (chain_file, "%d", &ii) == 1;
            if (ii == PTR_KENNUNG)
               elem_ptr->u_el.next_action = elem_ptr;
            else
               elem_ptr->u_el.next_action = NULL;
         } /* Elem */
         else
         { /* AK-Header-Param */
            ok = TRUE;
            for (ii = 0;  (ii < AK_ERG_WERTE_MAX) && ok;  ii++)
               ok = fscanf(chain_file, "%le", &(elem_ptr->sim_erg_werte[ii]))==1;
         } /* AK-Header-Param */
         if (ok)
            ok = fgets (skip_buffer, MAX_STRING, chain_file) != NULL;
         if (!eof_found)
            eof_found = feof (chain_file);
      break; /* FILE_TXT_IO */

      case MPI2_IO:
#ifdef MPI_PAR_VERS
         if (elem_rec)
         { /* Elem */
	    ai = (INT)trunc(akPuffer[akPufferIdx]);
            elem_ptr->u_el.activity = ai;
	    if ((akPuffer[akPufferIdx++] - ai) > 0.0)  /* NULL-Pointer-Indikator */
	       elem_ptr->u_el.next_action = NULL; 
	    else
               elem_ptr->u_el.next_action = elem_ptr;
	    switch (elem_ptr->u_el.activity)
            {
	       case AK_HEADER:
		  elem_ptr->u_el.u_xel.u_hdr.fitness_note = akPuffer[akPufferIdx++];
		  elem_ptr->u_el.u_xel.u_hdr.ref_counter  = (SHORT)trunc(akPuffer[akPufferIdx++]);
		  elem_ptr->u_el.u_xel.u_hdr.chain_length = (SHORT)trunc(akPuffer[akPufferIdx++]);
		  elem_ptr->u_el.u_xel.u_hdr.guete        = (SHORT)trunc(akPuffer[akPufferIdx++]);
		  elem_ptr->u_el.u_xel.u_hdr.lfd_nr       = (SHORT)trunc(akPuffer[akPufferIdx++]);
		  elem_ptr->u_el.u_xel.u_hdr.chain_status = (SHORT)trunc(akPuffer[akPufferIdx++]);
		  elem_ptr->u_el.u_xel.u_hdr.segment_anz  = (SHORT)trunc(akPuffer[akPufferIdx++]);
		  elem_ptr->u_el.u_xel.u_hdr.ak_index     = (SHORT)trunc(akPuffer[akPufferIdx++]);
		  elem_ptr->u_el.u_xel.u_hdr.parIndIdx    = (SHORT)trunc(akPuffer[akPufferIdx++]);
	       break; /* AK_HEADER */

               case ACTIVITY_ACT:
		  elem_ptr->u_el.u_xel.u_sub.ref_typ      = (SHORT)trunc(akPuffer[akPufferIdx++]);
		  elem_ptr->u_el.u_xel.u_sub.sub_ak_guete = (SHORT)trunc(akPuffer[akPufferIdx++]);
		  elem_ptr->u_el.u_xel.u_sub.sub_ak_lfd_nr= (SHORT)trunc(akPuffer[akPufferIdx++]);
		  elem_ptr->u_el.u_xel.u_sub.sub_ak_index = (SHORT)trunc(akPuffer[akPufferIdx++]);
               break; /* ACTIVITY_ACT */

               default: /* Aktion */
		  for (ii = 0;  ii < int_pro_aktion[ai];  ii++)
		     elem_ptr->u_el.u_xel.u_act.i_params[ii] = (INT)trunc(akPuffer[akPufferIdx++]);
		  memcpy (elem_ptr->u_el.u_xel.u_act.r_params, &(akPuffer[akPufferIdx]), 
			  real_pro_aktion[ai] * sizeof(DOUBLE));
		  akPufferIdx += real_pro_aktion[ai];
            } /* CASE "activity" */
         } /* Elem */
         else
         { /* AK-Header-Param */
	    memcpy (&(elem_ptr->sim_erg_werte), &(akPuffer[akPufferIdx]), 
		    AK_ERG_WERTE_MAX * sizeof(DOUBLE));
	    akPufferIdx += AK_ERG_WERTE_MAX;
         } /* AK-Header-Param */
         ok = akPufferIdx <= akPufferLen;
         if (!eof_found)
            eof_found = (akPufferIdx >= akPufferLen);
#endif /* MPI_PAR_VERS */
         break; /* MPI2_IO */
   } /* CASE "ch_io_mode" */

   if (ch_io_mode == MPI2_IO) {
#ifdef MPI_PAR_VERS
      if (!(ok || ((item_nr == 0) && eof_found)))
         fehler (CHIO_CHAIN_IO, 61, item_nr, NO_INFO, "", CHIO_RECV_FAIL);
#else
      if (!(ok || ((item_nr == 0) && eof_found)))
         fehler (CHIO_CHAIN_IO, 61, item_nr, NO_INFO, "", CH_FILE_IO_ERR);
#endif /* MPI_PAR_VERS */
   } /* end if (ch_io_mode == MPI2_IO) */
   else {
      if (!(ok || ((item_nr == 0) && eof_found)))
         fehler (CHIO_CHAIN_IO, 62, item_nr, NO_INFO, "", CH_FILE_IO_ERR);
   }
   return (ok);
} /* read_elem */



/* =============================== read_descr =============================== */
static BOOL read_descr (REF_DESCR_PTR_TYPE  descr_ptr,
                        INT                 item_nr)
/*----------------------------------------------------------------------------*/
/*  Liefert TRUE, wenn ein Descriptorelement erfolgreich gelesen werden konn- */
/*  te und liefert die Daten in dem Element, auf das "descr_ptr" zeigt, ab.   */
/*  Sonst FALSE und eine Fehlermeldung unter Verwendung der "item_nr"         */
/*  (<-1:Descr).                                                              */
/*----------------------------------------------------------------------------*/
{ 
   SAVE_CAST_TYPE  save_rec;
   STRING_TYPE     skip_buffer;
   INT             ii, info;
   SHORT           wert;
   BOOL            ok = FALSE;

   /* read_descr */
   switch (ch_io_mode)
   {
      case FILE_BIN_IO:
         ok = fread (&save_rec, sizeof(SAVE_CAST_TYPE), 1, chain_file) == 1;
         *descr_ptr = save_rec.descr_elem;
         if (!eof_found)
            eof_found = feof (chain_file);
      break; /* FILE_BIN_IO */

      case FILE_TXT_IO:
         ok = TRUE;
         for (ii = 0;  (ii < SEGM_PTR_ANZ) && ok;  ii++)
         {
            ok = fscanf (chain_file, "%hd", &wert) == 1;
            descr_ptr->segm_length[ii] = wert;
         }
         if (ok)
            ok = fgets (skip_buffer, MAX_STRING, chain_file) != NULL;
         if (!eof_found)
            eof_found = feof (chain_file);
      break; /* FILE_TXT_IO */

      case MPI2_IO:
#ifdef MPI_PAR_VERS
         for (ii = 0;  ii < SEGM_PTR_ANZ;  ii++) 
            descr_ptr->segm_length[ii] = (SHORT)trunc(akPuffer[akPufferIdx++]);
	 ok = akPufferIdx <= akPufferLen;
#endif /* MPI_PAR_VERS */
      break; /* MPI2_IO */
   } /* CASE "ch_io_mode" */

   if (ch_io_mode == MPI2_IO) {
#ifdef MPI_PAR_VERS
      if (!ok)
         fehler (CHIO_CHAIN_IO, 63, item_nr, NO_INFO, "", CHIO_SEND_FAIL);
#else
      if (!ok)
         fehler (CHIO_CHAIN_IO, 63, item_nr, NO_INFO, "", CH_FILE_IO_ERR);
#endif /* MPI_PAR_VERS */
   } /* end if (ch_io_mode == MPI2_IO) */
   else {
      if (!ok)
         fehler (CHIO_CHAIN_IO, 64, item_nr, NO_INFO, "", CH_FILE_IO_ERR);
   }
   return (ok);
} /* read_descr */



/* =============================== read_chain =============================== */
BOOL read_chain (CHAIN_ELEM_PTR_TYPE *ch_ptr,
                 BOOL                 reset_state)
{
   CHAIN_ELEM_PTR_TYPE  sub_ch_ptr, 
                        chain_ptr, ptr;
   REF_DESCR_PTR_TYPE   old_sd_ptr, 
                        sd_ptr;
   SHORT   hdr_code;
   INT     elem_nr, index,  ind_sum, segm_anz, 
           item_anz, sd_anz, ch_len,  ii;
   BOOL    r_read_chain, ak_rest,
           AK_EndeErreicht, nextIstNull;
   GSTATE  ret_state = GLEAM_OK;
  
   /* read_chain */
   if (reset_state)
      ch_state = NOTHING;
   *ch_ptr      = NULL;
   r_read_chain = FALSE;
   if (!enough_free_mem (AK_HEADER, 0, 0))
   { /* MemOut */
      fmeld (CH_REST_ERR_MEM_OUT, NO_INFO, 20);
      return (r_read_chain);
   } /* MemOut */

   /* ----------------------------- read header: ---------------------------- */
   new_chain_elem (&chain_ptr);
   if (read_elem (chain_ptr, 0, ELEM_REC))
   { /* Header gelesen */
      hdr_code  = chain_ptr->u_el.activity;
      ch_len    = chain_ptr->u_el.u_xel.u_hdr.chain_length;
      ak_rest   = hdr_code == AK_HEADER;
      if (ak_rest)       /* Pruefung auf alle gueltigen Header (z.Zt. nur AK) */
      { /* valid header */
         if (((ch_state == NOTHING)        && ak_rest) ||
             ((ch_state == AK_RESTORE)     && ak_rest) ||
             ((ch_state == LOC_AK_RESTORE) && ak_rest))
         { /* valid chain context */
            if (ak_rest) 
            { /* AK */
               if (ch_state == NOTHING) 
                  ch_state = AK_RESTORE;
               else
                  if (ch_state == AK_RESTORE)
                     ch_state = LOC_AK_RESTORE;
                  else
                     loc_ch_ctr++;
               segm_anz = chain_ptr->u_el.u_xel.u_hdr.segment_anz;
               item_anz = ch_len + 2;   /* +1: VK-Item, +1: VK-Item falls BAK */
            } /* AK */

            if (enough_free_mem (hdr_code, item_anz, segm_anz))
            { /* Platz ok */
               chain_ptr->u_el.next_action                = NULL;
               chain_ptr->u_el.u_xel.u_hdr.last_action    = chain_ptr;
               chain_ptr->u_el.u_xel.u_hdr.segm_descr_ptr = NULL;
               chain_ptr->u_el.u_xel.u_hdr.header_par_ptr = NULL;
               chain_ptr->u_el.u_xel.u_hdr.chain_length   = 0;
               chain_ptr->u_el.u_xel.u_hdr.ref_counter    = 0;
               /* "segment_anz" muss wegen "append_item" 0 sein: */
               chain_ptr->u_el.u_xel.u_hdr.segment_anz    = 0;

               /* ---------------- read header param record: ---------------- */
               if (ak_rest) 
               { /* AK (Kette mit ParamRecord und (optionaler) Segmentierung) */
                  new_chain_elem (&ptr);
                  ch_ok = read_elem (ptr, -1, AK_HDR_REC);
                  if (ch_ok)
                  { /* HeaderParam gelesen */
                     chain_ptr->u_el.u_xel.u_hdr.header_par_ptr = ptr;
                     if (segm_anz > 0)
                     { /* AK: Segment-Descriptoren lesen */
                        sd_anz  = (segm_anz / SEGM_PTR_ANZ) + 1;
                        elem_nr = -2;
                        ii      = 0;
                        do 
                        { /* einen Segment-Descriptor lesen */
                           /* ---------- read segment descriptor: ----------- */
                           if (new_ref_descr (&sd_ptr) == GLEAM_OK)
                           { /* Ref-Descr ok */
                              if (read_descr (sd_ptr, elem_nr)) 
                              { /* I/O ok */
                                 if (ii == 0) 
                                    chain_ptr->u_el.u_xel.u_hdr.segm_descr_ptr =
                                       sd_ptr;
                                 else 
                                    old_sd_ptr->next_ref_descr_ptr = sd_ptr;
                                 old_sd_ptr = sd_ptr;
                                 ii++;
                                 elem_nr--;
                              } /* I/O ok */
                              else
                              { /* I/O-Error */
                                 ch_ok = FALSE;
                                 free_ref_descr (sd_ptr);
                              } /* I/O-Error */
                           } /* Ref-Descr ok */
                           else
                              ch_ok = FALSE;
                        } while (ch_ok && (ii < sd_anz));
                        sd_ptr->next_ref_descr_ptr = NULL;
                     } /* AK: Segment-Descriptoren lesen */
                  } /* HeaderParam gelesen */
               } /* AK (Kette mit ParamRecord und (optionaler) Segmentierung) */

               /* --------------- Einlesen der Kettensequenz: --------------- */
               elem_nr = 0;
               while (ch_ok && (elem_nr < ch_len)) 
               { /* Kettensequenz lesen */
                  elem_nr++;
                  if (new_chain_elem (&ptr) == GLEAM_OK)
                  { /* Elem ok */
                     /* ------------------ read chain elem: ----------------- */
                      if (read_elem (ptr, elem_nr, ELEM_REC))
                     { /* I/O ok */
                        if (ptr->u_el.activity == ACTIVITY_ACT) 
                        { /* Activity-Act-Item */
                           ptr->u_el.u_xel.u_sub.sub_chain = NULL;
                           if ((ptr->u_el.u_xel.u_sub.ref_typ == LOC_SUB_AK) &&
                               ((ch_state == AK_RESTORE) || (ch_state == LOC_AK_RESTORE)))
                           { /* Sub-Ak im gueltigen Kontext */
                              if (read_chain (&sub_ch_ptr, NO_CH_START))
                                 ch_ok = append_sub_chain (ptr, sub_ch_ptr) == GLEAM_OK;
                              else
                                 ch_ok = FALSE;
                           } /* Sub-Ak im gueltigen Kontext */
                           else
                              ch_ok = ((ch_state == AK_RESTORE) || (ch_state == LOC_AK_RESTORE)) &&
                                       (ptr->u_el.u_xel.u_sub.ref_typ == SUB_AK_REF);
                        } /* Activity-Act-Item */
			nextIstNull = (ptr->u_el.next_action == NULL);
                        if (nextIstNull && (elem_nr < ch_len))
                        { /* I/O-Error */
                           fmeld (CH_REST_ERR_BAD_LEN, NO_INFO, 21);
                           free_chain_elem (ptr);
                           ch_ok = FALSE;
                        } /* I/O-Error */
                        else 
                           if (ch_ok)
                              ch_ok = append_item (chain_ptr, ptr) == GLEAM_OK;
                     } /* I/O ok */
                     else 
                     { /* I/O-Error */
                        ch_ok = FALSE;
                        free_chain_elem (ptr);
                     } /* I/O-Error */
                  } /* Elem ok */
                  else 
                     ch_ok = FALSE;
               } /* Kettensequenz lesen */
               if (ch_ok && !nextIstNull && (ch_len != 0))
                  fmeld (CH_REST_ERR_BAD_LEN, NO_INFO, 22);   /* -> ch_ok = FALSE */

               /* ---- "segment_anz" erst nach Aufbau der Sequenz setzen, --- */
               /* ---- da "append_item" die Segmentstruktur pflegt:       --- */
               if (ak_rest && ch_ok) 
                  chain_ptr->u_el.u_xel.u_hdr.segment_anz = segm_anz;

               /* -- Segmentierung von AKs restaurieren u. Kette abliefern: - */
               if (ch_ok) 
               { /* Kette erfolgreich gelesen */
                  if (ak_rest) 
                  { /* AK */
                     sd_ptr = chain_ptr->u_el.u_xel.u_hdr.segm_descr_ptr;
                     if (sd_ptr != NULL) 
                     { /* AK: Segmente restaurieren */
                        ind_sum = 0;
                        index   = 1;
                        ii      = 0;
                        sd_ptr->segm_ptrs[0] = chain_ptr;
                        while (ch_ok && (ii < segm_anz)) 
                        { /* Segment restaurieren */
                           ind_sum += sd_ptr->segm_length[index];
                           if (count_out_item (&ptr, chain_ptr, ind_sum, 
                               &ret_state))
                              sd_ptr->segm_ptrs[index] = ptr;
                           else 
                           { /* Loesche fehlerhafte Kette */
                              fmeld (CH_REST_ERR_SEGM_ERR, NO_INFO, 23);
                              delete_chain (&chain_ptr); 
                           } /* Loesche fehlerhafte Kette */
                           inc_segm_nr (&sd_ptr, &index);
                           ii++;
                        } /* Segment restaurieren */
                     } /* AK: Segmente restaurieren */
                     if (ch_state == LOC_AK_RESTORE)
		     {
                        if (loc_ch_ctr > 0)
                           loc_ch_ctr--;
                        else
                           ch_state = AK_RESTORE;
                     }
                  } /* AK */
                  *ch_ptr      = chain_ptr;
                  r_read_chain = ch_ok;
               } /* Kette erfolgreich gelesen */
               else 
                  delete_chain (&chain_ptr);     /* loe fehlerhafte Teilkette */
            } /* Platz ok */
            else
            { /* Platz nicht ok */
               fmeld (CH_REST_ERR_MEM_OUT, NO_INFO, 24);
               free_chain_elem (chain_ptr);
            } /* Platz nicht ok */
         } /* valid chain context */
         else 
         { /* invalid chain context */
            fmeld (CH_REST_ERR_UNZUL_HDR, hdr_code, 25);
            free_chain_elem (chain_ptr);
         } /* invalid chain context */
      } /* valid header */
      else 
      { /* invalid header */
         fehler (CHIO_CHAIN_IO, 26, hdr_code, NO_INFO, "", CH_REST_ERR_BAD_HDR);
         free_chain_elem (chain_ptr);
      } /* invalid header */
   } /* Header gelesen */
   else
      free_chain_elem (chain_ptr);
   return (r_read_chain);
} /* read_chain */



/* ============================= restore_ch_ref ============================= */
BOOL restore_ch_ref (CHAIN_ELEM_PTR_TYPE  ch_ptr)
{
   CHAIN_ELEM_PTR_TYPE  elem_ptr, ak_ptr;
   BOOL                 rest_ok = TRUE;

   /* restore_ch_ref */
   elem_ptr = ch_ptr->u_el.next_action;
   while ((elem_ptr != NULL) && rest_ok) 
   { /* Kette nach "activity_act"-Item mit Sub-AK absuchen */
      if ((elem_ptr->u_el.activity == ACTIVITY_ACT) && 
          (elem_ptr->u_el.u_xel.u_sub.sub_ak_index != 0)) 
      { /* "activity_act"-Item mit Sub-AK-Ref gefunden */
         if (search_ak (&ak_ptr, 
                        elem_ptr->u_el.u_xel.u_sub.sub_ak_index, 
                        elem_ptr->u_el.u_xel.u_sub.sub_ak_guete, 
                        elem_ptr->u_el.u_xel.u_sub.sub_ak_lfd_nr))
            append_sub_chain (elem_ptr, ak_ptr);
         else 
         { /* AK gibts nicht */
            rest_ok = FALSE;
            fehler (CHIO_CHAIN_IO, 27, NO_INFO, NO_INFO, "", 
                    SUB_AK_NOT_FOUND_ERR);
         } /* AK gibts nicht */
      } /* "activity_act"-Item mit Sub-AK-Ref gefunden */
      elem_ptr = elem_ptr->u_el.next_action;
   } /* Kette nach "activity_act"-Item mit Sub-AK absuchen */
   return (rest_ok);
} /* restore_ch_ref */



/* ================================== fmeld ================================= */
static VOID fmeld (INT   fnr,
                   LONG  info,
                   INT   ident)
/*----------------------------------------------------------------------------*/
/*  Erzeugt aus den uebergebenen Daten eine Fehlermeldung unter Angabe der    */
/*  aktuell behandelten Kettenart gemaess dem Wert von "ch_state" und setzt   */
/*  "ch_ok" auf FALSE. Die Routine wird nur von "read_chain" benutzt.         */
/*----------------------------------------------------------------------------*/
{
   STR_TYPE  buf;

   /* fmeld */
   state_to_txt (buf);
   fehler (CHIO_CHAIN_IO, ident, info, NO_INFO, buf, fnr);
   ch_ok = FALSE;
} /* fmeld */



/* ================================ search_ak =============================== */
static BOOL search_ak (CHAIN_ELEM_PTR_TYPE *ak_ptr, 
                       INT                  ak_ind, 
                       INT                  guete,
                       INT                  lfd_nr) 
/*----------------------------------------------------------------------------*/
/*  Sucht gesteuert von "ak_ind" wie eingangs beschrieben die zu "ak_ind"     */
/*  und "guete" bzw zu "lfd_nr" und "guete" gehoerige AK und liefert sie      */
/*  in "ak_ptr" ab. Die Funktion ist dann TRUE. Wenn die AK nicht auffind-    */
/*  bar ist, wird ein NIL-Pointer und FALSE abgeliefert.                      */
/*----------------------------------------------------------------------------*/
{ 
   CHAIN_ELEM_PTR_TYPE  ptr;
   BOOL                 r_search_ak, found;
   GSTATE               ret_state = GLEAM_OK;
 
   /* search_ak */
#ifdef CHECKS
   if ((guete < 0) || (MAX_GUETE < guete)) 
      return (fehler (CHIO_CHAIN_IO, 28, guete, NO_INFO, "", MEINE_GUETE) == 
              GLEAM_OK);
#endif /* CHECKS */
   if (ak_ind == NO_INDEX) 
   { /* AK ueber AK-Adresse finden */
      r_search_ak = get_chain (ak_ptr, &lfd_nr, AK, guete, &guete, &ret_state);
   } /* AK ueber AK-Adresse finden */
   else 
   { /* AK ueber "ak_index" finden */
      if (stnd_io)
      { /* Standard File-I/O */
         *ak_ptr = NULL;
         found   = FALSE;
         ptr     = get_vk (AK, guete, &ret_state)->u_el.next_action;
         while (!found && (ptr != NULL)) 
         { /* VK absuchen */
            if (ptr->u_el.u_xel.u_sub.sub_chain->u_el.u_xel.u_hdr.ak_index == 
                ak_ind) 
            { /* AK found */
               found   = TRUE;
               *ak_ptr = ptr->u_el.u_xel.u_sub.sub_chain;
            } /* AK found */
            else 
               ptr = ptr->u_el.next_action;
         } /* VK absuchen */
         r_search_ak = found;
      } /* Standard File-I/O */
      else
      { /* AK ist BAK im PVM-I/O-Kontext */
         guete = BASIS_AK_GUETE;
         r_search_ak = get_chain (ak_ptr, &ak_ind, AK, guete, &guete, 
                                  &ret_state);
      } /* AK ist BAK im PVM-I/O-Kontext */
   } /* AK ueber "ak_index" finden */
   return (r_search_ak);
} /* search_ak */



/* ============================ del_restored_chs ============================ */
static INT del_restored_chs (CHAIN_ELEM_PTR_TYPE  vk_ptr,
                             GSTATE              *ret_state)
/*----------------------------------------------------------------------------*/
/*  Sucht und loescht die restaurierten Ketten, die vom "vk_ptr" verwaltet    */
/*  werden und liefert die Anzahl geloeschter Ketten als Funktionsergebnis    */
/*  ab. In "ret_state" werden evtl. auftretende Fehler zurueckgemeldet.       */
/*----------------------------------------------------------------------------*/
{
   CHAIN_ELEM_PTR_TYPE  akt_ptr, del_ptr, pred_ptr;
   INT                  del_sum;
   BOOL                 restored_ch;
  
   /* del_restored_chs */
   del_sum  = 0;
   akt_ptr  = vk_ptr->u_el.next_action;
   pred_ptr = vk_ptr;
   while ((akt_ptr != NULL) && (*ret_state == GLEAM_OK)) 
   { /* restaurierte Kette suchen */
      if (akt_ptr->u_el.u_xel.u_sub.sub_chain->u_el.activity == AK_HEADER)
         restored_ch = akt_ptr->u_el.u_xel.u_sub.sub_chain->
            u_el.u_xel.u_hdr.ak_index != 0;
      else 
         restored_ch = FALSE;
      akt_ptr = akt_ptr->u_el.next_action;
      if (restored_ch) 
      { /* ein Opfer gefunden */
         if ((*ret_state = remove_next_item (&del_ptr, vk_ptr, pred_ptr)) == 
             GLEAM_OK)
            *ret_state = delete_item (del_ptr);
         del_sum++;
      } /* ein Opfer gefunden */
      else 
         pred_ptr = pred_ptr->u_el.next_action;
   } /* restaurierte Kette suchen */
   return (del_sum);
} /* del_restored_chs */



/* ============================== chain_io_exit ============================= */
static VOID chain_io_exit (VOID)
/*----------------------------------------------------------------------------*/
/*  Exithandler. Schliesst ein evenetuell noch offenes Ketten-File.           */
/*----------------------------------------------------------------------------*/
{
   if (chainfile_open) 
      fclose (chain_file);
} /* chain_io_exit */



/* ============================== chain_io_init ============================= */
BOOL chain_io_init (BOOL  mit_logging)
{
   chainfile_open     = FALSE;
   chio_logging       = mit_logging;
   initialChrFileMode = FILE_BIN_IO;
   atexit (chain_io_exit);                  /* Etablierung des Exit-Handlers  */
#ifdef MPI_PAR_VERS
   return (initMPI_Chio());
#else
   return (TRUE);
#endif
} /* chain_io_init */
