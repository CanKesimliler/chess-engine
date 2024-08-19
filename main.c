#include <stdio.h>
#include <stdlib.h>
#include "engine.h"
#include "parser.h"

int main(void){
    Game game;
    restart_game(&game);
    parse_fen_engine(&game, "r2q1rk1/pp1n1ppp/2pb1n2/3p4/3P4/2NBPN2/PPP2PPP/R2Q1RK1 w - - 0 18");
    // 65535d
    // 18446462598732840960d
    init_pieces_attacks();
    init_slider_attacks();
    //GenerateMoves(&game);
    addMove(&MoveList, ENCODE_MOVE(E2, E4, wQ, 0, 0, 0, 0, 0));
    return 0;
}