#-----------------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Include file for package "evo"    for ESS versions 
#
# This file provides the include for the ESS-based parallel versions (External Simulation 
# Services).
# It is based on "evo_common_inc.mak".
#
# There are also two other alternative include files: 
#   evo_inc.mak      for the standard versions without parallelism of simulations and a
#                    pipe-based interface to external simulators 
#   evo_mpi_inc.mak  for the parallel mpi version
#
# There are two object groups defined in men_common_inc.mak for the versions with
#   - textual user interface (TUI) and the
#   - command line version (CLV).
#
#-----------------------------------------------------------------------------------------
#  Impl.: EVO_LSVRC_ESS_* sind vorlaeufig!
#-----------------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-----------------------------------------------------+----------------------------------+
#   Autor: W.Jakob                                    |   Stand: 22.08.2018              |
#-----------------------------------------------------+----------------------------------+


#----------------------------- common package include-file: ------------------------------
include $(EVO)/evo_common_inc.mak


#-----------------------------------------------------------------------------------------
#      specific definition of objects:
#-----------------------------------------------------------------------------------------
EVO_ESS_TUI = $(EVO_BASE)  $(EVO)/parSimEvo.o  $(EVO)/evo_anzg.o  $(EVO)/evo_cmen.o  
EVO_ESS_CLV = $(EVO_BASE)  $(EVO)/parSimEvo.o  $(EVO)/evo_anzg.o  

EVO_LSVRC_ESS_TUI = $(EVO_ESS_TUI)  $(LSVRC_BASE)      
EVO_LSVRC_ESS_CLV = $(EVO_ESS_CLV)  $(LSVRC_BASE)      


#-----------------------------------------------------------------------------------------
#      specific dependencies:
#-----------------------------------------------------------------------------------------
$(EVO)/parSimEvo.o: $(EVO_ALL)        $(EVO_TXT)        $(WEBIO_TXT)      $(SIMU_TXT)       \
                    $(HMOD)/hmod.h    $(BEW)/bew.h      $(WEBIO)/webIO.h  $(SIMU)/simu.h    
$(EVO)/evo_gsw.o  : $(EVO_ALL)        $(EVO_TXT)        $(GLOB_TXT)       $(CHIO)/chio.h    \
                    $(HMOD)/hmod.h    $(AUFG)/aufg.h    $(APPL)/appl.h    $(SIMU)/simu.h    
$(EVO)/evo_steu.o : $(EVO_ALL)        $(EVO_TXT)        $(BEW)/bew.h      $(SIMU)/simu.h    \
                    $(HMOD)/hmod.h    $(WEBIO)/webIO.h  
