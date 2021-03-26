*************************** Genmodell fuer GLEAM/AE ****************************
GLEAM/AE  MathFkt  MBF-Griewank   withLS                              09.04.2018
1  1  1           gen_len_mode (1,2)  gen_akt_mode (1,2,3)  gen_segm_vert (1,2)
 5   5    1   1   min_ketten_len  max_ketten_len  min_abschn_len  abschn_delta
 5                Anzahl der Gentypen
 1   0.001        akt_roh_erg_werte des (Ext)Simu   small_change_frac (optional)
 0                Anzahl der Zusatz-Files
BEGIN_LS
 2  0             Anzahl aktiver LSVs   Anzahl aktiver Heuristiken
0  1              Liste der aktiven LSVs
5001  0.02        LSV0 (Rosen): Iterationslimit  Abbruchschranke 
503               LSV1 (Compl): Iterationslimit
10  100 200 350 500 750       1000 1250 1500 1750 2000         LSV0 (Rosen) par0
 9  .1 .01 .001 .0001 .00001  1.0e-06 1.0e-07 1.0e-08 1.0e-09  LSV0 (Rosen) par1
    0  0                  LSV0 (Rosen): startLevel aller Param
    0.5 0.3  0.334 0.333  LSV0 (Rosen): initP1stLevel initP2ndLevel  aller Param
    0.5  0 0.334 0.333    LSV0: lsvStartP, allP: startLevel initP1stLevel initP2ndLevel
10   50 100 150 200 300        400  500  650  800 1000         LSV1 (Compl) par0
    0                     LSV1 (Compl): startLevel aller Param
    0.5 0.3               LSV1 (Compl): initP1stLevel initP2ndLevel  aller Param
    0.5  0 0.334 0.333    LSV1: lsvStartP, allP: startLevel initP1stLevel initP2ndLevel
 6  0.0 0.2 0.4 0.6 0.8 1.0  allP aller LSV
END_LS
********************************************************************************
 1) 2) 3)    4)     5)      1: Prioritaet der Aktion  2: Anzahl Integer-Parameter
                            3: Anzahl Real-Parameter  4: Untere Parametergrenze  
                            5: Obere Parametergrenze MaxBezLen=20 MaxParBezLen=10
--------------------------------------------------------------------------------
                            Bezeichner/ParBez     Konstantennamen Konstantenwert  
********************************************************************************
  0  0  0                   Unterkette
  1  0  1                   par01
           -600.0  +600.0     p01
                                ! ! !
  1  0  1                   par02
           -600.0  +600.0     p02
                                ! ! !
  1  0  1                   par03
           -600.0  +600.0     p03
                                ! ! !
  1  0  1                   par04
           -600.0  +600.0     p04
                                ! ! !
  1  0  1                   par05
           -600.0  +600.0     p05
                                ! ! !
