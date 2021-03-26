#-------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Include file for package "ctio" for the objects of the 
#   - version wiht Textual User-Interface (TUI) and of the
#   - Command Line Version (CLV) 
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
CTIO_TUI = $(CTIO)/scr_gsw.o   $(CTIO)/user_gsw.o  $(CTIO)/str_gsw.o 
CTIO_CLV = $(CTIO)/str_gsw.o

#-------------------------------------------------------------------------------
#      Beschreibung der Abhaengigkeiten:
#-------------------------------------------------------------------------------
CTIO_TXT = $(CTIO)/ctio_d.h   $(CTIO)/ctio_gb.h   
#-------------------------------------------------------------------------------
$(CTIO_TUI)        : $(CTIO)/ctio.h  $(CTIO)/ctioInt.h  $(GLOB_TXT)  $(CTIO_TXT) \
                     $(SYS)/sys.h    
$(CTIO)/user_gsw.o : $(CHAINDEF)     $(FBHM)/fbhm.h    
$(CTIO)/str_gsw.o  : $(CHAINDEF)         
