#-------------------------------------------------------------------------------
#
# Makefile for MAKE Utility
#
# Include file for package: opal_v
#
#-------------------------------------------------------------------------------
# This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
# version 3, see the comment section at the end of the main program files or the 
# files "schalter.h".
#-------------------------------------------+----------------------------------+
#   Author: W.Jakob                         |   Date:  16.08.2020              |
#-------------------------------------------+----------------------------------+

#-------------------------------------------------------------------------------
#      Definition of Objects:
#-------------------------------------------------------------------------------
OPALV_OBJ = $(OPALV)/opal_io.o  $(OPALV)/fio_if.o  $(OPALV)/opal_bew.o

#-------------------------------------------------------------------------------
OPALV_TXT = $(OPALV)/opal_d.h    $(OPALV)/opal_gb.h    
OPALV_ALL = $(OPALV)/opalInt.h   $(CTIO)/ctio.h    $(FBHM_INC)          \
            $(CHAINDEF)          $(HMOD)/hmod.h      

#-------------------------------------------------------------------------------
#      Description of the Dependencies:
#-------------------------------------------------------------------------------
$(OPALV)/opal_io.o : $(OPALV_ALL)      $(SYS)/sys.h      
$(OPALV)/fio_if.o  : $(OPALV_ALL)      $(GLOB_TXT)       $(SYS)/sys.h       \
                     $(APPL)/appl.h    
$(OPALV)/opal_bew.o: $(OPALV_ALL)      $(OPALV_TXT)      $(SYS)/sys.h       \
                     $(APPL)/appl.h    $(AUFG)/aufg.h    $(SIMU)/simu.h     