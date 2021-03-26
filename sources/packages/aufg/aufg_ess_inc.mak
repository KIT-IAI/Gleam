#-----------------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Include file for package "aufg"    for ESS versions 
#
# This file provides the include for the ESS-based parallel version (External Simulation 
# Services).
# It is based on "aufg_common_inc.mak".
#
# There is also "aufg_inc.mak" for the standard versions without parallelism.
#
#-----------------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-----------------------------------------------------+----------------------------------+
#   Autor: W.Jakob                                    |   Stand:  09.01.2018             |
#-----------------------------------------------------+----------------------------------+


#----------------------------- common package include-file: ------------------------------
include $(AUFG)/aufg_common_inc.mak


#-----------------------------------------------------------------------------------------
#      specific dependencies:
#-----------------------------------------------------------------------------------------
$(AUFG)/tsk_data.o: $(AUFG_ALL)  $(BEW)/bew.h  $(APPL)/appl.h  $(LSKP)/joints.h  $(WEBIO)/webIO.h    
