#-----------------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Include file for package: webIO
# WEBIO_CURL_OBJ: These objects cover the curl-library based communication interface.
#                 They are independent from the presence of a user interface.
# WEBIO_FILE_TUI: These objects provide a dialog based test-interface, which substitutes 
#                 the curl-interface and which communicates with "testOpalSimu" via files,
#                 if DBG_MODE is set. In this case, these objects work with TUI versions 
#                 only.
#
#-----------------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-----------------------------------------------------+----------------------------------+
#   Autor: W.Jakob                                    |   Stand:  04.05.2018             |
#-----------------------------------------------------+----------------------------------+

#-----------------------------------------------------------------------------------------
#      definition of objects:
#-----------------------------------------------------------------------------------------
#WEBIO_VISU_CURL_OBJ = $(WEBIO_CURL_OBJ)  $(WEBIO)/visuIO.o 
WEBIO_VISU_CURL_OBJ = $(WEBIO)/visuIO.o 
WEBIO_CURL_OBJ      = $(WEBIO)/listIO.o  $(WEBIO)/curlIO.o 
WEBIO_FILE_TUI      = $(WEBIO)/listIO.o  $(WEBIO)/fileIO.o 

#-----------------------------------------------------------------------------------------
#      description of the dependencies:
#-----------------------------------------------------------------------------------------
WEBIO_TXT  = $(WEBIO)/webIO_d.h $(WEBIO)/webIO_gb.h    
WEBIO_ALL  = $(WEBIO_TXT)       $(WEBIO)/webIO.h   $(WEBIO)/webIOInt.h  $(SYS)/sys.h       \
             $(CTIO)/ctio.h     $(FBHM_INC)        $(CHAINDEF)          
ESS_IO_ALL = $(WEBIO_ALL)       $(GLOB_TXT)        $(HMOD)/hmod.h       $(SIMU)/simu.h       
#-----------------------------------------------------------------------------------------
$(WEBIO)/visuIO.o : $(WEBIO_ALL)    $(EVO)/evo.h
$(WEBIO)/listIO.o : $(ESS_IO_ALL)   
$(WEBIO)/curlIO.o : $(ESS_IO_ALL)   $(CTIO)/ctio.h 
$(WEBIO)/fileIO.o : $(ESS_IO_ALL)   $(CTIO)/ctio.h 

