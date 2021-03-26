#-----------------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Common include file for package: aufg
# This file is used by 
#   - aufg_inc.mak      for the standard versions without parallelism
#   - aufg_ess_inc.mak  for the include of the version based on external simulation 
#                       services (ESS) for parallel simulation requiring more dependencies.
#
# There are two object groups for the 
#   - version with textual user interface (TUI) and the
#   - command line version (CLV).
#
#-----------------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-----------------------------------------------------+----------------------------------+
#   Autor: W.Jakob                                    |   Stand:  09.01.2018             |
#-----------------------------------------------------+----------------------------------+


#-----------------------------------------------------------------------------------------
#      common definition of objects:
#-----------------------------------------------------------------------------------------
AUFG_TUI = $(AUFG)/tsk_data.o  $(AUFG)/aufgcmen.o 
AUFG_CLV = $(AUFG)/tsk_data.o  


#-----------------------------------------------------------------------------------------
#      common definitions:
#-----------------------------------------------------------------------------------------
AUFG_TXT = $(AUFG)/aufg_d.h        $(AUFG)/aufg_gb.h  
AUFG_ALL = $(GLOB_TXT) $(AUFG_TXT) $(SYS)/sys.h       $(CTIO)/ctio.h     $(FBHM_INC)        \
           $(CHAINDEF)             $(HMOD)/hmod.h     $(SIMU)/simu.h     $(EVO)/evo.h       \
           $(AUFG)/aufg.h          $(AUFG)/aufgInt.h       

#-----------------------------------------------------------------------------------------
#      common dependencies:
#-----------------------------------------------------------------------------------------
$(AUFG)/aufgcmen.o: $(AUFG_ALL)         
