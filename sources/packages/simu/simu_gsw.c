/*******************************************************************************
GLEAM/AE                                                                  V2.2.1

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: SIMUlator
                        Module: SIMUlator GrundSoftWare
 
Package: simu                   File: simu_gsw.c 
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or of 
the files "schalter.h".
--------------------------------------------------------------------------------

Der Modul enthaelt Routinen fuer die GLEAM_USR-Varianten, welche in der 
TEXT_MODE-Variante vom Modul "simucmen" benutzt werden. Bis auf (Fehler-)Meldun-
gen gehen alle Ausgaben dieser Routinen grundsaetzlich in den Textpuffer des 
Packages "fbhm" (im folgenden auch nur kurz Textpuffer genannt). 


Die Routinen des globalen Exports im einzelnen:

void behandle_neue_note (CHAIN_ELEM_PTR_TYPE  ak_ptr,
                         INT                  znr, 
                         DOUBLE               neu_fit_note);
          Die neue Note "neu_fit_note" wird in den AK-Header eingetragen. Die 
          Gueteklassenzuordnung wird entsprechend der neuen Note ueberprueft. 
          Gegebenenfalls muss die unter Guete/Lfd-Nr im AK-Speicher stehende AK 
          neu einsortiert werden. Es erfolgt eine Meldung mit der neuen AK-
          Adresse oder dem Hinweis, dass die alte weiterhin gilt, sofern "znr" 
          groesser als 0 ist. Die Meldungen erfolgen ab Zeile "znr" in den 
          Textpuffer. 

BOOL get_ak_and_check_ak (BOOL *must_simu,
                          INT   lfd_nr,
                          INT   guete);
          Beschafft die durch "guete" und "lfd_nr" bezeichnete AK und hinterlegt
          sie in der lokalen Variable "simu_ak_ptr" fuer Routine "perform_simu".
          Wenn die Kette nicht verfuegbar ist, wird FALSE abgeliefert, ansonsten
          TRUE. Ausserdem wird geprueft, ob der externe Simulator benoetigt wird
          und das Ergebnis in "must_simu" abgeliefert. 

void perform_simu (BOOL  simu_oben,
                   INT   znr);
          Fuehrt eine Simulation durch, wenn "simu_oben" TRUE ist. Ansonsten  
          erfolgt eine Meldung. Das Ergebnis der Simulation wird ausgegeben 
          und gegebebenenfalls wird die AK mit Hilfe von "behandle neue_note" 
          unter Ausgabe weiterer Meldungen neu einsortiert.
          Alle Meldungen und Ausgaben gehen in den Textpuffer von "fbhm".

BOOL save_simu_erg (char *fspec,
                    BOOL  neu_file,
                    BOOL  mitIndex);
          Rettet die Ergebnisanzeige der letzten interaktiven Simulation und an-
          wendungsabhaengig die Ergebnisdaten in das durch "fspec" bezeichnete 
          File zusammmen mit den Kopfinformationen. "neu_file" gibt an, ob die 
          Datei neu angelegt oder erweiternd geschrieben wird. "mitIndex" wird
          durchgereicht und kontrolliert die Ausgbe der Kriterienindices. 
          Liefert TRUE, wenn alles gut ging, sonst FALSE.

INT show_simu_erg (INT   znr,
                   BOOL  mitIndex);
          Je nach Applikation wird "extractChrParam" gesetzt und entsprechend
          werden ausser beim GUI-Mode zunaechst die evolutionierten Eingangs-
          parameter des Simulators ab "znr" angezeigt oder diese Ausgabe unter-
          bleibt. Danach folgt die Ausgabe der Ergebniswerte der Bewertung und 
          des letzten Simulationslaufs.
          Die Ausgabe der Notenbewertung erfolgt in folgendem Format:
            xx: NameDesKriteriums  :Prio  Gew ungewN   MindGN (gewNot) SimErg
          wobei "xx" fuer den Index des Kriteriums steht. Die Ausgaben erfolgen 
          in den Textpuffer.                                                  
          Bei gesetztem "mitIndex" wird der Kriterienindex (xx) ausgegeben. Bei 
          Doppelkriterien wird nur der relevante Teil ausgegeben. 
          Die Funktion liefert die Anzahl der erzeugten Zeilen als Ergebnis.

void prep_status_anzg (void);
          Baut die Statusanzeige im Textpuffer auf. Diese umfasst Angaben ueber 
          das Logging der Simulatorschnittstelle, den Status der Kommunikation 
          (sim_ok) und den Ladestatus des Simulatormodells (sim_up).
          Im Falle der externen Simualtionsdienste sieht das folgendermassen aus,
          wobei die letzte Zeile nur erscheint, wenn ein Modell geladen ist:
             Externe Simulationsdienste:
             URL-Anfang: "http://nixda.iai.kit.edu:8090/opt"
             Logging der Simulatorschnittstelle   = aktiv
             Kommunikationspaket bereit  (sim_ok) = ja
             Modell "irgendwas" geladen  (sim_up) = ja
               Arbeits-URL: "http://nixda.iai.kit.edu:8090/opt/42/4711"


Nachstehende Routinen sind nur bei gesetztem GUI_MODE Teil des Package-Exports, 
sonst sind sie nur package-lokal:

INT  zeige_simu_bew (INT   znr,
                     BOOL  mitIndex);
          Gibt die Ergebnisse des letzten interaktiven Simulatorlaufs ab Zeilen-
          nummer "znr" mit Hilfe der Routine "show_simu_erg" aus. Der Returnwert
          ist der von "show_simu_erg". Wenn bisher kein Simulatorlauf stattge-
          funden hat, wird eine entsprechende Meldung ausgegeben. "mitIndex"
          wird an "show_simu_erg" durchgereicht und kontrolliert die Ausgabe
          des KriterienIndex.

INT  zeige_evo_best_bew (INT  znr);
          Gibt die Ergebnisse des besten bisher evolutionierten Individuums ab 
          Zeilennummer "znr" mit Hilfe der Routine "show_simu_erg" aus. Der  
          Returnwert ist der von "show_simu_erg". Wenn bisher keine Evoltion 
          stattgefunden hat, wird eine entsprechende Meldung ausgegeben. 

GSTATE ak_mem_neu_bewert (BOOL  clr_simu_flag);
          Fuehrt die Neubewertung des AK-Speichers durch und liefert GLEAM_OK, 
          wenn dabei keine Fehler auftraten. "clr_simu_flag" steuert, ob auch
          neu simuliert werden muss.
          Der Kettenspeicher wird dabei neu organisiert und alle AKs entspre-
          chend ihrer neuen Bewertung neu einsortiert. Eventuelle "unbewertet-
          Flags im Kettenstatus werden zurueckgesetzt. Unbewertbare Ketten 
          werden geloescht. Die Routine erzeugt im Textpuffer Meldungen 
          ueber geloeschte Ketten sowie eine Schlussmeldung.


Die Routinen des package-lokalen Exports im einzelnen:

void plot_akt_wert (INT  krit_nr);
          Die Routine setzt die vorherige Ausfuehrung der Routinen "init_plot" 
          und "prep_gnu_plot" voraus. Dies wird in der TEXT_MODE-Variante durch 
          den vorherigen Aufruf der Routine "c_prep_gnu_plot" erreicht.
          Die Routine legt das File mit den Daten des aktuellen Werts an und er-
          weitert den bereits im "gnu_buffer" stehenden Plot-Befehl um die An-
          weisung zum Zeichnen der aktuellen Werte. Der nunmehr fertige Plot-
          Befehl wird in das ".cmd"-File geschrieben. In den "gnu_buffer" wird
          der Load-cmd-File-Befehl eingetragen.

--------------------------------------------------------------------------------
basiert auf C-Unit: simu
--------------------------------------------------------------------------------
Aenderung gegenueber Version 2.1.0:
 1. Die Simu-Ergebnisse werden mit "erg_genauigkeit" Stellen angezeigt.
 2. Korrektur an "value_2_string" wegen laengerer Namen der Aktionsparameter.
 3. "behandle_neue_note" wird zum globalen Package-Export.           WJ 23.08.00
 4. "behandle_neue_note" schreibt nur bedingt in Textpuffer.         WJ 07.12.00
 5. Routine "show_simu_erg" zeigt bei ext. Simu von dyn. AKs keine 
    Eingangsparameter mehr an.                                       WJ 28.04.01
 6. "show_simu_erg": Es wird nur noch der relevante Anteil eines 
    Doppelkriteriums angezeigt. Anzeige der X-Werte bei MBF. Ent-
    sprechende Anpassungen in "save_simu_erg" u. "zeige_simu_bew".   WJ 07.05.02
 7. Berechnung des Fkt-Ergebnis von "show_simu_erg" an 6. angepasst. WJ 03.07.02
 8. Linux-Portierung: Aus "round" wird "l_round" und "i_round".
    "plot_akt_wert" an den Linux-GNUPlot angepasst.                  WJ 12.05.05
 9. Integration der MBFs der CEC05-Sammlung (CEC_MATHFKT_APPL).      WJ 26.10.07
10. Anpassung der Darstellung des akt. Werts in "plot_akt_wert" und
    Datenfile mit nur einem Wert, da Impuls-Darstellung.             WJ 08.01.14
11. "show_simu_erg" wird vollstaendig ueberarbeitet. "zeige_simu_bew",
    "zeige_evo_best_bew", "save_simu_erg" und "value_2_string" werden 
    ueberarbeitet. "value_2_string"wird eine lokale Routine. 
    "show_simu_erg" und "save_simu_erg" werden Packageexport. 
    "zeige_simu_bew" und "zeige_evo_best_bew" sind package-export bei 
    gesetztem GUI_MODE und ansonsten nur package-lokal.              WJ 05.12.14
12. Renovierung: Einige Anpassungen an geaenderte Konstanten. Korrek-
    tur eines fprintf-Aufrufs. Anpassung einiger Ausgaben. Streichung 
    des "link"-Includes.                                             WJ 18.04.16
13. "show_simu_erg" hinterlegt die Indices der Kriterien in der 
    Ausgabereihenfolge in "critLine2Index". "plot_akt_wert" ist im
    GUI_MODE Package-Export anstelle von package-lokal.  Ueberarbei-
    tung der Behandlung von Straffunktionen in "show_simu_erg" und
    in "plot_akt_wert". Verbesserte Anzeige in "prep_status_anzg"
    und "perform_simu".  Meldungen nutzen den Namen des Kriteriums 
    anstelle der Nummer. "ak_mem_neu_bewert" wird im GUI_MODE zum
    Package-Export. Neue Farbgebung in "plot_akt_wert".              WJ 19.09.16
14. Webbasierte Anbindung externer Simulatoren: Abfrage auf 
    SIMU_SERV_APPL in "zeige_simu_bew".                              WJ 19.05.17
15. Simu-Logging in "get_ak_and_check_ak" korrigiert.                WJ 13.09.17
16. Anpassung von "save_simu_erg" an die Erfordernisse der um die 
    "-W"-Option erweiterten Kommandozeilenversion (CLV).             WJ 19.01.18
17. "prep_status_anzg" zeigt bei gesetztem EXT_MATLAB_SIMU zus‰tz-
    lich den Wert der Variable "extractCaseFile" an.                 WJ 02.02.18
18. Fehler in behandle_neue_note() beseitigt.                        WJ 03.07.18
19. Ersetzung von "matlabApplName" durch "ext_sim_buf".              WJ 19.10.18
20. Anpassung der Aufrufe von get_act_descr().                       WJ 28.06.19
21. Avoiding buffer overflow of static strings: behandle_neue_note(),
    get_ak_and_check_ak(), show_simu_erg(), ak_mem_neu_bewert().     WJ 18.07.20
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 18.07.2020
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
#include "hmod.h"
#ifdef EXT_SIMU_SERV
  #include "webIO.h"
#endif /* EXT_SIMU_SERV */
#include "appl.h"
#include "simuInt.h"
#include "simu.h"

#ifdef GNUPLOT
  #include <unistd.h>
#endif

#include GLOBAL_TEXTS
#include SIMU_TEXTS

#ifdef GLO_CHECKS
  #define CHECKS
#endif



/* ======================== Package-globale Variable: ======================= */
BOOL   evo_plot_data_ready = FALSE;    /* True=plottbare EvoErgdaten verfuegb.*/



/* ============================ Lokale Variable: ============================ */
static CHAIN_ELEM_PTR_TYPE  simu_ak_ptr; /* Zur Kommunikation von "get_ak_and */
                                         /* "start_simu" mit "perform_simu"   */


/* ============================ Lokale Routinen: ============================ */
static BOOL check_krit_for_display (INT       krit_idx);

static void value_2_string         (STR_TYPE  res_buf,
                                    STR_TYPE  std_unit,
                                    STR_TYPE  add_unit,
                                    INT       name_len,
                                    INT       prec,
                                    DOUBLE    value);




/* =========================== behandle_neue_note =========================== */
void behandle_neue_note (CHAIN_ELEM_PTR_TYPE  ak_ptr, 
                         INT                  znr, 
                         DOUBLE               neu_fit_note)
{
   STR_TYPE  buf2;
   BUF_TYPE  buf1;
   INT       neu_lfd_nr, neu_guete;
  
   /* BEHANDLE_NEUE_NOTE/LSK_GSW */
   if (berechne_guete (&neu_guete, neu_fit_note))
   { /* neue Noten sind ok */
      ak_ptr->u_el.u_xel.u_hdr.chain_status &= ~UNBEWERTETE_BASIS_AK;
      ak_ptr->u_el.u_xel.u_hdr.fitness_note = neu_fit_note;
      if (neu_guete != ak_ptr->u_el.u_xel.u_hdr.guete)
      { /* AK neu einsortieren */
	 umhaengen (&neu_lfd_nr, AK, neu_guete, ak_ptr->u_el.u_xel.u_hdr
		    .guete, ak_ptr->u_el.u_xel.u_hdr.lfd_nr);
	 if (znr > 0)
	 { /* Meldung erstatten */
	    adr_to_text (buf1, neu_guete, neu_lfd_nr, NO_ADJUST);
	    sprintf (buf2, AK_ADR_NEU_TXT, buf1);
	    write_buf (znr, buf2);
	 } /* Meldung erstatten */
      } /* AK neu einsortieren */
      else
	 if (znr > 0)
         { /* melden, dass AK in der alten Gueteklasse bleibt */
	    adr_to_text (buf1, neu_guete, ak_ptr->u_el.u_xel.u_hdr.lfd_nr, NO_ADJUST);
	    sprintf (buf2, AK_ADR_BLEIBT_TXT, buf1);
	    write_buf (znr, buf2);
	 } /* melden, dass AK in der alten Gueteklasse bleibt */
   } /* neue Noten sind ok */
   else
      if (znr > 0)
      { /* Neue Noten fehlerhaft */
         write_buf (znr, NOTE_UNVERANDERT_TXT);
         sprintf (buf2, NOTE_SCHLECHT_TXT, l_round (neu_fit_note));
         write_buf (znr+1, buf2);
      } /* Neue Noten fehlerhaft */
} /* behandle_neue_note */ 



/* =========================== get_ak_and_check_ak ========================== */
BOOL get_ak_and_check_ak (BOOL *must_simu,
                          INT   lfd_nr,
                          INT   guete)
{
   STRING_TYPE  buf2;
   BUF_TYPE     buf;
   GSTATE       ret_state = GLEAM_OK;

   /* get_ak_and_check_ak */
   *must_simu = FALSE;
   if (get_chain (&simu_ak_ptr, &lfd_nr, AK, guete, &guete, &ret_state))
   { /* AK existiert */
#ifdef MITSIMULOG
      adr_to_text (buf, guete, lfd_nr, NO_ADJUST);
      if (mit_simulog)
      { /* Simu-Start ins Logfile schreiben */
	 sprintf(buf2, "\n========================================================="
		 "===============\n  Simulation des Chromosoms %s:", buf); 
	 write_to_simulog(buf2, CLOSE_LOG);
      } /* Simu-Start ins Logfile schreiben */
#endif /* MITSIMULOG */
      *must_simu = always_sim || 
	           !(simu_ak_ptr->u_el.u_xel.u_hdr.chain_status & SIMULATED);
      return (TRUE);
   } /* AK existiert */
   else
      return (FALSE);
} /* get_ak_and_check_ak */



/* ============================== perform_simu ============================== */
void perform_simu (BOOL  simu_oben,
                   INT   znr)
{
   STR_TYPE  buf;
   DOUBLE    alte_note, note;

   /* perform_simu */
   int_sim_data_ready = FALSE;
   if (simu_oben)
   { /* Simulator ok */
      alte_note = simu_ak_ptr->u_el.u_xel.u_hdr.fitness_note;
      simulator (&note, SIMU_SHOW, simu_ak_ptr);
      if (get_line_anz () > 0)
      { /* Simu-Hochfahren-Meldung */
	 get_buf (1, buf);
	 write_buf (znr++, buf);
      } /* Simu-Hochfahren-Meldung */
      if (note == KEINE_NOTE)
	 write_buf (znr, NO_SIMU_DONE_TXT);              /* simulation failed */
      else
      { /* Na, Prima! */
	 if (fabs (alte_note - note) < 0.001)
	 {
	    sprintf (buf, NOTE_BLEIBT_TXT, l_round(note), note);
	    write_buf (znr, buf);
	 }
	 else
	 {
	    sprintf (buf, NOTE_NEU_TXT, l_round(note), note);
	    write_buf (znr, buf);
	    behandle_neue_note (simu_ak_ptr, znr+1, note);
	 }
	 int_sim_data_ready  = TRUE;
	 evo_plot_data_ready = FALSE;
	 bew_aend_sim        = FALSE;
      } /* Na, Prima! */
   } /* Simulator ok */
   else
      write_buf (znr, NIX_EXT_SIMU_TXT);
} /* perform_simu */



/* ============================= save_simu_erg ============================== */
BOOL save_simu_erg (char *fspec,
                    BOOL  neu_file,
                    BOOL  mitIndex)
{
   STR_TYPE  buf, buf2;
   FILE     *res_file;
   INT       z_anz, ii;
   BOOL      erg, 
             ok = TRUE;

   /* save_simu_erg */
   if (neu_file)
      strcpy (buf, "wt");
   else
      strcpy (buf, "at");
   if (open_file (&res_file, fspec, buf))
   { /* File ok */
      z_anz = zeige_simu_bew (1, mitIndex);  /*liefert 2: keine anzeigb.Sim*/
      erg   = z_anz > 2;
      get_time_stamp (buf, buf2, buf2, buf2);    /* "buf2" ist Dummy-Param */
      fprintf (res_file, ERG_HDR, prog_id_buf, buf);
      fprintf (res_file, EXP_FILES_HDR);
      erg &= print_exp_data (res_file);   /* Ausgabe der Experimentdateien */
      z_anz = get_line_anz ();                     /* auch Bewertung ausgeben */
      for (ii = 1;  (ii <= z_anz) && ok;  ii++)
      { /* Schreibe Ergebnis-Puffer ins File */
	 ok = get_buf (ii, buf);
	 if (ok)
	    ok = fprintf (res_file, "%s\n", buf) > 0;
      } /* Schreibe Ergebnis-Puffer ins File */
      reset_text_buf ();
      erg &= ok;
      if ((appl_code == ADD_APPL) && erg)
      { /* Schreibe anwendungsabhaengig weitere Ergebnisse ins File */
	 fprintf (res_file, "\n%s%s\n\n", STRICH_40_TXT, STRICH_40_TXT);
	 erg = add_appl_save_erg (res_file);
      } /* Schreibe anwendungsabhaengig weitere Ergebnisse ins File */
      fprintf (res_file, "\n\n");
      fclose (res_file);
   } /* File ok */
   else
      erg = FALSE;
   return (erg);
} /* save_simu_erg */



/* ============================= value_2_string ============================= */
static void value_2_string (STR_TYPE  res_buf,
                            STR_TYPE  std_unit,
                            STR_TYPE  add_unit,
                            INT       name_len,
                            INT       prec,
                            DOUBLE    value)
/*----------------------------------------------------------------------------*/
/*   Die Routine erzeugt aus den Eingabeparameter "value" mit Hilfe der An-   */
/*   gaben zur Masseinheit "std_unit" bzw. "add_unit" einen String, der aus   */
/*   dem in "res_buf" bereits enthaltenen Bezeichner des Wertes besteht, dem  */
/*   Wert selbst und einer geeigneter Masseinheit. Sollte der "res_buf" dage- */
/*   gen leer sein, werden nur der Wert samt Massangabe abgeliefert und die   */
/*   nachfolgend beschriebene Normierung des Bezeichners entfaellt.           */
/*   Bezeichnernormierung: Wenn das letzte Zeichen des Bezeichners nicht ein  */
/*      ":" ist oder er nicht eine Laenge von "name_len" Zeichen hat, wird    */
/*      der String auf eine Laenge von "name_len" aufgefuellt und um ein ":"  */
/*      ergaenzt.                                                             */
/*   Die Routine wandelt "value" im "%G"-Format mit der Praezision "prec" um, */
/*   es sei denn, dass es sich bei der in "std_unit" enthaltenen Maﬂeinheit   */
/*   um das "%"-Zeichen handelt. In diesem Fall wird auf %-Ausgabe normiert:  */
/*   "xxx.xx %". Das erste Zeichen ("<flag>") des "add_unit"-Strings steuert  */
/*   die 2.Ausgabe von "value" bei Bereichsueberschreitung wie folgt:         */
/*        !: Keine 2.Ausgabe.                                                 */
/*        <: 2.Ausgabe des 1000-fachen des Werts im "%G"-Format, wenn der     */
/*           Wert kleiner als 1/1000 ist.                                     */
/*        >: 2.Ausgabe des 1/1000 des Werts im "%G"-Format, wenn der Wert     */
/*           groesser-gleich 1000 ist.                                        */
/*        %: 2.Ausgabe des Werts im "%G"-Format, wenn der Wert kleiner als    */
/*           1.0 oder groesser-gleich 100000 ist.                             */
/*   "add_unit" hat folgendes Format: <flag><blank><bezeichner>. Der an-      */
/*   schliessend in "res_buf" enthaltene Ergebnisstring setzt sich somit      */
/*   folgendermassen zusammen:                                                */
/*     [<bezeichner>: ]<wert> <std-unit>[  <wert> <bezeichner von add_unit>]  */
/*----------------------------------------------------------------------------*/
{
   STR_TYPE  buf;
   INT       len, ii;
   char      flag;
   BOOL      prozent;

   /* value_2_string */
   len = strlen (res_buf);
   if ((len > 0) && ((len <= name_len) || (res_buf[name_len] != ':')))
   { /* Bezeichner vorhanden UND zu kurz ODER kein ":"-Abschluss */
      for (ii = len;  ii <= name_len;  ii++)
	 strcat (res_buf, " ");          /* Auf name_len+1 Zeichen auffuellen */
      res_buf[name_len] = ':';           /* name_len-tes Zeichen wird ein ":" */
   } /* Bezeichner vorhanden UND zu kurz ODER kein ":"-Abschluss */
   prozent = std_unit[0] == '%';
   if (prozent)
      sprintf (buf, " %6.2f %%", value);
   else
      sprintf (buf, " %6.*G %s", prec, value, std_unit);
   strcat (res_buf, buf);
   flag = add_unit[0];
   if (flag != '!')
   { /* 2.Ausgabe falls Bereichsueberschreitung */
      strcpy (buf, "");
      if ((flag == '%') && ((value < 1.0) || (100000 <= value)))
	 sprintf (buf, " %G %%", value);
      else
	 if ((flag == '<') && (value < 0.001))
	    sprintf (buf, " %6.*G %s", prec, 1000*value, &(add_unit[2]));
         else
	    if ((flag == '>') && (1000 <= value))
               sprintf (buf, " %6.*G %s", prec, value/1000, &(add_unit[2]));
      strcat (res_buf, buf);
   } /* 2.Ausgabe falls Bereichsueberschreitung */
} /* value_2_string */



/* ======================== check_krit_for_display ========================== */
static BOOL check_krit_for_display (INT  krit_idx)
/*----------------------------------------------------------------------------*/
/*   Pr¸ft, ob ein Kriterium zu einem Doppelkriterium gehˆrt. Liefert TRUE,   */
/*   wenn dies nicht der Fall ist ODER wenn es der aktive Teil eines Doppel-  */
/*   kriteriums ist.                                                          */
/*----------------------------------------------------------------------------*/
{
   BOOL  display_it = TRUE;

   /* check_krit_for_display */
   if (!kriterien[krit_idx].nmax_cont)
   { /* Doppelkriterium */
      if ((kriterien[krit_idx].bew_alg == LIN_MINCR)   ||        /* steigend? */
	  (kriterien[krit_idx].bew_alg == EXP_MINCR)   ||
	  (kriterien[krit_idx].bew_alg == MIXED_MINCR)   )       /* steigend: */
	 display_it = int_simu_erg[krit_idx]  < kriterien[krit_idx].xend;
      else                                                       /* fallend:  */
	 display_it = int_simu_erg[krit_idx] >= kriterien[krit_idx].xstart;
   } /* Doppelkriterium */
   return (display_it);
} /* check_krit_for_display */



/* ============================= show_simu_erg ============================== */
INT show_simu_erg (INT   znr,
                   BOOL  mitIndex)
{
   char                buffer[2*MAX_STRING];
   STR_TYPE            buf1, buf3, strichBuf;
   BUF_TYPE            buf2, sbuf;
   ACT_DESCR_PTR_TYPE  this_act_ptr;
   DOUBLE              unbestrafte_note;
   INT                 diese_klasse,
                       ipar_index, rpar_index,
                       lineIdx,
                       critIdx,
                       ii, jj;
   BOOL                straf_title_printed = FALSE;

   /* show_simu_erg */
   strcpy (strichBuf, STRICH_TXT);
   if (mitIndex) 
      strcat (strichBuf, "----");                   /* Ausgabe mit Krit-Index */
#ifndef GUI_MODE
   if (paramsExtracted)
   { /* Simu-Parameter-Anzeige */
      sprintf (buf1, PARAM_HDR_TXT, opt_param_name);
      write_buf (znr++, buf1);
      ipar_index = 0;
      rpar_index = 0;
      for (ii = 1;  ii <= last_act;  ii++)
      { /* Parameterwert anzeigen */
	 get_act_descr (&this_act_ptr, ii, WITHOUT_GENECODE_CHECK);
	 if ((this_act_ptr->i_par_anz + this_act_ptr->r_par_anz) > 1)
 	 { /* Ausgabe des Aktionsnamens: */
	    sprintf (buffer, "%s:", this_act_ptr->act_name);
	    write_buf (znr++, buffer);
	 } /* Ausgabe des Aktionsnamens: */
	 for (jj = 0;  jj < this_act_ptr->i_par_anz;  jj++)
	 { /* alle I-Parameter */
	    strcpy (buf1, this_act_ptr->i_par_descr[jj].par_name);
	    value_2_string (buf1, this_act_ptr->i_par_descr[jj].einheit,
			    this_act_ptr->i_par_descr[jj].alt_einheit, 
			    PAR_NAME_MAX, erg_genauigkeit, 
                            int_i_params[ipar_index]);
	    ipar_index++;
	    strcpy (buffer, "   ");
	    strcat (buffer, buf1);
	    write_buf (znr++, buffer);
	 } /* alle I-Parameter */
	 for (jj = 0;  jj < this_act_ptr->r_par_anz;  jj++)
	 { /* alle R-Parameter */
	    strcpy (buf1, this_act_ptr->r_par_descr[jj].par_name);
	    value_2_string (buf1, this_act_ptr->r_par_descr[jj].einheit,
			    this_act_ptr->r_par_descr[jj].alt_einheit, 
			    PAR_NAME_MAX, erg_genauigkeit, 
			    int_r_params[rpar_index]);
	    rpar_index++;
	    strcpy (buffer, "   ");
	    strcat (buffer, buf1);
	    write_buf (znr++, buffer);
	 } /* alle R-Parameter */
      } /* Parameterwert anzeigen */
   } /* Simu-Parameter-Anzeige */
   else
      if (displayableParams)
      { /* parameter display not prepared by previous simulation */
	 sprintf(buffer, NO_PARAMS_WITHOUT_SIMU, opt_param_name);
	 write_buf (znr++, buffer);
      } /* parameter display not prepared by previous simulation */
   write_buf (znr++, strichBuf);
#endif /* kein GUI_MODE */

   if (mitIndex) { /* Ausgabe mit Krit-Index */
      strcpy (buffer, BEWERT_ITITLE1_TXT);
      strcpy (buf1,   BEWERT_ITITLE2_TXT);
   } /* Ausgabe mit Krit-Index */
   else { /* Ausgabe ohne Krit-Index */
      strcpy (buffer, "");
      strcpy (buf1,   "");
   } /* Ausgabe ohne Krit-Index */
   strcat (buffer, BEWERT_TITLE1_TXT);
   strcat (buf1,   BEWERT_TITLE2_TXT);
   write_buf (znr++, buffer);
   write_buf (znr++, buf1);
   unbestrafte_note = 0;
   lineIdx = 0;
   for (diese_klasse = 1;  diese_klasse <= max_klasse;  diese_klasse++)
   { /* Bewertung zeigen */
      for (ii = 0;  ii < akt_anz_krit;  ii++)
      { /* alle Kriterien pruefen */
	 if ((kriterien[ii].klasse == diese_klasse) && 
	     (mitIndex || check_krit_for_display(ii)))
	 { /* Kriterium in der aktuellen Klasse und anzuzeigen */
#ifdef GUI_MODE
	    critLine2Index[lineIdx++] = ii;     /* merke Index des Kriteriums */
#endif /* GUI_MODE */
	    if (mitIndex)
	       sprintf (sbuf, "%2d: ", ii+1);
	    else
	       strcpy (sbuf, "");
	    strcpy (buf3, "");
            if (int_krit_gueltig[ii]){ /* Kriterium ist gueltig */
	       unbestrafte_note += int_gew_note[ii];
	       sprintf (buf2, " %6ld ", l_round (int_gew_note[ii]));
	    } /* Kriterium ist gueltig */
	    else
	       sprintf (buf2, "(%6ld)", l_round (int_gew_note[ii]));
	    value_2_string (buf3, kriterien[ii].einheit, 
			    kriterien[ii+ max_anz_bew_krit].einheit, 
			    0, erg_genauigkeit, int_simu_erg[ii]);
	    sprintf (buffer, "%s%s  %2d  %3d  %6ld   %6ld %s %s",
		     sbuf, kriterien[ii].bezeichnung, diese_klasse, 
		     i_round(kriterien[ii].noten_gewicht*100),
		     l_round (int_ungew_note[ii]), 
		     l_round (kriterien[ii].erfuellungs_note), buf2, buf3);
	    write_buf (znr++, buffer);
	 } /* Kriterium in der aktuellen Klasse und anzuzeigen */
      } /* alle Kriterien pruefen */
   } /* Bewertung zeigen */
   write_buf (znr++, strichBuf);
   strcpy (buf3, "");
   if  (mitIndex)                                   /* Ausgabe mit Krit-Index */
      strcpy (buf3, "    ");
   sprintf (buffer, "%s%s%6ld", ROH_SUMME_TXT, buf3, l_round (unbestrafte_note));
   write_buf (znr++, buffer);
   for (ii = max_anz_bew_krit;  ii < krit_og;  ii++)
   { /* alle Kriterien auf Straffunktion pruefen */
      critIdx = ii-max_anz_bew_krit;
      if (int_simValOutOfEvalRange[critIdx])
      { /* simVal im Strafbereich */
	 if (!straf_title_printed) { /* Drucke Straf-Kopfzeile */
	    write_buf (znr++, STRAF_TITLE_TXT);
	    straf_title_printed = TRUE;
	 } /* Drucke Straf-Kopfzeile */
	 strcpy (buf1, kriterien[critIdx].bezeichnung);
	 buf1[KRIT_BEZ_LEN+1] = '\0';
	 if (int_krit_gueltig[ii]) /* aktive Straffunktion ? */
	    sprintf (buffer, "   %s %s = %G", buf1, STRAF_FAKTOR_TXT, 
		     int_straf_fkt[critIdx]);
	 else
	    sprintf (buffer, "   %s %s =(%G)", buf1, STRAF_FAKTOR_TXT, 
		     int_straf_fkt[critIdx]);
	 write_buf (znr++, buffer);
      } /* simVal im Strafbereich */
   } /* alle Kriterien auf Straffunktion pruefen */
   if (!straf_title_printed)
      write_buf (znr++, KEINE_STRAFE_TXT);
   sprintf (buffer, "%s%s%6ld", ERG_NOTE_TXT, buf3, l_round (int_gesamt_note));
   write_buf (znr++, buffer);
   sprintf (buffer, "%s%s", buf3, ERG_UNTERSTRICH_TXT);
   write_buf (znr, buffer);
   return (znr);
} /* show_simu_erg */



/* ============================ prep_status_anzg ============================ */
void prep_status_anzg (void)
{
   STRING_TYPE  longBuf;
   STR_TYPE     buf;
   INT          znr, modNameLen, blankLen;

   /* prep_status_anzg */
   write_buf(2, STATUS_ANZG_HDR_TXT);                          /* header line */
   znr = 4;
#ifdef EXT_SIMU_SERV
   write_buf(znr++, STATUS_ANZG_ESS1_TXT);       /* Ext.Sim.Serv. header line */
   sprintf(buf, STATUS_ANZG_ESS2_TXT, serverBuffer);
   write_buf(znr++, buf);                               /* URL beginning line */
   modNameLen = strlen(extSimuModelName);
   if (modNameLen < 10)
   { /* model name shorter than 10 characters */
      sprintf(buf, STATUS_ANZG_ESS4_TXT, extSimuModelName, 10-modNameLen, " ");
      blankLen = 1;
   } /* model name shorter than 10 characters */
   else
   { /* model name longer than 9 characters */
      sprintf(buf, STATUS_ANZG_ESS4_TXT, extSimuModelName, 1, " ");
      blankLen = modNameLen - 8;
   } /* model name longer than 9 characters */
   if (sim_up)
   { /*  "sim_up" line and work URL line */
      strcat(buf, JA_TXT);
      sprintf(longBuf, STATUS_ANZG_ESS6_TXT, urlComBuf);
      write_buf (znr+3, longBuf);
   } /*  "sim_up" line and work URL line */
   else
      strcat(buf, NEIN_TXT);
   write_buf (znr+2, buf);                                   /* "sim_up" line */
   sprintf(buf, STATUS_ANZG_ESS3_TXT, blankLen, " ");
   if (sim_ok)
      strcat(buf, JA_TXT);
   else
      strcat(buf, NEIN_TXT);
   write_buf (znr+1, buf);                                   /* "sim_ok" line */
   sprintf(buf, STATUS_ANZG_ESS5_TXT, blankLen, " ");
#else /* no EXT_SIMU_SERV */
  #ifdef EXT_MATLAB_SIMU
   if (sim_ok)
      sprintf(buf, STATUS_ANZG_MP1_TXT, STATUS_ANZG_AKTIV_TXT);
   else
      sprintf(buf, STATUS_ANZG_MP1_TXT, STATUS_ANZG_AUS_TXT);
   write_buf(znr++, buf);                                    /* "sim_ok" line */
   modNameLen = strlen(extSimuModelName);
   if (modNameLen < 13)       /* max length of 12 for correct column display  */
      sprintf(buf, STATUS_ANZG_MP2_TXT, extSimuModelName, 13-modNameLen, " ");
   else
      sprintf(buf, STATUS_ANZG_MP2_TXT, extSimuModelName, 1, " ");
   if (sim_up)
      strcat(buf, JA_TXT);
   else
      strcat(buf, NEIN_TXT);
   write_buf(znr++, buf);                                    /* "sim_up" line */
   if (extractCaseFile)
      sprintf(buf, STATUS_ANZG_MP4_TXT, STATUS_ANZG_AKTIV_TXT);
   else
      sprintf(buf, STATUS_ANZG_MP4_TXT, STATUS_ANZG_AUS_TXT);
   write_buf(znr++, buf);                                 /* "case file" line */
   sprintf(buf, STATUS_ANZG_MP5_TXT, ext_sim_buf);
   write_buf(znr++, buf);                                 /* "matlab id" line */
   strcpy(buf, STATUS_ANZG_MP3_TXT);
  #else /* weder EXT_MATLAB_SIMU noch EXT_MATLAB_SIMU */
   if (mit_ext_simu)
   { /* Status des ext. Sumulators */
      if (sim_ok)
	 sprintf(buf, STATUS_ANZG_SIM1_TXT, STATUS_ANZG_AKTIV_TXT);
      else
	 sprintf(buf, STATUS_ANZG_SIM1_TXT, STATUS_ANZG_TOT_TXT);
      write_buf(znr++, buf);                                 /* "sim_ok" line */
      if (sim_up)
	 sprintf(buf, STATUS_ANZG_SIM2_TXT, STATUS_ANZG_AKTIV_TXT);
      else
	 sprintf(buf, STATUS_ANZG_SIM2_TXT, STATUS_ANZG_TOT_TXT);
   } /* Status des ext. Sumulators */
   else
      if (sim_up)
	 sprintf(buf, STATUS_ANZG_SIM3_TXT, STATUS_ANZG_AKTIV_TXT);
      else
	 sprintf(buf, STATUS_ANZG_SIM3_TXT, STATUS_ANZG_TOT_TXT);
   write_buf(znr++, buf);                                    /* "sim_up" line */
   strcpy(buf, STATUS_ANZG_LOG_TXT);
  #endif /* weder EXT_MATLAB_SIMU noch EXT_MATLAB_SIMU */
#endif /* no EXT_SIMU_SERV */
#ifdef MITSIMULOG
   if (mit_simulog)
      strcat(buf, STATUS_ANZG_AKTIV_TXT);
   else
      strcat(buf, STATUS_ANZG_AUS_TXT);
#else
   strcat(buf, NIX_LOG_TXT);
#endif /* MITSIMULOG */
   write_buf (znr, buf);                                    /* logging line */
} /* prep_status_anzg */



/* =========================== ak_mem_neu_bewert ============================ */
GSTATE ak_mem_neu_bewert (BOOL  clr_simu_flag)
{
   STR_TYPE             buf;
   BUF_TYPE             buf1;
   CHAIN_ELEM_PTR_TYPE  ak_ptr, adm_item_ptr, adm_chain_ptr;
   DOUBLE	        ak_note;
   INT                  guete, lfd_nr, del_ctr, ak_anz, 
                        znr = 1;
   GSTATE               ret_state;

   /* ak_mem_neu_bewert */
   ret_state    = clr_and_reorg (&adm_chain_ptr, AK);
   adm_item_ptr = adm_chain_ptr->u_el.next_action;
   ak_anz  = 0;
   del_ctr = 0;
   while ((adm_item_ptr != NULL) && (ret_state == GLEAM_OK))
   { /* Alle Verwaltungsketten-Items behandeln */
      ak_anz++;
      ret_state = remove_sub_chain (&ak_ptr, adm_item_ptr);
      if (clr_simu_flag)
	 ak_ptr->u_el.u_xel.u_hdr.chain_status &= ~SIMULATED;
      simulator (&ak_note, CALC_FITNESS, ak_ptr);
      if (ak_note != KEINE_NOTE)
      { /* AK wurde neu bewertet */
	 ak_ptr->u_el.u_xel.u_hdr.chain_status &= ~UNBEWERTETE_BASIS_AK;
	 ak_ptr->u_el.u_xel.u_hdr.fitness_note = ak_note;
	 if (berechne_guete (&guete, ak_note))
	    ret_state |= store_chain (&lfd_nr, ak_ptr, AK, guete);
	 else
	    ret_state |= fehler (SIMU_CMEN, 1, guete, NO_INFO, "", MEINE_GUETE);
      } /* AK wurde neu bewertet */
      else
      { /* nun ist sie unbewertbar geworden ... */
	 del_ctr++;
	 adr_to_text (buf1, ak_ptr->u_el.u_xel.u_hdr.guete,  
		      ak_ptr->u_el.u_xel.u_hdr.lfd_nr, NO_ADJUST);
	 sprintf (buf, NEUBEWERT_AK_DEL_TXT , del_ctr, buf1);
	 write_buf (znr++, buf);
	 ret_state |= delete_chain (&ak_ptr);
      } /* nun ist sie unbewertbar geworden ... */
      adm_item_ptr = adm_item_ptr->u_el.next_action;
   } /* Alle Verwaltungsketten-Items behandeln */
   if (ret_state == GLEAM_OK)
      ret_state = delete_chain (&adm_chain_ptr);
   sprintf (buf, NEUBEWERT_ERG_TXT, ak_anz);
   write_buf (znr, buf);
   return (ret_state);
} /* ak_mem_neu_bewert */



/* ============================= zeige_simu_bew ============================= */
INT zeige_simu_bew (INT   znr,
                    BOOL  mitIndex)
{
   if (int_sim_data_ready)
   { /* Bewertung der letzten interaktiven Simulation anzeigen */
      if (bew_aend_sim)
      { /* Aufgabe geaendert */
	 bewert_data_to_int_data (ak_bewerten (TRUE));  /*TRUE: interakt.Modus*/
	 bew_aend_sim = FALSE;
      } /* Aufgabe geaendert */
      write_buf (znr++, ZEIG_BEW_HDR_TXT);
      return (show_simu_erg (znr, mitIndex));
   } /* Bewertung der letzten interaktiven Simulation anzeigen */
   else {
      write_buf (znr++, NO_SIMU_DONE_TXT);
      return (znr);
   }
} /* zeige_simu_bew */



/* =========================== zeige_evo_best_bew =========================== */
INT zeige_evo_best_bew (INT   znr,
                        BOOL  mitIndex)
{
   DOUBLE  note;
   INT     ii;

   /* zeige_evo_best_bew */
   if (best_ges_note != -1)
   { /* Bewertung des Evo-Besten des letzten Evo-Laufs anzeigen */
      evo_plot_data_ready = TRUE;
      write_buf (znr++, ZEIG_EVO_BEW_HDR_TXT);
      if (bew_aend_sim)
      { /* Aufgabe geaendert */
	 for (ii = 0;  ii < akt_anz_krit;  ii++)
	    simu_erg[ii] = best_simu_erg[ii]; /* Neubewert.vorbereit.*/
	 note = ak_bewerten (mitIndex); /*Bewertung im interakt.Modus*/
	 bewert_data_to_int_data (note);
	 bewert_data_to_best_data (note);
	 bew_aend_sim = FALSE;
      } /* Aufgabe geaendert */
      else
      { /* AK-Werte auf Simu-Daten kopieren */
	 int_gesamt_note = best_ges_note;
	 for (ii = 0;  ii < akt_anz_krit;  ii++)
	    int_simu_erg[ii] = best_simu_erg[ii];
	 for (ii = 0;  ii < akt_anz_krit;  ii++)
	 {
	    int_ungew_note[ii] = best_ug_note[ii];
	    int_gew_note[ii]   = best_g_note[ii];
	    int_straf_fkt[ii]  = best_str_fkt[ii];
	    int_simValOutOfEvalRange[ii] = best_simValOutOfEvalRange[ii];
	 }
	 for (ii = 0;  ii < krit_og;  ii++)
	    int_krit_gueltig[ii] = best_krit_g[ii];
	 for (ii = 0;  ii < i_par_ges_anz;  ii++)
	    int_i_params[ii] = best_i_params[ii];
	 for (ii = 0;  ii < r_par_ges_anz;  ii++)
	    int_r_params[ii] = best_r_params[ii];
         int_sim_data_ready = TRUE;
      } /* AK-Werte auf Simu-Daten kopieren */
      return (show_simu_erg (znr, mitIndex)); 
   } /* Bewertung des Evo-Besten des letzten Evo-Laufs anzeigen */
   else
   {
      write_buf (znr, NO_EVO_DONE_TXT);
      return (znr);
   }
} /* zeige_evo_best_bew */



#ifdef GNUPLOT
/* =============================  plot_akt_wert  ============================ */
void plot_akt_wert (INT  krit_nr)
{
   STR_TYPE  gnu_dat_fname, buf;
   FILE     *gnu_dat_file;
   DOUBLE    rwert, rdelta;
   INT       ii, kk, anz;
   BOOL      straf_funkt = FALSE;

   /* plot_akt_wert */
   kk = krit_nr;
   if (kk > max_anz_bew_krit)
   { /* Straffunktion */
      kk -= max_anz_bew_krit;
      straf_funkt = TRUE;
   } /* Straffunktion */
   kk--;                                                  /* Nummer --> Index */
   if ((last_x_start <= int_simu_erg [kk]) && (int_simu_erg [kk] <= gnu_x_max))
   { /* aktueller Wert liegt im Anzeigebereich */
      sprintf (gnu_dat_fname, "k%02d_erg.tmp", krit_nr);
      gnu_dat_file = fopen (gnu_dat_fname, "w");
      if (straf_funkt)
	 rwert = int_straf_fkt[kk];                             /* Straffunktion */
      else
      { /* Bewertung */
	 rwert = int_ungew_note[kk];
	 rwert = rwert / 1000.0;
      } /* Bewertung */
      fprintf (gnu_dat_file, "%f %f\n", int_simu_erg [kk], rwert);
      fclose (gnu_dat_file);

      strcat (gnu_buffer, ", '");
      strcat (gnu_buffer, gnu_dat_fname);
      sprintf (buf, "' title '%s' with impulses lc rgbcolor \"#0000ff\" lw 1.6\n", PLOT_AKT_WERT_TXT);
      strcat (gnu_buffer, buf);
   } /* aktueller Wert liegt im Anzeigebereich */
   else {
      make_krit_text(buf, "%s", kk);
      meldung (SIMU_GSW, NO_INFO, NO_INFO, buf, SIMU_VAL_OUT_OF_RANGE);
   }
   fprintf (gnu_com_file, "%s\n", gnu_buffer);
   fclose (gnu_com_file);
   sprintf (gnu_buffer, "load '%s.cmd'\n", gnu_fname);
} /* plot_akt_wert */
#endif /* GNUPLOT */



