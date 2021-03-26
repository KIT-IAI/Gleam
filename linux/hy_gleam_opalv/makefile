#-------------------------------------------------------------------------------
#
# Makefile for Linux: Ubuntu 16.04.5 LTS - GNU gcc V5.4.0 Compiler   MAKE Utility
#
# Call: make [min_clean | clean | ]
#         clean:     Deletes all objects in the involved directories and the executable.
#         min_clean: Deletes the executable and all objects except ROCO, LSKP, and CEC.
#         <empty>:   creates the executable
#
# For: hyGleamOpalv (hy_gleam_opalv)
#
#-------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-------------------------------------------+----------------------------------+
#   Author: W.Jakob                         |   Date: 23.10.2018               |
#-------------------------------------------+----------------------------------+
.SILENT:

# --------------------------------- Executable: --------------------------------
ziel = hyGleamOpalv

# --- Targets: ------
all: $(ziel)


#---------------------------- Global Include File: -----------------------------
include glob_inc.mak


#------------------------- Defintion of the Compiler: --------------------------
CC = g++

#------------------------- Compiler Switches for g++: --------------------------
#dbg_flag = -g
dbg_flag = 
#OptFlag     =
#OptFlag     = -O1
OptFlag     = -O2
ProfileFlag =
#ProfileFlag = -p
WarnFlag    = -Wno-literal-suffix 
#-------------------------------------------------------------------------------
CFLAGS   = $(STD_C_WARN_FLAGS) $(WarnFlag) $(OptFlag) $(dbg_flag) $(ProfileFlag)
CPPFLAGS = $(STD_CPP_WARN_FLAGS) $(WarnFlag) $(OptFlag) $(dbg_flag) $(ProfileFlag)
INCFLAGS = $(STD_INC_FLAGS)  -I$(CHED) -I$(ROCO) -I$(OPALV)
#-------------------------------------------------------------------------------


#-------------------------------------------------------------------------------
#      Definition of Objects:
#-------------------------------------------------------------------------------
ALLOBJ = $(TUI_OBJ)  $(EVO_LSVRC_TUI)  $(ALLG)/gleam_ae.o


#-------------------------------------------------------------------------------
#      Description of the Common Dependencies of all Objects:
#-------------------------------------------------------------------------------
$(ALLOBJ) :   $(MAIN)/schalter.h  $(SYS)/ttype.h     $(ALLG)/szsunsol.h


#-------------------------------------------------------------------------------
#             Linker:
#-------------------------------------------------------------------------------
$(ziel): $(ALLOBJ) 
	echo =============================== Linking $@ =============================
	$(CC) $(CFLAGS) -o $@ $(ALLOBJ) -lm
	echo 
