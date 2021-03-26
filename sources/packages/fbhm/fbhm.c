/*******************************************************************************
GLEAM/AE                                                                  V2.0.1

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                    Package: FehlerBeHandlung und Meldungen
                     Module: FehlerBeHandlung und Meldungen
 
Package: fbhm                     File: fbhm.c
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Das Package stellt 
  - eine auf Textdateien basierende Fehler- und Meldungsbehandlung und
  - einen mehrzeiligen Textpuffer der Groesse MAX_BUF_ANZ 
zur Verfuegung. 
Es enthaelt einige Routinen, die nur im TEXT_MODE zur Verfuegung stehen und 
nachfolgend als solche gekennzeichnet sind.


Fehler- und Meldungsbehandlung:
===============================
Es gibt 3 Meldungskategorien: Meldung, Fehler (korrespon-
diert mit GLEAM_ERROR) und fatale Fehler (korrespondiert mit GLEAM_FATAL). 
Fehler der Klasse GLEAM_ERROR koennen vom Benutzer behoben werden, Fehler der 
Klasse GLEAM_FATAL fuehren dagegen zum Programmabbruch. Die Codes sind so auf-
gebaut, dass sie bitweise geodert werden koennen: GLEAM_ERROR | GLEAM_FATAL 
ergibt GLEAM_FATAL. Ein Aufruf von "fehler" und "fatal" liefert den jeweiligen 
Code zurueck, sodass die aufrufende Software (beim Aufruf von "fehler" mittels 
des bitweisen OR-Operators) daraus einen Returnstatus aufbauen kann.

Das Packages verwaltet einen internen Fehlerstatus, der entsprechend der aufge-
rufenen Routinen gesetzt bzw. zurueckgesetzt wird. Es gibt folgende Moeglich-
keiten:
             GLEAM_OK  <--->  GLEAM_ERROR  --->  GELAM_FATAL

Die globale Variable "err_state" wird nur von den Routinen "get_fbuf_data", 
und "show_and_clear_fbuf" aktualisiert. Ansonsten obliegt die Pflege der 
aufrufenden SW. 

"fbhmInit" etabliert einen Exithandler, der bei Programmbeendigung eventuell 
noch nicht ausgegebene Meldungen ueber "ctio"-Routinen ausgibt. Entsprechendes 
gilt auch fuer Fehlermeldungen, die durch eine fehlerhafte Fehlertextdatei 
entstehen.

Die Meldungsdaten werden von den drei Meldungsroutinen zusammen mit einem Time-
stamp in einen Puffer geschrieben. Wenn dieser voll ist, werden weitere Meldungen
ignoriert, aber mitgezaehlt. Der Puffer kann mittels "get_1msg" oder "show_and_
clear_fbuf" (TEXT_MODE-Variante) nach dem fifo-Prinzip geleert werden. Dabei 
werden die Meldungen zusammen mit dem Time-Stamp ins Logfile geschrieben, falls 
zumindest ein fataler Fehler vorliegt oder falls "log_all_msgs" TRUE ist. Mit 
Hilfe von "get_fbuf_data" kann man neben dem internen Status auch die Meldungs-
anzahl abfragen, um z.B. erkennen zu koennen, ob Meldungen, die ja keinen Status 
hinterlassen, vorhanden sind.

Die Meldungen haben folgenden Aufbau:
    *** <Zeitstempel> FATAL (<modul>/<xxx>): <text>
    *** <Zeitstempel> ERROR (<modul>/<xxx>): <text>
    *** <Zeitstempel> Meldung (<modul>): <text>

Dabei steht <modul> fuer die maximal 9-stellige Package-/Modulkennung (Text), 
<xxx> fuer die maximal 3-stellige Herkunfts-Id (Integer) und <text> fuer einen 
Text, der zusammen mit den optionalen Zusatzinfos (Integer und String) maximal 
255 Zeichen lang sein kann. Der Text stammt aus der Initialisierungsdatei 
"ftext_<sp>.txt", wobei <sp> die verwendete Sprache angibt. Die Zusatzinfor-
mation wird entsprechend den Formatangaben im zu "fnr" bzw "mnr" gehoerigen 
String eingebaut. Dabei wird unterstellt, dass der Meldungstext nur maximal 
eine String- und bis zu zwei Integer-Formatangaben enthaelt. Die Position der 
Stringangabe relativ zu den beiden anderen moeglichen Formatangaben wird 
ermittelt. Wenn kein String in der Massage angegeben ist, wird "sprintf" auch 
nur mit den beiden "info"-Parametern aufgerufen. Ansonsten so, dass der 
"info_buf" an der richtigen Stelle steht. Die Reihenfolge der beiden "info"-
Parameter ist immer "info1" gefolgt von "info2". 
VORSICHT: Meldungstexte, die mehr als ein "%s"-Format und mehr als zwei "%ld"-
          Formate enthalten, verursachen einen Programmabsturz! 

Parallelversion:
"fbhmInit" liest die Standardfehlertexte (FTEXTE_FNAME) und eventuelle zusaetz-
liche - je nach ADD_FTEXTE FNAME - ein und belegt damit den internen Speicher 
"ftexte".'In "akt_text_anz" steht die Anzahl insgesamt gelesener Texte. In der 
Parallelversion, siehe MPI_PAR_VERS, werden anschliessend die in der durch 
FTEXTEPAR_FNAME bezeichneten Datei stehenden Fehlertexte gelesen. Damit ergibt sich fuer 
"ftexte" folgendes Bild:
                   =
                 0 |    }
                  ...   }  Bereich der Standard- und zusaetzlichen Fehlertetxte
                   |    }
                   -
  ftextpar_begin > |    }
                  ...   }  Bereich der Fehlertetxte der Parallelversion
                   |    }
                   -
    akt_text_anz > |    }
                  ...   }  ungenutzter Bereich
                   |    }
  MAX_ANZ_FTEXTE > =

Die Meldungsnummern der Parallelversion beginnen bei PAR_FNR_BEGIN und werden 
auf den fuer sie vorgesehenen Bereich abgebildet.


Textpuffer:
===========
Der mehrzeilige Textpuffer erlaubt es den Standardmodulen der Packages Ausgaben 
vorzubereiten. Unter Standardmodule werden alle verstanden, die die (Hy)GLEAM-
Funktionalitaet realisieren und nichts mit einer wie auch immer gearteten Be-
nutzerführung zu tun haben. Also fast alles, was zur Engine gehoert.
Die Routinen zur Ausgabevorbereitung eines Packages werden haeufig in jeweils 
einem eigenen Modul zusammengefasst. Die Routinen des Packages fuellen den mehr-
zeiligen Puffer, der durch Routinen einer uebergeordneten Userinterface-Schicht 
oder durch entsprechende Routinen der Engine ausgegeben werden kann.
Das Puffer-Feld "text_buf" besteht aus MAX_BUF_ANZ Zeilen. Dieser Wert richtet 
sich nach der maximal erwarteten Anzahl an Segmenten (SEGM_ANZ_MAX), welche 
durch den Ketteneditor "chedi" dargestellt werden sollen. AKs mit mehr Segmenten 
sind trotzdem moeglich, aber nicht durch den Ketteneditor behandelbar.
Geschrieben wird mittels der Routine "write_buf". Dabei koennen auch durch ge-
eignete Vorgaben von Zeilennummern Leerzeilen implizit entstehen. Ferner stehen 
Routinen zur Ausgabe des Puffers auf das Terminal und ins Logfile zur Verfuegung.


Die Routinen der Fehler- und Meldungsbehandlung im Einzelnen:
-------------------------------------------------------------

void meldung (const MOD_STR_TYPE  modul,
              LONG                info1,
              LONG                info2,
              const STR_TYPE      info_buf,
              INT                 mnr);
     Speichert die durch die Parameter spezifizierte Meldung im internen Fehler-
     puffer. Bei Pufferueberlauf wird die Meldung ignoriert und in "skipped_msg_
     ctr" mitgezaehlt.

GSTATE fehler (const MOD_STR_TYPE  modul,
               INT                 herkunft,
               LONG                info1,
               LONG                info2,
               const STR_TYPE      info_buf,
               INT                 fnr);
     Speichert die durch die Parameter spezifizierte Meldung im internen Fehler-
     puffer und liefert GLEAM_ERROR zurueck. Der interne Fehlerstatus "error_
     state" wird auf "error_state | GLEAM_ERROR" gesetzt. Bei Pufferueberlauf 
     wird die Meldung ignoriert und in "skipped_msg_ctr" mitgezaehlt.

GSTATE fatal (const MOD_STR_TYPE  modul,
              INT                 herkunft,
              LONG                info1,
              LONG                info2,
              const STR_TYPE      info_buf,
              INT                 fnr);
     Speichert die durch die Parameter spezifizierte Meldung im internen Fehler-
     puffer und liefert GLEAM_FATAL zurueck. Der interne Fehlerstatus "error_
     state" wird auf GLEAM_FATAL gesetzt. Bei Pufferueberlauf wird die Meldung 
     ignoriert und in "skipped_msg_ctr" mitgezaehlt.

INT get_fbuf_data (GSTATE *state);
     Liefert die Anzahl der im Fehlerpuffer stehenden Eintraege als Funktions-
     ergebnis und in "state" den aktuellen Fehlerstatus ab. Wenn ein Puffer-
     ueberlauf stattgefunden hat ("skipped_msg_ctr" > 0), wird MAX_ANZ_MSG+1
     abgeliefert. "err_state" wird aktualisiert.

BOOL get_1msg (char *mbuf);
     Entfernt den aeltesten Eintrag im Fehlerpuffer, baut den dazugehoerigen 
     Meldungsstring auf und liefert ihn in "mbuf" ab. Das Funktionsergebnis ist
     TRUE, wenn in "mbuf" ein gueltiger Meldungstext steht. Mit Auslesen der 
     letzten Meldung wird der internen Fehlerstatus zurueckgesetzt, sofern er 
     nicht GLEAM_FATAL ist. Wenn der Puffer leer und "skipped_msg_ctr" > 0 ist, 
     liefert die Routine TRUE und in "mbuf" eine Meldung ueber die Anzahl igno-
     rierter Meldungen ab und setzt "skipped_msg_ctr" auf 0. Wenn dagegen der 
     Puffer leer und "skipped_msg_ctr" Null ist, liefert die Routine FALSE und 
     "mbuf" bleibt unveraendert. Wenn der interne Fehlerstatus GLEAM_FATAL ist
     oder "fbhmInit" mit TRUE aufgerufen wurde, werden die Meldungen mit ihrem 
     Timestamp auch ins Logfile geschrieben.

void clear_fbuf (void);
     Loescht den Fehlerpuffer und setzt den internen Fehlerstatus zurueck, so-
     fern er nicht GLEAM_FATAL ist. "err_state" wird aktualisiert.

void clear_err_state (void);
     Setzt den internen Fehlerstatus zurueck, sofern er nicht GLEAM_FATAL ist. 
     "err_state" wird aktualisiert. 

BOOL log_msgs (void);
     Leert den (Fehler-)Meldungspuffer und schreibt die Meldungen in das Log-
     file, sofern der interne Fehlerstatus GLEAM_FATAL ist oder "fbhmInit" mit 
     TRUE aufgerufen wurde (durch Aufruf von "get_1msg). 
     Die Routine liefert TRUE, wenn der Fehlerstatus NICHT GLEAM_OK ist. 

void show_and_clear_fbuf (BOOL  logAllMsgs);        Nur im TEXT_MODE!
     Gibt alle Meldungen des Fehlerpuffers incl einer evtl. Ueberlaufmeldung 
     aus, setzt "skipped_msg_ctr" auf Null und den internen Fehlerstatus zu-
     rueck, sofern er nicht GLEAM_FATAL ist. "err_state" wird aktualisiert. 
     Wenn der interne Fehlerstatus GLEAM_FATAL ist oder "fbhmInit" mit TRUE 
     aufgerufen wurde oder "logAllMsgs" TRUE ist, werden die Meldungen mit 
     ihrem Timestamp auch ins Logfile geschrieben. Wenn der Puffer leer ist, 
     geschieht nichts.
     Die Ausgabe erfolgt bei einer Meldung in einer Zeile. Bei mehreren Meldungen 
     erfolgt eine Sammelmeldung mit Anzahl der Meldungen und dem Fehlerstatus. 
     Es besteht nun die Moeglichkeit, die Meldungen entweder zu ignorienen (Quitt), 
     sie in einen kleinen Scrollbereich im Textfenster auszugeben (Display) oder
     einzeln (auch mehrfach) in die Meldungszeile (Scroll). Letzteres kann durch 
     Quitt abgebrochen werden.


Die Routinen der Textpuffer-Verwaltung im Einzelnen:
----------------------------------------------------

BOOL write_buf (INT         line_nr,
                const char *buf);
     Sofern "line_nr" zwischen 1 und MAX_BUF_ANZ liegt, schreibt die Routine
     den Puffer "buf" in den durch "line_nr" angegebenen Puffer, wobei der 
     Puffer-Index = line_nr - 1 ist. Dabei wird der interne Zaehler "line_anz"
     gepflegt. Dieser gibt die groesste Zeilennummer an, in die seit dem letzten 
     "reset_text_buf" etwas geschrieben wurde. Die Laenge einer Zeile wird ohne
     Fehlermeldung auf MAX_STRING Zeichen begrenzt.
     Bei korrekter "line_nr" liefert die Routine TRUE, sonst FALSE und eine 
     Fehlermeldung (GLEAM_ERROR).

BOOL get_buf (INT   line_nr,
              char *buf);
     Die Routine liefert in "buf" den durch "line_nr" bezeichneten Inhalt des
     Textpuffers ab, sofern "line_nr" kleiner als MAX_BUF_ANZ ist. Wenn "line_nr" 
     zwischen 1 und "line_anz" liegt, liefert die Routine dabei  TRUE sonst FALSE 
     ab. Bei einer "line_nr" groesser als MAX_BUF_ANZ erzeugt sie eine Fehler-
     meldung (GLEAM_ERROR), liefert FALSE und einen unveraenderten "buf" ab.

void clear_buf (INT  line_nr);
     Der durch "line_nr" angegebenen Puffer wird geloescht. Ab "line_nr" wird 
     geprueft, ob "line_anz" gepflegt werden muss. Bei einer "line_nr" groesser 
     als MAX_BUF_ANZ erzeugt sie eine Fehlermeldung (GLEAM_ERROR).

INT get_line_anz (void);
     Liefert "line_anz" ab.

void reset_text_buf (void);
     Setzt "line_anz" auf 0 und loescht alle Strings in "text_buf".

void logTextBuffer  (BOOL  mitTerm);
     Leert den Textpuffer des Packages "fbhm" in das das Logfile, so es angelegt 
     ist, und schreibt ihn bei gesetztem "mitTerm" auch auf das Terminal. 
     Der Textpuffer wird in jedem Fall zurueckgesetzt.

void write_text_buf (BOOL  del_buf);        Nur im TEXT_MODE!
     Der gesamte Inhalt des Textpuffers wird bis "line_anz" auf das Terminal 
     ausgegeben. Dabei kann die Ausgabe alle MAX_LINE Zeilen unterbrochen und 
     kann fortgesetzt oder abgebrochen werden. 
     Nach erfolgter Ausgabe wird der Textpuffer mit "reset_text_buf" geloescht, 
     sofern "del_buf" gesetzt ist. 


BOOL fbhmInit (BOOL msg_log);
     Modulinitialisierung. Liefert TRUE, wenn der Speicher ausreicht und alles 
     gut ging, sonst FALSE.
     Zunaechst werden alle Fehlerstrings mit "Nichtinitialisierter Meldungstext" 
     vorbesetzt. Der interne Fehlerstatus wird zurueck und "skipped_msg_ctr" 
     auf 0 gesetzt. Die interne Variable "log_all_msgs" erhaelt den Wert von 
     "msg_log".
     Etabliert den Exithandler "fbhm_exit".
     Beim Oeffnen der Fehlertextdateien wird bei deren Nichtexistenz im Arbeits-
     verzeichnis geprueft, ob die durch GLEAM_ROOT_ENV bezeichnetze Umgebungs-
     variable existiert. Wenn ja, werden die Dateien dort gesucht. Zunaechst 
     wird die durch "FTEXTE_FNAME" bezeichnete Fehlermeldungsdatei eingelesen.
     Wenn dies nicht moeglich ist, liegt ein fataler Fehler vor. Prueft, ob die 
     durch "ADD_FTEXTE_FNAME" bezeichnete Fehlermeldungsdatei existiert und liest 
     positivenfalls die darin gespeicherten Fehlertexte zusaetzlich ein. 
     Bei gesetztem MPI_PAR_VERS wird auch die durch "FTEXTEPAR_FNAME" bezeichnete 
     Fehlermeldungsdatei eingelesen. Der Pfad ergibt sich aus der Environment-
     variable, deren Name in GLEAM_ROOT_ENV hinterlegt ist.

--------------------------------------------------------------------------------
basiert auf C-Unit: fatal
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. Erweiterungen fuer die Parallel-Version.
 2. Applikationsspezifische Erweiterung fuer die ADD_FTEXTE_FNAME-Datei. 
 3. Vergroesserung der Fehlermeldungspuffer in "make_meldung".
 4. "log_msgs" hinzugefuegt.                                         WJ 08.01.08
 5. Umstellung der Verarbeitung der Zusatinformationen "info1", 
    "info2" und "info_buf", derart, dass keine Programmabstuerze 
    mehr erfolgen, solange die Meldungstexte nicht mehr als einen
    String- und zwei Integer-Formatangaben enthalten.                WJ 12.08.08
 6. "fbhmInit": Zugriff auf die Fehlertextfiles ueber GLEAM_ROOT.    WJ 28.08.09
 7. Deklaration von Stringparameter, die reine Eingangsparameter 
    sind, als "const".                                               WJ 18.04.12
 8. Entfernung von BGI_MODE plus Code in "fbhm_exit", von GLEAM_TCL 
    samt zugehoerigem Code, von "get_fbuf_state" und "get_fbuf_size".
    GLEAM_C --> TEXT_MODE: "show_and_clear_fbuf". Streichung ueber-
    fluessiger xxx_window-Aufrufe aus ctio. "fbhm_exit" angepasst.   WJ 28.06.12
 9. Erweiterungen fuer die MPI-Parallelversion.                      WJ 09.11.12
10. "lies_ftext_file:" Meldung bei fehlender Fehlertextdatei in den
    den Textpuffer. Bei TEXT_MODE: Ausgabe auch auf den Schirm.      WJ 13.11.14
11. Renovierung: Korrektur des fgets-Aufrufs. Integration der Routi-
    nen zur Verwaltung des Textpuffers (ehemaliges Package "link").
    Streichung des "chaindef"-Includes und "write_line"-Aufrufe.
    "show_and_clear_fbuf" ueberarbeitet.                             WJ 09.05.16
12. Verbesserte Fehlerbehandlung in "lies_ftext_file".               WJ 01.07.16
13. Anpassung von MSG_HDR_TXT fuer GUI.                              WJ 02.08.16
14. Begrenzung der Zeilenlaenge des "text_buf" auf MAX_STRING.       WJ 11.11.16
15. Aus MOD_FILE_IDENT wird PROG_IDENT. Anpassung des Aufrufs von
    "write_to_log". Verbesserte Meldungsanzeige ("make_meldung").    WJ 17.01.17
16. Verbesserung der Fehlermeldungen in "lies_ftext_file" und neue
    Warnung bei fehlendem Zusatzfehlertextefile, obwohl eines ange-
    geben ist. Einfuehrung von "textBufOK".                          WJ 05.04.17
17. Neuer Parameter fuer "show_and_clear_fbuf". MAX_ANZ_MSG etwas 
    vergroessert.                                                    WJ 17.11.17
18. "fbhmInt.h" gestrichen.                                          WJ 01.12.17
19. fbhmInit() gibt eventuelle Felhlermeldungen auf stderr aus.      WJ 06.04.18
20. New variable "numberOfStdErrMsgs".                               WJ 04.06.18
21. fbhm_exit() loescht die durch EVO_ABBRUCH_FSPEC bezeichnete 
    Datei, wenn sie existiert.                                       WJ 20.08.18
22. fbhmInit() wertet beim Oeffnen der Fehlertextdateien die in 
    GLEAM_ROOT_ENV bezeichnete Umgebungsvariable aus.                WJ 23.08.18
23. Korrektur 21 nur, wenn ESS_EVAL_SLAVE nicht gesetzt ist.         WJ 10.10.18
24. make_meldung(), lies_ftext_file(), fbhmInit(): Avoiding buffer 
    overflow of static strings.                                      WJ 16.08.20
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 16.08.2020
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#ifdef MPI_PAR_VERS
  #include "mPar.h"                   /* benoetigt Deklarationen aus "fbhm.h" */
#endif /* MPI_PAR_VERS */

#include GLOBAL_TEXTS
#include FBHM_TEXTS


#undef TESTS



/* ========================== Exportierte Variable: ========================= */
GSTATE  err_state = GLEAM_FATAL;      /* Statusvariable fuer Fehler-Level     */
INT     numberOfStdErrMsgs;           /* number of standard error messages    */
BOOL    textBufOK = FALSE;            /* TRUE if the text buffer is available */


/* ============================ Lokale Konstante: =========================== */
#define MAX_ANZ_MSG         30  /* Groesse des Fehlerpuffers                  */
#define MAX_ANZ_FTEXTE     300  /* Max.Anz der Fehlermeldungstexte            */



/* ======================== Lokale Typdeklarationen: ======================== */
typedef struct MSG_BUF_TYPE
  { MOD_STR_TYPE  modul_id;
    INT           herkunft,
                  meld_nr;
    LONG          info1,
                  info2;
    GSTATE        meld_typ;
    STR_TYPE      info_buf;
    time_t        time_stamp;
  } MSG_BUF_TYPE;



/* ============================ Lokale Variable: ============================ */
static char         *ftexte     [MAX_ANZ_FTEXTE];   /* Fehlermeldungstexte    */
static MSG_BUF_TYPE *msg_buffer;           /* Meldungspuffer                  */
static LONG          msg_ind;              /* MldgAusgabe: aktueller Index    */
static INT           msg_ctr,              /* Anzahl gespeicherter  Meldungen */
                     skipped_msg_ctr,      /* Anzahl ignorierter Meldungen    */
                     akt_text_anz,         /* Anz. eingelesener Fehlertexte   */
                     ftextpar_begin;       /* 1.Fehlertext aus FTEXTEPAR_FNAME*/
static GSTATE        error_state;          /* interner Fehlerstatus           */
static BOOL          log_all_msgs = FALSE; /* Meldungsausgabe ins Logfile     */

static char         *text_buf [MAX_BUF_ANZ];/*Textpuffer-Feld                 */
static INT           line_anz;             /* Anzahl beschriebener Zeilen     */



/* ============================ Lokale Routinen: ============================ */
static void   store_mldg      (const MOD_STR_TYPE  modul,
                               INT                 herkunft,
                               INT                 mnr,
                               GSTATE              mtyp,
                               LONG                info1,
                               LONG                info2,
                               const STR_TYPE      info_buf);

static BOOL   make_meldung     (char              *mbuf,
                                BOOL               clear_buf);

static void   fbhm_exit        (void);

#ifdef MPI_PAR_VERS
  static void send2Master      (GSTATE  errState);
#endif /* MPI_PAR_VERS */

static BOOL   lies_ftext_file  (char   *file_name);




/* =============================== store_mldg =============================== */
static void store_mldg (const MOD_STR_TYPE  modul,
                        INT                 herkunft,
                        INT                 mnr,
                        GSTATE              mtyp,
                        LONG                info1,
                        LONG                info2,
                        const STR_TYPE      info_buf)
/*----------------------------------------------------------------------------*/
/*  Traegt die Meldungsdaten zusammen mit einem Time-Stamp in den Meldungs-   */
/*  puffer ("msg_buffer") ein, sofern noch Platz ist. Anonsten wird die Mel-  */
/*  dung ignoriert und "skipped_msg_ctr" inkrementiert. In "msg_ctr" wird     */
/*  die Anzahl abgespeicherter Meldungen mitgezaehlt.                         */
/*----------------------------------------------------------------------------*/
{
   msg_ind = NO_INFO;                 /* indicate 1st call for Tcl/Tk variant */
   if (msg_ctr == MAX_ANZ_MSG)
      skipped_msg_ctr++;
   else
   { /* Meldung mit TimeStamp speichern */
      msg_buffer [msg_ctr].herkunft   = herkunft;
      msg_buffer [msg_ctr].meld_nr    = mnr;
      msg_buffer [msg_ctr].meld_typ   = mtyp;
      msg_buffer [msg_ctr].info1      = info1;
      msg_buffer [msg_ctr].info2      = info2;
      msg_buffer [msg_ctr].time_stamp = time (NULL);
      strcpy (msg_buffer [msg_ctr].modul_id, modul);
      strcpy (msg_buffer [msg_ctr].info_buf, info_buf);
      msg_ctr++;
   } /* Meldung mit TimeStamp speichern */
} /* store_mldg */ 



/* ============================== make_meldung ============================== */
static BOOL make_meldung (char *mbuf,
                          BOOL  clear_buf)
/*----------------------------------------------------------------------------*/
/*  Baut aus den Daten der juengsten Meldung den zugehoerigen String auf und  */
/*  liefert ihn in "mbuf" ab. Solange es noch weitere Meldungen im Fehler-    */
/*  puffer gibt, ist das Funktionsergebnis TRUE. Im Falle eines Fehlerpuffer- */
/*  ueberlaufs wird nach der Meldungsausgabe noch eine entsprechende Meldung  */
/*  erzeugt. Durch Setzen von "clear_buf" wird mit Ausgabe der letzten        */
/*  Meldung der Fehlerpuffer geloescht.                                       */
/*  Details der Implementierung:                                              */
/*  Wenn der "msg_buffer" leer und "skipped_msg_ctr" == 0 ist, ist das Funk-  */
/*  tionsergebnis FALSE und "mbuf" unveraendert. Wenn der "msg_buffer" leer   */
/*  und "skipped_msg_ctr" groesser als Null ist, wird eine Meldung mit        */
/*  "skipped_msg_ctr" generiert, "skipped_msg_ctr" auf Null gesetzt und TRUE  */
/*  abgeliefert. Wenn der "msg_buffer" nicht leer ist, wird der Meldungs-     */
/*  string abgeliefert und das Funktionsergebnis ist TRUE. Ausserdem wird bei */
/*  Ausgabe der letzten Meldung der Meldungspuffer durch Setzten von "msg_    */
/*  ctr" auf Null geloescht, sofern "clear_buf" TRUE ist. Wenn "clear_buf"    */
/*  gesetzt ist, wird bei internem fatalen Fehlerstatus oder bei gesetztem    */
/*  "log_all_msgs" die Meldung auch in das Logfile geschrieben. Die Bedingung */
/*  "clear_buf" verhindert, dass Meldungen mehrfach ins Logfile geschrieben   */
/*  werden.                                                                   */
/*----------------------------------------------------------------------------*/
{
   char         logbuf[2*MAX_STRING];
   STRING_TYPE  buf;
   INT          msg_txt_nr, msg_txt_ind, strPos, ii;
   time_t       act_time;
   char        *zbuf;
   char        *msgRest, *msg;
   BOOL         skip_msg = FALSE,
                gefunden = TRUE;

   /* make_meldung */
   if (((msg_ctr == 0) || (msg_ind == msg_ctr)) && (skipped_msg_ctr == 0))
      return (FALSE);
   else
   { /* Es gibt noch Meldungen oder ignorierte Meldungen */
      if (msg_ctr > 0)
      { /* Meldungstext aufbauen und Pufferfuellstand pflegen */
         switch (msg_buffer [msg_ind].meld_typ)                /* Meldungstyp */
         {
            case GLEAM_OK:
               strcpy (mbuf, MLDG_TXT);  break;
            case GLEAM_ERROR:
               strcpy (mbuf, ERROR_TXT); break;
            case GLEAM_FATAL:
               strcpy (mbuf, FATAL_TXT); break;
            default:
               strcpy (mbuf, NIX_TXT);   break;
         } /* CASE (meld_typ) */
#ifdef MPI_PAR_VERS
	 if (master)
	    strcat (mbuf, MASTER_TXT);
	 else { /* Slave */
	    sprintf (buf, SLAVE_TXT, kNr);
	    strcat (mbuf, buf);
	 } /* Slave */
#endif /* MPI_PAR_VERS */
         strcat (mbuf, msg_buffer [msg_ind].modul_id);       /* Modul-Kennung */
         if (msg_buffer [msg_ind].meld_typ != GLEAM_OK)      /* Herkunft:     */
            sprintf (buf, "/%-2d): ", msg_buffer [msg_ind].herkunft);
         else
            strcpy (buf, "): ");
         strcat (mbuf, buf);
         msg_txt_nr = msg_buffer[msg_ind].meld_nr;
#ifdef MPI_PAR_VERS
         if (msg_txt_nr >= PAR_FNR_BEGIN)
	    msg_txt_ind = msg_txt_nr - PAR_FNR_BEGIN + ftextpar_begin;
	 else
#endif /* MPI_PAR_VERS */
	    msg_txt_ind = msg_txt_nr - 1;
         if ((msg_txt_ind < 0) || (MAX_ANZ_FTEXTE <= msg_txt_ind))
            sprintf (buf, BAD_MSGNR_TXT, msg_txt_nr);
         else
         { /* Gueltige Meldungs-Nummer */
#ifdef TESTS
            printf ("make_meldung: msg_txt_nr=%d  msg_txt_ind=%d\n", msg_txt_nr, msg_txt_ind);
#endif /* TESTS */
            strPos = 0;
            msg    = ftexte[msg_txt_ind];
	    for (ii = 0;  (ii < 3) && gefunden;  ii++) 
	    { /* Suche Stringposition */
	       msgRest = strchr (msg, '%');
	       if (msgRest) { /* %-Zeichen gefunden */
		  if (msgRest[1] == 's')
		     strPos = ii + 1;
		  msg = msgRest + 1;
	       } /* %-Zeichen gefunden */
	       else
		  gefunden = FALSE;
	    } /* Suche Stringposition */
	    switch (strPos) {
	    case 0: /* --------------- Meldung ohne Text-Info --------------- */
	       sprintf (buf, ftexte[msg_txt_ind], msg_buffer[msg_ind].info1,
			     msg_buffer[msg_ind].info2);
	       break;

	    case 1: /* ---------- Meldung mit Text-Info an 1.Stelle --------- */
	       sprintf (buf, ftexte[msg_txt_ind], msg_buffer[msg_ind].info_buf,
			msg_buffer[msg_ind].info1, msg_buffer[msg_ind].info2);
	       break;

	    case 2: /* ---------- Meldung mit Text-Info an 2.Stelle --------- */
	       sprintf (buf, ftexte[msg_txt_ind], msg_buffer[msg_ind].info1, 
			msg_buffer[msg_ind].info_buf, msg_buffer[msg_ind].info2);
	       break;

	    case 3: /* ---------- Meldung mit Text-Info an 3.Stelle --------- */
	       sprintf (buf, ftexte[msg_txt_ind], msg_buffer[msg_ind].info1, 
			msg_buffer[msg_ind].info2, msg_buffer[msg_ind].info_buf);
	       break;

	    default: /* ---------- Meldung mit Text-Info an 1.Stelle --------- */
	       sprintf (buf, BAD_MSG_TXT, msg_ind);
	    } /* esac */
         } /* Gueltige Meldungs-Nummer */
         strcat (mbuf, buf);

         msg_ind++;
         if ((msg_ind == msg_ctr) && clear_buf)
            msg_ctr = 0;              /* Ausgelesenen Meldungspuffer loeschen */
      } /* Meldungstext aufbauen und Pufferfuellstand pflegen */
      else
      { /* Anzahl ignorierter Meldungen ausgeben */
         sprintf (mbuf, SKIP_TXT, skipped_msg_ctr);
         skipped_msg_ctr = 0;
         skip_msg        = TRUE;
      } /* Anzahl ignorierter Meldungen ausgeben */
      if (clear_buf && (log_all_msgs || (error_state == GLEAM_FATAL)))
      { /* Meldung auch ins Logfile schreiben */
         if (skip_msg)
         {
            act_time = time (NULL);
            zbuf     = ctime (&act_time);
         }
         else
            zbuf = ctime (&(msg_buffer[msg_ind-1].time_stamp));
         zbuf [24] = '\0';
         strncpy (buf, &(zbuf[4]), 21);
         sprintf (logbuf, "*** %s %s", buf, mbuf);
         write_to_log (logbuf, CLOSE_LOG);
      } /* Meldung auch ins Logfile schreiben */
      return (TRUE);
   } /* Es gibt noch Meldungen oder ignorierte Meldungen */
} /* make_meldung */ 



/* ================================ fbhm_exit =============================== */
static void fbhm_exit (void)
{
   STRING_TYPE  buf;
   
   /* fbhm_exit */
#ifndef ESS_EVAL_SLAVE
   /* ------ Beseitigung einer eventuell vorhandenen EVO-Abbruch-Datei: ----- */
   if (file_exists(EVO_ABBRUCH_FSPEC))
      if (remove(EVO_ABBRUCH_FSPEC) == -1) 
      { /* file administration failed */
	 sprintf(buf, "*** Failed to delete stop file \"%s\": %s", EVO_ABBRUCH_FSPEC, strerror(errno));
	 write_to_log(buf, CLOSE_LOG);
      } /* file administration failed */
#endif /* no ESS_EVAL_SLAVE */

   /* ------------------------ Ausgabe von Meldungen: ----------------------- */
   if (msg_ctr > 0)
   { /* Es sind Meldungen vorhanden */
      sprintf (buf, PROG_FIN_TXT, PROGRAM_NAME);
      write_to_log (buf, CLOSE_LOG);
#if defined(TEXT_MODE)
  #if defined(MPI_PAR_VERS)
      if (master) { /* Master */
	 printf ("%s\n", buf);
	 show_and_clear_fbuf(LOG_ALL_MSGS);
      } /* Master */
      else
	 log_msgs ();                /* Slave: (Fehler-)Meldungen ins LogFile */
  #else /* TEXT_MODE aber keine MPI_PAR_VERS */
      printf ("%s\n", buf);
      show_and_clear_fbuf(LOG_ALL_MSGS);
  #endif /* TEXT_MODE aber keine MPI_PAR_VERS */
#else /* Engine oder GUI */
      log_msgs ();                          /* (Fehler-)Meldungen ins LogFile */
#endif /* Engine oder GUI */
   } /* Es sind Meldungen vorhanden */

   /* -------------------------- Schlussmeldungen: -------------------------- */
#ifdef MPI_PAR_VERS
   if (master)
      printf (PROG_ENDE_TXT, PROGRAM_NAME);
#else
   printf (PROG_ENDE_TXT, PROGRAM_NAME);
#endif
} /* fbhm_exit */ 



#ifdef MPI_PAR_VERS
/* =============================== send2Master ============================== */
static void send2Master (GSTATE  errState)
/*----------------------------------------------------------------------------*/
/*  Bei Slaves veranlasst die Routine die Versendung der Fehlermeldung an den */
/*  Master. Danach erfolgt der Zustandswechsel entsprechend dem aktuellen     */
/*  Status des Slaves.                                                        */
/*  Beim Master macht die Routine nichts.                                     */
/*----------------------------------------------------------------------------*/
{
   STR_TYPE  tmpBuf;                       /* Temporaerer Textpuffer          */

   /* printf ("Prozess %d:  master = %d\n", kNr, master);   /* test */
   if (!master) { /* only slaves do that */
      get_1msg (&(tmpBuf[1]));
      tmpBuf[0] = errState + STATE_OFFSET;        /* 0 ist nicht uebertragbar */
      sendS2M_Error (tmpBuf);
      if (nodeState < EXP_INIT_ERR) /* Slave nicht in Fehlerzustand? */
	 switch (nodeState)
         { /* Wechsel je nach aktuellem Status des Slaves */
            case EXP_INIT_BUSY:
            case EXP_INIT:          changeState (EXP_INIT_ERR); break;

            case POP_INIT_BUSY:
	    case POP_INIT:          changeState (POP_INIT_ERR); break;

            case EVO_INIT_BUSY_GEN:
            case EVO_INIT_BUSY_REC: changeState (EVO_INIT_ERR); break;

            case EVO_BUSY:
	    case EVO_DONE:          changeState (EVO_ERR);      break;

            default:
	       sprintf (tmpBuf, BAD_STATE_TXT, nodeBuf, nodeState);
	       write_to_log (tmpBuf, CLOSE_LOG);
         } /* Wechsel je nach aktuellem Status des Slaves */
   } /* only slaves do that */
} /* send2Master */
#endif /* MPI_PAR_VERS */



/* ================================= meldung ================================ */
void meldung (const MOD_STR_TYPE  modul,
              LONG                info1,
              LONG                info2,
              const STR_TYPE      info_buf,
              INT                 mnr)
{
   store_mldg (modul, NO_INFO, mnr, GLEAM_OK, info1, info2, info_buf);
#ifdef MPI_PAR_VERS
   send2Master (GLEAM_OK);
#endif /* MPI_PAR_VERS */
} /* meldung */ 



/* ================================= fehler ================================= */
GSTATE fehler (const MOD_STR_TYPE  modul,
               INT                 herkunft,
               LONG                info1,
               LONG                info2,
               const STR_TYPE      info_buf,
               INT                 fnr)
{
   /* fehler */
   store_mldg (modul, herkunft, fnr, GLEAM_ERROR, info1, info2, info_buf);
   error_state = error_state | GLEAM_ERROR;
#ifdef MPI_PAR_VERS
   send2Master (GLEAM_ERROR);
#endif /* MPI_PAR_VERS */
   return (GLEAM_ERROR);
} /* fehler */ 



/* ================================== fatal ================================= */
GSTATE fatal (const MOD_STR_TYPE  modul,
              INT                 herkunft,
              LONG                info1,
              LONG                info2,
              const STR_TYPE      info_buf,
              INT                 fnr)
{
   /* fatal */
   store_mldg (modul, herkunft, fnr, GLEAM_FATAL, info1, info2, info_buf);
   error_state = GLEAM_FATAL;
#ifdef MPI_PAR_VERS
   send2Master (GLEAM_FATAL);
#endif /* MPI_PAR_VERS */
   return (GLEAM_FATAL);
} /* fatal */ 



/* ============================== get_fbuf_data ============================= */
INT get_fbuf_data (GSTATE *state)
{
   *state    = error_state;
   err_state = error_state;
   if (skipped_msg_ctr > 0)
      return (MAX_ANZ_MSG + 1);
   else
      return (msg_ctr);
} /* get_fbuf_data */ 



/* ============================== lies_ftext_file =========================== */
static BOOL lies_ftext_file (char *file_name)
/*----------------------------------------------------------------------------*/
/*  Oeffnet die durch "file_name" bezeichnete Datei, prueft ihre Identitaet   */
/*  und liest die darin enthaltenen Meldungstexte ein. Im Fehlerfalle werden  */
/*  Meldungen in den Textpuffer ab der ersten freien Zeile geschrieben. In    */
/*  der modul-lokalen Variable "akt_text_anz" werden die eingelesenen Mel-    */
/*  dungen mitgezaehlt. Am Schluss wird das File geschlossen und TRUE abge-   */
/*  liefert, wenn alles gut ging, sonst FALSE.                                */
/*----------------------------------------------------------------------------*/
{
   FILE        *ftxt_file;
   char         buffer[2*MAX_STRING];
   char         buf2[MAX_STRING+MAX_STR];
   STRING_TYPE  buf1;
   STR_TYPE     ident_buf;
   char         dir_delim,
               *check_buf,
               *c_dummy;
   BOOL         io_ok, fin, file_id_ok;

   /* lies_ftext_file */
   if ((ftxt_file = fopen (file_name, "rt")) == NULL)
   { /* Fehlerfile nicht offen */
      sprintf (buf2, NO_FTEXT_FILE, file_name);
      sprintf(buffer, "%s%s", LOCAL_FAT_TXT, buf2);
      write_buf (get_line_anz() + 1, buffer);
      return (FALSE);
   } /* Fehlerfile nicht offen */
   else
   { /* Fehlerfile offen */
      c_dummy = fgets (buffer, MAX_STRING, ftxt_file); /* skip 1st header line*/
      io_ok = fscanf (ftxt_file, "%s", ident_buf) == 1;  /* read File-Ident   */
      c_dummy = fgets (buffer, MAX_STRING, ftxt_file); /* skip rest of line   */
      c_dummy = fgets (buffer, MAX_STRING, ftxt_file); /* skip 3rd header line*/
      c_dummy = fgets (buffer, MAX_STRING, ftxt_file); /* skip 4th header line*/
      fin       = feof (ftxt_file);
      dir_delim = DIR_DELIMITER[0];
      if ((check_buf = strrchr (file_name, dir_delim)) == NULL)
         file_id_ok = !strcmp (ident_buf, file_name);
      else 
      { /* vom Pfad befreiten Filenamen vergleichen */
         check_buf++;                                   /* skip DIR_DELIMITER */
         file_id_ok = !strcmp (ident_buf, check_buf);
      } /* vom Pfad befreiten Filenamen vergleichen */
      if ((!strcmp (ident_buf, PROG_IDENT) || file_id_ok) && io_ok && !fin)
      { /* Fehlertext-File: Kopf ok */
         while (io_ok && !fin && (akt_text_anz <= MAX_ANZ_FTEXTE))
         { /* Fehlertexte zeilenweise einlesen */
            io_ok = fscanf (ftxt_file, "%[^\n]s", ftexte [akt_text_anz]) == 1;
            c_dummy = fgets (buffer, MAX_STRING, ftxt_file); /*skip rest of line*/
            fin = feof (ftxt_file);
            if (!fin)
               akt_text_anz++;
         } /* Fehlertexte zeilenweise einlesen */
         io_ok = fin && (akt_text_anz <= MAX_ANZ_FTEXTE);
         if (!io_ok)
         { /* Fehler */
            if (!fin && (akt_text_anz > MAX_ANZ_FTEXTE))
               sprintf (buf2, FTXT_OVL, MAX_ANZ_FTEXTE);
            else
               sprintf (buf2, FTXTFILE_ERR2, file_name);
            sprintf (buffer, "%s%s", LOCAL_FAT_TXT, buf2);
	    write_buf (get_line_anz() + 1, buffer);
         } /* Fehler */
         return (io_ok);
      } /* Fehlertext-File: Kopf ok */
      else
      { /* Fehlertext-File: Kopf fehlerhaft */
         if (strcmp (ident_buf, PROG_IDENT) != 0)
            sprintf (buf2, BAD_FTXT_FILE, ident_buf);
         else
            sprintf (buf2, FTXTFILE_ERR1, file_name);
         sprintf (buffer, "%s%s", LOCAL_FAT_TXT, buf2);
	 write_buf (get_line_anz() + 1, buffer);
         io_ok = FALSE;
      } /* Fehlertext-File: Kopf fehlerhaft */
      fclose (ftxt_file);
      return (io_ok);
   } /* Fehlerfile offen */
} /* lies_ftext_file */



/* ================================ get_1msg ================================ */
BOOL get_1msg (char *mbuf)
{
   if (msg_ind == NO_INFO)
      msg_ind = 0;                     /* 1st call since last message routine */
   return (make_meldung (mbuf, TRUE));
} /* get_1msg */ 



/* =============================== clear_fbuf =============================== */
void clear_fbuf (void)
{
   clear_err_state ();
   msg_ctr         = 0;
   skipped_msg_ctr = 0;
} /* clear_fbuf */



/* ============================= clear_err_state ============================ */
void clear_err_state (void)
{
   if (error_state == GLEAM_ERROR)
   {
      error_state = GLEAM_OK;
      err_state   = GLEAM_OK;
   }
} /* clear_err_state */



/* ================================ log_msgs ================================ */
BOOL log_msgs (void)
{
   GSTATE       state;
   STRING_TYPE  buf;

   if (get_fbuf_data (&state))                  /* get_fbuf_data (&state) > 0 */
   {
      do
	 ;
      while (get_1msg (buf));
   }
   return (state != GLEAM_OK);
} /* log_msgs */



#ifdef TEXT_MODE
/* =========================== show_and_clear_fbuf ========================== */
void show_and_clear_fbuf (BOOL  logAllMsgs)
{
   STR_TYPE  buf, buf2;
   BOOL      savLogAllMsgs;
   
   /* show_and_clear_fbuf */
   if (msg_ctr > 0)
   { /* Es sind Meldungen vorhanden */
      msg_ind = 0;
      strcpy (buf2, "");
      switch (error_state) 
      {
         case GLEAM_OK:
            strcpy (buf, OK_STATE_TXT);  break;
         case GLEAM_ERROR:
            strcpy (buf2, "***");
            strcpy (buf, ERR_STATE_TXT); break;
         case GLEAM_FATAL:
            strcpy (buf2, "*****");
            strcpy (buf, FAT_STATE_TXT); break;
         default:
            strcpy (buf, "???");         break;
      } /* switch */
      printf (MSG_HDR_TXT, 0, " ", buf2, msg_ctr, buf); /* Meldungskopfzeile  */
      if (logAllMsgs) {
	 savLogAllMsgs = log_all_msgs;                   /* save actual value */
	 log_all_msgs = TRUE;       /* forces "make_meldung" to log everthing */
      }
      while (make_meldung (buf, TRUE))
	 printf ("%s\n", buf);                 /* (Fehler-)Meldungen ausgeben */
      if (logAllMsgs)
	 log_all_msgs = savLogAllMsgs;                /* restore actual value */
   } /* Es sind Meldungen vorhanden */
   clear_fbuf ();
} /* show_and_clear_fbuf */ 
#endif /* TEXT_MODE  */



/* ================================ write_buf =============================== */
BOOL write_buf (INT         line_nr,
                const char *buf)
{
   BOOL  erg;

   /* write_buf */
   erg = TRUE;
   if ((0 < line_nr) && (line_nr <= MAX_BUF_ANZ))
   { /* line_nr OK */
      if (line_anz < line_nr)
         line_anz = line_nr;
      if (strlen (buf) > MAX_STRING)
      { /* zu viele Zeichen */
	 strncpy (text_buf[line_nr-1], buf, MAX_STRING);
         text_buf[line_nr-1][MAX_STRING] = '\0';
      } /* zu viele Zeichen */
      else
	 strcpy (text_buf[line_nr-1], buf);
   } /* line_nr OK */
   else { /* line_nr fehlerhaft */
      fehler (FBHM_FBHM, 1, line_nr, MAX_BUF_ANZ, "", FBHM_BAD_ZNR);
      erg = FALSE;
   } /* line_nr fehlerhaft */
   return (erg);
} /* write_buf */



/* ================================= get_buf ================================ */
BOOL get_buf (INT   line_nr,
              char *buf)
{
   BOOL  erg;

   /* write_buf */
   erg = FALSE;
   if (line_anz > 0) 
   { /* Zeilenanzahl positv */
      if ((0 < line_nr) && (line_nr <= MAX_BUF_ANZ))
      { /* line_nr OK */
	 strcpy (buf, text_buf[line_nr-1]);
	 erg = line_nr <= line_anz;
      } /* line_nr OK */
      else /* line_nr fehlerhaft */
	 fehler (FBHM_FBHM, 2, line_nr, MAX_BUF_ANZ, "", FBHM_BAD_ZNR);
   } /* Zeilenanzahl positv */
   return (erg);
} /* get_buf */



/* ================================ clear_buf =============================== */
void clear_buf (INT  line_nr)
{
   INT   ii;
   BOOL  leer;

   /* line_nr fehlerhaft */
   if ((0 < line_nr) && (line_nr <= MAX_BUF_ANZ))
   { /* line_nr OK */
      strcpy (text_buf[line_nr-1], "");
      leer = TRUE;
      for (ii = line_anz - 1;  (ii >= 0) && leer;  ii--)
         leer = strlen (text_buf[ii]) == 0;
      line_anz = ii + 2;     /* +2: Schleife decr 1 zuweit u. Index->Zeilennr */
   } /* line_nr OK */
   else /* line_nr fehlerhaft */
      fehler (FBHM_FBHM, 3, line_nr, MAX_BUF_ANZ, "", FBHM_BAD_ZNR);
} /* clear_buf */



/* ============================== get_line_anz ============================== */
INT get_line_anz (void)
{
   return (line_anz);
} /* get_line_anz */



/* ============================= reset_text_buf ============================= */
void reset_text_buf (void)
{
   INT  ii;

   for (ii = 0;  ii < MAX_BUF_ANZ;  ii++)
      strcpy (text_buf[ii], "");
   line_anz = 0;
} /* reset_text_buf */



/* ============================== logTextBuffer ============================= */
void logTextBuffer  (BOOL  mitTerm)
{
   STR_TYPE  buf;
   INT       ii;

   /* logTextBuffer */
   if (!logfile_open) {
      logfile      = fopen (logfilename, "at");
      logfile_open = logfile != NULL;
   }
   if (logfile_open || mitTerm) { /* alles muss raus */
      ii = 1;
      while (get_buf(ii++, buf))
      { /* Ausgabe des Textpuffers */
	 if (logfile_open)
	    fprintf (logfile, "%s\n", buf);
	 if (mitTerm)
	    printf("%s\n", buf);
      } /* Ausgabe des Textpuffers */
   } /* alles muss raus */
   if (logfile_open) {
      fflush (logfile);
      fclose (logfile);
   }
   logfile_open = FALSE;
   reset_text_buf ();
} /* logTextBuffer */



#ifdef TEXT_MODE
/* ============================= write_text_buf ============================= */
void write_text_buf (BOOL  del_buf)
{
   INT   ii;
   char  query;

   /* write_text_buf */
   for (ii = 0;  ii < line_anz;  ii++) { /* Ausgabeschleife */
      printf ("%s\n", text_buf[ii]);
      if ((((ii+1) % MAX_LINE) == 0) && ((ii+1) != line_anz)) 
      { /* Unterbrechung ? */  
	 query = char_dialog (SCROLL_QUERY);
	 if (query == ESC)
	    break;
      } /* Unterbrechung ? */
   } /* Ausgabeschleife */

   if (del_buf)
      reset_text_buf ();
} /* write_text_buf */
#endif /* TEXT_MODE */



/* ================================ fbhmInit ================================ */
BOOL fbhmInit (BOOL msg_log)
{
   char         buf1[MAX_STRING+MAX_BUF], buf2[MAX_STRING+MAX_STR];
   STRING_TYPE  fspec, fspec2;
   char        *envBuf;
   INT          ii;
   BOOL         ret_stat, ok;
   
   /* fbhmInit */
   ok = TRUE;
   for (ii = 0;  (ii < MAX_BUF_ANZ) && ok;  ii++)
      ok = (text_buf[ii] = (char*)malloc((MAX_STRING+1) * sizeof(char))) != NULL;
   if (ok) { /* text buffer ready */
      reset_text_buf (); 
      textBufOK = TRUE;
   } /* text buffer ready */
   else { /* start to abort program immediately */
      fprintf (stderr, FATAL_INIT_TXT);
      return (FALSE);
   } /* start to abort program immediately */

   log_all_msgs       = msg_log;
   akt_text_anz       = 0;
   ftextpar_begin     = 0;                    /* also keine ParFMeldungstexte */
   msg_ctr            = 0;
   skipped_msg_ctr    = 0;
   numberOfStdErrMsgs = 0;
   error_state        = GLEAM_OK;
   err_state          = GLEAM_OK;         /* globale error state is set to OK */
   for (ii = 0;  ii < MAX_ANZ_FTEXTE;  ii++)
   {
      ftexte[ii] = (char*) malloc ((MAX_STR+1) * sizeof(char));
      strcpy (ftexte[ii], EMPTY_TXT);
   }
   msg_buffer = (MSG_BUF_TYPE*)malloc(MAX_ANZ_MSG * sizeof (MSG_BUF_TYPE));
   atexit (fbhm_exit);                       /* Etablierung des Exit-Handlers */
   envBuf = getenv(GLEAM_ROOT_ENV);
   strcpy (fspec, FTEXTE_FNAME);
   strcpy (fspec2, ADD_FTEXTE_FNAME);
   if (envBuf != NULL) 
   { /* environment variable exists */
      if (!file_exists(fspec))
	 sprintf (fspec, "%s%s%s", envBuf, DIR_DELIMITER, FTEXTE_FNAME);
      if (strlen(fspec2) > 0)        /* additional error text file specified? */
	 if (!file_exists(fspec2))
	    sprintf (fspec2, "%s%s%s", envBuf, DIR_DELIMITER, ADD_FTEXTE_FNAME);
   } /* environment variable exists */
   ret_stat = lies_ftext_file (fspec);       /* lese Standard-Fehlermeldungen */
   numberOfStdErrMsgs = akt_text_anz;
   if (strlen(ADD_FTEXTE_FNAME) > 0)
   { /* an application-specific additional error message file is specified */
      if (file_exists (fspec2))
         ret_stat &= lies_ftext_file (fspec2); /*lese zusatzl.Fehlermeldungen */
#ifndef MPI_PAR_VERS
      else
      { /* report missing file as warning */
	 sprintf (buf1, NO_FTEXT_FILE, fspec2);
	 sprintf(buf2, "%s%s", LOCAL_WARN_TXT, buf1);
	 write_buf (get_line_anz() + 1, buf2);
      } /* report missing file as warning */
#endif /* kein MPI_PAR_VERS */
   } /* an application-specific additional error message file is specified */

#ifdef MPI_PAR_VERS
   ftextpar_begin = akt_text_anz;  /* Pos. der 1. PAR-Fehlermeldung in ftexte */
   /*zusaetzl.Einlesen der Fehlertextedatei f. Parallelversion: */
   if (envBuf == NULL)
      strcpy (fspec, FTEXTEPAR_FNAME);
   else
      sprintf (fspec, "%s%s%s", envBuf, DIR_DELIMITER, FTEXTEPAR_FNAME);
   ret_stat &= lies_ftext_file (fspec);
#endif /* MPI_PAR_VERS */
#ifdef TESTS
   printf ("fbhmInit: akt_text_anz=%d  ftextpar_begin=%d \n", akt_text_anz, ftextpar_begin);
#endif /* TEST */

   return (ret_stat && ok);
} /* fbhmInit */ 

