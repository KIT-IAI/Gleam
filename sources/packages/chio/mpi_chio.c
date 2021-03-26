/*******************************************************************************
GLEAM/AE                                                                  V1.0.0

                               G L E A M  /  A E
  General Learning Evolutionary Algorithm and Method / Application Environment

                           Package: chio
                            Module: MPI_CHain-I/O
 
Package: chio                                                   File: mpi_chio.c
--------------------------------------------------------------------------------

Der Modul erledigt die Verwaltung unerledigter Send-Aufrufe samt deren Puffer,
die ein- und mehrfache Versendung einer AK und den Empfang von AK-Messages.

AK-Versendung:
Da mit nichtblockierendem Senden gearbeitet wird, muessen sowohl die Versendung 
selbst als auch der zugehoerige Sendepuffer verwaltet werden. Dabei kann es 
durchaus vorkommen, dass eine AK an mehrere Nachbarn zu versenden ist und damit 
der Sendepuffer mehrfach verwendet werden kann. Das bedeutet, dass die Freigabe 
eines solchen Puffers von der Erledigung aller zugehoerigen Sende-Operationen 
abhaengt. Es muss also pro Puffer eine Liste der zugeordneten Sende-Operationen 
geben.
Fuer den symmetrischen Ring kann im Extremfall, d.h. ein Individuum pro Knoten, 
die Nachricht ein und derselben AK an maximal alle MAX_ANZ_NACHBARN Nachbarn 
gehen. Im anderen Extremfall, d.h. mindesten MAX_ANZ_NACHBARN AKs pro Knoten, 
koennen in einer Generation der Subpopulation MAX_ANZ_NACHBARN unterschiedliche 
AKs an Nachbarn versendet werden, was ebenso viele Puffer erfordert. Zwischen 
der Versendung unterschiedlicher AKs erfolgt jeweils die Bearbeitung einer 
Paarung. Daher kann angenommen werden, dass die Bearbeitung des AK-Empfangs 
genuegend schnell erfolgt, so dass eine Liste der Laenge MAX_ANZ_NACHBARN 
ausreicht.
Die Liste heisst "openSendList" und besteht aus folgenden Daten pro Eintrag:
  - ein Pufferlink
  - eine Liste noch offener Sendeoperationen der Laenge MAX_ANZ_NACHBARN
  - die aktuelle Anzahl noch offener Sendeoperationen "openSendsCount".
Die Eintraege in der "openSendList" werden je nach Abarbeitung der Uebertragung 
frei, also nicht in der Reihenfolge der Versendung oder aehnliches. Sobald ein 
Eintrag frei wird, wird der letzte Eintrag an die Stelle des frei werdenden 
kopiert und der Zaehler noch offener Sendeoperationen "openSendsCount" dekremen-
tiert. Zur schnelleren Bearbeitung werden die Eintraege als Zeiger implementiert
und es erfolgt eine Vertauschung beider Elemente. 
Dazu ein Beispiel mit belegten (a,b,c,d) Eintraegen und einem frei werdenden (f):

vorher:                                   nachher:
 +---+---+---+---+---+---+     +---+       +---+---+---+---+---+---+     +---+
 | a | f | b | c | d | - | ... | - |       | a | d | b | c | f | - | ... | - |
 +---+---+---+---+---+---+     +---+       +---+---+---+---+---+---+     +---+
  ^                   ^                     ^               ^
  0                   openSendsCount = 5    0               openSendsCount = 4

Die Groesse der "openSendList" bestimmt die maximale Anzahl parallel ver-
sendbarer unterschiedlicher AKs. Wenn die Liste nicht ausreicht, muss eine vor-
gegebene Zeit auf die Beendigung von Kommunikationen gewartet werden, bevor 
die Situation als fataler Fehler gewertet wird. Daher ist es interessant, die 
maximale Belegung zu ermitteln und ins Logfile zu schreiben.

Die Groesse eines Sendepuffers haengt immer von der konkreten AK ab, da die 
Anzahl der Segmente und damit der zu uebertragenden Segmentdescriptoren schwan-
ken kann. Ausserdem werden im Gegensatz zur File-I/O nur die tatsaechlich ver-
wendeten Daten der Kettenelemente ("u_el") uebertragen. Daher sind Sendepuffer 
immer wieder neu zu allokieren und nach Gebrauch freizugeben. Aehnliches gilt 
fuer die Empfangspuffer mit der Ausnahme des Empfangspuffers der MsgM2S_InitAK-
Nachricht. Das besondere an dieser Nachricht ist, dass die empfangene AK sofort 
und unveraendert an die Nachbarknoten weiter gesendet wird. Daher kann der 
Empfangspuffer als Sendepuffer weiter genutzt werden, siehe "recSendInitAK".

Auf den jeweils aktuellen Puffer zeigt die package-globale Variable "akPuffer" 
mit "akPufferLen". Die Routinen "read_..." und "write_..." aus Modul "chain_io"
greifen auf diese Variablen zu.


Die Routinen im einzelnen:

BOOL  sendeAK (CHAIN_ELEM_PTR_TYPE  akPtr,
               INT                  msgCode,
               INT                  indIdx,
               INT                  recAnz,
	       INT                 *ReceiverList);
        Die Routine allokiert einen Sendepuffer fuer die AK und schreibt die AK-
        Daten mit Hilfe von "open_ch_file" und "save_chain" aus Modul "chain_io"
        in den Sendepuffer. Zuvor wird der "indIdx" in "parIndIdx" im AK-Kopf
        eingetragen. Die AK wird mit Hilfe von "sendAKMsg" versandt.
        Wenn dabei ein Fehler auftrat, liefert die Routine FALSE, setzt den 
        "err_state" und erzeugt Fehlermeldungen.

BOOL  recSendInitAK (DOUBLE *note,
                     INT     laenge);
        Die Routine empfaengt die Nachricht MsgM2S_InitAK des Masters mit der 
        angegebenen "laenge" mit Hilfe von "recAKMsg". Die so erhaltenen AK wird 
        unter ihrem "indIdx" in der Populationsliste gespeichert. Die Note wird
        in "note" zurueckgeliefert.
        Mit Hilfe des "indIdx" wird auch die Liste der Knoten benachbarter Indi-
        viduen "nbKNrList" bestimmt, an die die AK als MsgS2S_UpdateNbAK zu 
        versenden ist. Dazu wird der Empfangspuffer weiter verwendet, so dass 
        "sendAKMsg" unmittelbar nach Empfang und Eintragung der AK aufgerufen 
        werden kann.
        Wenn dabei ein Fehler auftrat, liefert die Routine FALSE, setzt den 
        "err_state" und erzeugt Fehlermeldungen.

BOOL  recAKMsg (CHAIN_ELEM_PTR_TYPE *akPtr,
                INT                 *indIdx,
	        INT                  laenge,
                INT                  msgCode,
	        INT                  sender);
        Die Routine empfaengt die durch "msgCode" angegebene Nachricht vom Ab-
        sender "sender". Wenn der package-globale "akPuffer" NULL ist, wird er 
        mit der Laenge "laenge" angelegt. 
        Daraus wird mit Hilfe der Routinen "open_ch_file" und "read_chain" aus
        Modul "chain_io" eine AK gebildet, die in "akPtr" abgeliefert wird. 
        In "indIdx" wird ihr Index zurueckgeliefert. Der "akPuffer" wird frei-
        gegeben; es sei denn, eine MsgM2S_InitAK wurde empfangen, dann wird er 
        noch zum Versenden als Nachbar-AK von der aufrufenden Routine 
        "recSendInitAK" benoetigt.
        Wenn dabei ein Fehler auftrat, liefert die Routine FALSE, setzt den 
        "err_state" und erzeugt Fehlermeldungen.

void   logMaxParMSGCount (void);
        Schreibt eine Meldung mit "maxOpenMsgs" ins Logfile.

void   freeAKBuffers (void);
        Die Routine entfernt alle nicht mehr benoetigten Eintraege in der 
        "openSendList" und beseitigt dabei entstehende Luecken in den Listen.


Package-lokaler Export:

BOOL initMPI_Chio (void);
        Legt die Datenstrukturen des Modules an und liefert TRUE, wenn alles 
        gut ging.


--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0 vom 22.01.2013:
 1. Renovierung: Testweisen "char_dialog"-Aufruf angepasst.          WJ 09.05.16
 2. Anpassung des Aufrufs von "write_to_log".                        WJ 19.12.16
--------------------------------------------------------------------------------
Status: OK                Autor: W.Jakob / D.Knoell            Datum: 19.12.2016
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mpi.h"

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "chaindef.h"
#include "mPar.h"
#include "hmod.h"
#include "evo.h"
#include "chio.h"
#include "chioInt.h"

#include CHIO_TEXTS


/* Schalter AKBUF_TST in "chioInt.h" */
#undef DBG_AUSG
#ifdef GLO_CHECKS
  #define CHECKS
#endif



/* ========================== Exportierte Variable: ========================= */



/* ======================== Package-globale Variable: ======================= */
DOUBLE  *akPuffer;            /* aktueller Puffer zur AK-Versendung           */
INT      akPufferIdx,         /* Beginn des freie Bereichs im akPuffer        */
         akPufferLen;         /* Laenge des akPuffer                          */



/* ============================ Lokale Konstante: =========================== */
#define WAIT_TIME     20        /* Wartezeit zwischen Sendeversuchen in msec  */
#define WAIT_TRIALS  100        /* maximale Anzahl an Warteversuchen          */



/* ======================== Lokale Typdeklarationen: ======================== */
typedef struct OPEN_SEND_LIST_ELEM_TYPE 
  { DOUBLE      *sendBuffer;                     /* Sendepuffer               */
    MPI_Request  openSendOps [MAX_ANZ_NACHBARN]; /* Handle offener Sendeops   */
    BOOL         sendOpOpen  [MAX_ANZ_NACHBARN]; /* Flag f. offene Sendeops   */
    INT          actOpenSendCount,               /* Akt.Anz.offener SendeOps  */
                 orgOpenSendCount;               /* urspr.Anz.offener SendeOps*/
  } OPEN_SEND_LIST_ELEM_TYPE;



/* ============================ Lokale Variable: ============================ */
static OPEN_SEND_LIST_ELEM_TYPE *openSendList [MAX_ANZ_NACHBARN];
static INT                       maxOpenMsgs,    /* groesste Pufferbelegung   */
                                 orgManagedMsgsCount; /* Anzahl anfaengl.verw.*/
                                                      /* Msgs in openSendList */
#if defined (DBG_AUSG) || defined (AKBUF_TST)
static STRING_TYPE  txtbuf;
#endif /* DBG-Ausgaben */



/* ============================ Lokale Routinen: ============================ */
static BOOL sendAKMsg        (INT  msgCode,
                              INT  recAnz,
                              INT *receiverList);

static INT  calcBufferLen    (CHAIN_ELEM_PTR_TYPE  akPtr);

#ifdef AKBUF_TST
   static void dumpOpenSendList (void);
#else /* kein AKBUF_TST */
   static void belegeEintrag (INT  freeElemIdx,
		              INT  anzahl);
#endif /* kein AKBUF_TST */




/* ================================ sendeAK ================================= */
BOOL  sendeAK (CHAIN_ELEM_PTR_TYPE  akPtr,
               INT                  msgCode,
               INT                  indIdx,
               INT                  recAnz,
               INT                 *ReceiverList)
{
   INT   akSize;
   BOOL  erg;

   /* sendeAK */
#ifdef CHECKS
   if (akPtr == NULL) {
      err_state = fatal (CHIO_MPI, 1, NO_INFO, NO_INFO, "", NIL_PTR);
      return (FALSE);
   }
   if (recAnz > MAX_ANZ_NACHBARN) {
      err_state = fatal (CHIO_MPI, 2, recAnz, NO_INFO, "", TOO_MANY_REC);
      return (FALSE);
   }
#endif /* CHECKS */

   akPtr->u_el.u_xel.u_hdr.parIndIdx = indIdx;
   akPufferLen = calcBufferLen (akPtr);
   akPuffer    = (DOUBLE*)malloc(akPufferLen * sizeof(DOUBLE));
   if (akPuffer == NULL) {
      err_state = fatal (CHIO_MPI, 3, NO_INFO, NO_INFO, "sendeAK", NO_MEM_ERR);
      return (FALSE);
   }
   erg = FALSE;
   if (open_ch_file ("", 1, CH_WRITE, SELECTIVE_SAVE, MPI2_IO))
      if (save_chain (akPtr, FALSE)) {
	 erg = sendAKMsg(msgCode, recAnz, ReceiverList);
      }
   return (erg);
} /* sendeAK */



/* ============================= belegeEintrag ============================== */
#ifdef AKBUF_TST
          void belegeEintrag (INT  freeElemIdx,
	   	              INT  anzahl)
#else /* kein AKBUF_TST */
   static void belegeEintrag (INT  freeElemIdx,
		              INT  anzahl)
#endif /* kein AKBUF_TST */
/*----------------------------------------------------------------------------*/
/*   Die Routine vollzieht die Belegung eines Sendeeintrags in der            */
/*   "openSendList".                                                          */
/*   Sie steht zu Testzwecken package-lokal zur Verfuegung. Ansonsten ist sie */
/*   modul-lokal.                                                             */
/*----------------------------------------------------------------------------*/
{
   INT  ii;

   /* belegeEintrag */
   for (ii = 0;  ii < anzahl;  ii++) 
      openSendList[freeElemIdx]->sendOpOpen[ii] = TRUE;  /* mark sends        */
   openSendList[freeElemIdx]->actOpenSendCount = anzahl;
   openSendList[freeElemIdx]->orgOpenSendCount = anzahl;
   if (freeElemIdx == orgManagedMsgsCount)
      orgManagedMsgsCount++;
   if (orgManagedMsgsCount > maxOpenMsgs)                /* maximale Belegung */
      maxOpenMsgs = orgManagedMsgsCount;                 /* ermitteln.        */
#ifdef AKBUF_TST
   printf ("belegeEintrag: Eintrag mit Idx=%d belegt.\n", freeElemIdx);
   dumpOpenSendList ();
#endif /* AKBUF_TST */
} /* belegeEintrag */



/* =============================== sendAKMsg ================================ */
static BOOL sendAKMsg (INT  msgCode,
                       INT  recAnz,
                       INT *receiverList)
/*----------------------------------------------------------------------------*/
/*   Die Routine entfernt durch den Aufruf von "freeAKBuffers" zunaechst alle */
/*   nicht mehr benoetigten Eintraege in der "openSendList" und belegt mit    */
/*   Hilfe von "belegeEintrag" einen freien. Wenn keiner frei ist, wird bis   */
/*   zu WAIT_TRIALS mal WAIT_TIME msec auf die Beendigung einer Sendeoperation*/
/*   gewartet. Negativenfalls liefert die Routime FALSE setzt den "err_state" */
/*   und erzeugt einen fatalen Fehler.                                        */
/*   Die im package-globalen "akPuffer" enthaltene AK wird an alle Empfaenger */
/*   der "receiverList" als "msgCode"-Nachricht versandt. Die dabei erhalte-  */
/*   nen Handles und der "akPuffer" werden in die "openSendOps"-Liste des     */
/*   Pufferlisteneintrags eingetragen. "akPuffer" wird auf NULL gesetzt.      */
/*   Wenn alles gut ging, liefert die Routine TRUE. Andernfalls nach Erzeu-   */
/*   gung der entsprechenden Fehlermeldungen FALSE.                           */
/*----------------------------------------------------------------------------*/
{   
   INT   waitCount,          /* Zaehler zum Warten auf Beendigung             */
         freeElemIdx,        /* Index des freien Eintrags                     */
         ii;
   BOOL  noEmptyElem,        /* TRUE, wenn kein freier Eintrag                */
         erg;                /* TRUE, wenn Message versendet wurde            */

   /* sendAKMsg */
   erg       = FALSE;
   waitCount = WAIT_TRIALS;
#ifdef DBG_AUSG
   printf ("\nsendAKMsg KNr=%d: Msg=%d AKIdx=%d  %d Rec:", kNr, msgCode, 
	   i_round(akPuffer[9]), recAnz);
   for (ii = 0;  ii < recAnz;  ii++)
      printf (" %d", receiverList[ii]);
   printf ("\n");
#endif /* DBG_AUSG */
   do 
   { /* solange, bis freien Eintrag gefunden oder TMO */
      noEmptyElem = !freeAKBuffers (&freeElemIdx);
      if (noEmptyElem) {
	 warte (WAIT_TIME);
	 waitCount--;
#ifdef AKBUF_TST
	 printf ("sendAKMsg *** %s wartet auf Pufferfreigabe: waitCount=%d\n", 
		 nodeBuf, waitCount);
#endif /* AKBUF_TST */
      }
   } /* solange, bis freien Eintrag gefunden oder TMO */
   while ((waitCount > 0) && noEmptyElem);

   /* ------ Belegung des freien Eintrags und Versendung der Nachricht ------ */
   if (noEmptyElem) /* keinen freien Eintrag gefunden? */
      err_state = fatal (CHIO_MPI, 10, NO_INFO, NO_INFO, "", TOO_MANY_PAR_AK_MSGS);
   else
   { /* Freier Eintrag gefunden: Message Senden und Eintrag belegen */
      for (ii = 0;  ii < recAnz;  ii++) /* versenden und Handles speichern */
	 MPI_Isend (akPuffer, akPufferLen, MPI_DOUBLE, receiverList[ii], msgCode, 
		    MPI_COMM_WORLD, &(openSendList[freeElemIdx]->openSendOps[ii]));
      openSendList[freeElemIdx]->sendBuffer = akPuffer; /* Sendepuffer merken */
      akPuffer = NULL;
      erg      = TRUE;
      belegeEintrag (freeElemIdx, recAnz);
#ifdef AKBUF_TST
      sprintf (txtbuf, "sendAKMsg: %d.Element von %d verwalteten belegt. ",
	       freeElemIdx, orgManagedMsgsCount);
      if (!master) {
	 strcat (txtbuf, "Resultierende openSendList:");
	 write_to_log (txtbuf, KEEP_LOG_OPEN);
	 dumpOpenSendList ();
      } 
      else 
	 write_to_log (txtbuf, KEEP_LOG_OPEN);
      write_to_log ("\n", CLOSE_LOG);
#endif /* AKBUF_TST */
   } /* Freier Eintrag gefunden: Message Senden und Eintrag belegen */
   return (erg);
} /* sendAKMsg */



/* ============================== recSendInitAK ============================= */
BOOL recSendInitAK (DOUBLE *note,
                    INT     laenge)
{
   CHAIN_ELEM_PTR_TYPE  akPtr;
   INT                  akIdx;
   BOOL                 erg;

   /* recSendInitAK */
   erg = FALSE;
   if (recAKMsg (&akPtr, &akIdx, laenge, MsgM2S_InitAK, MASTER))
   { /* Init-AK empfangen */
      if ((akIdx < firstIndIdx) || (lastIndIdx < akIdx)) 
	 err_state = fatal (CHIO_MPI, 20, akIdx, kNr, "", BAD_IND_IDX);
      else 
      { /* AK gehoert zum Slave: Eintragen und weiter versenden */
	 *note = akPtr->u_el.u_xel.u_hdr.fitness_note;
	 pop_feld[akIdx].chain_ptr = akPtr;
	 pop_feld[akIdx].note      = *note;
	 pop_feld[akIdx].guete     = akPtr->u_el.u_xel.u_hdr.guete;
	 pop_feld[akIdx].new_chain = FALSE;        /* d.h., AK ist vorgegeben */
	 akPtr->u_el.u_xel.u_hdr.ref_counter = 1;
	 if (nbKNrListLen[akIdx])
	    erg = sendAKMsg (MsgS2S_UpdateNbAK, nbKNrListLen[akIdx], nbKNrList[akIdx]);
	 else { /* Indiv hat keine Nachbarn */
	    free (akPuffer);
	    akPuffer = NULL;
	    erg      = TRUE;
	 } /* Indiv hat keine Nachbarn */
      } /* AK gehoert zum Slave: Eintragen und weiter versenden */
#ifdef letzterTest
      printf ("recSendInitAK: AK mit Index=%d und Note=%g erhalten.\n", 
	      akIdx, akPtr->u_el.u_xel.u_hdr.fitness_note);
      nbKNrListLen[akIdx] = 1;
      nbKNrList[akIdx][0] = 0;
      erg = sendAKMsg (MsgS2S_UpdateNbAK, nbKNrListLen[akIdx], nbKNrList[akIdx]);
      printf ("recSendInitAK: AK-Versendung: %d\n", erg);
#endif /* letzterTest */
   } /* Init-AK empfangen */
   return (erg);
} /* recSendInitAK */



/* ================================ recAKMsg ================================ */
BOOL recAKMsg (CHAIN_ELEM_PTR_TYPE *akPtr,
               INT                 *indIdx,
               INT                  laenge,
               INT                  msgCode,
               INT                  sender)
{
   MPI_Status  status;
   INT         istLen;
   BOOL        erg;

   /* recAKMsg */
   if (akPuffer == NULL) 
      if ((akPuffer = (DOUBLE*)malloc(laenge * sizeof(DOUBLE))) == NULL) {
	 err_state = fatal (CHIO_MPI, 30, NO_INFO, NO_INFO, "AK-Empfang", NO_MEM_ERR);
	 return (FALSE);
      }
   akPufferLen = laenge;
   erg         = FALSE;
   MPI_Recv (akPuffer, akPufferLen, MPI_DOUBLE, sender, msgCode, MPI_COMM_WORLD, &status);
   MPI_Get_count(&status, MPI_DOUBLE, &istLen);
   if (open_ch_file ("", 1, CH_READ, SELECTIVE_SAVE, MPI2_IO))
      if (read_chain (akPtr, CH_START))
      { /* AK erfolgreich restauriert */
	 *indIdx = (*akPtr)->u_el.u_xel.u_hdr.parIndIdx;
	 if (msgCode != MsgM2S_InitAK) { /* Puffer wird bei MsgM2S_InitAK noch gebraucht */
	    free (akPuffer);
	    akPuffer = NULL;
	 } /* Puffer wird bei MsgM2S_InitAK noch gebraucht */
	 erg = TRUE;
      } /* AK erfolgreich restauriert */
      else
	 err_state = fatal (CHIO_MPI, 31, NO_INFO, NO_INFO, "", AK_EMPFANGSFEHLER);
#ifdef DBG_AUSG
   printf ("recAKMsg KNr=%d: Msg=%d von %d  AKIdx=%d\n", kNr, msgCode, sender, *indIdx);
#endif /* DBG_AUSG */
   return (erg);
} /* recAKMsg */



/* ============================= calcBufferLen ============================== */
static INT calcBufferLen (CHAIN_ELEM_PTR_TYPE  akPtr)
/*----------------------------------------------------------------------------*/
/*  Die Routine liefert die Groesse eines Realfeldes ab, in das die Elemente  */
/*  und Descriptorrecords der AK gespeichert werden koennen.                  */
/*----------------------------------------------------------------------------*/
{
   CHAIN_ELEM_PTR_TYPE  elemPtr;
   INT  segmEntryCount,   /* Anzahl der zu uebertragenden Segmenteintraege    */
        elemEntryCount,   /* Anzahl der zu uebertragenden Elementeintraege    */
        subChCount,       /* Anzahl der Unter-AKS (ACTIVITY_ACT-Elemente)     */
        elemAnz;          /* Anzahl der AK-Elemente                           */

   /* calcBufferLen */
#ifdef CHECKS
   if (akPtr == NULL) {
      fatal (CHIO_MPI, 100, NO_INFO, NO_INFO, "", NIL_PTR);
      return (0);
   }
#endif /* CHECKS */
   segmEntryCount  = ((akPtr->u_el.u_xel.u_hdr.segment_anz / SEGM_PTR_ANZ) + 1) * SEGM_PTR_ANZ;
   elemEntryCount  = AK_ERG_WERTE_MAX + 10;  /* AK-Kopf + AK-Header-Parameter */
   elemAnz         = akPtr->u_el.u_xel.u_hdr.chain_length;
   elemEntryCount += elemAnz;      /* Platz f.die activity-Codes der Elemente */
   subChCount      = 0;
   elemPtr         = akPtr->u_el.next_action;
   if (dyn_aks)
   { /* AK-Typ 3: AK dynamischer Laenge */
      while (elemPtr != NULL) { /* all along the chain */
	 if (elemPtr->u_el.activity == ACTIVITY_ACT) { /* lokale Sub-Chain */
	    subChCount++;
	    if (elemPtr->u_el.u_xel.u_sub.ref_typ == LOC_SUB_AK)
	       elemEntryCount += calcBufferLen (elemPtr->u_el.u_xel.u_sub.sub_chain);
	 } /* lokale Sub-Chain */
	 else
	    elemEntryCount +=  int_pro_aktion[elemPtr->u_el.activity] +
	                      real_pro_aktion[elemPtr->u_el.activity];
	 elemPtr = elemPtr->u_el.next_action;
      } /* all along the chain */
   } /* AK-Typ 3: AK dynamischer Laenge */
   else
   { /* AK fester Laenge */
      elemEntryCount += i_par_ges_anz + r_par_ges_anz;
      while (elemPtr != NULL) { /* all along the chain */
	 if (elemPtr->u_el.activity == ACTIVITY_ACT)
	    subChCount++;
	 elemPtr = elemPtr->u_el.next_action;
      } /* all along the chain */
   } /* AK fester Laenge */
   elemEntryCount += subChCount * 4;
#ifdef DBG_AUSG
   printf ("calcBufferLen: elemAnz=%d subChCount=%d  elemEntryCount=%d segmEntryCount=%d"
	   " Summe=%d\n", elemAnz, subChCount, 
	   elemEntryCount, segmEntryCount, elemEntryCount+segmEntryCount);
#endif /* DBG_AUSG */
   return (elemEntryCount + segmEntryCount);
} /* calcBufferLen */



/* ============================ logMaxParMSGCount =========================== */
void logMaxParMSGCount (void)
{
   STR_TYPE  buf;

   sprintf (buf, MAX_COUNT_TXT, maxOpenMsgs, orgManagedMsgsCount);
   write_to_log (buf, CLOSE_LOG);
} /* logMaxParMSGCount */



/* ============================== freeAKBuffers ============================= */
BOOL freeAKBuffers (INT *freeElemIdx)
{
   MPI_Status  mpiStatus;
   INT         ii, jj;        /* zur Suche beendeter Sendeauftraege           */
   BOOL        finished,      /* TRUE, wenn Sendeoperation beendet ist        */
               found,         /* TRUE, wenn freier Eintrag gefunden -> FktErg */
               weiter,        /* TRUE, solange actOpenSendCount > 0           */
               finMsgFound;   /* beendete Sendeoperation gefunden             */
   char        ch;

   /* freeAKBuffers */
#ifdef AKBUF_TST
   printf ("freeAKBuffers: Start: orgManagedMsgsCount=%d\n", orgManagedMsgsCount);
   dumpOpenSendList ();
#endif /* AKBUF_TST */

   if (orgManagedMsgsCount == 0) { /* keine Sendeoperationen offen */
      *freeElemIdx = 0;
      return (TRUE);
   } /* keine Sendeoperationen offen */

   /* ----- Beendete SendeOps finden und fertige Versendungen freigeben: ---- */
   finMsgFound = FALSE;
   for (ii = 0;  ii < orgManagedMsgsCount;  ii++) 
   { /* alle Eintraege in der "openSendList" auf beendete SendeOps pruefen */
      if (openSendList[ii]->actOpenSendCount)
      { /* es sind noch SendeOps offen */
	 weiter = TRUE;
	 for (jj = 0;  (jj < openSendList[ii]->orgOpenSendCount) && weiter;  jj++)
	 { /* Schleife ueber die Handle-Liste einer Versendung */
	    if (openSendList[ii]->sendOpOpen[jj]) 
            { /* offene SendeOp gefunden */
#ifdef AKBUF_TST
	       sprintf (txtbuf, "SendeOp [%d, %d] fertig? [J/n]: ", ii, jj);
	       ch = char_dialog (txtbuf);
	       finished = (ch == 'J') || (ch == CR);
#else /* kein AKBUF_TST */
	       MPI_Test (&(openSendList[ii]->openSendOps[jj]), &finished, &mpiStatus);
#endif /* kein AKBUF_TST */
	       if (finished)
               { /* beendete Sendeoperation gefunden */
		  openSendList[ii]->sendOpOpen[jj] = FALSE;
		  openSendList[ii]->actOpenSendCount--;
		  if (openSendList[ii]->actOpenSendCount == 0)
		  { /* letzte SendeOp des Eintrags beendet */
		     free (openSendList[ii]->sendBuffer);   /* Pufferfreigabe */
		     openSendList[ii]->sendBuffer = NULL;
		     weiter      = FALSE; /* akt.Eintrag nicht weiter pruefen */
		     finMsgFound = TRUE;  /* beendete Sendeoperation gefunden */
		  } /* letzte SendeOp des Eintrags beendet */
	       } /* beendete Sendeoperation gefunden */
	    } /* offene SendeOp gefunden */
	 } /* Schleife ueber die Handle-Liste einer Versendung */
      } /* es sind noch SendeOps offen */
   } /* alle Eintraege in der "openSendList" auf beendete SendeOps pruefen */

   /* ----- orgManagedMsgsCount aktualisieren und freien Eintrag finden: ---- */
   if (finMsgFound)          /* wurde mindestens eine Sendeoperation beendet? */
      while ((openSendList[orgManagedMsgsCount - 1]->actOpenSendCount == 0) &&
	     (orgManagedMsgsCount > 1))
	 orgManagedMsgsCount--;      /* Anzahl verwalteter Eintraege anpassen */
   if (orgManagedMsgsCount == 1)
   { /* Sonderfall: nur ein Eintrag */
      found = TRUE;
      if (openSendList[0]->actOpenSendCount == 0)
      { /* keine offenen SendeOperationen mehr in der Liste */
	 orgManagedMsgsCount = 0;
	 *freeElemIdx        = 0;
      } /* keine offenen SendeOperationen mehr in der Liste */
      else         /* der erste Eintrag ist die einzige offene SendeOperation */
	 *freeElemIdx = 1;
   } /* Sonderfall: nur ein Eintrag */
   else
   { /* es werden noch mehrere Eintraege verwaltet */
      found = FALSE;
      for (*freeElemIdx = 0;  (*freeElemIdx < orgManagedMsgsCount) && !found;  (*freeElemIdx)++)
	 found = (openSendList[*freeElemIdx]->actOpenSendCount == 0);
      if (found)        /* im verwalteten Teil der openSendList ist was frei? */
	 (*freeElemIdx)--;                       /* der Index davor war frei! */
      else
	 found = *freeElemIdx < MAX_ANZ_NACHBARN;    /* Liste noch nicht voll */
   } /* es werden noch mehrere Eintraege verwaltet */
#ifdef AKBUF_TST
   printf ("freeAKBuffers: Ende: found=%d  freeElemIdx=%d  orgManaged"
	   "MsgsCount=%d  Resultierende openSendList:\n", found, *freeElemIdx,
	   orgManagedMsgsCount);
   dumpOpenSendList ();
#endif /* AKBUF_TST */
   return (found);
} /* freeAKBuffers */



#ifdef AKBUF_TST
/* ============================ dumpOpenSendList ============================ */
static void dumpOpenSendList (void)
/*----------------------------------------------------------------------------*/
/*  Testausgabe der "openSendList"                                            */
/*----------------------------------------------------------------------------*/
{
   INT  ii, jj;

   /* dumpOpenSendList */
   printf ("openSendList-Verwaltung: orgManagedMsgsCount=%d maxOpenMsgs=%d\n", 
	   orgManagedMsgsCount, maxOpenMsgs);
   printf ("  Daten der openSendList pro Eintrag: act/orgOpenSendCount, Belegtbits \n");
   for (ii = 0;  ii < orgManagedMsgsCount;  ii++) {
      printf ("  %2d: %2d/%2d BelegtBits: ", ii, openSendList[ii]->actOpenSendCount, 
	      openSendList[ii]->orgOpenSendCount);
      for (jj = 0;  jj < openSendList[ii]->orgOpenSendCount;  jj++) {
	 printf ("%1d", openSendList[ii]->sendOpOpen[jj]);
	 if (((jj+1) % 4) == 0) printf (" ");
	 if (((jj+1) % 8) == 0) printf (" ");
      }
      printf ("\n");
   }
   printf ("\n");
} /* dumpOpenSendList */
#endif /* AKBUF_TST */



/* ============================== initMPI_Chio ============================== */
BOOL initMPI_Chio (void)
{
   INT   elemSize,
         ii;
   BOOL  erg;

   /* initMPI_Chio */
   erg                 = TRUE;
   maxOpenMsgs         = 0;
   orgManagedMsgsCount = 0;
   elemSize            = sizeof(OPEN_SEND_LIST_ELEM_TYPE);
   for (ii = 0;  (ii < MAX_ANZ_NACHBARN) && erg;  ii++) 
   { /* Listeneintrag anlegen */
      erg = ((openSendList[ii] = (OPEN_SEND_LIST_ELEM_TYPE*)malloc(elemSize)) != NULL);
      if (erg) { /* "openSendList"-Eintrag angelegt */
	 openSendList[ii]->actOpenSendCount = 0;
	 openSendList[ii]->sendBuffer       = NULL;
	 memset (openSendList[ii]->sendOpOpen, 0, MAX_ANZ_NACHBARN*sizeof(BOOL));
      } /* "openSendList"-Eintrag angelegt */
      else
	 fatal (CHIO_MPI, 100, NO_INFO, NO_INFO, "initMPI_Chio", NO_MEM_ERR);
   } /* Listeneintrag anlegen */

   return (erg);
} /* initMPI_Chio */


