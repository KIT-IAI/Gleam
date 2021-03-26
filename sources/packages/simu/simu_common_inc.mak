#-----------------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Common include file for package: simu
# This file is used by 
#   - simu_inc.mak      for the standard versions without ESS based parallelism of simulations, 
#                       but with pipe-based interface to external simulators or an interface to 
#                       MatPower/Matlab
#   - simu_ess_inc.mak  for the include of the version based on external simulation 
#                       services (ESS) for parallel simulation requiring more dependencies
#   - simu_cec_inc.mak  for the standard version enhanced by the CEC'05 benchmarks
#
# There are two object groups each for the 
#   - version with textual user interface (TUI) and the
#   - command line version (CLV).
#
#-----------------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-----------------------------------------------------+----------------------------------+
#   Autor: W.Jakob                                    |   Stand:  08.07.2019             |
#-----------------------------------------------------+----------------------------------+


#-----------------------------------------------------------------------------------------
#      common definition of objects:
#-----------------------------------------------------------------------------------------
SIMU_ALLG = $(SIMU)/mbf_sim.o   $(SIMU)/simu_gsw.o  $(SIMU)/simu.o      $(SIMU)/cec_sim.o 


#-----------------------------------------------------------------------------------------
#      common definitions:
#-----------------------------------------------------------------------------------------
SIMU_TXT = $(SIMU)/simu_d.h  $(SIMU)/simu_gb.h  
SIMU_ALL = $(SYS)/sys.h      $(CTIO)/ctio.h     $(FBHM_INC)       $(HMOD)/hmod.h \
           $(SIMU)/simu.h    $(SIMU)/simuInt.h  


#-----------------------------------------------------------------------------------------
#      common dependencies:
#-----------------------------------------------------------------------------------------
$(SIMU)/mbf_sim.o : $(SIMU_ALL)      $(SIMU_TXT)      $(CHAINDEF)      $(AUFG)/aufg.h   
$(SIMU)/simu.o    : $(SIMU_ALL)      $(SIMU_TXT)      $(GLOB_TXT)      $(LGSW_INC)      \
                    $(BEW)/bew.h     $(APPL)/appl.h   $(AUFG)/aufg.h   

$(SIMU)/ext_sim.o:  $(SIMU_ALL)      $(SIMU_TXT)      $(CHAINDEF)      $(AUFG)/aufg.h   
$(SIMU)/eldokopl.o: $(SIMU_ALL)      $(SIMU_TXT)      $(GLOB_TXT)      $(CHAINDEF)      \
                    $(AUFG)/aufg.h   
$(SIMU)/mathkopl.o: $(SIMU_ALL)      $(SIMU_TXT)      $(GLOB_TXT)      $(LGSW_INC)      \
                    $(AUFG)/aufg.h   
$(SIMU)/gen_kopl.o: $(SIMU_ALL)      $(SIMU_TXT)      $(GLOB_TXT)      $(LGSW_INC)      \
                    $(AUFG)/aufg.h   
