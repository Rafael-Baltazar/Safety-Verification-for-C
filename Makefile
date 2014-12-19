UNAME=$(shell uname)
Z3DIR=../z3-4.3.2
PARSERDIR=./parser

ifeq ($(UNAME), Darwin)
CC=clang++
endif
ifeq ($(UNAME), Linux)
CC=g++
endif

CFLAGS=-Wall

smt: smt.cpp
	$(CC) $(CFLAGS) -o smt smt.cpp -lhp -lz3 -I. -L$(PARSERDIR) -L$(Z3DIR)/lib -I$(PARSERDIR) -I$(Z3DIR)/include

clean:
	rm -rf smt

rerun: clean smt
	./runme
