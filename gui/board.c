#include "gui.h"
#include <stdio.h>
#include <stdlib.h>
#include "../engine.h"

U64 pawn_attack_table[2][64];
U64 knight_attack_table[64];
U64 king_attack_table[64];

Piece *CreatePiece(int color, int type, Texture2D *texture)
{
    Piece *piece = (Piece *)malloc(sizeof(Piece));
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
        board->pieces[square] = CreatePiece(WHITE_P, wP, &white_pawn_texture);
        REMOVE_BIT(temp_bb, square);
    }
    
    /*Black pawns*/
    temp_bb = board->bitboards[bP];
    while((square = get_first_1bit(temp_bb)) != NO_SQ){
        board->pieces[square] = CreatePiece(BLACK_P, bP, &black_pawn_texture);
        REMOVE_BIT(temp_bb, square);
    }

    /**************/
    /*Init Knights*/
    /**************/

    /*White Knights*/
    temp_bb = board->bitboards[wN];
    while((square = get_first_1bit(temp_bb)) != NO_SQ){
        board->pieces[square] = CreatePiece(WHITE_P, wN, &white_knight_texture);
        REMOVE_BIT(temp_bb, square);
    }
    
    /*Black Knights*/
    temp_bb = board->bitboards[bN];
    while((square = get_first_1bit(temp_bb)) != NO_SQ){
        board->pieces[square] = CreatePiece(BLACK_P, bN, &black_knight_texture);
        REMOVE_BIT(temp_bb, square);
    }

    /**************/
    /*Init Bishops*/
    /**************/

    /*White Bishops*/
    temp_bb = board->bitboards[wB];
    while((square = get_first_1bit(temp_bb)) != NO_SQ){
        board->pieces[square] = CreatePiece(WHITE_P, wB, &white_bishop_texture);
        REMOVE_BIT(temp_bb, square);
    }
    
    /*Black Bishops*/
    temp_bb = board->bitboards[bB];
    while((square = get_first_1bit(temp_bb)) != NO_SQ){
        board->pieces[square] = CreatePiece(BLACK_P, bB, &black_bishop_texture);
        REMOVE_BIT(temp_bb, square);
    }

    /************/
    /*Init Rooks*/
    /************/

    /*White Rooks*/
    temp_bb = board->bitboards[wR];
    while((square = get_first_1bit(temp_bb)) != NO_SQ){
        board->pieces[square] = CreatePiece(WHITE_P, wR, &white_rook_texture);
        REMOVE_BIT(temp_bb, square);
    }
    
    /*Black Rooks*/
    temp_bb = board->bitboards[bR];
    while((square = get_first_1bit(temp_bb)) != NO_SQ){
        board->pieces[square] = CreatePiece(BLACK_P, bR, &black_rook_texture);
        REMOVE_BIT(temp_bb, square);
    }
    
    /*************/
    /*Init Queens*/
    /*************/

    /*White Queens*/
    temp_bb = board->bitboards[wQ];
    while((square = get_first_1bit(temp_bb)) != NO_SQ){
        board->pieces[square] = CreatePiece(WHITE_P, wQ, &white_queen_texture);
        REMOVE_BIT(temp_bb, square);
    }
    
    /*Black Queens*/
    temp_bb = board->bitboards[bQ];
    while((square = get_first_1bit(temp_bb)) != NO_SQ){
        board->pieces[square] = CreatePiece(BLACK_P, bQ, &black_queen_texture);
        REMOVE_BIT(temp_bb, square);
    }

    /************/
    /*Init Kings*/
    /************/

    /*White King*/
    temp_bb = board->bitboards[wK];
    while((square = get_first_1bit(temp_bb)) != NO_SQ){
        board->pieces[square] = CreatePiece(WHITE_P, wK, &white_king_texture);
        REMOVE_BIT(temp_bb, square);
    }
    
    /*Black King*/
    temp_bb = board->bitboards[bK];
    while((square = get_first_1bit(temp_bb)) != NO_SQ){
        board->pieces[square] = CreatePiece(BLACK_P, bK, &black_king_texture);
        REMOVE_BIT(temp_bb, square);
    }

}

void InitBoard(Board *board)
{
    int i = 0;
    for (; i < 64; i++) board->squares[i].position = (Vector2){i % FILES, i / RANKS};
    RestartBoard(board);
    init_pieces_attacks();
    init_slider_attacks();
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
    board->promotion_square = NO_SQ;

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
                        ((i % RANKS) * TILESIZE) + 5,
                        ((i / FILES) * TILESIZE) + 2,
                        WHITE);
        }
    }
}

U64 GetAttacks(Board *board){
    int square = board->piece_selected;
    U64 attacks = 0UL;
    switch (board->pieces[board->piece_selected]->type)
    {
    case wP:
        attacks |= pawn_attack_table[WHITE_P][square] & board->bitboards[bA];
        if(!((1ULL << (square - 8)) & ~board->bitboards[AP])) break;
        attacks |= (1ULL << (square - 8)) & ~board->bitboards[AP];
        if(A2 <= square && square <= H2) attacks |= ((1ULL << (square - 16)) & ~board->bitboards[AP]);
        //enpassant
        if(board->enpassant != NO_SQ){
            if(square % FILES != 0 && square - 9 == board->enpassant) attacks |= (1ULL << (square - 9));
            if(square % FILES != 7 && square - 7 == board->enpassant) attacks |= (1ULL << (square - 7));
        }
        break;
    case bP:
        attacks |= pawn_attack_table[board->pieces[board->piece_selected]->color][square] & board->bitboards[wA];
        if(!((1ULL << (square + 8)) & ~board->bitboards[AP])) break;
        attacks |= (1ULL << (square + 8)) & ~board->bitboards[AP];
        if(A7 <= square && square <= H7) attacks |= ((1ULL << (square + 16)) & ~board->bitboards[AP]);
        //enpassant
        if(board->enpassant != NO_SQ){
            if(square % FILES != 0 && square + 7 == board->enpassant) attacks |= (1ULL << (square + 7));
            if(square % FILES != 7 && square + 9 == board->enpassant) attacks |= (1ULL << (square + 9));
        }
        break;
    case wN:
        attacks |= knight_attack_table[square] & ~board->bitboards[wA];
        break;
    case bN:
        attacks |= knight_attack_table[square] & ~board->bitboards[bA];
        break;
    case wB:
        attacks |= generate_bishop_attacks(square, board->bitboards[AP]) & ~board->bitboards[wA];
        break;
    case bB:
        attacks |= generate_bishop_attacks(square, board->bitboards[AP]) & ~board->bitboards[bA];
        break;
    case wR:
        attacks |= generate_rook_attacks(square, board->bitboards[AP]) & ~board->bitboards[wA];
        break;
    case bR:
        attacks |= generate_rook_attacks(square, board->bitboards[AP]) & ~board->bitboards[bA];
        break;
    case wQ:
        attacks |= generate_rook_attacks(square, board->bitboards[AP]) & ~board->bitboards[wA];
        attacks |= generate_bishop_attacks(square, board->bitboards[AP]) & ~board->bitboards[wA];
        break;
    case bQ:
        attacks |= generate_rook_attacks(square, board->bitboards[AP]) & ~board->bitboards[bA];
        attacks |= generate_bishop_attacks(square, board->bitboards[AP]) & ~board->bitboards[bA];
        break;
    case wK:
        attacks |= king_attack_table[square] & ~board->bitboards[wA];
        //White castles king side
        if(board->castle & 0b1000){
            //TODO: INSTEAD OF CHECKING SQUARES ONE BY ON CHECK THEM ALL AT ONCE USING BITMAP
            bool can_castle = !(GET_BIT(board->bitboards[AP], F1)) &&
                              !(GET_BIT(board->bitboards[AP], G1)) && 
                              !is_square_attacked(E1, BLACK_P, board->bitboards) &&
                              !is_square_attacked(F1, BLACK_P, board->bitboards) &&
                              !is_square_attacked(G1, BLACK_P, board->bitboards);
            if(can_castle){
                SET_BIT(attacks, G1);
            }
        }
        //White castles queen side
        if(board->castle & 0b0100){
            bool can_castle = !(GET_BIT(board->bitboards[AP], D1)) && 
                              !(GET_BIT(board->bitboards[AP], C1)) && 
                              !(GET_BIT(board->bitboards[AP], B1)) &&
                              !is_square_attacked(E1, BLACK_P, board->bitboards) &&
                              !is_square_attacked(D1, BLACK_P, board->bitboards) &&
                              !is_square_attacked(C1, BLACK_P, board->bitboards);
            if(can_castle){
                SET_BIT(attacks, C1);
            }
        }
        break;
    case bK:
        attacks |= king_attack_table[square] & ~board->bitboards[bA];
        //Black castles king side
        if(board->castle & 0b0010){
            bool can_castle = !(GET_BIT(board->bitboards[AP], F8)) &&
                              !(GET_BIT(board->bitboards[AP], G8)) && 
                              !is_square_attacked(E8, WHITE_P, board->bitboards) &&
                              !is_square_attacked(F8, WHITE_P, board->bitboards) &&
                              !is_square_attacked(G8, WHITE_P, board->bitboards);
            if(can_castle){
                SET_BIT(attacks, G8);
            }
        }
        if(board->castle & 0b0001){
            bool can_castle = !(GET_BIT(board->bitboards[AP], D8)) && 
                              !(GET_BIT(board->bitboards[AP], C8)) && 
                              !(GET_BIT(board->bitboards[AP], B8)) &&
                              !is_square_attacked(E8, WHITE_P, board->bitboards) &&
                              !is_square_attacked(D8, WHITE_P, board->bitboards) &&
                              !is_square_attacked(C8, WHITE_P, board->bitboards);
            if(can_castle){
                SET_BIT(attacks, C8);
            }
        }
        break;
    default:
        break;
    }
    return attacks;
}

void DrawAttacks(Board *board){
    U64 attacks = GetAttacks(board);
    while(attacks){
        int attack_square = get_first_1bit(attacks);
        if(SET_BIT_NUM(0, attack_square) & board->bitboards[bA + board->side]){
            Vector2 ring_center = {attack_square % FILES * TILESIZE + TILESIZE / 2, attack_square / RANKS * TILESIZE + TILESIZE / 2};
            DrawRing(ring_center, 43, 47, 0, 360, 1, LIGHTGRAY);
        }
        else{
            DrawCircle(attack_square % FILES * TILESIZE + TILESIZE / 2, attack_square / RANKS * TILESIZE + TILESIZE / 2, 10, LIGHTGRAY);
        }
        REMOVE_BIT(attacks, attack_square);
    }
}

int ActivateSquare(Board *board, Vector2 *coordinates){
    int square = GET_SQUARE(coordinates->x, coordinates->y);
    /*If no square is active at the moment*/
    if (NO_SQ == board->piece_selected)
    {
        /*If the clicked square is empty do nothing*/
        if (NULL == board->pieces[square]) return 0;
        /*If the clicked square has an enemy piece*/
        if (1ULL << square & board->bitboards[bA + board->side]) return 0;
        /*If not activate that square*/
        board->squares[square].active = true;
        /*Update the selected piece variable*/
        board->piece_selected = square;
        return 0;
    }
    /*If the same square with a piece is clicked again*/
    else if(square == board->piece_selected){
        /*Deactivate the square*/
        board->squares[board->piece_selected].active = false;
        /*Update the selected piece variable*/
        board->piece_selected = NO_SQ;
        return 0;
    }
    /*If a different square is clicked*/
    else if(square != board->piece_selected){
        U64 pointer = (1ULL << square);
        int friendly = board->side ? bA : wA; 
        //If the square is friendly piece
        if(pointer & board->bitboards[friendly]){
            /*Deactivate the square*/
            board->squares[board->piece_selected].active = false;
            /*Update the selected piece variable*/
            board->piece_selected = square;
            /*Activate the new square*/
            board->squares[board->piece_selected].active = true;
            return 0;
        }
        //If the square is not friendly
        
        //getting possible moves
        U64 attacks = GetAttacks(board);

        //if clicked square is not within posibble moves
        if(!(attacks & pointer)) return 0;
        
        //Copy the board state
        Board board_copy;
        memcpy(&board_copy, board, sizeof(Board));

        MakeMove(board, square, -1);
        if(is_square_attacked(get_first_1bit(board->bitboards[wK+((1 ^ board->side)*6)]), board->side, board->bitboards)){
            PlaySound(*board->sound_effects[ILLEGAL_SOUND]);
            memcpy(board, &board_copy, sizeof(Board));
            return 0;
        }

        
        //if the move is a promotion
        bool white_promotion = board->pieces[board->piece_selected]->type == wP && A8 <= square && square <= H8;
        bool black_promotion = board->pieces[board->piece_selected]->type == bP && A1 <= square && square <= H1;
        if(white_promotion || black_promotion){
            //Restore the board state
            memcpy(board, &board_copy, sizeof(Board));
            board->promotion_square = square;
            return 0;
        }

        return 1;
    }
    return 0;
}

void MakeMove(Board *board, int target_square, int promotion){
        
    U64 bit_rep = (1ULL << target_square);
        //If the clicked square has an enemy piece
        if(bit_rep & board->bitboards[bA + board->side]){
            board->bitboards[bA + board->side] ^= bit_rep;
            board->bitboards[board->pieces[target_square]->type] ^= bit_rep;
        }
        //If it is an enpassant capture
        else if(board->enpassant == target_square){
            if (board->side == WHITE_P){
                REMOVE_BIT(board->bitboards[bP], target_square + 8);
                REMOVE_BIT(board->bitboards[bA], target_square + 8);
            }
            else
            {
                REMOVE_BIT(board->bitboards[wP], target_square - 8);
                REMOVE_BIT(board->bitboards[wA], target_square - 8);
            }
        }
        
        board->previous_enpassant = board->enpassant;

        //If the move is a pawn double push ??
        if(board->pieces[board->piece_selected]->type == wP &&
           A2 <= board->piece_selected && 
           board->piece_selected <= H2 &&
           target_square == board->piece_selected - 16)
        {
           board->enpassant = target_square + 8;
        }
        else if(board->pieces[board->piece_selected]->type == bP &&
                A7 <= board->piece_selected &&
                board->piece_selected <= H7 &&
                target_square == board->piece_selected + 16)
        {
            board->enpassant = target_square - 8;
        }
        else{
            board->enpassant = NO_SQ;
        }
        
        //If the move is a castle
        if(board->pieces[board->piece_selected]->type == wK){
            if(target_square == G1){
                SET_BIT(board->bitboards[wR], F1);
                SET_BIT(board->bitboards[wA], F1);
                REMOVE_BIT(board->bitboards[wR], H1);
                REMOVE_BIT(board->bitboards[wA], H1);
            }
            else if(target_square == C1){
                SET_BIT(board->bitboards[wR], D1);
                SET_BIT(board->bitboards[wA], D1);
                REMOVE_BIT(board->bitboards[wR], A1);
                REMOVE_BIT(board->bitboards[wA], A1);
            }
        }
        else if(board->pieces[board->piece_selected]->type == bK){
            if(target_square == G8){
                SET_BIT(board->bitboards[bR], F8);
                SET_BIT(board->bitboards[bA], F8);
                REMOVE_BIT(board->bitboards[bR], H8);
                REMOVE_BIT(board->bitboards[bA], H8);
            }
            else if(target_square == C8){
                SET_BIT(board->bitboards[bR], D8);
                SET_BIT(board->bitboards[bA], D8);
                REMOVE_BIT(board->bitboards[bR], A8);
                REMOVE_BIT(board->bitboards[bA], A8);
            }
        }

        //Updating the castling rights
        board->castle &= castling_rights[target_square];
        board->castle &= castling_rights[board->piece_selected];

        //Updating the bitboards
        
        //If the move is a promotion
        if(promotion != -1){
            SET_BIT(board->bitboards[promotion], target_square);
        }
        else{
            SET_BIT(board->bitboards[board->pieces[board->piece_selected]->type], target_square);
        }
        REMOVE_BIT(board->bitboards[board->pieces[board->piece_selected]->type], board->piece_selected);

        board->bitboards[bA + (1^board->side)] ^= (1ULL << board->piece_selected);
        board->bitboards[bA + (1^board->side)] |= bit_rep; 

        board->bitboards[AP] = board->bitboards[wA] | board->bitboards[bA];
       
        board->side ^= 1;

        return;
}

void HandleTextures(Board *board, int square, int promotion){
        
        //Updating the textures
        
        //if the move is a capture
        if(board->pieces[square] != NULL){
            free(board->pieces[square]);
            board->pieces[square] = NULL;
            //Check if it is a check 
            if(is_square_attacked(get_first_1bit(board->bitboards[wK+((board->side)*6)]), 1^board->side, board->bitboards)){
                PlaySound(*board->sound_effects[CHECK_SOUND]);
            }
            else {PlaySound(*board->sound_effects[CAPTURE_SOUND]);}
        }
        //if the move is an enpassant
        else if(board->previous_enpassant == square){
            if(board->side == BLACK_P){
                free(board->pieces[square + 8]);
                board->pieces[square + 8] = NULL;
            }
            else{
                free(board->pieces[square - 8]);
                board->pieces[square - 8] = NULL;
            }
            PlaySound(*board->sound_effects[CAPTURE_SOUND]);
        }
        //if the move is a castle move
        else if (board->pieces[board->piece_selected]->type == wK){
            if(square == G1){
                board->pieces[F1] = board->pieces[H1];
                board->pieces[H1] = NULL;
                PlaySound(*board->sound_effects[CASTLE_SOUND]);
            }
            else if(square == C1){
                board->pieces[D1] = board->pieces[A1];
                board->pieces[A1] = NULL;
                PlaySound(*board->sound_effects[CASTLE_SOUND]);
            }
            //Check if it is a check
            if(is_square_attacked(get_first_1bit(board->bitboards[wK+((board->side)*6)]), 1^board->side, board->bitboards)){
                PlaySound(*board->sound_effects[CHECK_SOUND]);
            }
            else {PlaySound(*board->sound_effects[MOVE_SOUND]);}
        }
        else if(board->pieces[board->piece_selected]->type == bK){
            if(square == G8){
                board->pieces[F8] = board->pieces[H8];
                board->pieces[H8] = NULL;
                PlaySound(*board->sound_effects[CASTLE_SOUND]);
            }
            else if(square == C8){
                board->pieces[D8] = board->pieces[A8];
                board->pieces[A8] = NULL;
                PlaySound(*board->sound_effects[CASTLE_SOUND]);
            }
             //Check if it is a check
            if(is_square_attacked(get_first_1bit(board->bitboards[wK+((board->side)*6)]), 1^board->side, board->bitboards)){
                PlaySound(*board->sound_effects[CHECK_SOUND]);
            }
            else {PlaySound(*board->sound_effects[MOVE_SOUND]);}
        }
        else{
            //Check if it is a check
            if(is_square_attacked(get_first_1bit(board->bitboards[wK+((board->side)*6)]), 1^board->side, board->bitboards)){
                PlaySound(*board->sound_effects[CHECK_SOUND]);
            }
            else {PlaySound(*board->sound_effects[MOVE_SOUND]);}
        }

        //If the move is a promotion
        if(promotion != -1){
            PlaySound(*board->sound_effects[PROMOTION_SOUND]);
            board->pieces[board->piece_selected]->type = promotion;
            printf("Promotion: %d\n", promotion);
            switch (promotion)
            {
            case wQ:
                board->pieces[board->piece_selected]->texture = &white_queen_texture;
                break;
            case wR:
                board->pieces[board->piece_selected]->texture = &white_rook_texture;
                break;
            case wB:
                board->pieces[board->piece_selected]->texture = &white_bishop_texture;
                break;
            case wN:
                board->pieces[board->piece_selected]->texture = &white_knight_texture;
                break;
            case bQ:
                board->pieces[board->piece_selected]->texture = &black_queen_texture;
                break;
            case bR:
                board->pieces[board->piece_selected]->texture = &black_rook_texture;
                break;
            case bB:
                board->pieces[board->piece_selected]->texture = &black_bishop_texture;
                break;
            case bN:
                board->pieces[board->piece_selected]->texture = &black_knight_texture;
                break;
            }
        }

        board->pieces[square] = board->pieces[board->piece_selected];
        board->pieces[board->piece_selected] = NULL;
        
        //Updating the board information
        board->squares[board->piece_selected].active = false;
        board->piece_selected = NO_SQ;
        return;
}

void DrawPromotionMenu(Board *board, int side){
    int origin_x = board->squares[board->promotion_square].position.x;
    int origin_y;
    if(WHITE_P == side){
        origin_y = 0;
        DrawRectangle(origin_x * TILESIZE, origin_y * TILESIZE, TILESIZE, 4*TILESIZE, WHITE);
        DrawRectangleLines(origin_x * TILESIZE, origin_y * TILESIZE, TILESIZE, 4*TILESIZE, BLACK);
        DrawTexture(white_queen_texture, origin_x * TILESIZE, origin_y * TILESIZE, WHITE);
        DrawTexture(white_rook_texture, origin_x * TILESIZE, (origin_y + 1) * TILESIZE, WHITE);
        DrawTexture(white_bishop_texture, origin_x * TILESIZE, (origin_y + 2) * TILESIZE, WHITE);
        DrawTexture(white_knight_texture, origin_x * TILESIZE, (origin_y + 3) * TILESIZE, WHITE);
    }
    else if(BLACK_P == side){
        origin_y = 7;
        DrawRectangle(origin_x * TILESIZE, (origin_y-3) * TILESIZE, TILESIZE, 4*TILESIZE, WHITE);
        DrawRectangleLines(origin_x * TILESIZE, origin_y * TILESIZE, TILESIZE, 4*TILESIZE, WHITE);
        DrawTexture(black_queen_texture, origin_x * TILESIZE, origin_y * TILESIZE, WHITE);
        DrawTexture(black_rook_texture, origin_x * TILESIZE, (origin_y - 1) * TILESIZE, WHITE);
        DrawTexture(black_bishop_texture, origin_x * TILESIZE, (origin_y - 2) * TILESIZE, WHITE);
        DrawTexture(black_knight_texture, origin_x * TILESIZE, (origin_y - 3) * TILESIZE, WHITE);
    }
}