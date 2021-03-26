#-----------------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Include file for package "chedi" for the objects of the version with 
#   - textual user interface (TUI) and with
#   - grafical user interface (GUI)
#
#-----------------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-----------------------------------------------------+----------------------------------+
#   Autor: W.Jakob                                    |   Stand:  09.01.2018             |
#-----------------------------------------------------+----------------------------------+

#-----------------------------------------------------------------------------------------
#      Definitionen der Objects:
#-----------------------------------------------------------------------------------------
CHED_TUI = $(CHED)/ch_ausg.o  $(CHED)/c_ch_edi.o
CHED_GUI = $(CHED)/ch_ausg.o  


#-----------------------------------------------------------------------------------------
#      Beschreibung der Abhaengigkeiten:
#-----------------------------------------------------------------------------------------
CHED_TXT = $(CHED)/ched_d.h    $(CHED)/ched_gb.h  
CHED_ALL = $(GLOB_TXT)         $(CHED_TXT)         $(SYS)/sys.h        $(CTIO)/ctio.h      \
           $(FBHM_INC)         $(LGSW_INC)         $(HMOD)/hmod.h      $(CHED)/ched.h      \
           $(CHED)/chedInt.h   
#-----------------------------------------------------------------------------------------
$(CHED)/ch_ausg.o : $(CHED_ALL)      
$(CHED)/c_ch_edi.o: $(CHED_ALL)  $(BEW)/bew.h        
