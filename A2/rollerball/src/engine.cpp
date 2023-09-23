#include <algorithm>
#include <random>
#include <iostream>
#include <map>

#include "board.hpp"
#include "engine.hpp"

int move_number = 0;

using namespace std;

//# define MAX_DEPTH 3



int MAX_DEPTH = 0;
// actual depth is this value + 1. So if MAX_DEPTH = 3, then depth = 4

constexpr U8 cw_90[64] = {
        48, 40, 32, 24, 16, 8,  0,  7,
        49, 41, 33, 25, 17, 9,  1,  15,
        50, 42, 18, 19, 20, 10, 2,  23,
        51, 43, 26, 27, 28, 11, 3,  31,
        52, 44, 34, 35, 36, 12, 4,  39,
        53, 45, 37, 29, 21, 13, 5,  47,
        54, 46, 38, 30, 22, 14, 6,  55,
        56, 57, 58, 59, 60, 61, 62, 63
};

constexpr U8 acw_90[64] = {
        6, 14, 22, 30, 38, 46, 54, 7,
        5, 13, 21, 29, 37, 45, 53, 15,
        4, 12, 18, 19, 20, 44, 52, 23,
        3, 11, 26, 27, 28, 43, 51, 31,
        2, 10, 34, 35, 36, 42, 50, 39,
        1,  9, 17, 25, 33, 41, 49, 47,
        0,  8, 16, 24, 32, 40, 48, 55,
        56, 57, 58, 59, 60, 61, 62, 63
};

constexpr U8 cw_180[64] = {
        54, 53, 52, 51, 50, 49, 48, 7,
        46, 45, 44, 43, 42, 41, 40, 15,
        38, 37, 18, 19, 20, 33, 32, 23,
        30, 29, 26, 27, 28, 25, 24, 31,
        22, 21, 34, 35, 36, 17, 16, 39,
        14, 13, 12, 11, 10,  9,  8, 47,
        6,  5,  4,  3,  2,  1,  0, 55,
        56, 57, 58, 59, 60, 61, 62, 63
};

constexpr U8 id[64] = {
        0,  1,  2,  3,  4,  5,  6,  7,
        8,  9, 10, 11, 12, 13, 14, 15,
        16, 17, 18, 19, 20, 21, 22, 23,
        24, 25, 26, 27, 28, 29, 30, 31,
        32, 33, 34, 35, 36, 37, 38, 39,
        40, 41, 42, 43, 44, 45, 46, 47,
        48, 49, 50, 51, 52, 53, 54, 55,
        56, 57, 58, 59, 60, 61, 62, 63
};

#define cw_90_pos(p) cw_90[p]
#define cw_180_pos(p) cw_180[p]
#define acw_90_pos(p) acw_90[p]
#define id_pos(p) id[p]
#define cw_90_move(m) move_promo(cw_90[getp0(m)], cw_90[getp1(m)], getpromo(m))
#define acw_90_move(m) move_promo(acw_90[getp0(m)], acw_90[getp1(m)], getpromo(m))
#define cw_180_move(p) move_promo(cw_180[getp0(m)], cw_180[getp1(m)], getpromo(m))


bool protected_check(Board b, U8 piece_pos, PlayerColor player_to_play)
{
    Board b_copy = *b.copy();

//    U8 piece = b.data.board_0[piece_pos];

    // delete this piece from b_copy
    b_copy.data.board_0[piece_pos] = EMPTY;
    b_copy.data.board_90[cw_90[piece_pos]] = EMPTY;
    b_copy.data.board_180[cw_180[piece_pos]] = EMPTY;
    b_copy.data.board_270[acw_90[piece_pos]] = EMPTY;

    b_copy.data.player_to_play = player_to_play;

    auto moveset = b_copy.get_legal_moves();

    for (auto m : moveset)
    {
        auto p1 = getp1(m);
        if (p1 == piece_pos)
        {
            return true;
        }
    }

    return false;
}

int how_many_protected_score(Board& b)
{
    // for each piece
    int score = 0;

    for (int i = 0; i < 64; i++)
    {
        if (b.data.board_0[i] == EMPTY)
            continue;
        else
        {
            if (b.data.board_0[i] == (WHITE | PAWN)) {
                if (protected_check(b, i, WHITE))
                {
                    score += 3;
                }
                else
                {
                    score -= 1;
                }
            }

            if (b.data.board_0[i] == (BLACK | PAWN)) {
                if (protected_check(b, i, BLACK))
                {
                    score -= 3;
                }
                else
                {
                    score += 1;
                }
            }

//          else if (b.data.board_0[i] == BISHOP)
//                    score += 2;
//          else if (b.data.board_0[i] == ROOK)
//                    score += 1;
//          else if (b.data.board_0[i] == KING)
//                    score += 10;
        }
    }

    return 0;
}

// similarly how many attacked

// wrong code below of count_pawn_score, uses b.data.w_pawn_ws and b.data.w_pawn_bs
int get_pawn_score_white(U8 P)
{
    map<U8, int> pawn_scores;
    pawn_scores[pos(2, 0)] = 0;
    pawn_scores[pos(2, 1)] = 0;
    for (int i = 0; i < 6; i++)
    {
        pawn_scores[pos(0, i)] = i+1;
        pawn_scores[pos(1, i)] = i+1;
    }
    pawn_scores[pos(0, 6)] = 6;
    pawn_scores[pos(1, 6)] = 6;

    for (int i = 2; i <= 4; i++)
    {
        pawn_scores[pos(i, 5)] = 5+i;
        pawn_scores[pos(i, 6)] = 5+i;
    }

    pawn_scores[pos(3, 6)] = 10;
    pawn_scores[pos(3, 5)] = 10;
    return pawn_scores[P];
}

int get_pawn_score_black(U8 P1)
{
    U8 P = cw_180_pos(P1);

    return get_pawn_score_white(P);
}

int get_bishop_score(U8 P)
{
    map<U8, int> bishop_scores;
    bishop_scores[pos(1, 0)] = 3;
    bishop_scores[pos(2, 1)] = 8;
    bishop_scores[pos(3, 0)] = 7;
    bishop_scores[pos(4, 1)] = 6;
    bishop_scores[pos(5, 0)] = 3;
//    bishop_scores[pos(6, 1)] = 3;

    if (gety(P) <= 1 && getx(P) >= 1 && getx(P) <= 5)
    {
        return bishop_scores[P];
    }

    U8 P1 = cw_90_pos(P);
    U8 P2 = cw_180_pos(P);
    U8 P3 = acw_90_pos(P);
    if (gety(P1) <= 1 && getx(P1) >= 1 && getx(P1) <= 5)
    {
        return bishop_scores[P1];
    }
    if (gety(P2) <= 1 && getx(P2) >= 1 && getx(P2) <= 5)
    {
        return bishop_scores[P2];
    }
    if (gety(P3) <= 1 && getx(P3) >= 1 && getx(P3) <= 5)
    {
        return bishop_scores[P3];
    }

    cout<<"no matching position for bishop";
    return -1;
}

// old bishop stupid no rotation code commented below
/*

 //    bishop_scores[pos(0, 1)] = 3;
//    bishop_scores[pos(1, 0)] = 3;
//    bishop_scores[pos(2, 1)] = 8;
//    bishop_scores[pos(3, 0)] = 7;
//    bishop_scores[pos(4, 1)] = 6;
//    bishop_scores[pos(5, 0)] = 3;
//    bishop_scores[pos(6, 1)] = 3;
//
//    bishop_scores[pos(1, 2)] = 6;
//    bishop_scores[pos(0, 3)] = 7;
//    bishop_scores[pos(1, 4)] = 8;
//
//    bishop_scores[pos(5, 2)] = 8;
//    bishop_scores[pos(6, 3)] = 7;
//    bishop_scores[pos(5, 4)] = 6;
//    if (gety(P) >= 5)
//    {
//        int x1 = getx(P);
//        int y1 = gety(P);
//        return bishop_scores[pos(6-x1, 6-y1)];
//    }
//    else
//    {
//        return bishop_scores[P];
//    }
*/

int get_rook_score_white(U8 P)
{
    map<U8, int> rook_scores;
    rook_scores[pos(1, 0)] = 13;
    for (int i = 2; i <= 5; i++)
    {
        rook_scores[pos(i, 0)] = 8+ i;
    }
    rook_scores[pos(6, 0)] = 13;

    for (int i = 2; i <= 4; i++)
    {
        rook_scores[pos(i, 1)] = i+ 2;
    }
    rook_scores[pos(5, 1)] = 8;

    if ((gety(P) == 0 && getx(P) >= 1) || (gety(P) == 1 && getx(P) <= 5 && getx(P) >= 2))
    {
        rook_scores[pos(6, 0)] = 3;
        rook_scores[pos(5, 0)] = 3;
        rook_scores[pos(4, 0)] = 5;
        rook_scores[pos(3, 0)] = 7;
        rook_scores[pos(2, 0)] = 8;
        rook_scores[pos(1, 0)] = 15;
        rook_scores[pos(1, 0)] = 15;

        rook_scores[pos(6, 1)] = 2;
        rook_scores[pos(5, 1)] = 2;
        rook_scores[pos(4, 1)] = 4;
        rook_scores[pos(3, 1)] = 6;
        rook_scores[pos(2, 1)] = 7;
        rook_scores[pos(1, 1)] = 15;
        rook_scores[pos(1, 1)] = 15;

        return rook_scores[P];
    }

    U8 P1 = cw_90_pos(P);
    U8 P2 = cw_180_pos(P);
    U8 P3 = acw_90_pos(P);
    if ((gety(P1) == 0 && getx(P1) >= 1) || (gety(P1) == 1 && getx(P1) <= 5 && getx(P1) >= 2))
    {
        return rook_scores[P1];
    }
    if ((gety(P2) == 0 && getx(P2) >= 1) || (gety(P2) == 1 && getx(P2) <= 5 && getx(P2) >= 2))
    {
        return rook_scores[P2]+4;
    }
    if ((gety(P3) == 0 && getx(P3) >= 1) || (gety(P3) == 1 && getx(P3) <= 5 && getx(P3) >= 2))
    {
        return rook_scores[P3]+4;
    }

    cout<<"no matching position for bishop";
    return -1;
}

int get_rook_score_black(U8 P)
{
    U8 P1 = cw_180_pos(P);
    return get_rook_score_white(P1);
}


int get_king_score_white(U8 P)
{
//    map<U8, int> king_scores;
//
//    king_scores[pos(3, 1)] = 0;
//    king_scores[pos(3, 0)] = 0;
//
//    if ((getx(P) == 3) || getx)
//    {
//        return king_scores[P] = 0;
//    }

    return 10;
}

int get_king_score_black(U8 P)
{
    U8 P1 = cw_180_pos(P);
    return get_king_score_white(P1);
}


int calculate_material(const Board& b)
{
    int material = 0;
//    string board_str = board_to_str(b.data.board_0);
//    cout<<"\n";
//    cout<<board_str;
//    cout<<"\n";


    for (unsigned char i : b.data.board_0)
    {
        if (i == (WHITE | PAWN))
        {
            material += 1;
        }
        else if (i == (BLACK | PAWN))
        {
            material -= 1;
        }
        else if (i == (WHITE | BISHOP))
        {
            material += 5;
        }
        else if (i == (BLACK | BISHOP))
        {
            material -= 5;
        }
        else if (i == (WHITE | ROOK))
        {
            material += 3;
        }
        else if (i == (BLACK | ROOK))
        {
            material -= 3;
        }
        else if (i == (WHITE | KING))
        {
            material += 10;
        }
        else if (i == (BLACK | KING))
        {
            material -= 10;
        }
    }

    /*
    for (auto c : board_str)
    {
        if (c == 'p')
        {
            material -= 1;
        }
        else if (c == 'b')
        {
            material -= 5;
        }
        else if (c == 'r')
        {
            material -= 3;
        }
        else if (c == 'k')
        {
            material -= 10;
        }
        else if (c == 'P')
        {
            material += 1;
        }
        else if (c == 'B')
        {
            material += 5;
        }
        else if (c == 'R')
        {
            material += 3;
        }
        else if (c == 'K')
        {
            material += 10;
        }
    }
     */


    return material;
}

int calculate_positional_score(const Board& b)
{
    int positional_score = 0;

    for (int i = 0; i < 64; i++)
    {
        if (b.data.board_0[i] == EMPTY)
            continue;
        else
        {
            // PAWN score belongs to (-9 to 9)
            if (b.data.board_0[i] == (WHITE | PAWN))
            {
                positional_score += (get_pawn_score_white(i)+3) * 2;
            }
            else if (b.data.board_0[i] == (BLACK | PAWN))
            {
                 positional_score -= (get_pawn_score_black(i)+3) * 2;
            }

            // BISHOP score belongs to (-8 to 8)
            else if (b.data.board_0[i] == (WHITE | BISHOP))
            {
                positional_score += get_bishop_score(i);
            }
            else if (b.data.board_0[i] == (BLACK | BISHOP))
            {
                positional_score -= get_bishop_score(i);
            }

            // ROOK score belongs to (
            else if (b.data.board_0[i] == (WHITE | ROOK))
            {
                positional_score += get_rook_score_white(i);
            }
            else if (b.data.board_0[i] == (BLACK | ROOK))
            {
                positional_score -= get_rook_score_black(i);
            }

            // below is useless (for now) +100 - 100 = 0 always
            else if (b.data.board_0[i] == (WHITE | KING))
            {
                positional_score += get_king_score_white(i);
            }
            else if (b.data.board_0[i] == (BLACK | KING))
            {
                positional_score -= get_king_score_black(i);
            }
        }
    }

    return positional_score;
}

int count_pawn_score(Board b)
{
    int pawn_score = 0;

    pair<int, int> wp1 = make_pair(getx(b.data.w_pawn_ws), gety(b.data.w_pawn_ws));
    pair<int, int> wp2 = make_pair(getx(b.data.w_pawn_bs), gety(b.data.w_pawn_bs));

    pair<int, int> bp1 = make_pair(getx(b.data.b_pawn_ws), gety(b.data.b_pawn_ws));
    pair<int, int> bp2 = make_pair(getx(b.data.b_pawn_bs), gety(b.data.b_pawn_bs));

    if (wp1.first == 0 || wp1.first == 1)
    {
        if (wp1.second == 6)
            pawn_score += 5 + wp1.first;
        else
            pawn_score += (wp1.second + 1);
    }
    else if (wp1.second == 5 || wp1.second == 6)
    {
        pawn_score += 5+wp1.first;
    }

    if (wp2.first == 0 || wp2.first == 1)
    {
        if (wp2.second == 6)
            pawn_score += 5 + wp2.first;
        else
            pawn_score += (wp2.second + 1);
    }
    else if (wp2.second == 5 || wp2.second == 6)
    {
        pawn_score += 5+wp2.first;
    }

    if (bp1.first == 5 || bp1.first == 6)
    {
        if (bp1.second == 0)
            pawn_score -= 5 + (6-bp1.first);
        else
            pawn_score -= (7-bp1.second);
    }
    else if (bp1.second == 1 || bp1.second == 0)
    {
        pawn_score -= 5+ (6-bp1.first);
    }

    if (bp2.first == 5 || bp2.first == 6)
    {
        if (bp2.second == 0)
            pawn_score -= 5 + (6-bp2.first);
        else
            pawn_score -= (7-bp2.second);
    }
    else if (bp2.second == 1 || bp2.second == 0)
    {
        pawn_score -= 5+ (6-bp2.first);
    }

    return pawn_score;
}

int calc_check_score(Board b)
{

    if (b.in_check())
    {
        if (b.data.player_to_play == WHITE)
            return -10;
        else
            return 10;
    }
    else
        return 0;
}

int evaluate_function(Board b)
{
    if (b.get_legal_moves().empty())
    {
        if (b.in_check()) {
            if (b.data.player_to_play == WHITE)
                return -100000;
            else
                return 100000;
        }
        else
            return 0;
    }

    int material = calculate_material(b);  // range -5 to +5
    int w1 = 100;

    int pawn_score = 0; // count_pawn_score(b); // range -20 to +20
    int w2 = 6;
    // wrong code for count_pawn_score, uses b.data.w_pawn_ws and b.data.w_pawn_bs
    int protected_score = 0;  // how_many_protected_score(b);  // -1 to 1
    int w4 = 8;

    int check_score = calc_check_score(b); // -10 or 10
    int w3 = 7;

    int positional_score = calculate_positional_score(b); // range -20/-13 to +20/+13 (for rook)
                                                            // -9 to 9 for pawn, -8 to 8 for bishop
    int w5 = 2;

    int final_score = (w1*material) + (w2*pawn_score) + (w3*check_score) + (w4*protected_score) + (w5*positional_score);
    return final_score;
}


pair<int, U16> Min_value(Board b, int depth, int alpha, int beta, Engine* e);

pair<int, U16> Max_value(Board b, int depth, int alpha, int beta, Engine* e)
{
    if (!(e->search))
    {
        return make_pair(-789, U16(e->best_move));
    }

    if (depth > MAX_DEPTH)
    {
        return make_pair(evaluate_function(b), 0);
    }
    else
    {
        U16 best_move = 0;
        auto moveset = b.get_legal_moves();
        if (moveset.size() == 0) {
            if (b.in_check()) {
                if (b.data.player_to_play == WHITE)
                    return make_pair(-100000, best_move);
                else
                    return make_pair(100000, best_move);
            }
            else
                return make_pair(0, best_move);
        }
        else {

            int max_value = -100050;
            for (auto m : moveset) {
                if (!(e->search))
                {
                    return make_pair(0, U16(e->best_move));
                }

                Board b_copy = *b.copy();
                b_copy.do_move(m);
                auto min_ans = Min_value(b_copy, depth + 1, alpha, beta, e);
                if (!(e->search))
                {
                    return make_pair(6789, U16(e->best_move));
                }

                alpha = max(alpha, min_ans.first);
                if (alpha>=beta)
                    return make_pair(min_ans.first, m);


                if (min_ans.first > max_value)
                {
                    max_value = min_ans.first;
                    best_move = m;
//                    if (depth == 0)
//                        e->best_move = m;
                }
            }

            return make_pair(max_value, best_move);
        }
    }
}

pair<int, U16> Min_value(Board b, int depth, int alpha, int beta, Engine* e)
{
    if (!(e->search))
    {
        return make_pair(6789, U16(e->best_move));
    }

    if (depth > MAX_DEPTH) {
        return make_pair(evaluate_function(b), 0);
    }

    else {
        U16 best_move = 0;
        auto moveset = b.get_legal_moves();
        if (moveset.empty()) {
            if (b.in_check()) {
                if (b.data.player_to_play == WHITE)
                    return make_pair(-100000, best_move);
                else
                    return make_pair(100000, best_move);
            }
            else
                return make_pair(0, best_move);
        }
        else {
            int min_value = 100050;
            for (auto m: moveset) {
                if (!(e->search))
                {
                    return make_pair(6789, U16(e->best_move));
                }

                Board b_copy = *b.copy();
                b_copy.do_move(m);
                auto max_ans = Max_value(b_copy, depth + 1, alpha, beta, e);
                if (!(e->search))
                {
                    return make_pair(6789, U16(e->best_move));
                }

                beta = min(beta, max_ans.first);
                if (alpha>=beta)
                    return make_pair(max_ans.first, m);
                if (max_ans.first < min_value) {
                    min_value = max_ans.first;
                    best_move = m;
//                    if (depth == 0)
//                        e->best_move = m;
                }
            }

            return make_pair(min_value, best_move);
        }
    }
}


pair<int, U16> MiniMax(Board b, PlayerColor colour, Engine* e)
{
    auto moveset = b.get_legal_moves();

    if (moveset.empty()) {
        return make_pair(0, 0);
    }
    else {
        auto ans = make_pair(0, 0);
        if (colour == BLACK)
        {
            ans = Min_value(b, 0, -100000, 100000, e);
            cout<<"\n\n\n";
            cout<< "final evaluation is, "<<ans.first<< " at MAX_DEPTH: "<<MAX_DEPTH<<'\n';
            cout<<"best move at this depth is: "<<move_to_str(ans.second)<<"\n";
            cout<<"\n\n\n";
            return ans;
        }
        else
        {
            ans = Max_value(b, 0, -100000, 100000, e);
            cout<<"\n\n\n";
            cout<< "final evaluation is, "<<ans.first<< " at MAX_DEPTH: "<<MAX_DEPTH<<'\n';
            cout<<"best move at this depth is: "<<move_to_str(ans.second)<<"\n";
            cout<<"\n\n\n";
            return ans;
        }
    }
}


void Engine::find_best_move(const Board& b) {

    auto colour = b.data.player_to_play;
    MAX_DEPTH = 0;
    if (this->search) {
        while (this->search)
        {
            if ((b.data.player_to_play == WHITE) && ( b.data.board_0[pos(2, 1)] == (WHITE|PAWN)) && (b.data.board_0[pos(1, 2)] == EMPTY) ) {

                this->best_move = (pos(2, 1) << 8) | (pos(1, 2));
                cout << move_to_str(this->best_move) << endl;
                return ;
            }

            auto p = MiniMax(b, colour, this);
            this->best_move = p.second;
            if (p.first == 100000)
                break;

            if (MAX_DEPTH == 0)
                MAX_DEPTH++;
            else
                MAX_DEPTH += 2;
//            MAX_DEPTH++;
        }
        return;
    }
}


    // pick a random move
//    std::cout<<move_number++<<'\n';
//
//    auto moveset = b.get_legal_moves();
//    if (moveset.size() == 0) {
//        this->best_move = 0;
//    }
//    else {
//        std::vector<U16> moves;
//        std::cout << all_boards_to_str(b) << std::endl;
//        for (auto m : moveset) {
//            std::cout << move_to_str(m) << " ";
//        }
//        std::cout << std::endl;
//        std::sample(
//            moveset.begin(),
//            moveset.end(),
//            std::back_inserter(moves),
//            1,
//            std::mt19937{std::random_device{}()}
//        );
//        this->best_move = moves[0];
//    }

