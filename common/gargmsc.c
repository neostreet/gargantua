#include <stdio.h>
#include "garg.h"
#include "garg.glb"
#include "garg.fun"
#include "garg.mac"

extern char piece_ids[]; /* "RNBQK" */
extern char fmt_str[];

static int format_square(int square)
{
  int bBlack;
  int return_char;

  if (!square)
    return (int)'.';

  if (square < 0) {
    bBlack = TRUE;
    square *= -1;
  }
  else
    bBlack = FALSE;

  if (square == 1)
    return_char = 'P';
  else
    return_char = piece_ids[square - 2];

  if (!bBlack)
    return_char += ('a' - 'A');

  return return_char;
}

void print_bd(struct game *gamept)
{
  int m;
  int n;
  int square;

  for (m = 0; m < NUM_RANKS; m++) {
    for (n = 0; n < NUM_FILES; n++) {
      square = get_piece2(gamept,(NUM_RANKS - 1) - m,n);
      printf("%c ",format_square(square));
    }

    putchar(0x0a);
  }
}

void fprint_game_bin(struct game *gamept,char *filename)
{
  FILE *fptr;

  if ((fptr = fopen(filename,"w")) == NULL)
    return;

  fprintf(fptr,fmt_str,gamept->title);

  set_initial_board(gamept);

  for (gamept->curr_move = 0;
       gamept->curr_move <= gamept->num_moves;
       gamept->curr_move++) {

    fprintf_move(fptr,gamept);

    update_board(gamept);
  }

  fclose(fptr);
}

void fprint_game(struct game *gamept,char *filename)
{
  FILE *fptr;
  char buf[20];

  if ((fptr = fopen(filename,"w")) == NULL)
    return;

  fprintf(fptr,fmt_str,gamept->title);

  set_initial_board(gamept);

  for (gamept->curr_move = 0;
       gamept->curr_move <= gamept->num_moves;
       gamept->curr_move++) {

    sprintf_move(gamept,buf,20);
    fprintf(fptr,fmt_str,buf);

    update_board(gamept);
  }

  fclose(fptr);
}

void fprint_bd(struct game *gamept,char *filename)
{
  int m;
  int n;
  FILE *fptr;
  int square;

  if ((fptr = fopen(filename,"w")) == NULL)
    return;

  for (m = 0; m < NUM_RANKS; m++) {
    for (n = 0; n < NUM_FILES; n++) {
      square = get_piece2(gamept,(NUM_RANKS - 1) - m,n);
      fprintf(fptr,"%c ",format_square(square));
    }

    fputc(0x0a,fptr);
  }

  fclose(fptr);
}
