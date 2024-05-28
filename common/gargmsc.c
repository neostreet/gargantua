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
  (unsigned char)0xff, (unsigned char)0xff, (unsigned char)0xff, (unsigned char)0xff, (unsigned char)0xff,
  (unsigned char)0x9e, (unsigned char)0xdc, (unsigned char)0xba, (unsigned char)0xcd, (unsigned char)0xe9,
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
    bBlack = true;
    square *= -1;
  }
  else
    bBlack = false;

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
      if (!gamept->orientation)
        square = get_piece2(gamept->board,(NUM_RANKS - 1) - m,n);
      else
        square = get_piece2(gamept->board,m,(NUM_FILES - 1) - n);

      printf("%c",format_square(square));

      if (n < (NUM_FILES - 1))
        putchar(' ');
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

    sprintf_move(gamept,buf,20,true);
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

    sprintf_move(gamept,buf,20,true);
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
      square = get_piece2(gamept->board,(NUM_RANKS - 1) - m,n);
      fprintf(fptr,"%c ",format_square(square));
    }

    fputc(0x0a,fptr);
  }

  fclose(fptr);
}

void fprint_bd2(unsigned char *board,FILE *fptr)
{
  int m;
  int n;
  int square;

  for (m = 0; m < NUM_RANKS; m++) {
    for (n = 0; n < NUM_FILES; n++) {
      square = get_piece2(board,(NUM_RANKS - 1) - m,n);
      fprintf(fptr,"%c ",format_square(square));
    }

    fputc(0x0a,fptr);
  }
}

void print_moves(struct game *gamept)
{
  int m;
  int n;
  int and_val;
  int hit;
  int dbg_move;
  int dbg;

  dbg_move = -1;

  for (n = 0; n < gamept->num_moves; n++) {
    if (n == dbg_move)
      dbg = 1;

    printf("%3d %2d %2d",n,gamept->moves[n].from,gamept->moves[n].to);

    and_val = 0x1;
    hit = 0;

    for (m = 0; m < num_special_moves; m++) {
      if (gamept->moves[n].special_move_info & and_val) {
        hit = 1;
        putchar(' ' );
        printf("%s",special_moves[m]);
      }

      and_val <<= 1;
    }

    if (!hit)
      printf(" SPECIAL_MOVE_NONE");

    putchar(0x0a);
  }

  dbg = 1; // see if you get here
}

void fprint_moves(struct game *gamept,char *filename)
{
  int n;
  FILE *fptr;

  if ((fptr = fopen(filename,"w")) == NULL)
    return;

  for (n = 0; n < gamept->num_moves; n++) {
    fprintf(fptr,"%d %d %d %x\n",n,gamept->moves[n].from,gamept->moves[n].to,gamept->moves[n].special_move_info);
  }

  fclose(fptr);
}

void fprint_moves2(struct game *gamept,FILE *fptr)
{
  int n;

  fprintf(fptr,"fprint_moves2:\n");

  for (n = 0; n < gamept->num_moves; n++) {
    fprintf(fptr,"%d %d %x\n",
      gamept->moves[n].from,
      gamept->moves[n].to,
      gamept->moves[n].special_move_info);
  }
}

void set_initial_board(struct game *gamept)
{
  int n;

  for (n = 0; n < CHARS_IN_BOARD; n++)
    gamept->board[n] = initial_board[n];
}

void position_game(struct game *gamept,int move)
{
  set_initial_board(gamept);

  for (gamept->curr_move = 0; gamept->curr_move < move; gamept->curr_move++) {
    update_board(gamept,NULL,NULL);
  }
}

static int update_board_calls;
static int dbg_update_board_call;
static int dbg;

void update_board(struct game *gamept,int *invalid_squares,int *num_invalid_squares)
{
  bool bBlack;
  int from_piece;
  int to_piece;
  bool bKingsideCastle = false;
  bool bQueensideCastle = false;
  bool bEnPassantCapture = false;
  int square_to_clear;

  update_board_calls++;

  if (dbg_update_board_call == update_board_calls)
    dbg = 0;

  bBlack = (gamept->curr_move % 2);

  from_piece = get_piece1(gamept->board,gamept->moves[gamept->curr_move].from);
  to_piece = get_piece1(gamept->board,gamept->moves[gamept->curr_move].to);

  if (from_piece * to_piece < 0)
    gamept->moves[gamept->curr_move].special_move_info |= SPECIAL_MOVE_CAPTURE;

  switch (gamept->moves[gamept->curr_move].special_move_info) {
    case SPECIAL_MOVE_KINGSIDE_CASTLE:
      bKingsideCastle = true;

      break;
    case SPECIAL_MOVE_QUEENSIDE_CASTLE:
      bQueensideCastle = true;

      break;
    case SPECIAL_MOVE_EN_PASSANT_CAPTURE:
      bEnPassantCapture = true;

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

  set_piece1(gamept->board,gamept->moves[gamept->curr_move].to,from_piece);

  set_piece1(gamept->board,gamept->moves[gamept->curr_move].from,0);  /* vacate previous square */

  if (bKingsideCastle) {
    if (!(gamept->curr_move % 2)) {
      // it's White's move
      set_piece1(gamept->board,6,ROOK_ID);

      if (invalid_squares)
        invalid_squares[(*num_invalid_squares)++] = 6;

      set_piece1(gamept->board,8,0);

      if (invalid_squares)
        invalid_squares[(*num_invalid_squares)++] = 8;
    }
    else {
      // it's Blacks's move
      set_piece1(gamept->board,76,ROOK_ID * -1);

      if (invalid_squares)
        invalid_squares[(*num_invalid_squares)++] = 76;

      set_piece1(gamept->board,78,0);

      if (invalid_squares)
        invalid_squares[(*num_invalid_squares)++] = 78;
    }
  }
  else if (bQueensideCastle) {
    if (!(gamept->curr_move % 2)) {
      // it's White's move
      set_piece1(gamept->board,4,ROOK_ID);

      if (invalid_squares)
        invalid_squares[(*num_invalid_squares)++] = 4;

      set_piece1(gamept->board,1,0);

      if (invalid_squares)
        invalid_squares[(*num_invalid_squares)++] = 1;
    }
    else {
      // it's Blacks's move
      set_piece1(gamept->board,74,ROOK_ID * -1);

      if (invalid_squares)
        invalid_squares[(*num_invalid_squares)++] = 74;

      set_piece1(gamept->board,71,0);

      if (invalid_squares)
        invalid_squares[(*num_invalid_squares)++] = 71;
    }
  }
  else if (bEnPassantCapture) {
    if (!(gamept->curr_move % 2)) {
      // it's White's move
      square_to_clear = gamept->moves[gamept->curr_move].to - NUM_FILES;
    }
    else {
      // it's Blacks's move
      square_to_clear = gamept->moves[gamept->curr_move].to + NUM_FILES;
    }

    set_piece1(gamept->board,square_to_clear,0);

    if (invalid_squares)
      invalid_squares[(*num_invalid_squares)++] = square_to_clear;
  }
}

int get_piece1(unsigned char *board,int board_offset)
{
  unsigned int bit_offset;
  unsigned short piece;
  int piece_int;

  bit_offset = board_offset * BITS_PER_BOARD_SQUARE;

  piece = get_bits(BITS_PER_BOARD_SQUARE,board,bit_offset);
  piece_int = piece;

  if (piece & 0x8)
    piece_int |= 0xfffffff0;

  return piece_int;
}

int get_piece2(unsigned char *board,int rank,int file)
{
  int board_offset;

  board_offset = rank * NUM_FILES + file;
  return get_piece1(board,board_offset);
}

static int set_piece_calls;
static int dbg_set_piece_call;
static int dbg_board_offset;
static int dbg_piece;

void set_piece1(unsigned char *board,int board_offset,int piece)
{
  unsigned int bit_offset;

  set_piece_calls++;

  if (dbg_set_piece_call == set_piece_calls)
    dbg = 0;

  if (debug_level == 2) {
    if (debug_fptr != NULL)
      fprintf(debug_fptr,"set_piece: board_offset = %d, piece %d\n",board_offset,piece);
  }

  bit_offset = board_offset * BITS_PER_BOARD_SQUARE;
  set_bits(BITS_PER_BOARD_SQUARE,board,bit_offset,piece);
}

void set_piece2(unsigned char *board,int rank,int file,int piece)
{
  int board_offset;

  board_offset = rank * NUM_FILES + file;
  set_piece1(board,board_offset,piece);
}
