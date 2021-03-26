/*******************************************************************************
GLEAM/AE                                                                  V3.4.1

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                         Package: SIMUlator
                          Module: CEC Mathematische BenchMarkfunktions-SIMulator
 
Package: simu                     File: cec_mbf_sim.c 
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of this file.
--------------------------------------------------------------------------------

Der Modul realisiert 25 mathematische Benchmark-Funktion der CEC'05-Sammlung.
Davon wurden die Funktionen f1 bis f15 getestet.
Benutzt wurden bisher f1-f7, f9-f12 und f15.

Bedeutung der Schalter 
MIT_CEC_MBFS: Die MBFs der CEC'05-Sammlung werden unabhaengig von den CEC-Expe-
              rimentierbedingungen zur Verfuegung gestellt.
CEC_MBF:      Aktivierung der CEC-Experimentierbedingungen (FEV-Limit=1.0e-8,
              (FEsMax=dim*10000, usw.)


Die Routinen im einzelnen:

BOOL do_cec05_mbf_sim (DOUBLE *result, 
                       INT     parAmount,
                       INT     fctNr,
                       DOUBLE *param);
          Fuehrt die durch "fctNr" bezeichnete Funktion aus und liefert den 
          Ergebniswert in "result" ab. 
          Die in "parAmount" enthaltene Anzahl der Parameter "param" wird mit 
          dem zu SIMU_MBF_X_ANZ_TXT gehoerigen Eintrag der TSK-Daten verglichen.
          Bei Nichtuebereinstimmung oder sonstiger Nichtbewertbarkeit liefert 
          die Routine FALSE sonst TRUE.
          Moeglicher Ausbau:
          ParamAnz und FileInit nur wenn fctNr anders als beim letzten Aufruf.

--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. Einfuehrung der Compilerschalter CEC_MBF und MIT_CEC_MBFS zur
    Deaktivierung der CEC-MBFs.                                      WJ 05.12.07
 2. Aktivierung von MIT_CEC_MBFS bei gesetztem CEC_MBF.              WJ 02.01.08
 3. "do_cec05_mbf_sim": Streichung des Vergleichs von "parAmount"
    mit der entsprechenden TSK-File-Angabe. Der in "get_ak_params" 
    aus "simu.c" angestellte Abgleich mit dem Genmodell genuegt.     WJ 26.10.16
--------------------------------------------------------------------------------
Status: impl                     Autor: W.Jakob                Datum: 26.10.2016
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "schalter.h"
#include "sys.h"
#include "fbhm.h"
#include "chaindef.h"
#include "aufg.h"
#include "simu.h"
#include "simuInt.h"

#undef MIT_CEC_MBFS        /* Mit MBFs von CEC'05 unabhaengig von CEC-ExpBed */


#if defined(CEC_MBF) || defined(MIT_CEC_MBFS)
  #ifndef MIT_CEC_MBFS
    #define MIT_CEC_MBFS
  #endif /* gesetztes CEC_MBF benoetigt MIT_CEC_MBFS */
  #include "global.h"
  #include "rand.h"
#endif /* CEC_MBF || MIT_CEC_MBFS */


/* ============================ Lokale Konstante: =========================== */



/* ======================== Lokale Typdeklarationen: ======================== */



/* ============================ Lokale Variable: ============================ */
INT  lastFctNr = -1;              /* Nummer der zuletzt verwendeten Funktion  */





/* =============================== do_mbf_sim =============================== */
BOOL do_cec05_mbf_sim (DOUBLE *result, 
                       INT     parAmount,
                       INT     fctNr,
                       DOUBLE *param)
{
   INT   idx;
   BOOL  erg, erg2;

   /* do_mbf_sim */
#if defined(CEC_MBF) || defined(MIT_CEC_MBFS)
   if (lastFctNr != fctNr)
   { /* andere Testfunktion als beim letzten Aufruf */
      if ((parAmount != 2) && (parAmount != 10) && (parAmount != 30) && (parAmount != 50))
      {
         fatal (SIMU_CEC_MBF, 2, parAmount, 50, "", BAD_PAR_AMOUNT);
         return (FALSE);
      }
      if (lastFctNr != -1)
         free_memory();
      nfunc = 2;                                       /* valid for f1 to f14 */
      nreal = parAmount;
      erg2  = TRUE;
      randomize();
      initrandomnormaldeviate();
      switch (fctNr) {
      case 1: 
         allocate_memory();
         erg = initialize_f1();
         break;

      case 2: 
         allocate_memory();
         erg = initialize_f2();
         break;

      case 3: 
         allocate_memory();
         erg = initialize_f3();
         break;

      case 4: 
         allocate_memory();
         erg = initialize_f4();
         break;

      case 5: 
         allocate_memory();
         erg = initialize_f5();
         break;

      case 6: 
         allocate_memory();
         erg = initialize_f6();
         break;

      case 7: 
         allocate_memory();
         erg = initialize_f7();
         break;

      case 8: 
         allocate_memory();
         erg = initialize_f8();
         break;

      case 9: 
         allocate_memory();
         erg = initialize_f9();
         break;

      case 10: 
         allocate_memory();
         erg = initialize_f10();
         break;

      case 11: 
         allocate_memory();
         erg = initialize_f11();
         break;

      case 12: 
         allocate_memory();
         erg = initialize_f12();
         break;

      case 13: 
         allocate_memory();
         erg = initialize_f13();
         break;

      case 14: 
         allocate_memory();
         erg = initialize_f14();
         break;

      case 15: 
         nfunc = 10;
         allocate_memory();
         erg = initialize_f15();
         if (erg)
            calc_benchmark_norm15();
         break;

      case 16: 
         nfunc = 10;
         allocate_memory();
         erg = initialize_f16();
         if (erg)
            calc_benchmark_norm16();
         break;

      case 17: 
         nfunc = 10;
         allocate_memory();
         erg = initialize_f17();
         if (erg)
            calc_benchmark_norm17();
         break;

      case 18: 
         nfunc = 10;
         allocate_memory();
         erg = initialize_f18();
         if (erg)
            calc_benchmark_norm18();
         break;

      case 19: 
         nfunc = 10;
         allocate_memory();
         erg = initialize_f19();
         if (erg)
            calc_benchmark_norm19();
         break;

      case 20: 
         nfunc = 10;
         allocate_memory();
         erg = initialize_f20();
         if (erg)
            calc_benchmark_norm20();
         break;

      case 21: 
         nfunc = 10;
         allocate_memory();
         erg = initialize_f21();
         if (erg)
            calc_benchmark_norm21();
         break;

      case 22: 
         nfunc = 10;
         allocate_memory();
         erg = initialize_f22();
         if (erg)
            calc_benchmark_norm22();
         break;

      case 23: 
         nfunc = 10;
         allocate_memory();
         erg = initialize_f23();
         if (erg)
            calc_benchmark_norm23();
         break;

      case 24: 
      case 25: 
         nfunc = 10;
         allocate_memory();
         erg = initialize_f24_f25();
         if (erg)
            calc_benchmark_norm24_25();
         break;

      default:
         fatal (SIMU_CEC_MBF, 3, fctNr, NO_INFO, "", BAD_EXT_SIMU_CODE);
         erg2 = FALSE;
      } /* switch */
      if (!erg)
         fatal (SIMU_CEC_MBF, 4, NO_INFO, NO_INFO, "",  NO_CEC_INI_FILE);
      erg &= erg2;
      if (erg)
         lastFctNr = fctNr;
   } /* andere Testfunktion als beim letzten Aufruf */
   else
      erg = TRUE;

   if (erg) 
   { /* Fkt ist initialisiert, also ausfuehren */
      switch (fctNr) { /* Verteiler */
      case 1:
         *result = calc_benchmark_func1 (param);
         break;

      case 2:
         *result = calc_benchmark_func2 (param);
         break;

      case 3:
         *result = calc_benchmark_func3 (param);
         break;

      case 4:
         *result = calc_benchmark_func4 (param);
         break;

      case 5:
         *result = calc_benchmark_func5 (param);
         break;

      case 6:
         *result = calc_benchmark_func6 (param);
         break;

      case 7:
         *result = calc_benchmark_func7 (param);
         break;

      case 8:
         *result = calc_benchmark_func8 (param);
         break;

      case 9:
         *result = calc_benchmark_func9 (param);
         break;

      case 10:
         *result = calc_benchmark_func10 (param);
         break;

      case 11:
         *result = calc_benchmark_func11 (param);
         break;

      case 12:
         *result = calc_benchmark_func12 (param);
         break;

      case 13:
         *result = calc_benchmark_func13 (param);
         break;

      case 14:
         *result = calc_benchmark_func14 (param);
         break;

      case 15:
         *result = calc_benchmark_func15 (param);
         break;

      case 16:
         *result = calc_benchmark_func16 (param);
         break;

      case 17:
         *result = calc_benchmark_func17 (param);
         break;

      case 18:
         *result = calc_benchmark_func18 (param);
         break;

      case 19:
         *result = calc_benchmark_func19 (param);
         break;

      case 20:
         *result = calc_benchmark_func20 (param);
         break;

      case 21:
         *result = calc_benchmark_func21 (param);
         break;

      case 22:
         *result = calc_benchmark_func22 (param);
         break;

      case 23:
         *result = calc_benchmark_func23 (param);
         break;

      case 24:
      case 25:
         *result = calc_benchmark_func24_25 (param);
         break;

      default:
         fatal (SIMU_CEC_MBF, 10, fctNr, NO_INFO, "", BAD_EXT_SIMU_CODE);
         erg = FALSE;
      } /* Verteiler */
   } /* Fkt ist initialisiert, also ausfuehren */
#else /* weder CEC_MBF noch MIT_CEC_MBFS */
   erg = FALSE;
  #ifdef GLEAM_USR
   fehler (SIMU_CEC_MBF, 11, NO_INFO, NO_INFO, "", NOT_IN_THIS_VERSION);
  #else /* kein GLEAM_USR, also Engine-Version */
   fatal (SIMU_CEC_MBF, 11, NO_INFO, NO_INFO, "", NOT_IN_THIS_VERSION);
  #endif  /* kein GLEAM_USR, also Engine-Version */
#endif /* weder CEC_MBF noch MIT_CEC_MBFS */
   return (erg);
} /* do_mbf_sim */

