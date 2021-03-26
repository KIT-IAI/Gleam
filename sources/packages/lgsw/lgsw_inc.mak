#-------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Include-File fuer Package: lgsw
#
#-------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-------------------------------------------+----------------------------------+
#   Autor: W.Jakob                          |   Stand:  06.07.2019             |
#-------------------------------------------+----------------------------------+

#-------------------------------------------------------------------------------
#      Definitionen der Objects:
#-------------------------------------------------------------------------------
LGSW_OBJ = $(LGSW)/mmsw.o  $(LGSW)/ch_init.o  $(LGSW)/balisto.o  $(LGSW)/bacha.o   


#-------------------------------------------------------------------------------
#      Beschreibung der Abhaengigkeiten:
#-------------------------------------------------------------------------------
LGSW_TXT = $(LGSW)/lgsw_d.h  $(LGSW)/lgsw_gb.h   
LGSW_INC = $(LGSW)/lgsw.h    $(CHAINDEF)
#-------------------------------------------------------------------------------
$(LGSW_OBJ)       : $(LGSW_INC)  $(FBHM_INC)     
$(LGSW)/mmsw.o    : $(LGSW_TXT)  $(GLOB_TXT)  $(LGSW)/lgswInt.h $(SYS)/sys.h   
$(LGSW)/ch_init.o :                           $(LGSW)/lgswInt.h 
$(LGSW)/balisto.o : $(LGSW_TXT)  $(GLOB_TXT)  $(SYS)/sys.h 
$(LGSW)/bacha.o   : $(SYS)/sys.h              $(LGSW)/lgswInt.h      
