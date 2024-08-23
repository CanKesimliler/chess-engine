#include <stdio.h>
#include <stdlib.h>
#include "engine.h"
#include "parser.h"


int main(void){
    restart_game();
    parse_fen_engine(&game, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    // 65535d
    //18446462598732840960d
    init_pieces_attacks();
    init_slider_attacks();
    GenerateMoves();
    print_bitboard(game.bitboards[wP]);
    COPY_GAME();
    make_move(MoveList.moves[0]);
    RESTORE_GAME();
    print_bitboard(game.bitboards[wP]);
    return 0;
}