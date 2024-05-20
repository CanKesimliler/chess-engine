#include "board.h"

U64 pawn_attack_table[2][64];
U64 rook_attack_table[64];
U64 bishop_attack_table[64];
U64 knight_attack_table[64];
U64 king_attack_table[64];

void print_bitboard(U64 bitboard){
    int rank = 0;
    int file = 0;
    int square = 0;
    printf("    A B C D E F G H\n");
    for(;rank<8;rank++){
        printf("   -----------------\n");
        printf("%d- ", 8-rank);
        for(file = 0;file<8;file++){
            square = rank*8+file;
            printf("|%d", GET_BIT(bitboard, square) ? 1:0);
        }
        printf("|\n");
    }
    printf("   -----------------\n");
    printf("Bitboard: %lud\n",bitboard);
}

U64 pawn_attacks(int square, int color){
    U64 attacks = 0ULL;
    if(color == WHITE){
        if((SET_BIT_NUM(0UL, square) >> 7) & NOT_A_FILE) attacks |= (SET_BIT_NUM(0UL, square) >> 7);
        else if((SET_BIT_NUM(0UL, square) >> 9) & NOT_H_FILE) attacks |= (SET_BIT_NUM(0UL, square) >> 9);
    }
    else{
        if((SET_BIT_NUM(0UL, square) << 7) & NOT_H_FILE) attacks |= (SET_BIT_NUM(0UL, square) << 7);
        else if((SET_BIT_NUM(0UL, square) << 9) & NOT_A_FILE) attacks |= (SET_BIT_NUM(0UL, square) << 9);
    }
    return attacks;
}

U64 knight_attacks(int square){
    
    /*Result attack bitboards*/
    U64 attacks = 0UL;

    /*Generate knight attacks 17 15 10 6*/
    if((SET_BIT_NUM(0UL, square) >> 17 & NOT_H_FILE)) attacks |= (SET_BIT_NUM(0UL, square) >> 17);
    if((SET_BIT_NUM(0UL, square) >> 15 & NOT_A_FILE)) attacks |= (SET_BIT_NUM(0UL, square) >> 15);
    if((SET_BIT_NUM(0UL, square) >> 10 & NOT_GH_FILE)) attacks |= (SET_BIT_NUM(0UL, square) >> 10);
    if((SET_BIT_NUM(0UL, square) >> 6 & NOT_AB_FILE)) attacks |= (SET_BIT_NUM(0UL, square) >> 6);
    if((SET_BIT_NUM(0UL, square) << 17 & NOT_H_FILE)) attacks |= (SET_BIT_NUM(0UL, square) << 17);
    if((SET_BIT_NUM(0UL, square) << 15 & NOT_A_FILE)) attacks |= (SET_BIT_NUM(0UL, square) << 15);
    if((SET_BIT_NUM(0UL, square) << 10 & NOT_AB_FILE)) attacks |= (SET_BIT_NUM(0UL, square) << 10);
    if((SET_BIT_NUM(0UL, square) << 6 & NOT_GH_FILE)) attacks |= (SET_BIT_NUM(0UL, square) << 6);

    return attacks;
}

U64 rook_attacks(int square){
    U64 attacks = 0ULL;
    int i = 0;
    for(;i < 8;i++){
        attacks |= (1ULL << (square-(square%8)+i));
    }
    for(i=0;i<8;i++){
        attacks |= (1ULL << (i*8+square%8));
    } 
    return REMOVE_BIT(attacks,square);
}

void init_pieces_attacks(){
    int i = 0;
    for(;i < 64; i++){
        pawn_attack_table[WHITE][i] = pawn_attacks(i, WHITE);
        pawn_attack_table[BLACK][i] = pawn_attacks(i, BLACK);

        rook_attack_table[i] = rook_attacks(i);

        knight_attack_table[i] = knight_attacks(i);
    }
}

