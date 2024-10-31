.PHONY: all clean test

crossw: crossw.c solver.c structs.c  solver.h structs.h
	gcc -o crossw crossw.c solver.c structs.c -ggdb

test: crossw
	./crossw data/Crossword1.txt -dict data/WordsPerfect1.txt
	./crossw data/Crossword1.txt -dict data/WordsPerfect1.txt -draw
	./crossw data/Crossword1.txt -dict data/WordsPerfect1.txt | ./crossw data/Crossword1.txt -dict data/Words.txt -check -draw
