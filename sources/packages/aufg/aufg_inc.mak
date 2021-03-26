#-------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Include file for package "aufg"   for standard versions
#
# This file provides the standard include for versions without ESS based parallelism.
# It is based on "aufg_common_inc.mak".
#
# There is also "aufg_ess_inc.mak" for the versions based on external simulation 
# services (ESS) for parallel simulation.
#
#-------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-------------------------------------------+----------------------------------+
#   Autor: W.Jakob                          |   Stand:  18.04.2018             |
#-------------------------------------------+----------------------------------+


#------------------------ common package include-file: -------------------------
include $(AUFG)/aufg_common_inc.mak


#-------------------------------------------------------------------------------
#      specific dependencies:
#-------------------------------------------------------------------------------
$(AUFG)/tsk_data.o: $(AUFG_ALL)  $(CHIO)/chio.h  $(BEW)/bew.h  $(APPL)/appl.h  \
                    $(LSKP)/joints.h  
