#include <stdio.h>
#include <stdlib.h>
#include "engine.h"
#include "parser.h"


int main(void){
    restart_game();
    parse_fen_engine(&game, "n1n5/1Pk5/8/8/8/8/5Kp1/5N1N w - - 0 1");
    // 65535d
    //18446462598732840960d
    init_pieces_attacks();
    init_slider_attacks();

    /*get initial time*/
    int start = get_time_ms();

    //perft
    perft_driver(2);

    Move moves = {{0}, 0};
    //GenerateMoves(&moves);
    //printMoveList(&moves);




    
    //make_move(ENCODE_MOVE(D5, D6, wP, 0, 0, 0, 0, 0));
    //printf("attacked: %d",is_square_attacked(E1, BLACK_P, game.bitboards));


    printf("Time taken: %d ms\n", get_time_ms() - start);
    printf("Nodes: %ld\n", nodes);
    return 0;
}