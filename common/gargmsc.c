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

    update_board(gamept,NULL,NULL);
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

    update_board(gamept,NULL,NULL);
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
    fprintf(fptr,"%d %d %d\n",gamept->moves[n].from,gamept->moves[n].to,gamept->moves[n].special_move_info);
  }

  fclose(fptr);
}

void fprint_moves2(struct game *gamept,FILE *fptr)
{
  int n;

  for (n = 0; n < gamept->num_moves; n++) {
    fprintf(fptr,"%d %d %d\n",gamept->moves[n].from,gamept->moves[n].to,gamept->moves[n].special_move_info);
  }
}

void set_initial_board(struct game *gamept)
{
  int n;

  for (n = 0; n < CHARS_IN_BOARD; n++)
    gamept->board[n] = initial_board[n];
}

static int update_board_calls;
static int dbg_update_board_call;
static int dbg;

void update_board(struct game *gamept,int *invalid_squares,int *num_invalid_squares)
{
  bool bBlack;
  int from_piece;
  bool bKingsideCastle = false;
  bool bQueensideCastle = false;;

  update_board_calls++;

  if (dbg_update_board_call == update_board_calls)
    dbg = 0;

  bBlack = (gamept->curr_move % 2);

  from_piece = get_piece1(gamept,gamept->moves[gamept->curr_move].from);

  switch (gamept->moves[gamept->curr_move].special_move_info) {
    case SPECIAL_MOVE_KINGSIDE_CASTLE:
      bKingsideCastle = true;

      break;
    case SPECIAL_MOVE_QUEENSIDE_CASTLE:
      bQueensideCastle = true;

      break;
    case SPECIAL_MOVE_PROMOTION_QUEEN:
      from_piece = (bBlack ? QUEEN_ID * -1 : QUEEN_ID);

      break;
    case SPECIAL_MOVE_PROMOTION_ROOK:
      from_piece = (bBlack ? ROOK_ID * -1 : ROOK_ID);

      break;
    case SPECIAL_MOVE_PROMOTION_BISHOP:
      from_piece = (bBlack ? BISHOP_ID * -1 : BISHOP_ID);

      break;
    case SPECIAL_MOVE_PROMOTION_KNIGHT:
      from_piece = (bBlack ? KNIGHT_ID * -1 : KNIGHT_ID);

      break;
    case SPECIAL_MOVE_PROMOTION_GARGANTUA:
      from_piece = (bBlack ? GARGANTUA_ID * -1 : GARGANTUA_ID);

      break;
  }

  if (invalid_squares) {
    *num_invalid_squares = 0;
    invalid_squares[(*num_invalid_squares)++] = gamept->moves[gamept->curr_move].from;
    invalid_squares[(*num_invalid_squares)++] = gamept->moves[gamept->curr_move].to;
  }

  set_piece(gamept,gamept->moves[gamept->curr_move].to,from_piece);

  set_piece(gamept,gamept->moves[gamept->curr_move].from,0);  /* vacate previous square */

  if (bKingsideCastle) {
    if (!(gamept->curr_move % 2)) {
      // it's White's move
      set_piece(gamept,16,
        get_piece1(gamept,18));
      set_piece(gamept,18,0);

      if (invalid_squares) {
        invalid_squares[(*num_invalid_squares)++] = 16;
        invalid_squares[(*num_invalid_squares)++] = 18;
      }
    }
    else {
      // it's Blacks's move
      set_piece(gamept,86,
        get_piece1(gamept,88));
      set_piece(gamept,88,0);

      if (invalid_squares) {
        invalid_squares[(*num_invalid_squares)++] = 86;
        invalid_squares[(*num_invalid_squares)++] = 88;
      }
    }
  }
  else if (bQueensideCastle) {
    if (!(gamept->curr_move % 2)) {
      // it's White's move
      set_piece(gamept,14,
        get_piece1(gamept,11));
      set_piece(gamept,11,0);

      if (invalid_squares) {
        invalid_squares[(*num_invalid_squares)++] = 14;
        invalid_squares[(*num_invalid_squares)++] = 11;
      }
    }
    else {
      // it's Blacks's move
      set_piece(gamept,84,
        get_piece1(gamept,81));
      set_piece(gamept,81,0);

      if (invalid_squares) {
        invalid_squares[(*num_invalid_squares)++] = 84;
        invalid_squares[(*num_invalid_squares)++] = 81;
      }
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

static int set_piece_calls;
static int dbg_set_piece_call;
static int dbg_board_offset;
static int dbg_piece;

void set_piece(struct game *gamept,int board_offset,int piece)
{
  unsigned int bit_offset;

  set_piece_calls++;

  if (set_piece_calls == dbg_set_piece_call)
    dbg = 1;

  if ((board_offset == dbg_board_offset) && (piece == dbg_piece))
    dbg = 1;

  if (gamept->debug_level == 2) {
    if (gamept->debug_fptr != NULL)
      fprintf(gamept->debug_fptr,"set_piece: board_offset = %d, piece %d\n",board_offset,piece);
  }

  bit_offset = board_offset * BITS_PER_BOARD_SQUARE;
  set_bits(BITS_PER_BOARD_SQUARE,gamept->board,bit_offset,piece);
}
