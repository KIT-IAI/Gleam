#-----------------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Include file for package "evo"   for the mpi version
#
# This file provides the include for the mpi-based parallel version.
# It is based on "evo_common_inc.mak".
#
# There are also two other alternative include files: 
#   evo_inc.mak     for the standard versions without parallelism
#   evo_ess_inc.mak for the version based on external simulation services (ESS) 
#                   for parallel simulation
#
# There are two object groups for the versions with
#   - textual user interface (TUI), with
#   - command line version (CLV).
#
#-----------------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-----------------------------------------------------+----------------------------------+
#   Autor: W.Jakob                                    |   Stand: 10.01.2018              |
#-----------------------------------------------------+----------------------------------+


#----------------------------- common package include-file: ------------------------------
include $(EVO)/evo_common_inc.mak


#-----------------------------------------------------------------------------------------
#      specific dependencies:
#-----------------------------------------------------------------------------------------
$(EVO)/evo.o     : $(EVO_ALL)         $(EVO_TXT)         $(HMOD)/hmod.h     $(SIMU)/simu.h     \
                   $(CHIO)/chio.h     $(MPAR)/mPar.h     
$(EVO)/evo_gsw.o : $(EVO_ALL)         $(EVO_TXT)         $(GLOB_TXT)        $(CHIO)/chio.h     \
                   $(HMOD)/hmod.h     $(APPL)/appl.h     $(SIMU)/simu.h     $(MPAR)mPar.h      
$(EVO)/evo_steu.o: $(EVO_ALL)         $(EVO_TXT)         $(BEW)/bew.h       $(SIMU)/simu.h     

