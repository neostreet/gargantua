#include <stdio.h>

#include "garg.h"
#define MAKE_GLOBALS_HERE
#include "garg.glb"
#include "garg.fun"
#include "garg.mac"

char couldnt_get_status[] = "couldn't get status of %s\n";
char couldnt_open[] = "couldn't open %s\n";

int bHaveGame;
int afl_dbg;

int main(int argc,char **argv)
{
  int retval;
  struct game curr_game;

  if (argc != 2) {
    printf("usage: prgame filename\n");
    return 1;
  }

  retval = read_binary_game(argv[1],&curr_game);

  if (retval) {
    printf("read_binary_game of %s failed: %d\n",argv[1],retval);
    return 2;
  }

  print_game(&curr_game);
  print_bd(&curr_game);

  return 0;
}
