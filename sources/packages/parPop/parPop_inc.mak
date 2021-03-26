#-----------------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Include file for package: parPop
#
#-----------------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-----------------------------------------------------+----------------------------------+
#   Author: W.Jakob / P.Ostheimer                     |   Date:  06/20/2018              |
#-----------------------------------------------------+----------------------------------+

#-----------------------------------------------------------------------------------------
#      definition of objects:
#-----------------------------------------------------------------------------------------
PAR_POP_OBJ = $(PAR_POP)/parPop.o 

#-----------------------------------------------------------------------------------------
#      description of the dependencies:
#-----------------------------------------------------------------------------------------
PAR_POP_TXT = $(PAR_POP)/parPop_d.h  $(PAR_POP)/parPop_gb.h    
PAR_POP_ALL = $(PAR_POP)/parPop.h    $(PAR_POP)/parPopInt.h $(PAR_POP_TXT)         \
              $(GLOB_TXT)            $(SYS)/sys.h           $(CTIO)/ctio.h         \
              $(FBHM_INC)            $(CHAINDEF)            $(EVO)/evo.h
#-----------------------------------------------------------------------------------------
$(PAR_POP)/parPop.o : $(PAR_POP_ALL)   $(AUFG)/aufg.h   $(CHIO)/chio.h


