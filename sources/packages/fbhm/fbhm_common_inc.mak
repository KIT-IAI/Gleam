#-------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Common include-file for package: fbhm 
# This file is used by 
#   - fbhm_inc.mak      for the standard include
#   - fbhm_mpi_inc.mak  for the include of the parallel mpi version requiring 
#                       more dependencies.
#
#-------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-------------------------------------------+----------------------------------+
#   Autor: W.Jakob                          |   Stand:  01.12.2017             |
#-------------------------------------------+----------------------------------+

#-------------------------------------------------------------------------------
#      common definition of objects:
#-------------------------------------------------------------------------------
FBHM_OBJ = $(FBHM)/fbhm.o 

#-------------------------------------------------------------------------------
#      common dependencies:
#-------------------------------------------------------------------------------
FBHM_TXT = $(FBHM)/fbhm_d.h $(FBHM)/fbhm_gb.h   
#-------------------------------------------------------------------------------
$(FBHM)/fbhm.o: $(FBHM_INC) $(FBHM_TXT) $(GLOB_TXT) $(SYS)/sys.h $(CTIO)/ctio.h  
