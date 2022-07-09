#!/usr/bin/env bash

#normal 
#cc vimm.c -lncurses -lform -o vimo

#debug
cc -g -O0 vimm.c -lncurses -lform -o vimodebug
gdb --args vimodebug ../game/main.c
