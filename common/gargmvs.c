#include <stdio.h>
#include <string.h>

#include "garg.h"
#include "garg.glb"
#include "garg.mac"
#include "garg.fun"

int do_pawn_move(struct game *gamept)
{
  bool bWhiteMove;
  int start_rank;
  int start_file;
  int end_rank;
  int end_file;
  int rank_diff;
  int file_diff;
  int retval;

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

  if (rank_diff > 2)
    return 6; // failure

  if (rank_diff > 1) {
    if (file_diff)
      return 7; // failure

    if (bWhiteMove) {
      if (start_rank != 1)
        return 8; // failure
    }
    else {
      if (start_rank != 6)
        return 9; // failure
    }
  }

  if (file_diff == 1) {
    if (rank_diff != 1)
      return 10; // failure

    if (!gamept->move_end_square_piece) {
      // check for en passant capture
      if (bWhiteMove && (start_rank == 4) &&
        (get_piece2(gamept,4,end_file) == PAWN_ID * -1) &&
        (gamept->moves[gamept->curr_move-1].special_move_info == SPECIAL_MOVE_TWO_SQUARE_PAWN_ADVANCE)) {

        gamept->moves[gamept->curr_move].special_move_info = SPECIAL_MOVE_EN_PASSANT_CAPTURE;
      }
      else if (!bWhiteMove && (start_rank == 3) &&
        (get_piece2(gamept,3,end_file) == PAWN_ID) &&
        (gamept->moves[gamept->curr_move-1].special_move_info == SPECIAL_MOVE_TWO_SQUARE_PAWN_ADVANCE)) {

        gamept->moves[gamept->curr_move].special_move_info = SPECIAL_MOVE_EN_PASSANT_CAPTURE;
      }
      else
        return 11; // failure
    }
  }

  gamept->moves[gamept->curr_move].from = gamept->move_start_square;
  gamept->moves[gamept->curr_move].to = gamept->move_end_square;
  retval = 0;

  if (rank_diff > 1)
    gamept->moves[gamept->curr_move].special_move_info = SPECIAL_MOVE_TWO_SQUARE_PAWN_ADVANCE;

  return retval; // success
}

char piece_ids[] = "RNBQKG";

int (*piece_functions[])(struct game *) = {
  rook_move2,
  knight_move2,
  bishop_move2,
  queen_move2,
  king_move2,
  gargantua_move2
};

int do_piece_move(struct game *gamept)
{
  int which_piece;
  int retval;

  which_piece = gamept->move_start_square_piece;

  if (which_piece < 0)
    which_piece *= -1;

  which_piece -= ROOK_ID;

  retval = (*piece_functions[which_piece])(gamept);

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

  // first, check if this is a castling move
  if (!(gamept->curr_move % 2)) {
    // it's White's move

    // check for kingside castle
    if ((file1 == 5) && (rank1 == 1) && (file2 == 7) && (rank2 == 1) &&
      !get_piece1(gamept,16) && !get_piece1(gamept,17) && (get_piece1(gamept,18) == ROOK_ID)) {
      gamept->moves[gamept->curr_move].special_move_info = SPECIAL_MOVE_KINGSIDE_CASTLE;
      return 0;
    }

    // check for queenside castle
    if ((file1 == 5) && (rank1 == 1) && (file2 == 3) && (rank2 == 1) &&
      !get_piece1(gamept,12) && !get_piece1(gamept,13) && !get_piece1(gamept,14) && (get_piece1(gamept,11) == ROOK_ID)) {
      gamept->moves[gamept->curr_move].special_move_info = SPECIAL_MOVE_QUEENSIDE_CASTLE;
      return 0;
    }
  }
  else {
    // it's Black's move

    // check for kingside castle
    if ((file1 == 5) && (rank1 == 8) && (file2 == 7) && (rank2 == 8)) {
      gamept->moves[gamept->curr_move].special_move_info = SPECIAL_MOVE_KINGSIDE_CASTLE;
      return 0;
    }

    // check for queenside castle
    if ((file1 == 5) && (rank1 == 8) && (file2 == 3) && (rank2 == 8)) {
      gamept->moves[gamept->curr_move].special_move_info = SPECIAL_MOVE_QUEENSIDE_CASTLE;
      return 0;
    }
  }

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
  // don't allow gargantuas to capture each other

  if (!(get_piece2(gamept,rank1,file1) + get_piece2(gamept,rank2,file2)))
    return 1;  /* failure */

  if (!queen_move(gamept,file1,rank1,file2,rank2))
    return 0;  /* success */

  if (!knight_move(gamept,file1,rank1,file2,rank2))
    return 0;  /* success */

  return 2;    /* failure */
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
