/*******************************************************************************
GLEAM/AE                                                                  V2.2.3

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: HAndlungsMODell spezifische software
                        Module: SEGMentierung und GENerierung
 
Package: hmod                   File: segm_gen.c 
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Der Modul umfasst alle allgemeinen Routinen zur Sequenz- und Kettengenerierung, 
Segmentierung, Aktionsparametrierung und -aenderung sowie zum Genetic Repair. 
Anwendungsspezifische Routinen dieser Funktionalitaet stehen in Module mit dem 
Anwendungskuerzel gefolgt von "_grep". 

Die Aktionen einer Kette bzw. einer Sequenz koennen fix in fester Reihenfolge 
oder zufaellig bestimmt werden. Die feste Reihenfolge wird durch die genetischen
Operatoren wie z.B. Rekombination veraendert. Diese Veraenderungen werden vom 
Genetic Repair nicht behoben und der Simulator muss dies beruecksichtigen. Eine 
"fixe" Reihenfolge bedeutet also vor allem, dass die Reihenfolge zwar veraender-
lich aber nicht bedeutungstragend ist.

Die Segmentlaengen unterliegen einer Schwankungsbreite, wobei die verschiedenen 
zulaessigen Laengen gleichwahrscheinlich (GLEICH_VERT) oder mit fallender Wahr-
scheinlichkeit (FALLEND_VERT) behaftet sein koennen. Bei fallender Wahrschein-
lichkeit ergibt sich folgende Verteilung, wobei "segm_werte_anz" die Anzahl der
zulaessigen Laengen darstellt:

    ak_slen_sum = SUM (1, ... , segm_werte_anz) 
                = (segm_werte_anz * (segm_werte_anz + 1)) / 2

           segm_werte_anz - i + 1
    P(i) = ----------------------    mit: i = 1, ..., segm_werte_anz
               ak_slen_sum

Bei der Parameteraenderung wird vom aktuellen Wert (PWert) ausgegangen. Je nach-
dem, ob zufallsbedingt vergroessert oder verkleinert werden soll, wird das In-
tervall [PWert, OG] bzw [UG, PWert] bestimmt, wobei UG und OG die im "act_descr"
der Aktion hinterlegten Parametergrenzen sind. Die Wertaenderungen finden in
diesem Intervall statt, wobei folgende Verteilung fuer den Umfang der Aenderung
gilt:
                      Aenderung im Bereich   Wahrscheinlichkeit
                            0 - 10 %               29,3 %
                           10 - 20 %               19,3 %
                           20 - 30 %               14,3 %
                           30 - 40 %               10,9 %
                           40 - 50 %                8,5 %
                           50 - 60 %                6,5 %
                           60 - 70 %                4,8 %
                           70 - 80 %                3,4 %
                           80 - 90 %                2,1 %
                           90 -100 %                1,0 %

Die Grenzen eines Parameters einer Aktion koennen identisch sein, der Parameter 
ist dann fixiert. Alle fixierten Parameter, die am Ende der Integer- bzw. Real-
parameterliste einer Aktion stehen, werden also solche erkannt und fuer Parame-
teraenderungen durch "modify_param" ausgeblendet.

Alle Routinen sind so implementiert, dass ein schlechter uebergebener Return-
status die Nichtausfuehrung der Routine bewirkt. 


Die Routinen im einzelnen:

BOOL   gen_chain (CHAIN_ELEM_PTR_TYPE *chain_ptr,
                  GSTATE              *ret_state);
          Die Funktion wuerfelt eine Kette aus, liefert sie in "chain_ptr" ab
          und ergibt TRUE, wenn alles gut ging. Wenn dazu nicht genuegend Spei-
          cher vorhanden ist, liefert die Funktion einen Nil-Pointer und FALSE 
          ab. Das Statusbit RANDOMLY_GENERATED ist gesetzt. 
          Bei GADO, MatLab und LESAK ("appl_code") oder Standard-AK-Generierung
          erfolgt die Steuerung durch die package-globalen Variablen "gen_len_
          mode" und "gen_akt_mode". Ansonsten wird an das "appl"-Package 
          verzweigt.
          "gen_len_mode" steuert die Längenbestimmung: Bei "FIX_LEN" wird die 
          im MOD-File angegebene "min_ketten_len" genommen; bei "RAND_LEN" 
          zwischen "min_ketten_len" und "max_ketten_len" ausgewürfelt. "gen_
          akt_mode" steuert die Aktionsauswahl derart, dass bei "FIX_ACT_FIX_
          GEN" jede gueltige Aktion in aufsteigender Reihenfolge 1x generiert 
          wird, bis alle Aktionen generiert sind. Bei "FIX_ACT_RAND_GEN" kommt 
          jede Aktion ebenfalls genau einmal vor, die Reihenfolge ist aber zu-
          faellig. Wenn dabei "len" groesser als "last_act" ist, erfolgt ein  
          fataler Fehler. Bei "RAND_ACT_GEN" werden die Aktionen willkuerlich 
          aus der Menge der gueltigen ausgewaehlt.

INT    calc_segm_len (INT  rest_len);
          Berechnet die Segmentlaenge ausgehend von "rest_len" und den in "ak_
          segm_laengen" hinterlegten Daten. Wenn "rest_len" <= "ak_max_segm_len"
          wird "rest_len" abgeliefert. Ansonsten wird die Laenge ausgehend von 
          der in "ak_segm_laengen" hinterlegten Verteilung bestimmt.

void   init_seq_gen (CHAIN_ELEM_PTR_TYPE  chain_ptr);
          Die interne Datenstruktur "akt_flags" wird zurueckgesetzt und ent-
          sprechend den in der Kette enthaltenen Aktionen initialisiert. Dies 
          dient der Vorbereitung einer Sequenzgenerierung mittels "gen_act_seq" 
          beim Generierungsmodus "FIX_ACT_RAND_GEN".

GSTATE gen_act_seq (CHAIN_ELEM_PTR_TYPE *seq_ptr,
                    CHAIN_ELEM_PTR_TYPE *seq_end_ptr,
                    INT                  number, 
                    INT                  start_act);
          Erzeugt eine Sequenz (d.h. eine Kette ohne Kopf) von "number" Aktionen
          und liefert dabei auftretende Fehlerstati als Funktionsergebnis ab. 
          Dabei wird genuegend Speicher vorausgesetzt. Das Ergebnis steht in
          "seq_ptr" und "seq_end_ptr".
          Bei GADO, MatLab und LESAK ("appl_code") oder Standard-AK-Generierung
          wird die Generierung durch die package-globalen Variablen "gen_len_
          mode" u. "gen_akt_mode" gesteuert, die die gleiche Bedeutung wie in 
          "gen_chain" haben. Ansonsten wird an das "appl"-Package verzweigt.
          Die Aktionen werden mit der Funktion "parameterize" parametriert mit 
          Ausnahme von "activity_act"-Aktionen, die mit "select_action_chain" 
          parametriert werden. Wenn "number" null ist, wird fuer beide Pointer 
          NIL abgeliefert. Die Routine setzt voraus, dass genuegend Speicher-
          platz zur Generierung der Sequenz zur Verfuegung steht.
          "start_act" hat nur bei den Generierungsmodes "FIX_ACT_FIX_GEN" und 
          "FIX_ACT_RAND_GEN" eine Wirkung: 
          Bei "FIX_ACT_FIX_GEN" wird die Generierung mit "start_act" begonnen.
          Bei "FIX_ACT_RAND_GEN" dient "start_act" lediglich Prüfzwecken und 
          gibt die laufende Nummer der nächsten zu generierenden Aktion an. Wenn
          z.B. eine Kette noch 5 Elemente enthält und 8 haben soll, muessen 
          "number"=3 ab "start_act"=6 Aktionen generiert werden. Ausserdem 
          muessen die bereits benutzten Aktionen der Kette durch einen Aufruf 
          von "init_seq_gen" zuvor ermittelt worden sein. 

BOOL   parameterize (CHAIN_ELEM_PTR_TYPE  item_ptr,
                     GSTATE              *ret_state);
          Parametriert die durch "item_ptr" bezeichnete Aktion. Die Routine lie-
          fert TRUE, wenn "item_ptr" nicht NIL ist und es sich um eine von "ac-
          tivity_act" verschiedene Aktion handelt, deren Code kleiner als "last_
          act" ist. Bei NIL oder unzulaessigem "activity_act"-Wert erfolgt ein 
          fataler Fehler und "ret_state" liefert GLEAM_FATAL.
          Bei GADO, MatLab und LESAK ("appl_code") erfolgt die Parametrierung 
          mittels der Routinen "calc_i_param" und "calc_r_param". Ansonsten 
          fataler Fehler.
          Diese wuerfeln den Parameter gleich-verteilt aus dem im Handlungs-
          modell vorgegebenen Wertebereich aus. "activity_act"-Aktionen werden 
          nicht veraendert.  

BOOL   modify_param (CHAIN_ELEM_PTR_TYPE  item_ptr,
                     INT                  mod_kind,
                     GSTATE              *ret_state);
          Grosse/Kleine Veraenderung oder Neubestimmung einer auszuwuerfelnden 
          Anzahl von Parametern der durch "item_ptr" bezeichneten Aktion". 
          "activity_act"-Aktionen werden nicht veraendert. Die Routine liefert 
          TRUE, wenn sie mindestens einen Parameter der Aktion veraendert hat 
          und keine Fehler auftraten. Sie liefert FALSE, wenn die Aktion entwe-
          der keine Parameter enthaelt oder wenn die Unter- und Obergrenzen der
          ausgewaehlten Parameter identisch und daher unveraendert geblieben 
          sind oder wenn alle Parameter zufallsbedingt unveraendert blieben oder
          ein (fataler) Fehler auftrat, der sich auch im "ret_state" ausdrueckt.
          Bei Aktionen mit Parametern liegt die Anzahl der zu modifizierenden 
          Parameter zwischen 1 und der Anzahl der Parameter der Aktion. 
          Bei gesetztem "std_ak_appl" oder "std_segm_gen_appl" (z.B. GADO, GAMA
          oder LESAK; sonst Fehler) wird die Anzahl zu veraendernder Parameter
          durch eine klassengesteuerte Verteilung mit "par_ges_anz" Klassen 
          durch die Routine "calc_par_anz" bestimmt. Die Parameter selbst werden
          dann per Zufall bestimmten und der Wert wird in Abhaengigkeit von 
          "mod_kind" folgendermassen neu bestimmt, wobei die zum Parameter ge-
          hoerigen Parametergrenzen eingehalten werden:
          mod_kind == CHANGE_NEW:
            Der Wert wird neu bestimmt.
          mod_kind == CHANGE_REL:
            Der Wert wird relativ zum aktuellen Wert wie im allg. Teil erlaeu-
            tert veraendert.
          mod_kind == CHANGE_SMALL:
            Der Wert wird um maximal SMALL_CHANGE_FRAC vom Paramterintervall 
            veraendert wobei eine aehnliche Verteilung wie CHANGE_REL benutzt 
            wird.
          
GSTATE repair_2_aks (CHAIN_ELEM_PTR_TYPE  ak1_ptr,
                     CHAIN_ELEM_PTR_TYPE  ak2_ptr);
          Die Routine ist fuer AKs fester Länge vorgesehen, d.h. "gen_akt_mode"
          muss die Werte FIX_ACT_FIX_GEN oder FIX_ACT_RAND_GEN haben. Dies ent-
          spricht derzeit dem Kontext der Anwendungen GADO und MatLab.
          Sie fuehrt ein Genetic Repair fuer 2 AKs fester Laenge durch, die der
          Rekombination bzw. einem Cross-Over unterworfen waren. Es werden immer
          zwei korrekte AKs abgeliefert. Dies geschieht dadurch, dass Kettenele-
          mente, die in der einen Kette fehlen und die in der anderen demzufolge
          doppelt sind, rueckgetauscht werden. Dabei wird zunaechst nach Paaren 
          in beiden Ketten gesucht und die Inhalte des jeweils letzte Paarele-
          ments getauscht. Dadurch wird die Segmentstruktur bewahrt, durch das 
          Tauschen des letzten Paarelements bleibt die Mutationsaenderung meist 
          erhalten. Bei danach noch verbleibenden doppelten Aktionen einer Kette
          werden die jeweils zuletzt gefundenen an die andere Kette angehaengt.
          Das letzte Segment wird nachsegmentiert, wenn seine Segmentlaenge zu
          gross geworden ist. Im Ergebnis entstehen 2 Ketten, die alle Elemen-
          te jeweils einmal aber in beliebiger Reihenfolge enthalten.
          Der Return-Status wird bei unzulaessigem "appl_code" oder bei aufge-
          tretenen SW-Fehlern wegen unzulaessiger Ausgangsketten schlecht.

BOOL   repair_1_chain (CHAIN_ELEM_PTR_TYPE *ch_ptr,
                       GSTATE              *ret_state);
          Die Routine fuehrt ein Genetic Repair fuer eine Kette durch und lie-
          fert TRUE ab, wenn sie korrigiert werden konnte und keine Fehler auf-
          traten. Ansonsten FALSE, wobei die Kette geloescht und "ch_ptr" NIL 
          wird, sofern zuvor keine Fehler auftraten. Leere Ketten sind immer 
          irreparabel unplausibel.
          Das Genetic Repair ist anwendungsabhaengig ("appl_code") und kann auch
          einen Plausibilitaetstest beinhalten. 

GSTATE segmentiere_ch (CHAIN_ELEM_PTR_TYPE  ch_ptr,
                       BOOL                 komplett);
          Bei gesetztem "komplett" wird die gesamte Kette "ch_ptr" neusegmen-
          tiert. Eine eventuell bereits vorhandene Segmentierung geht dabei zu-
          sammen mit den Descriptoren verloren! Bei nicht gesetztem "komplett"
          wird nur das letzte Segment neu segmentiert, sofern es zu lang ist 
          (> "ak_max_segm_len"). Dabei werden eventuell vorhandene ueberflues-
          sige Descriptoren korrekt geloescht. Die konkreten Segmentlaengen 
          werden entsprechend der vorgegebenen Verteilung berechnet. Die Routi-
          ne liefert einen schlechten Returnstatus, wenn "ch_ptr" Nil oder 
          keine segmentierbare Kette ist oder bei der Segmentierung Fehler auf-
          traten.


Package-globale Routinen:

DOUBLE calc_max_abst (INT  len);
          Berechnet fuer Module "hamming" den "max_abst", der zur Berechnung der
          Abweichungen der Aktionsreihenfolge gebraucht wird. Definition, siehe
          Module "hamming".

void   reset_seq_gen (void);
          Die interne Datenstruktur "akt_flags" wird zur Vorbereitung einer 
          Ketten- oder Sequenzgenerierung zurueckgesetzt.

INT    kettenlaenge (GSTATE *ret_state);
          Berechnet die Laenge einer neuen Kette und liefert sie ab. Bei auftre-
          tenden Fehlern ist "ret_state" von "GLEAM_OK" verschieden und der 
          Funktionswert ist undefiniert.
          Bei GADO, MatLab und LESAK ("appl_code"; sonst Fehler) geschieht dies 
          gemaess der packag-globalen Variable "gen_len_mode": 
          Bei "FIX_LEN" wird die "min_ketten_len" abgeliefert. Bei "RAND_LEN" 
          wird frei zwischen "min_ketten_len" u. "max_ketten_len" ausgewuerfelt.

--------------------------------------------------------------------------------
basiert auf C-Unit: ptest_gen.
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
  Verlagerung der "act_descr"- und MOD-File-Ladefunktionen zum Modul "mod_data".
Aenderung gegenueber Version 2.0.0:
 1. Erweiterung der Routine "modify_param" um kleine Aenderungen fuer die neuen 
    gen. Operatoren PAR_CHANGE_SMALL und SEGM_CHANGE_SMALL.
 2. Modul "lsk_grep" ins "simu"/"lskp"-Package verschoben.
 3. Korr. wegen g++-Compiler. "calc_par_anz" wird lokale Routine.    WJ 13.08.00
 4. "calc_i_param" fuer grosse Wertebereiche angepasst.              WJ 12.02.00
 5. Linux-Portierung: Aus "round" wird "i_round".                    WJ 11.05.05
 6. Durch "gen_chain" erzeugte AKs haben das Statusbit 
    RANDOMLY_GENERATED gesetzt.                                      WJ 09.02.06
 7. Erweiterung fuer die MatLab-Koppung: Aus GAMA wird MatLab.       WJ 22.08.07
 8. Integration der MBFs der CEC05-Sammlung (CEC_MATHFKT_APPL).      WJ 26.10.07
 9. Anpassung an neue Datentypen fuer Int- und Real-Parameter
    (neues "chaindef.h" fuer 64 bit)                                 WJ 07.01.08
10. Kleine Laufzeitoptimierung in "small_r_change".                  WJ 29.04.08
11. "calc_r_param" an SonderInit fuer CEC f07 angepasst.             WJ 02.06.08
12. Entfernung ueberfluessiger Variable.                             WJ 26.06.12
13. Renovierung: Ersetzung von testweisen "write_line"-Ausgaben durch
    "printf"-Aufrufe. Anpassung der "char_dialog"- und "lies_wert"-
    Aufrufe.                                                         WJ 04.05.16
14. Integration von SIMU_SERV_APPL.                                  WJ 29.05.17
15. change_i_param() effizienter fuer quasi-boolsche Parameter 
    gemacht.                                                         WJ 28.10.19
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 28.10.2019
================================================================================ */
#include <stdio.h>
#include <stdlib.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "lgsw.h"
#include "appl.h"
#include "simu.h"
#include "hmod.h"
#include "hmodInt.h"


#ifdef GLO_CHECKS
  #define CHECKS
#endif
#undef TEST
#undef MTEST
#undef ZTEST


/* ============================ Lokale Konstanten: ========================== */
#define MAX_AKTPAR   (I_PAR_ANZ_MAX + R_PAR_ANZ_MAX)



/* ========================= Lokale Typdeklarationen: ======================= */
typedef BOOL  AKTPAR_MARK_TYP [MAX_AKTPAR];



/* ============================ Lokale Variable: ============================ */
static INT             act_ctr;           /* Zaehlt die bereits gen.Acts mit  */

static CHAIN_ELEM_PTR_TYPE  gc_chain_ptr;    /* fuer "gen_chain"              */
static REF_DESCR_PTR_TYPE   gc_sd_ptr;       /* fuer "gen_chain"              */
static INT                  gc_sd_index;     /* fuer "gen_chain"              */
static INT                  gc_abschn_anz;   /* fuer "gen_chain"              */
static CHAIN_ELEM_PTR_TYPE *gas_seq_end_ptr; /* f. "gen_act_seq"              */
static CHAIN_ELEM_PTR_TYPE  gas_new_ptr;     /* f. "gen_act_seq"              */

#if defined (TEST) || defined (MTEST) || defined (ZTEST)
static STR_TYPE        buf1, buf2;
#endif



/* ============================ Lokale Routinen: ============================ */
static GSTATE first_segm       (INT    len);

static GSTATE next_segm        (INT    len);

static GSTATE do_gen_act_seq   (CHAIN_ELEM_PTR_TYPE *seq_ptr,
                                CHAIN_ELEM_PTR_TYPE *seq_end_ptr,
                                INT                  number);

static GSTATE define_act       (void);

static SHORT  calculate_act    (GSTATE *ret_state);

static INT    calc_i_param     (SHORT   act,
                                INT     par_ind);    

static DOUBLE calc_r_param     (SHORT   act,
                                INT     par_ind);    

static INT    calc_par_anz     (SHORT            act_code,
                                AKTPAR_MARK_TYP  mod_aktpar);

static BOOL   change_i_param   (INT    *wert,
                                INT     act,
                                INT     par_ind);

static BOOL   small_i_change   (INT    *wert,
                                INT     act,
                                INT     par_ind);

static BOOL   change_r_param   (DOUBLE *wert,
                                SHORT   act,
                                INT     par_ind);

static BOOL   small_r_change   (DOUBLE *wert,
                                SHORT   act,
                                INT     par_ind);

static VOID   analyze_ak       (TAB_ELEM_TYPE       *akt_tab,
                                INT                 *akt_d_anz,
                                CHAIN_ELEM_PTR_TYPE  ak_ptr);

static GSTATE phase2           (TAB_ELEM_TYPE       *akt_tab,
                                INT                  akt_d_anz,
                                CHAIN_ELEM_PTR_TYPE  ak_ptr,
                                CHAIN_ELEM_PTR_TYPE  akx_ptr);





/* ================================ gen_chain =============================== */
BOOL gen_chain (CHAIN_ELEM_PTR_TYPE *chain_ptr,
                GSTATE              *ret_state)
{
   BOOL  r_gen_chain;
   INT   kum_len, abschn_len, len;
  
   /* gen_chain */
   *chain_ptr = NULL;
   if (std_ak_appl)
   { /* LESAK, GADO, MatLab oder Standard-Generierung bei Nicht-Std-Appl */
#ifdef MTEST
      lies_wert (&len, "GEN: Kettenlänge: ", 0, K32, K32+1);
#else
      len = kettenlaenge (ret_state);
#ifdef ZTEST
      sprintf (buf1, "GEN: len=%1u", len);
      printf ("%s\n", buf1);
#endif
#endif /* MTEST */
      r_gen_chain = enough_free_mem (AK_HEADER, len,     
                                     (INT)(len / min_abschn_len) + 1);
      if (r_gen_chain && (*ret_state == GLEAM_OK))
      { /* enough memory und uebergebener Status ist ok */
         if (gen_akt_mode <= FIX_ACT_RAND_GEN)
         { /*Aktionsausw.u.-sequenzierung: FIX_ACT_RAND_GEN od.FIX_ACT_FIX_GEN*/
            act_ctr = ACTIVITY_ACT;
            if (gen_akt_mode == FIX_ACT_RAND_GEN)
               reset_seq_gen ();
         } /*Aktionsausw.u.-sequenzierung: FIX_ACT_RAND_GEN od.FIX_ACT_FIX_GEN*/
         gc_abschn_anz = 0;
         gc_sd_index   = 1;
         *ret_state    = init_chain (chain_ptr, AK_HEADER);
         gc_chain_ptr  = *chain_ptr;
         if ((len > 0) && (*ret_state == GLEAM_OK))
         { /* Keine leere Kette und Status ok */
            if ((*ret_state = new_ref_descr (&gc_sd_ptr)) == GLEAM_OK)
            { /* 1.SegmDescr erhalten */
               gc_sd_ptr->segm_ptrs[0] = gc_chain_ptr;
               gc_chain_ptr->u_el.u_xel.u_hdr.segm_descr_ptr = gc_sd_ptr;
               if (len > min_abschn_len) 
               { /* Kettenlaenge > min_abschn_len: 1 oder mehrere Abschnitte */
                  abschn_len = calc_segm_len (len);
#ifdef TEST
                  sprintf (buf1, "len\'s: %1u", abschn_len);
#endif
                  kum_len = abschn_len;
                  *ret_state = first_segm (abschn_len);       /* 1. Abschnitt */
                  while ((kum_len < len) && (*ret_state == GLEAM_OK))
                  { /* Alle weiteren Abschnitte */
                     abschn_len = calc_segm_len (len - kum_len);
#ifdef TEST
                     sprintf (buf2, " %1u", abschn_len);
                     strcat (buf1, buf2);
#endif
                     kum_len += abschn_len;
                     *ret_state = next_segm (abschn_len);     /* n. Abschnitt */
                  } /* Alle weiteren Abschnitte */
#ifdef TEST
                  printf ("%s\n", buf1);
                  sprintf (buf1, "kum_len=%1u", kum_len);
                  printf ("%s\n", buf1);
#endif
               } /* Kettenlaenge > min_abschn_len: 1 oder mehrere Abschnitte */
               else 
                  *ret_state = first_segm (len);               /* 1 Abschnitt */
               gc_chain_ptr->u_el.u_xel.u_hdr.chain_length = len;
               gc_chain_ptr->u_el.u_xel.u_hdr.segment_anz  = gc_abschn_anz;
            } /* 1.SegmDescr erhalten */
         } /* Keine leere Kette und Status ok */
      } /* enough memory und uebergebener Status ist ok */
   } /* LESAK, GADO, MatLab oder Standard-Generierung bei Nicht-Std-Appl */
   else
      r_gen_chain = add_anw_ch_gen (chain_ptr, ret_state);    /* spez. AK-Gen */
   if (*chain_ptr != NULL)
      (*chain_ptr)->u_el.u_xel.u_hdr.chain_status |= RANDOMLY_GENERATED;
   return (r_gen_chain && (*ret_state == GLEAM_OK));
} /* gen_chain */



/* ============================= calc_segm_len ============================== */
INT calc_segm_len (INT  rest_len)
{
   INT  z_zahl, segm_len;

   /* calc_segm_len */
   if (rest_len <= ak_max_segm_len)
      return (rest_len);
   else
   { /* Segmentlaenge berechnen */
      if (segm_werte_anz == 1)
         return (min_abschn_len);
      else
      { /* Segmentlaenge auswuerfeln */
         z_zahl   = irand (ak_slen_sum) + 1;
         segm_len = 0;
         while (z_zahl > ak_segm_laengen[segm_len])
            segm_len++;
         return (segm_len + min_abschn_len);
      } /* Segmentlaenge auswuerfeln */
   } /* Segmentlaenge berechnen */
} /* calc_segm_len */



/* =============================== first_segm =============================== */
static GSTATE first_segm (INT  len)
/*----------------------------------------------------------------------------*/
/*  Erzeugt unter Verwendung der globalen Variablen "gc_chain_ptr", "gc_sd_   */
/*  ptr" und "gc_abschn_anz" das erste Segment mit der Laenge "len" der in    */
/*  "gc_chain_ptr" zu erzeugenden Kette. Als Funktionsergebnis wird der Sta-  */
/*  tus derbenutzten Routine "do_gen_act_seq" abgeliefert.                    */
/*----------------------------------------------------------------------------*/
{
   GSTATE  rstate;

   /* first_segm */
   rstate = do_gen_act_seq (&(gc_chain_ptr->u_el.next_action), 
                            &(gc_chain_ptr->u_el.u_xel.u_hdr.last_action), len);
   gc_sd_ptr->segm_ptrs  [1] = gc_chain_ptr->u_el.u_xel.u_hdr.last_action;
   gc_sd_ptr->segm_length[1] = len;
   gc_abschn_anz++;
   return (rstate);
} /* first_segm */



/* ================================ next_segm =============================== */
static GSTATE next_segm (INT  len)
/*----------------------------------------------------------------------------*/
/*  Erzeugt unter Verwendung der globalen Variablen "gc_chain_ptr", "gc_sd_   */
/*  ptr", "gc_sd_index" und "gc_abschn_anz" ein Folgesegment mit der Laenge   */
/*  "len" der in "gc_chain_ptr" zu erzeugenden Kette. Als Funktionsergebnis   */
/*  wird der Status der benutzten Routinen "do_gen_act_seq", "new_ref_descr"  */
/*  und "inc_segm_nr" abgeliefert.                                            */
/*----------------------------------------------------------------------------*/
{
   GSTATE  rstate;

   /* next_segm */
   rstate = do_gen_act_seq (&(gc_chain_ptr->u_el.u_xel.u_hdr.last_action->
                              u_el.next_action),
                            &(gc_chain_ptr->u_el.u_xel.u_hdr.last_action), len);
   if ((gc_sd_index + 1) == SEGM_PTR_ANZ) 
      rstate |= new_ref_descr (&(gc_sd_ptr->next_ref_descr_ptr));
   if ((rstate |= inc_segm_nr (&gc_sd_ptr, &gc_sd_index)) == GLEAM_OK)
   { /* Status ok */
      gc_sd_ptr->segm_ptrs  [gc_sd_index] = 
         (gc_chain_ptr)->u_el.u_xel.u_hdr.last_action;
      gc_sd_ptr->segm_length[gc_sd_index] = len;
      gc_abschn_anz++;
   } /* Status ok */
   return (rstate);
} /* next_segm */



/* ============================== init_seq_gen ============================== */
void init_seq_gen (CHAIN_ELEM_PTR_TYPE  chain_ptr)
{
   CHAIN_ELEM_PTR_TYPE  akt_ptr;

   /* init_seq_gen */
   reset_seq_gen ();
   akt_ptr = chain_ptr->u_el.next_action;
   while (akt_ptr != NULL)
   { /* all along the chain */
      akt_flags[akt_ptr->u_el.activity] = TRUE;
      akt_ptr = akt_ptr->u_el.next_action;
   } /* all along the chain */
} /* init_seq_gen */



/* ============================== gen_act_seq =============================== */
GSTATE gen_act_seq (CHAIN_ELEM_PTR_TYPE *seq_ptr,
                    CHAIN_ELEM_PTR_TYPE *seq_end_ptr,
                    INT                  number, 
                    SHORT                start_act)
{
   if (std_ak_appl || std_segm_gen_appl)
   { /* LESAK, GADO, MatLab oder Standard-Generierung bei Nicht-Std-Appl */
      act_ctr  = start_act - 1;
      return (do_gen_act_seq (seq_ptr, seq_end_ptr, number));
   } /* LESAK, GADO, MatLab oder Standard-Generierung bei Nicht-Std-Appl */
   else
      return (add_gen_act_seq (seq_ptr, seq_end_ptr, number));
} /* gen_act_seq */



/* ============================= do_gen_act_seq ============================= */
static GSTATE do_gen_act_seq (CHAIN_ELEM_PTR_TYPE *seq_ptr,
                              CHAIN_ELEM_PTR_TYPE *seq_end_ptr,
                              INT                  number)
/*----------------------------------------------------------------------------*/
/*  Die Routine generiert eine Sequenz der Laenge "number" und liefert sie in */
/*  "seq_ptr" und "seq_end_ptr" ab. Die globalen Variablen zur Steuerung des  */
/*  Generierungsmodus "gen_kind" und ggf. "act_ctr" muessen vor Aufruf ge-    */
/*  setzt worden sein. Wenn "number" gleich Null ist, werden leere Pointer    */
/*  abgeliefert. Als Funktionsergebnis wird der Status der benutzten Routinen */
/*  "define_act" und "new_chain_elem" abgeliefert.                            */
/*  Arbeitet im Kontext der Anwendungen LESAK, GADO, MatLab.                  */
/*----------------------------------------------------------------------------*/
{
   INT                  ii;
   GSTATE               ret_state = GLEAM_OK;
  
   /* do_gen_act_seq */
   gas_seq_end_ptr = seq_end_ptr;
   if (number > 0) 
   { /* number > 0 */
      ret_state    = new_chain_elem (seq_ptr);
      *seq_end_ptr = *seq_ptr;
      for (ii = 1;  (ii < number) && (ret_state == GLEAM_OK);  ii++) 
      { /* alle weiteren Items */
         if ((ret_state = new_chain_elem (&gas_new_ptr)) == GLEAM_OK)
            ret_state = define_act ();
         *seq_end_ptr = gas_new_ptr;
      } /* alle weiteren Items */
      gas_new_ptr = NULL; /*damit "define_act" die Sequenz mit NULL terminiert*/
      ret_state |= define_act ();
   } /* number > 0 */
   else 
   { /* eine leere Sequenz */
      *seq_ptr     = NULL;
      *seq_end_ptr = NULL;
   } /* eine leere Sequenz */
   return (ret_state);
} /* do_gen_act_seq */



/* =============================== define_act =============================== */
static GSTATE define_act (void)
/*----------------------------------------------------------------------------*/
/*  Legt bei der Kettengenerierung die Daten der durch die globale Variable   */
/*  "gas_seq_end_ptr" bezeichneten Aktion fest. Als Funktionsergebnis wird der*/
/*  Status der benutzten Routinen "calculate_act", "parameterize" und "select_*/
/*  action_chain" abgeliefert.                                                */
/*  Arbeitet im Kontext der Anwendungen LESAK, GADO, MatLab.                  */
/*----------------------------------------------------------------------------*/
{
   SHORT   act;
   GSTATE  ret_state = GLEAM_OK;
  
   /* define_act */
   (*gas_seq_end_ptr)->u_el.next_action = gas_new_ptr;
   (*gas_seq_end_ptr)->u_el.activity    = calculate_act (&ret_state);
   if (ret_state != GLEAM_OK)
      return (ret_state);
   if ((*gas_seq_end_ptr)->u_el.activity == ACTIVITY_ACT) 
   { /* activity_act */
      (*gas_seq_end_ptr)->u_el.u_xel.u_sub.sub_chain = NULL;
      if (!select_action_chain (*gas_seq_end_ptr, &ret_state))
      { /* no action chain available */
         do 
            act = calculate_act (&ret_state);
         while ((act == ACTIVITY_ACT) && (ret_state == GLEAM_OK));
         (*gas_seq_end_ptr)->u_el.activity = act;
         parameterize (*gas_seq_end_ptr, &ret_state);
      } /* no action chain available */
   } /* activity_act */
   else 
      parameterize (*gas_seq_end_ptr, &ret_state);
   return (ret_state);
} /* define_act */



/* ============================== calculate_act ============================= */
static SHORT calculate_act (GSTATE *ret_state)
/*----------------------------------------------------------------------------*/
/*  Erwuerfelt den Aktionscode entsprechend den globalen Variablen "gen_akt_  */
/*  mode" und ggf. "act_ctr" und liefert diesen als Funktionsergebnis ab.     */ 
/*  Wenn "gen_akt_mode" einen unzulaessigen Wert hat oder bei FIX_ACT_?_GEN   */
/*  die zu generierende Kette laenger sein soll, als es Aktionen hat, wird    */
/*  in "ret_state" ein schlechter Status abgeliefert und das Funktionsergeb-  */
/*  nis ist undefiniert.                                                      */
/*  Arbeitet im Kontext der Anwendungen LESAK, GADO, MatLab.                  */
/*----------------------------------------------------------------------------*/
{
   SHORT  ai;
   INT    selector, index;
   
   /* calculate_act */
   if (*ret_state == GLEAM_OK)
   { /* Status ok */
      if (gen_akt_mode == RAND_ACT_GEN) 
      { /* Aktionen frei aus der Menge der gueltigen auswuerfeln */
         selector = irand (act_prio_sum);
         ai       = 0;
         do 
            ai++;
         while (selector >= act_descr[ai]->prio_sum);
      } /* Aktionen frei aus der Menge der gueltigen auswuerfeln */
      else if (gen_akt_mode == FIX_ACT_FIX_GEN)
      { /* jede Aktion 1x in aufsteigender Reihenfolge */
         act_ctr++;
         if (act_ctr > last_act)
            *ret_state = fatal (HMOD_SEGM_GEN, 1, last_act+1, act_ctr,  "", 
                                ZU_WENIG_AKTIONEN);
         else
            ai = act_ctr;
      } /* jede Aktion 1x in aufsteigender Reihenfolge */
      else if (gen_akt_mode == FIX_ACT_RAND_GEN)
      { /* jede Aktion 1x in zufaelliger Reihenfolge */
         act_ctr++;
         if (act_ctr > last_act)
            *ret_state = fatal (HMOD_SEGM_GEN, 2, act_ctr, NO_INFO, "", 
                                BAD_ACT_AMOUNT);
         if (act_ctr < act_anz_haelfte)
         { /* bisher unbenutzte Aktion auswuerfeln */
            do
               ai = irand (last_act) + 1;
            while (akt_flags[ai]);
         } /* bisher unbenutzte Aktion auswuerfeln */
         else
         { /* wuerfeln und suchen */
            index = irand (last_act - act_ctr + 1) + 1;     /* 1 .. AnzUnused */
            ai    = 1;
            while ((ai <= last_act) && (index > 0))
            {
               if (!akt_flags[ai])
                  index--;                           /* freie Aktion gefunden */
               if (index != 0)
                  ai++;
            }
            if (index > 0)
               *ret_state |= fatal (HMOD_SEGM_GEN, 3, act_ctr, NO_INFO, "", 
                                    BAD_ACT_AMOUNT);
         } /* wuerfeln und suchen */
         akt_flags[ai] = TRUE;
      } /* jede Aktion 1x in zufaelliger Reihenfolge */
      else
         *ret_state = fatal (HMOD_SEGM_GEN, 4, gen_akt_mode, NO_INFO, "", 
                             BAD_GEN_KIND);
   } /* Status ok */
   return (ai);
} /* calculate_act */



/* ============================== parameterize ============================== */
BOOL parameterize (CHAIN_ELEM_PTR_TYPE  item_ptr,
                   GSTATE              *ret_state)
{
   INT    ii;
   SHORT  act;
   BOOL   done = FALSE;
  
   /* parameterize */
#ifdef CHECKS
   if (item_ptr == NULL) 
      *ret_state |= fatal (HMOD_SEGM_GEN, 5, NO_INFO, NO_INFO, "", NIL_PTR);
#endif
   if (*ret_state == GLEAM_OK)
   { /* Status ok */
      act = item_ptr->u_el.activity;
      if ((ACTIVITY_ACT < act) && (act <= last_act))
      { /* Gueltige Aktion */
         if (std_ak_appl || std_segm_gen_appl)
         { /* LESAK, GADO, MatLab: Aktion (AK-Element) */
            for (ii = 0;  ii < act_descr[act]->i_par_anz;  ii++)
               item_ptr->u_el.u_xel.u_act.i_params[ii] = calc_i_param (act, ii);
            for (ii = 0;  ii < act_descr[act]->r_par_anz;  ii++)
               item_ptr->u_el.u_xel.u_act.r_params[ii] = calc_r_param (act, ii);
            done = TRUE;
         } /* LESAK, GADO, MatLab: Aktion (AK-Element) */
         else
         { /* andere Anwendung: noch nicht implementiert */
            *ret_state = fatal (HMOD_SEGM_GEN, 6, NO_INFO, NO_INFO, "", 
                                NOT_YET_IMPLEMENTED);
         } /* andere Anwendung: noch nicht implementiert */
      } /* Gueltige Aktion */
      else 
         if (act != ACTIVITY_ACT)
            *ret_state = fatal (HMOD_SEGM_GEN, 7, act, NO_INFO, "", BAD_ITEM);
   } /* Status ok */
   return (done);
} /* parameterize */



/* ============================== calc_i_param ============================== */
static INT calc_i_param (SHORT  act,
                         INT    par_ind)     
/*----------------------------------------------------------------------------*/
/*  Erwuerfelt den Parameterwert entsprechend den zu "act" und "par_nr"       */
/*  gehoerigen Eintraegen in "act_descr" und liefert den Integer-Wert ab.     */
/*  Arbeitet im Kontext der Anwendungen LESAK, GADO, MatLab.                  */
/*----------------------------------------------------------------------------*/
{
   /* calc_i_param */
   if (act_descr[act]->i_par_descr[par_ind].big_range)
   { /* Wertebereich > INT_MAX */
      return ((INT)(frand() * act_descr[act]->i_par_descr[par_ind].par_bereich) +
              act_descr[act]->i_par_descr[par_ind].par_u_grenze);
   } /* Wertebereich > INT_MAX */
   else
      return (irand ((INT)(act_descr[act]->i_par_descr[par_ind].par_bereich) + 1)
              + act_descr[act]->i_par_descr[par_ind].par_u_grenze);
} /* calc_i_param */



/* ============================== calc_r_param ============================== */
static DOUBLE calc_r_param (SHORT  act,
                            INT    par_ind)    
/*----------------------------------------------------------------------------*/
/*  Erwuerfelt den Parameterwert entsprechend den zu "act" und "par_nr"       */
/*  gehoerigen Eintraegen in "act_descr" und liefert den REAL-Wert ab.        */
/*  Arbeitet im Kontext der Anwendungen LESAK, GADO, MatLab.                  */
/*----------------------------------------------------------------------------*/
{
   DOUBLE bereich;
  
   /* calc_r_param */
   bereich = act_descr[act]->r_par_descr[par_ind].par_bereich;
   if (mit_cec_f07) {
      bereich = act_descr[act]->r_par_descr[par_ind].par_o_grenze;
      return (frand()*bereich);
   }
   else
      return (frand()*bereich + act_descr[act]->r_par_descr[par_ind].par_u_grenze);
} /* calc_r_param */



/* ============================== calc_par_anz ============================== */
static INT calc_par_anz (SHORT            act_code,
                         AKTPAR_MARK_TYP  mod_aktpar)
/*----------------------------------------------------------------------------*/
/*  Liefert die Anzahl der zu aendernden Aktionsparameter zurueck und mar-    */
/*  kiert die betroffenen Paramter in "mod_aktpar". Bei Aktionen ohne Para-   */
/*  meter liefet die Routine 0 ab. Sonst liegt die Anzahl zwischen 1 und der  */
/*  Gesamtzahl der Parameter der Aktion.                                      */
/*  Bei GADO, MatLab und LESAK ("appl_code"; sonst Fehler) geschieht dies     */
/*  folgendermassen:                                                          */
/*  Auf das BOOL-Feld "mod_aktpar" werden zuerst die Integer- und dann die    */
/*  Real-Parameter abgebildet. Zur Markierung werden soviel Klassen gebil-    */
/*  det, wie die Aktion insgesamt Paramater hat und eine Klasse per Zufall    */
/*  gleichverteilt ausgewaehlt. Die Klassennummer repraesentiert dann die     */
/*  Obergrenze der auszuwuerfelnden Aenderungs-Anzahl. Damit ergibt sich      */
/*  eine fallende Verteilung derart, dass die Aenderung von nur einem Para-   */
/*  meter am wahrscheinlichsten ist (vgl. Verteilung bei der Parameterwert-   */
/*  aenderung).                                                               */
/*----------------------------------------------------------------------------*/
{
   INT  ges_par_anz, index, ii;
   INT  aend_par_anz = 0;

   /* calc_par_anz */
   if (std_ak_appl || std_segm_gen_appl)
      ges_par_anz = act_descr[act_code]->aend_ges_par_anz; /*LESAK,GADO,MatLab*/
   else
   { /* andere Anwendung: noch nicht implementiert */
      ges_par_anz = 0;
      fehler (HMOD_SEGM_GEN, 24, NO_INFO, NO_INFO, "",  NOT_YET_IMPLEMENTED);
   } /* andere Anwendung: noch nicht implementiert */
   if (ges_par_anz > 0)
   { /* Es sind ueberhaupt Parameter zu modifizieren */
      if (ges_par_anz == 1)
      { /* Der einzige Parameter wird auch modifiziert! */
         aend_par_anz   = 1;
         mod_aktpar [0] = TRUE;
      } /* Der einzige Parameter wird auch modifiziert! */
      else
      { /* Mehrere Parameter */
         aend_par_anz = irand (irand (ges_par_anz) + 1) + 1;
         if (aend_par_anz <= (ges_par_anz / 2))
         { /* Auswahl durch Markieren der zu aendernden Parameter */
            for (ii = 0;  ii < ges_par_anz;  ii++)    /* Init: Keinen aendern */
               mod_aktpar [ii] = FALSE;
            for (ii = 0;  ii < aend_par_anz;  ii++)
            { /* Markierung der zu aendernden Parameter */
               do                         
                  index = irand (ges_par_anz);
               while (mod_aktpar [index]);    /* Suche unmarkierten Parameter */   
               mod_aktpar [index] = TRUE;                 /* und markiere ihn */
            } /* Markierung der zu aendernden Parameter */
         } /* Auswahl durch Markieren der zu aendernden Parameter */
         else
         { /* Auswahl durch Loeschen d.Markierung nicht zu aendernder Params */
            for (ii = 0;  ii < ges_par_anz;  ii++)      /* Init: Alle aendern */
               mod_aktpar [ii] = TRUE;
            for (ii = aend_par_anz;  ii < ges_par_anz;  ii++)
            { /* Loeschung der Markierung nicht zu aendernder Parameter */
               do                         
                  index = irand (ges_par_anz);
               while (!mod_aktpar [index]);     /* Suche markierten Parameter */
               mod_aktpar [index] = FALSE;              /* und demarkiere ihn */
            } /* Loeschung der Markierung nicht zu aendernder Parameter */
         } /* Auswahl durch Loeschen d.Markierung nicht zu aendernder Params */
      } /* Mehrere Parameter */
   } /* Es sind ueberhaupt Parameter zu modifizieren */
   return (aend_par_anz);
} /* calc_par_anz */



/* ============================== modify_param ============================== */
BOOL modify_param (CHAIN_ELEM_PTR_TYPE  item_ptr,
                   INT                  mod_kind,
                   GSTATE              *ret_state)
{
   AKTPAR_MARK_TYP  param_aend;   
   DOUBLE           old_rwert,    new_rwert;
   INT              aend_par_anz, ipar_anz, 
                    done_anz,     index,
                    old_iwert,    new_iwert,
                    ii;
   SHORT            act;
   BOOL             altered = FALSE;
  
   /* modify_param */
#ifdef CHECKS
   if (item_ptr == NULL)
   {
      *ret_state |= fatal (HMOD_SEGM_GEN, 8, NO_INFO, NO_INFO, "", NIL_PTR);
      return (FALSE);
   }
#endif
   act = item_ptr->u_el.activity;
   if ((ACTIVITY_ACT < act) && (act <= last_act)  && (*ret_state == GLEAM_OK))
   { /* gueltige Aktion */
      aend_par_anz = calc_par_anz (act, param_aend);
      if (aend_par_anz > 0)
      { /* Es gibt aenderbare Parameter */
         if (std_ak_appl || std_segm_gen_appl)
         { /* LESAK, GADO, MatLab: Aktion (AK-Element) */
            ipar_anz = act_descr[act]->aend_ipar_anz;
            done_anz = 0;
            ii       = 0;
            do
            { /* Aendere alle zu aendernden Aktionsparameter */
               if (param_aend [ii])
               { /* Aendere diesen Parameter */
                  if (ii < ipar_anz)
                  { /* Integer-Parameter */
                     switch (mod_kind)
                     {
                        case CHANGE_NEW:
                        old_iwert = item_ptr->u_el.u_xel.u_act.i_params[ii];
                        new_iwert = calc_i_param (act, ii);
                        if (old_iwert != new_iwert)
                        {
                           altered = TRUE;
                           item_ptr->u_el.u_xel.u_act.i_params[ii] = new_iwert;
                        }
                        break; /* CHANGE_NEW */

                        case CHANGE_REL:
                        altered = change_i_param (&(item_ptr->u_el.u_xel.u_act.
                                                     i_params[ii]), act, ii);
                        break; /* CHANGE_REL */

                        case CHANGE_SMALL:
                        altered = small_i_change (&(item_ptr->u_el.u_xel.u_act.
                                                    i_params[ii]), act, ii);
                        break; /* CHANGE_SMALL */

                        default:
                        altered = FALSE;
                     } /* switch */
                  } /* Integer-Parameter */
                  else
                  { /* Real-Parameter */
                     index = ii - ipar_anz;
                     switch (mod_kind)
                     {
                        case CHANGE_NEW:
                        old_rwert = item_ptr->u_el.u_xel.u_act.r_params[index];
                        new_rwert = calc_r_param (act, index);
                        if (old_rwert != new_rwert)
                        {
                           altered = TRUE;
                           item_ptr->u_el.u_xel.u_act.r_params[index] = new_rwert;
                        }
                        break; /* CHANGE_NEW */

                        case CHANGE_REL:
                        altered = change_r_param (&(item_ptr->u_el.u_xel.u_act.
                                                    r_params[index]), 
                                                  act, index);
                        break; /* CHANGE_REL */

                        case CHANGE_SMALL:
                        altered = small_r_change (&(item_ptr->u_el.u_xel.u_act.
                                                    r_params[index]),act,index);
                        break; /* CHANGE_SMALL */

                        default:
                        altered = FALSE;
                     } /* switch */
                  } /* Real-Parameter */
                  done_anz++;
               } /* Aendere diesen Parameter */
               ii++;
            } /* Aendere alle zu aendernden Aktionsparameter */
            while (done_anz < aend_par_anz);
         } /* LESAK, GADO, MatLab: Aktion (AK-Element) */
         else
         { /* andere Anwendung: noch nicht implementiert */
            *ret_state = fehler (HMOD_SEGM_GEN, 9, NO_INFO, NO_INFO, "", 
                                 NOT_YET_IMPLEMENTED);
         } /* andere Anwendung: noch nicht implementiert */
      } /* Es gibt aenderbare Parameter */
   } /* gueltige Aktion */
   else 
      if (act != ACTIVITY_ACT)
         *ret_state = fatal (HMOD_SEGM_GEN, 10, act, NO_INFO, "", BAD_ITEM);
   return (altered);
} /* modify_param */



/* ============================= change_i_param ============================= */
static BOOL change_i_param (INT *wert,
                            INT  act,
                            INT  par_ind)
/*----------------------------------------------------------------------------*/
/*  Ausgehend vom aktuellen "wert" wird je nachdem, ob zufallsbedingt ver-    */
/*  groessert oder verkleinert werden soll, mit Hilfe der zu "act" und        */
/*  "par_ind" gehoerigen Eintraegen in "act_descr" das Intervall [wert, og]   */
/*  bzw [ug, wert] bestimmt. Die Wertaenderung findet dann in diesem Inter-   */
/*  vall statt und der neue Integer-Wert wird in "wert" abgeliefert.          */
/*  Wenn die Ober- und Untergrenzen des Parameters identisch sind, bleibt er  */
/*  unveraendert. Wenn nur Werte von 0 und 1 zugelassen sind, werden diese    */
/*  ohne Zufall direkt geaendert.                                             */
/*  Die Routine liefert TRUE, wenn eine Wertaenderung stattgefunden hat.      */
/*----------------------------------------------------------------------------*/
{
   I_PAR_DESCR_TYPE *par_descr_ptr;
   DOUBLE            klassen_wert;
   INT               klasse;
   INT               delta, vz;
   BOOL              kleiner, erg;

   /* change_i_param */
   par_descr_ptr = &(act_descr[act]->i_par_descr[par_ind]);
   if (par_descr_ptr->par_bereich > 1)
   { /* Parameter zufaellig aendern */
      if (par_descr_ptr->par_u_grenze >= *wert)
         kleiner = FALSE;
      else
         if (par_descr_ptr->par_o_grenze <= *wert)
            kleiner = TRUE;
         else
            kleiner = (irand(2) == 1);
      if (kleiner)
      { /* verkleinern */
         klassen_wert = *wert - par_descr_ptr->par_u_grenze;
         vz           = -1;
      } /* verkleinern */
      else
      { /* vergroessern */
         klassen_wert = par_descr_ptr->par_o_grenze - *wert;
         vz           = 1;
      } /* vergroessern */
      klassen_wert = klassen_wert / 10.0;

      klasse = irand(10) + 1;
      delta  = irand(i_round(klasse * klassen_wert)) + 1;
      *wert += delta * vz;
      erg = delta > 0;
   } /* Parameter zufaellig aendern */
   else
      if (par_descr_ptr->par_bereich == 0)
         erg = FALSE;                                    /* Parameter fixiert */
      else
      { /* Parameter mit boolschem Wertebereich */
         erg = TRUE;
	 if (*wert == 1)
	    *wert = 0;
	 else
	    *wert = 1;
      } /* Parameter mit boolschem Wertebereich */
   return (erg);
} /* change_i_param */
  


/* ============================= small_i_change ============================= */
static BOOL small_i_change (INT *wert,
                            INT  act,
                            INT  par_ind)
/*----------------------------------------------------------------------------*/
/*  Fuehrt kleine Wertaenderungen im Bereich "small_delta" des durch "act"    */
/*  und "par_ind" bezeichenten Parameters im Aktionsdeskriptors durch, wobei  */
/*  die Intervallgrenzen des Paramters eingehalten werden. Der neue Integer-  */ 
/*  Wert wird in "wert" abgeliefert.                                          */
/*  Wenn die Ober- und Untergrenzen des Parameters identisch sind, bleibt er  */
/*  unveraendert. Die Routine liefert TRUE, wenn eine Wertaenderung stattge-  */
/*  funden hat.                                                               */
/*----------------------------------------------------------------------------*/
{
   I_PAR_DESCR_TYPE *par_descr_ptr;
   INT               klasse;
   INT               delta, nwert;
   BOOL              erg;

   /* small_i_change */
   par_descr_ptr = &(act_descr[act]->i_par_descr[par_ind]);
   if (par_descr_ptr->small_delta == 0.0)
      return (FALSE);       /* Parameter fixiert oder Werteintervall zu klein */
   else
   { /* Parameter aenderbar */
      klasse = irand (10) + 1;
      delta = irand(i_round(klasse * par_descr_ptr->small_delta)) + 1;
      if (irand(2) == 1)
         nwert = *wert - delta;
      else
         nwert = *wert + delta;
      if (nwert < par_descr_ptr->par_u_grenze)
         nwert = par_descr_ptr->par_u_grenze;
      if (nwert > par_descr_ptr->par_o_grenze)
         nwert = par_descr_ptr->par_o_grenze;
      erg   = *wert != nwert;
      *wert = nwert;
      return (erg);
   } /* Parameter aenderbar */
} /* small_i_change */
  


/* ============================= change_r_param ============================= */
static BOOL change_r_param (DOUBLE *wert,
                            SHORT   act,
                            INT     par_ind)
/*----------------------------------------------------------------------------*/
/*  Ausgehend vom aktuellen "wert" wird je nachdem, ob zufallsbedingt ver-    */
/*  groessert oder verkleinert werden soll, mit Hilfe der zu "act" und        */
/*  "par_nr" gehoerigen Eintraegen in "act_descr" das Intervall [wert, og]    */
/*  bzw [ug, wert] bestimmt. Die Wertaenderung findet dann in diesem Inter-   */
/*  vall statt und der neue Real-Wert wird in "wert" abgeliefert. Wenn die    */
/*  Ober- und Untergrenzen des Parameters identisch sind, bleibt er unveraen- */
/*  dert.Die Routine liefert TRUE, wenn eine Wertaenderung stattgefunden hat. */
/*  Arbeitet im Kontext der Anwendungen LESAK, GADO, MatLab.                  */
/*----------------------------------------------------------------------------*/
{
   R_PAR_DESCR_TYPE *par_descr_ptr;
   DOUBLE            klassen_wert, delta, vz;
   INT               klasse;
   BOOL              kleiner;

   /* change_r_param */
   par_descr_ptr = &(act_descr[act]->r_par_descr[par_ind]);
   if (par_descr_ptr->par_bereich <= min_delta)
      return (FALSE);                                    /* Parameter fixiert */
   else
   { /* Parameter aenderbar */
      if ((*wert - par_descr_ptr->par_u_grenze) <= min_delta)
         kleiner = FALSE;
      else
         if ((par_descr_ptr->par_o_grenze - *wert) <= min_delta)
            kleiner = TRUE;
         else
            kleiner = (irand(2) == 1);
      if (kleiner)
      { /* verkleinern */
         klassen_wert = *wert - par_descr_ptr->par_u_grenze;
         vz           = -1.0;
      } /* verkleinern */
      else
      { /* vergroessern */
         klassen_wert = par_descr_ptr->par_o_grenze - *wert;
         vz           = 1.0;
      } /* vergroessern */
      klassen_wert = klassen_wert / 10.0;
      do
      { /* "delta" bestimmen, bis es ungleich Null ist */
         klasse = irand (10) + 1;
         delta  = frand() * klasse * klassen_wert;
      } /* "delta" bestimmen, bis es ungleich Null ist */
      while (delta == 0.0);
      *wert += delta * vz;
      return (TRUE);
   } /* Parameter aenderbar */
} /* change_r_param */
  


/* ============================= small_r_change ============================= */
static BOOL small_r_change (DOUBLE *wert,
                            SHORT   act,
                            INT     par_ind)
/*----------------------------------------------------------------------------*/
/*  Fuehrt kleine Wertaenderungen im Bereich "small_delta" des durch "act"    */
/*  und "par_ind" bezeichenten Parameters im Aktionsdeskriptors durch, wobei  */
/*  die Intervallgrenzen des Paramters eingehalten werden. Der neue Integer-  */ 
/*  Wert wird in "wert" abgeliefert.                                          */
/*  Wenn die Ober- und Untergrenzen des Parameters identisch sind, bleibt er  */
/*  unveraendert. Die Routine liefert TRUE, wenn eine Wertaenderung stattge-  */
/*  funden hat.                                                               */
/*----------------------------------------------------------------------------*/
{
   R_PAR_DESCR_TYPE *par_descr_ptr;
   INT               klasse;
   DOUBLE            delta, nwert;
   BOOL              erg;

   /* small_r_change */
   par_descr_ptr = &(act_descr[act]->r_par_descr[par_ind]);
   if (par_descr_ptr->small_delta <= min_delta)
      return (FALSE);       /* Parameter fixiert oder Werteintervall zu klein */
   else
   { /* Parameter aenderbar */
      klasse = irand (10) + 1;
      delta  = frand() * klasse * par_descr_ptr->small_delta;
      if (irand(2) == 1) { /* verkleinern */
         nwert = *wert - delta;
	 if (nwert < par_descr_ptr->par_u_grenze)
	    nwert = par_descr_ptr->par_u_grenze;
      } /* verkleinern */
      else { /* vergroessern */
         nwert = *wert + delta;
	 if (nwert > par_descr_ptr->par_o_grenze)
	    nwert = par_descr_ptr->par_o_grenze;
      } /* vergroessern */
      erg   = *wert != nwert;
      *wert = nwert;
      return (erg);
   } /* Parameter aenderbar */
} /* small_r_change */
  


/* ============================== repair_2_aks ============================== */
GSTATE repair_2_aks (CHAIN_ELEM_PTR_TYPE  ak1_ptr,
                     CHAIN_ELEM_PTR_TYPE  ak2_ptr)
{
   CHAIN_ELEM_TYPE  scratch_akt;
   INT              ak1_doppel_akt_anz, t1_i,
                    ak2_doppel_akt_anz, t2_i;
   SHORT            scratch_act;
  
   /* repair_2_aks */
#ifdef CHECKS
   if (ak1_ptr == NULL)
      return (fatal (HMOD_SEGM_GEN, 11, NO_INFO, NO_INFO, "", NIL_PTR));
   if (ak2_ptr == NULL)
      return (fatal (HMOD_SEGM_GEN, 12, NO_INFO, NO_INFO, "", NIL_PTR));
   if (ak1_ptr->u_el.activity != AK_HEADER)
      return (fatal (HMOD_SEGM_GEN, 13, ak1_ptr->u_el.activity, NO_INFO, "",
                     BAD_CHAIN_KIND));
   if (ak2_ptr->u_el.activity != AK_HEADER)
     return  (fatal (HMOD_SEGM_GEN, 14, ak2_ptr->u_el.activity, NO_INFO, "", 
                     BAD_CHAIN_KIND));
#endif
   if (FIX_ACT_RAND_GEN < gen_akt_mode )               /* d.h. FIX_ACT_?_GEN, */
      return (fatal (HMOD_SEGM_GEN, 15, appl_code, NO_INFO, "",
                     INCONSISTANT_GENCODE));           /* derzeit LESAK       */
   analyze_ak (ak1_tab, &ak1_doppel_akt_anz, ak1_ptr); /* Doppelte Aktionen in*/
   analyze_ak (ak2_tab, &ak2_doppel_akt_anz, ak2_ptr); /* beiden Ketten suchen*/
   t1_i = 0;
   t2_i = 0;
   while ((ak1_doppel_akt_anz > 0) && (ak2_doppel_akt_anz > 0))
   { /* Ein Paar doppelte Aktionen inhaltlich austauschen */
      while (ak1_tab[t1_i].akt_anz != 2)
         t1_i++;
      while (ak2_tab[t2_i].akt_anz != 2)
         t2_i++;
      ak1_tab[t1_i].akt_anz = 1;
      ak2_tab[t2_i].akt_anz = 1;
      /* Inhalte der Aktionen austauschen: */
      scratch_akt.u_el.u_xel.u_act = ak1_tab[t1_i].akt_ptr->u_el.u_xel.u_act;
      scratch_act                  = ak1_tab[t1_i].akt_ptr->u_el.activity;
      ak1_tab[t1_i].akt_ptr->u_el.u_xel.u_act = ak2_tab[t2_i].akt_ptr->
                                                u_el.u_xel.u_act;
      ak1_tab[t1_i].akt_ptr->u_el.activity    = ak2_tab[t2_i].akt_ptr->u_el.
                                                activity;
      ak2_tab[t2_i].akt_ptr->u_el.u_xel.u_act = scratch_akt.u_el.u_xel.u_act;
      ak2_tab[t2_i].akt_ptr->u_el.activity    = scratch_act;
      ak1_doppel_akt_anz--;
      ak2_doppel_akt_anz--;
   } /* Ein Paar doppelte Aktionen inhaltlich austauschen */
   /* Bei evtl. verbliebenen Paaren einen Partner an die andere Ak haengen: */
   if (ak1_doppel_akt_anz > 0)
      return (phase2 (ak1_tab, ak1_doppel_akt_anz, ak1_ptr, ak2_ptr));
   if (ak2_doppel_akt_anz > 0)
      return (phase2 (ak2_tab, ak2_doppel_akt_anz, ak2_ptr, ak1_ptr));
   return (GLEAM_OK);
} /* repair_2_aks */ 



/* =============================== analyze_ak =============================== */
static VOID analyze_ak (TAB_ELEM_TYPE       *akt_tab,
                        INT                 *akt_d_anz,
                        CHAIN_ELEM_PTR_TYPE  ak_ptr)
/*----------------------------------------------------------------------------*/
/*  Analysiert eine AK auf doppelte Aktionen. Diese werden in der "akt_tab"   */
/*  vermerkt und in "akt_d_anz" mitgezaehlt.                                  */
/*----------------------------------------------------------------------------*/
{
   CHAIN_ELEM_PTR_TYPE  el_ptr;
   INT                  ii;
  
   /* analyze_ak */
   for (ii = 0;  ii < last_act + 2;  ii++)
      akt_tab[ii].akt_anz = 0;
   *akt_d_anz = 0;
   el_ptr     = ak_ptr->u_el.next_action;
   while (el_ptr != NULL)
   { /* eine AK untersuchen */
      ii = el_ptr->u_el.activity;
      (akt_tab[ii].akt_anz)++;
      if (akt_tab[ii].akt_anz == 2)
      { /* Doppelte Aktion gefunden */
         (*akt_d_anz)++;
         akt_tab[ii].akt_ptr = el_ptr;
      } /* Doppelte Aktion gefunden */
      el_ptr = el_ptr->u_el.next_action;
   } /* eine AK untersuchen */
} /* analyze_ak */ 



/* ================================= phase2 ================================= */
static GSTATE phase2 (TAB_ELEM_TYPE       *akt_tab,
                      INT                  akt_d_anz,
                      CHAIN_ELEM_PTR_TYPE  ak_ptr,
                      CHAIN_ELEM_PTR_TYPE  akx_ptr)
/*----------------------------------------------------------------------------*/
/*  Sucht in der Tabelle "akt_tab" alle verbliebenen Doppel-Aktionen und      */
/*  haengt sie an die andere AK ("akx_ptr") dran. Das letzte Segment der      */
/*  anderen AK wird mit "segmentiere_ch" nachsegmentiert. Auftretende Fehler  */
/*  werden als Funktionsergebnis abgeliefert.                                 */
/*----------------------------------------------------------------------------*/
{
   CHAIN_ELEM_PTR_TYPE  el_ptr, pred_ptr;
   INT                  tab_i, ii;
   GSTATE               ret_state = GLEAM_OK;
  
   /* phase2 */
   tab_i = 1;
   for (ii = 1;  (ii <= akt_d_anz) && (ret_state == GLEAM_OK);  ii++)
   { /* eine doppelte Aktion umhaengen */
      while (akt_tab[tab_i].akt_anz != 2)
         tab_i++;
      ret_state  = find_predecessor (&pred_ptr, ak_ptr, akt_tab[tab_i].akt_ptr);
      ret_state |= remove_next_item (&el_ptr, ak_ptr, pred_ptr);
      ret_state |= append_item (akx_ptr, el_ptr);
      tab_i++;
   } /* eine doppelte Aktion umhaengen */
   ret_state |= segmentiere_ch (akx_ptr, FALSE); /* FALSE = Nachsegmentierung */
   return (ret_state);
} /* phase2 */ 



/* ============================= repair_1_chain ============================= */
BOOL repair_1_chain (CHAIN_ELEM_PTR_TYPE *ch_ptr,
                     GSTATE              *ret_state)
{
   BOOL  rep_result = FALSE;
  
   /*repair_1_chain */
#ifdef CHECKS
   if (*ch_ptr == NULL)
      *ret_state |= fatal (HMOD_SEGM_GEN, 16, NO_INFO, NO_INFO, "", NIL_PTR);
#endif
   if (*ret_state == GLEAM_OK)
   { /* Status ok */
      if ((*ch_ptr)->u_el.u_xel.u_hdr.chain_length == 0)
      { /* Leere Kette */
         rep_result = FALSE;
         *ret_state = delete_chain (ch_ptr);
      } /* Leere Kette */
      else
      { /* Pruefe nichtleere Kette */
         switch (appl_code)
         {
            case GADO_APPL:
            rep_result = TRUE;
            /* to be implemented ... */
            break; /* GADO_APPL */

            case SIMU_SERV_APPL:
            case MATLAB_APPL:
            rep_result = TRUE;
            break; /* SIMU_SERV_APPL or MATLAB_APPL */

            case LESAK_APPL:
            rep_result = lesak_p_test (ch_ptr, ret_state);
            break; /* LESAK_APPL */

            case CEC_MATHFKT_APPL:
            case MATH_FKT_APPL:
            rep_result = TRUE;
            break; /* MATH_FKT_APPL */

            case ADD_APPL:
            rep_result = add_appl_repair_1ch (ch_ptr, ret_state);
            break; /* LESAK_APPL */

            default:
            *ret_state = fatal (HMOD_SEGM_GEN, 17, appl_code, NO_INFO, "", 
                                BAD_APPL_CODE);
         } /* CASE */
      } /* Pruefe nichtleere Kette */
   } /* Status ok */
   return (rep_result);
} /* repair_1_chain */



/* ============================= segmentiere_ch ============================= */
GSTATE segmentiere_ch (CHAIN_ELEM_PTR_TYPE  ch_ptr,
                       BOOL                 komplett)
{
   REF_DESCR_PTR_TYPE   new_sd_ptr, segm_d_ptr, next_ref_d_ptr, ref_d_ptr;
   CHAIN_ELEM_PTR_TYPE  elem_ptr, dummy_ptr;
   INT                  ch_len, segm_index, segm_len, segm_anz, elem_ind;
   GSTATE               ret_state = GLEAM_OK;
   BOOL                 do_it;
  
   /* segmentiere_ak */
#ifdef CHECKS
   if (ch_ptr == NULL)
      return (fatal (HMOD_SEGM_GEN, 18, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   if (ch_ptr->u_el.activity != AK_HEADER)
      return (fatal (HMOD_SEGM_GEN, 19, ch_ptr->u_el.activity, NO_INFO, "",  
                         BAD_CHAIN_KIND));
   ch_len = ch_ptr->u_el.u_xel.u_hdr.chain_length;
   do_it  = TRUE;
   if (komplett)
   { /* Vorbereitung der Komplett-Segmentierung */
      segm_anz   = 0;
      elem_ind   = 0;
      segm_index = 1;
      elem_ptr   = ch_ptr;
      if ((ret_state = new_ref_descr (&segm_d_ptr)) == GLEAM_OK)
      { /* Status ok */
         segm_d_ptr->segm_ptrs[0] = ch_ptr;
         ch_ptr->u_el.u_xel.u_hdr.segm_descr_ptr = segm_d_ptr;
      } /* Status ok */
      else
         do_it = FALSE;
   } /* Vorbereitung der Komplett-Segmentierung */
   else
   { /* Vorbereitung der Neu-Segmentierung des letzten Segments */
      segm_anz = ch_ptr->u_el.u_xel.u_hdr.segment_anz;
      if (segm_anz == 0)
      { /* Ehemals leere AK mit einem leeren Segment-Descriptor */
         segm_d_ptr = ch_ptr->u_el.u_xel.u_hdr.segm_descr_ptr;
         if (segm_d_ptr == NULL)
            return (fatal (HMOD_SEGM_GEN, 20, NO_INFO, NO_INFO, "", 
                           BAD_ABSCHNITT));
         segm_d_ptr->segm_ptrs[1]   = ch_ptr->u_el.u_xel.u_hdr.last_action;
         segm_d_ptr->segm_length[1] = ch_len;               /* Die Elemente   */
         elem_ptr   = ch_ptr;                               /* der AK werden  */
         segm_len   = ch_len;                               /* zu einem neuen */
         segm_index = 1;                                    /* Segment zu-    */
         segm_anz   = 1;                                    /* sammengefasst  */
      } /* Ehemals leere AK mit einem leeren Segment-Descriptor */
      else
         if (!ermittle_abschnitt (&elem_ptr, &dummy_ptr, &segm_len, &segm_index,
                                  &segm_d_ptr, ch_ptr, segm_anz, &ret_state))
            return (fatal (HMOD_SEGM_GEN, 21, segm_anz,  NO_INFO, "", 
                           SEGM_NOT_FOUND));
      if (segm_len > ak_max_segm_len)
      { /* Letztes Segment ist zu lang */
         if (segm_anz > 1)
         { /* Letztes von mehr als einem Segment ist zu lang */
            segm_anz--;
            elem_ind = ch_len - segm_len;
         } /* Letztes von mehr als einem Segment ist zu lang */
         else
         { /* das einzige Segment ist zu lang */
            segm_anz   = 0;
            elem_ind   = 0;
            segm_index = 1;
            elem_ptr   = ch_ptr;
         } /* das einzige Segment ist zu lang */
         ref_d_ptr = segm_d_ptr->next_ref_descr_ptr;            /* Loesche    */
         while (ref_d_ptr != NULL)                              /*            */
         { /* Segmentdescriptoren freigeben */                  /* eventuelle */
            next_ref_d_ptr = ref_d_ptr->next_ref_descr_ptr;     /*            */
            free_ref_descr (ref_d_ptr);                         /* Folge-     */
            ref_d_ptr      = next_ref_d_ptr;                    /* SegmDescr- */
         } /* Segmentdescriptoren freigeben */                  /*            */
         segm_d_ptr->next_ref_descr_ptr = NULL;                 /* Records    */
      } /* Letztes Segment ist zu lang */
      else
         do_it = FALSE;
   } /* Vorbereitung der Neu-Segmentierung des letzten Segments */

   if (do_it)
   { /* Segmentierung */
      do
      {
         segm_len = calc_segm_len (ch_len - elem_ind);
         count_out_item (&elem_ptr, elem_ptr, segm_len, &ret_state);
         if (segm_index == SEGM_PTR_ANZ)
         { /* Ein neues Segmentdescriptor-Item */
            segm_index = 0;
            if ((ret_state |= new_ref_descr (&new_sd_ptr)) != GLEAM_OK)
               return (ret_state);
            segm_d_ptr->next_ref_descr_ptr = new_sd_ptr;
            segm_d_ptr = new_sd_ptr;
         } /* Ein neues Segmentdescriptor-Item */
         segm_d_ptr->segm_ptrs[segm_index]   = elem_ptr;
         segm_d_ptr->segm_length[segm_index] = segm_len;
         elem_ind += segm_len;
         segm_anz++;
         segm_index++;
      }
      while ((ch_len != elem_ind) && (ret_state == GLEAM_OK));
      ch_ptr->u_el.u_xel.u_hdr.segment_anz = segm_anz;
   } /* Segmentierung */
   return (ret_state);
} /* segmentiere_ch */ 



/* ============================= calc_max_abst ============================== */
DOUBLE calc_max_abst (INT  len)
{
   if ((len % 2) == 0)
      return (((DOUBLE)len * (DOUBLE)len) / 2.0);       /* "len" ist gerade   */
   if (len == 1)
      return (1.0);           /* Grenzfall. Durch das Ergebnis wird dividiert */
   return ((((DOUBLE)len * (DOUBLE)len) - 1.0) / 2.0);  /* "len" ist ungerade */
} /* calc_max_abst */



/* ============================== reset_seq_gen ============================= */
void reset_seq_gen (void)
{
   INT  ii;

   for (ii = 0;  ii <= last_act;  ii++)
      akt_flags[ii] = FALSE;
} /* reset_seq_gen */



/* ============================== kettenlaenge ============================== */
INT kettenlaenge (GSTATE *ret_state)
{
   INT  r_kettenlaenge;
  
   /* kettenlaenge */
   if (*ret_state == GLEAM_OK)
   { /* alle paletti */
      if (std_ak_appl)    
      { /* LESAK, GADO, MatLab */
         if (gen_len_mode == FIX_LEN)
            r_kettenlaenge = min_ketten_len;
         else
	 { /* AK mit variabler Laenge */
            if (gen_len_mode == RAND_LEN)
               r_kettenlaenge = min_ketten_len +    /* Kettenlänge auswürfeln */
                                irand (max_ketten_len - min_ketten_len + 1);
            else
               *ret_state = fatal (HMOD_SEGM_GEN, 22, gen_len_mode, NO_INFO, 
                                   "", BAD_GEN_KIND);
	 } /* AK mit variabler Laenge */
      } /* LESAK, GADO, MatLab */
      else
      { /* andere Anwendung: noch nicht implementiert */
        *ret_state = fehler (HMOD_SEGM_GEN, 23, NO_INFO, NO_INFO, "", 
                             NOT_YET_IMPLEMENTED);
      } /* andere Anwendung: noch nicht implementiert */
   } /* alle paletti */
   return (r_kettenlaenge);
} /* kettenlaenge */



/* ================================ gen_chain_tsp =============================== */
BOOL gen_chain_tsp (CHAIN_ELEM_PTR_TYPE *chain_ptr,
                    GSTATE              *ret_state)
{
   BOOL  r_gen_chain;
   INT   kum_len, abschn_len, len;
  
   /* gen_chain_tsp */
   *chain_ptr = NULL;
   if (std_ak_appl)
   { /* LESAK, GADO, MatLab oder Standard-Generierung bei Nicht-Std-Appl */
#ifdef MTEST
      lies_wert (&len, "GEN: Kettenlänge: ", 0, K32, K32+1);
#else
      len = kettenlaenge (ret_state);
#ifdef ZTEST
      sprintf (buf1, "GEN: len=%1u", len);
      printf ("%s\n", buf1);
#endif
#endif /* MTEST */
      r_gen_chain = enough_free_mem (AK_HEADER, len,     
                                     (INT)(len / min_abschn_len) + 1);
      if (r_gen_chain && (*ret_state == GLEAM_OK))
      { /* enough memory und uebergebener Status ist ok */
         if (gen_akt_mode <= FIX_ACT_RAND_GEN)
         { /*Aktionsausw.u.-sequenzierung: FIX_ACT_RAND_GEN od.FIX_ACT_FIX_GEN*/
            act_ctr = ACTIVITY_ACT;
            if (gen_akt_mode == FIX_ACT_RAND_GEN)
               reset_seq_gen ();
         } /*Aktionsausw.u.-sequenzierung: FIX_ACT_RAND_GEN od.FIX_ACT_FIX_GEN*/
         gc_abschn_anz = 0;
         gc_sd_index   = 1;
         *ret_state    = init_chain (chain_ptr, AK_HEADER);
         gc_chain_ptr  = *chain_ptr;
         if ((len > 0) && (*ret_state == GLEAM_OK))
         { /* Keine leere Kette und Status ok */
            if ((*ret_state = new_ref_descr (&gc_sd_ptr)) == GLEAM_OK)
            { /* 1.SegmDescr erhalten */
               gc_sd_ptr->segm_ptrs[0] = gc_chain_ptr;
               gc_chain_ptr->u_el.u_xel.u_hdr.segm_descr_ptr = gc_sd_ptr;
               if (len > min_abschn_len) 
               { /* Kettenlaenge > min_abschn_len: 1 oder mehrere Abschnitte */
                  abschn_len = calc_segm_len (len);
#ifdef TEST
                  sprintf (buf1, "len\'s: %1u", abschn_len);
#endif
                  kum_len = abschn_len;
                  *ret_state = first_segm (abschn_len);       /* 1. Abschnitt */
                  while ((kum_len < len) && (*ret_state == GLEAM_OK))
                  { /* Alle weiteren Abschnitte */
                     abschn_len = calc_segm_len (len - kum_len);
#ifdef TEST
                     sprintf (buf2, " %1u", abschn_len);
                     strcat (buf1, buf2);
#endif
                     kum_len += abschn_len;
                     *ret_state = next_segm (abschn_len);     /* n. Abschnitt */
                  } /* Alle weiteren Abschnitte */
#ifdef TEST
                  printf ("%s\n", buf1);
                  sprintf (buf1, "kum_len=%1u", kum_len);
                  printf ("%s\n", buf1);
#endif
               } /* Kettenlaenge > min_abschn_len: 1 oder mehrere Abschnitte */
               else 
                  *ret_state = first_segm (len);               /* 1 Abschnitt */
               gc_chain_ptr->u_el.u_xel.u_hdr.chain_length = len;
               gc_chain_ptr->u_el.u_xel.u_hdr.segment_anz  = gc_abschn_anz;
            } /* 1.SegmDescr erhalten */
         } /* Keine leere Kette und Status ok */
      } /* enough memory und uebergebener Status ist ok */
   } /* LESAK, GADO, MatLab oder Standard-Generierung bei Nicht-Std-Appl */
   else
      r_gen_chain = add_anw_ch_gen (chain_ptr, ret_state);    /* spez. AK-Gen */

   return (r_gen_chain && (*ret_state == GLEAM_OK));
} /* gen_chain_tsp */
