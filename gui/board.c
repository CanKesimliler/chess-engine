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
        break;
    case bK:
        attacks |= king_attack_table[square] & ~board->bitboards[bA];
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
        DrawCircle(attack_square % FILES * TILESIZE + TILESIZE / 2, attack_square / RANKS * TILESIZE + TILESIZE / 2, 10, GRAY);
        REMOVE_BIT(attacks, attack_square);
    }
}

void ActivateSquare(Board *board, Vector2 *coordinates){
    int square = GET_SQUARE(coordinates->x, coordinates->y);
    /*If no square is active at the moment*/
    if (NO_SQ == board->piece_selected)
    {
        /*If the clicked square is empty do nothing*/
        if (NULL == board->pieces[square]) return;
        /*If the clicked square has an enemy piece*/
        if (1ULL << square & board->bitboards[bA + board->side]) return;
        /*If not activate that square*/
        board->squares[square].active = true;
        /*Update the selected piece variable*/
        board->piece_selected = square;
        return;
    }
    /*If the same square with a piece is clicked again*/
    else if(square == board->piece_selected){
        /*Deactivate the square*/
        board->squares[board->piece_selected].active = false;
        /*Update the selected piece variable*/
        board->piece_selected = NO_SQ;
        return;
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
            return;
        }
        //If the square is not friendly
        
        //getting possible moves
        U64 attacks = GetAttacks(board);

        //if clicked square is not within posibble moves
        if(!(attacks & pointer)) return;
        
        //Copy the board state
        Board board_copy;
        memcpy(&board_copy, board, sizeof(Board));

        MakeMove(board, square);
        if(is_square_attacked(get_first_1bit(board->bitboards[wK+((1 ^ board->side)*6)]), board->side, board->bitboards)){
            puts("Illegal move");
            memcpy(board, &board_copy, sizeof(Board));
            return;
        }

        //Updating the textures
        //if the move is a capture
        if(board->pieces[square] != NULL){
            free(board->pieces[square]);
        }
        //if the move is an enpassant
        else if(board_copy.enpassant == square){
            puts("Enpassant capture");
            if(board->side == BLACK_P){
                free(board->pieces[square + 8]);
                board->pieces[square + 8] = NULL;
            }
            else{
                free(board->pieces[square - 8]);
                board->pieces[square - 8] = NULL;
            }
        }
        board->pieces[square] = board->pieces[board->piece_selected];
        board->pieces[board->piece_selected] = NULL;
        
        //Updating the board information
        board->squares[board->piece_selected].active = false;
        board->piece_selected = NO_SQ;
        return;
    }
}

void MakeMove(Board *board, int target_square){
       
    U64 bit_rep = (1ULL << target_square);
        //If the clicked square has an enemy piece
        if(bit_rep & board->bitboards[bA + board->side]){
            board->bitboards[bA + board->side] ^= bit_rep;
            board->bitboards[board->pieces[target_square]->type] ^= bit_rep;
            board->pieces[target_square] = NULL;
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

        //If the move is a pawn double push ??
        if(board->pieces[board->piece_selected]->type == wP &&
           A2 <= board->piece_selected && 
           board->piece_selected <= H2 &&
           target_square == board->piece_selected - 16){
            board->enpassant = target_square + 8;
        }
        else if(board->pieces[board->piece_selected]->type == bP &&
                A7 <= board->piece_selected &&
                board->piece_selected <= H7 &&
                target_square == board->piece_selected + 16){
            board->enpassant = target_square - 8;
        }
        else{
            board->enpassant = NO_SQ;
        }
        puts("Move made");
        printf("Enpassant square: %d\n", board->enpassant);

        //Updating the bitboards
        board->bitboards[board->pieces[board->piece_selected]->type] ^= (1ULL << board->piece_selected);
        board->bitboards[board->pieces[board->piece_selected]->type] |= bit_rep; 

        board->bitboards[bA + (1^board->side)] ^= (1ULL << board->piece_selected);
        board->bitboards[bA + (1^board->side)] |= bit_rep; 

        board->bitboards[AP] = board->bitboards[wA] | board->bitboards[bA];
       
        board->side ^= 1;

        return;
}