/*******************************************************************************
GLEAM/AE                                                                  V3.0.0

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: BEWertung
                        Module: BEWertung-GrundSoftWare
 
Package: bew                    File: bew_gsw.c                   
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Der Modul enthaelt Routinen fuer die Anzeigefunktionen. Sie hinterlegen die 
Informationen im Textpuffer des "fbhm"-Packages.


Die Routinenim einzelnen:

void make_krit_text (char       *erg_buf,
                     const char *txt_buf,
                     INT         index);
          Aus der komprimierten Bezeichnung des Kriteriums mit Index "index", 
          der das terminierende ":" entfernt wurde, wird mit Hilfe des Format-
          strings "txt_buf" der Ergebnisstring "erg_buf" gebildet und abgelie-
          fert.

void   prep_gnu_plot (INT   k_index,
                      BOOL  simRes);
          Bereitet die Ausgabe der Bewertungsfuntion mittels des GnuPlots vor. In 
          der Datei "gnu_com_file" (Extention ".cmd") werden alle an den GnuPlot
          gesandten Befehle mitgeschrieben. In den verschiedenen Dateien "gnu_dat_
          file" werden die Kurvenwerte hinterlegt. Die Filenamen beginnen mit 
          "k<ii>", wobei <ii> fuer die Nummer des Kriteriums steht. Die Extention
          ist immer ".tmp". Es gibt folgende Files: Kurve der Bewertungsfunktion
          (k<ii>.tmp), Stuetzpunkte (k<ii>_lim.tmp) und Erfüllungswert (k<ii>_
          erf.tmp). Der GnuPlot-Kommandostring wird in "gnu_buffer" aufgebaut. Bei
          nicht gesetztem "simRes", d.h. im Kontext des Bewertungs-Menues, wird
          das "gnu_com_file" geschlossen und "gnu_buffer" enthaelt lediglich die
          Anweisung "load 'k<ii>.cmd'". Bei gesetztem "simRes", d.h. im Kontext
          des Simulator-Menues, bleibt das "gnu_com_file" offen und "gnu_buffer" 
          enthaelt das bisher aufgebaute "plot"-Kommando. 
          Bei noch nicht definiertem Algorithmus (bew_alg == NIX_IDX) geschieht 
          nichts und es erfolgt eine Meldung.

void   gnu_hard_copy (void);
          Erzeugt die Kommandosequenz zur Erzeugung eines PostScripts-Hardcopy-
          Files des aktuellen GnuPlots im "gnu_buffer".


Packagelokale Routinen, wobei "zeige_kriterien" bei GUI_MODE und "addiere_noten" 
bei nicht gesetztem TEXT_MODE zum Export gehoeren:

void compress_crit_name (STR_TYPE  erg_buf,
                         STR_TYPE  name_buf);
          "name_buf" enthaelt einen String, der mit ":" endet. Dieses Ende wird
          entfernt, danach erfolgt ein Blank-Supress rechts und der verbleibende 
          String wird ergaenzt um ein ":" in "erg_buf" abgeliefert. Bei einem
          leerem String ist dies " :".

INT zeige_kriterien (BOOL *gew_ok);
          Gibt die aktuellen Daten der benutzten Kriterien mit Hilfe der Routine 
          "zeige_krit_werte" im nachstehenden Format in den Textpuffer ("fbhm")
          aus und liefert die Anzahl angezeigter Kriterien ab. Format:
             Nr. Kriterium           Prio-      Noten-    Erfullungs-   StrafFkt Nmax
                                     Klasse     anteil    note    wert  PrioKl.  Cont
             xx: 1234567890123456789:tttxxttt (xxxxxx%) xxxxxx xxxxxxx  tttxxttt xxxx
          Bei getztem GUI_MODE wird anstelle der Spalte Notenanteil eine Spalte 
          mit dem Notengewicht in Prozent angezeigt. Die Liste endet mit der Sum-
          me der Notenanteile bzw. der Notengewichte, wobei wie in "addiere_noten" 
          gezaehlt wird.
          Erfuellungsnote und -wert werden nur angezeigt, wenn die Erfuellungsnote 
          ungleich KEINE_NOTE (= undefinierte Funktion oder Wert) ist. Andernfalls
          erscheint ein "noch undef.".
          Die Routine liefert "gew_ok" zurueck, ob die Notengewichte die Summe von 
          100% ergeben oder nicht.
          
LONG addiere_noten (void);
          Zaehlt die Maximalnoten ("max_note") aller aktiven Kriterien zusammen 
          und liefert sie als Ergebnis ab. Dabei wird die Maximalnote eines 
          Kriteriums mit nichtgesetztem "nmax_cont" nur halb gezaehlt.
          
BOOL zeige_krit_werte (INT  znr);
          Schreibt die Werte der Kriterien zeilenweise ab Zeile "znr" in den 
          Textpuffer des "fbhm"-Packages und bildet darunter die Summe der Noten-
          anteile (TEXT_MODE) bzw. der Notengewichte (GUI_MODE), wobei wie in 
          "addiere_noten" gezaehlt wird. Liefert TRUE zurueck, wenn die Noten-
          gewichte die Summe von 100% ergeben.

void zeige_x_wert (STR_TYPE  buf,
                   DOUBLE    x_wert);
          Wandelt den "x_wert" in einen 11 Zeichen langen String um und liefert
          diesen in "buf" ab. Das Format haengt wie folgt vom Wert ab:
             1: Werte mit mehr als 11 Stellen werden in Exponent-Darstellung 
                dargestellt:           -1.1234e+12 
             2: 100000 <= abs (x):  -9999999999
             3:    100 <= abs (x):  -99999.9
             4:     10 <= abs (x):      99.99 
             5:    0.1 <= abs (x):       9.9999 
             6:    0.1 >  abs (x):   0.09999999

void zeige_bew_par (INT             znr,
                    const STR_TYPE  param_txt,
                    DOUBLE          x_wert,
                    INT             fkt_index);
          Aus "param_txt", "x_wert" und "n_wert" wird eine Zeile aufgebaut, die
          die Parameternamen und den X-Wert enthaelt. Je nach "fkt_index" folgt
          der Straffaktor, sonst die ungewichtete Note sowie deren Prozentan-
          teil an der Gesamtnote. Geschrieben wird in die durch "znr" bezeichne-
          te Zeile des Textpuffers von "fbhm".

void fkt_to_text (char       *buf,
                  const char *abst_buf,
                  INT         index);
          Erzeugt fuer den zur durch "index" bezeichneten Bewertungs- oder 
          Straffunktion gehoerigen Funktionstyp einen erlaeuternden String in
          "buf". "abst_buf" verschoenert die Formatierung fuer die Ausgabe bei 
          der Funktionsaenderung.

void zeig_fkt (INT  index,
               INT  krit_index);
          Ausgabe des Typs, der Grenzen und Stuetzpunkte der Bewertungs- oder
          Straffunktion mit dem Index "index" ab Zeile 3 des Textpuffer ("fbhm"). 
          "krit_index" gibt den Index des zur Funktion gehoerigen Kriteriums an.


--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
Bereinigung des Bewertungs-Packages mit dem Ziel, alle Bewertungsroutinen darin
zusammenzufassen und einen Kern bereit zustellen, der die reine Algorithmik der 
Bewertung und das File-I/O abdeckt. Dazu wurden "compress_crit_name" und die 
GNU-Plott-Routinen "prep_gnu_plot" und "gnu_hard_copy" aus Modul "bewert" hier-
her uebernommen.
--------------------------------------------------------------------------------
Aenderung gegenueber Version V2.1.0:
"prep_gnu_plot" legt die Datenfiles mit groesserer Genauigkeit im G-Format an.
--------------------------------------------------------------------------------
Aenderung gegenueber Version V2.2.0:
 1. Aus den Int-Konstanten in "zeige_x_wert" Realkonstante gemacht.  WJ 08.08.00
 2. Linux-Portierung: Aus "round" wird "l_round". Neue Werte fuer
    "small_gnu_pos".                                                 WJ 12.05.05
 3. Besser Darstellung von Werten < 0.1 durch "zeige_x_wert".        WJ 10.11.05
 4. Deklaration von Stringparameter, die reine Eingangsparameter 
    sind und mit Stringkonstanten aufgerufen werden, als "const".    WJ 19.04.12
 5. Explizite Klammern zur eindeutigen Zuordnung von else-Zweigen. 
    Entfernung ueberfluessiger Variable.                             WJ 28.06.12
 6. Anpassungen an die neue gnuplot-Version 4.4.                     WJ 05.07.12
 7. Renovierung: Aus "prop" wird "steigend", Verbesserung der 
    Platzierung der gnuplot-Legende. Korrektur des fprintf-Aufrufs.
    "zeige_bew_par" an geaenderte Stringkonstanten angepasst. Korrektur 
    von "compress_crit_name". GnuPlot: xtics und "samll_gnu_pos" sowie 
    "big_gnu_pos" entfernt. Streichung des "link"-Includes. Anpassung
    von "zeige_krit_werte", "zeige_kriterien", "zeig_fkt" und
    "zeige_bew_par".                                                 WJ 27.04.16
 8. "gnu_win_pos" von "bew_cmen" uebernommen. Ist wie "zeige_kriterien"
    je nach GUI_MODE Package-Export oder package-lokal. "prep_gnu_plot"
    zeigt Endstuetzpunkte und blendet Stuetzpunkte und Erfuellungswerte 
    aus, die ausserhalb des angezeigten X-Intervalls liegen. Erweiterung
    von "zeige_kriterien" und "zeige_krit_werte" derart, dass die 
    Notenanteile (TEXT_MODE bzw. die Notengewichte (GUI_MODE) ange-
    zeigt und aufsummiert werden.                                    WJ 01.08.16
 9. Verbesserung der Kriterienanzeige in "zeige_krit_werte" und des
    Plottens in "prep_gnu_plot".                                     WJ 18.08.16
10. Anpassung an ueberarbeitete Bewertung: "prep_gnu_plot", "zeig_fkt" 
    und "fkt_to_text". Meldungen nutzen den Namen des Kriteriums an- 
    stelle der Nummer. Neue Farbgebung in "prep_gnu_plot".           WJ 19.09.16
11. Avoiding buffer overflow of static strings: compress_crit_name(),
    zeige_krit_werte(), zeige_bew_par().                             WJ 18.07.20
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 18.07.2020
================================================================================ */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>

#include "schalter.h"
#include "sys.h"
#include "fbhm.h"
#include "chaindef.h"
#include "bew.h"
#include "bewInt.h"

#include GLOBAL_TEXTS
#include BEW_TEXTS



#ifdef GNUPLOT
/* ========================== Exportierte Variable: ========================= */
  char        *gnu_buffer;              /* Puffer fuer "gnuplot"-Kommando     */
  FILE        *gnu_com_file;            /* Kommandofile fuer "gnuplot"        */
  char        *gnu_fname;               /* GnuPlot-File-Name ohne Extention   */
  INT          last_gnu_index = NIX_IDX;/* letzter GnuPlot-Fenster-Index      */


/* ---------- Package-Export oder package-lokal je nach GUI_MODE: ----------- */
  STR_TYPE     gnu_win_pos [MAX_GNU] =  {"+0+0","+575+0","+0+440","+575+440"};
                                             /* Positionen des Plot-Fensters  */


/* ======================== Package-globale Variable: ======================= */
  BOOL         gnuplot_done = FALSE;    /* TRUE, wenn GnuPlot mind.1x plottet */


/* ============================ Lokale Variable: ============================ */
  static FILE *gnu_dat_file;            /* Datenfile fuer "gnuplot"           */
#endif /* GNUPLOT */




/* ============================= make_krit_text ============================= */
void make_krit_text (char       *erg_buf,
                     const char *txt_buf,
                     INT         index)
{
   STR_TYPE  buf;
   
   /* make_krit_text */
   compress_crit_name (buf, kriterien[index].bezeichnung);
   buf [strlen (buf) - 1] = '\0';                             /* Entferne ":" */
   sprintf (erg_buf, txt_buf, buf);
} /* make_krit_text */ 



/* =========================== compress_crit_name =========================== */
void compress_crit_name (STR_TYPE  erg_buf,
                         STR_TYPE  name_buf)
{
   STR_TYPE  buffer, buf1;
   char     *pos_ptr;
   int       pos, amount;
   
   /* compress_crit_name */
   strcpy(buffer, name_buf);
   pos_ptr = strstr(buffer, ":");
   if (pos_ptr != NULL)
   {
      amount = strlen(name_buf) - strlen(pos_ptr);
      strncpy (buf1, buffer, amount);
      buf1[amount] = '\0';
      strcat(buf1, pos_ptr+1);
      strcpy(buffer, buf1);
   }

   pos = strlen(buffer);
   do 
      pos--;
   while ((pos > 0) && (buffer[pos] == ' '));
   buffer[pos+1] = '\0';                             /* delete rest of buffer */
   sprintf(erg_buf, "%s:", buffer);
} /* compress_crit_name */ 



#ifdef GNUPLOT
/* ============================== prep_gnu_plot ============================= */
void prep_gnu_plot (INT   k_index,
                    BOOL  simRes)
{
   STR_TYPE  gnu_dat_fname,      /* GnuPlot-Daten-Filename mit Extention      */
             gnu_long_title,     /* Titel des Plots und der X-Achse.          */
             buf;                /* Hilfs-Puffer                              */
   DOUBLE    x_wert,             /* X-Wert zur Berechnung der Kurvenpunkte    */
             xlf,                /* Zur X-Positionierung der Legende          */
             y_wert,             /* Y-Wert fuer Position der Legende          */
             kor_wert;           /* Y-Korrektur-Wert zur Skalierung d.Y-Wert e*/
   INT       ii;                 /* Hilfsvariable                             */
   BOOL      krit,               /* TRUE, wenn Kriterium, FALSE, wenn StrfFk t*/
             steigend;           /* TRUE, wenn steigende Bew/Straf-Funktion   */
   
   /* prep_gnu_plot */
   if (kriterien[k_index].bew_alg == NIX_IDX)
   { /* noch undefinierter Bewertungsalgorithnmus */
      meldung (BEW_BEWERTUNG, NO_INFO, NO_INFO, "", UNDEF_BEW_STRF_FKT);
      return;
   } /* noch undefinierter Bewertungsalgorithnmus */
   gnu_x_max = last_x_start + last_delta * x_anz;
   krit  = k_index < max_anz_bew_krit;

   /* ----- Erzeugung der Puffer u. Oeffnen der GnuPlot-Kommando-Datei: ----- */
   sprintf (gnu_fname, "k%02d", k_index+1);
   compress_crit_name (gnu_long_title, kriterien[k_index].bezeichnung);
   gnu_long_title[strlen (gnu_long_title) - 1] = '\0';
   strcpy (buf, gnu_fname);
   strcat (buf, ".cmd");
   gnu_com_file = fopen (buf, "w");                 /* GnuPlot-Kommando-Datei */
   
   /* -------------------- Voreinstellungen des GnuPlots: ------------------- */
   fprintf (gnu_com_file, "set grid\n");
   strcpy (buf, "\n");
   if (simRes)
      xlf = 0.375;                            /* Plot mit Simulationsergebnis */
   else
      xlf = 0.35;   /* 0.28 */   /* Plot einer Bewertungs- oder Straffunktion */
   if (krit)
   { /* Kriterium */
      y_wert   = 0.00095 * RNOTEN_MAX;
      kor_wert = 0.001;
      fprintf (gnu_com_file, "set ylabel '%s' offset 2,0\n", GNU_NOTE_TXT);
      fprintf (gnu_com_file, "set yrange [0:100]\n");
      fprintf (gnu_com_file, "set title '%s \"%s\"'\n", PREP_GNU_KRIT, 
               gnu_long_title);
   } /* Kriterium */
   else
   { /* Straffunktion */
      y_wert   = 0.95;
      kor_wert = 1.0 / RNOTEN_MAX;
      fprintf (gnu_com_file, "set ylabel '%s' offset 2,0\n", GNU_STRAF_TXT);
      fprintf (gnu_com_file, "set yrange [0:1]\n");
      fprintf (gnu_com_file, "set title '%s \"%s\"'\n", PREP_GNU_StrF, 
               gnu_long_title);
   } /* Straffunktion */
   steigend = ((kriterien[k_index].bew_alg == LIN_MINCR)   ||
               (kriterien[k_index].bew_alg == MIXED_MINCR) ||
               (kriterien[k_index].bew_alg == EXP_MINCR)     );
   if (steigend)
      fprintf (gnu_com_file, "set key at %g,%g box spacing 1.1 height 0.5\n", 
              last_x_start + (gnu_x_max - last_x_start)*xlf, y_wert); /* steigend */
   else
      fprintf (gnu_com_file, "set key at %g,%g box spacing 1.1 height 0.5\n",
                gnu_x_max - (gnu_x_max - last_x_start)*0.06, y_wert);     /* fallend  */
   if (krit)
      ii = k_index;
   else
      ii = k_index - max_anz_bew_krit;
   if (strcmp (kriterien[ii].einheit, " ") == 0)
      fprintf (gnu_com_file, "set xlabel '%s'\n", gnu_long_title);
   else
      fprintf (gnu_com_file, "set xlabel '%s [%s]'\n",
               gnu_long_title, kriterien[ii].einheit);

   /* ------------ Datenfiles anlegen und Plot-Kommando aufbauen: ----------- */
   strcpy (gnu_dat_fname, gnu_fname);
   strcat (gnu_dat_fname, ".tmp");
   gnu_dat_file = fopen (gnu_dat_fname, "w");             /* data file: Kurve */
   for (ii = 0;  ii < x_anz;  ii++)
   { /* Kurven-Werte */
      x_wert = last_x_start + (last_delta * ii);
      if (kriterien[k_index].nmax_cont                         || 
          (steigend  && (x_wert <= kriterien[k_index].xend))   ||
          (!steigend && (x_wert >= kriterien[k_index].xstart))   )
      { /* plottbarer Funktionsteil */
         if (!kriterien[k_index].nmax_cont && steigend && 
             (x_wert == kriterien[k_index].xend))
            x_wert = x_wert * (1.0 - DBL_EPSILON);      /* Anzeigen-Korrektur */
         fprintf (gnu_dat_file, "%g %g\n", x_wert,
                  calc_ungew_note (k_index, x_wert) * kor_wert);
      } /* plottbarer Funktionsteil */
   } /* Kurven-Werte */
   fclose (gnu_dat_file);
   /* Speicherreservierung fuer gnu_buffer */
   strcpy (gnu_buffer, "plot '");
   strcat (gnu_buffer, gnu_dat_fname);
   if (krit)
      strcat (gnu_buffer, GNU_LABEL1N);
   else
      strcat (gnu_buffer, GNU_LABEL1S);

   if (!simRes)
   { /* function soecification: show interpolation points */
      strcpy (gnu_dat_fname, gnu_fname);
      strcat (gnu_dat_fname, "_lim.tmp");
      strcat (gnu_buffer, ", '");
      strcat (gnu_buffer, gnu_dat_fname);
      gnu_dat_file = fopen (gnu_dat_fname, "w");   /* data file: Stuetzpunkte */
      if ((kriterien[k_index].bew_alg == LIN_MINCR) || 
	  (kriterien[k_index].bew_alg == LIN_MDECR)   )
      { /* linear */
	 strcat (gnu_buffer, GNU_LABEL2A);
	 if ((last_x_start <= kriterien[k_index].xs1) && 
	     (kriterien[k_index].xs1 <= gnu_x_max))
	    fprintf (gnu_dat_file, "%g %g\n", kriterien[k_index].xs1,
		     calc_ungew_note (k_index, kriterien[k_index].xs1) * kor_wert);
	 if ((last_x_start <= kriterien[k_index].xs2) && 
	     (kriterien[k_index].xs2 <= gnu_x_max))
	    fprintf (gnu_dat_file, "%g %g\n", kriterien[k_index].xs2,
		     calc_ungew_note (k_index, kriterien[k_index].xs2) * kor_wert);
      } /* linear */
      if ((kriterien[k_index].bew_alg == EXP_MINCR) ||
	  (kriterien[k_index].bew_alg == EXP_MDECR))
      { /* reines EXP */
	 strcat (gnu_buffer, GNU_LABEL2B);
	 if ((last_x_start <= kriterien[k_index].xdrittel) && 
	     (kriterien[k_index].xdrittel <= gnu_x_max))
	    fprintf (gnu_dat_file, "%g %g\n", kriterien[k_index].xdrittel,
		     calc_ungew_note (k_index, kriterien[k_index].xdrittel) * kor_wert);
      } /* reines EXP */
      if ((kriterien[k_index].bew_alg == MIXED_MINCR) ||
	  (kriterien[k_index].bew_alg == MIXED_MDECR))
      { /* EXP/LIN-Kombi */
	 strcat (gnu_buffer, GNU_LABEL2C);
	 if ((last_x_start <= kriterien[k_index].xs1) && 
	     (kriterien[k_index].xs1 <= gnu_x_max))
	    fprintf (gnu_dat_file, "%g %g\n", kriterien[k_index].xs1,
		     calc_ungew_note (k_index, kriterien[k_index].xs1) * kor_wert);
	 if ((last_x_start <= kriterien[k_index].xs2) && 
	     (kriterien[k_index].xs2 <= gnu_x_max))
	    fprintf (gnu_dat_file, "%g %g\n", kriterien[k_index].xs2,
		     calc_ungew_note (k_index, kriterien[k_index].xs2) * kor_wert);
	 if ((last_x_start <= kriterien[k_index].xdrittel) && 
	     (kriterien[k_index].xdrittel <= gnu_x_max))
	    fprintf (gnu_dat_file, "%g %g\n", kriterien[k_index].xdrittel,
		     calc_ungew_note (k_index, kriterien[k_index].xdrittel) * kor_wert);
      } /* EXP/LIN-Kombi */
      if (steigend) { /* steigend */       /* Anzeige der Start/Endwerte wenn ungleich y-Achse */
	 if ((kriterien[k_index].xend <= gnu_x_max) &&           /* im Anzeigebereich?         */
	     (fabs(kriterien[k_index].xend) > (last_delta / 2))) /* genug Abstand zur y-Achse? */
	    fprintf (gnu_dat_file, "%g %g\n", kriterien[k_index].xend,
		     calc_ungew_note (k_index, kriterien[k_index].xend) * kor_wert);
      } else { /* fallend */
	 if ((last_x_start <= kriterien[k_index].xstart) &&        /* im Anzeigebereich?         */
	     (fabs(kriterien[k_index].xstart) > (last_delta / 2))) /* genug Abstand zur y-Achse? */
	    fprintf (gnu_dat_file, "%g %g\n", kriterien[k_index].xstart,
		     calc_ungew_note (k_index, kriterien[k_index].xstart) * kor_wert);	    
      } /* fallend */
      fclose (gnu_dat_file);
      strcat (gnu_buffer, GNU_IP_COL);
   } /* function soecification: show interpolation points */

   if (krit && (kriterien[k_index].erfuellungs_note != KEINE_NOTE))
   { /* Erfuellungswert */
      strcat (gnu_buffer, ", '");
      strcpy (gnu_dat_fname, gnu_fname);
      strcat (gnu_dat_fname, "_erf.tmp");        /* data file: Erfüllungswert */
      strcat (gnu_buffer, gnu_dat_fname);
      strcat (gnu_buffer, GNU_LABEL3);
      gnu_dat_file = fopen (gnu_dat_fname, "w");
      if ((last_x_start <= kriterien[k_index].erfuellungs_wert) && 
          (kriterien[k_index].erfuellungs_wert <= gnu_x_max))
         fprintf (gnu_dat_file, "%g %g\n", kriterien[k_index].erfuellungs_wert,
                  calc_ungew_note (k_index, kriterien[k_index].erfuellungs_wert) * kor_wert);
      else { /* Meldung und Leereintrag zur Vermeidung von GnuPlot-Meldung */
	 fprintf (gnu_dat_file, "%g 0\n", last_x_start);   /* Dummy-Eintrag f. GnuPlot */
	 make_krit_text(buf, "%s", k_index);
	 meldung (BEW_GSW, NO_INFO, NO_INFO, buf, FULFILLMENT_VAL_OUT_OF_RANGE);
      } /* Meldung und Leereintrag zur Vermeidung von GnuPlot-Meldung */
      fclose (gnu_dat_file);
   } /* Erfuellungswert */
   if (!simRes)
   { /* Kontext Bewertungs-Menue */
      fprintf (gnu_com_file, "%s\n", gnu_buffer);
      fclose (gnu_com_file);
      sprintf (gnu_buffer, "load '%s.cmd'\n", gnu_fname);
   } /* Kontext Bewertungs-Menue */
} /* prep_gnu_plot */ 



/* ============================== gnu_hard_copy ============================= */
void gnu_hard_copy (void)
{
   /* gnu_hard_copy */
   strcpy (gnu_buffer, "set term postscript\n");
   strcat (gnu_buffer, "set out '");
   strcat (gnu_buffer, gnu_fname);
   strcat (gnu_buffer, ".ps'\n");
   strcat (gnu_buffer, "replot\n");
   strcat (gnu_buffer, "set term x11\n");
   strcat (gnu_buffer, "set out\n");
   strcat (gnu_buffer, "replot\n");
} /* gnu_hard_copy */ 
#endif /* GNUPLOT */



/* ============================= zeige_kriterien ============================ */
INT zeige_kriterien (BOOL *gew_ok)
{
   reset_text_buf ();
   write_buf (2, NOTEN_HDR1);
   write_buf (4, NOTEN_HDR2);
   write_buf (5, NOTEN_HDR3);
   if (akt_anz_used_krit > 0)
      *gew_ok = zeige_krit_werte (6);
   else {
      *gew_ok = FALSE;
      write_buf (6, NO_CRITS_TXT);
   }
   return (akt_anz_used_krit);
} /* zeige_kriterien */ 



/* ============================== addiere_noten ============================= */
LONG addiere_noten (void)
{
   LONG  noten_summe = 0;
   INT   ii;
   
   /* addiere_noten */
   for (ii = 0;  ii < akt_anz_krit;  ii++)
      if (kriterien[ii].klasse != KRITERIUM_AUS)
      {
         if (kriterien[ii].nmax_cont)
            noten_summe += l_round (kriterien[ii].max_note);
         else
            noten_summe += l_round (kriterien[ii].max_note * 0.5);
      }
   return (noten_summe);
} /* addiere_noten */ 



/* ============================ zeige_krit_werte ============================ */
BOOL zeige_krit_werte (INT  znr)
{
   char      buffer[2*MAX_STRING];
   STR_TYPE  buf1, buf2;
   BUF_TYPE  buf3, buf4, buf5, buf6, buf7;
   LONG      noten_summe;
   INT       ii;
   
   /* zeige_krit_werte */
   for (ii = 0;  ii < akt_anz_used_krit;  ii++)
   { /* Alle benutzten Kriterien anzeigen */
      if (kriterien[ii].klasse == KRITERIUM_AUS) 
      { /* Kriterium deaktiviert */
         strcpy (buf7, DEACTIV_TXT);
#ifdef TEXT_MODE
	 sprintf (buf5, " (%6.0f)", kriterien[ii].max_note);
#endif /* TEXT_MODE */
#ifdef GUI_MODE
	 sprintf (buf5, "(%6.2f"PROZ")", 100 * kriterien[ii].max_note / RNOTEN_MAX);
#endif /* GUI_MODE */
      } /* Kriterium deaktiviert */
      else 
      { /* Kriterium aktiviert */
         sprintf (buf7, "  %2d    ", kriterien[ii].klasse);
#ifdef TEXT_MODE
        sprintf (buf5, "  %6.0f ", kriterien[ii].max_note);
#endif /* TEXT_MODE */
#ifdef GUI_MODE
        sprintf (buf5, " %6.2f"PROZ" ", 100 * kriterien[ii].max_note / RNOTEN_MAX);
#endif /* GUI_MODE */
      } /* Kriterium aktiviert */
      if (kriterien[ii+max_anz_bew_krit].klasse == KRITERIUM_AUS)
         strcpy (buf3, DEACTIV_TXT);
      else
         sprintf (buf3, "  %2d    ", kriterien[ii+max_anz_bew_krit].klasse);
      if (kriterien[ii].nmax_cont)
         sprintf (buf4, "%*s", JA_NEIN_LEN, JA_TXT);
      else
         sprintf (buf4, "%*s", JA_NEIN_LEN, NEIN_TXT);
      if (kriterien[ii].erfuellungs_note == KEINE_NOTE)
	 strcpy (buf6, NKRIT_TXT_05B);
      else
	 sprintf (buf6, "%6.0f %7G", kriterien[ii].erfuellungs_note,
		  kriterien[ii].erfuellungs_wert);
      sprintf (buffer, "%2u: %s%s %s %s  %s %s", ii+1,
               kriterien[ii].bezeichnung, buf7, buf5, buf6, buf3, buf4);
      write_buf (ii + znr, buffer);
   } /* Alle benutzten Kriterien anzeigen */
   znr = znr + ii;
   write_buf (znr++, STRICH_40_TXT"--------------------------------");
   noten_summe = addiere_noten ();
#ifdef TEXT_MODE
   sprintf (buf1, "%35s%6ld ", " ", noten_summe);
   sprintf (buf2, "%35s======", " ");
#endif /* TEXT_MODE */
#ifdef GUI_MODE
   sprintf (buf1, "%34s%6.2f"PROZ, " ", 100* noten_summe / RNOTEN_MAX);
   sprintf (buf2, "%34s=======", " ");
#endif /* GUI_MODE */
   write_buf (znr++, buf1);
   write_buf (znr, buf2);
   return (noten_summe == INOTEN_MAX);
} /* zeige_krit_werte */ 



/* ============================== zeige_x_wert ============================== */
void zeige_x_wert (STR_TYPE  buf,
                   DOUBLE    x_wert)
{
   if ((x_wert <= -10000000000.0) || (100000000000.0 <= x_wert))
      sprintf (buf, "%11.4e", x_wert);
   else
      if ((x_wert <= -100000) || (100000 <= x_wert))
         sprintf (buf, "%11.0f", x_wert);
      else
         if ((x_wert <= -100) || (100 <= x_wert))
            sprintf (buf, "%8.1f   ", x_wert);
         else
            if ((x_wert <= -10) || (10 <= x_wert))
               sprintf (buf, "%9.2f  ", x_wert);
            else
               if ((x_wert <= -10) || (10 <= x_wert))
                  sprintf (buf, "%11.4f  ", x_wert);
               else
                  sprintf (buf, "%11g", x_wert);
} /* zeige_x_wert */ 



/* ============================== zeige_bew_par ============================= */
void zeige_bew_par (INT             znr,
                    const STR_TYPE  param_txt,
                    DOUBLE          x_wert,
                    INT             fkt_index)
{
   STRING_TYPE  buffer;
   STR_TYPE     buf, buf2;
   double       n_wert;
   BOOL         str_fkt;
   
   zeige_x_wert (buf, x_wert);
   str_fkt = fkt_index >= max_anz_bew_krit;
   n_wert  = calc_ungew_note (fkt_index, x_wert);
   if (strlen (param_txt) == 0)
      strcpy (buf2, "");
   else
      sprintf (buf2, "%s: ", param_txt);
   if (str_fkt)
      sprintf (buffer, "%s%s    %6.4f", buf2, buf, n_wert / RNOTEN_MAX);
   else
      sprintf (buffer, "%s%s  %6ld %6.2f", buf2, buf, l_round (n_wert), 
               (100 * n_wert) / RNOTEN_MAX);
   write_buf (znr, buffer);
} /* zeige_bew_par */ 



/* =============================== fkt_to_text ============================== */
void fkt_to_text (char       *buf,
                  const char *abst_buf,
                  INT         index)
{
   /* fkt_to_text */
   switch (kriterien[index].bew_alg)
   {
      case LIN_MINCR:
         sprintf (buf, "%s%s: %s, %s", ZEIG_FKT_TXT1, abst_buf, MINCR_TXT, LIN_TXT);
         break;
         
      case LIN_MDECR:
         sprintf (buf, "%s%s: %s, %s", ZEIG_FKT_TXT1, abst_buf, MDECR_TXT, LIN_TXT);
         break;
         
      case EXP_MINCR:
         sprintf (buf, "%s%s: %s, %s", ZEIG_FKT_TXT1, abst_buf,
                  MINCR_TXT, EXP_TXT);
         break;
         
      case EXP_MDECR:
         sprintf (buf, "%s%s: %s, %s", ZEIG_FKT_TXT1, abst_buf,
                  MDECR_TXT, EXP_TXT);
         break;
         
      case MIXED_MINCR:
         sprintf (buf, "%s%s: %s, %s %s %s", ZEIG_FKT_TXT1, abst_buf,
                  MINCR_TXT, LIN_TXT, ZEIG_FKT_TXT2, EXP_TXT);
         break;
         
      case MIXED_MDECR:
         sprintf (buf, "%s%s: %s, %s %s %s", ZEIG_FKT_TXT1, abst_buf,
                  MDECR_TXT, LIN_TXT, ZEIG_FKT_TXT2, EXP_TXT);
         break;
         
      case NIX_IDX:
         sprintf (buf, "%s%s: %s", ZEIG_FKT_TXT1, abst_buf, NIX_FKT_TXT);
         break;
   } /* CASE */
} /* fkt_to_text */ 



/* ================================ zeig_fkt ================================ */
void zeig_fkt (INT  index,
               INT  krit_index)
{
   STR_TYPE  buf1;
   INT       znr;
   
   /* zeig_fkt */
   write_buf (1, "");
   make_krit_text (buf1, NKRIT_TXT_01, krit_index);
   write_buf (2, buf1);
   fkt_to_text (buf1, "", index);
   write_buf (3, "");
   write_buf (4, buf1);
   if (kriterien[index].bew_alg == NIX_IDX)
   { /* noch undefinierter Bewertungsalgorithnmus */
      meldung (BEW_BEWERTUNG, NO_INFO, NO_INFO, "", UNDEF_BEW_STRF_FKT);
      return;
   } /* noch undefinierter Bewertungsalgorithnmus */
   write_buf (5, "");
   write_buf (6, ZEIG_FKT_TXT3);
   if (index > krit_index)
      write_buf (7, ZEIG_FKT_TXT5);                     /* Straf-Funktion     */
   else
      write_buf (7, ZEIG_FKT_TXT4);                     /* Bewertungsfunktion */
   znr = 8;
   switch (kriterien[index].bew_alg)
   {
      case LIN_MINCR:
      case MIXED_MINCR:
         zeige_bew_par (znr++, XMIN_TXT, kriterien[index].xstart, index);
         zeige_bew_par (znr++, XS1_TXT,  kriterien[index].xs1,    index);
	 if (kriterien[index].bew_alg == MIXED_MINCR)
	    zeige_bew_par (znr++, XD_TXT, kriterien[index].xdrittel, index);
         zeige_bew_par (znr++, XS2_TXT,  kriterien[index].xs2,  index);
         zeige_bew_par (znr++, XMAX_TXT, kriterien[index].xend, index);
         break;
         
      case EXP_MINCR:
         zeige_bew_par (znr++, XD_TXT,   kriterien[index].xdrittel, index);
         zeige_bew_par (znr++, XMAX_TXT, kriterien[index].xend,     index);
         break;
         
      case EXP_MDECR:
         zeige_bew_par (znr++, XMIN_TXT, kriterien[index].xstart,   index);
         zeige_bew_par (znr++, XD_TXT,   kriterien[index].xdrittel, index);
         break;
         
      case LIN_MDECR:
      case MIXED_MDECR:
         zeige_bew_par (znr++, XMIN_TXT, kriterien[index].xstart, index);
         zeige_bew_par (znr++, XS1_TXT,  kriterien[index].xs1,    index);
	 if (kriterien[index].bew_alg == MIXED_MDECR)
	    zeige_bew_par (znr++, XD_TXT, kriterien[index].xdrittel, index);
         zeige_bew_par (znr++, XS2_TXT,  kriterien[index].xs2,  index);
         zeige_bew_par (znr++, XMAX_TXT, kriterien[index].xend, index);
         break;
   } /* CASE */
} /* zeig_fkt */ 
