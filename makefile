gargantua: gargantua.o gargdbg.o gargmsc.o gargmvs.o gargrd.o bitfuns.o gargantua.res
	g++ -mwindows -g -L"/cygdrive/c/Program Files (x86)/Windows Kits/8.0/Lib/win8/um/x86" -o gargantua gargantua.o gargmsc.o gargdbg.o gargmvs.o gargrd.o bitfuns.o gargantua.res -lGdi32 -lcurses -lComDlg32 -lComCtl32

gargantua.o: gargantua.c
	g++ -g -O0 -c -I./common gargantua.c

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

gargantua.res: ./common/gargantua.rc
	windres ./common/gargantua.rc -O coff -o gargantua.res

clean:
	rm *.o *.res *.exe
