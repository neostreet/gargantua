#include <stdio.h>
#include "garg.h"
#include "garg.glb"
#include "garg.fun"
#include "garg.mac"
#include "bitfuns.h"

int format_square(int square)
{
  bool bBlack;
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

void print_bd0(unsigned char *board,int orientation)
{
  int m;
  int n;
  int square;

  for (m = 0; m < NUM_RANKS; m++) {
    for (n = 0; n < NUM_FILES; n++) {
      if (!orientation)
        square = get_piece2(board,(NUM_RANKS - 1) - m,n);
      else
        square = get_piece2(board,m,(NUM_FILES - 1) - n);

      printf("%c",format_square(square));

      if (n < (NUM_FILES - 1))
        putchar(' ');
    }

    putchar(0x0a);
  }
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

void print_bd_cropped(struct game *gamept)
{
  int m;
  int n;
  int square;

  for (m = 0; m < NUM_RANKS; m++) {
    for (n = 1; n < NUM_FILES - 1; n++) {
      if (!gamept->orientation)
        square = get_piece2(gamept->board,(NUM_RANKS - 1) - m,n);
      else
        square = get_piece2(gamept->board,m,(NUM_FILES - 1) - n);

      printf("%c",format_square(square));

      if (n < (NUM_FILES - 2))
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

    update_board(gamept,NULL,NULL,false);
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

    update_board(gamept,NULL,NULL,false);
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

void print_moves(struct game *gamept,bool bHex)
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

    if (bHex) {
      printf("%3d %2d %2d %04x\n",
        n,gamept->moves[n].from,gamept->moves[n].to,gamept->moves[n].special_move_info);
    }
    else {
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

void print_special_moves(struct game *gamept)
{
  int n;
  int and_val;
  int hit;

  and_val = 0x1;
  hit = 0;

  for (n = 0; n < num_special_moves; n++) {
    if (gamept->moves[gamept->curr_move].special_move_info & and_val) {
      if (hit)
        putchar(' ' );

      printf("%s",special_moves[n]);

      hit++;
    }

    and_val <<= 1;
  }

  if (hit)
    putchar(0x0a);
  else
    printf("SPECIAL_MOVE_NONE\n");
}

void position_game(struct game *gamept,int move)
{
  set_initial_board(gamept);

  for (gamept->curr_move = 0; gamept->curr_move < move; gamept->curr_move++) {
    update_board(gamept,NULL,NULL,false);
    update_piece_info(gamept);
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
  int dbg;

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
