#-----------------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Include file for package "simu"    for CEC versions 
#
# This file provides the include for the standard version enhanced by CEC'05 benchmarks
# It is based on "simu_common_inc.mak".
#
# There are also two other alternative include files: 
#   simu_inc.mak     for the standard versions without parallelism
#   simu_ess_inc.mak for the include of the version based on external simulation 
#                    services (ESS) for parallel simulation requiring more dependencies
#
# There are two object groups defined in men_common_inc.mak for the versions with
#   - textual user interface (TUI) and the
#   - command line version (CLV).
#
#-----------------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-----------------------------------------------------------------------------------------
# Die Einbindung der CEC'05-Benchmarkfunktionen erfolgt auf der SW-Ebene durch Aktivierung 
# des globalen Schalters CEC_MBF im schalter.h-File oder lokal durchden Schalter MIT_CEC_MBFS 
# im Modul "simu/cec_sim.c". Siehe auch die dortige Dokumentation. Auf der make-Ebene muessen 
# die Module des CEC-Unterverzeichnisses mit uebersetzt und eingebunden werden.
#-----------------------------------------------------+----------------------------------+
#   Autor: W.Jakob                                    |   Stand: 09.01.2018              |
#-----------------------------------------------------+----------------------------------+


#----------------------------- common package include-file: ------------------------------
include $(SIMU)/simu_common_inc.mak


#-----------------------------------------------------------------------------------------
#      specific definition of objects:
#-----------------------------------------------------------------------------------------
EXT_PIPE_SIMU = $(SIMU)/eldokopl.o  $(SIMU)/mathkopl.o  $(SIMU)/gen_kopl.o  $(SIMU)/ext_sim.o   

SIMU_CEC_TUI  = $(SIMU_ALLG)  $(EXT_PIPE_SIMU)  $(SIMU)/simucmen.o   
SIMU_CEC_CLV  = $(SIMU_ALLG)  $(EXT_PIPE_SIMU)  


#-----------------------------------------------------------------------------------------
#      specific dependencies:
#-----------------------------------------------------------------------------------------
$(SIMU)/cec_sim.o : $(SYS)/sys.h       $(FBHM_INC)        $(CHAINDEF)        $(AUFG)/aufg.h     \
                    $(SIMU)/simu.h     $(SIMU)/simuInt.h  $(CEC)/global.h    $(CEC)/rand.h
$(SIMU)/simu_gsw.o: $(SIMU_ALL)        $(SIMU_TXT)        $(GLOB_TXT)        $(LGSW_INC)        \
                    $(BEW)/bew.h       $(APPL)/appl.h     
$(SIMU)/simucmen.o: $(SIMU_ALL)        $(SIMU_TXT)        $(GLOB_TXT)        $(LGSW_INC)        \
                    $(BEW)/bew.h       $(MEN)/men.h       



