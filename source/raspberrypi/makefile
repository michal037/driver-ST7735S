#
# MIT License
# Copyright (c) from 2017, Michal Kozakiewicz, github.com/michal037
#

# Can be edit
OUTNAME=st7735s_app
EXAMPLE=main.c

# Do not edit
CC=gcc
CFLAGS=-Wall -O2
SOURCES=st7735s.c st7735s.h
LIBS=-lwiringPi

.PHONY: help compile run clean

help:
	@echo "MAKEFILE HELP:\n Use:\n  make compile/run/clean/help"

compile:
	@clear # Clear display
	$(CC) $(LIBS) $(CFLAGS) -o $(OUTNAME) $(EXAMPLE) $(SOURCES)

clean:
	rm -rf $(OUTNAME)

run: clean compile
	./$(OUTNAME)
