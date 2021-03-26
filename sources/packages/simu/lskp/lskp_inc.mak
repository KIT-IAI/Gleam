#-----------------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Include file for package "simu/lskp" for the objects of the 
#   - version with Textual User-Interface (TUI) and of the
#   - Command Line Version (CLV) 
#
# There are three object groups:
# 1. LSKP_TUI/_CLV:   Robot interpreter based on a unified action for the motors and an 
#                     axis number as action parameter
# 2. HYLSKP_TUI/_CLV: Robot interpreter based on different motor actions for each axis.
# 3. NO_ROB_OBJ:      No robot interpreter. The dummy module just satisfies the interface.
#
#-----------------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-----------------------------------------------------+----------------------------------+
#   Autor: W.Jakob                                    |   Stand:  08.01.2018             |
#-----------------------------------------------------+----------------------------------+

#-----------------------------------------------------------------------------------------
#      Definitionen der Objects:
#-----------------------------------------------------------------------------------------
LSK_BASE    = $(LSKP)/pasromin.o   $(LSKP)/rob_gsw.o    
LSKP_BASE   = $(LSK_BASE)          $(LSKP)/lsk_grep.o   $(LSKP)/rob_sim.o   
HYLSKP_BASE = $(LSK_BASE)          $(LSKP)/hy_grep.o    $(LSKP)/robsimhy.o   

LSKP_TUI    = $(LSKP_BASE)         $(LSKP)/rob_dial.o   
LSKP_CLV    = $(LSKP_BASE)         

HYLSKP_TUI  = $(HYLSKP_BASE)       $(LSKP)/rob_dial.o   
HYLSKP_CLV  = $(HYLSKP_BASE)       

NO_ROB_OBJ  = $(LSKP)/no_rob.o

#-----------------------------------------------------------------------------------------
#      Beschreibung der Abhaengigkeiten:
#-----------------------------------------------------------------------------------------
SIMU_TXT = $(SIMU)/simu_d.h    $(SIMU)/simu_gb.h  
LSKP_ALL = $(FBHM_INC)         $(LSKP)/joints.h   

#-----------------------------------------------------------------------------------------
$(LSKP)/lsk_grep.o: $(CTIO)/ctio.h      $(LGSW_INC)         $(LSKP)/lsk_mod.h   
$(LSKP)/hy_grep.o:  $(CTIO)/ctio.h      $(LGSW_INC)         $(LSKP)/lskmodhy.h  
$(LSKP)/pasromin.o: $(LSKP_ALL)         $(SYS)/sys.h        $(CHAINDEF)         \
                    $(LSKP)/rob_int.h   
$(LSKP)/rob_gsw.o:  $(LSKP_ALL)         $(SIMU_TXT)         $(SYS)/sys.h        \
                    $(CTIO)/ctio.h      $(CHAINDEF)         $(LSKP)/rob_int.h   
$(LSKP)/rob_dial.o: $(LSKP_ALL)         $(SIMU_TXT)         $(GLOB_TXT)         \
                    $(SYS)/sys.h        $(CTIO)/ctio.h      $(CHAINDEF)         \
                    $(LSKP)/rob_int.h   $(SIMU)/simu.h      

$(LSKP)/rob_sim.o : $(LSKP_ALL)         $(SIMU_TXT)         $(GLOB_TXT)         \
                    $(SYS)/sys.h        $(CTIO)/ctio.h      $(LGSW_INC)         \
                    $(BEW)/bew.h        $(AUFG)/aufg.h      $(LSKP)/rob_int.h   \
                    $(LSKP)/lsk_mod.h   

$(LSKP)/robsimhy.o: $(LSKP_ALL)         $(SIMU_TXT)         $(GLOB_TXT)         \
                    $(SYS)/sys.h        $(CTIO)/ctio.h      $(LGSW_INC)         \
                    $(BEW)/bew.h        $(AUFG)/aufg.h      $(LSKP)/rob_int.h   \
                    $(LSKP)/lskmodhy.h   

$(LSKP)/no_rob.o  : $(SYS)/sys.h        $(CTIO)/ctio.h      $(FBHM_INC)         \
                    $(CHAINDEF)         $(SIMU)/simu.h      
