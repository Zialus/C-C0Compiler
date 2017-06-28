all: preclean bison flex compilador clean

preclean:
	touch compilador
	rm compilador

bison:
	bison -dv parser.y

flex:
	flex flex.l

compilador:
	clang parser.tab.c lex.yy.c tree.c assembler.c -o compilador

clean:
	rm lex.yy.c parser.tab.c parser.tab.h
