#-----------------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Include file for package "chio"   for the mpi version
#
# This file provides the include for the mpi-based parallel version.
# It is based on "chio_common_inc.mak".
#

# There is also "chio_inc.mak" for the standard include.
#
#-----------------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-----------------------------------------------------+----------------------------------+
#   Autor: W.Jakob                                    |   Stand:  14.02.2019             |
#-----------------------------------------------------+----------------------------------+


#----------------------------- common package include-file: ------------------------------
include $(CHIO)/chio_common_inc.mak


#-----------------------------------------------------------------------------------------
#      specific definitions of objects:
#-----------------------------------------------------------------------------------------
MPICHIO_OBJ = $(CHIO_OBJ)         $(CHIO)/mpi_chio.o

#-----------------------------------------------------------------------------------------
#      specific dependencies:
#-----------------------------------------------------------------------------------------
$(CHIO)/chain_io.o : $(CHIO_ALL)  $(GLOB_TXT)  $(LGSW_INC)     $(BEW)/bew.h   $(MPAR)/mPar.h \
                     $(CHIO)/chioInt.h   
$(CHIO)/mpi_chio.o : $(CHIO_ALL)  $(CHAINDEF)  $(MPAR)/mPar.h  $(EVO)/evo.h      
