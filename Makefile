# Get common rules from the following
include Makefile.inc

DIRS	= cli mol dataplane
EXE		= softswitch
OBJS	= main.o
DOXYDIR = doxy
# OBJLIBS	= libcli.a libmol.a
# LIBS	= -L. -lcli -lmol

.PHONY: all $(EXE) clean super_clean cscope doxy

all : main.o $(EXE) cscope # doxy

# introduce libcli.a and libmol.a in the following line
$(EXE) :
	for dir in $(DIRS); do \
		(cd $$dir; $(MAKE) all); \
	done
	$(LD) -o $(EXE) $(OBJECT_DIR)/*.o

clean :
	for dir in $(DIRS); do \
		(cd $$dir; $(MAKE) clean); \
	done
	$(RM) $(OBJS) $(EXE) $(OBJECT_DIR)/*.o
	
super_clean : clean cscope_clean doxy_clean

cscope : cscope_clean
	$(FIND) . -name  '*.cpp' -o -name '*.h' > cscope.files
	cscope -b -q -u

cscope_clean :
	$(RM) cscope.*

doxy :
	doxygen

doxy_clean :
	$(RM) -rf $(DOXYDIR)/*
