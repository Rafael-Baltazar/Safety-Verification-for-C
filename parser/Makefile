UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
CC=clang++
CFLAGS=-O3 -fPIC
libhp.dylib: verHornLexer.o verHornParser.o verVariable.h verClause.h verExpr.h verBExpr.h verFile.h
	$(CC) -shared -o libhp.dylib verHornLexer.o verHornParser.o
endif
ifeq ($(UNAME), Linux)
CC=g++
CFLAGS=-O3 -fPIC
libhp.so: verHornLexer.o verHornParser.o verVariable.h verClause.h verExpr.h verBExpr.h verFile.h
	$(CC) -shared -o libhp.so verHornLexer.o verHornParser.o
endif

.cpp.o:
	$(CC) $(CFLAGS) -c $<

verHornLexer.cpp: verHornLexer.lpp verHornParser.hpp
	flex -overHornLexer.cpp verHornLexer.lpp

verHornParser.cpp: verHornParser.ypp
	bison -o verHornParser.cpp -d verHornParser.ypp

verHornParser.hpp: verHornParser.ypp
	bison -o verHornParser.cpp -d verHornParser.ypp

clean:
	rm -rf libhp.* *.o	
