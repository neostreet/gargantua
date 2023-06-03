#include <stdio.h>
#include "garg.h"
#include "garg.glb"
#include "garg.fun"
#include "garg.mac"
#include "bitfuns.h"

static unsigned char initial_board[] = {
  (unsigned char)0x70, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x07,
  (unsigned char)0x02, (unsigned char)0x34, (unsigned char)0x56, (unsigned char)0x43, (unsigned char)0x20,
  (unsigned char)0x01, (unsigned char)0x11, (unsigned char)0x11, (unsigned char)0x11, (unsigned char)0x10,
  (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00,
  (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00,
  (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00,
  (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00,
  (unsigned char)0x0f, (unsigned char)0xff, (unsigned char)0xff, (unsigned char)0xff, (unsigned char)0xf0,
  (unsigned char)0x0e, (unsigned char)0xdc, (unsigned char)0xba, (unsigned char)0xcd, (unsigned char)0xe0,
  (unsigned char)0x90, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x09,
};

extern char piece_ids[]; /* "RNBQKG" */
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

void fprint_game2(struct game *gamept,FILE *fptr)
{
  char buf[20];

  fprintf(fptr,fmt_str,gamept->title);

  set_initial_board(gamept);

  for (gamept->curr_move = 0;
       gamept->curr_move <= gamept->num_moves;
       gamept->curr_move++) {

    sprintf_move(gamept,buf,20);
    fprintf(fptr,fmt_str,buf);

    update_board(gamept);
  }
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

void fprint_bd2(struct game *gamept,FILE *fptr)
{
  int m;
  int n;
  int square;

  for (m = 0; m < NUM_RANKS; m++) {
    for (n = 0; n < NUM_FILES; n++) {
      square = get_piece2(gamept,(NUM_RANKS - 1) - m,n);
      fprintf(fptr,"%c ",format_square(square));
    }

    fputc(0x0a,fptr);
  }
}

void fprint_moves(struct game *gamept,char *filename)
{
  int n;
  FILE *fptr;

  if ((fptr = fopen(filename,"w")) == NULL)
    return;

  for (n = 0; n < gamept->num_moves; n++) {
    fprintf(fptr,"%d %d\n",gamept->moves[n].from,gamept->moves[n].to);
  }

  fclose(fptr);
}

void fprint_moves2(struct game *gamept,FILE *fptr)
{
  int n;

  for (n = 0; n < gamept->num_moves; n++) {
    fprintf(fptr,"%d %d\n",gamept->moves[n].from,gamept->moves[n].to);
  }
}

void set_initial_board(struct game *gamept)
{
  int n;

  for (n = 0; n < CHARS_IN_BOARD; n++)
    gamept->board[n] = initial_board[n];
}

void update_board(struct game *gamept)
{
  bool bKingsideCastle;
  bool bQueensideCastle;

  set_piece1(gamept,gamept->moves[gamept->curr_move].to,
    get_piece1(gamept,gamept->moves[gamept->curr_move].from));

  set_piece1(gamept,gamept->moves[gamept->curr_move].from,0);  /* vacate previous square */

  bKingsideCastle = (gamept->moves[gamept->curr_move].special_move_info == SPECIAL_MOVE_KINGSIDE_CASTLE);
  bQueensideCastle = (gamept->moves[gamept->curr_move].special_move_info == SPECIAL_MOVE_QUEENSIDE_CASTLE);

  if (bKingsideCastle) {
    if (!(gamept->curr_move % 2)) {
      // it's White's move
      set_piece1(gamept,16,
        get_piece1(gamept,18));
      set_piece1(gamept,18,0);
    }
    else {
      // it's Blacks's move
      set_piece1(gamept,86,
        get_piece1(gamept,88));
      set_piece1(gamept,88,0);
    }
  }
  else if (bQueensideCastle) {
    if (!(gamept->curr_move % 2)) {
      // it's White's move
      set_piece1(gamept,14,
        get_piece1(gamept,11));
      set_piece1(gamept,11,0);
    }
    else {
      // it's Blacks's move
      set_piece1(gamept,84,
        get_piece1(gamept,81));
      set_piece1(gamept,81,0);
    }
  }
}

int get_piece1(struct game *gamept,int board_offset)
{
  unsigned int bit_offset;
  unsigned short piece;
  int piece_int;

  bit_offset = board_offset * BITS_PER_BOARD_SQUARE;

  piece = get_bits(BITS_PER_BOARD_SQUARE,gamept->board,bit_offset);
  piece_int = piece;

  if (piece & 0x8)
    piece_int |= 0xfffffff0;

  return piece_int;
}

int get_piece2(struct game *gamept,int row,int column)
{
  int board_offset;

  board_offset = row * NUM_RANKS + column;
  return get_piece1(gamept,board_offset);
}

void set_piece1(struct game *gamept,int board_offset,int piece)
{
  unsigned int bit_offset;

  bit_offset = board_offset * BITS_PER_BOARD_SQUARE;
  set_bits(BITS_PER_BOARD_SQUARE,gamept->board,bit_offset,piece);
}

void set_piece2(struct game *gamept,int row,int column,int piece)
{
  int board_offset;

  board_offset = row * NUM_RANKS + column;
  set_piece1(gamept,board_offset,piece);
}
