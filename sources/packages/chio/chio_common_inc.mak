#-------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Common include file for package: chio
# This file is used by 
#   - chio_inc.mak      for the standard include
#   - chio_mpi_inc.mak  for the include of the parallel mpi version requiring 
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
CHIO_OBJ  = $(CHIO)/chain_io.o

#-------------------------------------------------------------------------------
#      common definitions:
#-------------------------------------------------------------------------------
CHIO_TXT = $(CHIO)/chio_d.h  $(CHIO)/chio_gb.h  
CHIO_ALL = $(CHIO_TXT)       $(CHIO)/chio.h    $(SYS)/sys.h      $(CTIO)/ctio.h \
           $(FBHM_INC)       $(HMOD)/hmod.h     
#-------------------------------------------------------------------------------
