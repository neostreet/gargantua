/*** Gargantua function declarations ***/

int read_binary_game(char *filename,struct game *gamept);
int write_binary_game(char *filename,struct game *gamept);
char xlate_piece(char);
int get_word(FILE *fptr,char *word,int maxlen,int *wordlenpt);
int get_draw_input(struct game *gamept);
int get_xstart(struct game *gamept,int board_offset);
int get_ystart(struct game *gamept,int board_offset);
int get_color(int);
void put_square(struct game *gamept,int what,int where);
void update_move_number(struct game *gamept);

int do_castle(struct game *gamept,int direction,char *word,int wordlen);
int do_pawn_move(struct game *gamept,int direction,char *word,int wordlen);
int do_pawn_move2(struct game *gamept);
int get_piece_id_ix(char piece);
int do_piece_move(struct game *gamept,int direction,char *word,int wordlen);
int do_piece_move2(struct game *gamept);
int allow_user_moves(struct game *gamept);

int rook_move(struct game *,int,int,int,int);
int rook_move2(struct game *);
int knight_move(struct game *,int,int,int,int);
int knight_move2(struct game *);
int bishop_move(struct game *,int,int,int,int);
int bishop_move2(struct game *);
int queen_move(struct game *,int,int,int,int);
int queen_move2(struct game *);
int king_move(struct game *,int,int,int,int);
int king_move2(struct game *);
int gargantua_move(struct game *,int,int,int,int);
int gargantua_move2(struct game *);

int get_to_position(char *word,int wordlen,int *to_filept,int *to_rankpt);

void set_initial_board(struct game *gamept);
void update_board(struct game *gamept);
int get_piece1(struct game *gamept,int board_offset);
int get_piece2(struct game *gamept,int row,int column);
void set_piece1(struct game *gamept,int board_offset,int piece);
void set_piece2(struct game *gamept,int row,int column,int piece);

void print_bd(struct game *gamept);
void fprint_game(struct game *gamept,char *filename);
void fprint_game2(struct game *gamept,FILE *fptr);
void fprint_bd(struct game *gamept,char *filename);
void fprint_bd2(struct game *gamept,FILE *fptr);

void print_game(struct game *gamept);
void fprintf_move(FILE *fptr,struct game *gamept);
void sprintf_move(struct game *gamept,char *buf,int buf_len);
