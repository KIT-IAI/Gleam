/*******************************************************************************
GLEAM/AE                                                                  V1.2.3

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: SIMUlator
                        Module: ROBot-SIMulator fuer HYgleam
 
Package: simu                   File: robsimhy.c              Unterpackage: lskp
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of this file.

Die Benutzung des Robotersimulators erfolgt mit freundlicher Genehmigung von 
Prof.Dr. C.Blume, FH Koeln, Abt. Gummersbach, FB Elektrotechnik.
Zum Simulator gehoeren die Module "rob_sim", "rob_gsw", "rob_dial", "pasromin"
und "lsk_grep".
--------------------------------------------------------------------------------

Der Modul enthaelt den Robotersimulator und die Teach-In-Komponente von LESAK. 

Der Simulator erhaelt eine Aktionskette, interpretiert und simuliert diese. 
Resultat sind folgende Ergebniswerte, die in dem Feld "simu_erg_werte" in nach-
stehender Reihenfolge abgeliefert werden:
   Zielerreichung         Abweichung des Bahnendpunkts vom Zielpunkt
   Bewegungsdauer         Dauer der Bewegung in Sekunden
   Geradeninterpolation   Mass fuer die Bahnabweichung von der idealen Gerade
   Kettenlaenge           Anzahl der Aktionen in der Kette
   Befehlsanzahl          Anzahl der ausgefuehrten Aktionen 
   Restweg                Distanz des Bahnendpunkts vom Zielpunkt bei Hindernis-
                          Crash oder Erreichung eines Achsanschlags. Sonst 0.
   Energieverbrauch       Energieverbrauch der Motoren wenn "tsk_schalter[MIT_
                          ENERGIE]" gesetzt ist, sonst 0.0.
Wenn "std_simu_interface" nicht gesetzt ist, berechnet der Simulator ferner die 
ungewichteten Noten "ungew_note".  

In "update_rob_task" werden mehrere Parameter der TSK-Datei ausgewertet und 
steuern damit den Robotersimulator.

Wenn Simulationsende und die Erfuellung einer Abbruchbedingung zusammenfallen,
enthaelt der Ergebniswert Restweg immer einen Wert > 0.0.

Start- und Zielpunkt sind in Joint-Koordinaten in den Variablen "start_joints"
und "ziel_joints" des Packages "aufg" hinterlegt.

Die Grafik (Animation der Roboterbewegungen) wird auf zwei Ebenen gesteuert. 
Erstens kann durch den Compilerschalter MIT_ANIM festgelegt werden, ob der ent-
sprechende Code ueberhaupt mituebersetzt wird oder nicht. Zweitens kann durch 
den Aufrufparameter "interaktiv" kontrolliert werden, ob eine Animation auch 
tatsaechlich durchgefuehrt wird.


Die Routinen im einzelnen:

BOOL do_rob_sim (DOUBLE              *simu_erg_werte,
                 CHAIN_ELEM_PTR_TYPE  ch_ptr,
                 BOOL                 interaktiv);
          Der Simulator interpretiert die AK "ch_ptr" und simuliert sie. Die
          Ergebniswerte werden wie oben beschrieben ermittelt und in "simu_erg_
          werte" abgeliefert. Fehler bei der Dekodierung und Ausfuehrung von
          Aktionen fuehren zum Simulationsabbruch, einer Fehlermeldung und die
          Funktion liefert FALSE als Ergebnis. Bei bewertbaren Ketten, also auch
          solchen, bei denen die Simulation wegen Crash oder Achsanschlag abge-
          brochen wurde, wird TRUE abgeliefert. "interaktiv" steuert, ob waeh-
          rend der Simualtion eine Animation der erfolgten Roboterbewegungen 
          stattfindet (Animation derzeit nicht implementiert).

BOOL update_rob_task (void);
          Die Routine bewirkt die Uebernahme neuer Aufgabendaten des Packages
          "aufg". Wenn dabei Fehler auftreten, erfolgt eine Fehlermeldung und 
          die Routine liefert FALSE, sonst TRUE.

BOOL ladekinematik (const char *file_name,
	            INT         znr);
          Laedt die Kinematikdaten aus der durch "file_name" bezeichneten Datei
          und liefert TRUE, wenn dies fehlerfrei moeglich war. Im Textpuffer 
          des "fbhm"-Packages erfolgt eine Ergebnismeldung in die durch "znr" 
          angegebene Zeile. Auftretende Fehler werden gemeldet. 

BOOL hole_obs (const char *file_name,
	       INT         znr);
          Laedt die Hinderniskdaten aus der durch "file_name" bezeichneten Datei
          und liefert TRUE, wenn dies fehlerfrei moeglich war. Im Textpuffer
          des "fbhm"-Packages erfolgt eine Ergebnismeldung in die durch "znr" 
          angegebene Zeile. Auftretende Fehler werden gemeldet. 

void hindernismanagement (void);
          Verwaltung der Hindernisse. Noch nicht implementiert.

BOOL write_code_file (const char *fspec, 
	              INT         guete, 
	              INT         lfd_nr);
          Interpretiert die durch "guete" und "lfd_nr" bezeichnete AK und 
          schreibt den dabei anfallenden Robotercode in die durch "fscpec"
          bezeichnete Datei. Liefert TRUE, wenn alles gut ging, sonst FALSE.

BOOL rob_init (void);
          Modulinitialisierung. Liefert FALSE bei Speichermangel.

--------------------------------------------------------------------------------
Anmerkung:
Falls es im LEASK-Simulator zu einer Situation kommt, die die Simulation auch 
von bereits bewerteten AKs zwingend notwendig macht, z.B. weil Hindernisse ver-
aendert wurden, so muss die globale Variable "always_sim" auf TRUE gesetzt wer-
den.
--------------------------------------------------------------------------------
Basiert auf den LESAK-Units: "simu" und "simuhelp".
--------------------------------------------------------------------------------
Basiert auf V1.1.2 des allg. LESAK-2 Simulators. Diese Version ist auf den 
Mitsubishi mit einem Handlungsmodell mit Aktionen pro Motor zugeschnitten. Fuer
Experimente mit HyGLEAM.
Zu dieser Variante gehoeren: "rob_simhy.c", hy_grep.c" und "lskmodhy.h".
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. Deklaration von Stringparameter, die reine Eingangsparameter 
    sind und mit Stringkonstanten aufgerufen werden, als "const".    WJ 19.04.12
 2. Entfernung der Routine "write_to_simulog", die nun vom Package 
    "sys" zur Verfuegung gestellt wird.                              WJ 20.04.12
 3. Streichung der "#ifndef GLEAM_TCL"-Anweisungen. Neue TEXT_MODE-
    Anweisungen. Korrektur von Formatvorgaben. Entfernung ueber-
    fluessiger Variable.                                             WJ 29.06.12
 4. Renovierung: Korrektur der fscanf- und fgets-Aufrufe. Streichung 
    des "link"-Includes. Anpassung der "write_text_buf"- und 
    "char_dialog"-Aufrufe. Ersetzung der "write_line"- durch "printf"-
    Aufrufe. Streichung der Aufrufe von "clr_line". Aenderung der 
    Aufrufschnittstelle von "ladekinematik" und "hole_obs".          WJ 13.05.16
 5. Anpassung des Aufrufs von "write_to_simu_log".                   WJ 14.09.17
 6. Lokalisierung von JOINTS: Die Variablen "start_joints" und 
    "ziel_joints" werden von dem Modul "rob_sim.c" bzw. "robsimhy.c" 
    exportiert. Diese werden nur in den Packages "aufg" und "simu/lskp" 
    benoetigt. "joints.h" wird nicht mehr in "simu.h" inkludiert und 
    damit automatisch exportiert, sondern muss stattdessen separat 
    von den Module inkludiert werden, die es wirklich benoetigen. 
    Damit wird der Package-Import insgesamt schlanker.               WJ 08.01.18
 7. Aktionen zur Zeitsteuerung werden nicht als Instruktionen 
    gezaehlt.                                                        WJ 29.08.20
--------------------------------------------------------------------------------
Status: o.k.              Autoren: C.Blume/W.Jakob             Datum: 29.08.2020
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "lgsw.h"
#include "bew.h"
#include "aufg.h"
#include "joints.h"
#include "rob_int.h"
#include "lskmodhy.h"

#include SIMU_TEXTS
#include GLOBAL_TEXTS



/* ========================== Exportierte Variable: ========================= */
JOINTS     start_joints,            /* Startpunkt in Joint-Koordinaten        */
           ziel_joints;             /* Zielpunkt in Joint-Koordinaten         */


/* ======================== Package-globale Variable: ======================= */


/* ============================ Lokale Konstante: =========================== */
#define MAXZEITREG        5          /* max.Anz.von Zeit-Registern u. -Vars   */
#define ZIEL_ABW_IDX      0          /* Index im Bewertungs-"kriterien"-Feld  */
#define BEW_DAUER_IDX     1          /* Index im Bewertungs-"kriterien"-Feld  */
#define GERADEN_ABW_IDX   2          /* Index im Bewertungs-"kriterien"-Feld  */
#define AK_LEN_IDX        3          /* Index im Bewertungs-"kriterien"-Feld  */
#define BEF_ANZ_IDX       4          /* Index im Bewertungs-"kriterien"-Feld  */
#define RESTWEG_IDX       5          /* Index im Bewertungs-"kriterien"-Feld  */
#define ENERGIE_IDX       6          /* Index im Bewertungs-"kriterien"-Feld  */

#define RAMPE_MIN         0.1
#define RAMPE_MAX_AN     15.0
#define RAMPE_MAX_AUS    12.0
#define SPANNUNG_MIN   -120.0
#define SPANNUNG_MAX    120.0



/* ======================== Lokale Typdeklarationen: ======================== */
typedef struct
  { DOUBLE  spannung,
            rampe;                          /* Aenderung der Spannung je Takt */
  } STEUERFELDELEMENT;

typedef STEUERFELDELEMENT  T_STEUERFELD [MAXAXISANZ]; 
typedef DOUBLE             T_AKTUFELD   [MAXAXISANZ];  /* aktuelle Geschw.    */
typedef DOUBLE             T_ZEITREG    [MAXZEITREG];
typedef DOUBLE             T_DVOLD      [MAXAXISANZ];




/* ============================ Lokale Variable: ============================ */
static char                *hbuf,       /* Hilfspuffer                        */
                           *ch_tmp;     /* Hilfsvariable fuer Returnwerte     */

static FILE                *infile;     /* Einlesen d.Kinematik u.Hindernisse */
static BOOL                 kin_loaded, /* TRUE = eine Kinematik wurde geladen*/
                            mit_energie,/* TRUE=Erfassung d.Energieverbrauchs */
                            val_out_of_range,
                            user_abbruch;    /* TRUE=SimuAbbruch durch User   */
static CHAIN_ELEM_PTR_TYPE  act_item;        /* aktuelle Aktion der Simulat.  */
static CHAIN_ELEM_PTR_TYPE  nextak;          /* Zeiger auf aktuelle Aktion    */
static INT                  bef_ctr;         /* zaehlt die ausgef. Aktionen   */
static BOOL                 anschlag,
                            geradeninterpol,
                            simuabbruch,     /* Abbruch wegen Kol.od.Anschlag */
                            simuende,        /* normale Beendigung wg AK-Ende */
                            vergleichsvar,
                            startblock,
                            auslaufen;       /* Bewegung laeuft b.AK-Ende aus */
static INT                  springanz;       /* Anzahl der zu ueberspring.Akt */
static T_STEUERFELD         steuerfeld;      /* Spannung (Geschw) & Rampe     */
static T_AKTUFELD           aktufeld;        /* aktuelle Geschwindigkeit      */
static INT                  takt,
                            gtakt;           /* Takt fuer Simul. & TCP-Bahn   */
static T_ZEITREG            zeitvar,
                            zeitreg;         /* Register SIMU-Zeit            */
static DOUBLE               zeit,            /* simulierte Ausfuehrungszeit   */
                            gesamt_weg,      /* Gesamtweg von Start zum Ziel  */
                            max_dist_gpol;   /* MaxAbst.f.Geraden-Bewertung   */
static INT                  unveraendert_anz;/* Anzahl der Takte ohne Veraend */
static DOUBLE               bahndistanz,
                            bahndistanzhalbe,
                            zweibahndistanz,
                            bges,
                            summedv,
                            summeda;         /* Mass fuer die ges. Bew.beschl */
static T_DVOLD              dv,
                            dvold;           /* Geschw. vorher. Takt          */
static JOINTS               thmodziel;



/* ============================ Lokale Routinen: ============================ */
static void del_obstacles    (void);

#if defined (MIT_ANIM) || defined (MITSIMULOG)
static void compstr          (char *str,
                              INT   vgl_op);
#endif /* MIT_ANIM || MITSIMULOG */

#ifdef MIT_ANIM
static void abbruchtest      (void);
#endif /* MIT_ANIM */

static void motor_fehlt      (INT   motor);

static void setzeu           (INT   motor);

static void motoraus         (INT   motor);

static void aktinterpreter   (void);

static void eintakt          (void);




/* =============================== do_rob_sim =============================== */
BOOL do_rob_sim (DOUBLE              *simu_erg_werte,
                 CHAIN_ELEM_PTR_TYPE  ch_ptr,
                 BOOL                 interaktiv)
{
   DOUBLE  rwert, rest_weg;
   INT     ii;
   GSTATE  fstat = GLEAM_OK;
   BOOL    simu_done;

   /* do_rob_sim */
   if (ch_ptr->u_el.activity != AK_HEADER)
   { /* Kette ist keine AK */
      fehler (SIMU_ROB_SIM, 1, ch_ptr->u_el.activity, NO_INFO, "", 
	      BAD_CHAIN_KIND);
      return (FALSE);                                              /* Abbruch */
   } /* Kette ist keine AK */
   if (!kin_loaded)
   { /* Keine Kinematik geladen */            /* SIMU_RZ_MELD bringt nur Text */
      fehler (SIMU_ROB_SIM, 2,NO_INFO,NO_INFO, NO_KIN_LOADED_TXT, SIMU_RZ_MELD);
      return (FALSE);                                              /* Abbruch */
   } /* Keine Kinematik geladen */
   
   /* -------------------------- Initialisierungen: ------------------------- */
   act_item      = ch_ptr;
   nextak        = act_item->u_el.next_action;   /* 1.Element nach Kettenkopf */
   nichtevo      = interaktiv;
   zeit          = 0.0;       /* simulierte Ausfuehrungszeit                  */
   bahndistanz   = 0.0;       /* direkte Weglaenge von Start zum Ziel         */
   bef_ctr       = 0;         /* zaehlt die ausgefuehrten Aktionen (Befehle)  */
   takt          = 1;         /* Anzahl der ausgefuehrten Takte               */
   gtakt         = 1;         /* Takt fuer die Darstellung der TCP-Bahn       */
   sensordrehung = 0.0;       /* Noch keine Sensordrehung                     */
   anschlag      = FALSE;     /* TRUE, wenn Achsanschlag erfolgte             */
   kollision     = FALSE;     /* TRUE, wenn Kollision erfolgte                */
   val_out_of_range = FALSE;
   simuabbruch   = FALSE;     /* TRUE, wenn SimuAbbruch wg Kollis.od.Anschlag */
   user_abbruch  = FALSE;     /* TRUE, wenn Benutzer die Simulation abwuergt  */
   simuende      = nextak == NULL;  /* TRUE, wenn "normales" Simulationsende  */
   unveraendert_anz = 0;      /* Anzahl der Takte ohne Veraenderung           */
   vergleichsvar    = FALSE;  /* Merker f.nach Vergl. Aktionen ueberspringen  */
   springanz        = 0;      /* Anzahl der zu ueberspringenden Aktionen      */
   startblock       = FALSE;  /* Aktionen erst bei STARTEND gemeins. beginnen */
   geradeninterpol  = kriterien[GERADEN_ABW_IDX].klasse != KRITERIUM_AUS;
   mit_energie      = kriterien[ENERGIE_IDX].klasse     != KRITERIUM_AUS;
   for (ii = 0;  ii < axisanz;  ii++)
   { /* Startwerte aller aktuellen Roboterachsen setzen */
      robotjoints[ii]       = start_joints[ii];       /* akt. Roboterachsen   */
      oldrobotjoints[ii]    = start_joints[ii];       /* Werte vorher. Takt   */
      beschlrobotjoints[ii] = start_joints[ii];       /* fuer Beschl.berechng.*/
      sensorposvar[ii]      = 0.0;         /* Speicherung von Positionswerten */
   } /* Startwerte aller aktuellen Roboterachsen setzen */
   for (ii = 0;  ii < MAXZEITREG;  ii++)   /* Register fuer Zeitspeicherung   */
   {
      zeitreg[ii] = 0.0;
      zeitvar[ii] = 0.0;
   }
   *robotframe = *anfangsframe;            /* Startwert des Roboterframes     */
   for (ii = 0;  ii < axisanz;  ii++)
   { /* Startwerte des Steuerfelds fuer "eintakt" */
      steuerfeld[ii].spannung = 0.0;     /* Spannung = Sollgeschwdgk. in Grad */
      steuerfeld[ii].rampe    = 0.0;     /* Geschwdgks.rampe in Grad/Takt     */
      aktufeld[ii]            = 0.0;     /* aktuelle Spannung = Geschwindigk. */
   } /* Startwerte des Steuerfelds fuer "eintakt" */
   if (geradeninterpol)
   { /* Berechnungen fuer Bewertung der Bahnabweichung */
      /* Berechnung des Abstands zwischen Anfangs- und Zielposition: */
      bahndistanz      = vdist (&(zielframe->transl), &(anfangsframe->transl));
      bahndistanzhalbe = bahndistanz / 2.0;   /* fuer Formel in "eintakt"     */
      zweibahndistanz  = 2.0*bahndistanz;     /* fuer Formel in "eintakt"     */
      bges = 0.0;                             /* zurueckgelegte Bewegungsbahn */
   } /* Berechnung fuer Bahnabweichung */
   if (mit_energie)
   { /* Berechnungen fuer Kriterium "Energieverbrauch" */
      summedv = 0.0;         /* Geschwindigkeitskennung: Anfangswert auf Null */
      summeda = 0.0;         /* Beschleunigungskennung:  Anfangswert auf Null */
      for (ii = 0;  ii < axisanz;  ii++)
      {
	 dv[ii]    = 0.0;                /* Anfangsgeschwindigkeiten auf Null */
	 dvold[ii] = 0.0;
      }
   } /* Berechnungen fuer Kriterium "Energieverbrauch" */
  
   /* ------------------ AK-Interpretation und -Simulation: ----------------- */
   while (!(simuabbruch || simuende || (fstat != GLEAM_OK)))
   { /* Schleife ueber gesamte AK */
      aktinterpreter ();     /* Aufruf d. Aktions-Interpreters f. eine Aktion */
      if (springanz > 0)             /* sollen Aktionen uebersprungen werden? */
      { /* d.h. es war Vergleich auf Pos, Geschw, u.a. TRUE */
	 vergleichsvar = TRUE;       /* Merker setzen: Aktionen ueberspringen */
	 for (ii = 0;  ii < springanz;  ii++)
	 { /* Aktionen ueberspringen */
            if (nextak == NULL)
	       simuende = TRUE;                  /* Kettenende erreicht       */
            else
	       nextak = nextak->u_el.next_action; /*Zeiger auf naechste Aktion*/
	 } /* Aktionen ueberspringen */
#if defined MIT_ANIM || defined MITSIMULOG
	    if (nichtevo)
	    { /* interaktiver Aufruf */
	       sprintf (meld_buf, JUMP_DONE_TXT, springanz);
#ifdef MITSIMULOG
	       write_to_simulog (meld_buf, CLOSE_LOG);
#endif /* MITSIMULOG */
	    } /* interaktiver Aufruf */
#endif /* MIT_ANIM || MITSIMULOG */
      } /* d.h. es war Vergleich auf Pos, Geschw, u.a. TRUE */
      else 
      {  /* keine Aktionen ueberspringen */
	 if (unveraendert_anz <= 0)       /* Simu unveraendert weiterlaufen ? */
            eintakt ();  /* nein, normal einen Takt mit neuer Aktion ausfuehr.*/
	 else
	 { /* mehrere (Bew.-)Takte ohne neue Aktionen ausfuehren */
            do
            { /* Schleife ueber die Anzahl unveraenderter Takte */
	       eintakt ();                    /* einen Bewegungstakt aufrufen */
	       get_fbuf_data (&fstat);
	       unveraendert_anz--;            /* 1 Takt erledigt              */
#ifdef MIT_ANIM
	       if (nichtevo)                  /* bei Menue-Aufruf Test auf    */
		  abbruchtest ();             /* Simulationsabbruch           */
#endif /* MIT_ANIM */
	       if (simuabbruch || (fstat != GLEAM_OK))
		  unveraendert_anz = 0;       /* Schleife abbrechen           */
#ifdef MITSIMULOG
	       if (!startblock && nichtevo)
	       {
		  sprintf (meld_buf, TAKT_ERLEDIGT_TXT, takt, zeit);
		  write_to_simulog (meld_buf, CLOSE_LOG);
	       }
#endif /* MITSIMULOG */
	       takt++;                        /* naechster Simulationstakt    */
            } /* Schleife ueber die Anzahl unveraenderter Takte */
            while (unveraendert_anz > 0);   
            takt--;                   /* Takt ruecksetzen, sonst 1mal zuviel  */
	 } /* mehrere (Bew.-)Takte ohne neue Aktionen ausfuehren */
      } /* keine Aktionen ueberspringen */
        
#ifdef MITSIMULOG
      if (!startblock && nichtevo)
      {
	 sprintf (meld_buf, TAKT_ERLEDIGT2_TXT, takt, zeit);
	 write_to_simulog (meld_buf, CLOSE_LOG);
      }
#endif /* MITSIMULOG */
      if (!simuabbruch)             /* Falls kein Simulationsabbruch vorliegt */
      { /* Simu weiterfuehren */
#ifdef MIT_ANIM
	 if (nichtevo)                 /* vom Menue aus aufgerufen?           */
            abbruchtest ();            /* ja, Abbruchtest durchfuehren        */
#endif /* MIT_ANIM */
	 springanz = 0;                /* Anzahl zu ueberspring. Aktionen = 0 */
	 if (!startblock)              /* liegt kein Startblock vor?          */
            takt++;                    /* ja, Simulationstakt erhoehen        */
      }  /* Simu weiterfuehren */
      get_fbuf_data (&fstat);
   } /* Schleife ueber gesamte AK */

   /* ------------------ Roboterbewegung auslaufen lassen: ------------------ */
   auslaufen = FALSE;         /* Merker fuer Auslaufen der Achsen ruecksetzen */
   for (ii = 0;  ii < axisanz;  ii++)
   { /* Schleife ueber alle Roboterachsen */
      if (aktufeld[ii] != 0.0)    /* befindet sich Achse ii noch in Bewegung? */
      { /* ja, Wert fuer Rampe ermitteln */
	 auslaufen = TRUE;               /* Merker "Achsen laufen aus" setzen   */
	 if (fabs(aktufeld[ii]) < 5.0)   /* akt. Geschwdgk. < 5 Grad?           */
            steuerfeld[ii].rampe = 1.0;  /* ja, Geschw.rampe = 1 Grad/Takt    */
	 else                 /* nein, Geschw.rampe 1/5 der akt.Geschw Rampe:   */
            steuerfeld[ii].rampe = fabs (aktufeld[ii] / 5.0) + 1.0;
      } /* ja, Wert fuer Rampe ermitteln */
      steuerfeld[ii].spannung = 0.0;     /* Zwangsmaessig Bewegung beenden    */
   } /* Schleife ueber alle Roboterachsen */
   auslaufen &= (fstat == GLEAM_OK);
#if defined MIT_ANIM || defined MITSIMULOG
   if (nichtevo)
   { /* interaktiver Aufruf */
      strcpy (meld_buf, AK_FIN_TXT);
      if (simuende)
	 strcat (meld_buf, AK_ENDE_TXT);
      else if (simuabbruch)
	 strcat (meld_buf, AK_WUERG_TXT);
      else
	 strcat (meld_buf, AK_FEHLER_TXT);
#ifdef MITSIMULOG
      write_to_simulog (meld_buf, CLOSE_LOG);
#endif /* MITSIMULOG */
      if (auslaufen)
      { /* Das Auslaufen der Achsen melden */
	 sprintf (meld_buf, AUSLAUFEN_TXT);
#ifdef MITSIMULOG
	 write_to_simulog (meld_buf, CLOSE_LOG);
#endif /* MITSIMULOG */
      } /* Das Auslaufen der Achsen melden */
   } /* interaktiver Aufruf */
#endif /* MIT_ANIM || MITSIMULOG */
   startblock = FALSE;                      /* Reset aus Sicherheitsgruenden! */
   while (auslaufen && !simuabbruch)
   { /* Solange Geschwindgk. > 0 und kein Simulationsabbruch verlangt wurde */
      eintakt ();              /* einen Bewegungstakt des Roboters ausfuehren */
#ifdef MITSIMULOG
      if (nichtevo)
      {
	 sprintf (meld_buf, TAKT_ERLEDIGT_TXT, takt, zeit);
	 write_to_simulog (meld_buf, CLOSE_LOG);
      }
#endif /* MITSIMULOG */
#ifdef MIT_ANIM
      if (nichtevo)           /* bei Aufruf vom Menue aus -> auf Simulations- */
	 abbruchtest ();      /* abbruch testen bzw. abfragen                 */
#endif /* MIT_ANIM */
      auslaufen = FALSE;      /* Testen, ob Auslaufen fertig:                 */
      for (ii = 0;  ii < axisanz;  ii++)
      { /* Pruefen, ob eine Roboterachse noch bewegt werden muss */
	 if (aktufeld[ii] != 0)    /* Hat eine Achse noch Bewegungsgeschw?    */
            auslaufen = TRUE;      /* ja, Merker Auslaufen d. Achsen setzen   */
      } /* Pruefen, ob eine Roboterachse noch bewegt werden muss */
      get_fbuf_data (&fstat);
      auslaufen &= (fstat == GLEAM_OK);
      takt++;                    /* Simulationstakt erhoehen                */
   } /* Solange Geschwindgk. > 0 und kein Simulationsabbruch verlangt wurde */
#if defined MIT_ANIM || defined MITSIMULOG
   if (nichtevo && !simuabbruch)
   { /* interaktiver Aufruf */
      sprintf (meld_buf, SIMU_ENDE_TXT);
#ifdef MITSIMULOG
      write_to_simulog (meld_buf, CLOSE_LOG);
#endif /* MITSIMULOG */
   } /* interaktiver Aufruf */
#endif /* MIT_ANIM || MITSIMULOG */
   simu_done = !user_abbruch && (fstat == GLEAM_OK);

   /* ------------------- AK-Bewertungsdaten aufbereiten: ------------------- */
   if (simu_done)
   { /* Ergebnisdatenaufbereitung */
      rest_weg = vdist (&(zielframe->transl), &(robotframe->transl));
      if (bahndistanz > 0.0)                             /* Anfang != Zielpos */
	 simu_erg_werte [ZIEL_ABW_IDX] = 100.0 * (rest_weg / bahndistanz);
      else
	 simu_erg_werte [ZIEL_ABW_IDX] = rest_weg;
#ifdef MITSIMULOG
      if (nichtevo)
      {
	 sprintf (meld_buf, ZIEL_ABW_ERG_LOG_TXT, rest_weg, bahndistanz, simu_erg_werte [ZIEL_ABW_IDX]);
	 write_to_simulog (meld_buf, CLOSE_LOG);
      }
#endif /* MITSIMULOG */
      simu_erg_werte [BEW_DAUER_IDX] = zeit;
      simu_erg_werte [AK_LEN_IDX]    = ch_ptr->u_el.u_xel.u_hdr.chain_length;
      simu_erg_werte [BEF_ANZ_IDX]   = bef_ctr;
      if (geradeninterpol)
      { /* Abweichung von der idealen Bahn erfassen */
	 if (bahndistanz > 0.0)                          /* Anfang != Zielpos */
	    rwert = bges / (bahndistanz*bahndistanz);    /* Bereich: 0 .. 1.0 */
	 else
	    rwert = bges / 600.0;                        /* Bereich: 0 .. 1.0 */
	 if (rwert < 0.0)
	    rwert = 0.0;
	 if (rest_weg > max_dist_gpol)
	    rwert = 1.0;             /* Nur Punkte, wenn nahe genug am Ziel */
	 simu_erg_werte[GERADEN_ABW_IDX] = rwert * 100.0; /*Bereich: 0..100.0 */
#ifdef MITSIMULOG
	 if (nichtevo)
	 {
	    sprintf (meld_buf, BAHN_ABW_ERG_LOG_TXT, bges, bahndistanz, rwert);
	    write_to_simulog (meld_buf, CLOSE_LOG);
	 }
#endif /* MITSIMULOG */
      } /* Abweichung von der idealen Bahn erfassen */
      if (kollision || (anschlag && anschlag_ko) || val_out_of_range)
         simu_erg_werte [RESTWEG_IDX] = rest_weg;
      else
         simu_erg_werte [RESTWEG_IDX] = 0.0;
      if (mit_energie)
      { /* Erfassung des Energieverbrauchs */
	 if (zeit > 0.0)
	 { /* Energiemass berechnen. Bereich: 0.. 100.0 */
	    rwert = summedv / zeit + summeda; /* aus Gesamtgeschw. u. -beschl */
	    if (rwert == 0.0)
	       rwert = bahndistanz / bildtakt;       /* falls sonst zu gering */
	    if (rwert < 0.0)
	       rwert = 0.0;
#ifdef MITSIMULOG
	    if (nichtevo)
	    {
	       sprintf (meld_buf, ENERGIE_ERG_LOG_TXT, summedv, summeda, rwert);
	       write_to_simulog (meld_buf, CLOSE_LOG);
	    }
#endif /* MITSIMULOG */
	 } /* Energiemass berechnen. Bereich: 0.. 100.0 */
	 else
	    rwert = 0.0;
	 simu_erg_werte [ENERGIE_IDX] = rwert;
      } /* Erfassung des Energieverbrauchs */
      if (nichtevo && mit_simu_anim)
      { /* Interaktiv und mit Animation */
	 sprintf (meld_buf, GES_ERG_ALLG_TXT, bahndistanz, rest_weg);
	 if (geradeninterpol)
	 {
	    sprintf (hbuf, GES_ERG_BAHN_TXT, bges);
	    strcat (meld_buf, hbuf);
	 }
	 if (mit_energie)
	 {
	    sprintf (hbuf, GES_ERG_ENERG_TXT, summedv, summeda);
	    strcat (meld_buf, hbuf);
	 }
	 printf ("%s\n", meld_buf);
      } /* Interaktiv und mit Animation */
   } /* Ergebnisdatenaufbereitung */

   /* --------- Test: Ersatz einer Simulation: --------- */
#ifdef nix
   simu_erg_werte [0] =  0.5;  /* Ziel, vorlaeufig */
   simu_erg_werte [1] = 12.2;  /* Zeit, vorlaeufig */
   simu_erg_werte [2] =  0.08; /* Abw,  vorlaeufig */
   simu_erg_werte [3] = 38.0;  /* Len,  vorlaeufig */
   simu_erg_werte [4] = 25.0;  /* BefAnz,  vorlaeufig */
   simu_erg_werte [5] =  0.0;  /* Restweg, vorlaeufig */
   simu_erg_werte [6] =  0.0;  /* Energie, vorlaeufig */

   simu_erg_werte [0] = frand () * 30;  /* Ziel, vorlaeufig */
   simu_erg_werte [1] = frand () * 20;  /* Zeit, vorlaeufig */
   simu_erg_werte [2] = frand ();       /* Abw,  vorlaeufig */
   simu_erg_werte [3] = ch_ptr->u_el.u_xel.u_hdr.chain_length; /* Len */
   simu_erg_werte [4] = frand () * simu_erg_werte [3];  /* BefAnz */
   simu_erg_werte [5] =  0.0;  /* Restweg, vorlaeufig */
   simu_erg_werte [6] =  0.0;  /* Energie, vorlaeufig */
#endif /* nix */

   /* -------- Bewertung durch rob_sim: Berechnung der ungew. Noten: -------- */
   if (!std_simu_interface)
      for (ii = 0;  ii < akt_anz_krit;  ii++)
	 ungew_note[ii] = calc_ungew_note (ii, simu_erg_werte[ii]);

   return (simu_done);
} /* do_rob_sim */



/* ============================ update_rob_task ============================= */
BOOL update_rob_task (void)
{ /* update_rob_task */
   getBoolVal  (&mitr500,      LSKP_R500_IDX,         GLEAM_OK);
   getBoolVal  (&kollitest,    LSKP_COLLI_TEST_IDX,   GLEAM_OK);
   getBoolVal  (&anschlagtest, LSKP_STOP_TEST_IDX,    GLEAM_OK);
   getBoolVal  (&anschlag_ko,  LSKP_STOP_DEPREC_IDX,  GLEAM_OK);
   getIntVal   (&axisanz,      LSKP_AXES_NUMBER_IDX,  GLEAM_OK);
   getDoubleVal(&bildtakt,     LSKP_CYCLE_LENGTH_IDX, GLEAM_OK);
   if (kin_loaded)
   { /* Kinematik geladen */
      setze_start_ziel_frame ();
      gesamt_weg    = vdist (&(zielframe->transl), &(anfangsframe->transl));
      max_dist_gpol = gesamt_weg / 3.0;
   } /* Kinematik geladen */
   return (TRUE);
} /* update_rob_task */



/* ============================= ladekinematik ============================== */
BOOL ladekinematik (const char *file_name,
    	            INT         znr)
{
   STRING_TYPE  buffer;
   INT          achs_anz, ii;
   char         ch;
   BOOL         ok   = TRUE,
                stop = FALSE;      /* TRUE, wenn "Ende"-am ersten "E" erkannt */

   /* ladekinematik */
   for (ii = 0;  ii < MAXAXISANZ;  ii++)
   {
      robotjoints[ii]       = start_joints[ii];
      oldrobotjoints[ii]    = start_joints[ii];
      beschlrobotjoints[ii] = start_joints[ii];
   }
   if (mitr500)
   { /* R500 */
      init_mitsubishi ();
      write_buf (znr, MITSU_LOAD_TXT);
   } /* R500 */
   else
   { /* freidefinierter Roboter */
      if ((infile = fopen (file_name, "rt")) == NULL)
	 return (FALSE);
      else
      { /* File ist offen */
	 achs_anz = 0;
	 for (ii = 0;  ii < 4;  ii++)
	    ch_tmp = fgets (buffer, MAX_STRING, infile); /*skip 4 header lines*/
	 while (!feof(infile) && !stop && ok)
	 { /* Einlese-Schleife */
	    if (achs_anz == MAXAXISANZ)
	    { /* Meldung zuvieler Achsen */
	       stop = TRUE;                   /* SIMU_RZ_MELD bringt nur Text */
	       sprintf (meld_buf, ZUVIEL_ACHS_TXT, MAXAXISANZ);
	       meldung (SIMU_ROB_SIM, NO_INFO, NO_INFO, meld_buf, SIMU_RZ_MELD);
	    } /* Meldung zuvieler Achsen */
	    ch = fgetc (infile);
	    if ((ch == 'A') && !stop)
	    { /* Achs-Zeile */
	       do
		  ch = fgetc (infile);
	       while ((ch != ':') && (ch != CR));    /* ueberlese "Achse i :" */
	       do
		  ch = fgetc (infile);
	       while ((ch == ' ') && (ch != CR));         /* ueberlese Blanks */
	       switch (ch)
	       { /* Achsenkennung */
		  case 'X': axisdatfeld[achs_anz].axrichtg = XR; break;
		  case 'Y': axisdatfeld[achs_anz].axrichtg = YR; break;
		  case 'Z': axisdatfeld[achs_anz].axrichtg = ZR; break;
		  default:  ok = FALSE;
	       } /* Achsenkennung */
	       do
		  ch = fgetc (infile);
	       while ((ch != '/') && (ch != CR));           /* vor Achslaenge */
	       if (ok)
		  ok=fscanf(infile, "%d", &(axisdatfeld[achs_anz].axlaenge))==1;
	       do
		  ch = fgetc (infile);
	       while ((ch != '/') && (ch != CR));           /* vor Achsbreite */
	       if (ok)
		  ok=fscanf(infile, "%d", &(axisdatfeld[achs_anz].axbreite))==1;
	       do
		  ch = fgetc (infile);
	       while ((ch != '/') && (ch != CR));           /* vor Achshoehe */
	       if (ok)
		  ok=fscanf(infile, "%d", &(axisdatfeld[achs_anz].axhoehe)) ==1;
	       do
		  ch = fgetc (infile);
	       while ((ch != '/') && (ch != CR));        /* vor Rotationsart  */
      	       do
		  ch = fgetc (infile);
	       while ((ch == ' ') && (ch != CR));        /* Blanks ueberlesen */
      	       switch (ch)
	       { /* Rotationsart */
		  case 'f': axisdatfeld[achs_anz].bewart = FEST;  break;
		  case 'r': axisdatfeld[achs_anz].bewart = ROT;   break;
		  case 't': axisdatfeld[achs_anz].bewart = TRANS; break;
		  default:  ok = FALSE;
	       } /* Rotationsart */
	       do
		  ch = fgetc (infile);
	       while ((ch != '/') && (ch != CR));       /* vor Rotationsachse */
      	       do
		  ch = fgetc (infile);
	       while ((ch == ' ') && (ch != CR));       /* Blanks ueberlesen  */
      	       switch (ch)
	       { /* Rotationsachse */
		  case 'X': axisdatfeld[achs_anz].rotax = XR; break;
		  case 'Y': axisdatfeld[achs_anz].rotax = YR; break;
		  case 'Z': axisdatfeld[achs_anz].rotax = ZR; break;
		  default:  ok = FALSE;
	       } /* Rotationsachse */
	       ch_tmp = fgets (buffer, MAX_STRING, infile);  /* skip rest of line */
	       achs_anz++;
	    } /* Achs-Zeile */
	    else
	    { /* keine Achs-Zeile */
	       stop = TRUE;
	       ok   = ok && (ch == 'E');
	    } /* keine Achs-Zeile */
	 } /* Einlese-Schleife */
	 if (stop && ok)
	    if (axisanz == achs_anz)
	    { /* Alles paletti! */
	       sprintf (meld_buf, ACHS_OK_TXT, axisanz);
	       write_buf (znr, meld_buf);
	       robmodinit (achs_anz);
	       ktrwrobmod (robotjoints, axisanz, OHNEORI);
#ifdef MIT_ANIM
	       zeigrobmod (pm, &(p[0]), axisanz);
#endif /* MIT_ANIM */
	       for (ii = 0;  ii < MAXAXISANZ;  ii++)
	       {
		  anschlag_oben[ii]  =  160.0 * GRADRAD;
		  anschlag_unten[ii] = -160.0 * GRADRAD;
	       }
	    } /* Alles paletti! */
	    else
	    { /* Achsanzahl stimmt nicht! */
	       sprintf (meld_buf, ACHSANZ_UNGL_TXT, axisanz, achs_anz);
	       fehler (SIMU_ROB_SIM, 3, NO_INFO, NO_INFO, meld_buf, SIMU_RZ_MELD);
	       ok = FALSE;
	    } /* Achsanzahl stimmt nicht! */
	 else
	 { /* Fehler */
	    sprintf (meld_buf, ACHS_FEHLER_TXT, achs_anz);
	    fehler (SIMU_ROB_SIM, 4, NO_INFO, NO_INFO, meld_buf, SIMU_RZ_MELD);
	    ok = FALSE;                       /* SIMU_RZ_MELD bringt nur Text */
	 } /* Fehler */
	 fclose (infile);
      } /* File ist offen */
   } /* freidefinierter Roboter */
   kin_loaded = ok;
   if (kin_loaded)
   { /* Kinematik geladen */
      setze_start_ziel_frame ();
      gesamt_weg    = vdist (&(zielframe->transl), &(anfangsframe->transl));
      max_dist_gpol = gesamt_weg / 3.0;
   } /* Kinematik geladen */
   else
      write_buf (znr, KIN_ERR_TXT);
   return (ok);
} /* ladekinematik */



/* ============================== del_obstacles ============================= */
static void del_obstacles (void)
{
   HINDERNISZEIGER  neueshind;

   /* del_obstacles */
   if (hindernisanf != NULL)
   { /* Hindernisse loeschen: */
      hindernis = hindernisanf;
      do
      { /* Hindernis-Kette von vorn loeschen: */
	 neueshind = hindernis->next;
	 free (hindernis);
	 hindernis = neueshind;
      }
      while (hindernis != NULL);
      hindernisanf = NULL;
   } /* Hindernisse loeschen */
} /* del_obstacles */



/* ================================ hole_obs ================================ */
BOOL hole_obs (const char *file_name,
	       INT         znr)
{
   STRING_TYPE      buffer;
   HINDERNISZEIGER  neueshind;
   INT              x, y, z, b, l, h, ii;
   char             ch;
   BOOL             ok   = TRUE,
                    stop = FALSE;  /* TRUE, wenn "Ende"-am ersten "E" erkannt */

   /* hole_obs */
   if ((infile = fopen (file_name, "rt")) == NULL)
   { /* File gibt's nicht */
      strcpy (buffer, file_name);               /* to make the compiler happy */
      fehler (SIMU_ROB_SIM, 5, NO_INFO, NO_INFO, buffer, FILE_NOT_EXISTS);
      return (FALSE);
   } /* File gibt's nicht */
   else
   { /* File ist offen */
      ii = 0;
      del_obstacles ();
      ch_tmp = fgets (buffer, MAX_STRING, infile);       /* skip header line  */
      ch_tmp = fgets (buffer, MAX_STRING, infile);       /* skip header line  */
      ch = fgetc (infile);
      if (ch != 'H')
      { /* "Hindernis ..."-Zeile fehlt */
	 stop = TRUE;
	 ok   = FALSE;
	 fehler (SIMU_ROB_SIM, 6, NO_INFO, NO_INFO, NO_HIND_LINE_TXT, 
		 SIMU_RZ_MELD);
      } /* "Hindernis ..."-Zeile fehlt */
      else
      { /* Kettenbeginn */
	 hindernisanf = (HINDERNISZEIGER)malloc(sizeof(HINDERNISTYP));
	 neueshind    = hindernisanf;
      } /* Kettenbeginn */
      ch_tmp = fgets (buffer, MAX_STRING, infile);       /* skip rest of line */
      while (!feof(infile) && !stop && ok)
      { /* Einlese-Schleife */
	 ch = fgetc (infile);
	 if (ch == 'E')
	    stop = TRUE;                      /* "Ende"-am ersten "E" erkannt */
	 else
	 { /* Hinderniss-Zeile einlesen */
            hindernis = neueshind;
	    ii++;
	    ok = fscanf (infile, "%s%d%d%d%d%d%d", (char*)&(hindernis->name), 
			 &x, &y, &z, &b, &l, &h) == 7;
	    ch_tmp = fgets (buffer, MAX_STRING, infile); /* skip rest of line */
            hindernis->daten.hul.x = (DOUBLE)x;
            hindernis->daten.hul.y = (DOUBLE)y;
            hindernis->daten.hul.z = (DOUBLE)z;
            hindernis->daten.vul.x = (DOUBLE)x + b;
            hindernis->daten.vul.y = (DOUBLE)y;
            hindernis->daten.vul.z = (DOUBLE)z;
            hindernis->daten.hur.x = (DOUBLE)x;
            hindernis->daten.hur.y = (DOUBLE)y + l;
            hindernis->daten.hur.z = (DOUBLE)z;
            hindernis->daten.vur.x = (DOUBLE)x + b;
            hindernis->daten.vur.y = (DOUBLE)y + l;
            hindernis->daten.vur.z = (DOUBLE)z;
            hindernis->daten.hol.x = (DOUBLE)x;
            hindernis->daten.hol.y = (DOUBLE)y;
            hindernis->daten.hol.z = (DOUBLE)z + h;
            hindernis->daten.vol.x = (DOUBLE)x + b;
            hindernis->daten.vol.y = (DOUBLE)y;
            hindernis->daten.vol.z = (DOUBLE)z + h;
            hindernis->daten.hor.x = (DOUBLE)x;
            hindernis->daten.hor.y = (DOUBLE)y + l;
            hindernis->daten.hor.z = (DOUBLE)z + h;
            hindernis->daten.vor.x = (DOUBLE)x + b;
            hindernis->daten.vor.y = (DOUBLE)y + l;
            hindernis->daten.vor.z = (DOUBLE)z + h;
            neueshind = (HINDERNISZEIGER)malloc(sizeof(HINDERNISTYP));
            hindernis->next = neueshind;                  /* neues Kettenende */
	 } /* Hinderniss-Zeile einlesen */
      } /* Einlese-Schleife */
      if (ii > 0)
      { /* Es wurde eine Hindernisliste angelegt */
        free (neueshind);
        hindernis->next = NULL;                       /* Kettenende markieren */
      } /* Es wurde eine Hindernisliste angelegt */
      if (stop && ok)
      { /* Keine Fehler */
	 sprintf (buffer, HIND_OK_TXT, ii);
	 write_buf (znr, buffer);
	 ktrwrobmod (robotjoints, axisanz, OHNEORI);     /* bewirkt HindCheck */
      } /* Keine Fehler */
      else
      { /* Fehler */
	 sprintf (buffer, HIND_ERR_TXT);
	 write_buf (znr, buffer);
	 sprintf (buffer, HIND_FEHLER_TXT, ii);
	 fehler (SIMU_ROB_SIM, 7, NO_INFO, NO_INFO, buffer, SIMU_RZ_MELD);
	 del_obstacles ();                    /* SIMU_RZ_MELD bringt nur Text */
      } /* Fehler */
      fclose (infile);
   } /* File ist offen */
   return (ok);
} /* hole_obs */



/* =========================== hindernismanagement ========================== */
void hindernismanagement (void)
{
   meldung (SIMU_ROB_SIM, NO_INFO, NO_INFO, "", NOT_YET_IMPLEMENTED);
} /* hindernismanagement */



/* ============================= write_code_file ============================ */
BOOL write_code_file (const char *fspec, 
	              INT         guete, 
	              INT         lfd_nr)
{
   CHAIN_ELEM_PTR_TYPE  ch_ptr;
   GSTATE               erg_state = GLEAM_OK;
   BOOL                 erg       = FALSE;

   /* write_code_file */
   if ((infile = fopen (fspec, "wt")) == NULL)
      return (FALSE);
   if (get_chain (&ch_ptr, &lfd_nr, AK, guete, &guete, &erg_state))
   { /* AK ok */

      fprintf (infile, "Test! Code der AK %d/%d.\n", guete, lfd_nr);
      erg = TRUE;    /* vorlaeufig !!! */
   } /* AK ok */
   fclose (infile);
   
   return (erg);
} /* write_code_file */



/* ================================ rob_init ================================ */
BOOL rob_init (void)
{
   BOOL  ok;

   /* rob_init */
   ok = pasromin_init ();
   if (ok)
   {
      ok        = rob_gsw_init ();
      thmodziel = new_joints ();
      ok       &= (thmodziel != NULL) &&
	          ((hbuf = (char*)malloc(MAX_STR*sizeof(char))) != NULL);
   }
   kin_loaded = FALSE;
   nichtevo   = TRUE;
   return (ok);
} /* rob_init */



#if defined MIT_ANIM || defined MITSIMULOG
/* ================================ compstr ================================= */
static void compstr (char *str,
                     INT   vgl_op)
{ /* COMPSTR/SIMU */
   strcpy (str, " ");
   switch (vgl_op)
   {
      case EQ: strcpy (str, " = ");  break;
      case NE: strcpy (str, " <> "); break;
      case GT: strcpy (str, " > ");  break;
      case LT: strcpy (str, " < ");  break;
   } /* CASE */
} /* compstr */ 
#endif /* MIT_ANIM || MITSIMULOG */


#ifdef MIT_ANIM
/* ============================== abbruchtest =============================== */
static void abbruchtest (void)
{
  char  ch;
  
  /* ABBRUCHTEST/SIMU */
#ifdef TEXT_MODE
  if (kb_hit ())
  {
    ch = get_kb_char ();                                  /* ignore character */
    ch = char_dialog (SIMU_WUERG_QUERY_TXT);
    if (ch == JA)
    { /* Simulation abgebrochen */
      printf (MSG_LINE_TXT, SIMU_WUERG_TXT);
      simuabbruch  = TRUE;
      user_abbruch = TRUE;
#ifdef MITSIMULOG
      sprintf (meld_buf, SIMU_WUERG_LOG_TXT, takt);
      write_to_simulog (meld_buf, CLOSE_LOG);
#endif /* MITSIMULOG */
    } /* Simulation abgerbochen */
    else
      printf (MSG_LINE_TXT, SIMU_NOT_WUERG_TXT);
  } /* KeyPressed */
#endif /* TEXT_MODE */
} /* abbruchtest */ 
#endif /* MIT_ANIM */



/* ============================== motor_fehlt =============================== */
static void motor_fehlt (INT  motor)
{
   /* MOTOR_FEHLT/SIMU */
   sprintf (meld_buf, NIX_MOTOR_TXT, motor); /* SIMU_RZ_MELD bringt nur Text */
   fehler (SIMU_ROB_SIM, 8, NO_INFO, NO_INFO, meld_buf, SIMU_RZ_MELD);
#ifdef MITSIMULOG
   write_to_simulog (meld_buf, CLOSE_LOG);
#endif /* MITSIMULOG */
#ifdef MIT_ANIM
#endif /* MIT_ANIM */
} /* motor_fehlt */ 



/* ================================= setzeu ================================= */
static void setzeu (INT  motor)
{
   DOUBLE  ramp, uwert;
  
   /* SETZEU/AKTINTERPRETER/SIMU */
   if ((0 <= motor) && (motor < axisanz))
   { /* Falls Achse IN [1..axisanz] */
      ramp  = act_item->u_el.u_xel.u_act.r_params[RAMPE];
      uwert = act_item->u_el.u_xel.u_act.r_params[U_WERT];
      if (ramp < RAMPE_MIN)
      { /* Rampe zu klein */
	 sprintf (hbuf, "SimuAbbruch da Rampe zu klein! Min=%g Ist=%g",
                  RAMPE_MIN, ramp);
	 ramp             = RAMPE_MIN;
	 simuabbruch      = TRUE;
      } /* Rampe zu klein */
      else
	 if (RAMPE_MAX_AN < ramp)
         { /* Rampe zu gross */
	    sprintf (hbuf, "SimuAbbruch da Rampe zu gross! Max=%g Ist=%g",
		     RAMPE_MAX_AN, ramp);
	    ramp             = RAMPE_MAX_AN;
	    simuabbruch      = TRUE;
	 } /* Rampe zu gross */
      if (uwert < SPANNUNG_MIN)
      { /* Spannung zu klein */
	 sprintf (hbuf, "SimuAbbruch da Spannung zu klein! Min=%g Ist=%g",
                  SPANNUNG_MIN, uwert);
	 uwert            = SPANNUNG_MIN;
	 simuabbruch      = TRUE;
      } /* Spannung zu klein */
      else
	 if (SPANNUNG_MAX < uwert)
         { /* Spannung zu gross */
	    sprintf (hbuf, "SimuAbbruch da Spannung zu gross! Max=%g Ist=%g",
		     SPANNUNG_MAX, uwert);
	    uwert            = SPANNUNG_MAX;
	    simuabbruch      = TRUE;
	 } /* Spannung zu gross */
      if (simuabbruch)
      { /* Simu-Abbruch wegen Wertebereichsueberschreitung */
	 val_out_of_range = TRUE;
	 meldung (SIMU_ROB_SIM, NO_INFO, NO_INFO, hbuf, SIMU_RZ_MELD);
#ifdef MITSIMULOG
	 write_to_simulog (hbuf, CLOSE_LOG);
#endif /* MITSIMULOG */
      } /* Simu-Abbruch wegen Wertebereichsueberschreitung */
      steuerfeld[motor].spannung = uwert;
      steuerfeld[motor].rampe    = ramp;
#if defined MIT_ANIM || defined MITSIMULOG
      if (nichtevo)
      { /* interaktiver Aufruf */
	 sprintf (meld_buf, MOTOR_AN_LOG_TXT, motor, uwert, ramp);
#ifdef MITSIMULOG
	 write_to_simulog (meld_buf, CLOSE_LOG);
#endif /* MITSIMULOG */
#ifdef MIT_ANIM
#endif /* MIT_ANIM */
      } /* interaktiver Aufruf */
#endif /* MIT_ANIM || MITSIMULOG */
   } /* Falls Achse IN [1..axisanz] */
   else
      motor_fehlt (motor);
} /* setzeu */ 



/* ================================ motoraus ================================ */
static void motoraus (INT  motor)
{
   DOUBLE  ramp;
  
   /* MOTORAUS/AKTINTERPRETER/SIMU */
   if ((0 <= motor) && (motor < axisanz))
   { /* Falls Achse IN [1..axisanz] */
      ramp = act_item->u_el.u_xel.u_act.r_params[RAMPE];
      if (ramp < RAMPE_MIN)
      { /* Rampe zu klein */
	 sprintf (hbuf, "SimuAbbruch da Rampe zu klein! Min=%g Ist=%g",
                  RAMPE_MIN, ramp);
	 ramp             = RAMPE_MIN;
	 simuabbruch      = TRUE;
      } /* Rampe zu klein */
      else
	 if (RAMPE_MAX_AUS < ramp)
         { /* Rampe zu gross */
	    sprintf (hbuf, "SimuAbbruch da Rampe zu gross! Max=%g Ist=%g",
		     RAMPE_MAX_AUS, ramp);
	    ramp             = RAMPE_MAX_AUS;
	    simuabbruch      = TRUE;
	 } /* Rampe zu gross */
      if (simuabbruch)
      { /* Simu-Abbruch wegen Wertebereichsueberschreitung */
	 val_out_of_range = TRUE;
	 meldung (SIMU_ROB_SIM, NO_INFO, NO_INFO, hbuf, SIMU_RZ_MELD);
#ifdef MITSIMULOG
	 write_to_simulog (hbuf, CLOSE_LOG);
#endif /* MITSIMULOG */
      } /* Simu-Abbruch wegen Wertebereichsueberschreitung */
      steuerfeld[motor].spannung = 0.0;
      steuerfeld[motor].rampe    = ramp;
#if defined MIT_ANIM || defined MITSIMULOG
      if (nichtevo)
      { /* interaktiver Aufruf */
	 sprintf (meld_buf, MOTOR_AUS_LOG_TXT, motor, ramp);
#ifdef MITSIMULOG
	 write_to_simulog (meld_buf, CLOSE_LOG);
#endif /* MITSIMULOG */
#ifdef MIT_ANIM
#endif /* MIT_ANIM */
      } /* interaktiver Aufruf */
#endif /* MIT_ANIM || MITSIMULOG */
   } /* Falls Achse <= axisanz */
   else
      motor_fehlt (motor);
} /* motoraus */ 



/* ============================= aktinterpreter ============================= */
static void aktinterpreter (void)
{
   char    buf [4];
   INT     ii, var_nr;
   DOUBLE  distanz;
   BOOL    tcpvergl;
   VECTOR  pos;
  
   /* AKTINTERPRETER/SIMU */
   if (nextak == NULL)
      simuende = TRUE;
   else
   { /* Falls nicht NIL-Pointer */
      bef_ctr++;
      act_item = nextak;
      nextak   = nextak->u_el.next_action;
      switch (act_item->u_el.activity)
      {
	 case AK_HEADER:
	 case VK_HEADER:
	 fehler (SIMU_ROB_SIM, 10, NO_INFO, NO_INFO, BAD_AK_ELEM_TXT, 
		 SIMU_RZ_MELD);               /* SIMU_RZ_MELD bringt nur Text */
#ifdef MITSIMULOG
	 write_to_simulog (BAD_AK_ELEM_TXT, CLOSE_LOG);
#endif /* MITSIMULOG */
	 break;  /* .._HEADER */
        
	 case ACTIVITY_ACT:
	 fehler (SIMU_ROB_SIM, 11, NO_INFO, NO_INFO, NO_SUB_AK_TXT, 
		 SIMU_RZ_MELD);               /* SIMU_RZ_MELD bringt nur Text */
#ifdef MITSIMULOG
	 write_to_simulog (NO_SUB_AK_TXT, CLOSE_LOG);
#endif /* MITSIMULOG */
	 break; /* ACTIVITY_ACT */
        
	 case MOTOR_1:
	 setzeu (0);
	 break;

	 case MOTOR_2:
	 setzeu (1);
	 break;

	 case MOTOR_3:
	 setzeu (2);
	 break;

	 case MOTOR_4:
	 setzeu (3);
	 break;

	 case MOTOR_5:
	 setzeu (4);
	 break;

	 case MOTOR_1_AUS:
	 motoraus (0);
	 break;

	 case MOTOR_2_AUS:
	 motoraus (1);
	 break;

	 case MOTOR_3_AUS:
	 motoraus (2);
	 break;

	 case MOTOR_4_AUS:
	 motoraus (3);
	 break;

	 case MOTOR_5_AUS:
	 motoraus (4);
	 break;

	 case START_BLOCK_BEGIN:
	 startblock = TRUE;
	 bef_ctr--;                  /* do not count actions for time control */
#if defined MIT_ANIM || defined MITSIMULOG
	 if (nichtevo)
	 { /* interaktiver Aufruf */
#ifdef MITSIMULOG
	    write_to_simulog (START_BLOCK_ANF_LOGTXT, CLOSE_LOG);
#endif /* MITSIMULOG */
#ifdef MIT_ANIM
#endif /* MIT_ANIM */
	 } /* interaktiver Aufruf */
#endif /* MIT_ANIM || MITSIMULOG */
	 break; /* START_BLOCK_BEGIN */
        
	 case START_BLOCK_END:
	 startblock = FALSE;
	 bef_ctr--;                  /* do not count actions for time control */
#if defined MIT_ANIM || defined MITSIMULOG
	 if (nichtevo)
	 { /* interaktiver Aufruf */
#ifdef MITSIMULOG
	    write_to_simulog (START_BLOCK_END_LOGTXT, CLOSE_LOG);
#endif /* MITSIMULOG */
#ifdef MIT_ANIM
#endif /* MIT_ANIM */
	 } /* interaktiver Aufruf */
#endif /* MIT_ANIM || MITSIMULOG */
	 break; /* START_BLOCK_END */
        
	 case UNVERAENDERT:
	 bef_ctr--;                  /* do not count actions for time control */
	 unveraendert_anz = act_item->u_el.u_xel.u_act.i_params[TAKT_ANZAHL];
#if defined MIT_ANIM || defined MITSIMULOG
	 if (nichtevo)
	 { /* interaktiver Aufruf */
	    sprintf (meld_buf, UNVERAENDERT_LOG_TXT, unveraendert_anz);
#ifdef MITSIMULOG
	    write_to_simulog (meld_buf, CLOSE_LOG);
#endif /* MITSIMULOG */
#ifdef MIT_ANIM
#endif /* MIT_ANIM */
	 } /* interaktiver Aufruf */
#endif /* MIT_ANIM || MITSIMULOG */
	 break; /* UNVERAENDERT */
        
	 default:  /* keine gescheite Aktion erkannt: */
	 bef_ctr--;
	 sprintf (meld_buf, BAD_AKTION_TXT, act_item->u_el.activity);
	 fehler (SIMU_ROB_SIM, 16, NO_INFO, NO_INFO, meld_buf, SIMU_RZ_MELD);
#ifdef MITSIMULOG                             /* SIMU_RZ_MELD bringt nur Text */
	 write_to_simulog (meld_buf, CLOSE_LOG);
#endif /* MITSIMULOG */
      } /* CASE activity */
   } /* Falls nicht NIL-Pointer */
} /* aktinterpreter */ 



/* ================================ eintakt ================================= */
static void eintakt (void)
{
   INT     i, znr;
   DOUBLE  spannung, steuerfeldispannung;
#ifdef MIT_ANIM
   INT     k, a, bahnpos;
#endif /* MIT_ANIM */
   DOUBLE  kquad, rquad, bahndist,
           d, wurzelausdruck;
  
   /* EINTAKT/SIMU */
   if (startblock)
      return;
  
   for (i = 0;  i < axisanz;  i++)      
   { /* Schleife ueber alle Roboter-Achsen */
      spannung = aktufeld[i];
      steuerfeldispannung = steuerfeld[i].spannung;
      if ((steuerfeld[i].rampe > 0.0))
      {
	 if ((steuerfeldispannung > spannung))
	 {
	    spannung += steuerfeld[i].rampe;
	    if (steuerfeldispannung <= spannung)
	    {
	       spannung            = steuerfeldispannung;
	       steuerfeld[i].rampe = 0.0;
	    }
	 }
	 if ((steuerfeldispannung < spannung))
	 {
	    spannung -= steuerfeld[i].rampe;
	    if (steuerfeldispannung >= spannung)
	    {
	       spannung            = steuerfeldispannung;
	       steuerfeld[i].rampe = 0.0;
	    }
	 }
      } /* Rampenfaktor > 0 */
      thmodziel[i] = robotjoints[i] + spannung * bildtakt * GRADRAD;
      if (anschlagtest && ((thmodziel[i] > anschlag_oben[i]) ||
			   (thmodziel[i] < anschlag_unten[i])  ))
      { /* Achse am Anschlag und Anschlagtest */
	 if (nichtevo)
	 { /* interaktiver Aufruf */
	    sprintf (meld_buf, ACHS_AM_ANSCHLAG_TXT, i+1);
	    meldung (SIMU_ROB_SIM, NO_INFO, NO_INFO, meld_buf, SIMU_RZ_MELD);
#ifdef MITSIMULOG
	    write_to_simulog (meld_buf, CLOSE_LOG);
#endif /* MITSIMULOG */
#ifdef MIT_ANIM
#endif /* MIT_ANIM */
	 } /* interaktiver Aufruf */
	 aktufeld[i]            = 0.0;
	 steuerfeld[i].rampe    = 0.0;
	 steuerfeld[i].spannung = 0.0;
	 if (thmodziel[i] > anschlag_oben[i])
	    thmodziel[i] = anschlag_oben[i];
	 else
	    thmodziel[i] = anschlag_unten[i];
	 if (anschlag_ko)
	 {
	    anschlag    = TRUE;
	    simuabbruch = TRUE;
	 } /* anschlag_ko */
      } /* Achse am Anschlag und Anschlagtest */
      else
      { /* Achse NICHT am Anschlag oder KEIN Anschlagtest : */
	 while (thmodziel[i] >= ZWEIPI)            /* evtl. Winkel unter 2*pi */
	    thmodziel[i] = thmodziel[i]-ZWEIPI;    /* ruecksetzen             */
	 while (thmodziel[i] <= -ZWEIPI)           /* evtl. Winkel unter 2*pi */
	    thmodziel[i] = thmodziel[i] + ZWEIPI;  /* ruecksetzen             */
	 aktufeld[i] = spannung;
      } /* Achse NICHT am Anschlag oder KEIN Anschlagtest */
   } /* Schleife ueber alle Roboter-Achsen */
   memmove (beschlrobotjoints, oldrobotjoints, joints_mem_size);
   memmove (oldrobotjoints, robotjoints, joints_mem_size);
   memmove (robotjoints, thmodziel, joints_mem_size);
   if (geradeninterpol)
      memmove (oldrobotframe, robotframe, sizeof(FRAME));
   if (mitr500)
   {
      r500kollicheck (robotjoints);
      posktrwr500 (&(robotframe->transl), robotjoints);
   }
   else
   {
      if (geradeninterpol)
	 ktrwrobmod (robotjoints, axisanz, OHNEORI);
   }
   if (geradeninterpol)
   { /* Berechnungen fuer Bewertung der Bahnabweichung */
      kquad = fsqr (robotframe->transl.x - anfangsframe->transl.x) +
	      fsqr (robotframe->transl.y - anfangsframe->transl.y) +
	      fsqr (robotframe->transl.z - anfangsframe->transl.z);
      rquad = fsqr (zielframe->transl.x - robotframe->transl.x) +
	      fsqr (zielframe->transl.y - robotframe->transl.y) +
	      fsqr (zielframe->transl.z - robotframe->transl.z);
      if (zweibahndistanz > 0.000001)
	 wurzelausdruck = kquad - fsqr (((kquad - rquad) / zweibahndistanz) +
					bahndistanzhalbe);
      else
	 wurzelausdruck = kquad;
      if (wurzelausdruck >= 0.0)
	 bahndist = sqrt (wurzelausdruck);
      else
	 bahndist = sqrt (rquad);
      d    = vdist (&(robotframe->transl), &(oldrobotframe->transl));
      bges = bges + d*bahndist;
   } /* Berechnungen fuer Bewertung der Bahnabweichung */
  
   if (mit_energie)
      for (i = 0;  i < axisanz;  i++)
      { /* Berechnungen fuer Bewertung des Energieverbrauchs */
	 dv[i]    = fabs (robotjoints[i] - oldrobotjoints[i]);
	 summedv  = summedv + dv[i];               /* nur Abstand gespeichert */
	 summeda  = summeda + fabs (dv[i] - dvold[i]) / (2.0*bildtakt*bildtakt);
	 dvold[i] = dv[i];
      } /* Berechnungen fuer Bewertung des Energieverbrauchs */
   zeit = takt * bildtakt;
   if (nichtevo)
   { /* Falls "nichtevo", d.h. interaktive Simulation */
      if (gtakt > MAXBAHNP + 50)
	 gtakt -= 50;
      if (mit_simu_anim)
      {
#ifdef MIT_ANIM
	 if (auslaufen)
	    set_simu_zusatz_win ();
#endif /* MIT_ANIM */
	 if (takt > 0)
	 { /* Nicht bei Nullpos anfahren */
	    sprintf (meld_buf, TAKT_ZEIT_TXT, takt, zeit);
	    printf ("%s\n", meld_buf);
	 } /* Nicht bei Nullpos anfahren */
#ifdef MIT_ANIM
	 clear_simu_win (PARTIAL);
	 bahnpos = gtakt;
	 if (bahnpos > MAXBAHNP)
	 {
	    for (i = 1;  i <= MAXBAHNP;  i++)
	    {
	       p[i-1].x = p[i].x;
	       p[i-1].y = p[i].y;
	    }
	    bahnpos = MAXBAHNP;
	 }
	 drawkoordsyst (&graphkoorsys, pm);
	 if (mitr500)
	    zeigr500 (robotr500, robotjoints, pm, &(p[bahnpos]), OHNESTRAHL);
	 else
	 {
	    if (!(geradeninterpol))
	       ktrwrobmod (robotjoints, axisanz, OHNEORI);
	    zeigrobmod (pm, &(p[bahnpos]), axisanz);
	 }
	 if (hindernisanf != NULL)
	 {
	    _setcolor (YELLOW);
	    hindernis = hindernisanf;
	    do
	    {
	       drawcubus (&(hindernis-> daten), pm);
	       hindernis = hindernis-> next;
	    }
	    while (hindernis != NULL);
	    _setcolor (WHITE);
	 }
	 set_simu_win ();
	 _setcolor (LIGHTGREEN);
	 _moveto (p[0].x + mittelp.x, p[0].y + mittelp.y);
	 for (k = 1;  k <= bahnpos;  k++)
	    _lineto (p[k].x + mittelp.x, p[k].y + mittelp.y);
	 _setcolor (WHITE);
#endif /* MIT_ANIM */
	 znr = 1;
	 zeig_pos_joints (robotjoints, &znr);
#ifdef TEXT_MODE
	 write_text_buf (CLEAR);
#endif /* TEXT_MODE */
	 if (unveraendert_anz > 0)
	 {
	    sprintf (meld_buf, UNVERAENDERT_LOG_TXT, unveraendert_anz);
	    printf ("%s\n", meld_buf);
	 }
      } /* Robot-Grafik on */
      gtakt++;
#ifdef MITSIMULOG
      if (mit_simulog)
      { /* Mit Log-File */
	 if (!simu_logfile_open)
	    simu_logfile = fopen (simu_logfilename, "at");
	 if (unveraendert_anz > 0)
	 {
	    sprintf (meld_buf, UNVERAENDERT_LOG_TXT, unveraendert_anz);
	    fprintf (simu_logfile, "%s\n", meld_buf);
	 }
	 fprintf (simu_logfile, ZWISCHEN_ERG_TXT1);
	 for (i = 0;  i < axisanz;  i++)
	    fprintf (simu_logfile, ZWISCHEN_ERG_TXT2, i+1, aktufeld[i], 
		     steuerfeld[i].rampe, steuerfeld[i].spannung);

	 fprintf (simu_logfile, ZWISCHEN_ERG_TXT3);
	 for (i = 1;  i <= axisanz;  i++)
	 {
	    fprintf (simu_logfile, " %5.1lf", robotjoints[i-1]*RADGRAD);
	    if (i < axisanz)
	       fputc (',', simu_logfile);
	 }
	 fputc ('\n', simu_logfile);
	 fprintf (simu_logfile, ZWISCHEN_ERG_TXT4, robotframe->transl.x,
		   robotframe->transl.y, robotframe->transl.z);
	 fclose (simu_logfile);
	 simu_logfile_open = FALSE;
      } /* Mit Log-File */
#endif /* MITSIMULOG */
   } /* Falls "nichtevo", d.h. interaktive Simulation */
   else
   {
      if (mitr500)
	 r500kollicheck (robotjoints);
      else
	 ktrwrobmod (robotjoints, axisanz, OHNEORI);
   }
   if (kollitest && kollision)
      simuabbruch = TRUE;
} /* eintakt */ 



/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678 */
/*3456789012345678901234567890123456789012345678901234567890123456 */
