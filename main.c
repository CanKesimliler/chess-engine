#include <stdio.h>
#include <stdlib.h>
#include "engine.h"
#include "parser.h"


int main(void){
    Game game, game_copy;
    restart_game(&game);
    parse_fen_engine(&game, "r6r/1b2k1bq/8/8/7B/8/8/R3K2R b KQ - 3 2");
    // 65535d
    //18446462598732840960d
    init_pieces_attacks();
    init_slider_attacks();
    GenerateMoves(&game);
    //addMove(&MoveList, ENCODE_MOVE(H7, H8, wP, wQ, 0, 0, 0, 0));
    printMoveList(&MoveList);

    return 0;
}