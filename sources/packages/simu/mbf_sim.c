/*******************************************************************************
GLEAM/AE                                                                  V3.5.2

                               G L E A M  /  A E
       Genetic LEarning Algorithms and Methods / Application Environment

                         Package: SIMUlator
                          Module: Mathematische BenchMarkfunktions-SIMulator
 
Package: simu                     File: mbf_sim.c 
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------
Die Funktionen wurden z.T. uebernommen von:
  Copyright (c) 1990-1992
  Thomas Baeck
  Computer Science Department, LSXI
  University of Dortmund
  Baroper Str. 301
  D-4600 Dortmund 50

  e-mail: baeck@ls11.informatik.uni-dortmund.de

  Permission is hereby granted to copy all or any part of
  this program for free distribution.   The author's name
  and this copyright notice must be included in any copy.
--------------------------------------------------------------------------------

Der Modul realisiert 12 mathematische Benchmark-Funktion:
  Sphere, Weighted Sphere, Ackley, Fletcher, Shekel's Foxholes, generalized 
  Rastrigin, Fractal, Bracken, Beale, Bigg und Helical Valey sowie
  Rechenbergs "echtes multimodales Testproblem" aus "Evolutionsstrategie'94", 
  Frommann-Holzboog, Kap.12, S.154ff.

Ausserdem die Funktion "quick_and_dirty" zur schnellen Erprobung einfacher Test-
funktionen.

Alle Funktionen liefern einen Ergebniswert und eine Angabe ueber dessen Gueltig-
keit.

Anmerkung zur Implementierung:
Die Funktion "do_mbf_sim" muesste ueberarbeitet werden, da bei jedem Aufruf 
ein TSK-Fileparameter ueberprueft und ein temporaeres Feld angelegt wird.
Bei einer Ueberarbeitung ist auf folgendes zu achten:
1. Im interaktiven Kontext koennen durch Nachladen des MOD- oder des TSK-Files
   Aenderungen an der Parameteranzahl entstehen. Auch die Drehung koennte neu 
   vorgegeben werden. "init_mbf_sim" wird aber nur beim erstmaligen Lesen der
   Files aufgerufen. 
2. Die Anzahl an Realparametern eines Genmodells wird von "hmod" exportiert
   ("r_par_ges_anz") und sollte daher nicht aus dem TSK-File stammen.
3. Die Parameter muessen umkopiert werden, da das an "do_mbf_sim" uebergebene 
   Feld bei Lamarckscher Evolution in das Chromosom zurueckkopiert wird. Bei
   gedrehten Funktionen waere das fatal.


Die Routinen im einzelnen:

BOOL do_mbf_sim (DOUBLE *simu_erg, 
                 INT     r_par_anz, 
                 DOUBLE *r_params);
          Fuehrt die durch die globale Variable "ext_simu_code" bezeichnete 
          Funktion aus und liefert den Ergebniswert in "simu_erg" ab. 
          Die in "r_par_index" enthaltene Anzahl der Parameter "r_params" wird 
          mit dem zu SIMU_MBF_X_ANZ_TXT gehoerigen Eintrag der TSK-Daten ver-
          glichen. Bei Nichtuebereinstimmung oder sonstiger Nichtbewertbarkeit 
          liefert d. Routine FALSE sonst TRUE.

void init_mbf_sim (void);
          Wertet das TSK-File fuer eventuelle Drehungen und Verzoegerungen aus.


--------------------------------------------------------------------------------
Aenderung gegenueber Version 1.0.0:
 1. Da Borland C die Funktionen "lgamma" und "isnan" nicht kennt,  
    wird die MBF-Funktion "fractal" fuer BS_DOS || BS_WINXP_GNU de-
    aktiviert.
 2. Neue Funktion "rberg", die das "echte multimodale Testproblem" 
    von I.Rechenberg (Evolutionsstrategie 94, Kap12, S.154) reali-
    siert.
 3. Neue Funktionen fuer 2 Testfunktionen: Bracken u. McCormik 
     Beale. 
 4. HyGLEAM: An g++-Compiler angepasst u. "do_mbf_sim" globalisiert. WJ 11.08.00
 5. Anpassungen an neue TSK-Datenhaltung.                            WJ 24.04.02
 6. Optionale Rotation der KoordAchsen gesteuert durch das TSK-File. WJ 24.04.03
 7. Neue MBF "quick_and_dirty" zum schnellen Testen.                 WJ 13.11.03
 8. Anpassung an den "mecker'-Parameter von "get_id_idx".            WJ 27.11.03
 9. Die Drehung erfolgt im lokalen "x-werte"-Feld, damit die 
    "r_params" nicht verfaelscht werden. Alle bisherigen Ergebnisse 
    zu den rotierten MBF waren bei Lamarckscher Evolution falsch!    WJ 29.07.04
10. Linux-Portierung: Differenzierung zwischen Solaris und Linux.    WJ 11.05.05
11. Integration der Griewank-Funktion (F22).                         WJ 10.05.07
12. Neuer Schalter BS_WINXP_GNU fuer die Matlab-Version GLEAMKIT.    WJ 21.08.07
13. NOT_IN_THIS_VERSION in "do_mbf_sim".                             WJ 02.01.08
14. "fletcher" auf max. FLET_DIM (30) Dimensionen erweitert und 
    Fehlerstatus, wenn mehr angegeben werden. Bei Bereichsueber-
    schreitung der X-Werte erfolgt deren Begrenzung statt Fehler.    WJ 05.05.08
15. Foxholes: Korrektur der Definition der Matrix "a".               WJ 08.07.10
16. Entfernung der Schalter und des dazugehörigen Codes: BS_DOS, 
    BS_WINXP_GNU. Entfernung ueberfluessiger Variable. Explizite 
    Klammern zur eindeutigen Zuordnung von else-Zweigen.             WJ 27.06.12
17. Englische TSK-File-Texte.                                        WJ 05.04.16
18. "do_mbf_sim": Streichung des Vergleichs von "r_par_anz" mit
    mit der entsprechenden TSK-File-Angabe. Der in "get_ak_params" 
    aus "simu.c" angestellte Abgleich mit dem Genmodell genuegt.     WJ 26.10.16
19. Streichung von "delayInMsec". Anpassung der TSK-File-Parameter.  WJ 22.11.16
20. Fehlermeldung NOT_ENOUGH_MEM korrigiert.                         WJ 31.05.17
21. PAR_POP_VERS: Delay of "delayMSec" msec after MBF exexution for 
    performence measurements in the context of distributed popula-
    tions or simulevaluations.                                       WJ 24.01.19
--------------------------------------------------------------------------------
Status: o.k.                     Autor: W.Jakob                Datum: 24.01.2019
================================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "schalter.h"
#include "sys.h"
#include "ctio.h"
#include "fbhm.h"
#include "chaindef.h"
#include "hmod.h"
#include "aufg.h"
#include "simu.h"
#include "simuInt.h"

#include SIMU_TEXTS



/* ========================== Exportierte Variable: ========================= */
#ifdef PAR_POP_VERS
  INT     delayMSec = 0;                 /* delay after MBF execution in msec */
#endif /* PAR_POP_VERS */



/* ============================ Lokale Konstante: =========================== */
#define FLET_DIM  30                                       /* fuer "fletcher" */
#define EPSILON   1.0e-8                                   /* fuer "fractal"  */
#define IDXBND  100                                        /* fuer "fractal"  */
#define GCLIM     1.570796  /* gamma-cos limes for vicinity of 0. F."fractal" */

#define SQR(x) ((x)*(x))                                   /* fuer "foxholes" */


    /****************************************************************/
    /* For "fractal":                                               */
    /*     Rand computes a pseudo-random                            */
    /*     double value between 0 and 1, excluding 1.  Randint      */
    /*     gives an integer value between low and high inclusive.   */
    /*                                                              */
    /****************************************************************/
 
#define MASK ~(~0<<31)    /* 31 = INTSIZE - 1 */
#define PRIME 65539
#define SCALE 0.4656612875e-9
 
#define RBERG_IMAX  20
#define RBERG_KMAX  30



/* ======================== Lokale Typdeklarationen: ======================== */
typedef struct FITNESS_TYPE
  {
    BOOL    valid;
    DOUBLE  value;
  } FITNESS_TYPE;

typedef struct                                             /* fuer "fractal"  */
  {
    DOUBLE  FstVal,
            SndVal;
  } TwoVal;



/* ============================ Lokale Variable: ============================ */
static BOOL          mit_drehung;                          /* TRUE=Achsendreh.*/
static DOUBLE        sin_a, cos_a;                         /* Hilfsvar.f.Dreh.*/
static FITNESS_TYPE  result;

static double Avl[FLET_DIM][FLET_DIM] = {                 /* fuer fletcher */
    {-25.5668,  96.1559, -15.2661, -23.2470,  1.15129,   8.4544,  54.2205,  41.2237,  33.1251,   0.5674,
     -68.8670, -58.3628,  -8.3009, -97.6533,  61.1674, -77.2035, -29.0862,  99.6618,  44.4031,  16.4384,
      86.2657, -73.9821,  11.3594,  75.5782,  84.6617, -78.8448, -60.8731,  82.8905,  60.3778, -79.3920},
    {-97.5908, -82.9516,  50.3364, -96.5520,  29.4616,  30.3776, -35.5748,  62.4121,   3.3272, -74.3143,
      33.8170,  93.3556, -49.0037,  79.6870,  93.7838,  55.6858, -74.1750,  87.4147,  75.0636, -61.4719,
      63.7338,  26.9982, -15.0730, -69.9356,  32.9214, -79.7611,  95.5700,  89.6511, -84.5587, -12.0454},
    { 36.3096,  54.3934,  76.9586,  64.3481,  0.66188, -54.8080,  -0.7731,  38.8262, -19.8875, -89.8056,
     -56.2527,  88.8432, -67.5382, -14.8920,  31.3165, -38.3846,  85.5815,  67.6076, -30.3483, -82.2928,
      96.0474, -30.0304,  58.8901,  52.0675,  72.9875,  86.8610,  62.0540, -72.6880, -60.0433, -45.6785},
    {-58.7916,  30.4881, -22.9354,  36.1643,  62.2798, -27.6575, -15.3260, -91.7283, -11.2285,  37.5868,
     -36.3323,  63.2633,  40.9889,  31.5752, -39.6813, -77.9628,  22.0576,  12.5157, -67.4369, -13.9087,
      30.6985,  91.8151, -66.4065,  53.4701, -50.8032,  -9.1519,  -1.3816,  95.1191, -15.9998, -93.1303},
    { 66.5820, -18.2818,  2.30184, -38.6698, -80.3784, -77.5502,  63.9176, -46.7326, -12.8193, -95.1296,
      70.8507,   4.1149, -53.2598,  83.8560, -87.6349, -45.4270,  10.3677, -34.9786,   0.7164,  67.4950,
     -53.6057, -53.6364,  72.7498,  84.6396,  42.0975,  53.3332, -77.1209, -21.6982,  88.6348,  78.9552},
    {-83.0056,  55.2135, -41.2771, -28.7513, -57.3023, -96.3349, -83.6163, -54.7208, -12.1617, -18.8231,
     -34.7992, -94.8224, -82.8830,  58.5055, -31.6693, -56.6497,  41.9919,  68.8376, -63.2502, -93.5195,
      11.8233, -48.1354, -63.2165,  63.0207, -43.1697,  64.8390,  96.8477,  22.5408,   1.8269, -18.8980},
    { 57.6846,   7.0052, -37.2068,  75.9727,  99.4458,  86.9139,   8.2023,  65.9798,  60.1447,  15.9696,
     -11.1512, -83.5301,  28.7826,  36.8937,  42.0363,  51.5747, -73.5230,  14.1672, -10.1718, -83.1181,
      81.9438, -31.5601,  61.2066, -22.8853, -56.4041,  49.3356, -37.8706, -62.5471, -42.6852, -38.7399},
    { 36.4311,   7.2591,  62.7795, -60.9564, -51.7640, -31.0758,  36.3378,  10.9428, -29.8989,  52.9975,
      27.1767, -44.5979,  80.6937, -24.9984, -83.5122,  -9.6129, -85.2702,  71.2626,  75.0729,  81.0314,
      22.6285, -66.7474, -69.8582,  29.3805,  -2.5805, -74.4858,  81.8111,  51.4039,  24.5888, -35.0074},
    {-32.9766,  49.0229,  72.5696, -86.0361, -35.6027,  62.1559,  36.4388,   9.7572,   7.1633,  16.7999,
      97.1190,  27.7769,  40.2039,  64.6916,  88.2249,  96.2452,  66.8204,  -1.5407,  50.9365,  88.5003,
       4.8146,  -0.6243,  85.3820,  89.1591, -38.4269,  74.4450,   9.5431, -67.8695,  73.9404, -77.1593},
    { 51.3433,  50.1557,  -1.2103,  61.4950,  17.1698, -43.3526,  20.4658, -25.5906,  94.5613,  41.1613,
      25.9516, -65.8864, -94.7742,  30.5071, -69.9290,  96.3602, -22.8565, -80.4080, -18.1927,  75.9744,
     -45.4277, -28.5019,  97.5035, -26.2596, -81.2492,  27.5630,  24.0933,   7.9187, -90.4340, -51.4684},
    { -9.5379,  -8.3425, -17.8962,  60.9542,  40.5665,  37.0602, -82.3416,  33.3477,  19.1812, -78.5436,
      55.3852,  19.2438,  89.3514, -49.3889,  90.4063, -65.2128, -32.8058, -58.0835,  72.0236, -16.6919,
      60.1097,  38.3954, -39.9301,  85.1900, -37.6194,  77.5794,  89.7886, -54.0767,  -0.6971, -30.3815},
    {-59.4556, -83.3552,  -2.7112,  -6.4561, -11.1786,  28.4985, -99.1784, -27.6905,  -7.8080, -35.7676,
      43.8580, -75.4571,  42.1736, -42.3967, -72.4844, -32.0405,  37.2110, -35.9359, -49.9554, -64.1589,
      62.6173,  46.1464,  -0.3341,  -0.7725,  75.5001, -24.1654, -45.5082, -85.5278,  35.9409, -73.4020},
    { 63.1985,  55.1862,  10.5438, -47.2281,  10.2657,  71.5569, -86.3667, -82.5611, -66.5549,  46.4365,
     -89.3311, -93.7219, -30.1896,  99.9240, -43.1878, -30.9409,  -0.4828,  11.5134,   3.0889,  68.2245,
      50.6613,  75.9785,  98.4409,  31.6159,  69.8656,  46.3662, -73.5253,  40.7724,  75.0849, -79.2577},
    { 22.3266,  14.9017,  41.3544, -59.7104,  52.3621,  41.7597, -43.3001, -57.7845,  45.0891,  69.0617,
      82.1897,  74.5400,  26.5227,   3.1048,  -6.7357,  69.6854,  31.2315, -94.9626,  33.2976,  -6.0323,
      78.5002,  60.3449, -82.8027,  60.3350,  71.6421,  94.9062, -46.7451,  -2.6471, -99.1700,  83.3387},
    {-45.9626,   9.4820, -87.6991, -55.1796, -41.3643, -91.2712,   9.0335, -58.6719, -56.0784, -71.9978,
     -67.3405,  17.0430, -55.3583,  89.8320,  59.2962, -30.0319,  92.3010, -36.0288,  42.1098,  92.6137,
      10.1444, -67.0816,  22.8118,  13.5970, -66.5047,  40.2836, -11.8509, -87.5305,  56.7730, -66.4637},
    { -5.9080,  52.9336,  14.3159,  55.9834,  51.2989,  12.3811,  83.4429, -15.5508, -54.8860,  -3.4466,
      34.8757, -43.7298, -60.4750, -24.1424, -44.2770, -45.2479, -72.0023, -86.0935, -80.1677, -85.3819,
      88.8579,  69.2109,  93.7022, -77.6723,  75.9732,  18.3090, -64.3043,  -9.8261, -72.8998,  94.2576},
    { -7.4798,  13.1396,  55.7228,  70.2475, -81.5340, -75.4248, -36.0389,  -3.3212, -33.5127,  67.4607,
     -16.2518, -35.4920, -69.2546,  24.5574,  33.6469, -98.1548,  -9.2757, -99.0118,  35.2375,  89.0455,
      28.6060, -59.8047,  34.8852,  14.9544,   3.5134, -42.1028,  45.9734,  -0.8159,  85.8757,  -6.6505},
    { 88.5549,  18.6157, -71.9431,  63.2350, -63.8519,  37.6435,  -2.8650, -63.1337, -35.4598,  28.5594,
     -25.9018, -26.3366, -30.4831, -50.9430,  60.6065,  32.3062, -82.8811,  26.9319,  87.8704,  98.4041,
     -97.8536,  87.0577, -71.7558, -77.5487, -87.8534, -17.3173, -66.9090, -83.6543, -27.9201,  92.6749},
    {  2.7188, -55.4295,  29.4868, -10.1671, -93.7666,  50.6231,  34.5661, -65.3046, -40.3760,  81.3955,
     -19.4121, -93.0414,  16.0606,  78.5788, -42.9335, -54.1898, -21.8865,  78.9897,  -6.1999, -34.9914,
      -9.5843, -47.1283,  54.0751, -37.2207,  74.3109, -25.8332, -11.0290,  25.6151, -87.4779, -11.4054},
    { 63.8580,  84.5132,  49.2170, -31.1563, -11.9432,  18.3479,  34.5525, -68.3801, -52.4624,  98.8825,
     -13.5274,  64.6875,  89.9251, -37.1024, -32.0151,  64.9883,  25.5192,  97.2310, -34.7837, -71.3557,
      75.2294,  90.9652,   8.2715,  33.0115,  57.0748,  -5.0172,  89.4124,  98.4133,   1.1710, -77.3251},
    {-84.3970,  66.8585,  15.9723,   7.3975,  77.4030,  56.6574,  -3.8318, -76.3877,  44.0613, -19.2508,
     -81.6427,  -8.5597, -95.9258, -96.2852,  70.8912, -28.8659,  88.2076,  74.4768, -81.3350, -42.0989,
     -57.0911, -59.1917, -75.1788,  92.3817,  99.1645, -74.1622,   5.1592, -17.6938, -83.6714,  39.2190},
    { 46.4333,   1.4691,  11.5056, -14.3570,  -1.2314,  -1.1644, -69.8860,  15.6235,  80.3030,  59.6084,
     -82.2241,  98.3413, -12.7937, -51.6322, -58.6771, -40.2337,  65.8761,   9.4709, -70.7771, -37.1164,
      10.9339,  69.9417, -10.9917, -32.1331, -59.6580,  -6.1254,  69.3266, -78.6892, -41.6521, -25.6624},
    {  3.1746,  23.6396,  41.9915, -41.0754,  60.9748, -83.3522,  24.6377, -80.8560,  -0.2125,  38.5259,
      50.6392, -36.9747,  88.6366,  -2.9615, -57.9139,  16.5905, -31.7089, -86.2783,  58.1326, -34.2804,
      43.6458, -40.3411,  99.1511, -51.4089, -37.0339, -81.1964,  74.6683, -45.8996,  -6.2845,  25.7951},
    { 31.4906, -62.1426,  20.5952,  95.2368,  45.5789, -20.4199,  57.0573,  17.5767,  -9.8399,  29.6036,
     -55.7131, -12.2256, -21.5505,  24.0895, -93.0058, -32.7938, -29.4664, -24.7198, -83.0606, -17.2670,
      61.8707,  -7.6792, -15.2929,  31.1268, -79.9287, -22.0620, -62.2901,  89.3573,  94.8266, -64.5849},
    {-44.2329,   4.4238,  33.8730,  58.2354,  10.9890, -61.0382, -60.3366,  51.5350,  10.3612,  34.8538,
      94.0317,  30.7709,  59.7599,  96.2655,  61.7812,  80.0525,  77.6281, -78.7496, -87.8825,  42.3310,
      83.8562,  78.3108, -74.3672,   8.6212, -98.7218,  47.6543,  93.6961, -83.1884, -61.4832, -41.4137},
    { 79.3129, -53.8105,   6.6793, -47.6304, -40.3964, -24.0149,  69.7651,  97.4459, -21.0366, -19.9461,
      78.8482, -39.7543, -69.2283, -39.0531,  10.5268, -15.3507, -20.5824,  31.3351, -33.7044,  16.8420,
      11.7159, -34.0092,  15.0303,   5.8186,  48.9500, -49.1940,  27.4285,  54.7272,  -7.6282, -85.1723},
    { 88.9181, -48.1618,  53.6956, -44.2185,  44.2259,  95.0214,  28.7822,  58.4738,  58.6299,  78.6968,
     -98.7552,  -1.1725,  42.1552,  41.3899, -39.7896,  54.8003,  16.0011,  95.0025,  33.1200, -19.6731,
      75.8114, -40.9532,  73.5748,   1.6502,  72.6532,   5.3718, -87.7199,  69.4883, -33.4865, -52.0689},
    { 35.6680, -61.4089,  63.6349,  70.1261,  64.4976, -17.8359, -88.8967, -82.9318, -20.5512,   6.7890,
      50.9061,  55.2017, -98.6008,  78.8900,  20.4483,  46.2299,  52.7598,  62.1491,  -5.3592,  40.9078,
     -21.8481,  -7.1573,  -8.3399,  81.3614, -83.8512,  73.7869, -16.0462, -65.8561, -79.7739, -25.0457},
    { 69.7941,  51.8400, -88.9612, -91.9458, -51.4549,  85.9113,  -8.6386,  52.2097,  12.8073, -79.5122,
     -89.6849,  97.7611,   8.8826,  54.4359, -31.1959,  35.1927, -62.1918,  81.3562,  10.4638, -73.1035,
     -35.0691, -26.0605,  42.0834, -17.7646,  97.1385, -94.3435,  27.7219,  23.0664,  72.0634,  -9.9216},
    { 58.0373, -69.9348,   9.5870, -97.1898,  20.2640,  43.0321,   7.6819,   5.2914,  -2.5058, -65.3688,
     -44.9583, -88.5567, -25.7241, -38.8573,  -6.8047, -59.2215,  87.9631, -69.1666,  30.1460,  64.2533,
     -36.1689, -73.8756,   3.3976,  66.5552,  31.8779,  16.1644, -41.9702,  81.0254, -77.1897,  38.5994}
};

static double Bvl[FLET_DIM][FLET_DIM] = {                 /* fuer fletcher */
    {-38.9994, -49.1546, -52.2403,  66.4195, -36.6538,  68.9131,  77.0078,  61.3899,  90.8145,  46.9470,
     -64.3195,  51.6717, -80.1266,  57.2631,  96.5961, -81.6053, -54.4217, -99.3421, -83.9154,  22.7023,
      -8.0568,   4.2394,  88.8336,   2.4284,  78.9425,  97.6510, -50.5462, -55.5517, -30.6574,  59.6532},
    {-6.49429, -97.6865,  39.6734, -91.4542,  17.2329,  11.3941,  29.2592, -53.9791,  76.8088,   1.5392,
      99.3465, -24.6453,  10.1611, -88.8225,  -0.5497,  -5.4573,  93.8800, -25.6205, -21.8567, -61.7123,
     -53.2477, -14.6122,  88.8614, -73.1087, -53.8740,  25.2569, -99.4836,  97.9207, -44.3865,  48.1585},
    {-67.1681, -42.8325, -42.9307,  36.9705, -18.3463,  42.0040, -57.7744,   1.7690,   8.5977, -66.6249,
     -42.1059, -71.5274,  36.9813, -25.6398,  59.0088, -83.6213,   2.9082,  96.6437,  56.7289, -87.8477,
     -30.8228, -28.7810,  -4.1927,  70.0873,  26.4379, -19.1018,  98.3294,  46.9503, -84.1307, -91.7183},
    {-53.1271, -2.74637,  27.6244, -72.7537,  46.1798,  97.0102,  17.5367,  68.1148,  92.0471,  82.2297,
      79.5941,  99.3971, -86.7694,  30.1614, -93.2868,  64.3989,  85.3197, -95.4829,  78.9012, -60.1618,
      37.6529,  52.8830,  93.3388,  38.0495, -45.8571,  62.2727, -69.0914, -22.7219,  15.1556, -50.0905},
    {-1.47036, -95.2547,  78.6214, -54.3113,  84.6737,  54.2412,  81.4081,  -0.9394, -12.4624,  45.1189,
      78.6975,  44.2026,  74.5402, -13.8650,  14.6439,  17.6668, -38.7754,  64.6416,   1.8209, -46.8490,
      93.8568, -34.4957, -21.4829,  28.5968, -86.9370,  40.3934,  32.9996,  80.1597,  33.3424,  40.7836},
    {-68.4391, -29.0600, -99.4276, -96.8490,  42.1759, -67.9143,  44.0773, -70.2081,  13.1894,   5.0076,
      78.2319, -52.8647, -94.7967, -25.8177, -25.5259, -96.6151,   7.0869,  81.2095, -82.5793,  24.9379,
      32.2749, -53.6422, -81.4422, -49.4558,  93.3368, -59.1344, -81.6295,  45.9831, -89.0632, -98.0543},
    { 34.0804,  40.6330, -86.8990,  66.0947,  67.2882, -30.6480, -76.8593, -78.6119,  76.3040,  25.3059,
      86.2600, -94.5020,  34.9654, -13.3221,  28.0180, -34.0764,  77.0701,  55.5654, -79.7228, -31.8292,
     -12.3903,   1.9549,  15.5309,   5.3211,  13.8168, -71.2813, -72.3484, -99.3500,  86.2963,  32.4848},
    {-69.1134, -21.1078, -40.5088, -74.5059, -70.3244,  39.7210,  88.3495, -21.1797, -54.4269, -69.5356,
      81.8444,  49.3350, -34.1854, -31.4168, -67.8674, -58.5567,   8.1056,  -4.3121, -30.6137, -62.1027,
     -49.1213,  94.6881, -13.8666,  98.0544,  46.0462,  13.9987, -98.6839,  81.0333,  26.2559,  14.6390},
    {-26.0983,  97.7268, -89.4127, -12.2480,  50.0321, -49.9461,  36.8303,  34.8578,  48.3049, -23.1840,
      55.9659,  17.4887,   7.3656,  19.8172, -68.1913,  76.2983,  81.7348, -43.6054, -71.7984, -95.0092,
      59.4715,  48.2021, -64.4689, -92.6972, -36.6883, -57.8809,   1.1289, -65.2733, -99.6526, -33.2210},
    { 63.4416,  20.7696, -50.2042, -85.1855, -57.5795,  19.4853, -29.7575, -67.9969, -31.2505, -99.8779,
     -96.1704,  73.6976, -81.7269,  27.4397, -67.2446,  88.0085,  -3.0908, -96.8645, -28.6989, -25.9591,
     -47.3421,  95.5791, -36.7881,   0.0914,  97.9020, -66.4966,  99.0286, -12.7849, -54.4903,  78.3917},
    {-89.0520, -67.7701, -99.5980,  67.0902, -25.3653,   3.4260,  23.9587, -19.1226,  19.4188, -37.5079,
     -12.7388, -46.8574,  82.3891,  -3.9004, -48.3161,  81.4127, -59.4136, -43.0541,  28.3508, -79.7021,
      11.9660,  71.7483, -46.3058,  62.6885,  63.8345,  55.6882, -75.3545,  83.0501, -68.5268, -59.7147},
    {-72.6806,  37.8841,  94.6930, -36.1278,  96.5416, -33.5950,  41.7768, -41.8619,   4.9204,  84.4371,
     -35.0541, -13.8831, -90.2511,  44.7750,  66.0581,  20.1421,   9.8042, -20.7858, -66.7642,  -5.5579,
     -26.3009, -74.2650,  -3.2974,   6.0816, -95.9448,  66.5834, -55.9334, -22.1214,  70.2661, -99.9196},
    { 21.0374,  23.8746,  65.4730, -15.1673, -46.8561,  86.1639, -25.3079,  26.7003, -21.9049, -66.2154,
     -49.4698, -73.4232,  -1.7154, -49.5627,  15.9687,  24.0979,  60.4968, -13.3288,   1.2433,  15.2370,
      97.6628, -59.3892,  90.5200,  -0.0407, -31.2873,  64.1779, -63.9255,  42.3771,  -9.0491, -97.9042},
    { -9.1101,  65.9960, -50.6298, -30.1145,  28.4601,  29.6207, -96.0469,   3.8262, -59.7654, -42.6480,
     -95.0555,  87.8445,  21.2857,  54.0735, -86.3845,  25.0257, -66.2266, -95.8093,  49.0828,  98.5304,
     -48.3925, -92.3836, -82.1592, -90.0762,  79.9546, -12.3555,  -0.0267, -77.8491, -32.8554,  48.1335},
    { 73.3182, -72.3658,   6.9418,  53.1314, -46.4126,  45.3660,   4.5413, -55.3173,  54.6248,  88.6660,
     -45.8716, -48.8862,  59.2866,  79.8628, -30.8694, -81.1884,   9.1389,  -9.9522,   3.7428,  28.6926,
     -67.8004,  -0.1360,   6.9000, -59.3317,  34.8470, -12.2009,  24.2016,   5.0042, -20.0616,  10.5271},
    { -8.4071, -48.7494,  -8.0850, -71.5042,  94.7279,  29.2028,  95.6387,  10.3277,  63.3510, -48.9170,
      73.9329,  10.9297,  59.8458,  41.8868,  74.9703, -72.0986, -49.9657, -79.1940,  11.0312,  94.3737,
      22.7198, -60.8578,  51.6808,  37.4511,  78.3264,  20.6080,  54.6074, -78.2798,  27.9045,  72.9525},
    {-85.6021,  86.7121, -61.0356,  53.1244,   2.1882,  83.7675, -93.0741, -87.4608,  66.8297, -92.4039,
       8.9066,  68.3756,  60.9936, -84.8119,  64.2128,  38.2711,  24.1679, -77.9780, -47.0228,  39.5078,
     -79.9426,  83.2213,  66.2531,  43.0992,   0.4115,   4.0712,  28.5069, -55.4494,   3.2987,   6.8868},
    { 63.4464, -23.9541,  34.4927, -85.5103,  -1.2895, -47.9052, -90.2742,  27.5288, -78.0195,  78.0942,
     -72.4987, -15.3087,  24.0235, -72.2555, -88.0042,  40.7527,  72.1570, -67.1024, -81.0311,  94.8002,
     -54.6573, -46.6102,  80.8400,  95.3297,  89.9462, -16.9530,  17.0849,  63.9009,  99.0951, -17.2076},
    {-69.5942,  66.3967, -79.8500,  47.2058,  15.8855, -49.4491, -88.7163, -55.3280,  38.3534, -81.7075,
      22.4052,  14.2394,  81.8650,  47.7408,   6.1369, -51.5737,  84.5867,  86.6234, -24.6394,  87.5418,
     -26.8713, -51.1297,  34.7152, -52.1858, -26.2161, -70.3349,   9.7125,   6.0764,  66.1158, -40.0584},
    { 79.8023,  74.3736, -40.5637,  25.9589, -42.6155, -21.7773,   8.0884, -61.9451, -74.3613,  65.0448,
     -29.1119, -72.0253,   1.9908,  30.3384, -12.0061, -52.5774,  94.5294, -22.5866,  59.9143,  20.9670,
      -5.1795,  94.0184, -76.0256, -58.4981, -52.9156, -88.8186,  29.6579, -18.3429, -39.0637,  34.3507},
    {-97.2898,   3.4793, -81.1717, -64.0778,  86.2906, -16.8622, -38.4213, -43.9579, -26.0534, -92.7787,
     -76.8595,  62.8700, -37.4528,  50.3979, -98.0800, -84.8967, -97.0293,  96.9404,  88.9651,  92.4385,
       2.3257,  35.1271, -22.6856,  10.8573, -85.5333,  26.2076, -78.5438, -80.6404, -33.1679, -51.0334},
    { 36.5411, -63.6480,  23.0911, -54.4254,  74.6991, -16.2204,  25.2888,  71.1295,   7.1398,  24.8303,
      39.2779,  82.9529,   7.8775,  19.3429, -72.9856, -70.6763,  75.7879,  45.7308, -36.8082, -77.9703,
      59.7299,  68.2405, -83.2781, -57.8341, -54.2270,  75.7611,  35.4173, -48.6181, -79.1309, -91.6037},
    {-48.8284,   6.7846,   3.9630,  98.9089,  -0.1194,  30.5845, -90.4902, -30.5932,   2.8532,  17.0677,
      21.2790, -26.1768, -47.3307,  -0.5093,  63.3817, -98.4513,  95.1615,   6.0179,  83.5290,  47.8211,
      99.8272, -57.0051,  49.0781,   6.1120,   3.5688, -80.0638,   3.5587,  -6.1565,  27.8640,  15.9711},
    { 59.8178, -64.4893,  78.5868, -47.3370, -98.9692,   0.0054, -97.3618, -68.2845,  23.5743,  28.0721,
      -1.1525,  94.8626, -47.2566,  50.9547, -44.5707, -77.9003, -24.2450, -44.9037,  46.4264,  86.3462,
     -85.7493, -93.9232,  89.4369, -11.2218, -35.9150,  26.7270, -88.4355,  98.6854,  92.6502, -98.3510},
    { 45.6006, -17.8247, -51.9181, -71.6327,  80.2394,  20.8760,  10.7528,  82.3707, -96.4446,  68.6080,
      31.4816,  40.8205, -64.4276, -27.4299,  44.4538,   2.2675, -38.3037,  75.7853,  23.2016,  35.0320,
      12.8480,  27.4016,  37.9727,  25.7842,  39.7441,  43.0592, -39.0941, -73.7489, -12.0374,  93.1902},
    {-94.2664, -93.0461, -22.1361,  45.1806,  50.9622,  22.6305, -49.5945,  66.3126, -84.9911,  27.8508,
     -31.6935,   6.9512,  79.5575,   9.7234, -85.5831, -45.7079, -89.9513, -62.8951, -51.2963, -18.9574,
     -68.0334, -24.9646, -44.2436,  43.6630, -13.7276, -54.9252,  10.5636,  22.3863,  20.4419,  95.7322},
    { 26.5521, -20.5765,  52.2000, -95.6512,  32.1766, -40.0771, -34.0780, -12.1984,  -3.0064,  17.9010,
     -72.8501, -67.4389, -67.9442,  40.1305,   4.7333, -53.5895,   0.1666, -83.3910, -85.6652, -80.9169,
      57.2010,  -3.3648,  98.6812,  94.8958, -84.8799, -67.3156, -38.1624,  -2.0314,  69.8874, -13.6613},
    { 15.4106,   3.7723,  13.2934,  15.7513, -21.9511, -33.3980,  77.3468,  24.8836, -84.3643,  41.3354,
      14.7203, -98.4360,   3.5023,  82.8200,  60.6208,  52.9050,  34.8203, -26.1178,  52.9671,  39.5421,
     -42.3558, -17.1540, -97.9688, -30.5940,  79.1137, -53.7292, -68.4511, -79.6838, -23.0758,  28.1987},
    { 85.1260,  79.7203, -31.1753, -57.5186, -30.3898,   8.6616, -60.3024,  11.8041,  46.6234,  22.9155,
     -42.5576, -48.9606,  80.9414, -67.2947,  33.7800,  31.8641,  -3.2082, -32.8829, -69.8093, -58.3960,
     -92.0549,  32.0488, -35.9035,  76.6002,  -7.1356,  91.1198,  43.3272,  -3.2748,  40.9539,  25.4574},
    { 20.7208,  53.8169, -50.4942, -93.2157,   3.8755, -63.2882,  -6.6613,  23.0245, -68.3019,  46.2945,
     -98.9922,  19.1009,   9.2074, -74.2467,  74.1392, -21.7727,  10.4016,  77.3448,  -8.1923,  69.4324,
      66.8651,  53.8815,   2.4847, -47.9526, -26.2313,  30.4319, -34.9122,  62.3359,  97.2430, -47.9482}
};

static double Aph[FLET_DIM] = {                           /* fuer fletcher */
      1.98961, -1.39376, -2.03048, -0.62102,  3.12736,  1.98527, -2.68515,  1.34992, -0.24774,  0.58126,
      0.64749, -1.17108,  1.66009,  1.45035,  0.74294,  0.01326,  2.29768, -2.79613,  0.39295, -2.58240,
      0.83980,  1.61011, -1.39901,  1.38305,  3.10030, -2.14051,  0.50105, -2.99704,  2.82415, -0.35426
};


static int a[2][25] ={                                     /* fuer "foxholes" */
    {
        -32, -16,   0,  16,  32, -32, -16,   0,  16,  32, -32, -16,  0, 16, 32,
        -32, -16,   0,  16,  32, -32, -16,   0,  16,  32 },
    {
        -32, -32, -32, -32, -32, -16, -16, -16, -16, -16,   0,   0,  0,  0,  0,
         16,  16,  16,  16,  16,  32,  32,  32,  32,  32 }
};

static  DOUBLE  K = 500.0;                                 /* fuer "foxholes" */

static  DOUBLE *RndAng[2 * IDXBND + 1];  /* random phase angles for "fractal" */

static const double omega =   6.28318530717958647688,      /* fuer "rast"     */
                    amp   =  10.0;

static       BOOL   rberg_init_done = FALSE;               /* fuer "rberg"    */
static       INT   *rberg_z;                               /* fuer "rberg"    */





/* ============================ Lokale Routinen: ============================ */
static void         n_rot    (DOUBLE *r_params, 
                              INT     r_par_anz);

static FITNESS_TYPE sphere   (DOUBLE  *x,
                              INT      n);

static FITNESS_TYPE wsphere  (DOUBLE  *x,
                              INT      n);

static FITNESS_TYPE ackley   (DOUBLE  *x,
                              INT      n);

static FITNESS_TYPE fletcher (DOUBLE  *x,
                              INT      n);

static FITNESS_TYPE foxholes (DOUBLE  *x,
                              INT      n);

static FITNESS_TYPE fractal  (DOUBLE  *x,
                              INT      n);


static FITNESS_TYPE rast     (DOUBLE  *x,
                              INT      n);

static FITNESS_TYPE rberg    (DOUBLE  *x,
                              INT      n);

static void         rberg_init (void);

static BOOL         brack    (DOUBLE  *x,
                              DOUBLE  *erg);

static BOOL         beale    (DOUBLE  *x,
                              DOUBLE  *erg);

static FITNESS_TYPE bigg     (DOUBLE  *x,
                              INT      n);

static FITNESS_TYPE hvalley  (DOUBLE  *x,
                              INT      n);

static FITNESS_TYPE griewank (DOUBLE  *x, 
                              INT      n);

static FITNESS_TYPE quick_and_dirty (DOUBLE  *x,
                                     INT      n);



/* ================================== n_rot ================================= */
static void n_rot (DOUBLE *r_params, 
                   INT     r_par_anz)
{
   INT  ii, jj, r_anz;
   double xx;

   /* n_rot */
   r_anz = r_par_anz - 1;
   for (ii = 0;  ii < r_anz;  ii++)
   {
      jj = ii + 1;
      xx = r_params[ii];
      r_params[ii] = xx * cos_a - r_params[jj] * sin_a;
      r_params[jj] = xx * sin_a + r_params[jj] * cos_a;
   }
} /* n_rot */



/* =============================== do_mbf_sim =============================== */
BOOL do_mbf_sim (DOUBLE *simu_erg, 
                 INT     r_par_anz, 
                 DOUBLE *r_params)
{
   DOUBLE       *x_werte;
   INT           idx, ii;
   FITNESS_TYPE  erg;

   /* do_mbf_sim */
   if ((x_werte = (DOUBLE*)malloc(r_par_anz * sizeof (DOUBLE))) == NULL)
   { /* Nicht genug Speicherplatz */
      fatal (SIMU_MBF, 2, NO_INFO, NO_INFO, X_VAL_TXT, NOT_ENOUGH_MEM);
      return (FALSE);
   } /* Nicht genug Speicherplatz */

   for (ii = 0;  ii < r_par_anz;  ii++)            /* r_params lokal kopieren */
      x_werte[ii] = r_params[ii];
   if (mit_drehung)   
      n_rot (x_werte, r_par_anz);                  /* Drehung durchfuehren    */

   switch (ext_simu_code)
   { /* Verteiler */
      case MBF_SPHERE:
      erg = sphere (x_werte, r_par_anz);
      break;

      case MBF_WSPHERE:
      erg = wsphere (x_werte, r_par_anz);
      break;

      case MBF_ACKLEY:
      erg = ackley (x_werte, r_par_anz);
      break;

      case MBF_FLETCHER:
      erg = fletcher (x_werte, r_par_anz);
      break;

      case MBF_FOXHOLES:
      erg = foxholes (x_werte, r_par_anz);
      break;

      case MBF_FRACTAL:
#if defined(BS_SOLARIS) || defined(BS_LINUX)
      erg = fractal (x_werte, r_par_anz);
#else  /* Kein Solaris oder Linux */
      erg.valid = FALSE;
      fehler (SIMU_MBF, 3, NO_INFO, NO_INFO, "", NOT_IN_THIS_VERSION);
#endif /* Kein Solaris */
      break;

      case MBF_RAST:
      erg = rast (x_werte, r_par_anz);
      break;

      case MBF_RBERG:
      erg = rberg (x_werte, r_par_anz);
      break;

      case MBF_BRACK:
      erg.valid = brack (x_werte, simu_erg);
      break;

      case MBF_BEALE:
      erg.valid = beale (x_werte, simu_erg);
      break;

      case MBF_BIGG:
      erg = bigg (x_werte, r_par_anz);
      break;

      case MBF_HVALLEY:
      erg = hvalley (x_werte, r_par_anz);
      break;

      case MBF_GRIEWANK:
      erg = griewank (x_werte, r_par_anz);
      break;

      case MBF_QUICK_DIRTY:
      erg = quick_and_dirty (x_werte, r_par_anz);
      break;

      default:
      fatal (SIMU_MBF, 4, NO_INFO, NO_INFO, "", NOT_YET_IMPLEMENTED);
      erg.valid = FALSE;
   } /* CASE */
   if ((ext_simu_code < MBF_BRACK) || (MBF_BEALE < ext_simu_code))
      simu_erg[0] = erg.value;
   free (x_werte);
#ifdef PAR_POP_VERS
   warte(delayMSec);                     /* delay after MBF execution in msec */
#endif /* PAR_POP_VERS */
   return (erg.valid);
} /* do_mbf_sim */



/* ================================= sphere ================================= */
static FITNESS_TYPE sphere (DOUBLE  *x,
                            INT      n)
{
   int  i;

   result.valid = 1;
   result.value = 0.0;
   for (i = 0;  i < n;  i++, x++) 
      result.value += (*x * *x);
   return (result);
} /* sphere */



/* ================================= wsphere ================================ */
static FITNESS_TYPE wsphere (DOUBLE  *x,
                             INT      n)
{
   int  i;

   result.valid = 1;
   result.value = 0.0;
   for (i = 0;  i < n;  i++, x++) 
      result.value += (*x * *x * ((DOUBLE)(i+1)));
   return (result);
} /* wsphere */



/* ================================= ackley ================================= */
static FITNESS_TYPE ackley (DOUBLE  *x,
                            INT      n)
{
   register int  i;
   DOUBLE    A, B, D,
                 Val1,
                 Val2,
                 Sum;

   A = 20.0;
   B = 0.2;
   D = 2*M_PI;
   result.valid = 1; 
   for (Val1 = Val2 = 0.0, i = 0;  i < n;  i++) 
   {
      if (x[i]< -32.768 || x[i]>32.768 ) 
         result.valid = 0;

      Val1 += x[i] * x[i];  
      Val2 += cos(D * x[i]);
   }
   Sum = - A * exp(-B * sqrt(Val1 / n)) - exp(Val2 / n) + A + M_E;

   result.value = Sum;
   return (result);
} /* ackley */



/* ================================ fletcher ================================ */
static FITNESS_TYPE fletcher (DOUBLE  *x,
                              INT      n)
{
   register int  i,
                 j;
   DOUBLE    Res,
                 Val1,
                 Val2;

   if (n > FLET_DIM) {
      result.valid = 0;
      result.value = 100000.0;
   }
   else { /* ok */
      result.valid = 1;
      for (Res = 0.0, i = 0; i < n; i++) 
      {
	 if ( x[i]< -M_PI || x[i]> M_PI )
	 {
	    if (x[i]< -M_PI)   /* im Original ungültig */
	       x[i] =  -M_PI;
	    else
	       x[i] =   M_PI;
	 }
	 for (Val1 = Val2 = 0.0, j = 0;  j < n;  j++) 
         {
	    Val1 += (Avl[i][j] * sin(Aph[j]) 
		     + Bvl[i][j] * cos(Aph[j]));
	    Val2 += (Avl[i][j] * sin(x[j]) + Bvl[i][j] * cos(x[j]));
	 }
	 Res += (Val1 - Val2) * (Val1 - Val2);
      }
      result.value = Res;
   } /* ok */
   return (result);
} /* fletcher */



/* ================================ foxholes ================================ */
static FITNESS_TYPE foxholes (DOUBLE  *x,
                              INT      n)
{
   register int  i,
                 j;
   DOUBLE        Sum = 0.0,
                 Val,
                 Dif;

   for (Sum = 1.0 / K, j = 0;  j < 25;  j++) 
   {
      for (Val = j + 1, i = 0;  i < n;  i++) 
      {
         Dif  = x[i] - a[i][j];
         Val += pow(Dif, 6.0);
      }
      Sum += 1.0 / Val;
   }
   result.valid = 1;
   result.value = 1.0 / Sum;
   return (result);
} /* foxholes */


/* ===========================================================================*/
#if defined(BS_SOLARIS) || defined(BS_LINUX)

static double WMF(double *x, 
                  int     n, 
                  int     Bnd,
                  double  Dim, 
                  double  b,    
                  int     Mode, 
                  int     NewTrd, 
                  int     ElmTrd);

static double Twmf (int     Idx, 
                    double  x, 
                    int     Bnd, 
                    double  Dim, 
                    double  b, 
                    int     ElmTrd, 
                    int     Mode);

static double wmf (int     Idx, 
                   double  x, 
                   int     Bnd,
                   double  Dim, 
                   double  b,   
                   int     Mode);

static double ClcSum (double  x, 
                      int     XIdx, 
                      int     VIdx, 
                      double  Fst, 
                      double  Snd, 
                      int     Mode);


/* ================================= fractal ================================ */
static FITNESS_TYPE fractal (DOUBLE  *x,
                             INT      n)
{
   static      int     Flg = 1;
   static      double  M_2PI = 2.0 * M_PI;
   register    int     i, j;

   if (Flg) 
   {
      /*
       *  randomly initialize (IDXBND * 2 + 1) * n  angles 
       *  in the range [0, 2pi] and store them in RndAng.
       */

      for (i = 0; i < 2 * IDXBND + 1; i++) 
      {
         if ((RndAng[i] = (double *) calloc ((unsigned) n,
                         sizeof(double))) == NULL) 
         {
            fatal (SIMU_MBF, 5, NO_INFO, NO_INFO, "fractal function", NOT_ENOUGH_MEM);
            result.valid = 0;
            return (result);
         }
         for (j = 0; j < n; j++) 
            RndAng[i][j] = M_2PI * (double)frand();
      }
      Flg    = 0;
   }

   /*
    *  options for the three last parameters in the following call:
    *  general topics:          0 no angles
    *                           1 random angles
    *  superposition function:  0 sphere model
    *                           1 exponential
    *                           2 nothing
    *  trend elimination:       0 subtraction as described in [BL80]
    *                           1 division method by Schwefel
    *                           2 nothing
    */

   result.value = WMF(
      x,    /* object variable vector */
      n,    /* dimension of x         */
      IDXBND,   /* bound for index values */
      1.1,      /* Box Dimension          */
      1.5,      /* value of constant b    */
      0,        /* 0 modus of angle determ. */
      0,        /* 0 superposition function */
      1         /* 1 trend elimination      */
      );
   result.valid = 1;
   for (i = 0; i < n; i++) 
      if ( x[i]< -5 || x[i]> 5 ) 
      {   
         result.valid = 0;
         break;
      }

   return (result);
} /* fractal */



double WMF(double *x,       /* objective variables */
           int     n,       /* dimension of x[] */
           int     Bnd,     /* index bound */
           double  Dim,     /* box dimension */
           double  b,       /* value of b */
           int     Mode,    /* modus for angle determination */
           int     NewTrd,  /* new added trend */
           int     ElmTrd)  /* old trend to eliminate */
{

    double      Res;        /* result value */
    register int    i;

    /*
     *  calculate function values by calling Twmf, and
     *  add a new trend on the result.
     */

    for (Res = 0.0, i = 0; i < n; i++) {
        Res += Twmf(i, x[i], Bnd, Dim, b, ElmTrd, Mode);
        switch (NewTrd) {   
            case 0:     /* sphere model */
                Res += x[i] * x[i];
                break;

            case 1:     /* exponential function */
                Res -= exp(- x[i] * x[i]);
                break;

            default:    /* no trend */
                break;
        }
    }

    return(Res);

} /* end WMF */


double Twmf (int     Idx,       /* x vector index */
             double  x,         /* x value */
             int     Bnd,       /* index bound */
             double  Dim,       /* box dimension */
             double  b,         /* value of b */
             int     ElmTrd,        /* old trend to eliminate */
             int     Mode)      /* modus for angle determination */
{

    static  int Flg = 1;    /* flag for first call of Twmf */

    static  double  MTd,        /* multiplicative trend factor */
            STd;        /* subtractive trend summand */
            

    double      Nor,        /* normalization value */
            Res,        /* result value */
            Val1,
            Val2;

    if (Flg) {
        
        /*
         * multiplicative trend calculation according to
         * Schwefel's suggestion, May 1991
         */

        MTd  = wmf(Idx, 1.0, Bnd, Dim, b, Mode);

        /* 
         * subtractive trend calculation according to the trend
         * expression given by Berry and Lewis [BL80]
         */

        if (fabs(Dim - 1.0) < EPSILON)  /* exception case */
            STd  = GCLIM / log(b);
        else {
            Val1 = cos((2.0 - Dim) * M_PI / 2.0) / 
                   ((2.0 - Dim) * log(b));
            Val2 = lgamma(Dim - 1.0);
            STd  = exp(Val2) * Val1;
        }
        Flg  = 0;
    }
    
    
    switch (ElmTrd) {   /* eliminate old trend */
        case 0:         

        /*
         *  Subtract the trend according to the formula in [BL80].
         *  Be aware of the divergence problems caused by the trend 
         *  expression for Dim->2 as well as for b->1 !
         */

            Nor = STd * pow(fabs(x), 2.0 - Dim);
            Res = wmf(Idx, x, Bnd, Dim, b, Mode) - Nor;
            break;

        case 1:         

        /*
         *  Divide by trend as calculated by Schwefel. 
         *  The original function is symmetric w.r.t the origin, so 
         *  even the trend must be symmetric, i.e. |x| must be used in
         *  the normalization procedure. Also subtract 1 to have a
         *  function value of 0 at the global optimum.
         *  The exception case x->0 is handled by the trick to
         *  calculate first and then to test for division by zero
         *  (the isnan call below).
         */

            Nor = MTd * pow(fabs(x), 2.0 - Dim); 
            Res = wmf(Idx, x, Bnd, Dim, b, Mode) / Nor - 1.0;
            break;

        default:        

        /* 
         *  No enttreding measure is performed in this case.
         */

            Res = wmf(Idx, x, Bnd, Dim, b, Mode);
            break;
    }

    return (isnan(Res) ? 0.0 : Res);

} /* end Twmf */


double wmf (int     Idx,        /* x vector index */
            double  x,          /* x value */
            int     Bnd,        /* index bound */
            double  Dim,        /* box dimension */
            double  b,          /* value of b */
            int     Mode)       /* angle determination modus */
{   
    static   int        Flg = 1;
    static   TwoVal    *ValArr;

    register int        i,
                        k1,
                        k2;

    register double     Old,
                        Sum,
                        Sum1,
                        Sum2;


    if (Flg) {  /* calculate fixed values and store them */
        if ((ValArr = (TwoVal *) calloc ((unsigned) (2 * Bnd + 1), 
                         sizeof(TwoVal))) == NULL) {
            printf("No space allocated\n");
            exit(1);
        }
        for (i = 0; i < 2 * Bnd + 1; i++) {
            ValArr[i].FstVal = 
                pow(b, (double) (-Bnd + i));
            ValArr[i].SndVal = 
                pow(b, ((double) (-Bnd + i)) * (2.0 - Dim));
        }
        Flg  = 0;
    }

    for (Sum = ClcSum(x, Idx, Bnd, ValArr[Bnd].FstVal, ValArr[Bnd].SndVal, Mode),
            Old = 0.0,
            i   = 1;
         (i  <= Bnd) && (fabs(Sum - Old) > EPSILON * fabs(x));
         i++) {
       Old   = Sum;
       k1    = Bnd + i;
       k2    = Bnd - i;
       Sum1  = ClcSum(x, Idx, k1, ValArr[k1].FstVal, ValArr[k1].SndVal, Mode);
       Sum2  = ClcSum(x, Idx, k2, ValArr[k2].FstVal, ValArr[k2].SndVal, Mode);
       Sum  += (Sum1 + Sum2);
    }
    return(Sum);
} /* end wmf */



double ClcSum (double  x,           /* x value */
               int     XIdx,        /* index of value x */
               int     VIdx,        /* index in the ValArr */
               double  Fst,         /* FstVal in ValArr */
               double  Snd,         /* SndVal in ValArr */
               int     Mode)        /* calculation mode */
{
    static double   M_2PI = 2.0 * M_PI;
    double          Res,        /* function result */
                    Phi,        /* phase angle */
                    Val1,
                    Val2;

    switch (Mode) {
        case 0:     

        /*
         *  The deterministic variant with all angles set 0.
         */

            Res = (1.0 - cos(fmod(Fst * x, M_2PI))) / Snd;
            break;
        case 1: 

        /*
         *  The stochastic variant. All angles have been calculated
         *  in f_13 and are used here to evaluate the more general
         *  expression based upon the real part of the Weierstrass-
         *  Mandelbrot function.
         */

            Phi  = RndAng[VIdx][XIdx];
            Val1 = cos(Phi) * (1.0 - cos(fmod(Fst * x, M_2PI)));
            Val2 = sin(Phi) * sin(fmod(Fst * x, M_2PI));
            Res  = (Val1 + Val2) / Snd;
            break;

        default:

        /*
         *  Again the simpler deterministic variant.
         */

            Res = (1.0 - cos(fmod(Fst * x, M_2PI))) / Snd;
            break;
    }
    return(Res);

} /* end ClcSum */
#endif /* BS_SOLARIS oder BS_LINUX */



/* ================================== rast ================================== */
static FITNESS_TYPE rast (DOUBLE  *x,
                          INT      n)
{
   register  INT i;
   DOUBLE    fx = 0.0;

   /* rast */
   for (i = 0;  i < n;  i++) 
      fx += x[i] * x[i] - amp * cos(omega * x[i]);
   result.valid = 1;
   result.value = fx + n * amp;
   return (result);
} /* rast */



/* ================================== rberg ================================= */
static FITNESS_TYPE rberg (DOUBLE  *x,
                           INT      n)
{ 
   INT     ii, kk;
   DOUBLE  erg = 0.0,
           sum = 0.0;

   /* rberg */
   if (!rberg_init_done)
      rberg_init ();
   if (rberg_init_done)
   { /* Init ok */
      for (ii = 0;  ii <= RBERG_IMAX;  ii++)
      { /* i-Schleife */
         for (kk = 0;  kk < RBERG_KMAX;  kk++)
            sum += fsqr ((x[kk] - rberg_z[30*ii + kk+1]) / 30); /* k-Schleife */
         erg += (100 - ii) * exp (-sum);
      } /* i-Schleife */
      result.value = erg;
      result.valid = TRUE;
   } /* Init ok */
   else
      result.valid = FALSE;
   return (result);
} /* rberg */



/* =============================== rberg_init =============================== */
static void rberg_init (void)
{
   INT  ii, jj, z_anz;

   /* rberg_init */
   z_anz = (RBERG_IMAX + 1) * RBERG_KMAX;
   if ((rberg_z = (INT*)malloc((z_anz + 1) * sizeof(INT))) == NULL)
      fatal (SIMU_MBF, 6, NO_INFO, NO_INFO, "Rechenberg's function", NOT_ENOUGH_MEM);
   else
   { /* Feld angelegt */
      rberg_z[0] = 1;
      ii         = 0;
      for (jj = 1;  jj <= z_anz;  jj++)
      {
         rberg_z[jj] = (32 * rberg_z[jj-1] + 13*(ii+1)) % 31;
         if ((jj % 30) == 0)
            ii++;
      }
      rberg_init_done = TRUE;
   } /* Feld angelegt */
} /* rberg_init */



/* ================================== brack ================================= */
static BOOL brack (DOUBLE  *x,
                   DOUBLE  *erg)
{ 
   erg[0] = - fsqr(x[0]) - fsqr(x[1]);
   erg[1] = 1.0 - x[0];
   erg[2] = 5.0 - x[0] - 4.0*x[1];
   return (TRUE);
} /* brack */



/* ================================== beale ================================= */
static BOOL beale (DOUBLE  *x,
                   DOUBLE  *erg)
{
   erg[0] = 2.0*fsqr(x[0]) + 2.0*fsqr(x[1]) + fsqr(x[2]) + 2.0*x[0]*x[1] +
            2.0*x[0]*x[2] - 8.0*x[0] - 6.0*x[1] - 4.0*x[2] + 9.0;
   erg[1] = 3.0 - x[0] - x[1] - 2.0*x[2];
   return (TRUE);
} /* beale */



/* ================================== bigg ================================== */
static FITNESS_TYPE bigg (DOUBLE *x, 
                          INT n)
{
   INT     i;
   DOUBLE  z, y, tmp; 

   /* bigg */
   result.valid = 1;
   result.value = 0.0;
   for (i=1; i<=11; i++) 
   {
      z = -0.1 * i;
      y = exp(z) - 5*exp(10*z) + 3*exp(4*z);
      tmp = x[2]*exp(x[0]*z) - x[3]*exp(x[1]*z) + 3*exp(x[4]*z) - y;  
      result.value += tmp * tmp;
   }
        
   return (result);
} /* bigg */



/* ================================= hvalley ================================ */
static FITNESS_TYPE hvalley (DOUBLE *x, 
                             INT     n)
{
   DOUBLE  theta, tmp1, tmp2; 
 
   /* hvalley */
   result.valid = 1;
   result.value = 0.0;
 
   if (x[0] > 0) 
      theta = -(1/(2*M_PI)) * atan(x[1]/x[0]);
   else  
      if (x[0] < 0) 
         theta = 1/2.0 - (1/(2*M_PI) * atan(x[1]/x[0]));
      else 
         theta = 0.25;

   tmp1 = x[2] - 10*theta;
   tmp2 = sqrt(x[0]*x[0] + x[1]*x[1]) - 1;
   result.value = 100 * (tmp1*tmp1 + tmp2*tmp2) + x[2]*x[2];

   return (result);
} /* hvalley */



/* ================================= griewank =============================== */
static FITNESS_TYPE griewank (DOUBLE *x, 
                              INT     n)
{
   INT     i;
   DOUBLE  Val1, Val2; 
 
   /* griewank */
   result.valid = 1;
 
   for (Val1 = 0.0, Val2 = 1.0, i = 0;  i < n;  i++) {
      Val1 += x[i] * x[i];
      Val2 *= cos(x[i] / sqrt((double) (i + 1)) );
   }
   result.value = (Val1 / 4000.0) - Val2 + 1.0;
   return (result);
} /* griewank */



/* ============================= quick_and_dirty ============================ */
static FITNESS_TYPE quick_and_dirty (DOUBLE *x, 
                                     INT     n)
/*----------------------------------------------------------------------------*/
/*   Spielwiese zum schnellen Austesten von Funktionen. Z.Zt.:                */
/*   2 Testfunktionen fuer die Dis von J. Matthes.                            */
/*----------------------------------------------------------------------------*/
{
   /* Error-Fkt: 
   float C1_soll=3.5777;
   float C2_soll=2.2188;
   float C3_soll=8.0000;
   float C4_soll=3.5777;
   
   float x1=0;
   float y1=0;
   float x2=20;
   float y2=0;
   float x3=10;
   float y3=10;
   float x4=10;
   float y4=20;
   */

   /* Error2-Fkt: */
   float C1_soll=25.2982;
   float C2_soll=25.2982;
   float C3_soll=56.5685;
   float C4_soll=56.5685;

   float x1=-1;
   float y1=0;
   float x2=0;
   float y2=-1;
   float x3=1;
   float y3=0;
   float x4=0;
   float y4=1;


   float C1_ist;
   float C2_ist;
   float C3_ist;
   float C4_ist;

   float x0, y0, alpha;

   x0    = x[0];
   y0    = x[1];
   alpha = x[2];
   
   C1_ist = alpha*1/sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0));
   C2_ist = alpha*1/sqrt((x2-x0)*(x2-x0)+(y2-y0)*(y2-y0));
   C3_ist = alpha*1/sqrt((x3-x0)*(x3-x0)+(y3-y0)*(y3-y0));
   C4_ist = alpha*1/sqrt((x4-x0)*(x4-x0)+(y4-y0)*(y4-y0));

   result.value = (C1_ist-C1_soll)*(C1_ist-C1_soll)+
                  (C2_ist-C2_soll)*(C2_ist-C2_soll)+
                  (C3_ist-C3_soll)*(C3_ist-C3_soll)+
                  (C4_ist-C4_soll)*(C4_ist-C4_soll);
   result.valid = 1;
         
   return (result);
} /* quick_and_dirty */



/* =============================== init_mbf_sim ============================= */
void init_mbf_sim (void)
{
   DOUBLE  alpha;

   /* init_mbf_sim */
   mit_drehung = FALSE;
   sin_a       = 0.0;
   cos_a       = 0.0;
   if (getDoubleVal(&alpha, MBF_ROT_ANGLE_IDX, GLEAM_OK))
      if (alpha > 0.0)
      { /* there is a rotation angle given */
         alpha = (alpha / 180.0) * M_PI;
         sin_a = sin (alpha);
         cos_a = cos (alpha);
         mit_drehung = TRUE;
      } /* there is a rotation angle given */
/*   printf (" *** mit_drehung=%d winkel=%g  sin_a=%g cos_a=%g\n", mit_drehung, alpha, sin_a, cos_a); */
} /* init_mbf_sim */
