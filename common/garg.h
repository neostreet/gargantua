#define WHITE 0
#define BLACK 1
#define NUM_PLAYERS 2

#define NUM_RANKS  8
#define NUM_FILES  8

#define NUM_BOARD_SQUARES (NUM_RANKS * NUM_FILES)

#define CHARS_IN_BOARD \
(NUM_BOARD_SQUARES / 2)  // 80 squares / 2 (nibbles per char)

#define PAWN_ID           1
#define ROOK_ID           2
#define KNIGHT_ID         3
#define BISHOP_ID         4
#define GARGANTUA_ID      5
#define KING_ID           6
#define NUM_PIECE_TYPES_0 6
#define EMPTY_ID          7

#define NUM_PIECES_PER_PLAYER 20

#define SPECIAL_MOVE_NONE                      0x0000
#define SPECIAL_MOVE_TWO_SQUARE_PAWN_ADVANCE   0x0001
#define SPECIAL_MOVE_KINGSIDE_CASTLE           0x0002
#define SPECIAL_MOVE_GARGSIDE_CASTLE           0x0004
#define SPECIAL_MOVE_CHECK                     0x0008
#define SPECIAL_MOVE_CAPTURE                   0x0010
#define SPECIAL_MOVE_EN_PASSANT_CAPTURE        0x0020
#define SPECIAL_MOVE_PROMOTION_GARGANTUA       0x0040
#define SPECIAL_MOVE_PROMOTION_ROOK            0x0080
#define SPECIAL_MOVE_PROMOTION_KNIGHT          0x0100
#define SPECIAL_MOVE_PROMOTION_BISHOP          0x0200
#define SPECIAL_MOVE_MATE                      0x0400
#define SPECIAL_MOVE_STALEMATE                 0x0800

#define WORDLEN 80
#define MAX_MOVES 400
#define MAX_LEGAL_MOVES 500

#define WIDTH_IN_PIXELS 50
#define HEIGHT_IN_PIXELS 50

#define SHRUNK_WIDTH_IN_PIXELS (WIDTH_IN_PIXELS / 2)
#define SHRUNK_HEIGHT_IN_PIXELS (HEIGHT_IN_PIXELS / 2)

#define BOARD_WIDTH (NUM_FILES * width_in_pixels)
#define BOARD_HEIGHT (NUM_RANKS * height_in_pixels)

#define NUM_PIECE_TYPES 6

struct move {
  char from;
  char to;
  int special_move_info;
};

struct piece_info {
  char piece_id;
  char current_board_position;
  char move_count;
};

#define BITS_PER_BOARD_SQUARE 4

#define FONT_HEIGHT 12
#define FONT_WIDTH 9

#define Y_PIXELS 200

#define MAX_ANNOTATION_LINE_LEN 25
#define MAX_ANNOTATION_LINES (Y_PIXELS / FONT_HEIGHT)

#define ANNOTATION_X (8 * XLEN + 2 + FONT_WIDTH)
#define ANNOTATION_Y 5

#define MAX_FILE_NAME_LEN 32
#define MAX_TITLE_LEN 128

struct game {
  char title[MAX_TITLE_LEN];
  int orientation;
  int num_moves;
  int curr_move;
  struct move moves[MAX_MOVES];
  unsigned char board[CHARS_IN_BOARD];  /* 10 columns * 8 rows / 2 (nibbles per char) */
  struct piece_info white_pieces[NUM_PIECES_PER_PLAYER];
  struct piece_info black_pieces[NUM_PIECES_PER_PLAYER];
};

struct move_offset {
  char rank_offset;
  char file_offset;
};

typedef char (*GARG_FILE_LIST)[MAX_FILE_NAME_LEN];
