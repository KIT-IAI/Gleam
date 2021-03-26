#-----------------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Include-File fuer Package: evo     for standard versions
#
# This file provides the standard include for versions without parallelism.
# It is based on "evo_common_inc.mak".
#
# There are also two other alternative include files: 
#   evo_ess_inc.mak for the version based on external simulation services (ESS) 
#                   for parallel simulation
#   evo_mpi_inc.mak for the parallel mpi version
#
# There are two object groups for the versions with
#   - textual user interface (TUI), with
#   - command line version (CLV).
#
#-----------------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-----------------------------------------------------------------------------------------
# Impl.: Auskommentierte Object-Gruppen koennen geloescht werden, wenn klar ist wofuer sie 
#        gebraucht wurden.
#-----------------------------------------------------+----------------------------------+
#   Autor: W.Jakob                                    |   Stand:  17.07.2018             |
#-----------------------------------------------------+----------------------------------+


#------------------------ common package include-file: -------------------------
include $(EVO)/evo_common_inc.mak


#-----------------------------------------------------------------------------------------
#      specific definition of objects:
#-----------------------------------------------------------------------------------------
EVO_TUI       = $(EVO_BASE)        $(EVO)/evo.o       $(EVO)/evo_anzg.o  $(EVO)/evo_cmen.o  
EVO_CLV       = $(EVO_BASE)        $(EVO)/evo.o       $(EVO)/evo_anzg.o  

EVO_LSVRC_TUI = $(EVO_TUI)   $(LSVRC_BASE)       
EVO_LSVRC_CLV = $(EVO_CLV)   $(LSVRC_BASE)      


#-----------------------------------------------------------------------------------------
#      specific dependencies:
#-----------------------------------------------------------------------------------------
$(EVO)/evo.o     : $(EVO_ALL)         $(EVO_TXT)         $(HMOD)/hmod.h     $(AUFG)/aufg.h     \
                   $(SIMU)/simu.h     
$(EVO)/evo_gsw.o : $(EVO_ALL)         $(EVO_TXT)         $(GLOB_TXT)        $(CHIO)/chio.h     \
                   $(HMOD)/hmod.h     $(AUFG)/aufg.h     $(APPL)/appl.h     $(SIMU)/simu.h     
$(EVO)/evo_steu.o: $(EVO_ALL)         $(EVO_TXT)         $(AUFG)/aufg.h     $(BEW)/bew.h       \
                   $(SIMU)/simu.h     
