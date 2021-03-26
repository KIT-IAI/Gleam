#-------------------------------------------------------------------------------
#
# Makefile for Linux: Ubuntu 16.04.5 LTS - GNU gcc V5.4.0 Compiler   MAKE Utility
#
# Call: make -f hyGleamOpalvCLV.mak [min_clean | clean | ]
#         clean:     Deletes all objects in the involved directories and the executable.
#         min_clean: Deletes the executable and all objects except ROCO, LSKP, and CEC.
#         <empty>:   creates the executable
#
# For: hyGleamOpalvCLV  (hy_gleam_opalv)
#
#-------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-------------------------------------------+----------------------------------+
#   Author: W.Jakob                         |   Last update: Oct. 10, 2018     |
#-------------------------------------------+----------------------------------+
.SILENT:

# --------------------------------- Executable: --------------------------------
ziel = hyGleamOpalvCLV

# --- Targets: ------
all: $(ziel)


#---------------------------- Global Include File: -----------------------------
include glob_inc.mak


#------------------------- Defintion of the Compiler: --------------------------
CC = g++

#------------------------- Compiler Switches for g++: --------------------------
#dbg_flag = -g
dbg_flag =
OptFlag     = -O2
#OptFlag     = -O1
ProfileFlag =
WarnFlag    = -Wno-literal-suffix 
#WarnFlag    = -Wall
#-------------------------------------------------------------------------------
CFLAGS   = $(STD_C_WARN_FLAGS) $(WarnFlag) $(OptFlag) $(dbg_flag) $(ProfileFlag)
CPPFLAGS = $(STD_CPP_WARN_FLAGS) $(WarnFlag) $(OptFlag) $(dbg_flag) $(ProfileFlag)
INCFLAGS = $(STD_INC_FLAGS) -I$(ROCO)
#-------------------------------------------------------------------------------


#-------------------------------------------------------------------------------
#      Definitions of the Objects:
#-------------------------------------------------------------------------------
ALLOBJ = $(CLV_OBJ)  $(EVO_LSVRC_CLV)  $(ALLG)/hyGleamEngine2.o


#-------------------------------------------------------------------------------
#      Description of the Common Dependencies of all Objects:
#-------------------------------------------------------------------------------
$(ALLOBJ) : $(MAIN)/schalter.h  $(SYS)/ttype.h  $(ALLG)/szsunsol.h


#-------------------------------------------------------------------------------
#             Linker:
#-------------------------------------------------------------------------------
$(ziel): $(ALLOBJ) 
	echo =============================== Linking $@ =============================
	$(CC) $(CFLAGS) -o $@ $(ALLOBJ) -lm
	echo 

