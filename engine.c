#include "engine.h"
#include <string.h>

U64 pawn_attack_table[2][64];
U64 knight_attack_table[64];
U64 king_attack_table[64];

U64 bishopBlackMasks[64];
U64 rookBlackMasks[64];

U64 lookup_table[87988];

MagicTable RookBlackMagic[64];
MagicTable BishopBlackMagic[64];

Move MoveList = { {0}, 0 };

/*Index to square map*/
const char* CTSM[64] = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
};

/*Promoted pieces to piece char array*/
char promoted_pieces[] = {
    [wQ] = 'q',
    [wR] = 'r',
    [wB] = 'b',
    [wN] = 'n',
    [bQ] = 'q',
    [bR] = 'r',
    [bB] = 'b',
    [bN] = 'n'
};

/*Piece char array to promoted pieces*/
char pieces[] = {
    [wP] = 'P',
    [wR] = 'R',
    [wN] = 'N',
    [wB] = 'B',
    [wQ] = 'Q',
    [wK] = 'K',
    [bP] = 'p',
    [bR] = 'r',
    [bN] = 'n',
    [bB] = 'b',
    [bQ] = 'q',
    [bK] = 'k'
};

const int bishop_rel_bits[64] = {
        6, 5, 5, 5, 5, 5, 5, 6,
        5, 5, 5, 5, 5, 5, 5, 5,
        7, 7, 7, 7, 5, 5, 5, 5,
        7, 9, 9, 7, 5, 5, 5, 5,
        7, 9, 9, 7, 5, 5, 5, 5,
        7, 7, 7, 7, 5, 5, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
        6, 5, 5, 5, 5, 5, 5, 6
    };

const int rook_rel_bits[64] = {
        12, 11, 11, 11, 11, 11, 11, 12,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        12, 11, 11, 11, 11, 11, 11, 12
    };

static MagicTable BishopMagicTable[64] = {
  { 0xa7020080601803d8ull, 60984, 18428694421974023679ull },
  { 0x13802040400801f1ull, 66046, 18446673567257459711ull },
  { 0x0a0080181001f60cull, 32910, 18446743798293722623ull },
  { 0x1840802004238008ull, 16369, 18446744072633576447ull },
  { 0xc03fe00100000000ull, 42115, 18446744073671530495ull },
  { 0x24c00bffff400000ull,   835, 18446744065051963391ull },
  { 0x0808101f40007f04ull, 18910, 18446741857371152383ull },
  { 0x100808201ec00080ull, 25911, 18446176691079331839ull },
  { 0xffa2feffbfefb7ffull, 63301, 18437719247841787903ull },
  { 0x083e3ee040080801ull, 16063, 18428694421974024191ull },
  { 0xc0800080181001f8ull, 17481, 18446673567257329663ull },
  { 0x0440007fe0031000ull, 59361, 18446743798259908607ull },
  { 0x2010007ffc000000ull, 18735, 18446744063976144895ull },
  { 0x1079ffe000ff8000ull, 61249, 18446741857366966271ull },
  { 0x3c0708101f400080ull, 68938, 18446176691079348223ull },
  { 0x080614080fa00040ull, 61791, 18445609308449144831ull },
  { 0x7ffe7fff817fcff9ull, 21893, 18442231660775734783ull },
  { 0x7ffebfffa01027fdull, 62068, 18437719247841917951ull },
  { 0x53018080c00f4001ull, 19829, 18428694421940729343ull },
  { 0x407e0001000ffb8aull, 26091, 18446673558600936447ull },
  { 0x201fe000fff80010ull, 15815, 18446741581957421055ull },
  { 0xffdfefffde39ffefull, 16419, 18446176690007683071ull },
  { 0xcc8808000fbf8002ull, 59777, 18445609308453330943ull },
  { 0x7ff7fbfff8203fffull, 16288, 18444474543197110271ull },
  { 0x8800013e8300c030ull, 33235, 18444487867259288575ull },
  { 0x0420009701806018ull, 15459, 18442231660809025535ull },
  { 0x7ffeff7f7f01f7fdull, 15863, 18437719239318433791ull },
  { 0x8700303010c0c006ull, 75555, 18428692205904059903ull },
  { 0xc800181810606000ull, 79445, 18446106185164110847ull },
  { 0x20002038001c8010ull, 15917, 18445609034107058175ull },
  { 0x087ff038000fc001ull,  8512, 18444474544268767231ull },
  { 0x00080c0c00083007ull, 73069, 18442205014827982847ull },
  { 0x00000080fc82c040ull, 16078, 18445615974745634815ull },
  { 0x000000407e416020ull, 19168, 18444487875781718015ull },
  { 0x00600203f8008020ull, 11056, 18442229478797074431ull },
  { 0xd003fefe04404080ull, 62544, 18437151933931044863ull },
  { 0xa00020c018003088ull, 80477, 18427559794152570367ull },
  { 0x7fbffe700bffe800ull, 75049, 18444404311622941695ull },
  { 0x107ff00fe4000f90ull, 32947, 18442205289172170751ull },
  { 0x7f8fffcff1d007f8ull, 59172, 18437666504634789887ull },
  { 0x0000004100f88080ull, 55845, 18446181115098558463ull },
  { 0x00000020807c4040ull, 61806, 18445618156487565311ull },
  { 0x00000041018700c0ull, 73601, 18443929280722223103ull },
  { 0x0010000080fc4080ull, 15546, 18441114487701372927ull },
  { 0x1000003c80180030ull, 45243, 18435484901701451775ull },
  { 0xc10000df80280050ull, 20333, 18424225731840835071ull },
  { 0xffffffbfeff80fdcull, 33402, 18437736736746896383ull },
  { 0x000000101003f812ull, 25917, 18428729399784241151ull },
  { 0x0800001f40808200ull, 32875, 18446741857371152383ull },
  { 0x084000101f3fd208ull,  4639, 18446739641032753151ull },
  { 0x080000000f808081ull, 17077, 18446733009332731903ull },
  { 0x0004000008003f80ull, 62324, 18446721936374366207ull },
  { 0x08000001001fe040ull, 18159, 18446699801153110015ull },
  { 0x72dd000040900a00ull, 61436, 18446656078352351231ull },
  { 0xfffffeffbfeff81dull, 57073, 18446708820483505663ull },
  { 0xcd8000200febf209ull, 61025, 18446673567257459711ull },
  { 0x100000101ec10082ull, 81259, 18446176691079331839ull },
  { 0x7fbaffffefe0c02full, 64083, 18445609308449144831ull },
  { 0x7f83fffffff07f7full, 56114, 18443911593243705343ull },
  { 0xfff1fffffff7ffc1ull, 57058, 18441076915902087167ull },
  { 0x0878040000ffe01full, 58912, 18435410299260502015ull },
  { 0x945e388000801012ull, 22194, 18424217262266253311ull },
  { 0x0840800080200fdaull, 70880, 18437719247841787903ull },
  { 0x100000c05f582008ull, 11140, 18428694421974023679ull }
};

static MagicTable RookMagicTable[64] = {
  { 0x80280013ff84ffffull, 10890, 18446461494909402753ull },
  { 0x5ffbfefdfef67fffull, 50579, 18446178916109254019ull },
  { 0xffeffaffeffdffffull, 62020, 18445613758508956549ull },
  { 0x003000900300008aull, 67322, 18444483443308361609ull },
  { 0x0050028010500023ull, 80251, 18442222812907171729ull },
  { 0x0020012120a00020ull, 58503, 18437701552104791969ull },
  { 0x0030006000c00030ull, 51175, 18428659030500032449ull },
  { 0x0058005806b00002ull, 83130, 18410573987290513281ull },
  { 0x7fbff7fbfbeafffcull, 50430, 18446461494909370879ull },
  { 0x0000140081050002ull, 21613, 18446178916109222911ull },
  { 0x0000180043800048ull, 72625, 18445613758508926463ull },
  { 0x7fffe800021fffb8ull, 80755, 18444483443308333567ull },
  { 0xffffcffe7fcfffafull, 69753, 18442222812907147775ull },
  { 0x00001800c0180060ull, 26973, 18437701552104776191ull },
  { 0x4f8018005fd00018ull, 84972, 18428659030500033023ull },
  { 0x0000180030620018ull, 31958, 18410573987290513919ull },
  { 0x00300018010c0003ull, 69272, 18446461494901210879ull },
  { 0x0003000c0085ffffull, 48372, 18446178916101258751ull },
  { 0xfffdfff7fbfefff7ull, 65477, 18445613758501223423ull },
  { 0x7fc1ffdffc001fffull, 43972, 18444483443301152767ull },
  { 0xfffeffdffdffdfffull, 57154, 18442222812901011455ull },
  { 0x7c108007befff81full, 53521, 18437701552100728831ull },
  { 0x20408007bfe00810ull, 30534, 18428659030500163583ull },
  { 0x0400800558604100ull, 16548, 18410573987290644479ull },
  { 0x0040200010080008ull, 46407, 18446461492812250879ull },
  { 0x0010020008040004ull, 11841, 18446178914062433791ull },
  { 0xfffdfefff7fbfff7ull, 21112, 18445613756529245183ull },
  { 0xfebf7dfff8fefff9ull, 44214, 18444483441462867967ull },
  { 0xc00000ffe001ffe0ull, 57925, 18442222811330113535ull },
  { 0x4af01f00078007c3ull, 29574, 18437701551064604671ull },
  { 0xbffbfafffb683f7full, 17309, 18428659030533586943ull },
  { 0x0807f67ffa102040ull, 40143, 18410573987324067839ull },
  { 0x200008e800300030ull, 64659, 18446460958038490879ull },
  { 0x0000008780180018ull, 70469, 18446178392123244031ull },
  { 0x0000010300180018ull, 62917, 18445613251702815743ull },
  { 0x4000008180180018ull, 60997, 18444482970861959167ull },
  { 0x008080310005fffaull, 18554, 18442222409180246015ull },
  { 0x4000188100060006ull, 14385, 18437701285816819711ull },
  { 0xffffff7fffbfbfffull,     0, 18428659039089967103ull },
  { 0x0000802000200040ull, 38091, 18410573995880447999ull },
  { 0x20000202ec002800ull, 25122, 18446324055955930879ull },
  { 0xfffff9ff7cfff3ffull, 60083, 18446044775690665471ull },
  { 0x000000404b801800ull, 72209, 18445484016136879103ull },
  { 0x2000002fe03fd000ull, 67875, 18444362497029306367ull },
  { 0xffffff6ffe7fcffdull, 56290, 18442119458814160895ull },
  { 0xbff7efffbfc00fffull, 43807, 18437633382383869951ull },
  { 0x000000100800a804ull, 73365, 18428661229523288063ull },
  { 0x6054000a58005805ull, 76398, 18410576186313768959ull },
  { 0x0829000101150028ull, 20024, 18411277122820570879ull },
  { 0x00000085008a0014ull,  9513, 18411838968950554111ull },
  { 0x8000002b00408028ull, 24324, 18412399711257099263ull },
  { 0x4000002040790028ull, 22996, 18413521195870189567ull },
  { 0x7800002010288028ull, 23213, 18415764165096370175ull },
  { 0x0000001800e08018ull, 56002, 18420250103548731391ull },
  { 0xa3a80003f3a40048ull, 22809, 18429221980453453823ull },
  { 0x2003d80000500028ull, 44545, 18411136937243934719ull },
  { 0xfffff37eefefdfbeull, 36072, 9367204646130482943ull },
  { 0x40000280090013c1ull,  4750, 9511037255406190079ull },
  { 0xbf7ffeffbffaf71full,  6014, 9654587285881748479ull },
  { 0xfffdffff777b7d6eull, 36054, 9941687346832865279ull },
  { 0x48300007e8080c02ull, 78538, 10515887468735098879ull },
  { 0xafe0000fff780402ull, 28745, 11664287712539566079ull },
  { 0xee73fffbffbb77feull,  8555, 13961088200148500479ull },
  { 0x0002000308482882ull,  1009, 9331317138511593471ull }
};


void print_bitboard(U64 bitboard)
{
    int rank = 0;
    int file = 0;
    int square = 0;
    printf("    A B C D E F G H\n");
    for (; rank < 8; rank++)
    {
        printf("   -----------------\n");
        printf("%d- ", 8 - rank);
        for (file = 0; file < 8; file++)
        {
            square = rank * 8 + file;
            printf("|%d", GET_BIT(bitboard, square) ? 1 : 0);
        }
        printf("|\n");
    }
    printf("   -----------------\n");
    printf("Bitboard: %lud\n", bitboard);
}

U64 pawn_attacks(int square, int color)
{
    U64 attacks = 0UL;
    if (color == WHITE_P)
    {
        if ((SET_BIT_NUM(0UL, square) >> 7) & NOT_A_FILE)
            attacks |= (SET_BIT_NUM(0UL, square) >> 7);
        if ((SET_BIT_NUM(0UL, square) >> 9) & NOT_H_FILE)
            attacks |= (SET_BIT_NUM(0UL, square) >> 9);
    }
    else
    {
        if ((SET_BIT_NUM(0UL, square) << 7) & NOT_H_FILE)
            attacks |= (SET_BIT_NUM(0UL, square) << 7);
        if ((SET_BIT_NUM(0UL, square) << 9) & NOT_A_FILE)
            attacks |= (SET_BIT_NUM(0UL, square) << 9);
    }
    return attacks;
}

U64 knight_attacks(int square)
{

    /*Result attack bitboards*/
    U64 attacks = 0UL;

    /*Generate knight attacks 17 15 10 6*/
    if ((SET_BIT_NUM(0UL, square) >> 17 & NOT_H_FILE))
        attacks |= (SET_BIT_NUM(0UL, square) >> 17);
    if ((SET_BIT_NUM(0UL, square) >> 15 & NOT_A_FILE))
        attacks |= (SET_BIT_NUM(0UL, square) >> 15);
    if ((SET_BIT_NUM(0UL, square) >> 10 & NOT_GH_FILE))
        attacks |= (SET_BIT_NUM(0UL, square) >> 10);
    if ((SET_BIT_NUM(0UL, square) >> 6 & NOT_AB_FILE))
        attacks |= (SET_BIT_NUM(0UL, square) >> 6);
    if ((SET_BIT_NUM(0UL, square) << 17 & NOT_H_FILE))
        attacks |= (SET_BIT_NUM(0UL, square) << 17);
    if ((SET_BIT_NUM(0UL, square) << 15 & NOT_A_FILE))
        attacks |= (SET_BIT_NUM(0UL, square) << 15);
    if ((SET_BIT_NUM(0UL, square) << 10 & NOT_AB_FILE))
        attacks |= (SET_BIT_NUM(0UL, square) << 10);
    if ((SET_BIT_NUM(0UL, square) << 6 & NOT_GH_FILE))
        attacks |= (SET_BIT_NUM(0UL, square) << 6);

    return attacks;
}

U64 king_attacks(int square)
{
    /*Resulted attacks bitboard*/
    U64 attacks = 0UL;

    /*Generate king attacks*/
    if ((SET_BIT_NUM(0UL, square) >> 1) & NOT_H_FILE)
        attacks |= SET_BIT_NUM(0UL, square) >> 1;
    if ((SET_BIT_NUM(0UL, square) >> 7) & NOT_A_FILE)
        attacks |= SET_BIT_NUM(0UL, square) >> 7;
    if ((SET_BIT_NUM(0UL, square) >> 9) & NOT_H_FILE)
        attacks |= SET_BIT_NUM(0UL, square) >> 9;
    if (SET_BIT_NUM(0UL, square) >> 8)
        attacks |= SET_BIT_NUM(0UL, square) >> 8;

    if ((SET_BIT_NUM(0UL, square) << 1) & NOT_A_FILE)
        attacks |= SET_BIT_NUM(0UL, square) << 1;
    if ((SET_BIT_NUM(0UL, square) << 7) & NOT_H_FILE)
        attacks |= SET_BIT_NUM(0UL, square) << 7;
    if ((SET_BIT_NUM(0UL, square) << 9) & NOT_A_FILE)
        attacks |= SET_BIT_NUM(0UL, square) << 9;
    if (SET_BIT_NUM(0UL, square) << 8)
        attacks |= SET_BIT_NUM(0UL, square) << 8;

    return attacks;
}

/*Mask bishop attacks*/
static inline U64 mask_bishop_attacks(int square)
{

    /*Result attack bitboard*/
    U64 attacks = 0UL;

    /*Iterator variables*/
    int i, j;

    /*Bottom right*/
    for (i = RANK + 1, j = FILE + 1; i <= 6 && j <= 6; i++, j++)
    {
        attacks |= (1UL << (i * 8 + j));
    }
    /*Bottom right*/
    for (i = RANK - 1, j = FILE + 1; i >= 1 && j <= 6; i--, j++)
    {
        attacks |= (1UL << (i * 8 + j));
    }
    /*Bottom left*/
    for (i = RANK + 1, j = FILE - 1; i <= 6 && j >= 1; j--, i++)
    {
        attacks |= (1UL << (i * 8 + j));
    }
    /*Top left*/
    for (i = RANK - 1, j = FILE - 1; i >= 1 && j >= 1; i--, j--)
    {
        attacks |= (1UL << (i * 8 + j));
    }

    return attacks;
}

/*Function to generate rook attacks mask*/
static inline U64 mask_rook_attacks(int square)
{

    /* Result attack bitboard */
    U64 attacks = 0UL;

    /* Iterator variables */
    int i;

    /* Bottom */
    for (i = RANK + 1; i < 7; i++)
    {
        attacks |= (1UL << (FILE + i * 8));
    }

    /* Top */
    for (i = RANK - 1; i > 0; i--)
    {
        attacks |= (1UL << (FILE + i * 8));
    }

    /* Right */
    for (i = FILE + 1; i < 7; i++)
    {
        attacks |= (1UL << (RANK * 8 + i));
    }

    /* Left */
    for (i = FILE - 1; i > 0; i--)
    {
        attacks |= (1UL << (RANK * 8 + i));
    }

    /* Return attack map */
    return attacks;
}

void init_pieces_attacks()
{
    /*Initializing precalculated attack tables*/
    int i = 0;
    for (; i < 64; i++)
    {

        /*Initializing attack tables for pawns*/
        pawn_attack_table[WHITE_P][i] = pawn_attacks(i, WHITE_P);
        pawn_attack_table[BLACK_P][i] = pawn_attacks(i, BLACK_P);

        /*Initializing attack tables for kings*/
        king_attack_table[i] = king_attacks(i);

        /*Initializing attack tables for knights*/
        knight_attack_table[i] = knight_attacks(i);
    }
}

/*Generate bishop attack maps*/
static inline U64 generate_bishop_attacks(int square, U64 blockBB)
{

    /*Result attack bitboard*/
    U64 attacks = 0UL;

    /*Iterator variables*/
    int i, j;

    /*Bottom right*/
    for (i = RANK + 1, j = FILE + 1; i <= 7 && j <= 7; i++, j++)
    {
        if (1UL << (i * 8 + j) & blockBB)
        {
            attacks |= (1UL << (i * 8 + j));
            break;
        }
        attacks |= (1UL << (i * 8 + j));
    }
    /*Bottom right*/
    for (i = RANK - 1, j = FILE + 1; i >= 0 && j <= 7; i--, j++)
    {
        if (1UL << (i * 8 + j) & blockBB)
        {
            attacks |= (1UL << (i * 8 + j));
            break;
        }
        attacks |= (1UL << (i * 8 + j));
    }
    /*Bottom left*/
    for (i = RANK + 1, j = FILE - 1; i <= 7 && j >= 0; j--, i++)
    {
        if(1UL << (i*8+j) & blockBB){
            attacks |= (1UL << (i * 8 + j));
            break;
        }
        attacks |= (1UL << (i * 8 + j));
    }
    /*Top left*/
    for (i = RANK - 1, j = FILE - 1; i >= 0 && j >= 0; i--, j--)
    {
        if(1UL << (i*8+j) & blockBB){
            attacks |= (1UL << (i * 8 + j));
            break;
        }
        attacks |= (1UL << (i * 8 + j));
    }

    return attacks;
}

/*Generate rook attack maps*/
static inline U64 generate_rook_attacks(int square, U64 blockBB)
{

    /* Result attack bitboard */
    U64 attacks = 0UL;

    /* Iterator variables */
    int i;

    /* Bottom */
    for (i = RANK + 1; i < 8; i++)
    {
        if((1UL << (FILE + i * 8)) & blockBB){
            attacks |= (1UL << (FILE + i * 8));
            break;
        }
        attacks |= (1UL << (FILE + i * 8));
    }

    /* Top */
    for (i = RANK - 1; i >= 0; i--)
    {
        if((1UL << (FILE + i * 8)) & blockBB){
            attacks |= (1UL << (FILE + i * 8));
            break;
        }
        attacks |= (1UL << (FILE + i * 8));
    }

    /* Right */
    for (i = FILE + 1; i < 8; i++)
    {
        if((1UL << (RANK * 8 + i)) & blockBB){
            attacks |= (1UL << (RANK * 8 + i));
            break;
        }
        attacks |= (1UL << (RANK * 8 + i));
    }

    /* Left */
    for (i = FILE - 1; i >= 0; i--)
    {
        if((1UL << (RANK * 8 + i)) & blockBB){
            attacks |= (1UL << (RANK * 8 + i));
            break;
        }
        attacks |= (1UL << (RANK * 8 + i));
    }

    /* Return attack map */
    return attacks;
}


/*================================================*/
/*                                                */
/*Tom Romstads algorithm for finding magic numbers*/
/*                                                */
/*================================================*/



/**************************************************/
/*               Helper functions                 */
/**************************************************/

/*Get least significant first bit index*/
static inline int get_first_1bit(U64 bitboard){
    
    /*Check empty bitboards*/
    if(!bitboard) return -1;
    
    /*Count trailing bits*/
    return COUNT_BITS((bitboard & -bitboard)-1);
}

/*Check whether a square is occupied*/
int is_occupied(U64 bitboard, int square){
    return (bitboard & (1UL << square)) ? 1 : 0;
}

/**************************************************/
/*            Random Number Generators            */
/**************************************************/

/* 32-bit Pseudo random generator to find magic bits */
static inline int RNG_32B(){
 
    /*Seed for pseudo random numbers*/
    static unsigned int seed =  2156140123;

    /*XOR Shift Algorithm*/
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;   

    return seed;
}

/*Generate pseudo random 64-bit numbers*/
static U64 RNG_64B(){
    
    /*Define 4 random numbers*/
    U64 n1, n2, n3, n4;

    /*Initialize random numbers slicing 16 bits from Most Significant 1*/
    n1 = (U64)RNG_32B() & 0xFFFF;
    n2 = (U64)RNG_32B() & 0xFFFF;
    n3 = (U64)RNG_32B() & 0xFFFF;
    n4 = (U64)RNG_32B() & 0xFFFF;

    /*Return random number*/
    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

/*Generate magic candidates*/
static U64 generate_magic_number(){
    return RNG_64B() & RNG_64B() & RNG_64B(); 
 }



/*================================================*/
/*                                                */
/*  Initiliazing Attack tables for slider pieces  */
/*                                                */
/*================================================*/

/*Precalculating occupancy for attack tables bitboards*/
static U64 set_occupancy(int index, int bits, U64 attack_mask){

    /*Occupancy bitboard*/
    U64 occupancy = 0UL;
    /*Index of least significant 1*/
    int square;
    /*Iterator variable*/
    int i;

    /*Loop over the range of bits*/
    for(i = 0; i < bits; i++){
        
        /*Get the least significant 1*/
        square = get_first_1bit(attack_mask);
        
        /*Remove that bit*/
        REMOVE_BIT(attack_mask, square);

        /*Check whether the occupancy is valid*/
        if(index & (1UL << i)){
            /*Set the occupancy bit*/
            occupancy |= (1UL << square);
        }
    }
    return occupancy;
}

static U64 find_magic_number(int square, int relevant_bits, int piece) {
    int i = 0;
    int j = 0;
    int fail;
    U64 magic;
    U64 block_masks[4096];
    U64 attacks[4096];
    U64 used[4096];
    U64 attack_mask = piece ? mask_bishop_attacks(square) : mask_rook_attacks(square);
    int occupancy_indices = 1 << relevant_bits;

    for (i = 0; i < occupancy_indices; i++) {
        block_masks[i] = set_occupancy(i, relevant_bits, attack_mask);
        attacks[i] = piece ? generate_bishop_attacks(square, block_masks[i]) :
                             generate_rook_attacks(square, block_masks[i]);
    }

    for (j = 0; j < 100000000; j++) {
        magic = generate_magic_number();
        if (COUNT_BITS((attack_mask * magic) & 0xFF00000000000000ULL) < 6)
            continue;

        memset(used, 0, sizeof(used));

        for (i = 0, fail = 0; !fail && i < occupancy_indices; i++) {
            int magic_index = (int)((block_masks[i] * magic) >> (64 - relevant_bits));
            if (used[magic_index] == 0UL)
                used[magic_index] = attacks[i];
            else if (used[magic_index] != attacks[i])
                fail = 1;
        }

        if (!fail)
            return magic;
    }

    return 0UL;
}

/**
 * Initializes precalculated slider attack tables for rooks and bishops.
 * This function precomputes all possible attack patterns for rooks and bishops
 * based on the occupancy of other pieces on the board. It uses magic bitboards
 * to efficiently generate and store these patterns in a lookup table.
 */
void init_slider_attacks() {
    int sq, index;

    // Iterate over all squares on the chessboard
    for (sq = 0; sq < 64; sq++) {
        // Calculate the number of occupancy variations for rooks and bishops on the square
        int occupancy_bitsR = 1 << rook_rel_bits[sq]; // Number of variations for rooks
        int occupancy_bitsB = 1 << bishop_rel_bits[sq]; // Number of variations for bishops

        // Generate and store attack patterns for rooks
        for (index = 0; index < occupancy_bitsR; index++) {
            // Set occupancy based on index, count of bits, and mask
            U64 occ = set_occupancy(index, COUNT_BITS(~RookMagicTable[sq].blackmask), ~RookMagicTable[sq].blackmask);
            // Calculate the magic index for the rook's position and occupancy
            int magic_index = (((occ | RookMagicTable[sq].blackmask)) * RookMagicTable[sq].magic) >> (64 - 12);
            // Store the generated attack pattern in the lookup table
            lookup_table[RookMagicTable[sq].index + magic_index] = generate_rook_attacks(sq, occ);
        }

        // Generate and store attack patterns for bishops
        for (index = 0; index < occupancy_bitsB; index++) {
            // Set occupancy based on index, count of bits, and mask
            U64 occ = set_occupancy(index, COUNT_BITS(~BishopMagicTable[sq].blackmask), ~BishopMagicTable[sq].blackmask);
            // Calculate the magic index for the bishop's position and occupancy
            int magic_index = (((occ | BishopMagicTable[sq].blackmask)) * BishopMagicTable[sq].magic) >> (64 - 9);
            // Store the generated attack pattern in the lookup table
            lookup_table[BishopMagicTable[sq].index + magic_index] = generate_bishop_attacks(sq, occ);
        }
    }
}


/*================================================*/
/*                                                */
/*  Getting slider attacks from the lookup table  */
/*                                                */
/*================================================*/

/**
 * Calculates the attack bitboard for a rook on a given square.
 * 
 * @param square The square where the rook is located.
 * @param blockBB The bitboard representing the blocking pieces on the board.
 * @return The attack bitboard for the rook on the given square.
 */
static inline U64 rook_attack(int square, U64 blockBB){
    int magic_index = (int)(((blockBB | RookMagicTable[square].blackmask) * RookMagicTable[square].magic) >> (64 - 12));
    return lookup_table[RookMagicTable[square].index + magic_index];
}

/**
 * Calculates the bishop attack bitboard for a given square and blocking bitboard.
 *
 * @param square The square index of the bishop.
 * @param blockBB The blocking bitboard representing occupied squares.
 * @return The bishop attack bitboard.
 */
static inline U64 bishop_attack(int square, U64 blockBB) {
    int magic_index = (int)(((blockBB | BishopMagicTable[square].blackmask) * BishopMagicTable[square].magic) >> (64 - 9));
    return lookup_table[BishopMagicTable[square].index + magic_index];
}

static inline U64 queen_attack(int square, U64 blockBB){
    return rook_attack(square, blockBB) | bishop_attack(square, blockBB);
}


void restart_game(Game* game){
    
    /*Restarting the state flags of the game*/
    game->side = WHITE_P;
    game->enpassant = NO_SQ;
    game->castle = 0b1111;
    game->half_moves = 0;
    game->full_moves = 0;
    
    /*Restarting the bitboards*/
    game->bitboards[wP] = 0x00FF000000000000;
    game->bitboards[wN] = 0x4200000000000000;
    game->bitboards[wB] = 0x2400000000000000;
    game->bitboards[wR] = 0x8100000000000000;
    game->bitboards[wQ] = 0x0800000000000000;
    game->bitboards[wK] = 0x1000000000000000;
    
    game->bitboards[bP] = 0x000000000000FF00;
    game->bitboards[bN] = 0x0000000000000042;
    game->bitboards[bB] = 0x0000000000000024;
    game->bitboards[bR] = 0x0000000000000081;
    game->bitboards[bQ] = 0x0000000000000008;
    game->bitboards[bK] = 0x0000000000000010;

    game->bitboards[wA] = 18446462598732840960ULL;
    game->bitboards[bA] = 65535ULL;

    game->bitboards[AP] = 18446462598732906495ULL;
    
}

/*================================================*/
/*                                                */
/*         Generating moves for pieces            */
/*                                                */
/*================================================*/

static inline int is_square_attacked(int square, int side, U64 bitboards[]){
    /*Keeping track of the state*/
    U64 is_attacked = 0;

    /*Checking pawns*/
    is_attacked = (side == WHITE_P) ? (pawn_attack_table[BLACK_P][square] & bitboards[wP]) : (pawn_attack_table[WHITE_P][square] & bitboards[bP]);
    if(is_attacked) return 1;
    
    /*Cheking knights*/
    is_attacked = knight_attack_table[square] & ((side == WHITE_P) ? bitboards[wN] : bitboards[bN]);
    if(is_attacked) return 1;

    /*Checking bishops*/
    is_attacked = bishop_attack(square, bitboards[AP]) & ((side == WHITE_P) ? bitboards[wB] : bitboards[bB]);
    if(is_attacked) return 1;

    /*Checking rooks*/
    is_attacked = rook_attack(square, bitboards[AP]) & ((side == WHITE_P) ? bitboards[wR] : bitboards[bR]);
    if(is_attacked) return 1;

    /*Checking queens*/
    is_attacked = queen_attack(square, bitboards[AP]) & ((side == WHITE_P) ? bitboards[wQ] : bitboards[bQ]);
    if(is_attacked) return 1;

    /*Checking kings*/
    is_attacked = king_attack_table[square] & ((side == WHITE_P) ? bitboards[wK] : bitboards[bK]);
    if(is_attacked) return 1;

    return is_attacked;
}

static inline U64 squares_attacked(int side, U64 bitboards[]){
    U64 attacks = 0UL;
    int square = 0;
    for(; square < 64; square++){
        if(is_square_attacked(square, side, bitboards)){
            SET_BIT(attacks, square);
        }
    }
    return attacks;
}

// Inline helper functions

/**
 * Prints a regular move from one square to another.
 * @param from The starting square.
 * @param to The target square.
 */
inline static void print_move(const char* from, const char* to) {
    printf("%s%s\n", from, to);
}

/**
 * Prints a capture move from one square to another.
 * @param from The starting square.
 * @param to The target square.
 */
inline static void print_capture(const char* from, const char* to) {
    printf("%sx%s\n", from, to);
}

/**
 * Prints a promotion move from one square to another with a specified promotion piece.
 * @param from The starting square.
 * @param to The target square.
 * @param promotion The promotion piece.
 */
inline static void print_promotion(const char* from, const char* to, char promotion) {
    printf("%s%s%c\n", from, to, promotion);
}

/**
 * Handles pawn moves for a given game state.
 * @param game The game state.
 * @param source_sq The source square of the pawn.
 * @param direction The direction of the pawn's movement.
 * @param startRow The starting row of the pawns.
 * @param promoRow The promotion row of the pawns.
 * @param enemyPieces The bitboard representing the enemy pieces.
 */
inline static void handle_pawn_moves(Game* game, int source_sq, int direction, int startRow, int promoRow, U64 enemyPieces) {
    int target_sq = source_sq + direction;
    int bias = (int)((float)startRow*(-3/(float)20)+7.3);
    // Single pawn push
    if (!GET_BIT(game->bitboards[AP], target_sq)) {
        // Double pawn push
        if ((startRow <= source_sq && source_sq <= startRow + 7) && !GET_BIT(game->bitboards[AP], target_sq + direction)) {
            addMove(&MoveList, ENCODE_MOVE(source_sq, (target_sq + direction), (wP+bias), 0, 0, 1, 0, 0));
        }
        // Promotion
        if ((promoRow <= source_sq && source_sq <= promoRow + 7)) {
            addMove(&MoveList, ENCODE_MOVE(source_sq, target_sq, (wP+bias), (wQ+bias), 0, 0, 0, 0));
            addMove(&MoveList, ENCODE_MOVE(source_sq, target_sq, (wP+bias), (wR+bias), 0, 0, 0, 0));
            addMove(&MoveList, ENCODE_MOVE(source_sq, target_sq, (wP+bias), (wN+bias), 0, 0, 0, 0));
            addMove(&MoveList, ENCODE_MOVE(source_sq, target_sq, (wP+bias), (wB+bias), 0, 0, 0, 0));
        } else {
            addMove(&MoveList, ENCODE_MOVE(source_sq, target_sq, (wP+bias), 0, 0, 0, 0, 0));
        }
    }

    // Pawn captures
    U64 attacks = pawn_attack_table[game->side][source_sq] & game->bitboards[enemyPieces];
    while (attacks) {
        target_sq = get_first_1bit(attacks);
        if ((promoRow <= source_sq && source_sq <= promoRow + 7)) {
            addMove(&MoveList, ENCODE_MOVE(source_sq, target_sq, (wP+bias), (wQ+bias), 1, 0, 0, 0));
            addMove(&MoveList, ENCODE_MOVE(source_sq, target_sq, (wP+bias), (wR+bias), 1, 0, 0, 0));
            addMove(&MoveList, ENCODE_MOVE(source_sq, target_sq, (wP+bias), (wN+bias), 1, 0, 0, 0));
            addMove(&MoveList, ENCODE_MOVE(source_sq, target_sq, (wP+bias), (wB+bias), 1, 0, 0, 0));
        } else {
            addMove(&MoveList, ENCODE_MOVE(source_sq, target_sq, (wP+bias), 0, 1, 0, 0, 0));
        }
        REMOVE_BIT(attacks, target_sq);
    }
}

/**
 * Handles en passant captures for a given game state.
 * @param game The game state.
 * @param piece The piece type.
 */
inline static void handle_en_passant(Game* game, int piece) {
    if ((game->enpassant) != NO_SQ) {
        U64 attacks = pawn_attack_table[(game->side == WHITE_P) ? BLACK_P : WHITE_P][game->enpassant] & game->bitboards[piece];
        while (attacks) {
            int source_sq = get_first_1bit(attacks);
            addMove(&MoveList, ENCODE_MOVE(source_sq, game->enpassant, piece, 0, 1, 0, 1, 0));
            REMOVE_BIT(attacks, source_sq);
        }
    }
}

/**
 * Handles castling moves for a given game state.
 * @param game The game state.
 */
inline static void handle_castling(Game* game) {
    if (game->side == WHITE_P) {
        if ((game->castle & WKC) && !GET_BIT(game->bitboards[AP], F1) && !GET_BIT(game->bitboards[AP], G1)) {
            if (!is_square_attacked(E1, BLACK_P, game->bitboards) && !is_square_attacked(F1, BLACK_P, game->bitboards) && !is_square_attacked(G1, BLACK_P, game->bitboards)) {
                addMove(&MoveList, ENCODE_MOVE(E1, G1, wK, 0, 0, 0, 0, 1));
            }
        }
        if ((game->castle & WQC) && !GET_BIT(game->bitboards[AP], D1) && !GET_BIT(game->bitboards[AP], C1) && !GET_BIT(game->bitboards[AP], B1)) {
            if (!is_square_attacked(E1, BLACK_P, game->bitboards) && !is_square_attacked(D1, BLACK_P, game->bitboards) && !is_square_attacked(C1, BLACK_P, game->bitboards)) {
                addMove(&MoveList, ENCODE_MOVE(E1, C1, wK, 0, 0, 0, 0, 1));
            }
        }
    } else {
        if ((game->castle & BKC) && !GET_BIT(game->bitboards[AP], F8) && !GET_BIT(game->bitboards[AP], G8)) {
            if (!is_square_attacked(E8, WHITE_P, game->bitboards) && !is_square_attacked(F8, WHITE_P, game->bitboards) && !is_square_attacked(G8, WHITE_P, game->bitboards)) {
                addMove(&MoveList, ENCODE_MOVE(E8, G8, bK, 0, 0, 0, 0, 1));
            }
        }
        if ((game->castle & BQC) && !GET_BIT(game->bitboards[AP], D8) && !GET_BIT(game->bitboards[AP], C8) && !GET_BIT(game->bitboards[AP], B8)) {
            if (!is_square_attacked(E8, WHITE_P, game->bitboards) && !is_square_attacked(D8, WHITE_P, game->bitboards) && !is_square_attacked(C8, WHITE_P, game->bitboards)) {
                addMove(&MoveList, ENCODE_MOVE(E8, C8, bK, 0, 0, 0, 0, 1));
            }
        }
    }
}


/**
 * Generates all possible moves for a given game state.
 * @param game The game state.
 */
inline void GenerateMoves(Game* game) {
    int source_sq, target_sq;
    int move = 0;
    U64 attacks = 0UL;
    U64 temp_bb;
    int direction = (game->side == WHITE_P) ? -8 : 8;
    int startRow = (game->side == WHITE_P) ? A2 : A7;
    int promoRow = (game->side == WHITE_P) ? A7 : A2;
    int enemyPieces = (game->side == WHITE_P) ? bA : wA;
    int friendlyPieces = (game->side == WHITE_P) ? wA : bA;
    int start_piece = (game->side == WHITE_P) ? wP : bP;
    int end_piece = (game->side == WHITE_P) ? wK : bK;

    // Iterate through each piece type
    for (int piece = start_piece; piece <= end_piece; piece++) {
        temp_bb = game->bitboards[piece];

        // Iterate through each square with the current piece
        while ((source_sq = get_first_1bit(temp_bb)) != NO_SQ) {
            switch (piece) {
                case wP:
                case bP:
                    handle_pawn_moves(game, source_sq, direction, startRow, promoRow, enemyPieces);
                    break;

                case wN:
                case bN:
                    attacks = knight_attack_table[source_sq] & ~game->bitboards[game->side];
                    break;

                case wB:
                case bB:
                    attacks = bishop_attack(source_sq, game->bitboards[AP]) & ~game->bitboards[game->side];
                    break;

                case wR:
                case bR:
                    attacks = rook_attack(source_sq, game->bitboards[AP]) & ~game->bitboards[game->side];
                    break;

                case wQ:
                case bQ:
                    attacks = queen_attack(source_sq, game->bitboards[AP]) & ~game->bitboards[game->side];
                    break;

                case wK:
                case bK:
                    attacks = king_attack_table[source_sq] & ~game->bitboards[game->side];
                    handle_castling(game);
                    break;

                default:
                    break;
            }

            // Iterate through each target square and print the move
            while (attacks) {
                target_sq = get_first_1bit(attacks);
                if (GET_BIT(game->bitboards[enemyPieces], target_sq)) {
                    addMove(&MoveList, ENCODE_MOVE(source_sq, target_sq, piece, 0, 1, 0, 0, 0));
                } else if(!GET_BIT(game->bitboards[friendlyPieces], target_sq)){
                    addMove(&MoveList, ENCODE_MOVE(source_sq, target_sq, piece, 0, 0, 0, 0, 0));
                }
                REMOVE_BIT(attacks, target_sq);
            }

            // Handle en passant captures
            if (piece == wP || piece == bP) {
                handle_en_passant(game, piece);
            }

            REMOVE_BIT(temp_bb, source_sq);
        }
    }
}

/* 
Decoding the move 
        Binary Representation                    Hex Representation
    0000 0000 0000 0000 0011 1111 Source Square      0x3f
    0000 0000 0000 1111 1100 0000 Target Square      0xfc0
    0000 0000 1111 0000 0000 0000 Piece              0xf000
    0000 1111 0000 0000 0000 0000 Promoted Piece     0xf0000    
    0001 0000 0000 0000 0000 0000 Capture Flag       0x100000
    0010 0000 0000 0000 0000 0000 Double Pawn Push   0x200000
    0100 0000 0000 0000 0000 0000 EnPassant Flag     0x400000
    1000 0000 0000 0000 0000 0000 Castling Flag      0x800000
*/


/**
 * Adds a move to the MoveList.
 *
 * @param MoveList The MoveList to add the move to.
 * @param move The move to be added.
 */
 inline void addMove(Move *MoveList, int move){
    MoveList->moves[MoveList->moveCount++] = move;
}
/*For UCI*/
void printMove(int move){
    printf("%s%s%c\n",
    CTSM[GET_SOURCE_SQUARE(move)], 
    CTSM[GET_TARGET_SQUARE(move)], 
    GET_PROMOTED_PIECE(move));
}

/*For debugging*/
void printMoveList(Move *MoveList){
    if(MoveList->moveCount == 0){
        printf("\nNo moves found\n");
        return;
    }
    int index = 0;
    for(; index < MoveList->moveCount; index++){
        printf("\n Move  Piece  Capture  Double  Enpassant  Castling\n %s%s%c    %c       %d       %d        %d          %d\n\n",
        CTSM[GET_SOURCE_SQUARE(MoveList->moves[index])], 
        CTSM[GET_TARGET_SQUARE(MoveList->moves[index])], 
        promoted_pieces[GET_PROMOTED_PIECE(MoveList->moves[index])],
        pieces[GET_PIECE(MoveList->moves[index])],
        GET_CAPTURE_FLAG(MoveList->moves[index]),
        GET_DOUBLE_PP_FLAG(MoveList->moves[index]),
        GET_ENPASSANT_FLAG(MoveList->moves[index]),
        GET_CASTLE_FLAG(MoveList->moves[index]));
    }
    printf("\nTotal number of moves: %d\n", MoveList->moveCount);
}
