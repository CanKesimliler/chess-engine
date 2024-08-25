#include <stdio.h>
#include <stdlib.h>
#include "engine.h"
#include "parser.h"


int main(void){
    restart_game();
    parse_fen_engine(&game, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    // 65535d
    //18446462598732840960d
    init_pieces_attacks();
    init_slider_attacks();

    /*get initial time*/
    int start = get_time_ms();

    //perft
    perft_driver(5);
    //print_bitboard(22518006777118720);
    //print_board();
    //Move moves = {{0}, 0};
    //GenerateMoves(&moves);
    //printMoveList(&moves);
    //print_bitboard(queen_attack(A3, game.bitboards[AP]));




    
    //make_move(ENCODE_MOVE(D5, D6, wP, 0, 0, 0, 0, 0));
    //printf("attacked: %d",is_square_attacked(E1, BLACK_P, game.bitboards));


    printf("Time taken: %d ms\n", get_time_ms() - start);
    printf("Nodes: %ld\n", nodes);
    return 0;
}