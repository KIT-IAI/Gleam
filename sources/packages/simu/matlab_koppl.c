/*******************************************************************************
GLEAM/AE                                                                  V1.0.0

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                       Package: SIMUlator
                        Module: MATLAB-anKOPPLung
 
Package: simu                   File: matlab_kopplc   
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or of 
the files "schalter.h".
--------------------------------------------------------------------------------

The module provides an interface to the matlab-based external simulators, 
matpower in particular.

The ID in the mod- or tsk-Files is: "Matlab"
It is defined by the constant MATLAB_KENNUNG, which is defined in module 
"mod_data.c" of package "hmod".

The extérnal simulator can be used with different simulation models. The name of
a model is in variable "extSimuModelName", which is loaded from the tsk-File
according to the entry "simulator model", see SIMU_MOD_NAME_TXT. The German
equivalent is "Simulatormodell". The model name may be changed manually by the 
user.

The status of the interface is stored in two variables:
"sim_ok":  Is TRUE, if the environment variable, the name of which is stored in 
           MATLAB_ROOT_DIR_TXT, is defined. Its present name is "MATLABROOT".
           The existance of the variable is checked during the start phase of 
           the program. Its value is passed to the matlab simulator.
"sim_up":  Is TRUE, if the external simulator is running and ready and a valid 
           simulation model is loaded. A simulation model is valid, if the file
               <working_dir>/Matpower/matpower6.0/<extSimuModelName>.m
           exists.

Timeout-Mechanism:
The general following timeout-mechanism is not used, so that there is no monitoring 
of the execution time.
  Timeouts are implemented by cyclyc waiting with the fixed wait time of 
  WAIT_FOR_READ (defined in simuInt.h) milliseconds. The number of cycles is 
  calculated by using the macro "sec", which expands to "* (1000 / WAIT_FOR_READ)".
  This allows to specify e.g. "2 sec" for a the amount of 40 wait cycles. 
  The general simulator TMO is given by the "timeout for ext. simulator [sec]" 
  program parameter in the tsk-file and stored as wait cycles in "simuResultWaitCycles".


<< Allgemeine Beschreibung der Implementierung >>


Ein bei Programmende noch aktiver externer Simulator wird durch die Exit-Routine
"terminate_ext_simu", die von "init_ext_koppl" aktiviert wird, terminiert.
 

Die Routinen im einzelnen:

Packages-Export der allgemeinen Schnittstelle zu externen Simulatoren:

BOOL doMatlabSim (DOUBLE              *erg_werte,
                  CHAIN_ELEM_PTR_TYPE  chrPtr);
          Die Routine prueft bei gesetztem "sim_up" die Zulaessigkeit von 
          "ext_simu_code" und ruft die entsprechende lokale Matlab-Kopplungs-
          funktion auf. Dies sind zur Zeit:
            - doMatPowerSim()  fuer MatPower-Anwnedungen
          Bei unzulaessigem Code erfolgt eine Fehlermeldung (fatal) und es wird 
          FALSE zurueckgeliefert. Bei geloeschtem "sim_up" terminiert die Routine 
          und liefert ebenfalls FALSE.

BOOL ext_simu_hochfahren (BOOL  mit_fin);
          "mit_fin" hat in diesem Kontext keine Bedeutung mehr.
          Wenn ein neues oder ein erstes Modell geladen werden soll, wird die 
          Existenz folgender .m-Datei geprueft:
                <working_dir>/Matpower/matpower6.0/<extSimuModelName>.m
          Wenn Pfad und Datei existieren, wird der neue Modellname in 
          "lastSimuModelName" gemerkt und der Simulator kann gestartet werden.
          Der Start des Simulators erfolgt nur, wenn er nicht bereits läuft 
          ("sim_up" ist FALSE), und er wegen Modellwechsel oder Neustart gestartet 
          werden soll.
          Modellwechsel und Startergebnis werden im Logfile protokolliert und bei 
          gesetztem GLEAM_USR auch in den Textpuffer von Module "fmeld.c" geschrie-
          ben.
	  Die Routine liefert TRUE, wenn der externe Simulator (wieder) laeuft,
          "sim_ok" und "sim_up" also TRUE sind.

void update_ext_simu_tmo (INT  tmo_zeit_in_sec);
          The number of wait cycles is calculated based on "tmo_zeit_in_sec" and
          a wait time of WAIT_FOR_READ. The result is stored in "simuResultWaitCycles".

void terminate_ext_simu (void);
          Die Routine prueft, ob die aktuelle Anwendung einen externen Simulator
          benutzt und ob "sim_ok" TRUE ist. Wenn ja, wird der Simulator bei ge-
          setztem "sim_up" terminiert, "sim_up" auf FALSE gesetzt und eine ent-
          sprechende Meldung ins Logfile geschrieben. Diese wird bei gesetztem 
          GLEAM_USR auch in den Textpuffer von Module "fmeld.c" geschrieben.
          Die Routine wird auch als Exithandler genutzt.


Package-lokale Routinen der allgemeinen Schnittstelle zu externen Simulatoren,
wobei "restart_ext_sim" und "kleiner_ext_restart" bei GUI_MODE zum Package-Export 
gehoeren:

void ext_simu_recovery (void);
          Die Routine wird von der Routine "simulator" aus simu.c im Optimierungs-
          kontext aufgerufen, wenn der Simulator nach einer Simulation nicht mehr 
          verfuegbar ist, d.h. dass "sim_up" FALSE ist. Sie fuehrt ein zyklisches 
          Recovery des externen Simulators durch bis er entweder erfolgreich 
          gestartet werden konnte oder RECOVER_TRIALS Versuche fehlschlugen. Dabei
	  wird von Versuch zu Versuch laenger gewartet, indem nach jedem erfolglosen 
          Versuch RECOVER_DELAY_INCR msec laenger gewartet wird.
          In der vorliegenden Implementierung besteht das Recovery aus einem Aufruf 
          von ext_simu_hochfahren().

BOOL init_ext_koppl (BOOL *ext_sim_mod_ok);
          Module initialisation. If "mit_ext_simu" is TRUE, the value of the 
          environment variable, the name of which is stored in MATLAB_ROOT_DIR_TXT, 
          is read and stored in "matlab_root". If the environment variable does not 
          exist or the content of "matlab_root" is not a valid path to a directory, 
          "sim_ok" and "ext_sim_mod_ok" are set to FALSE and a fatal error message 
          is generated. 
          The time out value, which is not further used by this implementation, is
          set. 
          If "mit_ext_simu" is FALSE, "sim_up" and "ext_sim_mod_ok" are set to TRUE.
          TRUE is returned, if sim_ok is TRUE and if a timeout value was defined.

--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. Es wurden zwei Moeglichkeiten hinzugefuegt, Matlab als externe 
    Simulationssoftware fuer Gleam zu benutzen:
      --> Direkte Function-Call in Matlab mittels des API-Headers "engine.h" 
      --> system-Call und Austausch der Daten mit Matlab durch File-I/O
 							 	     AB 05.07.17
 2. Bei der direkten Methode (erste Option) wird nun ein Struct 
    uebergeben. Damit ist sichergestellt, dass die Uebergabe der Para-
    meter nur von den Angaben im Mod-File abhaengig ist. Man kann damit
    beliebige, im Matpower-Case-File definierten Parameter variieren, 
    was fuer evtl. Parameterstudien vorteilhaft sein kann.           AB 19.07.17
 3. Direkte Matlab-Anbindung fertiggestellt. Alles, was man angeben
    muss, wenn diese Methode gewaehlt wird, ist die Namen der Gen-
    Parameter fuer Gleam gleich denen im Matlab-case-File zu setzen 
    und die einzelnen Gene (Generator, Branch, Bus) mit den richtigen 
    Nummern zu versehen (Bspl: generator_1, bus_4 etc.).             AB 26.07.17
 4. Methode ueber File-I/O wurde geloescht, da nicht mehr benoetigt. AB 21.12.17
 5. Verbesserung des Starten u. Stoppens des Simulators samt Meldungen. 
    Verbesserung der Modulinitialisation. Dokumentation.             WJ 12.02.18
 6. Beseitigung einiger Memoryleaks in "doMatlabSim".                WJ 12.03.18
 7. Beseitigung der restlichen MemLeaks in "doMatlabSim".	     AB 15.03.18
 8. Uebergeben eines Flags abhaengig vom neuen, im Mod-File festge-
    legten String "matlabApplName", ob PF oder OPF gerechnet werden
    soll.                                                            AB 29.03.18
 9. Einfuegung einiger Typecasts bei malloc().                       WJ 19.04.18
10. Anpassungen zur Integration in HP "essEvalFrame". Zwischen Start
    und Stop der Matlabe-Engine muessen mind. 170 msec vergehen, 
    sonst Crash. Daher die Verzoegerung in terminate_ext_sim(). 
    Neue exportierte Variable matlabNoConvCtr, welche die Matlab-
    Laeufe ohne Konvergenz zaehlt.                                   WJ 26.04.18
11. Einfuehrung von NO_CONV_WARN.                                    WJ 28.06.18
12. Variable ""matlabApplName" durch "ext_simu_code" ersetzt. 
    Letztere steuert die gesamte Matlab-Schnittstelle und ist fuer
    einen spaeteren Ausbau auf andere Matlab-Anwendungen als nur
    MatPower vorgesehen.                                             WJ 19.10.18
13. "extractCaseFile" wird wegen der CLV zum Package-Export.         WJ 29.10.18
14. Anpassung der Aufrufe von get_act_descr().                       WJ 28.06.19
15. Include des gesamten Package-Exports von "lgsw" anstelle von 
    nur "chaindef.h".                                                WJ 12.08.19
--------------------------------------------------------------------------------
Status: Impl           Autoren: A.Blattmann / W.Jakob          Datum: 12.08.2019
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <errno.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "lgsw.h"
#include "hmod.h"
#include "aufg.h"
#ifdef ESS_EVAL_SLAVE
  #include "simuESS.h"
#else /* no ESS_EVAL_SLAVE */
  #include "simu.h"
  #include "simuInt.h"
#endif /* no ESS_EVAL_SLAVE */


// engine.h ist Matlab-Header, um von Gleam aus eine Matlab Engine starten und 
// dieser die benoetigten Variablen uebergeben zu koennen
#include "engine.h"

#include SIMU_TEXTS
#include GLOBAL_TEXTS


#undef MIT_DBG_AUSG
#undef NO_CONV_WARN



/* ========================== Exportierte Variable: ========================= */
INT     matlabNoConvCtr;         /* counts matlab runs without convergence    */
INT     count_evals;
DOUBLE  best_res;
BOOL    extractCaseFile;         /* TRUE: "doMatlabSim" causes the interface  */
                                 /*       to write a matlab case file         */

#ifdef ESS_EVAL_SLAVE
  char *extSimuModelName,        /* name of the simulation model of extSimu.  */
       *lastSimuModelName;       /* last loaded simulator model               */
#endif /* ESS_EVAL_SLAVE */



/* ======================== Package-globale Variable: ======================= */
char  *inbuffer;                /* Daten vom ExtSimu         [BUF_LEN+1]      */
char  *errbuffer;               /* Fehlermeldungen d.ExtSimu [BUF_LEN+1]      */
LONG   prompt_ctr,              /* Nummer des nachsten erwarteten Prompt      */
       wait_ctr;                /* Zaehlt Wartezyklen. Global f."mit_rz"      */
BOOL   ak_bewertet,             /* True, wenn AK erfolgreich bewertet         */
       detailed_iolog = FALSE;  /* Steuert Detailierungsgrad des IO_LOG       */



/* ============================ Lokale Konstante: =========================== */
#define MATLAB_ROOT_DIR_TXT   "MATLABROOT"
#define TYPE_TXT              "type"
#define PARAM_NAMES_TXT       "param_names"
#define PARAM_VALUES_TXT      "param_values"

/* some definitions for the "ext_simu_recovery" function:                     */
#define RECOVER_TRIALS          50    /* number of wait cycles                */
#define RECOVER_START_DELAY    500    /* initial wait time in msec            */
#define RECOVER_DELAY_INCR    1000    /* wait time increment per wait cycle   */



/* ============================ Lokale Variable: ============================ */
static STRING_TYPE  logbuffer, buffer;
static Engine      *matlab_engine;
static char        *matlab_root;
static const char  *field_names[] = {TYPE_TXT, PARAM_NAMES_TXT, PARAM_VALUES_TXT};  
                   //Definieren der drei Felder einer jeden Komponente des Individuum-Structs



/* ============================ Lokale Routinen: ============================ */
static BOOL doMatPowerSim (DOUBLE              *erg_werte,
                           CHAIN_ELEM_PTR_TYPE  chrPtr);




/* ============================== do_ext_sim ================================ */
BOOL do_ext_sim (DOUBLE *erg_werte,
                 INT    *i_params,
                 DOUBLE *r_params)
{
   fatal (SIMU_MATLAB_KOPPL, 1, NO_INFO, NO_INFO, GEN_EXT_SIMU_TXT, APPL_NOT_AVAILABLE);
   return (FALSE);
} /* do_ext_sim */



/* ============================== doMatlabSim =============================== */
BOOL doMatlabSim (DOUBLE              *erg_werte,
                  CHAIN_ELEM_PTR_TYPE  chrPtr)
{
   BOOL  ok;

   /* doMatlabSim */
   ok = FALSE;
   if (sim_up)
   { /* simulator communication active */
      if ((ext_simu_code == ML_MP_OPF) || (ext_simu_code == ML_MP_PF))
	 ok = doMatPowerSim(erg_werte, chrPtr);
      else  /* here check for other Matlab applications and call of appropriate functions */
	 err_state = fatal(SIMU_MATLAB_KOPPL, 30, NO_INFO , NO_INFO, "", NO_POWER_FLOW_OPTION);
   } /* simulator communication active */
   return (ok);
} /* doMatlabSim */



/* ============================= doMatPowerSim ============================== */
static BOOL doMatPowerSim (DOUBLE              *erg_werte,
                           CHAIN_ELEM_PTR_TYPE  chrPtr)
/* -----------------------------------------------------------------------------
    Die Routine fuehrt einen Simulationslauf mit dem Chromosom "chrPtr" aus und 
    liefert TRUE, wenn in "erg_werte" gueltige Werte stehen, d.h., die Simulation 
    erfolgreich durchgefuehrt werden konnte. 
    Die aktuelle Anzahl der Ergebniswerte steht in "akt_anz_roh_erg_werte". 
    Diese Mengenangabe wird beim Laden des MOD-Files durch Modul "mod_data"
    gesetzt. 
----------------------------------------------------------------------------- */
{
   CHAIN_ELEM_PTR_TYPE  itemPtr;					      	//Pointer auf die aktuelle Aktionskette
   ACT_DESCR_PTR_TYPE   thisGeneDescrPtr;					//Definition in hmod.h, Beschreibung der Aktionskette
   STRING_TYPE          geneBuffer,geneBufferToMatlab;		//Pointer auf das char array, wo der c_string gespeichert wird, der später auf die Konsole geprintet wird
   STR_TYPE             buf;
   INT                  geneCode, geneNr,
                        ii,jj,size,paramNr;
   BOOL                 ok;
   static char working_dir_path[256];
   
   mxArray *individuum;
   mxArray *gen_names[last_act];
   mxArray *param_names[last_act];
   mxArray *param_values[last_act];
   mxArray *write_case_file;
   mxArray *bew_matlab;
   mxArray *matpower_suceeded;
   mxArray *case_file_written;

   /* doMatPowerSim */
   geneNr  = 0;
   ok      = TRUE;

      itemPtr = chrPtr->u_el.next_action;
		while((itemPtr != NULL) && ok)
		{

			geneCode = itemPtr->u_el.activity;
			if (get_act_descr(&thisGeneDescrPtr, geneCode, WITHOUT_GENECODE_CHECK) == GLEAM_OK)	//Wenn kompatibel und die Gestalt des aktuellen Individuums zu der im erwarteten Genmodell passt
			{ 
				paramNr = 0;
				//Name des Gens (kann in Matpower nur generator_N, branch_N oder bus_N sein)
				gen_names[geneNr]=mxCreateString(thisGeneDescrPtr->act_name);
				//Anpassung der länge der Arrays act_par_names und par_values an die Anzahl der Parameter des aktuellen Gens
				size=int_pro_aktion[geneCode]+real_pro_aktion[geneCode];
				char *act_par_names[size];
				double* par_values = (double*)malloc(size*sizeof(double));
				//Befüllen der Arrays act_par_names und par_values mit den Integer Parametern (und entsprechender Cast in doubles, da Matpower nur doubles verarbeitet)
				for (ii = 0;  ii < int_pro_aktion[geneCode];  ii++)
				{ 
				   act_par_names[paramNr] = (char*)malloc(PAR_NAME_MAX+1);
					sprintf(act_par_names[paramNr],"%s",thisGeneDescrPtr->i_par_descr[ii].par_name);
					par_values[paramNr]=(double)itemPtr->u_el.u_xel.u_act.i_params[ii];
					paramNr++;
				} 
				//Befüllen der Arrays act_par_names und par_values mit den double-Parametern des Gens
				for (ii = 0;  ii < real_pro_aktion[geneCode];  ii++)
				{ 
				   act_par_names[paramNr] = (char*)malloc(PAR_NAME_MAX+1);
					sprintf(act_par_names[paramNr],"%s",thisGeneDescrPtr->r_par_descr[ii].par_name);
					par_values[paramNr]=itemPtr->u_el.u_xel.u_act.r_params[ii];
					paramNr++;
				} 

				//Erstellen der drei Felder der dem jeweiligen Gen entsprechenden Komponente des Individuum-Structs
				//Namen der Parameter des Gens 
				param_names[geneNr]=mxCreateCharMatrixFromStrings(size,(const char **)act_par_names);
				//Parameterwerte des Gens als mxArray definieren
				param_values[geneNr]=mxCreateDoubleMatrix(size,1,mxREAL);
				// Kopieren der durch die Evolution ermittelten Werte in das Matlab-Array
				memcpy(mxGetPr(param_values[geneNr]),par_values,size*sizeof(double));
				//dynamischen Speicher freigeben
				for (ii = 0;  ii < size;  ii++){
				   free(act_par_names[ii]); 
			   }
			   free(par_values);
			}else
			{ 
				ok = FALSE;
				err_state = fatal (SIMU_MATLAB_KOPPL, 10, geneCode, NO_INFO, "", BAD_ACTION);
			}
			geneNr++;	
			itemPtr=itemPtr->u_el.next_action;
		} /* loop along the genes of the chromosome */
		//Initialisieren des Individuum-Structs	
		individuum = mxCreateStructMatrix(1,last_act,3,field_names);
		//Befüllen der Felder mit den Namen der Gene, Namen der Parameter eines jeden Gens und den Werten der entsprechenden Parametern		
		for(ii=0; ii<last_act; ii++){
			mxSetField(individuum,ii,TYPE_TXT,gen_names[ii]);
			mxSetField(individuum,ii,PARAM_NAMES_TXT,param_names[ii]);
			mxSetField(individuum,ii,PARAM_VALUES_TXT,param_values[ii]);
			
		}
		//Setzen des richtigen Pfades zum Arbeitsverzeichnis vom Matpower und Austauschverzeichnis für Dateien
		sprintf(working_dir_path,"addpath ([pwd,'/Matpower/schnittstelle_ea'])");
		//Navigieren in das entsprechende Verzeichnis, wo die Matlab-seitige Schnittstelle liegt
		engEvalString(matlab_engine,working_dir_path);
		//Laden des Individuum-Structs in den Matlab Workspace
		const int individuum_loaded = engPutVariable(matlab_engine,"individuum",individuum);
		// Flag, ob case_file extrahiert werden soll
		write_case_file = mxCreateLogicalScalar(extractCaseFile);
		const int write_case_file_set = engPutVariable(matlab_engine,"write_case_file",write_case_file);
		//Aufrufen der Testfunktion mit den im Workspace geladenen Variablen, Bewertung wird im Matlab-WS gespeichert
		engEvalString(matlab_engine,"[bewertung,sim_success, case_written] = gleam_simulator.simulate(individuum,write_case_file);");
		//Laden der Ergebniswerte und Referenz darauf für die gleamseitige Weiterverarbeitung der Bewertung
		bew_matlab = engGetVariable(matlab_engine,"bewertung");
		matpower_suceeded = engGetVariable(matlab_engine,"sim_success");
		case_file_written = engGetVariable(matlab_engine,"case_written");
		BOOL success = mxIsLogicalScalarTrue(matpower_suceeded);
		if(!success){
#if defined(GLEAM_USR) && defined(NO_CONV_WARN)
		   printf("WARNING: Matpower did NOT converge! Current individuum is being removed!\n");
#endif /* with user interface and activated NO_CONV_WARN */
		   matlabNoConvCtr++;
		}
		BOOL case_exist = *mxGetLogicals(case_file_written);
		#ifdef GLEAM_USR
		if(case_exist){
			char current_dir[256];
				if(getcwd(current_dir,sizeof(current_dir))!= NULL){
					printf("Matpower case-file has been written to %s.\n",current_dir);
				}
		}else if(extractCaseFile && !case_exist){
				err_state |= fehler (SIMU_MATLAB_KOPPL, 11, NO_INFO , NO_INFO, "" , NO_CASE_FILE_WRITTEN);			
		}
		#endif
		// Kopieren der Bewertung in den Rückgabe-Pointer
		memcpy(erg_werte,mxGetPr(bew_matlab),akt_anz_roh_erg_werte*sizeof(double));
		// Extrahieren des besten Ergebnisses pber die Anzahl der Evaluationen
		count_evals++;
		if(erg_werte[0]>best_res && erg_werte[1]){
#if defined(GLEAM_USR) && defined(MIT_DBG_AUSG)
			printf("current_best before update: %.3f, at eval nr %d\n",best_res,count_evals);
#endif /* with user interface and activated MIT_DBG_AUSG */
			best_res=erg_werte[0];
#if defined(GLEAM_USR) && defined(MIT_DBG_AUSG)
			printf("best_result updated: current_best: %.3f\n",best_res);
#endif /* with user interface and activated MIT_DBG_AUSG */
			FILE* fid = fopen("results_over_evals.txt","a");
			fprintf(fid,"%d %f\n",count_evals,best_res);
			fclose(fid);
		}
		//Zerstören der mxArrays, freigeben des benötgten Speichers - Zerstören des structs gibt den Speicher aller beinhalteten Pointer frei
		mxDestroyArray(individuum);
		mxDestroyArray(bew_matlab);
		mxDestroyArray(matpower_suceeded);
		mxDestroyArray(case_file_written);
		mxDestroyArray(write_case_file);
   return (ok);
} /* doMatPowerSim */



/* ========================== ext_simu_hochfahren =========================== */
BOOL ext_simu_hochfahren (BOOL  mit_fin)
{
   BOOL  startEngine,
         newModel,
         use_opf;
   char  to_working_dir[256];
   char  matlab_start[256];
   char  matpower_model_name[256];
   static char working_dir_path[256];	
   
   mxArray *sim_use_opf;
   mxArray *model_name;
  
   /* ext_simu_hochfahren */
   startEngine = TRUE;
   newModel   = strcmp(extSimuModelName, lastSimuModelName) != 0;
   use_opf = (ext_simu_code == ML_MP_OPF);
#ifdef MIT_DBG_AUSG
   printf ("ext_simu_hochfahren: Anfang: sim_ok=%d sim_up=%d new_model=%d\n", sim_ok, sim_up, newModel);
#endif
   if (newModel)
   { /* new model or model changed */
      if (getcwd(to_working_dir, sizeof(to_working_dir)) == NULL)
      { /* working directory cannot be obtained */
	 err_state  |= fehler(SIMU_MATLAB_KOPPL, 20, NO_INFO, NO_INFO, "", NO_CWD_ERR);
	 startEngine = FALSE;
      } /* working directory cannot be obtained */
      else
      { /* working directory obtained */
	 sprintf(matpower_model_name, "%s/%s.mat", to_working_dir, extSimuModelName);
#ifdef MIT_DBG_AUSG
	 printf("ext_simu_hochfahren: Path to matpower case file: \"%s\"\n", matpower_model_name);
#endif
	 if (access(matpower_model_name, F_OK) == 0)
	 { /* model file exists */
	    if (strlen(lastSimuModelName) != 0) 
	    { /* report model change */
	       sprintf (logbuffer, MODEL_CHANGED_TXT, lastSimuModelName, extSimuModelName);
	       write_to_log (logbuffer, CLOSE_LOG);
#ifdef GLEAM_USR
	       write_buf(get_line_anz()+1, logbuffer);
#endif /* GLEAM_USR */
	    } /* report model change */
	    strcpy(lastSimuModelName, extSimuModelName);  /* save act.model name */
	 } /* model file exists */
	 else 
         { /* model file does not exist */
	    err_state |= fehler(SIMU_MATLAB_KOPPL, 21, NO_INFO, NO_INFO, matpower_model_name, 
				NO_EXT_SIMU_MODEL_FILE);
	    startEngine = FALSE;
	    terminate_ext_simu();
	 } /* model file does not exist */
      } /* working directory obtained */
   } /* new model or model changed */
   if (startEngine && !sim_up)
   { /* start matlab engine */
#ifdef MIT_DBG_AUSG
      printf ("ext_simu_hochfahren: Engine soll gestartet werden! sim_ok=%d matlab_root=\"%s\"\n", 
	      sim_ok, matlab_root);
#endif
      sprintf(buffer, SIMUSTART_TXT2, extSimuModelName);
      sprintf(matlab_start, "%s/bin/matlab -nosplash -nojvm", matlab_root);
      if (!(matlab_engine = engOpen(matlab_start))) 
	 fehler(SIMU_MATLAB_KOPPL, 22, NO_INFO , NO_INFO, STARTED_TXT, EXT_SIMU_NIX_DONE);	
      else 
      { /* Matlab engine successfully started */
	 sim_up = TRUE;
	 engSetVisible(matlab_engine,0);
	 
	 // set input for GleamSimulator
	 sprintf(working_dir_path,"addpath ([pwd,'/Matpower/schnittstelle_ea'])");
	 engEvalString(matlab_engine,working_dir_path);
	 model_name = mxCreateString(extSimuModelName);
	 const int model_passed = engPutVariable(matlab_engine,"model_name",model_name);
	 sim_use_opf = mxCreateLogicalScalar(use_opf);
	 const int opf_flag = engPutVariable(matlab_engine,"use_opf",sim_use_opf);
	 
	 // instantiate GleamSimulator
	 engEvalString(matlab_engine,"gleam_simulator = GleamSimulator(model_name,use_opf);");
	 
	 // free memory after variables have been passed
	 mxDestroyArray(sim_use_opf);
	 mxDestroyArray(model_name);
#ifdef GLEAM_USR
	 sprintf(logbuffer, "%s%s %s.", MATLAB_SIMU_TXT, buffer, STARTED_TXT);
	 write_buf(get_line_anz()+1, logbuffer);
#endif /* GLEAM_USR */
      } /* Matlab engine successfully started */
      sprintf(logbuffer, "\n%s %s%s ", STRICH_15, MATLAB_SIMU_TXT, buffer);
      if (!sim_up)
	  strcat(logbuffer, NICHT_TXT);
      strcat(logbuffer, STARTED_TXT);
      strcat(logbuffer, ". ");
      strcat(logbuffer, STRICH_15);
      write_to_log(logbuffer, CLOSE_LOG);
   } /* start matlab engine */
#ifdef MIT_DBG_AUSG
   printf ("ext_simu_hochfahren: Ende: sim_ok=%d sim_up=%d startEngine=%d\n", sim_ok, sim_up, startEngine);
#endif
   return (sim_ok && sim_up);
} /* ext_simu_hochfahren */



/* =========================== update_ext_simu_tmo ========================== */
void update_ext_simu_tmo (INT  tmo_zeit_in_sec)
{                                /* sec expands to "* (1000 / WAIT_FOR_READ)" */
   simuResultWaitCycles = l_round (tmo_zeit_in_sec sec);
} /* update_ext_simu_tmo */



/* ========================== terminate_ext_simu ============================ */
void terminate_ext_simu (void)
{
   /* terminate_ext_simu */
#ifdef MIT_DBG_AUSG
   printf ("terminate_ext_simu: Anfang: sim_ok=%d sim_up=%d\n", sim_ok, sim_up);
#endif
   if (sim_ok && mit_ext_simu)
   { 
      if (sim_up) 
      {
	 warte(200);    /* min.time between engine start and stop is 170 msec */
	 engClose(matlab_engine);
	 sim_up = FALSE;
	 sprintf(buffer, SIMUSTART_TXT2, extSimuModelName);
	 sprintf(logbuffer, "\n%s %s%s %s. %s", STRICH_15, MATLAB_SIMU_TXT, buffer, TERMINATED_TXT, STRICH_15);
	 write_to_log(logbuffer, CLOSE_LOG);
#ifdef GLEAM_USR
	 sprintf(logbuffer, "%s%s %s.", MATLAB_SIMU_TXT, buffer, TERMINATED_TXT);
	 write_buf(get_line_anz()+1, logbuffer);
#endif /* GLEAM_USR */
      }
      strcpy(lastSimuModelName, "");
   }
#ifdef MIT_DBG_AUSG
   printf ("terminate_ext_simu: Ende: sim_ok=%d sim_up=%d\n", sim_ok, sim_up);
#endif
} /* terminate_ext_simu */



/* =========================== ext_simu_recovery ============================ */
void ext_simu_recovery (void)
{
   unsigned INT  rec_delay;       /* Aktuelle Wartezeit fuer Simu-Recover     */
   INT           trial_ctr,       /* Recovery-Zaehler                         */
                 idx;
#ifdef MITSIMULOG
   STR_TYPE      date_buf, time_buf, dummy;
#endif

   /* ext_simu_recovery */
   rec_delay = RECOVER_START_DELAY; 
   trial_ctr = 0;
#ifdef MITSIMULOG
   if (mit_simulog)
   {
      get_time_stamp (dummy, date_buf, dummy, time_buf);
      sprintf(simuLogBuf, "SIMU-RECOVERY: Beginn am %s um %s  sim_up=%d "
	      "sim_ok=%d", date_buf, time_buf, sim_up, sim_ok);
      write_to_simulog(simuLogBuf, CLOSE_LOG);
   }
#endif /* MITSIMULOG */
   do
   { /* Warten und Starten */
      warte (rec_delay);                   /* Warte "rec_delay" Millisekunden */
      ext_simu_hochfahren(FALSE);
      trial_ctr++;
#ifdef MITSIMULOG
      if (mit_simulog)
      {
	 get_time_stamp (dummy, date_buf, dummy, time_buf);
	 sprintf(simuLogBuf, "SIMU-RECOVERY: Time=%s %s  sim_up=%d "
		 "sim_ok=%d Loop=%d Delay=%d msec", date_buf, time_buf,
		 sim_up, sim_ok, trial_ctr, rec_delay);
	 write_to_simulog(simuLogBuf, CLOSE_LOG);
      }
#endif /* MITSIMULOG */
      rec_delay += RECOVER_DELAY_INCR;
   } /* Warten und Starten */
   while (!sim_up && (trial_ctr < RECOVER_TRIALS));
} /* ext_simu_recovery */



/* ============================= init_ext_koppl ============================= */
BOOL init_ext_koppl (BOOL *ext_sim_mod_ok)
{
   INT   intVal;
   BOOL  ok;                                               /* function result */

   /* init_ext_koppl */
   inbuffer  = NULL;                                     /* used for pipe-i/o */
   errbuffer = NULL;                                     /* used for pipe-i/o */
   ok        = TRUE;
   extractCaseFile = FALSE;            /* nur bei Ergebnissimulation sinnvoll */
   matlabNoConvCtr = 0;
   count_evals=0;
   best_res=0.0;
#ifdef IO_LOG
   detailed_iolog = DETAILED_IO_LOG;
#endif
   sim_ok          = TRUE;
   *ext_sim_mod_ok = TRUE;                       /* it's possible to simulate */
   if (mit_ext_simu)
   { /* mit externen Simulator */
      matlab_root = getenv(MATLAB_ROOT_DIR_TXT);
      sim_up      = FALSE;          /* the simulator has not been started yet */
      if (matlab_root == NULL) 
      { /* environment variable does not exist */
	 ok        = FALSE;
	 err_state = fatal(SIMU_MATLAB_KOPPL, 100, NO_INFO, NO_INFO, 
			   MATLAB_ROOT_DIR_TXT, NO_ENV_VAR);
      } /* environment variable does not exist */
      else
	 if (!check_dir(matlab_root))
	 { /* directory does not exist */
	    ok        = FALSE;
	    err_state = fatal(SIMU_MATLAB_KOPPL, 101, NO_INFO, NO_INFO, matlab_root, NO_DIR_ERR);
	 } /* directory does not exist */
      if (!ok)
      { /* it's not possible to simulate */
	 sim_ok          = FALSE;
	 *ext_sim_mod_ok = FALSE;          
      } /* it's not possible to simulate */
#ifdef ESS_EVAL_SLAVE
      if (((extSimuModelName     = (char*)malloc(MAX_STRING * sizeof(char))) != NULL) &&
	  ((lastSimuModelName    = (char*)malloc(MAX_STRING * sizeof(char))) != NULL))
      { /* buffers available */
	 strcpy (extSimuModelName,  "unspecified");
	 strcpy (lastSimuModelName, "");      /* MUST be empty on program start! */
      } /* buffers available */
      else
	 ok = FALSE;
#else /* no ESS_EVAL_SLAVE */
      atexit (terminate_ext_simu);           /* Etablierung des Exit-Handlers */
      strcpy (akt_sim_mod_path, "");     /* d.h., dass kein startsim-File exist. */
      if (getIntVal(&intVal, SIMU_MAX_TIME_IDX, GLEAM_ERROR))
	 update_ext_simu_tmo(intVal);
      else 
	 ok = FALSE;
#endif /* no ESS_EVAL_SLAVE */
   } /* mit externen Simulator */
   else /* ohne externen Simulator */
      sim_up = TRUE;
   return (ok && sim_ok);
} /* init_ext_koppl */

