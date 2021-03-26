*************************** Gene model for GLEAM/AE ****************************
GLEAM/AE  MathFkt  MBF-Foxholes  withLS                               09.04.2018
1  1  1           gen_len_mode (1,2)  gen_akt_mode (1,2,3)  gen_segm_vert (1,2)
2  2    1   0     min_ketten_len  max_ketten_len  min_abschn_len  abschn_delta
 2                number of gene types
 1                "akt_roh_erg_werte" of the (Ext)Simu
 0                number of additional files
BEGIN_LS
 2  0             number of active LHCs   number of active heuristics
0  1              List of indices of active LHCs
3000  0.02        LSV0 (Rosen): iteration limit  abort limit 
300               LSV1 (Compl): iteration limit
10  100 200 350 500 750       1000 1250 1500 1750 2000         LSV0 (Rosen) par0
 9  .1 .01 .001 .0001 .00001  1.0e-06 1.0e-07 1.0e-08 1.0e-09  LSV0 (Rosen) par1
    0  0                  LSV0 (Rosen): startLevel of all parameters
    0.5 0.3  0.334 0.333  LSV0 (Rosen): initP1stLevel initP2ndLevel  of all parameters
    0.5  0 0.334 0.333    LSV0: lsvStartP, allP: startLevel initP1stLevel initP2ndLevel
10   50 100 150 200 300        400  500  650  800 1000         LSV1 (Compl) par0
    0                     LSV1 (Compl): startLevel of all parameters
    0.5 0.3               LSV1 (Compl): initP1stLevel initP2ndLevel  of all parameters
    0.5  0 0.334 0.333    LSV1: lsvStartP, allP: startLevel initP1stLevel initP2ndLevel
 6  0.0 0.2 0.4 0.6 0.8 1.0  allP of all parameters
END_LS
********************************************************************************
 1) 2) 3)    4)     5)    1: priority of a gene 2: number of integer parameters
                          3: number of real param. 4: lower parameter limit  
                          5: upper parameter limit maxNameLength: gene=20 param=10
----------------------------------------------------------------------------------
                          gene/parameter name   constant name    constant value  
********************************************************************************
  0  0  0                   sub chain
  1  0  1                   par1
             -500.0  +500.0     p1
                                ! ! !
  1  0  1                   par2
             -500.0  +500.0     p2
                                ! ! !
