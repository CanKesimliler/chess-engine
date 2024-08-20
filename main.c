#include <stdio.h>
#include <stdlib.h>
#include "engine.h"
#include "parser.h"

int main(void){
    Game game;
    restart_game(&game);
    parse_fen_engine(&game, "r3k3/6p1/8/1pP5/8/8/8/4K2R w Kq b3 2 0");
    // 65535d
    // 18446462598732840960d
    init_pieces_attacks();
    init_slider_attacks();
    GenerateMoves(&game);
    return 0;
}