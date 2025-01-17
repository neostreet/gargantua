/*** Gargantua function declarations ***/

int read_game(char *filename,struct game *gamept,char *err_msg);
int read_binary_game(char *filename,struct game *gamept);
int read_game_position(char *filename,struct game_position *position_pt);
int write_binary_game(char *filename,struct game *gamept);
int write_game_position(char *filename,struct game_position *position_pt);
char xlate_piece(char);
int get_word(FILE *fptr,char *word,int maxlen,int *wordlenpt);
int get_draw_input(struct game *gamept);
int get_xstart(struct game *gamept,int board_offset);
int get_ystart(struct game *gamept,int board_offset);
int get_color(int);
void put_square(struct game *gamept,int what,int where);
void update_move_number(struct game *gamept);
void copy_game(struct game *gamept_to,struct game *gamept_from);
void GetLine(FILE *fptr,char *line,int *line_len,int maxllen);
int populate_board_from_board_file(unsigned char *board,char *filename,int orientation);
int write_board_to_binfile(unsigned char *board,char *filename);
void calculate_seirawan_counts(struct game *gamept);

int do_castle(struct game *gamept,int direction,char *word,int wordlen,struct move *move_ptr);
int do_pawn_move(struct game *gamept,int direction,char *word,int wordlen,struct move *move_ptr);
int do_pawn_move2(struct game *gamept);
int get_piece_id_ix(char piece);
int do_piece_move(struct game *gamept,int direction,char *word,int wordlen,struct move *move_ptr);
int do_piece_move2(struct game *gamept);
int allow_user_moves(struct game *gamept);

int rook_move(struct game *,int,int,int,int);
int rook_move2(struct game *);
int knight_move(struct game *,int,int,int,int);
int knight_move2(struct game *);
int bishop_move(struct game *,int,int,int,int);
int bishop_move2(struct game *);
int gargantua_move(struct game *,int,int,int,int);
int gargantua_move2(struct game *);
int king_move(struct game *,int,int,int,int);
int king_move2(struct game *);

bool move_is_legal(struct game *gamept,char from,char to);
void get_legal_moves(struct game *gamept,struct move *legal_moves,int *legal_moves_count);
void legal_pawn_moves(struct game *gamept,char current_board_position,struct move *legal_moves,int *legal_moves_count);
void legal_rook_moves(struct game *gamept,char current_board_position,struct move *legal_moves,int *legal_moves_count);
void legal_knight_moves(struct game *gamept,char current_board_position,struct move *legal_moves,int *legal_moves_count);
void legal_bishop_moves(struct game *gamept,char current_board_position,struct move *legal_moves,int *legal_moves_count);
void legal_gargantua_moves(struct game *gamept,char current_board_position,struct move *legal_moves,int *legal_moves_count);
void legal_king_moves(struct game *gamept,char current_board_position,struct move *legal_moves,int *legal_moves_count);
bool mate_in_one_exists(struct game *gamept);
bool white_to_move(struct game *gamept);

int get_to_position(char *word,int wordlen,int *to_filept,int *to_rankpt);

void set_initial_board(struct game *gamept);
void position_game(struct game *gamept,int move);
void print_special_moves(struct game *gamept);
void update_board(struct game *gamept,int *invalid_squares,int *num_invalid_squares,bool bScratch);
void update_piece_info(struct game *gamept);
void fprint_piece_info(struct game *gamept,FILE *fptr);
void print_piece_info(struct game *gamept);
void print_piece_info2(struct piece_info *info_pt,bool bWhite,bool bAbbrev,bool bOnlyRemaining);
void populate_board_from_piece_info(struct piece_info *white_pt,struct piece_info *black_pt,unsigned char *board);
int populate_piece_info_from_board(unsigned char *board,struct piece_info *white_pt,struct piece_info *black_pt);
int compare_boards(unsigned char *board1,unsigned char *board2);
int get_piece1(unsigned char *board,int board_offset);
int get_piece2(unsigned char *board,int rank,int file);
void set_piece1(unsigned char *board,int board_offset,int piece);
void set_piece2(unsigned char *board,int rank,int file,int piece);
void copy_board(unsigned char *from_board,unsigned char *to_board);

int format_square(int square);
void print_bd0(unsigned char *board,int orientation);
void print_bd(struct game *gamept);
void print_bd_cropped(struct game *gamept);
void fprint_game(struct game *gamept,char *filename);
void fprint_game2(struct game *gamept,FILE *fptr);
void fprint_bd(struct game *gamept,char *filename);
void fprint_bd2(unsigned char *board,FILE *fptr);
void fprint_bd3(unsigned char *board,int orientation,FILE *fptr);
void print_moves(struct move *moves,int num_moves,bool bHex,bool bMoveNumbers);
void fprint_moves(struct move *moves,int num_moves,char *filename);
void fprint_moves2(struct move *moves,int num_moves,FILE *fptr);

void print_game(struct game *gamept);
void fprintf_move(FILE *fptr,struct game *gamept);
void sprintf_move(struct game *gamept,char *buf,int buf_len,bool bInline);

int square_attacks_square(unsigned char *board,int square1,int square2);
int pawn_attacks_square(unsigned char *board,int square1,int color,int square2);
int rook_attacks_square(unsigned char *board,int square1,int square2);
int knight_attacks_square(unsigned char *board,int square1,int square2);
int bishop_attacks_square(unsigned char *board,int square1,int square2);
int gargantua_attacks_square(unsigned char *board,int square1,int square2);
int king_attacks_square(unsigned char *board,int square1,int square2);
bool any_opponent_piece_attacks_square(int square,bool bBlack,unsigned char *board,int curr_move);
bool player_is_in_check(bool bBlack,unsigned char *board,int curr_move);
bool garg_is_attacked(bool bBlack,unsigned char *board,int curr_move);
int calc_square(char *algebraic_notation);
