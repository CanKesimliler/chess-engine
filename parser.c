#include "parser.h"
static void parse_fen(char *fen, U64 *bitboards, int *side, int *enpassant, int *castle, int *half, int* full){
    int i = 0;
    int square = 0;
    
    for(;i < 15; i++){
        bitboards[i] = BASE;
    }
    i = 0;
    
    while(fen[i] != ' '){
        switch(fen[i]){
            case 'p':
                SET_BIT(bitboards[bP], square++);
                break;
            case 'r':
                SET_BIT(bitboards[bR], square++);
                break;
            case 'n':
                SET_BIT(bitboards[bN], square++);
                break;
            case 'b':
                SET_BIT(bitboards[bB], square++);
                break;
            case 'q':
                SET_BIT(bitboards[bQ], square++);
                break;
            case 'k':
                SET_BIT(bitboards[bK], square++);
                break;
            case 'P':
                SET_BIT(bitboards[wP], square++);
                break;
            case 'R':
                SET_BIT(bitboards[wR], square++);
                break;
            case 'N':
                SET_BIT(bitboards[wN], square++);
                break;
            case 'B':
                SET_BIT(bitboards[wB], square++);
                break;
            case 'Q':
                SET_BIT(bitboards[wQ], square++);
                break;
            case 'K':
                SET_BIT(bitboards[wK], square++);
                break;
            case '/':
                break;
            default:
                square += (int)fen[i] - 48;
                break;
        }
        i++;
    }   
        bitboards[wA] = bitboards[wA] | bitboards[wP] | bitboards[wN] | bitboards[wB] | bitboards[wR] | bitboards[wQ] | bitboards[wK];
        bitboards[bA] = bitboards[bA] | bitboards[bP] | bitboards[bN] | bitboards[bB] | bitboards[bR] | bitboards[bQ] | bitboards[bK];
        bitboards[AP] = bitboards[wA] | bitboards[bA];
        i++;
        *side = (fen[i++] == 'w') ? WHITE_P : BLACK_P;
        *castle = 0;
        while (fen[++i] != ' ')
        {
            switch (fen[i])
            {
            case 'K':
                *castle |= WKC;
                break;
            case 'Q':
                *castle |= WQC;
                break;
            case 'k':
                *castle |= BKC;
                break;
            case 'q':
                *castle |= BQC;
                break;
            case '-':
                break;
            }
        }
        i++;
        *enpassant = (fen[i] != '-') ? ((fen[i] - 'a') + 8 * (8 - (fen[i+1] - '0'))) : NO_SQ;
        i += (fen[i] != '-') ? 3 : 2;
        *half = (int)fen[i++] - 48;
        *full = (int)fen[++i] - 48;
}

void parse_fen_gui(Board *board, char *fen){
    parse_fen(fen, board->bitboards, &board->side, &board->enpassant, &board->castle, &board->half_moves, &board->full_moves);
}

void parse_fen_engine(Game *game, char *fen){
    parse_fen(fen, game->bitboards, &game->side, &game->enpassant, &game->castle, &game->half_moves, &game->full_moves);
}
