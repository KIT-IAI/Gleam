#-----------------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Common include file for package: men
# This file is used by 
#   - men_inc.mak      for the standard include
#   - men_ess_inc.mak  for the include of the version based on external simulation 
#                      services (ESS) for parallel simulation requiring more dependencies.
#   - men_mpi_inc.mak  for the include of the parallel mpi version requiring more dependencies.
#
# There are three object groups for the versions with
#   - textual user interface (TUI), with
#   - grafical user interface (GUI), and the
#   - command line version (CLV).
#
#-----------------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-----------------------------------------------------+----------------------------------+
#   Autor: W.Jakob                                    |   Stand:  10.01.2018             |
#-----------------------------------------------------+----------------------------------+


#-----------------------------------------------------------------------------------------
#       common definition of objects:
#-----------------------------------------------------------------------------------------
MEN_TUI = $(MEN)/f_load.o   $(MEN)/men_gsw.o   $(MEN)/g_cmen.o
MEN_CLV = $(MEN)/f_load.o   
MEN_GUI = $(MEN)/f_load.o   $(MEN)/men_gsw.o   


#-----------------------------------------------------------------------------------------
#      common definitions:
#-----------------------------------------------------------------------------------------
MEN_TXT  = $(MEN)/men_d.h     $(MEN)/men_gb.h    
MEN_ALL  = $(MEN_TXT)         $(MEN)/men.h       $(MEN)/menInt.h    $(SYS)/sys.h       \
           $(CTIO)/ctio.h     $(FBHM_INC)        $(LGSW_INC)        $(CHIO)/chio.h     \
           $(BEW)/bew.h       $(APPL)/appl.h     $(HMOD)/hmod.h     $(AUFG)/aufg.h     \
           $(SIMU)/simu.h     $(EVO)/evo.h       


#-----------------------------------------------------------------------------------------
#      common dependencies:
#-----------------------------------------------------------------------------------------
$(MEN)/g_cmen.o : $(MEN_ALL)         $(GLOB_TXT)        $(CHED)/ched.h
