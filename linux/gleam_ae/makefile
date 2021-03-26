#-----------------------------------------------------------------------------------------
#
# Makefile for Linux: Ubuntu 16.04.5 LTS - GNU gcc V5.4.0 Compiler   MAKE Utility
#
# Call: make [clean | min_clean | ]
#         clean:     Deletes all objects in the involved directories and the executable.
#         min_clean: Deletes all objects in the current directory and the executable.
#         <empty>:   creates the executable
#
# For: gleam_ae (gleam_ae)
#
#-----------------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-----------------------------------------------------+----------------------------------+
#   Author: W.Jakob                                   |  Last update:  Sept. 27, 2018    |
#-----------------------------------------------------+----------------------------------+
.SILENT:

# ------------------------------------ Executable: ---------------------------------------
ziel = gleam_ae

# --- Targets: ------
all: $(ziel)


#-------------------------------- Global Include File: -----------------------------------
include glob_inc.mak


#----------------------------- Defintion of the Compiler: --------------------------------
CC = gcc

#----------------------------- Compiler Switches for gcc: --------------------------------
#dbg_flag    = -g
dbg_flag    = 
#OptFlag     =
#OptFlag     = -O1
OptFlag     = -O2
ProfileFlag =
WarnFlag    = -Wimplicit-function-declaration
#-----------------------------------------------------------------------------------------
CFLAGS   = $(STD_C_WARN_FLAGS) $(WarnFlag) $(OptFlag) $(dbg_flag) $(ProfileFlag)
INCFLAGS = $(STD_INC_FLAGS)  -I$(CHED) 
#-----------------------------------------------------------------------------------------


#----------------------------------- Specific Includes: ----------------------------------
#include $(VGL)/vgl_inc.mak


#-----------------------------------------------------------------------------------------
#      Definitionen of Objects:
#-----------------------------------------------------------------------------------------
#ALLOBJ = $(TUI_STND_OBJ)  $(VGL_OBJ)  $(ALLG)/gleam_ae.o
ALLOBJ = $(TUI_STND_OBJ)  $(ALLG)/gleam_ae.o


#-----------------------------------------------------------------------------------------
#      Description of the Common Dependencies of all Objects:
#-----------------------------------------------------------------------------------------
$(ALLOBJ) : $(MAIN)/schalter.h  $(SYS)/ttype.h  $(ALLG)/szsunsol.h


#-----------------------------------------------------------------------------------------
#             Linker:
#-----------------------------------------------------------------------------------------
$(ziel): $(ALLOBJ) 
	echo =============================== Linking $@ =============================
	$(CC) $(CFLAGS) -o $@ $(ALLOBJ) -lm
	echo 



