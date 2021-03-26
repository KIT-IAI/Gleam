#-----------------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Common include file for package: evo
# This file is used by 
#   - evo_inc.mak      for the standard versions with or without LHCs
#   - evo_ess_inc.mak  for the include of the versions based on external simulation 
#                      services (ESS) for parallel simulation requiring more dependencies
#   - evo_mpi_inc.mak  for the include of the parallel mpi version requiring some more
#                      more dependencies.
#
# There are two object groups each for the 
#   - version with textual user interface (TUI) and the
#   - command line version (CLV).
#
#-----------------------------------------------------------------------------------------
#  LSV_BASE     is intended for later integration of common adaptiveness of general LHCs.
#  LSVRC_BASE   combines LSV_BASE with an interface to Rosenbrock- and Complex-LHCs.
#-----------------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-----------------------------------------------------+----------------------------------+
#   Autor: W.Jakob                                    |   Stand:  29.08.2020             |
#-----------------------------------------------------+----------------------------------+


#-----------------------------------------------------------------------------------------
#      common definition of objects:
#-----------------------------------------------------------------------------------------
EVO_BASE     = $(EVO)/go_gsw.o    $(EVO)/go_rxo.o    $(EVO)/go_pmut.o   $(EVO)/go_amut.o   \
               $(EVO)/go_smut.o   $(EVO)/evo_gsw.o   $(EVO)/evo_steu.o  

LSV_BASE     = $(EVO)/lsv_steu.o  $(EVO)/adapt_di.o  
LSVRC_BASE   = $(LSV_BASE)        $(EVO)/rc_koppl.o  


#-----------------------------------------------------------------------------------------
#      common definitions:
#-----------------------------------------------------------------------------------------
EVO_TXT     = $(EVO)/evo_d.h   $(EVO)/evo_gb.h    
EVO_MIN_ALL = $(EVO)/evo.h     $(EVO)/evoInt.h  $(SYS)/sys.h   $(FBHM_INC)   $(LGSW_INC)      
EVO_ALL     = $(EVO_MIN_ALL)   $(CTIO)/ctio.h   


#-----------------------------------------------------------------------------------------
#      common dependencies:
#-----------------------------------------------------------------------------------------
$(EVO)/go_gsw.o  : $(EVO_ALL)         
$(EVO)/go_rxo.o  : $(EVO_ALL)         $(HMOD)/hmod.h     
$(EVO)/go_pmut.o : $(EVO_ALL)         $(HMOD)/hmod.h     
$(EVO)/go_amut.o : $(EVO_ALL)         $(HMOD)/hmod.h     
$(EVO)/go_smut.o : $(EVO_ALL)         $(HMOD)/hmod.h     
$(EVO)/evo_anzg.o: $(EVO_ALL)         $(EVO_TXT)         $(GLOB_TXT)        $(AUFG)/aufg.h     \
                   $(APPL)/appl.h     $(SIMU)/simu.h     
$(EVO)/evo_cmen.o: $(EVO_ALL)         $(EVO_TXT)         $(GLOB_TXT)        $(BEW)/bew.h       \
                   $(HMOD)/hmod.h     $(APPL)/appl.h     $(SIMU)/simu.h     

$(EVO)/lsv_steu.o: $(EVO_MIN_ALL)     $(EVO_TXT)         $(LSKP)/joints.h   $(AUFG)/aufg.h 
$(EVO)/rc_koppl.o: $(EVO_ALL)         $(EVO_TXT)         $(BEW)/bew.h       $(HMOD)/hmod.h     \
                   $(APPL)/appl.h     $(SIMU)/simu.h     $(ROCO)/complbox.h $(ROCO)/rosencon.h
$(EVO)/adapt_di.o: $(EVO_MIN_ALL)     $(EVO_TXT)         $(SIMU_TXT)        $(AUFG)/aufg.h     



