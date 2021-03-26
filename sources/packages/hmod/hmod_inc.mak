#-------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Include file for package: hmod
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
HMOD_OBJ = $(HMOD)/mod_data.o  $(HMOD)/segm_gen.o  $(HMOD)/hamming.o    


#-------------------------------------------------------------------------------
#      Beschreibung der Abhaengigkeiten:
#-------------------------------------------------------------------------------
HMOD_TXT = $(HMOD)/hmod_d.h   $(HMOD)/hmod_gb.h  
HMOD_ALL = $(HMOD)/hmod.h     $(HMOD)/hmodInt.h    $(CTIO)/ctio.h    $(FBHM_INC)    
#-------------------------------------------------------------------------------
$(HMOD)/hamming.o : $(HMOD_ALL)    $(CHAINDEF) 
$(HMOD)/mod_data.o: $(HMOD_ALL)    $(HMOD_TXT)    $(SYS)/sys.h   $(LGSW_INC)    \
                    $(BEW)/bew.h   $(APPL)/appl.h $(AUFG)/aufg.h $(EVO)/evo.h  
$(HMOD)/segm_gen.o: $(HMOD_ALL)    $(HMOD_TXT)    $(GLOB_TXT)    $(SYS)/sys.h   \
                    $(LGSW_INC)    $(APPL)/appl.h $(SIMU)/simu.h


