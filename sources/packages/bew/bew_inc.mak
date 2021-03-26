#-------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Include file for package "bew" for the objects of the versions with 
#   - textual user interface (TUI), with
#   - grafical user interface (GUI), and the
#   - command line version (CLV).
#
#-------------------------------------------+----------------------------------+
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-------------------------------------------+----------------------------------+
#   Autor: W.Jakob                          |   Stand:  19.01.2018             |
#-------------------------------------------+----------------------------------+

#-------------------------------------------------------------------------------
#      Definitionen der Objects:
#-------------------------------------------------------------------------------
BEW_TUI  = $(BEW)/bewert.o   $(BEW)/bew_gsw.o   $(BEW)/bew_cmen.o  
BEW_CLV  = $(BEW)/bewert.o   $(BEW)/bew_gsw.o   
BEW_GUI  = $(BEW)/bewert.o   $(BEW)/bew_gsw.o   


#-------------------------------------------------------------------------------
#      Beschreibung der Abhaengigkeiten:
#-------------------------------------------------------------------------------
BEW_TXT = $(BEW)/bew_d.h   $(BEW)/bew_gb.h    
BEW_ALL = $(BEW_TXT)       $(BEW)/bew.h         $(BEW)/bewInt.h   $(SYS)/sys.h \
          $(FBHM_INC)      $(CHAINDEF)  
#-------------------------------------------------------------------------------
$(BEW)/bew_gsw.o  : $(BEW_ALL)  $(GLOB_TXT)     
$(BEW)/bewert.o   : $(BEW_ALL)             
$(BEW)/bew_cmen.o : $(BEW_ALL)  $(GLOB_TXT)     $(CTIO)/ctio.h    
