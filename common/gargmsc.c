#include <stdio.h>
#include "garg.h"
#include "garg.glb"
#include "garg.fun"
#include "garg.mac"
#include "bitfuns.h"

static unsigned char initial_board[] = {
  (unsigned char)0x72, (unsigned char)0x34, (unsigned char)0x56, (unsigned char)0x43, (unsigned char)0x27,
  (unsigned char)0x11, (unsigned char)0x11, (unsigned char)0x11, (unsigned char)0x11, (unsigned char)0x11,
  (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00,
  (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00,
  (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00,
  (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00,
  (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00,
  (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00,
  (unsigned char)0xff, (unsigned char)0xff, (unsigned char)0xff, (unsigned char)0xff, (unsigned char)0xff,
  (unsigned char)0x9e, (unsigned char)0xdc, (unsigned char)0xba, (unsigned char)0xcd, (unsigned char)0xe9
};

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

void set_initial_board(struct game *gamept)
{
  int n;

  for (n = 0; n < CHARS_IN_BOARD; n++)
    gamept->board[n] = initial_board[n];
}

void update_board(struct game *gamept)
{
  if (gamept->moves[gamept->curr_move].from == SPECIAL_MOVE_KINGSIDE_CASTLE) {
    if (gamept->curr_move & 0x1) {
      /* black's move */

      set_piece1(gamept,60,0);
      set_piece1(gamept,61,ROOK_ID * -1);
      set_piece1(gamept,62,KING_ID * -1);
      set_piece1(gamept,63,0);
    }
    else {
      /* white's move */

      set_piece1(gamept,4,0);
      set_piece1(gamept,5,ROOK_ID);
      set_piece1(gamept,6,KING_ID);
      set_piece1(gamept,7,0);
    }
  }
  else if (gamept->moves[gamept->curr_move].from == SPECIAL_MOVE_QUEENSIDE_CASTLE) {
    if (gamept->curr_move & 0x1) {
      /* black's move */

      set_piece1(gamept,56,0);
      set_piece1(gamept,58,KING_ID * -1);
      set_piece1(gamept,59,ROOK_ID * -1);
      set_piece1(gamept,60,0);
    }
    else {
      /* white's move */

      set_piece1(gamept,0,0);
      set_piece1(gamept,2,KING_ID);
      set_piece1(gamept,3,ROOK_ID);
      set_piece1(gamept,4,0);
    }
  }
  else {
    set_piece1(gamept,gamept->moves[gamept->curr_move].to,
      get_piece1(gamept,gamept->moves[gamept->curr_move].from));

    set_piece1(gamept,gamept->moves[gamept->curr_move].from,0);  /* vacate previous square */
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
