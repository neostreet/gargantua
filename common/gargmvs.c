#include <stdio.h>
#include <string.h>

#include "garg.h"
#include "garg.glb"
#include "garg.mac"
#include "garg.fun"

int do_castle(struct game *gamept,int direction,char *word,int wordlen)
{
  int rank;

  if (direction == 1)  /* if white's move: */
    rank = 0;
  else
    rank = 7;

  /* make sure the king is on his original square: */
  if (get_piece2(gamept,rank,4) != 6 * direction)
    return 1;

  if (wordlen == 3) {  /* kingside castle */
    /* make sure there is a rook in the corner: */
    if (get_piece2(gamept,rank,7) != 2 * direction)
      return 2;

    /* make sure there are empty squares between king and rook: */
    if (get_piece2(gamept,rank,5) || get_piece2(gamept,rank,6))
      return 3;

    gamept->moves[gamept->curr_move].from = SPECIAL_MOVE_KINGSIDE_CASTLE;
  }
  else if (wordlen == 5) {  /* queenside castle */
    /* make sure there is a rook in the corner: */
    if (get_piece2(gamept,rank,0) != 2 * direction)
      return 4;

    /* make sure there are empty squares between king and rook: */
    if (get_piece2(gamept,rank,1) || get_piece2(gamept,rank,2) || get_piece2(gamept,rank,3))
      return 5;

    gamept->moves[gamept->curr_move].from = SPECIAL_MOVE_QUEENSIDE_CASTLE;
  }
  else
    return 6;

  return 0;  /* success */
}

int do_pawn_move2(struct game *gamept)
{
  bool bWhiteMove;
  int start_rank;
  int start_file;
  int end_rank;
  int end_file;
  int rank_diff;
  int file_diff;

  bWhiteMove = (gamept->move_start_square_piece > 0);

  if (bWhiteMove) {
    // white pawn move

    if (gamept->move_start_square > gamept->move_end_square)
      return 1; // failure
  }
  else {
    // black pawn move

    if (gamept->move_start_square < gamept->move_end_square)
      return 2; // failure
  }

  start_rank = RANK_OF(gamept->move_start_square);
  start_file = FILE_OF(gamept->move_start_square);
  end_rank = RANK_OF(gamept->move_end_square);
  end_file = FILE_OF(gamept->move_end_square);

  if (start_rank >= end_rank)
    rank_diff = start_rank - end_rank;
  else
    rank_diff = end_rank - start_rank;

  if (start_file >= end_file)
    file_diff = start_file - end_file;
  else
    file_diff = end_file - start_file;

  if (file_diff == 0) {
    if (gamept->move_end_square_piece)
      return 3; // failure
  }

  if (rank_diff == 0)
    return 4; // failure

  if (file_diff > 1)
    return 5; // failure

  if (rank_diff > 3)
    return 6; // failure

  if (rank_diff > 1) {
    if (file_diff)
      return 7; // failure

    if (bWhiteMove) {
      if (start_rank != 1)
        return 8; // failure
    }
    else {
      if (start_rank != 8)
        return 9; // failure
    }
  }

  if (file_diff == 1) {
    if (rank_diff != 1)
      return 10; // failure

    if (!gamept->move_end_square_piece)
      return 11; // failure
  }

  gamept->moves[gamept->curr_move].from = gamept->move_start_square;
  gamept->moves[gamept->curr_move].to = gamept->move_end_square;

  return 0; // success
}

char piece_ids[] = "RNBQKG";

int get_piece_id_ix(char piece)
{
  int which_piece;

  /* calculate the id to search for: */
  for (which_piece = 0; which_piece < NUM_PIECE_TYPES; which_piece++) {
    if (piece_ids[which_piece] == piece)
      break;
  }

  return which_piece;
}

int (*piece_functions[])(struct game *,int,int,int,int) = {
  rook_move,
  knight_move,
  bishop_move,
  queen_move,
  king_move,
  gargantua_move
};

int (*piece_functions2[])(struct game *) = {
  rook_move2,
  knight_move2,
  bishop_move2,
  queen_move2,
  king_move2,
  gargantua_move2
};

int do_piece_move2(struct game *gamept)
{
  int which_piece;
  int retval;

  which_piece = gamept->move_start_square_piece;

  if (which_piece < 0)
    which_piece *= -1;

  which_piece -= ROOK_ID;

  retval = (*piece_functions2[which_piece])(gamept);

  if (!retval) {
    gamept->moves[gamept->curr_move].from = gamept->move_start_square;
    gamept->moves[gamept->curr_move].to = gamept->move_end_square;
    return 0;  /* success */
  }

  return 1;
}

int get_to_position(char *word,int wordlen,int *to_filept,int *to_rankpt)
{
  *to_filept = word[wordlen - 2] - 'a';

  if ((*to_filept < 0) || (*to_filept > 7))
    return FALSE;

  *to_rankpt = word[wordlen - 1] - '1';

  if ((*to_rankpt < 0) || (*to_rankpt > 7))
    return FALSE;

  return TRUE;
}

int rook_move(
  struct game *gamept,
  int file1,
  int rank1,
  int file2,
  int rank2
)
{
  int n;

  if (file1 == file2) {
    if (rank1 > rank2) {
      for (n = rank2 + 1; n < rank1; n++)
        if (get_piece2(gamept,n,file1))
          return 1;  /* failure */
    }
    else
      for (n = rank1 + 1; n < rank2; n++)
        if (get_piece2(gamept,n,file1))
          return 2;  /* failure */

    return 0;  /* success */
  }

  if (rank1 == rank2) {
    if (file1 > file2) {
      for (n = file2 + 1; n < file1; n++)
        if (get_piece2(gamept,rank1,n))
          return 1;  /* failure */
    }
    else
      for (n = file1 + 1; n < file2; n++)
        if (get_piece2(gamept,rank1,n))
          return 2;  /* failure */

    return 0;  /* success */
  }

  return 3;  /* failure */
}

int rook_move2(
  struct game *gamept
)
{
  int retval;

  retval = rook_move(
    gamept,
    FILE_OF(gamept->move_start_square),
    RANK_OF(gamept->move_start_square),
    FILE_OF(gamept->move_end_square),
    RANK_OF(gamept->move_end_square)
    );

  return retval;
}

int knight_move(
  struct game *gamept,
  int file1,
  int rank1,
  int file2,
  int rank2
)
{
  int dist1;
  int dist2;

  dist1 = (file1 - file2);

  if (dist1 < 0)
    dist1 *= -1;

  dist2 = (rank1 - rank2);

  if (dist2 < 0)
    dist2 *= -1;

  if ((dist1 == 1) && (dist2 == 2))
    return 0;  /* success */

  if ((dist1 == 2) && (dist2 == 1))
    return 0;  /* success */

  return 1;    /* failure */
}

int knight_move2(
  struct game *gamept
)
{
  int retval;

  retval = knight_move(
    gamept,
    FILE_OF(gamept->move_start_square),
    RANK_OF(gamept->move_start_square),
    FILE_OF(gamept->move_end_square),
    RANK_OF(gamept->move_end_square)
    );

  return retval;
}

int bishop_move(
  struct game *gamept,
  int file1,
  int rank1,
  int file2,
  int rank2
)
{
  int dist1;
  int dist2;
  int file_dir;
  int rank_dir;

  dist1 = (file1 - file2);

  if (dist1 < 0) {
    dist1 *= -1;
    file_dir = 1;
  }
  else
    file_dir = -1;

  dist2 = (rank1 - rank2);

  if (dist2 < 0) {
    dist2 *= -1;
    rank_dir = 1;
  }
  else
    rank_dir = -1;

  if (dist1 != dist2)
    return 1;  /* failure */

  /* make sure there are no intervening pieces */
  for ( ; ; ) {
    file1 += file_dir;

    if (file1 == file2)
      break;

    rank1 += rank_dir;

    if (get_piece2(gamept,rank1,file1))
      return 2;  /* failure */
  }

  return 0;  /* success */
}

int bishop_move2(
  struct game *gamept
)
{
  int retval;

  retval = bishop_move(
    gamept,
    FILE_OF(gamept->move_start_square),
    RANK_OF(gamept->move_start_square),
    FILE_OF(gamept->move_end_square),
    RANK_OF(gamept->move_end_square)
    );

  return retval;
}

int queen_move(
  struct game *gamept,
  int file1,
  int rank1,
  int file2,
  int rank2
)
{
  if (!rook_move(gamept,file1,rank1,file2,rank2))
    return 0;  /* success */

  if (!bishop_move(gamept,file1,rank1,file2,rank2))
    return 0;  /* success */

  return 1;    /* failure */
}

int queen_move2(
  struct game *gamept
)
{
  int retval;

  retval = queen_move(
    gamept,
    FILE_OF(gamept->move_start_square),
    RANK_OF(gamept->move_start_square),
    FILE_OF(gamept->move_end_square),
    RANK_OF(gamept->move_end_square)
    );

  return retval;
}

int king_move(
  struct game *gamept,
  int file1,
  int rank1,
  int file2,
  int rank2
)
{
  int dist1;
  int dist2;

  dist1 = (file1 - file2);

  if (dist1 < 0)
    dist1 *= -1;

  dist2 = (rank1 - rank2);

  if (dist2 < 0)
    dist2 *= -1;

  if ((dist1 < 2) && (dist2 < 2))
    return 0;  /* success */

  return 1;  /* failure */
}

int king_move2(
  struct game *gamept
)
{
  int retval;

  retval = king_move(
    gamept,
    FILE_OF(gamept->move_start_square),
    RANK_OF(gamept->move_start_square),
    FILE_OF(gamept->move_end_square),
    RANK_OF(gamept->move_end_square)
    );

  return retval;
}

int gargantua_move(
  struct game *gamept,
  int file1,
  int rank1,
  int file2,
  int rank2
)
{
  int dist1;
  int dist2;

  dist1 = (file1 - file2);

  if (dist1 < 0)
    dist1 *= -1;

  dist2 = (rank1 - rank2);

  if (dist2 < 0)
    dist2 *= -1;

  if ((dist1 == 1) && (dist2 == 2))
    return 0;  /* success */

  if ((dist1 == 2) && (dist2 == 1))
    return 0;  /* success */

  if ((dist1 == 2) && (dist2 == 3))
    return 0;  /* success */

  if ((dist1 == 3) && (dist2 == 2))
    return 0;  /* success */

  return 1;    /* failure */
}

int gargantua_move2(
  struct game *gamept
)
{
  int retval;

  retval = gargantua_move(
    gamept,
    FILE_OF(gamept->move_start_square),
    RANK_OF(gamept->move_start_square),
    FILE_OF(gamept->move_end_square),
    RANK_OF(gamept->move_end_square)
    );

  return retval;
}
