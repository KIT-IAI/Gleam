/*******************************************************************************
GLEAM/AE                                                                  V1.2.2

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: EVOlution
                        Module: Genetische Operatoren GrundSoftWare
 
Package: evo                    File: go_gsw.c
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Der Modul enthaelt die Daten und Routinen zur Bestimmung von Wahrscheinlich-
keitsverteilungen und zum Lesen der EVO-Intialisierungsdatei. Ausserdem enthaelt
er die Package-Initialisierung.

Die Ausfuehrung genetischer Operatoren wird durch 2 Tabellen gesteuert. Die eine
regelt die Anzahl der Anwendungen eines Mutations-Operators auf eine AK ("mut_
data") waehrend die andere ("go_list") die Wahrscheinlichkeit der Ausfuehrung 
eines oder einer Liste genetischer Operatoren steuert, wobei im Falle einer 
Liste jeder Operator eine eigene Ausfuehrungswahrscheinlichkeit hat. Zu den ge-
nerellen Wahrscheinlichkeiten gibt es einen unteren Schwellwert fuer einen spae-
teren Ausbau in Richtung Adaptivitaet.

Die Datenstruktur "mut_data" hat folgenden Aufbau:
     Es gibt pro Mutations-Operator 4 Werte:
       - Algorithmusnummer (0..3)
       - Parameter a, b, c
 
     Die Algorithmen der Anzahl (anz) fuer das Auftreten einer Mutation sind
     (x=Kettenlaenge):

          0. (lin1) : bereich = round (x/a + b);    Min (bereich) = c
                      if (x < a2) 
                         anz = 0; 
                      else 
                         anz = irand (bereich) + 1;

          1. (lin2) : bereich = round (x/a + b);    Max (bereich) = c
                      if (x < a2) 
                         anz = 0; 
                      else 
                         anz = irand (bereich) + 1;

          2. (lin3) : if (x < c) 
                         bereich = round (x/a  + b);
                         anz = irand (bereich) + 1;
                      else 
                         bereich = round (x/a2 + b2);
                         if (bereich < 2) 
                            anz = 1;
                         else
                            anz = irand (bereich) + 1;

          3. (quad):  bereich = round (a*SQR(x) + b*x + c);
                      if (bereich < 2) 
                         anz = 1;
                      else
                         anz = irand (bereich) + 1;

"lin1" und "lin2" sind fuer proportionale Funktionen vorgesehen, waehrend bei
"lin3" die 2.Funktion auch umgekehrt proportional sein darft. Der Bereich 
"bereich" wird bei allen Formeln auf den Wert "x" beschraenkt, so dass "anz" 
nie groesser als "x", also die Kettenlaenge, werden kann. Das Ergbnis "anz"
ist immer Null, wenn x=0 ist.


Mit der Datenstruktur "go_list" soll eine flexible Steuerung folgender Schemata 
der Anwendung gen. Operatoren erreicht werden: 
  Eine genetische Operation ist pro Paarung definiert als die Anwendung
   1. eines einzelnen Mutationsoperators auf ein Kind.
   2. mehrere Mutationsoperatoren auf ein Kind, wobei jeder Operator 
      wahrscheinlichkeitbehaftet sein kann.
   3. eines einzelnen XO-Operators zur Erzeugung von 2 Nachkommen.
   4. eines einzelnen XO-Operators zur Erzeugung von 2 Nachkommen, wobei auf ein
      Kind zusaetzlich ein oder mehrere jeweils wahrscheinlichkeitbehaftete 
      Mutationsoperatoren angewandt werden .
Pro Paarung koennen entsprechend der Anzahl der Eintraege in "go_list" mehrere 
solcher Operationen erfolgen.

Die Datenstruktur "go_list" hat folgenden Aufbau:
     Es gibt pro Eintrag folgende Werte:
       - p_akt    double    Wahrscheinlichkeit zur Ausfuehrung des/der gen. Ops.
       - p_min    double    Untere Grenze von "p_akt"
       - eins     BOOL      Operator(en) vom Typ Mutation, erzeugen 1 Kind
       - op_anz   int       Anzahl der Operator-Eintraege
       - op_list  int[]     Auf die gleiche Kopie anzuwendenden gen. Operatoren
       - p_list   double[]  Wahrscheinlichkeit fuer jeden gen. Op. der Liste

Die zugehoerige Interpretation pro Paarung lautet in Pseudo-Code, wobei hier der
Code fuer wahrscheinlichkeitsbedingte Ausfuehrungen der Uebersicht halber wegge-
lassen ist:
   FOR alle eintraege in "go_list"
      IF "eins"
         erzeuge "child1" durch kopieren des elter.
      END_IF
      FOR alle eintraege in "op_list"
         SWITCH op_liste[ii]
            <mut_op>: wende mut_operator auf "child1" an.
            <xo_op>:  wende xo_operator an und erzeuge "child1" und "child2"
                      IF (keine kinder erzeugt) && (op_anz > 1)
                         erzeuge "child1" durch kopieren des elter.
                      END_IF
         END_SWITCH
      END_FOR
      IF "eins"
         pruef und speichere 1 kind
      ELSE
         pruef und speichere 2 kinder
      END_IF
   END_FOR
   fuehre akzeptansalorithmus fuer das beste kind aus.


Obige Datenstrukturen gibt es vier mal: Die ersten 3 Saetze sind fuer die Evo-
lution bestimmt und gelten fuer drei verschiedene Notenbereiche. Der vierte ist 
fuer andere Optimierungsstrategien wie z.B. dem Sintflut-Algorithmus vorgesehen.
 
 
Die Routinen im einzelnen:

INT  calc_p_go (BOOL   *one_child,
                INT     go_index,
                DOUBLE  note);
          Berechnet ausgehend von der "note", welcher Parametersatz zu benutzen 
          ist und hinterlegt dies in den lokalen Variablen "par_satz_index" und 
          "go_elem_ptr" fuer die nachfolgenden Aufrufe von "calc_p_op" und 
          "mutations_zahl". Danach wird anhand des zu "go_index" gehoerigen 
          "p_akt"-Eintrags in der "go_list" bestimmt, ob die gen. Operatoren der
          "op_list" abgearbeitet werden oder nicht. Das Funktionsergebnis ist 0
          wenn nicht; ansonsten gibt es die Anzahl der Operatoreintraege in 
          "op_list" an. "one_child" enthaelt den Wert des "eins"-Eintrags.

BOOL calc_p_op (INT *gen_op_id,
                INT  op_index);
          Setzt die vorherige Ausfuehrung von "calc_p_go" voraus. Berechnet aus-
          gehend von dem zu "op_index" gehoerigen "p_list"-Eintrag, ob der zu-
          hoerige gen. Operator in der "op_list" ausgefuehrt werden soll oder 
          nicht. Das Ergebnis wird zusammen mit der Kennung des Operators aus
          der "op_list" in "gen_op_id" abgeliefert. 

INT  mutations_zahl (INT  mutation,
                     INT  len);
          Setzt die vorherige Ausfuehrung von "calc_p_go" voraus. Berechnet aus-
          gehend von dem zur "mutation" gehoerigen Parametersatz in "mut_data" 
          und der Kettenlaenge "len" wie oft der gen. Operator auf die AK ange-
          wandt werden soll. Das Ergebnis wird abgeliefert.

GSTATE loadEvoParamFile (char *fspec,
                         BOOL *bad_data);
          Initialisiert die die genetischen Operatoren steuernde Datenstruktur 
          "mut_data" aus dem File "fspec". Wenn dabei ein Fehler (I/O-Error, 
          unkompatible Daten) auftritt, erfolgt eine Fehlermeldung und es wird 
          GLEAM_ERROR abgeliefert, sonst GLEAM_OK. "bad_data" gibt bei einem 
          Fehler an, ob die internen Daten bereits veraendert und damit unguel-
          tig geworden sind. Bestimmt die maximal maegliche Anzahl an Nachkommen 
          pro Paarung und hinterlegt sie in "maxOffsprAmount".

BOOL evoInit (void);
          Aufbau der package-lokalen Datenstrukturen. Liefert FALSE, wenn dazu 
          der verfuegbare Speicher nicht ausreichen sollte.

INT  get_last_go_idx (void);
          Liefert "LAST_GO" ab.

void get_noten_limits (LONG *n_lim1,
                       LONG *n_lim2);
          Liefert in "n_lim1" "n_og1" und in "n_lim2" "n_og2" ab.

INT  get_mz_bereich (void);
          Liefert den beim letzten Aufruf von "mutations_zahl" berechneten Wert
          fuer "bereich" ab.

--------------------------------------------------------------------------------
basiert auf C-Unit: go_gsw
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. Neue Routinen "get_last_go_idx", "get_noten_limits" und "get_mz_bereich".
 2. Umgestaltung der Routine "mutations_zahl".
 3. Erhoehung der Maximallaenge der Liste gen.Operatoren (ANZ_GEN_OPS) von 8 
    auf 16.
 4. "mutations_zahl" liefert fuer len==0 auch immer 0.
 5. "buf1" bis "buf3" vergroessert.                                  WJ 31.07.00
 6. Die Vorgabe einer Wahrscheinlichkeit von 0 bewirkt in 
    "calc_p_go"und "calc_p_op", dass keine Ausfuehrung stattfindet.  WJ 08.03.05
 7. Linux-Portierung: Aus "round" wird "i_round".                    WJ 12.05.05
 8. Anpassung an neue Datentypen fuer Int- und Real-Parameter
    (neues "chaindef.h" fuer 64 bit)                                 WJ 03.01.08
 9. Entfernung eines Includes im Kontext von GLEAM_TCL. Entfernung 
    ueberfluessiger Variable.                                        WJ 28.06.12
10. Renovierung: Korrektur der fgets-Aufrufe.  Ersetzung von test-
    weisen "write_line"-Ausgaben durch "printf"-Aufrufe und Anpassung 
    der "lies_wert"-Aufrufe.                                         WJ 04.05.16
11. Aus MOD_FILE_IDENT wird PROG_IDENT.                              WJ 05.12.16
12. Umbenennung von "go_gsw_init" nach "loadEvoParamFile" und 
    Ermittlung der maximalen Offspring-Anzahl in "maxOffspAmount".  
    Reunion of "evoIntg.h" and "evoInts.h" in "evoInt.h".            WJ 30.06.17
13. Rremoval of the superfluous 4th parameter sets (ANZ_PAR_SAETZE). WJ 26.07.20
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 30.06.2017
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "lgsw.h"
#include "evo.h"
#include "evoInt.h"

#ifdef GLO_CHECKS
  #define CHECKS
#endif
#undef MTEST
#undef MZTEST



/* ======================== Package-globale Variable: ======================= */
INT   go_anz,            /* Anzahl der Eintraege gen.Operationen in "go_list" */
      maxOffsprAmount;   /* maximal number of offspring per mating            */
BOOL  do_evo;            /* TRUE, wenn Evo und keine andere OptStrategie      */
char *buf1,              /* Hilfspuffer                                       */
     *buf2,              /* Hilfspuffer                                       */
     *buf3;              /* Hilfspuffer                                       */



/* ============================ Lokale Konstante: =========================== */
#define ANZ_PAR_SAETZE  3    /* Anz. d."mut_data" u."go_list"-Parametersaetze */
#define ANZ_GEN_OPS    16    /* MaxAnz Gen.Ops. pro Nachkomme(n) einer Paarung*/
#define LIN1            0    /* Algorithmus-Codierung fuer "mut_alg"          */
#define LIN2            1    /* Algorithmus-Codierung fuer "mut_alg"          */
#define LIN3            2    /* Algorithmus-Codierung fuer "mut_alg"          */
#define QUAD            3    /* Algorithmus-Codierung fuer "mut_alg"          */


/* ======================== Lokale Typdeklarationen: ======================== */
typedef struct
  { 
    INT     mut_alg,        /* Algorithmus zur Bestimmung der Mutationsanzahl */
            rounded_c;      /* gerundeter Wert von "c"                        */
    DOUBLE  a, b, c,        /* Paramter des                                   */
            a2, b2;         /* Algorithmus                                    */
  } MUT_DATA_SET;

typedef MUT_DATA_SET  MUT_DATA_TYPE [LAST_GO+1];

typedef INT           OP_LIST_TYPE  [ANZ_GEN_OPS];
typedef DOUBLE        P_LIST_TYPE   [ANZ_GEN_OPS];

typedef struct
  { 
    DOUBLE        p_akt,    /* Wahrscheinlichkeit zur Ausfuehrung der gen.Ops.*/
                  p_min;    /* Untere Grenze von "p_akt"                      */
    BOOL          eins;     /* Operator vom Typ Mutation, erzeugt 1 Kind      */
    INT           op_anz;   /* Anzahl der Operator-Eintraege in den Feldern:  */
    OP_LIST_TYPE  op_list;  /* Auf die gleiche Kopie anzuwendende gen. Ops.   */
    P_LIST_TYPE   p_list;   /* Wahrscheinlichkeit f. jeden gen. Op. der Liste */
  } GO_ELEM_TYPE;

typedef GO_ELEM_TYPE  GO_LIST_TYPE [ANZ_PAAR_OPS];


/* ============================ Lokale Variable: ============================ */
static MUT_DATA_TYPE *mut_data;       /* Fuer Mutationsanzahl pro AK          */
static GO_LIST_TYPE  *go_list;        /* P-Steuerung u.Kombination d.gen.Ops. */
static GO_ELEM_TYPE  *go_elem_ptr;    /* Zeiger auf d.akt. "go_list"-Eintrag  */
static DOUBLE         n_og1, n_og2;   /* Notengrenzen fuer "mut_data"-Saetze  */
static FILE          *evo_file;       /* EVO-File, fuer "loadEvoParamFile"    */
static INT            io_res,         /* letztes I/O-Erg.in "loadEvoParamFile"*/
                      bereich,        /* Random-Bereich in "mutations_zahl"   */
                      mut_anz,        /* Anz.d.MutationsOps. OG in "mut_data" */
                      par_satz_index; /* Index des aktuellen Parametersatzes  */



/* ============================ Lokale Routinen: ============================ */
static INT  mut_data_index (DOUBLE  note);

static BOOL io_err         (INT     f_pos,
                            INT     soll_anz);




/* ============================= mut_data_index ============================= */
static INT mut_data_index (DOUBLE note)
/*----------------------------------------------------------------------------*/
/*  Ermittelt bei gesetztem "do_evo" den Index des zur "note" gehoerigen      */
/*  "mut_data"-Parametersatzes und liefert ihn ab. Sonst den Index 3.         */
/*----------------------------------------------------------------------------*/
{
   if (do_evo)
   { /* Optimierung per Evolution */
      if (note < n_og1)
         return (0);
      else
         if (note < n_og2)
            return (1);
         else
            return (2);
   } /* Optimierung per Evolution */
   else
      return (3);
} /* mut_data_index */ 



/* ================================ calc_p_go =============================== */
INT calc_p_go (BOOL   *one_child,
               INT     go_index,
               DOUBLE  note)
{
   BOOL  do_it;
   
   /* calc_p_go */
   par_satz_index = mut_data_index (note);
#ifdef CHECKS
   if (go_anz <= go_index)
   { /* go_index zu gross */
      fatal (EVO_GO_GSW, 1, go_index, go_anz, "", BAD_MUT_AMOUNT);
      return (FALSE);
   } /* go_index zu gross */
#endif /* CHECKS */
   go_elem_ptr = &(go_list[par_satz_index][go_index]);
   if (go_elem_ptr->p_akt < 1.0)
      if (go_elem_ptr->p_akt == 0.0)
         do_it = FALSE;
      else
         do_it = frand () < go_elem_ptr->p_akt;
   else
      do_it = TRUE;
   if (do_it)
   {
      *one_child = go_elem_ptr->eins;
      return (go_elem_ptr->op_anz);
   }
   else
      return (0);
} /* calc_p_go */ 



/* ================================ calc_p_op =============================== */
BOOL calc_p_op (INT *gen_op_id,
                INT  op_index)
{
   DOUBLE  pwert;   
   BOOL    do_it;
   
   /* calc_p_op */
   pwert = go_elem_ptr->p_list[op_index];
   if (pwert < 1.0)
      if (pwert == 0.0)
         do_it = FALSE;
      else
         do_it = frand () < pwert;
   else
      do_it = TRUE;
   if (do_it)
   {
      *gen_op_id = go_elem_ptr->op_list[op_index];
      return (TRUE);
   }
   else
      return (FALSE);
} /* calc_p_op */ 



/* ============================= mutations_zahl ============================= */
INT mutations_zahl (INT  mutation,
                    INT  len)
{
   INT           anz;
   DOUBLE        rlen;
   MUT_DATA_SET *mpar_ptr;
   
   /* MUTATIONS_ZAHL/XOVR_MUT */
#ifdef MTEST
   return (1);
#else /* kein MTEST */
  #ifdef CHECKS
   if (mut_anz <= mutation)
   { /* "mutation"-Index zu gross */
      fatal (EVO_GO_GSW, 2, mutation, mut_anz, "", BAD_MUT_AMOUNT);
      return (0);
   } /* "mutation"-Index zu gross */
  #endif /* CHECKS */
   if (len == 0)
      return (0);   /* Tue nie einer leeren oder segmentlosen Kette etwas an! */
   mpar_ptr = &(mut_data[par_satz_index][mutation]);
   rlen     = len;
   switch (mpar_ptr->mut_alg)
   {
      case QUAD: /* mut_alg == 3 */
         bereich = i_round (mpar_ptr->a * rlen*rlen + mpar_ptr->b * rlen +
                            mpar_ptr->c);
         if (bereich < 2)
            return (1);
         break; /* mut_alg == 3 */

      case LIN1: /* mut_alg == 0 */
         if (len < (INT)(mpar_ptr->a2))
            return (0);
         else
         {
            bereich = i_round ((rlen / mpar_ptr->a) + mpar_ptr->b);
            if (bereich < mpar_ptr->rounded_c)
               bereich = mpar_ptr->rounded_c;
         }
         break; /* mut_alg == 0 */

      case LIN2: /* mut_alg == 1 */
         if (len < (INT)(mpar_ptr->a2))
            return (0);
         else
         {
            bereich = i_round ((rlen / mpar_ptr->a) + mpar_ptr->b);
            if (bereich > mpar_ptr->rounded_c)
               bereich = mpar_ptr->rounded_c;
         }
         break; /* mut_alg == 1 */

      case LIN3: /* mut_alg == 2 */
         if (len < mpar_ptr->rounded_c) 
            bereich = i_round ((rlen / mpar_ptr->a)  + mpar_ptr->b);
         else 
         {
            bereich = i_round ((rlen / mpar_ptr->a2) + mpar_ptr->b2);
            if (bereich < 2)
               return (1);
         }
         break; /* mut_alg == 2 */

      default:
         fatal (EVO_GO_GSW, 3, mpar_ptr->mut_alg, mutation, "", BAD_MUT_DATA);
   } /* switch */
   if (bereich > len)
      bereich = len;
  #ifdef MZTEST
   anz = irand (bereich) + 1;
   sprintf (buf1, "Bereich=%d  Anz=%d", bereich, anz);
   printf ("%s\n", buf1);
   return (anz);
  #else /* kein MZTEST */
   return (irand (bereich) + 1);
  #endif /* kein MZTEST */
#endif /* kein MTEST */
} /* mutations_zahl */ 



/* ================================= io_err ================================= */
static BOOL io_err (INT  f_pos,
                    INT  soll_anz)
/*----------------------------------------------------------------------------*/
/*  Die Routine prueft, ob die erforderliche Anzahl Werte gelesen wurde,      */
/*  indem sie die gewuenschte "soll_anz" mit den tatsaechlich erhaltenen      */
/*  "io_res" vergleicht. Ausserdem wird auf EOF geprueft. Im Fehlerfalle      */
/*  wird das EVO-File wird geschlossen, mit "f_pos" eine Fehlermeldung        */
/*  erzeugt und TRUE abgeliefert.                                             */
/*----------------------------------------------------------------------------*/
{
   GSTATE  ret_state = GLEAM_OK;
   
   if (io_res != soll_anz)
      ret_state = fehler (EVO_GO_GSW, f_pos, io_res, soll_anz, "",
                          EVO_FILE_DATA_ANZ_ERR);
   if (feof (evo_file))
      ret_state |= fehler (EVO_GO_GSW, f_pos, NO_INFO, NO_INFO, "",
                           EVO_FILE_EOF_ERR);
   if (ret_state != GLEAM_OK)
      fclose (evo_file);
   return (ret_state != GLEAM_OK);
} /* io_err */ 



/* ============================= loadEvoParamFile =========================== */
GSTATE loadEvoParamFile (char *fspec,
                         BOOL *bad_data)
{
   STRING_TYPE  buffer;
   STR_TYPE     id_buf;
   DOUBLE       new_n_og1, new_n_og2;
   INT          new_mut_anz, new_go_anz,
                offsprAnzPerSatz,
                ii, ij, tst, mi;
   char        *ch_tmp;
   
   /* loadEvoParamFile */
   *bad_data = FALSE;
   if (!open_file (&evo_file, fspec, "rt"))
      return (GLEAM_ERROR);
   for (ii = 1;  ii <= 4;  ii++)
      ch_tmp = fgets (buffer,  MAX_STRING+1, evo_file); /*skip general header */
   io_res = fscanf (evo_file, "%[^\n]s", id_buf);         /* Lies Programm-ID */
   if (io_err (4, 1))
      return (GLEAM_ERROR);
   if (strcmp (id_buf, PROG_IDENT) != 0)
   { /* falsches MOD-File-Ident */
      fclose (evo_file);
      if (strlen (id_buf) == 0)
         strcpy (buffer, " ");                       /* never an empty string */
      else
         strcpy (buffer, id_buf);
      return (fehler (EVO_GO_GSW, 5, NO_INFO, NO_INFO, buffer, BAD_EVO_FILE));
   } /* falsches MOD-File-Ident */
   ch_tmp = fgets (buffer, MAX_STRING+1, evo_file);      /* skip rest of line */
   io_res = fscanf (evo_file, "%lf%lf", &new_n_og1, &new_n_og2);
   ch_tmp = fgets (buffer,  MAX_STRING+1, evo_file);     /* skip rest of line */
   if (io_err (6, 2))
      return (GLEAM_ERROR);
   io_res = fscanf (evo_file, "%d %d", &new_mut_anz, &new_go_anz);
   ch_tmp = fgets (buffer, MAX_STRING+1, evo_file);      /* skip rest of line */
   if (io_err (7, 2))
      return (GLEAM_ERROR);
   if (new_mut_anz != (LAST_GO + 1))
   { /* Falsche Anzahl Mutations-Operatoren */
      fclose (evo_file);
      return (fehler (EVO_GO_GSW, 8, new_mut_anz, LAST_GO + 1, "",
                      BAD_MUT_AMOUNT));
   } /* Falsche Anzahl Mutations-Operatoren */
   if (new_go_anz > ANZ_PAAR_OPS)
   { /* Falsche Anzahl genetischer Operatoren */
      fclose (evo_file);
      return (fehler (EVO_GO_GSW, 9, new_go_anz, ANZ_PAAR_OPS, "",
                      BAD_MUT_AMOUNT));
   } /* Falsche Anzahl genetischer Operatoren */
   
   /* ---------- Ab jetzt verderben I/O-Fehler die internen Daten: ---------- */
   *bad_data = TRUE;
   mut_anz   = new_mut_anz;
   go_anz    = new_go_anz;
   n_og1     = new_n_og1;
   n_og2     = new_n_og2;
   
   /*  Einlesen der 4 Parametersaetze von "mut_data": Der Fehlercode hat 4    */
   /*  Bereiche zu je max. 99 Werten: 100..199, 200..299, usw. Der Wert gibt  */
   /*  den Operator beginnend mit 1 an.                                       */
   for (ii = 0;  ii < ANZ_PAR_SAETZE;  ii++)
   { /* Einen Parametersatz lesen */
      for (ij = 1;  ij <= 4;  ij++)
         ch_tmp = fgets (buffer,  MAX_STRING+1, evo_file); /*skip param.header*/
      tst = (ii+1) * 100;
      for (mi = 0;  mi < mut_anz;  mi++)
      { /* Initialisierung der Parameter aller Mutationen */
         io_res = fscanf (evo_file, "%d%lf%lf%lf%lf%lf",
                          &(mut_data[ii][mi].mut_alg),
                          &(mut_data[ii][mi].a),
                          &(mut_data[ii][mi].b),
                          &(mut_data[ii][mi].c),
                          &(mut_data[ii][mi].a2),
                          &(mut_data[ii][mi].b2));
         ch_tmp = fgets (buffer, MAX_STRING+1, evo_file); /*skip rest of line */
         mut_data[ii][mi].rounded_c = i_round (mut_data[ii][mi].c);
         if (io_err (tst + mi + 1, 6))
            return (GLEAM_ERROR);
      } /* Initialisierung der Parameter aller Mutationen */
   } /* Einen Parametersatz lesen */
   
   /*  Einlesen der 4 Parametersaetze von "go_list": Der Fehlercode hat 4     */
   /*  Bereiche zu je max. 999 Werten: 1000..1999, 2000..2999, usw.           */
   /*  Codierung: pgnn  mit: p=ParamSatzNr g=GenOpNr nn=lfdNr (1..49, 50.99)  */
   maxOffsprAmount = 0;
   for (ii = 0;  ii < ANZ_PAR_SAETZE;  ii++)
   { /* Einen Parametersatz lesen */
      offsprAnzPerSatz = 0;
      for (ij = 1;  ij <= 5;  ij++)
         ch_tmp = fgets (buffer,  MAX_STRING+1, evo_file); /*skip parame.header*/
      for (mi = 0;  mi < go_anz;  mi++)
      { /* Initialisierung der Parameter aller gen. Operatoren der "go_list" */
         tst = (ii+1) * 1000 + (mi+1) * 100;
         io_res = fscanf (evo_file, "%lf%lf%d%d",
                          &(go_list[ii][mi].p_akt),
                          &(go_list[ii][mi].p_min),
                          &(go_list[ii][mi].eins),
                          &(go_list[ii][mi].op_anz));
         ch_tmp = fgets (buffer, MAX_STRING+1, evo_file); /*skip rest of line */
         if (io_err (tst, 4))
            return (GLEAM_ERROR);
         if (ANZ_GEN_OPS < go_list[ii][mi].op_anz)
         { /* Falsche Anzahl genetischer Operatoren in der "op_list" */
            fclose (evo_file);
            return (fehler (EVO_GO_GSW, tst+999, go_list[ii][mi].op_anz,
                            ANZ_GEN_OPS, "", BAD_MUT_AMOUNT));
         } /* Falsche Anzahl genetischer Operatoren in der "op_list" */
         for (ij = 0;  ij < go_list[ii][mi].op_anz;  ij++)
         { /* Lies "op_list" des aktuellen "go_list"-Eintrags */
            io_res = fscanf (evo_file, "%d", &(go_list[ii][mi].op_list[ij]));
            if (io_err (tst + ij + 1, 1))
               return (GLEAM_ERROR);
         } /* Lies "op_list" des aktuellen "go_list"-Eintrags */
         for (ij = 0;  ij < go_list[ii][mi].op_anz;  ij++)
         { /* Lies "p_list" des aktuellen "go_list"-Eintrags */
            io_res = fscanf (evo_file, "%lf", &(go_list[ii][mi].p_list[ij]));
            if (io_err (tst + ij + 51, 1))
               return (GLEAM_ERROR);
         } /* Lies "p_list" des aktuellen "go_list"-Eintrags */
         ch_tmp = fgets (buffer, MAX_STRING+1, evo_file); /*skip rest of line*/
	 if (go_list[ii][mi].eins)
	    offsprAnzPerSatz++;       /* calculation of the maximum possible */
	 else
	    offsprAnzPerSatz += 2;    /* number of offspring per mating      */
      } /* Initialisierung der Parameter aller gen. Operatoren der "go_list" */
      if (maxOffsprAmount < offsprAnzPerSatz)
	 maxOffsprAmount = offsprAnzPerSatz;
   } /* Einen Parametersatz lesen */
   fclose (evo_file);
   *bad_data = FALSE;
   return (GLEAM_OK);
} /* loadEvoParamFile */ 



/* ================================ evoInit ================================= */
BOOL evoInit (void)
{
   BOOL  erg;

   /* evoInit */
   erg = ((buf1          = (char*)malloc(MAX_STRING * sizeof(char))) != NULL) &&
         ((buf2          = (char*)malloc(MAX_STRING * sizeof(char))) != NULL) &&
         ((buf3          = (char*)malloc(MAX_STRING * sizeof(char))) != NULL) &&
         ((mut_data      = (MUT_DATA_TYPE*)malloc(ANZ_PAR_SAETZE * sizeof(MUT_DATA_TYPE))) != NULL) &&
         ((go_list       = (GO_LIST_TYPE*) malloc(ANZ_PAR_SAETZE * sizeof(GO_LIST_TYPE)))  != NULL) &&
         rxo_init() && init_evo_steu() && init_evo_anzg();
   maxOffsprAmount = 0;
#ifdef SLAVE
   erg &= evo_init_sl();
#else
   erg &= evo_init();
#endif /* SLAVE */
   do_evo = TRUE;
   return (erg);
} /* evoInit */



/* ============================= get_last_go_idx ============================ */
INT get_last_go_idx (void)
{
   return (LAST_GO);
} /* get_last_go_idx */



/* ============================ get_noten_limits ============================ */
void get_noten_limits (LONG *n_lim1,
                       LONG *n_lim2)
{
   *n_lim1 = (LONG)n_og1;
   *n_lim2 = (LONG)n_og2;
} /* get_noten_limits */



/* ============================== get_mz_bereich ============================ */
INT get_mz_bereich (void)
{
   return (bereich);
} /* get_mz_bereich */
