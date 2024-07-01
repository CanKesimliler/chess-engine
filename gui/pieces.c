#include "./pieces.h"
#include <stdio.h>
#include <stdlib.h>
#include "../board.h"

Piece *CreatePiece(Vector2 position, int color, int type, Texture2D *texture)
{
    Piece *piece = (Piece *)malloc(sizeof(Piece));
    piece->position = position;
    piece->color = color;
    piece->type = type;
    piece->texture = texture;
    return piece;
}

void InitPieces(Board *board)
{

    int i = 0;

    board->pieces[0] = CreatePiece((Vector2){0, 0}, 1, ROOK, &black_rook_texture);
    board->pieces[1] = CreatePiece((Vector2){1, 0}, 1, KNIGHT, &black_knight_texture);
    board->pieces[2] = CreatePiece((Vector2){2, 0}, 1, BISHOP, &black_bishop_texture);
    board->pieces[3] = CreatePiece((Vector2){3, 0}, 1, QUEEN, &black_queen_texture);
    board->pieces[4] = CreatePiece((Vector2){4, 0}, 1, KING, &black_king_texture);
    board->pieces[5] = CreatePiece((Vector2){5, 0}, 1, BISHOP, &black_bishop_texture);
    board->pieces[6] = CreatePiece((Vector2){6, 0}, 1, KNIGHT, &black_knight_texture);
    board->pieces[7] = CreatePiece((Vector2){7, 0}, 1, ROOK, &black_rook_texture);

    board->pieces[56] = CreatePiece((Vector2){0, 7}, 0, ROOK, &white_rook_texture);
    board->pieces[57] = CreatePiece((Vector2){1, 7}, 0, KNIGHT, &white_knight_texture);
    board->pieces[58] = CreatePiece((Vector2){2, 7}, 0, BISHOP, &white_bishop_texture);
    board->pieces[59] = CreatePiece((Vector2){3, 7}, 0, QUEEN, &white_queen_texture);
    board->pieces[60] = CreatePiece((Vector2){4, 7}, 0, KING, &white_king_texture);
    board->pieces[61] = CreatePiece((Vector2){5, 7}, 0, BISHOP, &white_bishop_texture);
    board->pieces[62] = CreatePiece((Vector2){6, 7}, 0, KNIGHT, &white_knight_texture);
    board->pieces[63] = CreatePiece((Vector2){7, 7}, 0, ROOK, &white_rook_texture);

    for (; i < 8; i++)
    {
        board->pieces[8 + i] = CreatePiece((Vector2){i, 1}, 1, PAWN, &black_pawn_texture);
        board->pieces[48 + i] = CreatePiece((Vector2){i, 6}, 0, PAWN, &white_pawn_texture);
    }

    for (i = 16; i < 48; i++)
    {
        board->pieces[i] = NULL;
    }
}

Board InitBoard()
{
    Board board;
    int i, j;
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (i > 2 && i < 6)
            {
                board.board[i * 8 + j].occupied = 0;
            }
            else
            {
                board.board[i * 8 + j].occupied = 1;
            }
            board.board[i * 8 + j].active = 0;
            board.board[i * 8 + j].position = (Vector2){j, i};
        }
    }
    InitPieces(&board);
    return board;
};

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
        int isActive = board->board[i].active;
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
        DrawRectangle(board->board[i].position.x * TILESIZE, board->board[i].position.y * TILESIZE, 100, 100, color);
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

void ActivateSquare(Board *board, int x, int y, int *piece_selected){
    if (false == (x >= 0 && x < 8 && y >= 0 && y < 8)) return;
    if (-1 == *piece_selected)
    {
        if (false == board->board[GET_SQUARE(x, y)].occupied) return;

        board->board[GET_SQUARE(x, y)].active = 1;
        board->board[*piece_selected].active = 0;
        *piece_selected = GET_SQUARE(x, y);
        return;
    }
    if(GET_SQUARE(x,y) == *piece_selected) return;
    
    board->board[GET_SQUARE(x, y)].active = 0;
    *piece_selected = false;
}

U64 GetAttackBitboard(Piece *piece, U64 all_pieces)
{
    U64 attacks = 0;
    int square = GET_SQUARE(piece->position.x, piece->position.y);
    switch (piece->type)
    {
    case PAWN:
        attacks = pawn_attacks(square, piece->color);
        break;
    case KNIGHT:
        attacks = knight_attacks(square);
        break;
    case BISHOP:
        attacks = mask_bishop_attacks(square);
        break;
    case ROOK:
        attacks = rook_attacks(square, all_pieces);
        break;
    case QUEEN:
        attacks = queen_attacks(square, all_pieces);
        break;
    case KING:
        attacks = king_attacks(square);
        break;
    default:
        break;
    }
    return attacks;
}
