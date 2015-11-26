.PHONY: lex parser
all:	lex parser

lex:
	$(MAKE) -C src/lex
	bash lex/test.sh

parser:
	$(MAKE) -C src/parser
	bash parser/test.sh

clean:
	$(MAKE) -C src/parser clean
	$(MAKE) -C src/lex clean
	
clobber:
	$(MAKE) -C src/parser clobber
	$(MAKE) -C src/lex clobber
