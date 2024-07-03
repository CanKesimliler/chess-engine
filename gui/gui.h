#ifndef PIECES_H_
#define PIECES_H_
#include <raylib.h>
#include <stdbool.h>
#include "../engine.h"
#define RANKS 8
#define FILES 8
#define TILESIZE 100

#define GET_SQUARE(x, y) (int)((x)+(y*FILES))  

extern Texture2D white_pawn_texture, black_pawn_texture, black_rook_texture,
          white_rook_texture, white_bishop_texture, black_bishop_texture,
          white_knight_texture, black_knight_texture, white_queen_texture,
          black_queen_texture, white_king_texture, black_king_texture;


struct Piece
{
    Vector2 position;
    int color;
    int type;
    Texture2D* texture;
    
}typedef Piece;

Piece* CreatePiece(Vector2 position, int color, int type, Texture2D *texture);

struct Square{
    bool active;
    Vector2 position;
} typedef Square;

struct Board{
    Square squares[64];
    Piece* pieces[64];
    U64 bitboards[15];
    int side; /*Side to move*/
    int enpassant; /*Enpassant square*/
    int castle; /*Castle rights encoded bitwise*/
    /*1000 for white castle king*/
    /*0100 for white castle queen*/
    /*0010 for black castle king*/
    /*0001 for black castle queen*/
    int half_moves; /*Half moves counter for 50 moves rule*/
    int full_moves; /*Full moves counter*/
    int piece_selected; 
}typedef Board;

void InitBoard(Board *board);
void RestartBoard(Board *board);
void DrawBoard(Board *board);
void DrawPieces(Board *board);
void InitPieces(Board *board);
void ActivateSquare(Board *board, Vector2 *coordinates);

#endif

