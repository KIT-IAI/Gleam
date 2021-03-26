#-------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Include file for package: appl
#
#-------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-------------------------------------------+----------------------------------+
#   Author: W.Jakob                         |   Date:  09.01.2018              |
#-------------------------------------------+----------------------------------+

#-------------------------------------------------------------------------------
#      Definition of the Objects:
#-------------------------------------------------------------------------------
APPL_OBJ = $(APPL)/appl_if.o   $(APPL)/appl_sim.o  $(APPL)/appl_go.o    \
           $(APPL)/appl_rep.o  $(APPL)/appl_gen.o  $(APPL)/appl_fio.o  

#-------------------------------------------------------------------------------
APPL_TXT = $(APPL)/appl_d.h  $(APPL)/appl_gb.h  
APPL_ALL = $(APPL)/appl.h    $(APPL)/applInt.h  $(SYS)/sys.h    $(CTIO)/ctio.h  \
           $(FBHM_INC)       $(LGSW_INC)        $(HMOD)/hmod.h      

#-------------------------------------------------------------------------------
#      Description of the Dependencies:
#-------------------------------------------------------------------------------
$(APPL)/appl_if.o : $(APPL_ALL)   $(APPL_TXT)   $(BEW)/bew.h        
$(APPL)/appl_sim.o: $(APPL_ALL)   $(APPL_TXT)   $(SIMU)/simu.h     
$(APPL)/appl_go.o : $(APPL_ALL)   
$(APPL)/appl_rep.o: $(APPL_ALL)   
$(APPL)/appl_gen.o: $(APPL_ALL)   
$(APPL)/appl_fio.o: $(APPL_ALL)   $(APPL_TXT)   $(GLOB_TXT)          
