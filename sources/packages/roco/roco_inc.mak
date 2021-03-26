#
# Makefile for MAKE Utility
#
# Include-File fuer die MODOS-Quellen im "lsv"-Verzeichnis.
#
#-------------------------------------------------------------------------------
# Die Nutzung und Verbreitung dieser Software unterliegt einer Lizenz, die in 
# der Datei "schalter.h" und in der Quelle des Hauptprogramms enthalten ist. 
# Jegliche Nutzung und Verbreitung ausserhalb der Bedingungen dieser Lizenz ist 
# untersagt. Bei einer Entfernung oder Veraenderung dieses Texts oder des 
# Lizenztexts erlischt die Lizenz.
#-------------------------------------------+----------------------------------+
#   Autor: W.Jakob                          |   Stand:  20.08.2018             |
#-------------------------------------------+----------------------------------+

#-------------------------------------------------------------------------------
#      Definitionen der Objects:
#-------------------------------------------------------------------------------
ROCO_OBJ = $(ROCO)/mathe.o    $(ROCO)/merkmal.o  $(ROCO)/matrix.o $(ROCO)/rcStrings.o \
           $(ROCO)/vector.o   $(ROCO)/gramschm.o $(ROCO)/param.o  $(ROCO)/function.o  \
           $(ROCO)/constr.o   $(ROCO)/getfctp.o  $(ROCO)/objfct.o $(ROCO)/sim.o       \
           $(ROCO)/complbox.o $(ROCO)/rosencon.o

#-------------------------------------------------------------------------------
#      Beschreibung der Abhaengigkeiten:
#-------------------------------------------------------------------------------
$(ROCO)/mathe.o    : $(ROCO)/mathe.h
$(ROCO)/merkmal.o  : $(MAIN)/schalter.h   $(ROCO)/merkmal.h    $(ROCO)/bcc.h
$(ROCO)/matrix.o   : $(ROCO)/matrix.h     $(ROCO)/vector.h     $(MAIN)/schalter.h  \
                     $(ROCO)/bcc.h
$(ROCO)/rcStrings.o: $(ROCO)/rcStrings.h  $(MAIN)/schalter.h   $(ROCO)/bcc.h        \
                     $(ROCO)/matrix.h     $(ROCO)/vector.h     
$(ROCO)/vector.o   : $(ROCO)/vector.h     $(MAIN)/schalter.h   $(ROCO)/bcc.h        \
                     $(ROCO)/matrix.h     
$(ROCO)/gramschm.o : $(ROCO)/gramschm.h   $(ROCO)/vector.h     $(MAIN)/schalter.h  \
                     $(ROCO)/bcc.h
$(ROCO)/param.o    : $(ROCO)/param.h      $(ROCO)/merkmal.h    $(MAIN)/schalter.h  \
                     $(ROCO)/bcc.h
$(ROCO)/function.o : $(MAIN)/schalter.h   $(ROCO)/rcStrings.h  $(ROCO)/bcc.h       \
                     $(ROCO)/function.h   $(ROCO)/map.h        $(ROCO)/list.h      \
                     $(ROCO)/vector.h     
$(ROCO)/constr.o   : $(ROCO)/constr.h     $(ROCO)/vector.h     $(MAIN)/schalter.h  \
                     $(ROCO)/bcc.h        $(ROCO)/param.h      $(ROCO)/merkmal.h   \
                     $(ROCO)/function.h   $(ROCO)/rcStrings.h  $(ROCO)/map.h       \
                     $(ROCO)/list.h       
$(ROCO)/getfctp.o  : $(MAIN)/schalter.h   $(ROCO)/getfctp.h    $(ROCO)/vector.h    \
                     $(ROCO)/bcc.h        $(ROCO)/objfct.h     
$(ROCO)/objfct.o   : $(ROCO)/objfct.h     $(ROCO)/vector.h     $(MAIN)/schalter.h  \
                     $(ROCO)/bcc.h        
$(ROCO)/sim.o      : $(ROCO)/vector.h     $(MAIN)/schalter.h   $(ROCO)/bcc.h       \
                     $(ROCO)/matrix.h     $(ROCO)/function.h   $(ROCO)/rcStrings.h \
                     $(ROCO)/map.h        $(ROCO)/list.h       $(ROCO)/vector.h    \
                     $(ROCO)/merkmal.h    $(ROCO)/param.h      $(ROCO)/constr.h    \
                     $(ROCO)/getfctp.h    $(ROCO)/sim.h        
$(ROCO)/complbox.o : $(ROCO)/vector.h     $(MAIN)/schalter.h   $(ROCO)/bcc.h       \
                     $(ROCO)/param.h      $(ROCO)/merkmal.h    $(ROCO)/function.h  \
                     $(ROCO)/rcStrings.h  $(ROCO)/map.h        $(ROCO)/list.h      \
                     $(ROCO)/constr.h     $(ROCO)/sim.h        $(ROCO)/matrix.h    \
                     $(ROCO)/getfctp.h    $(ROCO)/complbox.h
$(ROCO)/rosencon.o : $(ROCO)/vector.h     $(MAIN)/schalter.h   $(ROCO)/bcc.h       \
                     $(ROCO)/param.h      $(ROCO)/merkmal.h    $(ROCO)/gramschm.h  \
                     $(ROCO)/function.h   $(ROCO)/rcStrings.h  $(ROCO)/map.h       \
                     $(ROCO)/list.h       $(ROCO)/constr.h     $(ROCO)/sim.h       \
                     $(ROCO)/matrix.h     $(ROCO)/getfctp.h    $(ROCO)/rosencon.h
