# Common Makefile definitions to be used in all directories

SHELL	= /bin/sh
CC	= g++
CFLAGS	= -Wall -Werror -ansi -pedantic -g
# CFLAGS	= -Wall -Werror -Weffc++ -g           # -Weffc++ brings lots of warning from spirit library.
LD		= g++
INCLUDE_DIRS	= -I/usr/include -I.

# The following is the one and only line where absolute directory name is refferred to
MAKE_ROOT	= /private/arun/code/simulator/
OBJECT_DIR	= $(MAKE_ROOT)/obj

RM		= /bin/rm -rf
MV		= /bin/mv -f
CP		= /bin/cp -f
FIND    = /usr/bin/find

# $(OBJECT_DIR)/%.o : %.cpp
#	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDE_DIRS) -c $< -o $@
	cp $@ $(OBJECT_DIR)/

