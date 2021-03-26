************************ Action Model for GLEAM/AE *******************************
GLEAM/AE   LESAK/Plus                             01.07.1998   revised: 29.06.2020
2  3  2           gen_len_mode (1,2)  gen_akt_mode (1,2,3)  gen_segm_vert (1,2)
5  50   4   6     min_ketten_len  max_ketten_len  min_abschn_len  abschn_delta
13                number of action types (gene types)
 7   0.002        akt_roh_erg_werte of (ext.)simu   small_change_frac (optional)
 2                number of additional files
0 kin Kinematics-File Kinematics  
0 obs Obstacle-File   Obstacles 
**********************************************************************************
 1) 2) 3)    4)     5)    1: priority of an action 2: number of integer parameter
                          3: number of real param. 4: lower parameter limit  
                          5: upper parameter limit maxNameLength: gene=20 param=10
----------------------------------------------------------------------------------
                          gene/parameter name   constant name    constant value  
**********************************************************************************
  0  0  0                 sub-chain             ACTIVITY_ACT        (00)
  5  0  2                 SetMotorSpeed_M1      MOTOR_1             (01)
              0.1   15.0      ramp  
                                ! ! !
           -120.0  120.0      voltVal_1 
                                V < mV
  5  0  2                 SetMotorSpeed_M2      MOTOR_2             (02)
              0.1   15.0      ramp  
                                ! ! !
           -120.0  120.0      voltVal_2 
                                V < mV
  5  0  2                 SetMotorSpeed_M3      MOTOR_3             (03)
              0.1   15.0      ramp  
                                ! ! !
           -120.0  120.0      voltVal_3 
                                V < mV
  5  0  2                 SetMotorSpeed_M4      MOTOR_4             (04)
              0.1   15.0      ramp  
                                ! ! !
           -120.0  120.0      voltVal_4 
                                V < mV
  5  0  2                 SetMotorSpeed_M5      MOTOR_5             (05)
              0.1   15.0      ramp  
                                ! ! !
           -120.0  120.0      voltVal_5 
                                V < mV
  5  0  1                 Motor1_off            MOTOR_1_AUS         (06)
              0.1   12.0      ramp  
                                ! ! !
  5  0  1                 Motor2_off            MOTOR_2_AUS         (07)
              0.1   12.0      ramp  
                                ! ! !
  5  0  1                 Motor3_off            MOTOR_3_AUS         (08)
              0.1   12.0      ramp  
                                ! ! !
  5  0  1                 Motor4_off            MOTOR_4_AUS         (09)
              0.1   12.0      ramp  
                                ! ! !
  5  0  1                 Motor5_off            MOTOR_5_AUS         (10)
              0.1   12.0      ramp  
                                ! ! !
 10  0  0                 BlockBegin            START_BLOCK_BEGIN   (11)
 10  0  0                 BlockEnd              START_BLOCK_END     (12)
 30  1  0                 Unchanged             UNVERAENDERT        (13)
              1     20        cycles 
                                ! ! !
