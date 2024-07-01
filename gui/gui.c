#include <raylib.h>
#include <stdio.h>
#include <raymath.h>
#include <time.h>
#include "pieces.h"
#include "../board.h"
 
/*Texture Definitions*/
Texture2D white_pawn_texture, black_pawn_texture, black_rook_texture,
          white_rook_texture, white_bishop_texture, black_bishop_texture,
          white_knight_texture, black_knight_texture, white_queen_texture,
          black_queen_texture, white_king_texture, black_king_texture;

/*Declaring bitboard representations for chess engine*/
/*Declaring bitboards for all pieces*/
U64 bitboards[14];

/*Declaring necessery flags*/
int side = -1; /*Side to move*/
int enpassant = NO_SQ; /*Enpassant square*/
int castle = 0b1111; /*Castle rights encoded bitwise*/ 
/*1000 for black castle queen*/
/*0100 for black castle king*/
/*0010 for white castle queen*/
/*0001 for white castle king*/

int main() {
    
    const int window_width = 800;
    const int window_height = 800;

    /*Declaring the chess board*/
    Board board;


    /*Declaring necessary flags*/
    int piece_selected = -1;

    // Initialize the window
    InitWindow(window_width, window_height, "Chess Engine");

    /*Preloading all the textures for the chess pieces*/
    white_pawn_texture = LoadTexture("textures/white_pawn.png");
    black_pawn_texture = LoadTexture("textures/black_pawn.png");

    black_rook_texture = LoadTexture("textures/black_rook.png");
    white_rook_texture = LoadTexture("textures/white_rook.png");

    white_bishop_texture = LoadTexture("textures/white_bishop.png");
    black_bishop_texture = LoadTexture("textures/black_bishop.png");

    white_knight_texture = LoadTexture("textures/white_knight.png");
    black_knight_texture = LoadTexture("textures/black_knight.png");

    white_queen_texture = LoadTexture("textures/white_queen.png");
    black_queen_texture = LoadTexture("textures/black_queen.png");

    white_king_texture = LoadTexture("textures/white_king.png");
    black_king_texture = LoadTexture("textures/black_king.png");

    // Check if any of the textures failed to load
    if (white_pawn_texture.id == 0 || black_pawn_texture.id == 0
        || black_rook_texture.id == 0 || white_rook_texture.id == 0
        || white_bishop_texture.id == 0 || black_bishop_texture.id == 0
        || white_knight_texture.id == 0 || black_knight_texture.id == 0 
        || white_queen_texture.id == 0 || black_queen_texture.id == 0 
        || white_king_texture.id == 0 || black_king_texture.id == 0) 
        {
        // Log which texture failed to load
        if (white_pawn_texture.id == 0) printf("Failed to load white_pawn_texture\n");
        if (black_pawn_texture.id == 0) printf("Failed to load black_pawn_texture\n");
        if (black_rook_texture.id == 0) printf("Failed to load black_rook_texture\n");
        if (white_rook_texture.id == 0) printf("Failed to load white_rook_texture\n");
        if (white_bishop_texture.id == 0) printf("Failed to load white_bishop_texture\n");
        if (black_bishop_texture.id == 0) printf("Failed to load black_bishop_texture\n");
        if (white_knight_texture.id == 0) printf("Failed to load white_knight_texture\n");
        if (black_knight_texture.id == 0) printf("Failed to load black_knight_texture\n");
        if (white_queen_texture.id == 0) printf("Failed to load white_queen_texture\n");
        if (black_queen_texture.id == 0) printf("Failed to load black_queen_texture\n");
        if (white_king_texture.id == 0) printf("Failed to load white_king_texture\n");
        if (black_king_texture.id == 0) printf("Failed to load black_king_texture\n");

        CloseWindow();
        return -1; // Exit if any texture was not loaded
    }

    /*Now that the textures are loaded we intialize the board*/
    board = InitBoard();

    /*Set the game states ready to play*/
    restart(bitboards, &side, &enpassant, &castle);

    // Set the game to run at 60 frames-per-second
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            
            Vector2 mouse_position = GetMousePosition();
            mouse_position.x = (int)mouse_position.x / TILESIZE;
            mouse_position.y = (int)mouse_position.y / TILESIZE;
            ActivateSquare(&board, mouse_position.x, mouse_position.y, &piece_selected);

            }
        

        // Start drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw the board
        DrawBoard(&board);
        
        DrawPieces(&board);

        // End drawing
        EndDrawing();


    }

    // Unload texture and close the window
    CloseWindow();

    return 0;
}