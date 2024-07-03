#include "gui.h"
#include <stdio.h>
#include <stdlib.h>
#include "../engine.h"

Piece *CreatePiece(Vector2 position, int color, int type, Texture2D *texture)
{
    Piece *piece = (Piece *)malloc(sizeof(Piece));
    piece->position = position;
    piece->color = color;
    piece->type = type;
    piece->texture = texture;
    return piece;
}

static inline int get_first_1bit(U64 bitboard){
    
    /*Check empty bitboards*/
    if(!bitboard) return -1;
    
    /*Count trailing bits*/
    return COUNT_BITS((bitboard & -bitboard)-1);
}

void InitPieces(Board *board)
{
    int square;
    U64 temp_bb;
    
    /*Init pieces to null*/
    for(int i = 0; i < 64; i++){
        board->pieces[i] = NULL;
    }


    /************/
    /*Init pawns*/
    /************/

    /*White pawns*/
    temp_bb = board->bitboards[wP];
    while((square = get_first_1bit(temp_bb)) != NO_SQ){
        board->pieces[square] = CreatePiece((Vector2){square % FILES, square / RANKS}, WHITE_P, wP, &white_pawn_texture);
        REMOVE_BIT(temp_bb, square);
    }
    
    /*Black pawns*/
    temp_bb = board->bitboards[bP];
    while((square = get_first_1bit(temp_bb)) != NO_SQ){
        board->pieces[square] = CreatePiece((Vector2){square % FILES, square / RANKS}, BLACK_P, bP, &black_pawn_texture);
        REMOVE_BIT(temp_bb, square);
    }

    /**************/
    /*Init Knights*/
    /**************/

    /*White Knights*/
    temp_bb = board->bitboards[wN];
    while((square = get_first_1bit(temp_bb)) != NO_SQ){
        board->pieces[square] = CreatePiece((Vector2){square % FILES, square / RANKS}, WHITE_P, wN, &white_knight_texture);
        REMOVE_BIT(temp_bb, square);
    }
    
    /*Black Knights*/
    temp_bb = board->bitboards[bN];
    while((square = get_first_1bit(temp_bb)) != NO_SQ){
        board->pieces[square] = CreatePiece((Vector2){square % FILES, square / RANKS}, BLACK_P, bN, &black_knight_texture);
        REMOVE_BIT(temp_bb, square);
    }

    /**************/
    /*Init Bishops*/
    /**************/

    /*White Bishops*/
    temp_bb = board->bitboards[wB];
    while((square = get_first_1bit(temp_bb)) != NO_SQ){
        board->pieces[square] = CreatePiece((Vector2){square % FILES, square / RANKS}, WHITE_P, wB, &white_bishop_texture);
        REMOVE_BIT(temp_bb, square);
    }
    
    /*Black Bishops*/
    temp_bb = board->bitboards[bB];
    while((square = get_first_1bit(temp_bb)) != NO_SQ){
        board->pieces[square] = CreatePiece((Vector2){square % FILES, square / RANKS}, BLACK_P, bB, &black_bishop_texture);
        REMOVE_BIT(temp_bb, square);
    }

    /************/
    /*Init Rooks*/
    /************/

    /*White Rooks*/
    temp_bb = board->bitboards[wR];
    while((square = get_first_1bit(temp_bb)) != NO_SQ){
        board->pieces[square] = CreatePiece((Vector2){square % FILES, square / RANKS}, WHITE_P, wR, &white_rook_texture);
        REMOVE_BIT(temp_bb, square);
    }
    
    /*Black Rooks*/
    temp_bb = board->bitboards[bR];
    while((square = get_first_1bit(temp_bb)) != NO_SQ){
        board->pieces[square] = CreatePiece((Vector2){square % FILES, square / RANKS}, BLACK_P, bR, &black_rook_texture);
        REMOVE_BIT(temp_bb, square);
    }
    
    /*************/
    /*Init Queens*/
    /*************/

    /*White Queens*/
    temp_bb = board->bitboards[wQ];
    while((square = get_first_1bit(temp_bb)) != NO_SQ){
        board->pieces[square] = CreatePiece((Vector2){square % FILES, square / RANKS}, WHITE_P, wQ, &white_queen_texture);
        REMOVE_BIT(temp_bb, square);
    }
    
    /*Black Queens*/
    temp_bb = board->bitboards[bQ];
    while((square = get_first_1bit(temp_bb)) != NO_SQ){
        board->pieces[square] = CreatePiece((Vector2){square % FILES, square / RANKS}, BLACK_P, bQ, &black_queen_texture);
        REMOVE_BIT(temp_bb, square);
    }

    /************/
    /*Init Kings*/
    /************/

    /*White King*/
    temp_bb = board->bitboards[wK];
    while((square = get_first_1bit(temp_bb)) != NO_SQ){
        board->pieces[square] = CreatePiece((Vector2){square % FILES, square / RANKS}, WHITE_P, wK, &white_king_texture);
        REMOVE_BIT(temp_bb, square);
    }
    
    /*Black King*/
    temp_bb = board->bitboards[bK];
    while((square = get_first_1bit(temp_bb)) != NO_SQ){
        board->pieces[square] = CreatePiece((Vector2){square % FILES, square / RANKS}, BLACK_P, bK, &black_king_texture);
        REMOVE_BIT(temp_bb, square);
    }

}

void InitBoard(Board *board)
{
    int i = 0;
    for (; i < 64; i++) board->squares[i].position = (Vector2){i % FILES, i / RANKS};
    RestartBoard(board);
    InitPieces(board);
};

void RestartBoard(Board *board){
    int i = 0;
    for (; i < 64; i++){
        board->squares[i].active = false;
        board->pieces[i] = NULL;
    } 
    
    board->side = WHITE_P;
    board->enpassant = NO_SQ;
    board->castle = 0b1111;
    board->half_moves = 0;
    board->full_moves = 0;
    board->piece_selected = NO_SQ;

    board->bitboards[wP] = 0x00FF000000000000;
    board->bitboards[wN] = 0x4200000000000000;
    board->bitboards[wB] = 0x2400000000000000;
    board->bitboards[wR] = 0x8100000000000000;
    board->bitboards[wQ] = 0x0800000000000000;
    board->bitboards[wK] = 0x1000000000000000;
    
    board->bitboards[bP] = 0x000000000000FF00;
    board->bitboards[bN] = 0x0000000000000042;
    board->bitboards[bB] = 0x0000000000000024;
    board->bitboards[bR] = 0x0000000000000081;
    board->bitboards[bQ] = 0x0000000000000008;
    board->bitboards[bK] = 0x0000000000000010;

    board->bitboards[wA] = 18446462598732840960ULL;
    board->bitboards[bA] = 65535ULL;

    board->bitboards[AP] = 18446462598732906495ULL;
}

/**
 * Draws the chessboard on the screen.
 * @param board The chessboard to be drawn.
 */
void DrawBoard(Board *board)
{
    int i;
    for (i = 0; i < 64; i++)
    {
        int isEvenSum = (((i % 8) + (i / 8)) % 2 == 0);
        int isActive = board->squares[i].active;
        Color color;

        // Determine the color of the square based on its position and activity status
        if (isEvenSum)
        {
            color = isActive ? (Color){184, 207, 220, 255} : (Color){241, 241, 241, 255};
        }
        else
        {
            color = isActive ? (Color){129, 144, 194, 255} : (Color){128, 118, 181, 255};
        }

        // Draw the square on the screen
        DrawRectangle(board->squares[i].position.x * TILESIZE, board->squares[i].position.y * TILESIZE, 100, 100, color);
    }
}

void DrawPieces(Board *board)
{
    int i;
    for (i = 0; i < 64; i++)
    {
        if (board->pieces[i] != NULL)
        {
            DrawTexture(*(board->pieces[i]->texture),
                        (board->pieces[i]->position.x * TILESIZE) + 5,
                        (board->pieces[i]->position.y * TILESIZE) + 2,
                        WHITE);
        }
    }
}

void ActivateSquare(Board *board, Vector2 *coordinates){
    printf("\n\n\n\n%d: %dllu\n\n\n\n\n",GET_SQUARE(coordinates->x, coordinates->y), board->pieces[GET_SQUARE(coordinates->x, coordinates->y)]);
    /*If no square is active at the moment*/
    if (NO_SQ == board->piece_selected)
    {
        /*If the clicked square is empty do nothing*/
        if (NULL == board->pieces[GET_SQUARE(coordinates->x, coordinates->y)]) return;
        
        /*If not activate that square*/
        board->squares[GET_SQUARE(coordinates->x, coordinates->y)].active = true;
        /*Update the selected piece variable*/
        board->piece_selected = GET_SQUARE(coordinates->x, coordinates->y);
        return;
    }
    /*If the same square with a piece is clicked again*/
    if(GET_SQUARE(coordinates->x, coordinates->y) == board->piece_selected){
        /*Deactivate the square*/
        board->squares[board->piece_selected].active = false;
        /*Update the selected piece variable*/
        board->piece_selected = NO_SQ;
        return;
    }
}