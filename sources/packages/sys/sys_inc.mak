#-------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Include-File fuer Package: sys
#
#-------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-------------------------------------------+----------------------------------+
#   Autor: W.Jakob                          |   Stand:  01.12.2017             |
#-------------------------------------------+----------------------------------+

#-------------------------------------------------------------------------------
#      Definitionen der Objects:
#-------------------------------------------------------------------------------
SYS_OBJ = $(SYS)/sys_gsw.o  $(SYS)/file_gsw.o $(SYS)/term_io.o  $(SYS)/extkoppl.o  


#-------------------------------------------------------------------------------
#      Beschreibung der Abhaengigkeiten:
#-------------------------------------------------------------------------------
SYS_TXT  = $(SYS)/sys_d.h     $(SYS)/sys_gb.h     
#-------------------------------------------------------------------------------
$(SYS_OBJ)        : $(SYS)/sys.h   $(SYS)/sysInt.h
$(SYS)/file_gsw.o : $(SYS_TXT)     $(FBHM_INC)         
$(SYS)/extkoppl.o : $(SYS_TXT)     $(FBHM_INC)
