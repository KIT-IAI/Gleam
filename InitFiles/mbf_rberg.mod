************************ Handlungsmodell fuer GLEAM/AE *************************
GLEAM/AE  MathFkt  MBF-Rberg  withLS             05.08.1998  revised: 21.10.2020
1  1  1           gen_len_mode (1,2)  gen_akt_mode (1,2,3)  gen_segm_vert (1,2)
30 30    2   4    min_ketten_len  max_ketten_len  min_abschn_len  abschn_delta
30                Anzahl der Aktionen
 1   0.0001     akt_roh_erg_werte des (Ext)Simu   small_change_frac (optional)
 0                Anzahl der Zusatz-Files
BEGIN_LS
 2  0             number of active LHCs   number of active heuristics
0  1              List of indices of active LHCs
5000  0.000001    LSV0 (Rosen): iteration limit  abort limit 
500               LSV1 (Compl): iteration limit
10  100 200 350 500 750       1000 1250 1500 1750 2000         LSV0 (Rosen) par0
 9  .1 .01 .001 .0001 .00001  1.0e-06 1.0e-07 1.0e-08 1.0e-09  LSV0 (Rosen) par1
    0  0                  LSV0 (Rosen): startLevel of all parameters
    0.5 0.3  0.334 0.333  LSV0 (Rosen): initP1stLevel initP2ndLevel  of all parameters
    0.5  0 0.334 0.333    LSV0: lsvStartP, allP: startLevel initP1stLevel initP2ndLevel
10   50 100 150 200 300        400  500  650  800 1000         LSV1 (Compl) par0
    0                     LSV1 (Compl): startLevel of all parameters
    0.5 0.3               LSV1 (Compl): initP1stLevel initP2ndLevel  of all parameters
    0.5  0 0.334 0.333    LSV1: lsvStartP, allP: startLevel initP1stLevel initP2ndLevel
 6  0.0 0.2 0.4 0.6 0.8 1.0  allP of all LHCs
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
          -5.0     +35.0       p01
                                ! ! !
  1  0  1                   par02
          -5.0     +35.0       p02
                                ! ! !
  1  0  1                   par03
          -5.0     +35.0       p03
                                ! ! !
  1  0  1                   par04
          -5.0     +35.0       p04
                                ! ! !
  1  0  1                   par05
          -5.0     +35.0       p05
                                ! ! !
  1  0  1                   par06
          -5.0     +35.0       p06
                                ! ! !
  1  0  1                   par07
          -5.0     +35.0       p07
                                ! ! !
  1  0  1                   par08
          -5.0     +35.0       p08
                                ! ! !
  1  0  1                   par09
          -5.0     +35.0       p09
                                ! ! !
  1  0  1                   par10
          -5.0     +35.0       p10
                                ! ! !
  1  0  1                   par11
          -5.0     +35.0       p11
                                ! ! !
  1  0  1                   par12
          -5.0     +35.0       p12
                                ! ! !
  1  0  1                   par13
          -5.0     +35.0       p13
                                ! ! !
  1  0  1                   par14
          -5.0     +35.0       p14
                                ! ! !
  1  0  1                   par15
          -5.0     +35.0       p15
                                ! ! !
  1  0  1                   par16
          -5.0     +35.0       p16
                                ! ! !
  1  0  1                   par17
          -5.0     +35.0       p17
                                ! ! !
  1  0  1                   par18
          -5.0     +35.0       p18
                                ! ! !
  1  0  1                   par19
          -5.0     +35.0       p19
                                ! ! !
  1  0  1                   par20
          -5.0     +35.0       p20
                                ! ! !
  1  0  1                   par21
          -5.0     +35.0       p21
                                ! ! !
  1  0  1                   par22
          -5.0     +35.0       p22
                                ! ! !
  1  0  1                   par23
          -5.0     +35.0       p23
                                ! ! !
  1  0  1                   par24
          -5.0     +35.0       p24
                                ! ! !
  1  0  1                   par25
          -5.0     +35.0       p25
                                ! ! !
  1  0  1                   par26
          -5.0     +35.0       p26
                                ! ! !
  1  0  1                   par27
          -5.0     +35.0       p27
                                ! ! !
  1  0  1                   par28
          -5.0     +35.0       p28
                                ! ! !
  1  0  1                   par29
          -5.0     +35.0       p29
                                ! ! !
  1  0  1                   par30
          -5.0     +35.0       p30
                                ! ! !

