Assembler: Assembler.o preAssembler.o code.o table.o firstPass.o lexer.o secondPass.o
	gcc -Wall -ansi -pedantic preAssembler.o firstPass.o lexer.o Assembler.o secondPass.o code.o table.o -o Assembler
Assembler.o: Assembler.c preAssembler.h globals.h
	gcc  -c -Wall -ansi -pedantic Assembler.c -o Assembler.o
preAssembler.o: preAssembler.c preAssembler.h globals.h
	gcc -c -Wall -ansi -pedantic preAssembler.c  -o preAssembler.o
code.o: code.c code.h table.h globals.h
	gcc -c -Wall -ansi -pedantic code.c -o code.o
table.o: table.c table.h globals.h
	gcc -c -Wall -ansi -pedantic table.c -o table.o
firstPass.o: firstPass.c firstPass.h globals.h lexer.h
	gcc -c -Wall -ansi -pedantic firstPass.c -o firstPass.o
lexer.o: lexer.c lexer.h firstPass.h globals.h
	gcc -c -Wall -ansi -pedantic lexer.c -o lexer.o
secondPass.o: secondPass.c secondPass.h globals.h
	gcc -c -Wall -ansi -pedantic secondPass.c -o secondPass.o