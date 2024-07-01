#ifndef PIECES_H_
#define PIECES_H_
#include <raylib.h>
#include <stdbool.h>
#define RANKS 8
#define FILES 8
#define TILESIZE 100

#define GET_SQUARE(x, y) (int)((x)+(y*FILES))  

extern Texture2D white_pawn_texture, black_pawn_texture, black_rook_texture,
          white_rook_texture, white_bishop_texture, black_bishop_texture,
          white_knight_texture, black_knight_texture, white_queen_texture,
          black_queen_texture, white_king_texture, black_king_texture;

enum PieceType{
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

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
    bool occupied;
    Vector2 position;
} typedef Square;

struct Board{
    Square board[64];
    Piece* pieces[64];
}typedef Board;

Board InitBoard();
void DrawBoard(Board *board);
void DrawPieces(Board *board);
void InitPieces(Board *board);
void ActivateSquare(Board *board, int x, int y, int *piece_selected);

#endif

