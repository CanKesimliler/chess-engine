#include <stdio.h>
#define U64 unsigned long
#define WHITE 0
#define BLACK 1


/*External variables*/
extern U64 pawn_attack_table[2][64];
extern U64 knight_attack_table[64];
extern U64 rook_attack_table[64];
extern U64 bishop_attack_table[64];
extern U64 knight_attack_table[64];
extern U64 king_attack_table[64];

/*Bitwise macros*/
#define GET_BIT(bitboard, square) (bitboard & (1ULL << square)) 
#define SQUARE(rank,file) rank*8+file
#define SET_BIT(board, square) board |= (1ULL << square)
#define SET_BIT_NUM(board, square) (board | (1ULL << square))

#define REMOVE_BIT(board, square) board &= ~(1ULL << square)

/*Enum for chess squares*/
enum ChessSquare {
    A1 = SQUARE(7, 0), B1, C1, D1, E1, F1, G1, H1,
    A2 = SQUARE(6, 0), B2, C2, D2, E2, F2, G2, H2,
    A3 = SQUARE(5, 0), B3, C3, D3, E3, F3, G3, H3,
    A4 = SQUARE(4, 0), B4, C4, D4, E4, F4, G4, H4,
    A5 = SQUARE(3, 0), B5, C5, D5, E5, F5, G5, H5,
    A6 = SQUARE(2, 0), B6, C6, D6, E6, F6, G6, H6,
    A7 = SQUARE(1, 0), B7, C7, D7, E7, F7, G7, H7,
    A8 = SQUARE(0, 0), B8, C8, D8, E8, F8, G8, H8
};

/*Not A file*/
#define NOT_A_FILE 0xFEFEFEFEFEFEFEFE

/*  A B C D E F G H
   -----------------
8- |0|1|1|1|1|1|1|1|
   -----------------
7- |0|1|1|1|1|1|1|1|
   -----------------
6- |0|1|1|1|1|1|1|1|
   -----------------
5- |0|1|1|1|1|1|1|1|
   -----------------
4- |0|1|1|1|1|1|1|1|
   -----------------
3- |0|1|1|1|1|1|1|1|
   -----------------
2- |0|1|1|1|1|1|1|1|
   -----------------
1- |0|1|1|1|1|1|1|1|
   -----------------
Bitboard: 18374403900871474942d*/


/*Not H file*/
#define NOT_H_FILE 0x7F7F7F7F7F7F7F7F

/*  A B C D E F G H
   -----------------
8- |1|1|1|1|1|1|1|0|
   -----------------
7- |1|1|1|1|1|1|1|0|
   -----------------
6- |1|1|1|1|1|1|1|0|
   -----------------
5- |1|1|1|1|1|1|1|0|
   -----------------
4- |1|1|1|1|1|1|1|0|
   -----------------
3- |1|1|1|1|1|1|1|0|
   -----------------
2- |1|1|1|1|1|1|1|0|
   -----------------
1- |1|1|1|1|1|1|1|0|
   -----------------
Bitboard: 9187201950435737471d*/

/*Not AB file*/
#define NOT_AB_FILE 15923880545981627644UL

/*  A B C D E F G H
   -----------------
8- |0|0|1|1|1|1|1|1|
   -----------------
7- |0|0|1|1|1|1|1|1|
   -----------------
6- |0|0|1|1|1|1|1|1|
   -----------------
5- |0|0|1|1|1|1|1|1|
   -----------------
4- |0|0|1|1|1|1|1|1|
   -----------------
3- |0|0|1|1|1|1|1|1|
   -----------------
2- |0|0|1|1|1|1|1|1|
   -----------------
1- |0|0|1|1|1|0|1|1|
   -----------------
Bitboard: 15923880545981627644d*/

/*Not GH File*/
#define NOT_GH_FILE 4557430888798830399UL

/*  A B C D E F G H
   -----------------
8- |1|1|1|1|1|1|0|0|
   -----------------
7- |1|1|1|1|1|1|0|0|
   -----------------
6- |1|1|1|1|1|1|0|0|
   -----------------
5- |1|1|1|1|1|1|0|0|
   -----------------
4- |1|1|1|1|1|1|0|0|
   -----------------
3- |1|1|1|1|1|1|0|0|
   -----------------
2- |1|1|1|1|1|1|0|0|
   -----------------
1- |1|1|1|1|1|1|0|0|
   -----------------
Bitboard: 4557430888798830399d*/

U64 pawn_attacks(int square, int color); 
U64 knight_attacks(int square);
U64 rook_attacks(int square);
void print_bitboard(U64 bitboard);
void init_pieces_attacks();
