#-----------------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Include-File fuer Package: simu    for standard versions
#
# This file provides the include for the standard versions without parallelism of simulations 
# and either 
#   - a pipe-based interface to external simulators, or
#   - a function-call-based interface to MatPower/Matlab.
# It is based on "simu_common_inc.mak".
#
# There are also two other alternative include files: 
#   simu_ess_inc.mak  for the include of the version based on external simulation 
#                     services (ESS) for parallel simulation requiring more dependencies
#   simu_cec_inc.mak  for the standard version enhanced by the CEC'05 benchmarks
#
# There are two object groups defined in men_common_inc.mak for the versions with
#   - textual user interface (TUI) and the
#   - command line version (CLV).
#
#-----------------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of this file.
#-----------------------------------------------------+----------------------------------+
#   Autor: W.Jakob                                    |   Stand:  12.08.2018             |
#-----------------------------------------------------+----------------------------------+

#----------------------------- common package include-file: ------------------------------
include $(SIMU)/simu_common_inc.mak


#-----------------------------------------------------------------------------------------
#      specific definition of objects:
#-----------------------------------------------------------------------------------------
EXT_PIPE_SIMU = $(SIMU)/eldokopl.o  $(SIMU)/mathkopl.o  $(SIMU)/gen_kopl.o  $(SIMU)/ext_sim.o   
EXT_MATL_SIMU = $(SIMU)/matlab_koppl.o

SIMU_TUI      = $(SIMU_ALLG)  $(EXT_PIPE_SIMU)  $(SIMU)/simucmen.o 
SIMU_CLV      = $(SIMU_ALLG)  $(EXT_PIPE_SIMU)  
SIMU_ML_TUI   = $(SIMU_ALLG)  $(EXT_MATL_SIMU)  $(SIMU)/simucmen.o 
SIMU_ML_CLV   = $(SIMU_ALLG)  $(EXT_MATL_SIMU)  


#-----------------------------------------------------------------------------------------
#      specific dependencies:
#-----------------------------------------------------------------------------------------
$(SIMU)/cec_sim.o :     $(SYS)/sys.h       $(FBHM_INC)        $(CHAINDEF)        $(AUFG)/aufg.h     \
                        $(SIMU)/simu.h     $(SIMU)/simuInt.h  
$(SIMU)/simu_gsw.o:     $(SIMU_ALL)        $(SIMU_TXT)        $(GLOB_TXT)        $(LGSW_INC)        \
                        $(BEW)/bew.h       $(APPL)/appl.h     
$(SIMU)/simucmen.o:     $(SIMU_ALL)        $(SIMU_TXT)        $(GLOB_TXT)        $(LGSW_INC)        \
                        $(BEW)/bew.h       $(MEN)/men.h       

$(SIMU)/matlab_koppl.o: $(SIMU_ALL)        $(SIMU_TXT)        $(GLOB_TXT)        $(LGSW_INC)        \
                        $(AUFG)/aufg.h     


