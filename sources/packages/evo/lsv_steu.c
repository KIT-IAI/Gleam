/*******************************************************************************
HyGLEAM/A                                                                 V1.2.1

                                  H y G L E A M
       Hybrid GeneraL purpose Evolutionary learning Algorithms and Methods

                       Package: EVOlution
                        Module: LSV-STEUerung
 
Package: evo                    File: lsv_steu.c
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------

Der Modul definiert und verwaltet die Datenobjekte fuer die (adaptive) LSV-
und Heuristiken-Steuerung.

Fuer die LSVs gilt:
              +---+---+---+---+---+-   -+---+
aktive_lsv:   | 0 | 2 | 3 |   |   | ... |   |    es sei akt_lsv_anz = 3
              +---+---+---+---+---+-   -+---+
       Index:   0   1   2   3   4        LSV_ANZ
              +---+---+---+---+---+-   -+---+
lsv:   aktiv: | T | F | T | T | F | ... | F |
              +---+---+---+---+---+-   -+---+
              |   |   |   |   |   |     |   |
                      . . .


Die Routinen im einzelnen:

BOOL  activate_lsv (INT  lsvIdx);
        Es wird geprueft, ob mit "lsvIdx" ein aktuell implementiertes LSV be-
        zeichnet wird. Positivenfalls wird sein "aktiv"-Eintrag auf TRUE gesetzt
        und die Routine liefert TRUE, sonst FALSE und Fehlermeldung;

BOOL  prep_active_lsvs (void);
        Erstellt "aktive_lsv" und prueft dabei, ob die "lsv_anz" korrekt ist,
        d.h. es auch genauso viele aktive LSVs in "lsv" gibt. Wenn nicht, wird 
        eine Fehlermeldung erzeugt und FALSE abgeliefert, sonst TRUE. Wenn alles
        ok ist und es aktive LSVs gibt, wird das Default-OptVerfahren und das 
        Default-LSV (erstes aktives gemaess Genmodel) eingestellt.

INT   get_lsv_idx (INT  idx);
        Liefert den Index in "lsv" des "idx"-ten aktiven LSVs ab, sofern es
        dieses LSV auch gibt. Sonst 0 und Fehlermeldung.

BOOL  parameterise_lsv (INT  lsv_idx);
        Setzt die Parameter des durch "lsv_idx" bezeichneten LSVs entsprechend 
        den Daten in lsv[lsv_idx].lsvPar[] neu und liefert TRUE ab, wenn alles 
        gut ging.

GSTATE parameteriseAllLSVs (void);
        Setzt die Parameter aller im aktuellen Kontext bekannten LSVs ent- 
        sprechend den Daten in lsv[].lsvPar[] neu und liefert GLEAM_OK ab, wenn 
        alles gut ging.


Package-lokale Routinen:

DOUBLE do_lsv_start (CHAIN_ELEM_PTR_TYPE *ak_ptr,
                     BOOL                 lamarck,
                     GSTATE              *erg_status);
        Auf die im "ak_ptr" uebergebene AK wird das durch "lsv_strat" bezeichnete
        LSV (Index in "lsv") angewandt und die resultierende Note als Funktions-
        ergebnis abgeliefert. Bei Fehlern wird KEINE_NOTE und ein entsprechend
        gesetzter Return-Status "erg_status" abgeliefert.
        Die Routine dient als Verteiler auf die implementierten LSVs. Die auf-
        gerufenen Routinen muessen neben der LSV-Anwendung noch folgendes be-
        wirken:
          1. Wenn die Note durch das LSV verbessert wurde, wird die neue Note in
             die AK eingetragen. 
          2. Wenn "lamarck" TRUE ist, wird die AK an das gefundene lokale Opti-
             mum angepasst.

BOOL  init_lsv_steu (void);
        Legt die Datenstrukturen an und initialisiert sie. Liefert TRUE ab, 
        wenn genuegend Speicherplatz verfuegbar ist, sonst FALSE.

--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. ROS_3PARAM: Erweiterung fuer 3. Rosenbrock-Parameter f. OPAL/V.  WJ 20.11.06
 2. Entfernung von LSV_2OPT und LSV_3OPT. NOT_YET_IMPLEMENTED 
    ersetzt durch NOT_IN_THIS_VERSION.                               WJ 02.01.08
 3. Anpassung von "parameterise_lsv", "do_lsv_start"  und 
    "init_lsv_steu" an mehrere Saetze alternativer LSVs im Kontext 
    der Einfuehrung der GORBA-LSVs.                                  WJ 19.11.09
 4. Neue Routine "parameteriseAllLSVs", die von "read_mod_file"
    aufgerufen wird.                                                 WJ 02.03.10
 5. Initialisierung von "lsv[].abzuschalten" in "init_lsv_steu".     WJ 14.04.10
 6. Renovierung: Streichung des "link"-Includes.                     WJ 18.04.16
 7. "prep_active_lsvs" setzt auch die durch OPT_PROCEDURE_IDX und
    LHC_INDEX_IDX bezeichneten Programm-Parameter auf die Defaults.  WJ 06.04.17
 8. Reunion of "evoIntg.h" and "evoInts.h" in "evoInt.h".            WJ 30.06.17
 9. Beseitigung eines Speicherallokationsfehlers in "init_lsv_steu". WJ 03.08.17
10. Language dependent texts for the two standard LHCs.              WJ 29.08.20
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 29.08.2020
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schalter.h"
#include "sys.h"
#include "fbhm.h"
#include "lgsw.h"
#include "joints.h"
#include "aufg.h"
#ifdef MIT_GORBA_LSVS
  #include "glsv.h"
#endif /* MIT_GORBA_LSVS */
#include "evo.h"
#include "evoInt.h"

#include EVO_TEXTS


#ifdef GLO_CHECKS
  #define CHECKS
#endif



/* ========================== Exportierte Variable: ========================= */
LSVDescrType *lsv;               /* Liste aller implementierten LSVs          */
INT           lsv_anz;           /* Anzahl aktiver LSVs                       */
INT           aktive_lsv[LSV_ANZ];  /* Im aktuellen MOD-Kontext gueltige LSVs */
INT           allPLevelAnz;      /* all-P: Anzahl der Level                   */
DOUBLE        allPLevelVal[LEVEL_MAX]; /* all-P: LSV-Wahrscheinl.fuer "all"   */



/* ======================== Package-globale Variable: ======================= */



/* ============================ Lokale Variablen: =========================== */
static size_t  dsize = sizeof(DOUBLE);
static size_t  lsize = sizeof(LONG);
static size_t  isize = sizeof(INT);
static size_t  bsize = sizeof(BOOL);
static size_t  psize = sizeof(DF_TYP);




/* ============================== activate_lsv ============================== */
BOOL activate_lsv (INT  lsvIdx)
{
   BOOL  erg = TRUE;

   if ((0 <= lsvIdx) && (lsvIdx < LSV_ANZ))
      if (lsv[lsvIdx].impl)
         lsv[lsvIdx].aktiv = TRUE;
      else
         fehler (EVO_LSVSTEU, 1, lsvIdx, NO_INFO, "", NOT_IMPL_LSV);
   else 
      erg = fatal(EVO_LSVSTEU, 2, lsvIdx, LSV_ANZ-1, "", BAD_LSV_IDX) == GLEAM_OK;
   return (erg);
} /* activate_lsv */



/* ============================ prep_active_lsvs ============================ */
BOOL  prep_active_lsvs (void)
{
   INT   local_lsv_anz, ii;
   BOOL  erg;

   /* prep_active_lsvs */
   local_lsv_anz = 0;
   erg           = TRUE;
   for (ii = 0;  ii < LSV_ANZ;  ii++)
      if (lsv[ii].aktiv) 
         aktive_lsv[local_lsv_anz++] = ii;
   if (local_lsv_anz != lsv_anz)
      erg = fatal(EVO_LSVSTEU, 3, lsv_anz, local_lsv_anz, "", BAD_LSV_AMOUNT) == GLEAM_OK;
   else
      if (lsv_anz > 0) { /* LSV-Daten ok und es gibt aktive LSVs */
         def_opt_strat = GLEAM_AMMA;       /* Default-Optimierungsverfahren   */
	 setIntVal(GLEAM_AMMA, OPT_PROCEDURE_IDX);
         def_lsv_strat = aktive_lsv[0];    /* Index des Default-LSVs in lsv[] */
	 setIntVal(def_lsv_strat, LHC_INDEX_IDX);
   } /* LSV-Daten ok und es gibt aktive LSVs */

   return (erg);
} /* prep_active_lsvs */



/* =============================== get_lsv_idx ============================== */
INT   get_lsv_idx (INT  idx)
{
   if (idx < lsv_anz)
      return (aktive_lsv[idx]);
   else {
      fatal (EVO_LSVSTEU, 4, idx, lsv_anz, "", BAD_LSV_IDX);
      return (0);
   }
} /* get_lsv_idx */



/* ============================ parameterise_lsv ============================ */
BOOL parameterise_lsv (INT  lsv_idx)
{
   BOOL  erg = TRUE;

   /* parameterise_lsv */
   switch (lsv_idx) { /* LSV-Verteiler */
#ifdef MIT_ROCO  /* ============= Parametrierung der ROCO-LSVs ============== */
   case LSV_ROSEN:
  #ifdef ROS_3PARAM
      erg = set_rc_limits (0, i_round(lsv[lsv_idx].lsvPar[0]), 
                           lsv[lsv_idx].lsvPar[1], lsv[lsv_idx].lsvPar[2]) == GLEAM_OK;
  #else /* standard (no ROS_3PARAM) */
      erg = set_rc_limits (0, i_round(lsv[lsv_idx].lsvPar[0]), 
                           lsv[lsv_idx].lsvPar[1], ROSEN_DEF_STEP_SIZE) == GLEAM_OK;
  #endif /* standard (no ROS_3PARAM) */
      break; /* LSV_ROSEN */

   case LSV_COMPL: 
      erg = set_rc_limits (i_round(lsv[lsv_idx].lsvPar[0]), 0, 0.0, 0.0) == GLEAM_OK;
      break; /*  */
#endif /* MIT_ROCO */
      
#ifdef MIT_GORBA_LSVS  /* ========== Parametrierung der GORBA-LSVs ========== */
   case LSV_LVKG: 
      setParamLVKG (lsv[lsv_idx].lsvPar[0], lsv[lsv_idx].lsvPar[1]);
      break; /* LSV_LVKG */

   case LSV_LVGK: 
      setParamLVGK (lsv[lsv_idx].lsvPar[0], lsv[lsv_idx].lsvPar[1]);
      break; /* LSV_LVGK */

   case LSV_RV: 
      setParamRV (lsv[lsv_idx].lsvPar[0]);
      break; /* LSV_RV */

   case LSV_RAS: 
      break; /* LSV_RAS */
#endif /* MIT_GORBA_LSVS */

   default:
      fehler (EVO_LSVSTEU, 5, lsv_idx, NO_INFO, "", UNKNOWN_LSV);
      erg = FALSE;
   } /* LSV-Verteiler */
   return (erg);
} /* parameterise_lsv */



/* =========================== parameteriseAllLSVs ========================== */
GSTATE parameteriseAllLSVs (void)
{
   GSTATE  ret_state = GLEAM_OK;

   /* parameteriseAllLSVs */
#ifdef MIT_ROCO  /* ============= Parametrierung der ROCO-LSVs ============== */
   ret_state = set_rc_limits (i_round(lsv[LSV_COMPL].lsvPar[0]), 
			      i_round(lsv[LSV_ROSEN].lsvPar[0]), lsv[LSV_ROSEN].lsvPar[1], 
			      ROSEN_DEF_STEP_SIZE);
#endif /* MIT_ROCO */

#ifdef MIT_GORBA_LSVS  /* ========== Parametrierung der GORBA-LSVs ========== */
   setParamLVKG (lsv[LSV_LVKG].lsvPar[0], lsv[LSV_LVKG].lsvPar[1]);
   setParamLVGK (lsv[LSV_LVGK].lsvPar[0], lsv[LSV_LVGK].lsvPar[1]);
   setParamRV   (lsv[LSV_RV  ].lsvPar[0]);
#endif /* MIT_GORBA_LSVS */

   return (ret_state);
} /* parameteriseAllLSVs */



/* ============================== do_lsv_start ============================== */
DOUBLE do_lsv_start (CHAIN_ELEM_PTR_TYPE *ak_ptr,
                     BOOL                 lamarck,
                     GSTATE              *erg_status)
{
   DOUBLE  lokale_note;

   /* do_lsv_start */
   if (*erg_status != GLEAM_OK)
      return (KEINE_NOTE);
   if ((*ak_ptr)->u_el.u_xel.u_hdr.chain_length == 0) /*Leere AK = unbewertbar*/
      return (KEINE_NOTE);

   switch (lsv_strat) { /* LSV-Verteiler */
#ifdef MIT_ROCO  /* ================== Aufruf der ROCO-LSVs ================= */
   case LSV_ROSEN:
      lokale_note = do_rc_1start (ak_ptr, lamarck, LSV_ROSENBROCK, erg_status);
      break;

   case LSV_COMPL: 
      lokale_note = do_rc_1start (ak_ptr, lamarck, LSV_COMPLEX, erg_status); 
      break;
#endif /* MIT_ROCO */

#ifdef MIT_GORBA_LSVS  /* ============== Aufruf der GORBA-LSVs ============== */
   case LSV_LVKG: 
      lokale_note = lsvLVKG (ak_ptr, erg_status);
      break; /* LSV_LVKG */

   case LSV_LVGK: 
      lokale_note = lsvLVGK (ak_ptr, erg_status);
      break; /* LSV_LVGK */

   case LSV_RV: 
      lokale_note = lsvRV (ak_ptr, erg_status);
      break; /* LSV_RV */

   case LSV_RAS: 
      lokale_note = lsvRAS (ak_ptr, erg_status);
      break; /* LSV_RAS */
#endif /* MIT_GORBA_LSVS */

   default:
      *erg_status = fatal (EVO_LSVSTEU, 6, lsv_strat, NO_INFO, "", UNKNOWN_LSV);
      lokale_note = KEINE_NOTE;
   } /* LSV-Verteiler */
   return (lokale_note);
} /* do_lsv_start */



/* ============================== init_lsv_steu ============================= */
BOOL init_lsv_steu (void)
{
   INT   ii, jj, kk;
   BOOL  ok;

   /* init_lsv_steu */
   lsv = (LSVDescrType*)malloc(sizeof(LSVDescrType) * LSV_ANZ);
   if (lsv == NULL)
      return (FALSE);                        /* Abbruch wegen Speichermangels */
   ok = TRUE;
   for (ii = 0;  (ii < LSV_ANZ) && ok;  ii++) 
   { /* alle LSVs */
      for (jj = 0;  (jj < LSV_PAR_MAX) && ok;  jj++) { /* alle LSV-SteuerParams */
         ok = (((lsv[ii].parLevelP[jj]     = (DFF_TYP)malloc(psize*NKL_MAX))  != NULL) &&
               ((lsv[ii].par2ndLevPSum[jj ]= (DF_TYP) malloc(dsize*NKL_MAX))  != NULL) &&
               ((lsv[ii].parLevelVal[jj]   = (DF_TYP) malloc(dsize*LEVEL_MAX))!= NULL) &&
               ((lsv[ii].parFgSum[jj]      = (DFF_TYP)malloc(psize*NKL_MAX))  != NULL) &&
               ((lsv[ii].parEvalSum[jj]    = (LFF_TYP)malloc(psize*NKL_MAX))  != NULL) &&
               ((lsv[ii].parCtr[jj]        = (IFF_TYP)malloc(psize*NKL_MAX))  != NULL) &&
               ((lsv[ii].parAllCtr[jj]     = (IF_TYP) malloc(isize*NKL_MAX))  != NULL) &&
               ((lsv[ii].par1stActLev[jj]  = (IF_TYP) malloc(isize*NKL_MAX))  != NULL) &&
               ((lsv[ii].statParBestLSum[jj]=(IFF_TYP)malloc(psize*NKL_MAX))  != NULL)   );
         for (kk = 0;  (kk < NKL_MAX) && ok;  kk++) /* 3.Dimension anlegen: */
            ok = (((lsv[ii].parLevelP[jj][kk]   =(DF_TYP)malloc(dsize*LEVEL_MAX)) != NULL) &&
                  ((lsv[ii].parFgSum[jj][kk]    =(DF_TYP)malloc(dsize*LEVEL_MAX)) != NULL) &&
                  ((lsv[ii].parEvalSum[jj][kk]  =(LF_TYP)malloc(lsize*LEVEL_MAX)) != NULL) &&
                  ((lsv[ii].parCtr[jj][kk]      =(IF_TYP)malloc(isize*LEVEL_MAX)) != NULL) &&
                  ((lsv[ii].statParBestLSum[jj][kk]=(IF_TYP)malloc(isize*LEVEL_MAX))!=NULL));
      } /* alle LSV-SteuerParams */
      for (jj = 0;  (jj < NKL_MAX) && ok;  jj++)  /* all-P: */
         ok = (((lsv[ii].allPLevelP[jj]      = (DF_TYP)malloc(dsize*LEVEL_MAX)) != NULL) &&
               ((lsv[ii].allPFgSum[jj]       = (DF_TYP)malloc(dsize*LEVEL_MAX)) != NULL) &&
               ((lsv[ii].allPEvalSum[jj]     = (LF_TYP)malloc(lsize*LEVEL_MAX)) != NULL) &&
               ((lsv[ii].allPCtr[jj]         = (IF_TYP)malloc(isize*LEVEL_MAX)) != NULL) &&
               ((lsv[ii].statAllPBestLSum[jj]= (IF_TYP)malloc(isize*LEVEL_MAX)) != NULL) &&
               ((lsv[ii].statLsvPNklHisto[jj]= (IF_TYP)malloc(isize*10       )) != NULL)   );
   } /* alle LSVs */
   if (ok) 
   { /* Statistik- und andere Daten initialisieren */
      for (ii = 0;  (ii < LSV_ANZ) && ok;  ii++) 
      { /* alle LSVs */
         for (jj = 0;  (jj < LSV_PAR_MAX) && ok;  jj++) 
         { /* alle LSV-SteuerParams */
            for (kk = 0;  (kk < NKL_MAX) && ok;  kk++) 
               memset (lsv[ii].statParBestLSum[jj][kk], 0, isize*LEVEL_MAX);
            memset (lsv[ii].statLsvPNklHisto[jj], 0, isize*10);   /* neu gegnueber 3.8. */
         } /* alle LSV-SteuerParams */
         for (kk = 0;  (kk < NKL_MAX) && ok;  kk++) 
            memset(lsv[ii].statAllPBestLSum[kk], 0, isize*LEVEL_MAX);
         memset(lsv[ii].statAllPJobAnz, 0, isize*NKL_MAX);
         memset(lsv[ii].statLsvPNklSum, 0, dsize*NKL_MAX);        /* neu gegnueber 3.8. */
         memset(lsv[ii].lsvOn,          0, bsize*NKL_MAX);        /* neu gegnueber 3.8. */
         memset(lsv[ii].abzuschalten,   0, bsize*NKL_MAX);
         memset(lsv[ii].lsvP,           0, dsize*NKL_MAX);        /* neu gegnueber 3.8. */
         memset(lsv[ii].lsvPSel,        0, dsize*NKL_MAX);        /* neu gegnueber 3.8. */
      } /* alle LSVs */
   } /* Statistik- und andere Daten initialisieren */
   else
      return (FALSE);                        /* Abbruch wegen Speichermangels */

   lsv_anz = 0;               /* ohne MOD-File-Angaben sind keine LSV aktiv ! */
   for (ii = 0;  ii < LSV_ANZ;  ii++) { /* reset */
      lsv[ii].impl  = FALSE;
      lsv[ii].aktiv = FALSE;
   } /* reset */

#ifdef MIT_ROCO  /* ======================== ROCO-LSVs ====================== */
   strcpy (lsv[LSV_ROSEN].lsvName, ROS_LONG_NAME_TXT);
   strcpy (lsv[LSV_ROSEN].lsvMenueName, ROS_MENUE_NAME_TXT);
   strcpy (lsv[LSV_ROSEN].lsvJobLiName, ROS_JOBLI_NAME_TXT);
   lsv[LSV_ROSEN].lsvCh     = 'R';
  #ifdef ROS_3PARAM
   lsv[LSV_ROSEN].lsvParAnz = 3;
   lsv[LSV_ROSEN].lsvPar[2] = 0.4;                             /* Defaultwert */
  #else /* Standard (kein ROS_3PARAM) */
   lsv[LSV_ROSEN].lsvParAnz = 2;
  #endif /* Standard (kein ROS_3PARAM) */
   lsv[LSV_ROSEN].lsvPar[0] = 2000;                            /* Defaultwert */
   lsv[LSV_ROSEN].lsvPar[1] = 0.001;                           /* Defaultwert */
   strcpy (lsv[LSV_ROSEN].lsvParName[0], ROS_PAR1_NAME_TXT);
   strcpy (lsv[LSV_ROSEN].lsvParJL1Name[0], ROS_PAR1_JOBLI_1);
   strcpy (lsv[LSV_ROSEN].lsvParJL2Name[0], ROS_PAR1_JOBLI_2);
   strcpy (lsv[LSV_ROSEN].lsvParName[1], ROS_PAR2_NAME_TXT);
   strcpy (lsv[LSV_ROSEN].lsvParJL1Name[1], ROS_PAR2_JOBLI_1);
   strcpy (lsv[LSV_ROSEN].lsvParJL2Name[1], ROS_PAR2_JOBLI_2);
  #ifdef ROS_3PARAM
   strcpy (lsv[LSV_ROSEN].lsvParName[2], ROS_PAR3_NAME_TXT);
   strcpy (lsv[LSV_ROSEN].lsvParJL1Name[2], ROS_PAR3_JOBLI_1);
   strcpy (lsv[LSV_ROSEN].lsvParJL2Name[2], ROS_PAR3_JOBLI_2);
  #endif /* ROS_3PARAM */
   lsv[0].impl = TRUE;

   strcpy (lsv[LSV_COMPL].lsvName, COMPL_LONG_NAME);
   strcpy (lsv[LSV_COMPL].lsvMenueName, COMPL_MENUE_NAME);
   strcpy (lsv[LSV_COMPL].lsvJobLiName, COMPL_JOBLI_NAME);
   lsv[LSV_COMPL].lsvCh     = 'C';
   lsv[LSV_COMPL].lsvParAnz = 1;
   lsv[LSV_COMPL].lsvPar[0] = 1000;                            /* Defaultwert */
   strcpy (lsv[LSV_COMPL].lsvParName[0], COMPL_PAR1_NAME);
   strcpy (lsv[LSV_COMPL].lsvParJL1Name[0], COMPL_PAR1_JOBLI_1);
   strcpy (lsv[LSV_COMPL].lsvParJL2Name[0], COMPL_PAR1_JOBLI_2);
   lsv[1].impl = TRUE;
#endif /* MIT_ROCO */

#ifdef MIT_GORBA_LSVS  /* ==================== GORBA-LSVs =================== */
   strcpy (lsv[LSV_LVKG].lsvName, "Linksverschiebung/KleinGross");
   strcpy (lsv[LSV_LVKG].lsvMenueName, "linksv/Kg");
   strcpy (lsv[LSV_LVKG].lsvJobLiName, "LVKG");
   lsv[LSV_LVKG].lsvCh     = 'K';
   lsv[LSV_LVKG].lsvParAnz = 2;
   lsv[LSV_LVKG].lsvPar[0] = 0.5;                              /* Defaultwert */
   strcpy (lsv[LSV_LVKG].lsvParName[0], "alpha1 (LVKG)");
   strcpy (lsv[LSV_LVKG].lsvParJL1Name[0], "  alpha1");
   strcpy (lsv[LSV_LVKG].lsvParJL2Name[0], "    LVKG");
   lsv[LSV_LVKG].lsvPar[1] = 1.0;                              /* Defaultwert */
   strcpy (lsv[LSV_LVKG].lsvParName[1], "alpha2 (LVKG)");
   strcpy (lsv[LSV_LVKG].lsvParJL1Name[1], "  alpha2");
   strcpy (lsv[LSV_LVKG].lsvParJL2Name[1], "    LVKG");

   strcpy (lsv[LSV_LVGK].lsvName, "Linksverschiebung/GrossKlein");
   strcpy (lsv[LSV_LVGK].lsvMenueName, "linksv/Gk");
   strcpy (lsv[LSV_LVGK].lsvJobLiName, "LVGK");
   lsv[LSV_LVGK].lsvCh     = 'G';
   lsv[LSV_LVGK].lsvParAnz = 2;
   lsv[LSV_LVGK].lsvPar[0] = 0.5;                              /* Defaultwert */
   strcpy (lsv[LSV_LVGK].lsvParName[0], "alpha1 (LVGK)");
   strcpy (lsv[LSV_LVGK].lsvParJL1Name[0], " alpha1");
   strcpy (lsv[LSV_LVGK].lsvParJL2Name[0], "   LVGK");
   lsv[LSV_LVGK].lsvPar[1] = 1.0;                              /* Defaultwert */
   strcpy (lsv[LSV_LVGK].lsvParName[1], "alpha2 (LVGK)");
   strcpy (lsv[LSV_LVGK].lsvParJL1Name[1], "  alpha2");
   strcpy (lsv[LSV_LVGK].lsvParJL2Name[1], "    LVGK");

   strcpy (lsv[LSV_RV].lsvName, "Rechtsverschiebung");
   strcpy (lsv[LSV_RV].lsvMenueName, "rechtsV");
   strcpy (lsv[LSV_RV].lsvJobLiName, "  RV");
   lsv[LSV_RV].lsvCh     = 'V';
   lsv[LSV_RV].lsvParAnz = 1;
   lsv[LSV_RV].lsvPar[0] = 0.75;                               /* Defaultwert */
   strcpy (lsv[LSV_RV].lsvParName[0], "beta (RV-Limit)");
   strcpy (lsv[LSV_RV].lsvParJL1Name[0], "    beta");
   strcpy (lsv[LSV_RV].lsvParJL2Name[0], "      RV");

   strcpy (lsv[LSV_RAS].lsvName, "LSV-RAS");
   strcpy (lsv[LSV_RAS].lsvMenueName, "lsv-Ras");
   strcpy (lsv[LSV_RAS].lsvJobLiName, " RAS");
   lsv[LSV_RAS].lsvCh     = 'R';
   lsv[LSV_RAS].lsvParAnz = 0;
   for (ii = 0;  ii < LSV_ANZ;  ii++)
      lsv[ii].impl = TRUE;
#endif /* MIT_GORBA_LSVS */

   return (TRUE);
} /* init_lsv_steu */

