#include <stdio.h>
#include <string.h>

#include "garg.h"
#define MAKE_GLOBALS_HERE
#include "garg.glb"
#include "garg.fun"
#include "garg.mac"

static char usage[] =
"usage: board_to_bin infile outfile\n";

int bHaveGame;
int afl_dbg;

int main(int argc,char **argv)
{
  int n;
  unsigned char board[CHARS_IN_BOARD];
  int retval;

  if (argc != 3) {
    printf(usage);
    return 1;
  }

  if (!strcmp(argv[1],argv[2])) {
    printf("outfile must differ from infile\n");
    return 2;
  }

  retval = populate_board_from_board_file(board,argv[1]);

  if (retval) {
    printf("populate_board_from_board_file failed: %d\n",retval);
    return 3;
  }

  write_board_to_binfile(board,argv[2]);

  return 0;
}
