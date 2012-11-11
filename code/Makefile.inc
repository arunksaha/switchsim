# Common Makefile definitions to be used in all directories

SHELL	        = /bin/sh
CC	            = g++
CFLAGS	        = -Wall -Werror -ansi -pedantic -g
LD		        = g++
INCLUDE_DIRS	= -I/usr/include -I/usr/include/boost_1_52_0/ -I.

# This must be the first this in Makefile.common
MAKE_ROOT := $(dir $(lastword $(MAKEFILE_LIST)))

OBJECT_DIR	= $(MAKE_ROOT)/obj

RM		= /bin/rm -rf
MV		= /bin/mv -f
CP		= /bin/cp -f
FIND    = /usr/bin/find

%.o : %.cpp
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDE_DIRS) -c $< -o $@
	$(CP) $@ $(OBJECT_DIR)/
