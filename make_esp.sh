#!/bin/sh
#
#	This simple script makes esparanoia on a Unix machine.
#
#	Revision:	$Revision: 1.1 $
#	Date:	$Date: 2019/12/12 14:19:23 $
#----------------------------------------------------------
gcc	-Wall -o esp32 -DSINGLE esparanoia.c -lm
gcc	-Wall -o esp64 -DDOUBLE esparanoia.c -lm
