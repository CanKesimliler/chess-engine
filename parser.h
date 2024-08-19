#ifndef PARSER_H_
#define PARSER_H_
#include "engine.h"
#include "./gui/gui.h"
#define BASE 0ULL
/*Parsing FEN strings*/
/*rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1*/

static void parse_fen(char *fen, U64 *bitboards, int *side, int *enpassant, int *castle, int *half, int* full);
void parse_fen_gui(Board *board, char* fen);
void parse_fen_engine(Game *game, char* fen);
#endif /* PARSER_H_ */