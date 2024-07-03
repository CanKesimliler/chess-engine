#ifndef PARSER_H_
#define PARSER_H_
#include "engine.h"
#define BASE 0ULL
/*Parsing FEN strings*/
/*rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1*/

void parse_fen(char *fen, U64 *bitboards, int *side, int *enpassant, int *castle, int *half, int* full);

#endif /* PARSER_H_ */