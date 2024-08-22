#include <stdio.h>
#include <stdlib.h>
#include "engine.h"
#include "parser.h"


int main(void){
    Game game, game_copy;
    restart_game(&game);
    parse_fen_engine(&game, "r3k2r/8/8/8/8/8/8/R1P1K2R w KQkq - 0 1");
    // 65535d
    //18446462598732840960d
    init_pieces_attacks();
    init_slider_attacks();
    generate_moves(&game);
    printMoveList(&MoveList);

    int index = 0;

    while (index < MoveList.moveCount){
        make_move(&game, &game_copy, MoveList.moves[index]);
        print_bitboard(game.bitboards[wP]);
        //RESTORE_GAME();
        //print_bitboard(game.bitboards[wP]);
        index++;
    }


    return 0;
}