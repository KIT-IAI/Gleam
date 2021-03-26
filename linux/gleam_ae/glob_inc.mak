
#-----------------------------------------------------------------------------------------
#
# Makefile for Linux: Ubuntu 16.04.5 LTS - GNU gcc V5.4.0 Compiler   MAKE Utility
#
# Global include file for GLEAM/AE (gleam_ae) as command line version (CLV) or with 
# textual user interface (TUI). 
#
#-----------------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-----------------------------------------------------+----------------------------------+
#   Author: W.Jakob                                   |  Last update: July 26, 2020      |
#-----------------------------------------------------+----------------------------------+

#------------------------------- Root Directory: -------------------------------
# This path MUST be adapted to the situation of the target system:
ROOT = /home/wilfried/gleam


#--------------------------- Directory Declarations: ---------------------------
ALLG     = $(ROOT)/sources
MAIN     = $(ROOT)/linux/gleam_ae


#-------------------- Include of General GLEAM Definitions: --------------------
include $(ALLG)/common_def.mak


#---------------------- Specific Includes of Some Packages: --------------------
include $(FBHM)/fbhm_inc.mak
include $(CHIO)/chio_inc.mak
include $(AUFG)/aufg_inc.mak
include $(SIMU)/simu_inc.mak
include $(EVO)/evo_inc.mak
include $(MEN)/men_inc.mak


#-----------------------------------------------------------------------------------------
#      Definition of the Basic Object Groups:
#-----------------------------------------------------------------------------------------
BASIC_OBJ     = $(SYS_OBJ)  $(FBHM_OBJ)  $(LGSW_OBJ)  $(CHIO_OBJ)  $(APPL_OBJ)  $(HMOD_OBJ) 

TUI_BASIC1_OBJ = $(CTIO_TUI) $(BEW_TUI)  $(AUFG_TUI)  $(CHED_TUI) 
CLV_BASIC1_OBJ = $(CTIO_CLV) $(BEW_CLV)  $(AUFG_CLV)             
#TUI_BASIC2_OBJ = $(LSKP_TUI) $(SIMU_TUI) $(EVO_TUI)  $(MEN_TUI) 
#CLV_BASIC2_OBJ = $(LSKP_CLV) $(SIMU_CLV) $(EVO_CLV)  $(MEN_CLV)
TUI_BASIC2_OBJ = $(HYLSKP_TUI) $(SIMU_TUI) $(EVO_TUI)  $(MEN_TUI) 
CLV_BASIC2_OBJ = $(HYLSKP_CLV) $(SIMU_CLV) $(EVO_CLV)  $(MEN_CLV)

TUI_STND_OBJ  = $(BASIC_OBJ) $(TUI_BASIC1_OBJ) $(TUI_BASIC2_OBJ)
CLV_STND_OBJ  = $(BASIC_OBJ) $(CLV_BASIC1_OBJ) $(CLV_BASIC2_OBJ)



#-----------------------------------------------------------------------------------------
#	Clean Up:
#-----------------------------------------------------------------------------------------
min_clean:
	echo rm -f $(ALLG)/*.o $(ziel)
	rm      -f $(ALLG)/*.o $(ziel)

#-----------------------------------------------------------------------------------------
clean:
	echo rm -f $(ALLG)/*.o $(ziel)
	rm      -f $(ALLG)/*.o $(ziel)
	echo rm -f $(SYS)/*.o
	rm      -f $(SYS)/*.o
	echo rm -f $(CTIO)/*.o
	rm      -f $(CTIO)/*.o
	echo rm -f $(FBHM)/*.o
	rm      -f $(FBHM)/*.o
	echo rm -f $(LGSW)/*.o
	rm      -f $(LGSW)/*.o
	echo rm -f $(CHIO)/*.o
	rm      -f $(CHIO)/*.o
	echo rm -f $(BEW)/*.o
	rm      -f $(BEW)/*.o
	echo rm -f $(AUFG)/*.o
	rm      -f $(AUFG)/*.o
	echo rm -f $(APPL)/*.o
	rm      -f $(APPL)/*.o
	echo rm -f $(HMOD)/*.o
	rm      -f $(HMOD)/*.o
	echo rm -f $(CHED)/*.o
	rm      -f $(CHED)/*.o
	echo rm -f $(SIMU)/*.o
	rm      -f $(SIMU)/*.o
	echo rm -f $(LSKP)/*.o
	rm      -f $(LSKP)/*.o
	echo rm -f $(VGL)/*.o
	rm      -f $(VGL)/*.o
	echo rm -f $(EVO)/*.o
	rm      -f $(EVO)/*.o
	echo rm -f $(MEN)/*.o
	rm      -f $(MEN)/*.o

