#-----------------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Include file for package "fbhm"     for the mpi version
#
# This file provides the include for the mpi-based parallel version.  
# It is based on "fbhm_common_inc.mak".
# 
# There is also the alternative include file "fbhm_inc.mak" for the standard versions.
#
#-----------------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-----------------------------------------------------+----------------------------------+
#   Autor: W.Jakob                                    |   Stand:  04.10.2020             |
#-----------------------------------------------------+----------------------------------+


#-----------------------------------------------------------------------------------------
#      specific definitions:
#-----------------------------------------------------------------------------------------
FBHM_INC = $(FBHM)/fbhm.h   $(ALLG)/f_codes.h  $(ALLG)/fcodes_par.h


#----------------------------- common package include-file: ------------------------------
include $(FBHM)/fbhm_common_inc.mak
