#-----------------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Include file for package "men"   for the mpi version
#
# This file provides the include for the mpi-based parallel version.
# It is based on "men_common_inc.mak".
#
# There are also two other alternative include files: 
#   men_inc.mak     for the standard versions without parallelism
#   men_ess_inc.mak for the version based on external simulation services (ESS) 
#                   for parallel simulation
#
# There are three object groups defined in men_common_inc.mak for the versions with
#   - textual user interface (TUI), with
#   - grafical user interface (GUI), and the
#   - command line version (CLV).
#
#-----------------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-----------------------------------------------------+----------------------------------+
#   Autor: W.Jakob                                    |   Stand: 10.01.2018              |
#-----------------------------------------------------+----------------------------------+


#----------------------------- common package include-file: ------------------------------
include $(MEN)/men_common_inc.mak


#-----------------------------------------------------------------------------------------
#      specific dependencies:
#-----------------------------------------------------------------------------------------
$(MEN)/f_load.o:  $(MEN_ALL)         $(GLOB_TXT)        $(MAIN_TXT)        $(MPAR)/mPar.h    
$(MEN)/men_gsw.o: $(MEN_ALL)         $(GLOB_TXT)        $(VERS_TXT)       
