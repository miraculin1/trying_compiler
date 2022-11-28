ez: lex_C
	gcc -o scan lex.yy.c rdparser.c -lfl -g
	rm log
	./scan >> log

AST: lex_C
	gcc -o scan lex.yy.c rdparserAST.c ast.c -lfl -g

lex_C: auto_lex.l
	flex auto_lex.l
	flex --header-file=lex.h auto_lex.l

