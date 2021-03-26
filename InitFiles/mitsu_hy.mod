************************ Handlungsmodell für GLEAM/AE **************************
GLEAM/AE   LESAK/Plus                         01.07.1998     revised: 23.03.2016
2  3  2           gen_len_mode (1,2)  gen_akt_mode (1,2,3)  gen_segm_vert (1,2)
5  50   4   6     min_ketten_len  max_ketten_len  min_abschn_len  abschn_delta
13                Anzahl der Aktionstypen
 7   0.002        akt_roh_erg_werte des (Ext)Simu   small_change_frac (optional)
 2                Anzahl der Zusatz-Files
0 kin Kinematik-Datei Kinematik  
0 obs Hindernis-Datei Hindernis
********************************************************************************
 1) 2) 3)    4)     5)      1: Prioritaet der Aktion  2: Anzahl Integer-Parameter
                            3: Anzahl Real-Parameter  4: Untere Parametergrenze  
                            5: Obere Parametergrenze MaxBezLen=20 MaxParBezLen=10
-----------------------------------------------------------------------------
                          Bezeichner/ParBez     Konstantennamen Konstantenwert  
******************************************************************************
  0  0  0                 Unterkette            ACTIVITY_ACT        (00)
  5  0  2                 Setze_Motor1_U        MOTOR_1             (01)
              0.1   15.0      Rampe 
                                ! ! !
           -120.0  120.0      U_Wert_1 
                                V < mV
  5  0  2                 Setze_Motor2_U        MOTOR_2             (02)
              0.1   15.0      Rampe 
                                ! ! !
           -120.0  120.0      U_Wert_2 
                                V < mV
  5  0  2                 Setze_Motor3_U        MOTOR_3             (03)
              0.1   15.0      Rampe 
                                ! ! !
           -120.0  120.0      U_Wert_3 
                                V < mV
  5  0  2                 Setze_Motor4_U        MOTOR_4             (04)
              0.1   15.0      Rampe 
                                ! ! !
           -120.0  120.0      U_Wert_4 
                                V < mV
  5  0  2                 Setze_Motor5_U        MOTOR_5             (05)
              0.1   15.0      Rampe 
                                ! ! !
           -120.0  120.0      U_Wert_5 
                                V < mV
  5  0  1                 Motor1_aus            MOTOR_1_AUS         (06)
              0.1   12.0      Rampe 
                                ! ! !
  5  0  1                 Motor2_aus            MOTOR_2_AUS         (07)
              0.1   12.0      Rampe 
                                ! ! !
  5  0  1                 Motor3_aus            MOTOR_3_AUS         (08)
              0.1   12.0      Rampe 
                                ! ! !
  5  0  1                 Motor4_aus            MOTOR_4_AUS         (09)
              0.1   12.0      Rampe 
                                ! ! !
  5  0  1                 Motor5_aus            MOTOR_5_AUS         (10)
              0.1   12.0      Rampe 
                                ! ! !
 10  0  0                 Block_Beginn          START_BLOCK_BEGIN   (11)
 10  0  0                 Block_End             START_BLOCK_END     (12)
 30  1  0                 Unveraendert          UNVERAENDERT        (13)
              1     20        Takt_Anz 
                                ! ! !
