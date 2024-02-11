print_garg_game: print_garg_game.o gargdbg.o gargmsc.o gargmvs.o gargrd.o bitfuns.o
	g++ -o print_garg_game print_garg_game.o gargmsc.o gargdbg.o gargmvs.o gargrd.o bitfuns.o

print_garg_game.o: print_garg_game.c
	g++ -g -O0 -c -I./common print_garg_game.c

gargdbg.o: ./common/gargdbg.c
	g++ -g -O0 -c -I./common ./common/gargdbg.c

gargmsc.o: ./common/gargmsc.c
	g++ -g -O0 -c -I./common ./common/gargmsc.c

gargmvs.o: ./common/gargmvs.c
	g++ -g -O0 -c -I./common ./common/gargmvs.c

gargrd.o: ./common/gargrd.c
	g++ -g -O0 -c -I./common ./common/gargrd.c

bitfuns.o: ./common/bitfuns.c
	g++ -g -O0 -c -I./common ./common/bitfuns.c

clean:
	rm *.o *.exe
