#-------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Include file for package: chio
# This file uses chio_common_inc.mak and both provide the default include. 
# There is also "chio_mpi_inc.mak" for the mpi-based parallel version.
#
#-------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-------------------------------------------+----------------------------------+
#   Autor: W.Jakob                          |   Stand:  14.02.2019             |
#-------------------------------------------+----------------------------------+


#------------------------ common package include-file: -------------------------
include $(CHIO)/chio_common_inc.mak


#-------------------------------------------------------------------------------
#      specific dependencies:
#-------------------------------------------------------------------------------
$(CHIO)/chain_io.o : $(CHIO_ALL)  $(GLOB_TXT)  $(LGSW_INC)  $(BEW)/bew.h       
