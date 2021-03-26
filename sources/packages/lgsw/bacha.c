/*******************************************************************************
GLEAM/AE                                                                  V1.0.2

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: Listen-GrundSoftWare
                        Module: BAsic CHain memory Administration
 
Package: lgsw                   File: bacha.c 
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Der Modul enthaelt Grundsoftware fuer die Verwaltung der speicherbaren Ketten-
arten; derzeit nur Aktions- und Regelketten (AKs und RKs). Ausserdem enthaelt es
Routinen zur Speicherbereinigung. 
 
Bei den meisten Routinen ist die Kettenart als Parameter ("ketten_art") vorzu-
geben. Generell sind nur speicherbare Ketten moeglich (derzeit AKs und RKs), wo-
bei dies bei einigen Routinen noch weiter eingeschraenkt sein kann. Die Routinen
ueberpruefen die Kettenart und melden ggf. einen fatalen Fehler. "ketten_art" 
darf nicht mit den ".._header"-Codierung von "activity" verwechselt werden. Eine
eigene Codierung ist notwendig, da "ketten_art" zur Indexierung benutzt wird. Es
gibt folgende Kettenarten: "AK", "RK" und "SONST". AKs und RKs werden entspre-
chend ihrer Guete gespeichert. Bei den "SONST"-igen Ketten differenziert der in 
den Routinen "store_chain", "get_chain" und "number_of_chains" anzugebende 
Guete-Parameter die Kettensorte (derzeit nicht verwendet).
 
Die oben und bei den Routinen beschriebene Pruefungen von insbesondere "ketten_
art"- u. "guete"-Parametern kann mit dem Compilerschalter "checks" an- und abge-
schaltet werden. Dieser steuert auch die Pruefung uebergebener Pointer auf NIL. 
Nichtabschaltbare Pruefungen sind in den Routinen besonders vermerkt. Die Routi-
nen setzen voraus, dass fuer ihre Operationen genuegend Speicherplatz zur Ver-
fuegung steht (sonst fataler Fehler). Dies ist durch die aufrufende Software 
sicherzustellen. Die Routinen liefern einen Status vom Typ GSTATE (s. Package
"fbhm") zurueck. Wenn dieser als Parameter uebergeben wird, fuehrt die jeweilige
Routine ihren Rumpf nur aus, wenn GLEAM_OK uebergeben wurde und eventuelle int-
erne Checks positiv verliefen.
 
Der beste Guetewert evolutionierbarer Ketten ist "MAX_GUETE", d.h. je groesser 
der Wert, desto besser die Guete. Der Wertebereich beginnt mit 1, der schlech-
testen Guete. Der Guetewert 0 bezeichnet die Verwaltungskette der Basis-AKs.


Die Routinen im einzelnen:

BOOL   berechne_guete (INT    *guete,
                       DOUBLE  fit_note);
          Berechnet aus der Note die zugehoerige Gueteklasse "guete" wie folgt: 
          Wenn "fit_note" == "KEINE_NOTE" ist, oder "FNOTEN_MAX" uebersteigt, 
          liefert die Funktion FALSE und als "guete" "KEINE_GUETE", sonst TRUE.

void   get_rank_limits (LONG *ug,
                        LONG *og,
                        INT   guete);
          Liefert die Notenober- und -untergrenzen des zu "guete" gehoerigen
          Guete-Intervalls als Integerwerte ab.

GSTATE store_chain (INT                 *number, 
                    CHAIN_ELEM_PTR_TYPE  chain_ptr, 
                    INT                  ketten_art, 
                    INT                  guete);
          Die Kette wird an die Verwaltungskette der Guete "guete" des durch 
          "ketten_art" bezeichneten Kettenspeichers angehaengt.
          "guete" muss zwischen 0 und "MAX_GUETE" liegen, sonst liegt ein fata-
          ler Fehler vor. In "number" wird die laufende Nummer der Kette inner-
          halb der VK zurueckgeliefert. "number" und "guete" werden im Kopf ge-
          speichert. Bei "ketten_art" = "SONST" enthaelt die "guete" die Ketten-
          sorte (derzeit nicht verwendet); im Kopf wird unter "guete" "KEINE_
          GUETE" gespeichert.
          Kann auch Basis-AKs ("guete" = "BASIS_AK_GUETE") speichern.

BOOL   get_chain (CHAIN_ELEM_PTR_TYPE *chain_ptr, 
                  INT                 *number, 
                  INT                  ketten_art, 
                  INT                  guete_ug, 
                  INT                 *guete_og,
                  GSTATE              *ret_status);
          Die Funktion liefert TRUE und einen Zeiger auf die gewuenschte Kette,
          sofern diese im durch "ketten_art" bezeichneten Kettenspeicher auf-
          findbar ist. Ansonsten FALSE und einen leeren Zeiger. 
          Mit "number" kann die "number"-te Kette vorgegebener Guete gesucht 
          werden. Wenn "number" gleich Null ist, wird aus den vorhandenen Ket-
          ten des mit "guete_ug" und "guete_og" vorgegeben Guete-Intervalls eine
          Kette per Zufall ausgewaehlt werden. Sind beide GueteWerte gleich, 
          kommen nur Ketten der vorgegebenen Guete in Frage. Ansonsten werden 
          aus dem angegebenen Guete-Intervall alle Werte selektiert, zu denen es
          auch Ketten gibt. Daraus wird dann ein Wert per Zufall ermittelt. 
          "number" und "guete_og" liefern ausserdem die Guete und die laufende 
          Nummer der selektierten Kette zurueck. Bei "ketten_art" = "SONST" gibt
          die "guete" die Kettensorte (derzeit unbenutzt) an. Bei "ketten_art" =
          "AK" sind unter der "guete" "BASIS_AK_GUETE"( = 0) die Basis-AKs ge-
          speichert.
          Wenn "guete_ug" kleiner als 0 oder "max_guete" < "guete_og" ist, liegt
          ein fataler Fehler vor ("ret_status"). Die Funktion wird nur ausge-
          fuehrt, wenn "ret_status" = GLEAM_OK!

INT    number_of_chains (INT     guete_ug, 
                         INT     guete_og, 
                         INT     ketten_art,
                         GSTATE *ret_status);
          Die Funktion liefert die Anzahl gespeicherter Ketten des vorgegebenen 
          Gueteintervalls im durch "ketten_art" bezeichneten Kettenspeicher.
          Wenn "guete_ug" kleiner als 0 oder "max_guete" < "guete_og" ist, liegt
          ein fataler Fehler vor ("ret_status"). Bei "ketten_art" = "SONST" gibt
          die "guete" die Kettensorte (derzeit unbenutzt) an. Kann auch die An-
          zahl der Basis-AKs abliefern ("BASIS_AK_GUETE").

CHAIN_ELEM_PTR_TYPE get_vk (INT     k_art, 
                            INT     guete,
                            GSTATE *ret_status);
          Es wird ein Zeiger auf die durch "k_art" und "guete" bezeichnete VK 
          abgeliefert. Bei fehlerhaften Parametern er folgt ein fataler Fehler 
          ("ret_status") und das Ergbnis ist NULL.

void   update_ch_adr (CHAIN_ELEM_PTR_TYPE  start_ptr);
          Ab der durch einschliesslich "start_ptr" bezeichneten Kettenposition 
          in einer VK werden alle "lfd_nr"-Eintraege in den Kettenkoepfen decre-
          mentiert.

GSTATE umhaengen (INT *neue_lfd_nr, 
                  INT  ketten_art, 
                  INT  neue_guete,
                  INT  alte_guete, 
                  INT  alte_lfd_nr);
          Die mit "ketten_art" (derzeit nur AK und RK), "alte_guete" und "alte_
          lfd_nr" bezeichnete Kette wird unter "neue_guete" gespeichert.
          "neue_guete" und "alte_guete" muessen zwischen 1 und "max_guete" lie-
          gen, sonst liegt ein fataler Fehler vor. Die "neue_lfd_nr" wird abge-
          liefert. Die "lfd_nr"-Eintraege in den Koepfen der verbleibenden Ket-
          ten werden upgedatet. Die Existenz der umzuhaengenden Kette wird immer
          abgeprueft (fataler Fehler).

BOOL   select_action_chain (CHAIN_ELEM_PTR_TYPE  item_ptr,
                            GSTATE              *ret_status);
          Die Funktion waehlt per Zufall eine Aktionskette der Basis-AKs aus und
          haengt sie an die durch "item_ptr" bezeichnete Aktion, die ein "acti-
          vity_act" sein muss (sonst fataler Fehler, siehe "ret_status"). Die 
          Funktion liefert FALSE und laesst die Aktion unveraendert, wenn es 
          weniger als MIN_GUTE_ KETTEN Kette gibt oder "ret_status" nicht GLEAM_
          OK ist.

INT    clear_chains (INT     ketten_art, 
                     INT     zu_loeschen,
                     GSTATE *ret_status);
          Wenn genuegend loeschbare Ketten da sind, loescht die Funktion solange
          Ketten vom Typ "ketten_art" (derzeit nur AKs und RKs) beginnend mit 
          denen niedrigster Guete, bis "zu_loeschen" Ketten geloescht sind. Da-
          bei werden die aelteren Ketten zuerst geloescht. Die "lfd_nr"-Eintrae-
          ge in den Koepfen der verbleibenden Ketten werden upgedatet. Die Rou-
          tine liefert die Anzahl geloeschter Ketten ab und inkrementiert den 
          "released_ak"- bzw. den "released_rk"-Zaehler des Moduls "mmsw". Eine
          fehlerhafte "ketten_art" verursacht einen fatalen Fehler ("ret_
          status").

GSTATE reclaim_memory (INT  ketten_art);
          Loescht maximal MAX_DEL_WDH-mal den durch "ketten_art" bezeichneten
          Kettenspeicher (derzeit nur AKs), bis darin nur noch REST_ANTEIL Ket-
          ten enthalten sind und erstattet eine Meldung ueber die Anzahl der ge-
          loeschten Ketten. REST_ANTEIL bezieht sich auf die aktuell loeschbaren
          Ketten und nicht auf den gesamten Kettenspeicher. Eine fehlerhafte 
          "ketten_art" verursacht einen fatalen Fehler.

GSTATE clr_and_reorg (CHAIN_ELEM_PTR_TYPE *chain_ptr, 
                      INT                  ketten_art);
          Die Routine erzeugt eine Verwaltungskette, haengt alle Ketten der Art
          "ketten_art" des Kettenspeichers dran und liefert sie in "chain_ptr" 
          ab. Das Funktionsergebnis ist GLEAM_OK, wenn dabei keine Fehler auf-
          traten.

void   bacha_init (void);
          Initialisiert die Kettenspeicher und ist vor Benutzung des Moduls 
          aufzurufen.

GSTATE clear_vk (INT  ketten_art,
                 INT  guete);
          Loescht alle Ketten der durch "ketten_art" und "guete" bezeichneten 
          Verwaltungskette. Das Funktionsergebnis reflektiert den Erfolg der 
          Loeschung.

GSTATE delete_ak_mem (void);
          Loescht den gesamten AK-Speicher und setzt den "released_ak"-Zaehler
          zurueck.

GSTATE delete_mem (void);
          Loescht den gesamten Kettenspeicher und setzt die "released_.."-
          Zaehler zurueck.
          Derzeit nur den AK-Speicher.

GSTATE check_bak (BOOL *ak_exists,
                  BOOL *ak_is_bak,
                  INT   guete,
                  INT   lfd_nr);
          Prueft, ob die durch "guete" und "lfd_br" angegebene AK existiert und
          sofern dies der Fall ist, ob es sich dabei um eine Basis-AK handelt.
          Die Ergebnisse werden in "ak_exists" und "ak_is_bak" zurueckgeliefert.

GSTATE do_bak_to_ak (CHAIN_ELEM_PTR_TYPE  ak_ptr);
          Wandelt die durch "ak_ptr" angegebene Basis-AK in eine AK um und 
          liefert GLEAM_OK zurueck, wenn alles gut ging. Wenn die angegebene AK 
          bereits keine BAK mehr ist, geschieht nichts. 

GSTATE del_chain (BOOL *chain_deleted,
                  INT   ketten_art, 
                  INT   guete, 
                  INT   lfd_nr, 
                  BOOL  del_bak);
          Loescht die durch "ketten_art", "guete" und "lfd_nr" bezeichnete Kette
          und liefert in "chain_deleted" TRUE an, wenn die Kette geloescht wer-
          den konnte.
          Bei gesetztem "del_bak" wird die Eigenschaft BAK zurueckgesetzt und 
          dann wie bei allen Ketten geprueft, ob sie loeschbar ist, d.h. ob der 
          Referenzzaehler 1 oder kleiner ist. Wenn ja, erfolgt die Loeschung. 
          Auftretende Fehler werden als Funktionsergebnis gemeldet.

--------------------------------------------------------------------------------
basiert auf C-Unit: bacha.
--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. Linux-Portierung: Aus "round" wird "l_round" und "i_round".      WJ 11.05.05
 2. Entfernung ueberfluessiger Variable.                           . WJ 26.06.12
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 26.06.2012
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schalter.h"
#include "sys.h"
#include "fbhm.h"
#include "lgsw.h"
#include "lgswInt.h"


#ifdef GLO_CHECKS
  #define CHECKS
#endif



/* ============================ Lokale Konstante: =========================== */
#define REST_ANTEIL   0.65     /* von "reclaim_memory" zu belassender %-Satz  */
#define MAX_DEL_WDH   5        /* Max. Anzahl d. Versuche, Ketten zu loeschen */



/* ======================== Lokale Typdeklarationen: ======================== */
typedef DOUBLE               GUETE_NOTEN_TYP [MAX_GUETE+1];

typedef CHAIN_ELEM_PTR_TYPE  CHAIN_MEM_TYPE  [SONST+1][MAX_GUETE+1];

typedef INT                  SUCH_FELD_TYPE  [MAX_GUETE+1];




/* ============================ Lokale Variable: ============================ */
static GUETE_NOTEN_TYP guete_og_note =   /*Obere Notengrenzen d.GüteIntervalle*/
  {0.0,    
       1.0,  2500.0,  5000.0, 10000.0,   20000.0, 30000.0, 40000.0, 50000.0,  
   60000.0, 70000.0, 75000.0, 80000.0,   85000.0, 90000.0, 95000.0, RNOTEN_MAX+0.5};

static CHAIN_MEM_TYPE  chain_mem;                       /* Der Kettenspeicher */





/* ============================= berechne_guete ============================= */
BOOL berechne_guete (INT    *guete,
                     DOUBLE  fit_note)
{
   BOOL  found;

   /* BERECHNE_GUETE/BACHA */
   found  = FALSE;
   *guete = 1;
   if (fit_note != KEINE_NOTE) 
   {
      do 
         if (fit_note < guete_og_note [*guete]) 
            found = TRUE;
         else 
            (*guete)++;
      while (!(found || (*guete > MAX_GUETE)));
   }
   if (!found) 
      *guete = KEINE_GUETE;
   return (found);
} /* berechne_guete */



/* ============================= get_rank_limits ============================ */
void get_rank_limits (LONG *ug,
                      LONG *og,
                      INT   guete)
{
   *ug = l_round (guete_og_note [guete-1]);
   *og = l_round (guete_og_note [guete] - 1.0);
} /* get_rank_limits */



/* =============================== store_chain ============================== */
GSTATE store_chain (INT                 *number, 
                    CHAIN_ELEM_PTR_TYPE  chain_ptr, 
                    INT                  ketten_art, 
                    INT                  guete)
{
   CHAIN_ELEM_PTR_TYPE  item_ptr;
   GSTATE               ret_state;

   /* STORE_CHAIN/BACHA */
#ifdef CHECKS
   if ((guete < 0) || (MAX_GUETE < guete)) 
      return (fatal (LGSW_BACHA, 1, guete, NO_INFO, "", MEINE_GUETE));
   if ((ketten_art < AK) || (SONST < ketten_art))
      return (fatal (LGSW_BACHA, 2, ketten_art, NO_INFO, "", BAD_CHAIN_KIND));
   if (chain_ptr == NULL) 
      return (fatal (LGSW_BACHA, 3, NO_INFO, NO_INFO, "", NIL_PTR));
#endif
   *number = 0;
   if (new_chain_elem (&item_ptr) != GLEAM_OK)
      return (GLEAM_FATAL);
   item_ptr->u_el.activity              = ACTIVITY_ACT;
   item_ptr->u_el.u_xel.u_sub.ref_typ   = VK_ITEM_REF;
   item_ptr->u_el.u_xel.u_sub.sub_chain = NULL;
   ret_state  = append_sub_chain (item_ptr, chain_ptr);        /* Kein Fehler */
   ret_state |= append_item (chain_mem[ketten_art][guete], item_ptr); /* mögl.*/
   *number = chain_mem[ketten_art][guete]->u_el.u_xel.u_hdr.chain_length;
   if (ketten_art == SONST) 
      guete = KEINE_GUETE;
   chain_ptr->u_el.u_xel.u_hdr.guete  = guete;
   chain_ptr->u_el.u_xel.u_hdr.lfd_nr = *number;
   return (ret_state);
} /* store_chain */



/* ================================ get_chain =============================== */
BOOL get_chain (CHAIN_ELEM_PTR_TYPE *chain_ptr, 
                INT                 *number, 
                INT                  ketten_art, 
                INT                  guete_ug, 
                INT                 *guete_og,
                GSTATE              *ret_status)
{
   BOOL                 r_get_chain;
   SUCH_FELD_TYPE       such_feld;
   CHAIN_ELEM_PTR_TYPE  item_ptr;
   INT                  gi, found_gi;

   /* GET_CHAIN/BACHA */ 
#ifdef CHECKS
   if ((guete_ug < 0) || (MAX_GUETE < guete_ug))
      *ret_status |= fatal (LGSW_BACHA, 4, guete_ug, NO_INFO, "", MEINE_GUETE);
   if ((*guete_og < 0) || (MAX_GUETE < *guete_og))
      *ret_status |= fatal (LGSW_BACHA, 5, *guete_og, NO_INFO, "", MEINE_GUETE);
   if ((ketten_art < AK) || (SONST < ketten_art))
      *ret_status |= fatal (LGSW_BACHA, 6, ketten_art, NO_INFO, "", BAD_CHAIN_KIND);
#endif
   if (*ret_status != GLEAM_OK)
      return (FALSE);
   r_get_chain = FALSE;
   *chain_ptr  = NULL;            /* "ret_status" liefert hier keinen Fehler: */
   if (number_of_chains (guete_ug, *guete_og, ketten_art, ret_status) > 0) 
   { /* es gibt Ketten */
      if (guete_ug < *guete_og) 
      { /* bestimme "guete_og" */
         found_gi = 0;
         for (gi = guete_ug;  gi <= *guete_og;  gi++) 
         { /* Suche alle Gueteklassen mit Ketten */
            if (chain_mem[ketten_art][gi]->u_el.u_xel.u_hdr.chain_length > 0)
            { /* Gueteklasse mit Ketten gefunden */
               such_feld[found_gi] = gi;
               found_gi++;
            } /* Gueteklasse mit Ketten gefunden */
         } /* Suche alle Gueteklassen mit Ketten */
         *guete_og = such_feld [irand (found_gi)];
         *number   = 0;
      } /* bestimme "guete_og" */
      if (*number == 0) 
         *number = irand (chain_mem[ketten_art][*guete_og]->
                          u_el.u_xel.u_hdr.chain_length) + 1;
      if (count_out_item (&item_ptr, chain_mem[ketten_art][*guete_og], 
                          *number, ret_status)) 
      { /* Kette gefunden */
         *chain_ptr  = item_ptr->u_el.u_xel.u_sub.sub_chain;
         r_get_chain = TRUE;
      } /* Kette gefunden */
   } /* es gibt Ketten */
   return (r_get_chain);
} /* get_chain */



/* ============================ number_of_chains ============================ */
INT number_of_chains (INT     guete_ug, 
                      INT     guete_og, 
                      INT     ketten_art,
                      GSTATE *ret_status)
{
   INT  ii, sum;

   /* NUMBER_OF_CHAINS/BACHA */
#ifdef CHECKS
   if ((guete_ug < 0) || (MAX_GUETE < guete_ug))
      *ret_status |= fatal (LGSW_BACHA, 7, guete_ug, NO_INFO, "", MEINE_GUETE);
   if ((guete_og < 0) || (MAX_GUETE < guete_og))
      *ret_status |= fatal (LGSW_BACHA, 8, guete_og, NO_INFO, "", MEINE_GUETE);
   if ((ketten_art < AK) || (SONST < ketten_art))
      *ret_status |= fatal (LGSW_BACHA, 9, ketten_art, NO_INFO, "", BAD_CHAIN_KIND);
#endif
   sum = 0;
   if (*ret_status == GLEAM_OK)
      for (ii = guete_ug;  ii <= guete_og;  ii++) 
         sum += chain_mem[ketten_art][ii]->u_el.u_xel.u_hdr.chain_length;
   return (sum);
} /* number_of_chains */



/* ================================= get_vk ================================= */
CHAIN_ELEM_PTR_TYPE get_vk (INT     k_art, 
                            INT     guete,
                            GSTATE *ret_status)
{ /* GET_VK/BACHA */
#ifdef CHECKS
   if ((guete < 0) || (MAX_GUETE < guete))
      *ret_status |= fatal (LGSW_BACHA, 10, guete, NO_INFO, "", MEINE_GUETE);
   if ((k_art < AK) || (SONST < k_art))
      *ret_status |= fatal (LGSW_BACHA, 11, k_art, NO_INFO, "", BAD_CHAIN_KIND);
#endif
   if (*ret_status == GLEAM_OK)
      return (chain_mem[k_art][guete]);
   else
      return (NULL);
} /* get_vk */



/* ============================== update_ch_adr ============================= */
void update_ch_adr (CHAIN_ELEM_PTR_TYPE  start_ptr)
{ /* UPDATE_CH_ADR/BACHA */
   while (start_ptr != NULL) 
   {
      (start_ptr->u_el.u_xel.u_sub.sub_chain->u_el.u_xel.u_hdr.lfd_nr)--;
      start_ptr = start_ptr->u_el.next_action;
   }
} /* update_ch_adr */



/* ================================ umhaengen =============================== */
GSTATE umhaengen (INT *neue_lfd_nr, 
                  INT  ketten_art, 
                  INT  neue_guete,
                  INT  alte_guete, 
                  INT  alte_lfd_nr)
{
   CHAIN_ELEM_PTR_TYPE  vk_neu, item_ptr, pred_ptr;
   GSTATE               ret_status = GLEAM_OK;

   /* UMHAENGEN/BACHA */ 
#ifdef CHECKS
   if ((neue_guete < 0) || (MAX_GUETE < neue_guete))
      return (fatal (LGSW_BACHA, 12, neue_guete, NO_INFO, "", MEINE_GUETE));
   if ((alte_guete < 0) || (MAX_GUETE < alte_guete))
      return (fatal (LGSW_BACHA, 13, alte_guete, NO_INFO, "", MEINE_GUETE));
   if ((ketten_art < AK) || (RK < ketten_art))
      return (fatal (LGSW_BACHA, 14, ketten_art, NO_INFO, "", BAD_CHAIN_KIND));
#endif
   if (count_out_item (&pred_ptr, chain_mem[ketten_art][alte_guete], 
                       alte_lfd_nr-1, &ret_status))    /*"ret_status" immer OK*/
   { /* Vorgaenger des Verwaltungs-Items ermittelt */
      update_ch_adr (pred_ptr->u_el.next_action);
      ret_status = remove_next_item (&item_ptr, chain_mem[ketten_art][alte_guete], 
                                     pred_ptr);
      if (item_ptr == NULL) 
         return (fatal (LGSW_BACHA, 15, NO_INFO, NO_INFO, "", NO_CHAIN));
      else 
      { /* chain ok */
         vk_neu = chain_mem[ketten_art][neue_guete];
         ret_status  |= append_item (vk_neu, item_ptr);
         *neue_lfd_nr = vk_neu->u_el.u_xel.u_hdr.chain_length;
         item_ptr->u_el.u_xel.u_sub.sub_chain->u_el.u_xel.u_hdr.lfd_nr= *neue_lfd_nr;
         item_ptr->u_el.u_xel.u_sub.sub_chain->u_el.u_xel.u_hdr.guete = neue_guete;
         return (GLEAM_OK);
      } /* chain ok */
   } /* Vorgaenger des Verwaltungs-Items ermittelt */
   else 
      return (fatal (LGSW_BACHA, 16, NO_INFO, NO_INFO, "", NO_CHAIN));
} /* umhaengen */

 

/* =========================== select_action_chain ========================== */
BOOL select_action_chain (CHAIN_ELEM_PTR_TYPE  item_ptr,
                          GSTATE              *ret_status)
{
   BOOL                 r_select_action_chain;
   CHAIN_ELEM_PTR_TYPE  selected_chain_ptr;
   INT                  lfd_nr, guete;
  
   /* SELECT_ACTION_CHAIN/BACHA */
#ifdef CHECKS
   if (item_ptr == NULL) 
      *ret_status |= fatal (LGSW_BACHA, 17, NO_INFO, NO_INFO, "", NIL_PTR);
#endif
   if ((chain_mem[AK][BASIS_AK_GUETE]->u_el.u_xel.u_hdr.chain_length < 
        MIN_GUTE_KETTEN) || (*ret_status != GLEAM_OK))
      r_select_action_chain = FALSE;
   else 
   { /* genuegend gute Ketten da */
      lfd_nr = 0;                                      /* Irgendeine Basis-AK */
      guete  = BASIS_AK_GUETE;                         /* Index 0 = Basis-Aks */
      if (get_chain (&selected_chain_ptr, &lfd_nr, AK, guete, &guete, 
                     ret_status))
         *ret_status |= append_sub_chain (item_ptr, selected_chain_ptr);
      item_ptr->u_el.u_xel.u_sub.ref_typ = SUB_AK_REF;
      r_select_action_chain              = *ret_status == GLEAM_OK;
   } /* genuegend gute Ketten da */
   printf ("%d", r_select_action_chain);
   return (r_select_action_chain);
} /* select_action_chain */



/* ============================== clear_chains ============================== */
INT clear_chains (INT     ketten_art, 
                  INT     zu_loeschen,
                  GSTATE *ret_status)
{
   CHAIN_ELEM_PTR_TYPE  item_ptr, v_chain_ptr, pred_item_ptr;
   INT                  len, ii, geloescht;
   INT                  neue_lfd_nr, gi;
   BOOL                 fertig;

   /* CLEAR_CHAINS/BACHA */
#ifdef CHECKS
   if ((ketten_art < AK) || (RK < ketten_art))
      *ret_status |= fatal (LGSW_BACHA, 18, ketten_art, NO_INFO, "", 
                            BAD_CHAIN_KIND);
#endif
   geloescht = 0;
   if ((zu_loeschen > 0) && (*ret_status == GLEAM_OK))
   { /* Es ist etwas zum Loeschen da */
      fertig = FALSE;
      gi     = 1;
      do 
      { /* Pruefe alle Gueteklassen */
         len = chain_mem[ketten_art][gi]->u_el.u_xel.u_hdr.chain_length;
         if (len > 0) 
         { /* Es gibt Ketten dieser Guete */
            ii            = 1;
            neue_lfd_nr   = 1;
            v_chain_ptr   = chain_mem[ketten_art][gi];
            pred_item_ptr = v_chain_ptr;
            do 
            { /* Pruefe alle Ketten dieser Gueteklasse */
               if ((pred_item_ptr->u_el.next_action->u_el.u_xel.u_sub.sub_chain->
                    u_el.u_xel.u_hdr.ref_counter <= 1) && !fertig) 
               { /* delete chain */
                  geloescht++;
                  fertig = geloescht == zu_loeschen;
                  *ret_status |= remove_next_item (&item_ptr, v_chain_ptr, 
                                                   pred_item_ptr);
                  *ret_status |= delete_item (item_ptr);
               } /* delete chain */
               else 
               {
                  pred_item_ptr = pred_item_ptr->u_el.next_action;
                  pred_item_ptr->u_el.u_xel.u_sub.sub_chain->u_el.u_xel.u_hdr.lfd_nr =
                     neue_lfd_nr;
                  neue_lfd_nr++;
               }
               ii++;
            } while (ii <= len);
         } /*  Es gibt Ketten dieser Guete */
         gi++;
      } while (!fertig && (gi <= MAX_GUETE) && (*ret_status == GLEAM_OK));
   } /* Es ist etwas zum Loeschen da */
   if (*ret_status == GLEAM_OK)
   {
      if (ketten_art == AK)
         incr_rel_aks (geloescht);
/*
      if (ketten_art == RK)
         incr_rel_rks (geloescht);
*/
   }
   return (geloescht);
} /* clear_chains */



/* ============================= reclaim_memory ============================= */
GSTATE reclaim_memory (INT  ketten_art)
{
   CHAIN_ELEM_PTR_TYPE  vk_item_ptr;
   INT                  ketten_anz, zu_loeschen, geloescht, 
                        ii, gi;
   DOUBLE               rr;
   GSTATE               ret_state = GLEAM_OK;

   /* RECLAIM_MEMORY/BACHA */
#ifdef CHECKS
   if ((ketten_art < AK) || (RK < ketten_art))
      return (fatal (LGSW_BACHA, 19, ketten_art, NO_INFO, "", BAD_CHAIN_KIND));
#endif
   ketten_anz = 0;
   for (gi = 1;  gi <= MAX_GUETE;  gi++)
   { /* Loeschbare Ketten aller Gueteklassen ermitteln */
      vk_item_ptr = chain_mem[ketten_art][gi]->u_el.next_action;
      for (ii=1; ii <= chain_mem[ketten_art][gi]->u_el.u_xel.u_hdr.chain_length; 
           ii++)
      {
         if (vk_item_ptr->u_el.u_xel.u_sub.sub_chain->u_el.u_xel.u_hdr.
             ref_counter <= 1) 
            ketten_anz++;
         vk_item_ptr = vk_item_ptr->u_el.next_action;
      }
   } /* Loeschbare Ketten aller Gueteklassen ermitteln */
   geloescht = 0;
   if (ketten_anz > 3)
   { /* Es gibt was zum Loeschen */
      rr = ketten_anz;
      rr = (1-REST_ANTEIL) * rr;
      zu_loeschen = i_round (rr);
      ii          = 0;
      do 
      {
         geloescht += clear_chains (ketten_art, zu_loeschen, &ret_state);
         ii++;
      } while ((geloescht < zu_loeschen) && (ii < MAX_DEL_WDH) && 
               (ret_state == GLEAM_OK));
   } /* Es gibt was zum Loeschen */
   if (geloescht > 0)
      meldung (LGSW_BACHA, geloescht, NO_INFO, "", MEM_OUT_WITH_RELEASE);
   else
      meldung (LGSW_BACHA, NO_INFO, NO_INFO, "", MEM_OUT_NO_RELEASE);
   return (ret_state);
} /* reclaim_memory */



/* ============================= clr_and_reorg ============================= */
GSTATE clr_and_reorg (CHAIN_ELEM_PTR_TYPE *chain_ptr, 
                      INT                  ketten_art)
{
   CHAIN_ELEM_PTR_TYPE  vk_ptr, anf_ptr, end_ptr;
   INT                  gi, chain_anz;
   GSTATE               ret_state = GLEAM_OK;
  
   /* CLR_AND_REORG/LSK_GSW */
#ifdef CHECKS
   if ((ketten_art < AK) || (RK < ketten_art))
      return (fatal (LGSW_BACHA, 19, ketten_art, NO_INFO, "", BAD_CHAIN_KIND));
#endif
   ret_state = init_chain (chain_ptr, VK_HEADER);
   for (gi = 1;  (gi <= MAX_GUETE) && (ret_state == GLEAM_OK);  gi++)
   { /* Fuer alle Gueteklassen */
      vk_ptr = get_vk (ketten_art, gi, &ret_state);
      chain_anz = vk_ptr->u_el.u_xel.u_hdr.chain_length;
      if (chain_anz>0)
      { /* Es gibt Ketten in dieser Gueteklasse */
         end_ptr = vk_ptr->u_el.u_xel.u_hdr.last_action;
         ret_state |= remove_seq (&anf_ptr, &end_ptr, &chain_anz, vk_ptr, 
                                  vk_ptr);
         ret_state |= insert_seq (*chain_ptr, *chain_ptr, anf_ptr, end_ptr, 
                                  chain_anz);
      } /* Es gibt Ketten in dieser Gueteklasse */
   } /* Fuer alle Gueteklassen */
   return (ret_state);
} /* clr_and_reorg */ 



/* ================================ clear_vk ================================ */
GSTATE clear_vk (INT  ketten_art,
                 INT  guete)
{
   CHAIN_ELEM_PTR_TYPE  vk_ptr, anf_ptr, end_ptr;
   INT                  chain_anz;
   GSTATE               ret_status = GLEAM_OK;

   /* CLEAR_VK/LSK_GSW */
#ifdef CHECKS
   if ((ketten_art < AK) || (SONST < ketten_art))
      return (fehler (MEN_GSW, 11, ketten_art, NO_INFO, "", BAD_CHAIN_KIND));
#endif /* CHECKS */
   vk_ptr    = get_vk (ketten_art, guete, &ret_status);
   chain_anz = vk_ptr->u_el.u_xel.u_hdr.chain_length;
   if (chain_anz > 0)
   { /* Es gibt Ketten in dieser VK */
      end_ptr     = vk_ptr->u_el.u_xel.u_hdr.last_action;
      ret_status |= remove_seq (&anf_ptr, &end_ptr, &chain_anz, vk_ptr, vk_ptr);
      ret_status |= delete_seq (anf_ptr, end_ptr, chain_anz);
   } /* Es gibt Ketten in dieser VK */
   return (ret_status);
} /* clear_vk */ 



/* ============================== delete_ak_mem ============================= */
GSTATE delete_ak_mem (void)
{
   CHAIN_ELEM_PTR_TYPE  ch_ptr;
   GSTATE               ret_status;

   /* delete_ak_mem */
   ret_status  = clear_vk (AK, BASIS_AK_GUETE);    /* Loeschung der Basis-AKs */
   ret_status |= clr_and_reorg (&ch_ptr, AK);      /* Loeschung des gesamten  */
   ret_status |= delete_chain (&ch_ptr);           /*   Kettenspeichers       */
   reset_rel_aks ();
   return (ret_status);
} /* delete_ak_mem */



/* =============================== delete_mem =============================== */
GSTATE delete_mem (void)
{
   GSTATE  ret_status;
  
   /* delete_mem */
   ret_status = delete_ak_mem ();
   return (ret_status);
} /* delete_mem */



/* =============================== check_bak ================================ */
GSTATE check_bak (BOOL *ak_exists,
                  BOOL *ak_is_bak,
                  INT   guete,
                  INT   lfd_nr)
{
   CHAIN_ELEM_PTR_TYPE  ak_ptr;
   GSTATE               res_state = GLEAM_OK;

   /* check_bak */
   if (get_chain (&ak_ptr, &lfd_nr, AK, guete, &guete, &res_state))
   { /* Es gibt diese Kette */
      *ak_exists = TRUE;
      *ak_is_bak = ak_ptr->u_el.u_xel.u_hdr.chain_status & BASIS_AK;
   } /* Es gibt diese Kette */
   else
   { /* Kette existiert nicht */
      *ak_exists = FALSE;
      *ak_is_bak = FALSE;
   } /* Kette existiert nicht */
   return (res_state);
} /* check_bak */



/* ============================== do_bak_to_ak ============================== */
GSTATE do_bak_to_ak (CHAIN_ELEM_PTR_TYPE  ak_ptr)
{
   CHAIN_ELEM_PTR_TYPE  vk_ptr, pred_ptr, item_ptr;
   GSTATE               res_state = GLEAM_OK;
   BOOL                 found;
  
   /* do_bak_to_ak */
#ifdef CHECKS
   if (ak_ptr == NULL)
      return (fehler (MEN_GSW, 12, NO_INFO, NO_INFO, "", NIL_PTR));
   if (ak_ptr->u_el.activity != AK_HEADER)
      return (fehler (MEN_GSW, 13, NO_INFO, NO_INFO, "", AK_EXPECTED));
#endif
   if (ak_ptr->u_el.u_xel.u_hdr.chain_status & BASIS_AK)
   { /* Basis-AK -> AK */
      ak_ptr->u_el.u_xel.u_hdr.chain_status &= ~BASIS_AK;
      pred_ptr = get_vk (AK, BASIS_AK_GUETE, &res_state);
      vk_ptr   = pred_ptr;
      item_ptr = pred_ptr->u_el.next_action;
      found = FALSE;
      while (!found && (item_ptr != NULL))
      { /* AK-Eintrag in Basis-AK-VK suchen */
         if (item_ptr->u_el.u_xel.u_sub.sub_chain == ak_ptr)
            found = TRUE;
         else
         {
            pred_ptr = item_ptr;
            item_ptr = item_ptr->u_el.next_action;
         }
      } /* AK-Eintrag in Basis-AK-VK suchen */
      if (found)
      { /* AK-Eintrag aus der Basis-AK-VK loeschen */
         res_state |= remove_next_item (&item_ptr, vk_ptr, pred_ptr);
         res_state |= delete_item (item_ptr);
      } /* AK-Eintrag aus der Basis-AK-VK loeschen */
      else
         res_state |= fehler (MEN_GSW, 14, ak_ptr->u_el.u_xel.u_hdr.guete, 
                              ak_ptr->u_el.u_xel.u_hdr.lfd_nr, "", 
                              BAK_NOT_FOUND);
   } /* Basis-AK -> AK */
   return (res_state);
} /* do_bak_to_ak */



/* =============================== del_chain ================================ */
GSTATE del_chain (BOOL *chain_deleted,
                  INT   ketten_art, 
                  INT   guete, 
                  INT   lfd_nr, 
                  BOOL  del_bak)
{
   CHAIN_ELEM_PTR_TYPE  vk_ptr, item_ptr, pred_ptr;
   GSTATE               res_state = GLEAM_OK;
  
   /* del_chain */
   *chain_deleted = FALSE;
#ifdef CHECKS
   if ((guete < 0) || (MAX_GUETE < guete))
      return (fehler (MEN_GSW, 15, guete, NO_INFO, "", MEINE_GUETE));
   if ((ketten_art < AK) || (AK < ketten_art))
      return (fehler (MEN_GSW, 16, ketten_art, NO_INFO, "", BAD_CHAIN_KIND));
   if (del_bak && (ketten_art != AK))
      return (fehler (MEN_GSW, 17, NO_INFO, NO_INFO, "", AK_EXPECTED));
#endif
   vk_ptr = get_vk (ketten_art, guete, &res_state);
   if (count_out_item (&pred_ptr, vk_ptr, lfd_nr-1, &res_state))
   { /* Vorgaenger des Verwaltungs-Items ermittelt */
      item_ptr = pred_ptr->u_el.next_action;
      if (item_ptr != NULL)
      { /* Es gibt das Verwaltungsitem zu dieser Kette */
         if (del_bak)
         { /* Loesche Basis-AK */
            if (item_ptr->u_el.u_xel.u_sub.sub_chain->
                  u_el.u_xel.u_hdr.chain_status & BASIS_AK)
               res_state |= do_bak_to_ak (item_ptr->u_el.u_xel.u_sub.sub_chain);
         } /* Loesche Basis-AK */
         if (item_ptr->u_el.u_xel.u_sub.sub_chain->u_el.u_xel.
             u_hdr.ref_counter == 1)
         { /* Die Kette kann geloescht werden */
            update_ch_adr (item_ptr->u_el.next_action);
            res_state |= remove_next_item (&item_ptr, vk_ptr, pred_ptr);
            res_state |= delete_item (item_ptr);
            *chain_deleted = res_state == GLEAM_OK;
            if (ketten_art == AK)
               incr_rel_aks (1);
         } /* Die Kette kann geloescht werden */
      } /* Es gibt das Verwaltungsitem zu dieser Kette */
   } /* Vorgaenger des Verwaltungs-Items ermittelt */
   return (res_state);
} /* del_chain */ 



/* =============================== bacha_init =============================== */
void bacha_init (void)
{
   INT  ii, ij;

   /* BACHA_INIT/BACHA */ 
   for (ij = AK;  ij <= AK;  ij++)                /* Init. z.Zt. nur fuer AK  */
      for (ii = 0;  ii <= MAX_GUETE;  ii++)
         init_chain (&(chain_mem[ij][ii]), VK_HEADER);
   for (ii = 0;  ii <= MAX_GUETE;  ii++)          /* Init. z.Zt. nur fuer AK  */
      chain_mem[RK][ii] = NULL;
   for (ii = 0;  ii <= MAX_GUETE;  ii++) 
      chain_mem[SONST][ii] = NULL;
} /* BACHA_INIT */


