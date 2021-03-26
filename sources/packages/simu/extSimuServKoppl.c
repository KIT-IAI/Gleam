/*****************************************************************************************
GLEAM/AE                                                                            V1.0.0

                                    G L E A M  /  A E
            Genetic LEarning Algorithms and Methods / Application Environment

                            Package: SIMUlator
                             Module: EXTernalSIMUlationSERVices-anKOPPLung
 
Package: simu                        File: extSimuServKoppl.c   
------------------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), version 3, 
see the comment section at the end of the main program files or of the files "schalter.h".
------------------------------------------------------------------------------------------

The module provides an interface to external simulators, which are provided by general 
simulation services. These services are accessed using web-based communication techniques.

The associated ID in the mod- or tsk-Files is: "ExtSimuServ".
It is defined by the constant SIMU_SERV_KENNUNG, which is defined in module "mod_data.c" of 
package "hmod".

The external simulator can be used with different simulation models corresponding to a 
specific workflow. The name of a model is stored in the variable "extSimuModelName", which 
is loaded from the tsk-File according to the entry "simulator model", see SIMU_MOD_NAME_TXT. 
The German equivalent is "Simulatormodell". The model name may be changed
  - manually by the user by editiong the corresponding program parameter,
  - by obtaining a model name list from the simulation services, from which one is selected
  - by loading a new program parameter file (tsk-File).
If it is changed, an attempt is made to stop the current model and to start the new one.
The same holds, if the server domain name is changed manually or by loading a new program 
parameter file.

The following variables are used to store the relevant information of the interface to the 
external simulation services. They are string buffers of length MAX_STRING:
variable name          defined in  type         example
extSimServDomainName   simu        char*        "iai-khalloof.iai.kit.edu"
lastSimServDomainName  simu        char*        "something.iai.kit.edu"
extSimModelName        simu        char*        "OpalDemo"
lastSimModelName       simu        char*        "undefined"  

serverBuffer           webIO       STRING_TYPE  "http://iai-khalloof.iai.kit.edu:8090/opt"
urlComBuf              webIO       STRING_TYPE  "http://iai-khalloof.iai.kit.edu:8090/opt/5/1"


The status of the interface is stored in two variables:
"sim_ok":  Is TRUE, if the communication package was successfully initialized.
"sim_up":  Is TRUE, if the external simulator has provided a task ID  stored in "taskID" 
           and a simulation model was loaded, which results in a valid workflow ID, stored 
           in "modelWFID".

Timeout-Mechanism:
Timeouts are implemented by cyclyc waiting with the fixed wait time of WAIT_FOR_READ (defined 
in simuInt.h) milliseconds. The number of cycles is calculated by using the macro "sec", 
which expands to "* (1000 / WAIT_FOR_READ)". This allows to specify e.g. "2 sec" for a the 
amount of 40 wait cycles. 
The general simulator TMO is given by the "timeout for ext. simulator [sec]" program parameter 
in the tsk-file and stored as wait cycles in "simuResultWaitCycles".


<< Allgemeine Beschreibung der Implementierung >>


Ein bei Programmende noch aktive Kommunikationsschnittstelle zum externen Simulations-
dienst wird durch die Exit-Routine "terminate_ext_simu", die von "init_ext_koppl" 
aktiviert wird, terminiert.
 

List of exported functions:
---------------------------
Package-Export der allgemeinen Schnittstelle zu externen Simulatoren:

BOOL ext_simu_hochfahren (BOOL  mit_fin);
          Wenn der Simulatoranbindung nicht aktiv ist ("sim_up" ist FALSE) oder 
	  ein neues Simulatormodell benutzt werden soll (d.h., dass sich 
          "extSimuModelName" und "lastSimuModelName" unterscheiden), wird das 
          neue Modell durch die Aufrufsequenz getTaskID(), checkModel() und 
          startModel() geladen. Zuvor wird mit terminate_ext_simu() ein altes
          Modell (Workflow) gestoppt, wenn "sim_up" TRUE ist (also ein altes Modell 
          aktiv ist), oder wenn "mit_fin gesetzt ist.
	  Die Routine liefert TRUE, wenn der externe Simulator (wieder) laeuft.

void update_ext_simu_tmo (INT  tmo_zeit_in_sec);
          The number of wait cycles is calculated based on "tmo_zeit_in_sec" and a wait 
          time of WAIT_FOR_READ. The result is stored in "simuResultWaitCycles".

void terminate_ext_simu (void);
          Die Routine prueft, ob die aktuelle Anwendung einen externen Simulator
          benutzt und ob die Kommunikation hochgefahren ist. Wenn ja, wird diese 
          terminiert. "sim_up" wird auf FALSE und "taskID" auf NIX_IDX gesetzt.
          Ueber das Resultat der Terminierung erfolg eine entsprechende Meldung.
          Die Routine wird auch als Exithandler genutzt.


Package-lokale Routinen der allgemeinen Schnittstelle zu externen Simulatoren, wobei 
"restart_ext_sim" und "kleiner_ext_restart" im Kontext der externen Simulatordienste 
entfallen sind:

BOOL do1ExtSimServRun (DOUBLE              *simuResults,
                       CHAIN_ELEM_PTR_TYPE  chrPtr);
          Die Routine fuehrt einen Simulationslauf mit dem Chromosom "chrPtr" 
          aus und liefert TRUE, wenn in "simuResults" gueltige Werte stehen, 
          d.h., die Simulation erfolgreich durchgefuehrt werden konnte. 
          Die aktuelle Anzahl erwarteter Ergebniswerte steht in "akt_anz_roh_erg_werte". 
          Diese Mengenangabe wird beim Laden des MOD-Files durch Modul "mod_data"
	  gesetzt. 
          Bei geloeschtem "sim_up" terminiert die Routine sofort und liefert FALSE.

void reportResultOfModelStart (char *resBuf);
          Das Ergebnis des Starts des Workflows eines (neuen) Modells wird entsprechend 
          des Wertes von "sim_up" ins Logfile un bei gesetztem MITSIMULOG auch ins 
          simu_logfile ausgegeben. Ausserdem steht das Ergebnis in "resBuf".

void ext_simu_recovery (void);
          Die Routine wird von simulator() aus Modul "simu.c im Optimierungskon-
          text aufgerufen, wenn der Simulator nach einer Simulation nicht mehr 
          verfuegbar ist, d.h. dass "sim_up" FALSE ist. Sie fuehrt zyklische  
          Aufrufe von ext_simu_hochfahren() durch bis er entweder erfolgreich 
          gestartet werden konnte oder RECOVER_TRIALS Versuche fehlschlugen. Dabei
	  wird von Versuch zu Versuch laenger gewartet, indem nach jedem erfolg-
          losen Versuch die Wartezeit um RECOVER_DELAY_INCR msec erhoeht wird.

BOOL init_ext_koppl (BOOL *ext_sim_mod_ok);
          Module initialisation and activation of the exit handler terminate_ext_simu().
          In contrast to other external simulators, no model check possible at this point 
          and thus "ext_sim_mod_ok" is set to TRUE, if "mit_ext_simu" is TRUE. The time
          out value for simulation jobs is set according to the task file parameter.
          Returns TRUE, if initListIO() is performed correctly and the time out parameter 
          was set. Returns FALSE otherwise.

------------------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
------------------------------------------------------------------------------------------
Status: impl                          Author: W.Jakob                     Date: 01/10/2018
========================================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "chaindef.h"
#include "hmod.h"
#include "aufg.h"
#include "webIO.h"
#include "simuInt.h"
#include "simu.h"

#include SIMU_TEXTS
#include GLOBAL_TEXTS


#undef SIMU_KOPPL_DBG



/* ============================= Package-globale Variable: ============================ */
char       *inbuffer;                /* Daten vom ExtSimu          (unused) [BUF_LEN+1] */
char       *errbuffer;               /* Fehlermeldungen d.ext.Simu (unused) [BUF_LEN+1] */
LONG        prompt_ctr,              /* Nummer des naechsten erwarteten Prompts         */
            wait_ctr;                /* Zaehlt Wartezyklen. Global fuer "mit_rz"        */
BOOL        ak_bewertet,             /* True, wenn AK erfolgreich bewertet wurde        */
            detailed_iolog = FALSE;  /* Steuert Detailierungsgrad des IO_LOG            */



/* ================================= Lokale Konstante: ================================ */
/* some definitions for the "ext_simu_recovery" function:                               */
#define RECOVER_TRIALS          10    /* number of trials for recovering the ext. simu. */
#define RECOVER_START_DELAY    500    /* initial wait time in msec                      */
#define RECOVER_DELAY_INCR     100    /* wait time increment per unsuccessful recovery  */



/* ================================= Lokale Variable: ================================= */



/* ================================= Lokale Routinen: ================================= */




/* ============================== do_ext_sim ================================ */
BOOL do_ext_sim (DOUBLE *erg_werte,
                 INT    *i_params,
                 DOUBLE *r_params)
{
   fatal (SIMU_ESD_KOPPL, 1, NO_INFO, NO_INFO, GEN_EXT_SIMU_TXT, APPL_NOT_AVAILABLE);
   return (FALSE);
} /* do_ext_sim */



/* ================================= do1ExtSimServRun ================================= */
BOOL do1ExtSimServRun (DOUBLE              *simuResults,
                       CHAIN_ELEM_PTR_TYPE  chrPtr)
{
   STR_TYPE  buf;
   INT       listStatus,           /* result state of the simulation                    */
             numberOfListEntries,  /* number of list entries (number of result vectors) */
             wCtr,                 /* wait cycle counter. not used in this context      */
             ii, gg;
   BOOL      ok;

   /* do1ExtSimServRun */
#ifdef SIMU_KOPPL_DBG
   printf("do1ExtSimServRun: Start: sim_up=%d\n", sim_up);
#endif /* SIMU_KOPPL_DBG */
   ok   = FALSE;
   wCtr = 0;
   if (sim_ok && sim_up)
   { /* simulation service communication active */
      if(writeChrListHeader(1))      /* create new chromosome list header for 1 chr. */
	 if(writeChrToList(chrPtr, 0, 0))
	    ok = sendChrList2OptServ();
	 else
	    fatal(SIMU_ESD_KOPPL, 10, NO_INFO, NO_INFO, CHR_ENTRY_TXT, GENERAL_WRITE_ERR);
      else
	 fatal(SIMU_ESD_KOPPL, 11, NO_INFO, NO_INFO, CHR_LIST_HDR, GENERAL_WRITE_ERR);
      if (ok)
      { /* chromosome list successfully sent */
	 if (waitForResultList(&wCtr))
	 { /* result list received */
	    if (getResultListHeader(&listStatus, &numberOfListEntries)) 
	    { /* Result list header successfully read */
	       if ((listStatus == 0) && (numberOfListEntries == 1))
	       { /* returned result list ok */
		     ok = getNextResults(simuResults, &gg, &ii);
#ifdef SIMU_KOPPL_DBG
		     printf("do1ExtSimServRun: Ergebnisse f"ue"r %d/%d erhalten. ok=%d\n", gg, ii, ok);
#endif /* SIMU_KOPPL_DBG */
	       } /* returned result list ok */
	       else
	       { /* there is something wrong */
		  ok = FALSE;
		  if (listStatus > 0) { /* report bad simulator runs */
		     sprintf(buf, BAD_SIMU_RUNS_TXT, numberOfListEntries, listStatus);
		     write_to_log(buf, CLOSE_LOG);
		  } /* report bad simulator runs */
		  if (numberOfListEntries != 0) { /* report bad number of entries */
		     if (numberOfListEntries > 1)
			sprintf(buf, TOO_MANY_RESLI_TXT, numberOfListEntries);
		     else
			sprintf(buf, BAD_RESLI_AMOUNT_TXT, numberOfListEntries);
		     write_to_log(buf, CLOSE_LOG);
		  } /* report bad number of entries */
		  fehler(SIMU_ESD_KOPPL, 13, NO_INFO, NO_INFO, RES_DATA_ENTRY_TXT, GENERAL_READ_ERR);
	       } /* there is something wrong */
	    } /* Result list header successfully read */
	    else
	       ok = FALSE;
	 } /* result list received */
	 else 
	    ok = (fehler(SIMU_ESD_KOPPL, 14, (LONG)((simuResultWaitCycles * WAIT_FOR_READ) / 1000), 
			 NO_INFO, RES_LIST_TXT, TMO_READ_ERR) == GLEAM_OK);
      } /* chromosome list successfully sent */
   } /* simulation service communication active */
   if (!ok)
      sim_up = FALSE;
   return (ok);
} /* do1ExtSimServRun */



/* ============================= reportResultOfModelStart ============================= */
void reportResultOfModelStart (char *resBuf)
{
   sprintf(resBuf, WORKFLOW_START_RES_TXT, extSimuModelName);
   if (sim_up) 
   { /* simulation model ready */
      strcat(resBuf, WF_ACTION_DONE_TXT);
      sprintf(simuLogBuf, "\n%s %s %s", STRICH_15, resBuf, STRICH_15);
      write_to_log(simuLogBuf, KEEP_LOG_OPEN);
#ifdef MITSIMULOG
      write_to_simulog(simuLogBuf, KEEP_LOG_OPEN);
#endif /* MITSIMULOG */
      sprintf(simuLogBuf, STATUS_ANZG_ESS6_TXT, urlComBuf);
   } /* simulation model ready */
   else
   { /* simulation model not ready */
      strcpy(lastSimuModelName, "");                       /* clear "lastSimuModelName" */
      strcat(resBuf, WF_ACTION_FAILED_TXT);
      sprintf(simuLogBuf, "\n%s %s %s", STRICH_15, resBuf, STRICH_15);
   } /* simulation model not ready */
   write_to_log(simuLogBuf, CLOSE_LOG);
#ifdef MITSIMULOG
   write_to_simulog(simuLogBuf, CLOSE_LOG);
#endif /* MITSIMULOG */
} /* reportResultOfModelStart */



/* =============================== ext_simu_hochfahren ================================ */
BOOL ext_simu_hochfahren (BOOL  mit_fin)
{
   STRING_TYPE  buffer;
   BOOL         newModel, modelKnown;
         
   /* ext_simu_hochfahren */
   if (strlen(extSimuModelName) > 0)
   { /* the model name contains something */
      newModel = strcmp(extSimuModelName, lastSimuModelName) != 0;
#ifdef SIMU_KOPPL_DBG
      printf ("ext_simu_hochfahren: Start: sim_ok=%d sim_up=%d mit_fin=%d new_model=%d\n", 
	      sim_ok, sim_up, mit_fin, newModel);
#endif /* SIMU_KOPPL_DBG */
      if (newModel || !sim_up || mit_fin)
      { /* (re)start ext. simu. with present model */
	 if (sim_up || mit_fin)
         { /* terminate ext.simu.services: task and  workflow */
	    sim_up = TRUE;
	    terminate_ext_simu();                                /* sets "sim_up" to FALSE */
	 } /* terminate ext.simu.services: task and  workflow */
	 if (getTaskID())
         { /* taskID received */
	    modelKnown = FALSE;
	    checkModel(&modelKnown, extSimuModelName);
	    if (modelKnown)
	    { /* model name is known */
	       if (startModel(extSimuModelName))
	       { /* workflow of present model is ready */
		  strcpy(lastSimuModelName, extSimuModelName);  /* store actual model name */
		  sim_up = TRUE;
	       } /* workflow of present model is ready */
	    } /* model name is known */
	 } /* taskID received */
	 reportResultOfModelStart(buffer);
#ifdef GLEAM_USR
	 write_buf(get_line_anz()+1, buffer);
#endif /* GLEAM_USR */
      } /* (re)start ext. simu. with present model */
   } /* the model name contains something */
#ifdef GLEAM_USR
   else 
   { /* empty model name */
      write_buf(get_line_anz()+1, NO_MODEL_NO_START_TXT);
      if (sim_up)
	 strcpy(extSimuModelName, lastSimuModelName);   /* the last name is still valid */
   } /* empty model name */
#endif /* GLEAM_USR */
#ifdef SIMU_KOPPL_DBG
   printf ("ext_simu_hochfahren: Ende: sim_ok=%d sim_up=%d simuResultWaitCycles=%d\n", 
	   sim_ok, sim_up, simuResultWaitCycles);
#endif /* SIMU_KOPPL_DBG */
   return (sim_up);
} /* ext_simu_hochfahren */



/* ================================ update_ext_simu_tmo =============================== */
void update_ext_simu_tmo (INT  tmo_zeit_in_sec)
{                                /* sec expands to "* (1000 / WAIT_FOR_READ)" */
   simuResultWaitCycles = l_round (tmo_zeit_in_sec sec);
} /* update_ext_simu_tmo */



/* =============================== terminate_ext_simu ================================= */
void terminate_ext_simu (void)
{
   STR_TYPE  buf;
   BOOL      ok;

   /* terminate_ext_simu */
#ifdef SIMU_KOPPL_DBG
   printf ("terminate_ext_simu: Start: sim_ok=%d sim_up=%d mit_ext_simu=%d\n", sim_ok, sim_up, mit_ext_simu); 
   fflush(stdout);
#endif /* SIMU_KOPPL_DBG */
   if (sim_up)
   { /* ExtSimu aktiv */
      sprintf(buf, WORKFLOW_TERM_RES_TXT, taskID);
      ok     = stopTask(); 
      sim_up = FALSE;
      strcpy(lastSimuModelName, "");
      strcpy(lastSimServDomainName, "");
      if (ok)
	 strcat(buf, WF_ACTION_DONE_TXT);
      else
	 strcat(buf, WF_ACTION_FAILED_TXT);
      sprintf(simuLogBuf, "\n%s %s %s", STRICH_15, buf, STRICH_15);
      write_to_log(simuLogBuf, KEEP_LOG_OPEN);
#ifdef MITSIMULOG
      write_to_simulog(simuLogBuf, KEEP_LOG_OPEN);
#endif /* MITSIMULOG */
#ifdef GLEAM_USR
      write_buf(get_line_anz()+1, buf);
#endif /* GLEAM_USR */
   } /* ExtSimu aktiv */
   taskID = NIX_IDX;
#ifdef SIMU_KOPPL_DBG
   printf ("terminate_ext_simu: Ende: ok=%d sim_ok=%d sim_up=%d\n", ok, sim_ok, sim_up); fflush(stdout);
#endif /* SIMU_KOPPL_DBG */
} /* terminate_ext_simu */



/* ================================ ext_simu_recovery ================================= */
void ext_simu_recovery (void)
{
   unsigned INT  rec_delay;               /* actual waiting time of each recovery cycle */
   INT           trial_ctr,               /* recovery counter                           */
                 idx;
#ifdef MITSIMULOG
   STR_TYPE      date_buf, time_buf, dummy;
#endif

   /* ext_simu_recovery */
   rec_delay = RECOVER_START_DELAY; 
   trial_ctr = 0;
   reset_text_buf();
#ifdef MITSIMULOG
   get_time_stamp (dummy, date_buf, dummy, time_buf);
   sprintf(simuLogBuf, "SIMU-RECOVERY: Beginn am %s um %s  sim_ok=%d sim_up=%d",
	   date_buf, time_buf, sim_ok, sim_up);
   write_to_simulog(simuLogBuf, KEEP_LOG_OPEN);
#endif /* MITSIMULOG */
   do
   { /* Warten und Starten */
      warte (rec_delay);                               /* wait "rec_delay" milliseconds */
      ext_simu_hochfahren(TRUE);         /* TRUE: includes call of terminate_ext_simu() */
      trial_ctr++;
#ifdef MITSIMULOG
      get_time_stamp (dummy, date_buf, dummy, time_buf);
      sprintf(simuLogBuf, "SIMU-RECOVERY: Time=%s %s  sim_ok=%d sim_up=%d Loop=%d Delay=%d msec",
	      date_buf, time_buf, sim_ok, sim_up, trial_ctr, rec_delay);
      write_to_simulog(simuLogBuf, CLOSE_LOG);
#endif /* MITSIMULOG */
      rec_delay += RECOVER_DELAY_INCR;
   } /* Warten und Starten */
   while (!sim_up && (trial_ctr < RECOVER_TRIALS));
#ifdef TEXT_MODE
   write_text_buf(CLEAR);                                    /* Ausgabe des Textpuffers */
#endif /* TEXT_MODE */
} /* ext_simu_recovery */



/* ================================== init_ext_koppl ================================== */
BOOL init_ext_koppl (BOOL *ext_sim_mod_ok)
{
   INT   intVal;
   BOOL  ok;                                                         /* function result */

   /* init_ext_koppl */
   if (initListIO())                  /* sets "sim_ok" according to init and setupCom() */
   { /* establish exit handlers  */
      atexit (finalizeCom);          /* this sequence is essential: terminate_ext_sim() */
      atexit (terminate_ext_simu);   /* must be called prior to finalizeCom()           */
   } /* establish exit handlers  */
   else
      return (FALSE);

   inbuffer  = NULL;                                               /* used for pipe-i/o */
   errbuffer = NULL;                                               /* used for pipe-i/o */
   ok = TRUE;
   strcpy (akt_sim_mod_path, "");               /* d.h., dass kein startsim-File exist. */
#ifdef IO_LOG
   detailed_iolog = DETAILED_IO_LOG;
#endif
   if (mit_ext_simu)
   { /* mit externen Simulator */
      sim_up = FALSE;                         /* no model loaded at initialization time */
      if (getIntVal(&intVal, SIMU_MAX_TIME_IDX, GLEAM_ERROR))
	 update_ext_simu_tmo(intVal);
      else 
	 ok = FALSE;
      *ext_sim_mod_ok = TRUE;                  /* no model check possible at this point */
   } /* mit externen Simulator */
   else
   { /* ohne externen Simulator */
      sim_ok = FALSE; 
      sim_up = FALSE;
   } /* ohne externen Simulator */
   return (ok);
} /* init_ext_koppl */

