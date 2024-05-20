#include <stdio.h>
#include <stdlib.h>
#include "board.h"

long whiteRooksBB = 0x8100000000000000;
long whiteKnightsBB = 0x4200000000000000; 
long whiteBishopsBB = 0x2400000000000000; 
long whiteQueenBB = 0x0800000000000000;
long whiteKingBB = 0x1000000000000000;
long whitePawnsBB = 0x00FF000000000000;

long blackPawnsBB = 0x000000000000FF00;
long blackRooksBB = 0x0000000000000081;
long blackKnightsBB = 0x0000000000000042; 
long blackBishopsBB = 0x0000000000000024; 
long blackQueenBB = 0x0000000000000008;
long blackKingBB = 0x0000000000000010;


int main(void){

    U64 board = 0UL;    
    init_pieces_attacks();
    print_bitboard(rook_attack_table[E5]);


    return 0;
}