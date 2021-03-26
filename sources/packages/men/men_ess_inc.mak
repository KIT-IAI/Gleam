#-----------------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Include file for package "men"    for ESS versions 
#
# This file provides the include for the ESS-based parallel version (External Simulation 
# Services).
# It is based on "men_common_inc.mak".
#
# There are also two other alternative include files: 
#   men_inc.mak     for the standard versions without parallelism
#   men_mpi_inc.mak for the parallel mpi version
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
$(MEN)/f_load.o:  $(MEN_ALL)         $(GLOB_TXT)        $(MAIN_TXT)        
$(MEN)/men_gsw.o: $(MEN_ALL)         $(GLOB_TXT)        $(VERS_TXT)       $(WEBIO)/webIO.h   
