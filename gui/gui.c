#include <raylib.h>
#include <stdio.h>
#include <raymath.h>
#include <time.h>
#include "gui.h"
#include "../engine.h"
#include "../parser.h"
 
/*Texture Definitions*/
Texture2D white_pawn_texture, black_pawn_texture, black_rook_texture,
          white_rook_texture, white_bishop_texture, black_bishop_texture,
          white_knight_texture, black_knight_texture, white_queen_texture,
          black_queen_texture, white_king_texture, black_king_texture;


int main() {
    
    char choice;


    const int window_width = 800;
    const int window_height = 800;

    /*Declaring the chess board*/
    Board board;

    
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
    InitBoard(&board);

    // Set the game to run at 60 frames-per-second
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        

        // Start drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw the board
        DrawBoard(&board);
        DrawPieces(&board);

        // Check if the user clicked on a square
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            // Get the mouse position
            Vector2 mouse = GetMousePosition();
            // Check if the mouse is within the board
            if (mouse.x >= 0 && mouse.x < window_width && mouse.y >= 0 && mouse.y < window_height) {
                // Get the square that was clicked
                mouse.x = (int)mouse.x / TILESIZE;
                mouse.y = (int)mouse.y / TILESIZE;
                // Activate the square
                ActivateSquare(&board, &mouse);
            }
        }

        // End drawing
        EndDrawing();


    }

    // Unload texture and close the window
    CloseWindow();

    return 0;
}