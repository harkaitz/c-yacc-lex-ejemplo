.POSIX:
.SUFFIXES:
.PHONY: all clean install check

PROJECT   =c-minicalc
VERSION   =1.0.0
CC        =$(TPREFIX)gcc -Wall -g3 -std=c99
PROGS     =$(BUILDDIR)/minicalc$(EXE)
PREFIX    =/usr/local
BUILDDIR ?=.build
SOURCES   =main.c lex.yy.c y.tab.c ast.c int.c lib.c

all: $(PROGS)
clean:
	rm -f $(PROGS)
install: $(PROGS)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $(PROGS) $(DESTDIR)$(PREFIX)/bin
check:

$(BUILDDIR)/minicalc$(EXE): $(SOURCES)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^ -lm

## -- BLOCK:c --
clean: clean-c
clean-c:
	rm -f *.o
## -- BLOCK:c --
## -- BLOCK:yacc --
lex.yy.c: lex.l 
	lex lex.l 
	sed -i '/yy_is_interactive =.*fileno/s|=.*\;|=$(or $(LEX_INTERACTIVE),1)\;|' $@
y.tab.c y.tab.h: parser.y 
	yacc -d $(YFLAGS) parser.y 
clean: clean-parser
clean-parser:
	rm -f lex.yy.c y.tab.c y.tab.h
## -- BLOCK:yacc --
