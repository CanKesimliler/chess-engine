#include <stdio.h>
#include <stdlib.h>
#include "engine.h"
#include "parser.h"


int main(void){
    Game game, game_copy;
    restart_game(&game);
    parse_fen_engine(&game, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
    // 65535d
    //18446462598732840960d
    init_pieces_attacks();
    init_slider_attacks();
    GenerateMoves(&game);
    printMoveList(&MoveList);
    return 0;
}