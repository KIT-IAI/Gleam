#-------------------------------------------------------------------------------
#
# Makefile for Linux: Ubuntu 16.04.5 LTS - GNU gcc V5.4.0 Compiler   MAKE Utility
#
# Common definitiones for "glob_inc.mak"-Include-Files fuer GLEAM, HyGLEAM,
# GLEAM_MPI, GLEAM_ESS, and derived applications.
#
#-------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-------------------------------------------+----------------------------------+
#   Author: W.Jakob                         |  Last update:  August 08, 2020   |
#-------------------------------------------+----------------------------------+


#-------------------------- All standard directories: --------------------------
PACK     = $(ALLG)/packages
SYS      = $(PACK)/sys
WEBIO    = $(PACK)/webIO
CTIO     = $(PACK)/ctio
FBHM     = $(PACK)/fbhm
LGSW     = $(PACK)/lgsw
CHIO     = $(PACK)/chio
BEW      = $(PACK)/bew
HMOD     = $(PACK)/hmod
AUFG     = $(PACK)/aufg
CHED     = $(PACK)/chedi
APPL     = $(PACK)/appl
SIMU     = $(PACK)/simu
LSKP     = $(SIMU)/lskp
CEC      = $(SIMU)/cec
VGL      = $(SIMU)/vgl
ROCO     = $(PACK)/roco
EVO      = $(PACK)/evo
MPAR     = $(PACK)/mPar
PAR_POP  = $(PACK)/parPop
MEN      = $(PACK)/men
VISU_IO  = $(PACK)/visuIO


#-------------------------------------------------------------------------------
#     Definition of file groups for the definition of dependencies:
#-------------------------------------------------------------------------------
MAIN_TXT      = $(MAIN)/gleam_d.h       $(MAIN)/gleam_gb.h
USR_MAIN_TXT  = $(ALLG)/gleam_usr_d.h   $(ALLG)/gleam_usr_gb.h
CLV_MAIN_TXT  = $(ALLG)/gleamCLV_d.h    $(ALLG)/gleamCLV_gb.h
GLOB_TXT      = $(ALLG)/glob_d.h        $(ALLG)/glob_gb.h   
VERS_TXT      = $(ALLG)/vers_d.h        $(ALLG)/vers_gb.h
CHAINDEF      = $(ALLG)/chaindef.h


#---------------- Dependencies of the common standard packages: ----------------
include $(SYS)/sys_inc.mak
include $(CTIO)/ctio_inc.mak
include $(LGSW)/lgsw_inc.mak
include $(BEW)/bew_inc.mak
include $(APPL)/appl_inc.mak
include $(HMOD)/hmod_inc.mak
include $(CHED)/ched_inc.mak
include $(LSKP)/lskp_inc.mak


#-------- Compiler include directives of the common standard packages: ---------
STD_INC_FLAGS = -I$(MAIN) -I$(ALLG) -I$(SYS)  -I$(CTIO) -I$(FBHM) -I$(LGSW) \
                -I$(CHIO) -I$(BEW)  -I$(AUFG) -I$(APPL) -I$(HMOD) -I$(SIMU) \
                -I$(LSKP) -I$(EVO)  -I$(MEN)


#-------------------------------------------------------------------------------
#      Set of standard compiler flags for handling warnings:
#-------------------------------------------------------------------------------
STD_C_WARN_FLAGS   = -Wunused-function 
STD_CPP_WARN_FLAGS = -Wunused-function -fpermissive -Wno-deprecated -Wno-write-strings -Wno-format-overflow


#-------------------------------------------------------------------------------
#     Dependencies of the two main programs:
#-------------------------------------------------------------------------------
MAIN_ALL = $(MAIN_TXT)     $(SYS)/sys.h     $(CTIO)/ctio.h   $(FBHM_INC)     \
           $(CHIO)/chio.h  $(HMOD)/hmod.h   $(AUFG)/aufg.h   $(SIMU)/simu.h  \
           $(MEN)/men.h    
CLV_ALL  = $(MAIN_ALL)     $(LGSW_INC)      $(APPL)/appl.h   $(BEW)/bew.h    \
           $(AUFG)/aufg.h  $(EVO)/evo.h    

$(ALLG)/gleam_ae.o :       $(MAIN_ALL)      $(USR_MAIN_TXT)  $(GLOB_TXT)  $(CHAINDEF)     

$(ALLG)/hyGleamEngine2.o:  $(CLV_ALL)       $(CLV_MAIN_TXT)  $(AUFG_TXT)  $(EVO_TXT)    


#-------------------------------------------------------------------------------
# -----  alte Kommandozeilenversion, welche noch von der MPI-Version benutzt wird: 
#ENG_MAIN_TXT = $(ALLG)/gleam_eng_d.h  $(ALLG)/gleam_eng_gb.h
#$(ALLG)/hy_gleam_engine.o: $(CLV_ALL)       $(ENG_MAIN_TXT)  $(GLOB_TXT)  



#-------------------------------------------------------------------------------
#             Definitions for compiling the objects:
#-------------------------------------------------------------------------------
COMP      = echo -------------------------------- $(@F) --------------------------------- ;\
            $(CC) -c $(CFLAGS) -o $@  $(INCFLAGS) 
CPPOMP    = echo -------------------------------- $(@F) --------------------------------- ;\
            $(CC) -c $(CPPFLAGS) -o $@  $(INCFLAGS) 
POSTCOMP  = echo ---------------------------------------------------------------------------- ;\
            echo
#-------------------------------------------------------------------------------
#             Common rules for generating objects: 
#-------------------------------------------------------------------------------
%.o : %.c
	$(COMP) $<
	$(POSTCOMP)
%.o : %.cc
	$(CPPOMP) $<
	$(POSTCOMP)

