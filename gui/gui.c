#include <raylib.h>
#include <stdio.h>
#include <raymath.h>
#include <time.h>
#include "gui.h"
#include "../engine.h"
#include "../parser.h"
#if defined WIN64
#include <windows.h>
#else
#include <unistd.h>
#include <fcntl.h>
#endif

/*Texture Definitions*/
Texture2D white_pawn_texture, black_pawn_texture, black_rook_texture,
    white_rook_texture, white_bishop_texture, black_bishop_texture,
    white_knight_texture, black_knight_texture, white_queen_texture,
    black_queen_texture, white_king_texture, black_king_texture;

int main()
{

    const int window_width = 800;
    const int window_height = 800;

    int player = WHITE_P;

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
    if (white_pawn_texture.id == 0 || black_pawn_texture.id == 0 || black_rook_texture.id == 0 || white_rook_texture.id == 0 || white_bishop_texture.id == 0 || black_bishop_texture.id == 0 || white_knight_texture.id == 0 || black_knight_texture.id == 0 || white_queen_texture.id == 0 || black_queen_texture.id == 0 || white_king_texture.id == 0 || black_king_texture.id == 0)
    {
        // Log which texture failed to load
        if (white_pawn_texture.id == 0)
            printf("Failed to load white_pawn_texture\n");
        if (black_pawn_texture.id == 0)
            printf("Failed to load black_pawn_texture\n");
        if (black_rook_texture.id == 0)
            printf("Failed to load black_rook_texture\n");
        if (white_rook_texture.id == 0)
            printf("Failed to load white_rook_texture\n");
        if (white_bishop_texture.id == 0)
            printf("Failed to load white_bishop_texture\n");
        if (black_bishop_texture.id == 0)
            printf("Failed to load black_bishop_texture\n");
        if (white_knight_texture.id == 0)
            printf("Failed to load white_knight_texture\n");
        if (black_knight_texture.id == 0)
            printf("Failed to load black_knight_texture\n");
        if (white_queen_texture.id == 0)
            printf("Failed to load white_queen_texture\n");
        if (black_queen_texture.id == 0)
            printf("Failed to load black_queen_texture\n");
        if (white_king_texture.id == 0)
            printf("Failed to load white_king_texture\n");
        if (black_king_texture.id == 0)
            printf("Failed to load black_king_texture\n");

        CloseWindow();
        return -1; // Exit if any texture was not loaded
    }

    // Initialize the audio
    InitAudioDevice();

    // Load the audio
    Sound move_sound = LoadSound("sounds/move-self.mp3");
    Sound capture_sound = LoadSound("sounds/capture.mp3");
    Sound check_sound = LoadSound("sounds/move-check.mp3");
    Sound checkmate_sound = LoadSound("sounds/game-end.mp3");
    Sound illegal_sound = LoadSound("sounds/illegal.mp3");
    Sound promotion_sound = LoadSound("sounds/promote.mp3");
    Sound castle_sound = LoadSound("sounds/castle.mp3");
    Sound time_sound = LoadSound("sounds/time.mp3");

    // Check if the audio loaded
    if (move_sound.stream.buffer == NULL || capture_sound.stream.buffer == NULL || check_sound.stream.buffer == NULL || checkmate_sound.stream.buffer == NULL || illegal_sound.stream.buffer == NULL || promotion_sound.stream.buffer == NULL || castle_sound.stream.buffer == NULL || time_sound.stream.buffer == NULL)
    {
        if (move_sound.stream.buffer == NULL)
            printf("Failed to load move_sound\n");
        if (capture_sound.stream.buffer == NULL)
            printf("Failed to load capture_sound\n");
        if (check_sound.stream.buffer == NULL)
            printf("Failed to load check_sound\n");
        if (checkmate_sound.stream.buffer == NULL)
            printf("Failed to load checkmate_sound\n");
        if (illegal_sound.stream.buffer == NULL)
            printf("Failed to load illegal_sound\n");
        if (promotion_sound.stream.buffer == NULL)
            printf("Failed to load promotion_sound\n");
        if (castle_sound.stream.buffer == NULL)
            printf("Failed to load castle_sound\n");
        if (time_sound.stream.buffer == NULL)
            printf("Failed to load time_sound\n");

        CloseAudioDevice();
        CloseWindow();
        return -1;
    }

    // Assign the sounds to the board
    board.sound_effects[MOVE_SOUND] = &move_sound;
    board.sound_effects[CAPTURE_SOUND] = &capture_sound;
    board.sound_effects[CHECK_SOUND] = &check_sound;
    board.sound_effects[CHECKMATE_SOUND] = &checkmate_sound;
    board.sound_effects[ILLEGAL_SOUND] = &illegal_sound;
    board.sound_effects[PROMOTION_SOUND] = &promotion_sound;
    board.sound_effects[CASTLE_SOUND] = &castle_sound;
    board.sound_effects[TIME_SOUND] = &time_sound;

/*Get the engine Up and running*/
#if defined WIN64
    // Windows-specific code (to be implemented)
#else
    int to_engine[2], from_engine[2];
    pid_t pid;

    if (pipe(to_engine) == -1 || pipe(from_engine) == -1)
    {
        perror("pipe failed");
        return -1;
    }

    pid = fork();

    if (pid == -1)
    {
        perror("fork failed");
        return -1;
    }
    else if (pid == 0)
    {
        // Child process
        // Redirect stdin and stdout
        dup2(to_engine[0], STDIN_FILENO);
        dup2(from_engine[1], STDOUT_FILENO);


        // Close unused pipe ends
        close(to_engine[1]);
        close(from_engine[0]);

        // Execute the engine
        execl("../main", "main", NULL);

        // If execl fails
        perror("execl failed");
        return -1;
    }
    else
    {
        // Parent process
        // Close unused pipe ends
        close(to_engine[0]);
        close(from_engine[1]);
    }

    // Send "isready" to the engine
    const char *cmd = "sasza\n";
    ssize_t bytes_written = write(to_engine[1], cmd, strlen(cmd));
    puts("Sent isready to engine");
    if (bytes_written == -1)
    {
        perror("Failed to write to engine");
        return -1;
    }

    char buffer[256];
    ssize_t n;

    // Wait for the response from the child
    printf("Response from receiver: ");
    while ((n = read(from_engine[0], buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[n] = '\0';
        printf("%s", buffer);

        // Break if newline is found
        if (strchr(buffer, '\n') != NULL)
            break;
    }
#endif

    /*Now that the textures are loaded we intialize the board*/
    InitBoard(&board);
    //parse_fen_gui(&board, "6b1/7P/k7/8/8/K7/7p/6R1 w - - 0 1");
    // 2q1rk1/pp1n1ppp/2pb1n2/3p4/3P4/2NBPN2/PPP2PPP/R2Q1RK1 w - - 0 18
    InitPieces(&board);
    // Set the game to run at 60 frames-per-second
    SetTargetFPS(60);
    // Main game loop
    while (!WindowShouldClose())
    {

        // Start drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw the board
        DrawBoard(&board);
        DrawPieces(&board);

        // Check if the user clicked on a square
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            // Get the mouse position
            Vector2 mouse = GetMousePosition();
            // Check if the mouse is within the board
            if (!(mouse.x >= 0 && mouse.x < window_width && mouse.y >= 0 && mouse.y < window_height))
                continue;

            // Get the square that was clicked
            mouse.x = (int)mouse.x / TILESIZE;
            mouse.y = (int)mouse.y / TILESIZE;
            // If the Promotion menu is open
            if (board.promotion_square != NO_SQ)
            {
                int bias = board.side ? -1 : 1;
                int origin_y = board.squares[board.piece_selected].position.y + bias * -1;
                int origin_x = board.squares[board.promotion_square].position.x;
                printf("Origin X: %d, Origin Y: %d\n", origin_x, origin_y);
                printf("Mouse X: %f, Mouse Y: %f\n", mouse.x, mouse.y);
                printf("Selected: %d\n", board.piece_selected);
                // if the clicked square is within the promotion menu
                if (mouse.y == origin_y && mouse.x == origin_x)
                {
                    // Promote the pawn to queen
                    MakeMove(&board, board.piece_selected, GET_SQUARE(mouse.x, origin_y), wQ + (board.side * 6));
                    HandleTextures(&board, GET_SQUARE(mouse.x, origin_y), wQ + ((board.side ^ 1) * 6));
                    board.promotion_square = NO_SQ;
                }
                else if (mouse.y == origin_y + bias && mouse.x == origin_x)
                {
                    // Promote the pawn to rook
                    MakeMove(&board, board.piece_selected ,GET_SQUARE(mouse.x, origin_y), wR + (board.side * 6));
                    HandleTextures(&board, GET_SQUARE(mouse.x, origin_y), wR + ((board.side ^ 1) * 6));
                    board.promotion_square = NO_SQ;
                }
                else if (mouse.y == origin_y + 2 * bias && mouse.x == origin_x)
                {
                    // Promote the pawn to bishop
                    MakeMove(&board, board.piece_selected, GET_SQUARE(mouse.x, origin_y), wB + (board.side * 6));
                    HandleTextures(&board, GET_SQUARE(mouse.x, origin_y), wB + ((board.side ^ 1) * 6));
                    board.promotion_square = NO_SQ;
                }
                else if (mouse.y == origin_y + 3 * bias && mouse.x == origin_x)
                {
                    // Promote the pawn to knight
                    MakeMove(&board, board.piece_selected ,GET_SQUARE(mouse.x, origin_y), wN + (board.side * 6));
                    HandleTextures(&board, GET_SQUARE(mouse.x, origin_y), wN + ((board.side ^ 1) * 6));
                    board.promotion_square = NO_SQ;
                }
                board.promotion_square = NO_SQ;
                board.piece_selected = NO_SQ;
                continue;
            }
            // If the promotion window is not open
            //  Activate the square
            if (ActivateSquare(&board, &mouse) == 1)
            {
                HandleTextures(&board, GET_SQUARE(mouse.x, mouse.y), -1);
            }
        }

        if (board.piece_selected != NO_SQ)
        {
            DrawAttacks(&board);
        }

        if (board.promotion_square != NO_SQ)
        {
            DrawPromotionMenu(&board, board.side);
        }

        // Check if the engine is to move
        if (board.side != player)
        {
#if defined WIN64
            // Windows-specific code (to be implemented)
#else
           
#endif
        }
        //*/

        // End drawing
        EndDrawing();
    }

    // Unload texture
    UnloadTexture(white_pawn_texture);
    UnloadTexture(black_pawn_texture);
    UnloadTexture(black_rook_texture);
    UnloadTexture(white_rook_texture);
    UnloadTexture(white_bishop_texture);
    UnloadTexture(black_bishop_texture);
    UnloadTexture(white_knight_texture);
    UnloadTexture(black_knight_texture);
    UnloadTexture(white_queen_texture);
    UnloadTexture(black_queen_texture);
    UnloadTexture(white_king_texture);
    UnloadTexture(black_king_texture);

    // Unload audio
    UnloadSound(move_sound);
    UnloadSound(capture_sound);
    UnloadSound(check_sound);
    UnloadSound(checkmate_sound);
    UnloadSound(illegal_sound);
    UnloadSound(promotion_sound);
    UnloadSound(castle_sound);
    UnloadSound(time_sound);

    // Close the audio device
    CloseAudioDevice();

    // Close the window
    CloseWindow();
    write(to_engine[1], "quit\n", 5);
    
    close(to_engine[0]);
    close(from_engine[1]);

    return 0;
}
