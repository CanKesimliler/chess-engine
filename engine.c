#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "parser.h"

/*Declaring bitboards for all pieces*/
U64 bitboards[15];

/*Declaring necessery flags*/
int side = -1; /*Side to move*/
int enpassant = NO_SQ; /*Enpassant square*/
int castle = 0b1111; /*Castle rights encoded bitwise*/ 
/*1000 for white castle king*/
/*0100 for white castle queen*/
/*0010 for black castle king*/
/*0001 for black castle queen*/
int half_moves = 0; /*Half moves counter*/
int full_moves = 0; /*Full moves counter*/


int main(void){
    
    // 65535d
    // 18446462598732840960d
    return 0;
}