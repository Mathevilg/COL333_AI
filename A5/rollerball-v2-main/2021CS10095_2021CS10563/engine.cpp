#include <algorithm>
#include <random>
#include <iostream>
#include <thread>

#include "board.hpp"
#include "engine.hpp"
#include "butils.hpp"

using namespace std;

BoardType board_type;

class Time{
    chrono::high_resolution_clock::time_point start_time;
    public:
    Time(){
        start_time = chrono::high_resolution_clock::now();
    }

    float get_elapsed_time(){
        auto end_time = chrono::high_resolution_clock::now();
        auto time_taken = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
        return time_taken/1e3;
    }
};

class engine_b1
{
public:

    int MAX_DEPTH = 4;
    float time_left_to_match;
    Time* time;
    float delta = 0.2;
    float timeAllotted;
    chrono::high_resolution_clock::time_point start_time;
    map<U8, int> pawn_scores = {
        // Initial entries
        {pos(2, 0), 0}, {pos(2, 1), 0},

        {pos(0, 0), 1}, {pos(1, 0), 1},
        {pos(0, 1), 2}, {pos(1, 1), 3},
        // Entries from your existing code
        {pos(0, 2), 5}, {pos(1, 2), 5},
        {pos(0, 3), 6}, {pos(1, 3), 6},
        {pos(0, 4), 7}, {pos(1, 4), 7},
        {pos(0, 5), 8}, {pos(1, 5), 8},
        {pos(0, 6), 8}, {pos(1, 6), 9},
        {pos(2, 5), 10}, {pos(2, 6), 10},
        // {pos(3, 5), 11}, {pos(3, 6), 11},
        // Remaining entries
        {pos(4, 5), 12}, {pos(4, 6), 12},
        {pos(3, 5), 14}, {pos(3, 6), 14}
    };

    map<U8, int> bishop_scores = {
        {pos(1, 0), 3},
        {pos(2, 1), 8},
        {pos(3, 0), 2},
        {pos(4, 1), 6},
        {pos(5, 0), 3}
    };

    map<U8, int> rook_scores = {
        {pos(6, 0), 5},
        {pos(5, 0), 7},
        {pos(4, 0), 7},
        {pos(3, 0), 10},
        {pos(2, 0), 11},
        {pos(1, 0), 16},
        {pos(0, 0), 16},
        {pos(6, 1), 2},
        {pos(5, 1), 2},
        {pos(4, 1), 4},
        {pos(3, 1), 6},
        {pos(2, 1), 7},
        {pos(1, 1), 15},
        {pos(0, 1), 15}
    };


    map<U8, int> king_scores = {
            {pos(6, 0), 2},
            {pos(5, 0), 2},
            {pos(4, 0), 2},
            {pos(3, 0), 2},
            {pos(2, 0), 2},
            {pos(1, 0), 2},
            {pos(5, 1), 4},
            {pos(4, 1), 4},
            {pos(3, 1), 4},
            {pos(2, 1), 4},
    };

    int get_pawn_score_white(U8 P);
    int get_pawn_score_black(U8 P);
    int get_bishop_score(U8 P);
    int get_rook_score_white(U8 P);
    int get_rook_score_black(U8 P);
    int get_king_score_white(U8 P);
    int get_king_score_black(U8 P);
    U16 return_best_move(const Board& b, Engine *e);
    int pawn_closeness_score_white(const Board& b);
    int pawn_closeness_score_black(const Board& b);

    int calc_check_score(const Board& b);
    int calc_positional_score(const Board& b);
    int calculate_material(const Board& b);

    bool isTimeValid();
    float time_for_this_move = 4;

    pair<int, U16> MiniMax(Board& b, PlayerColor colour, Engine* e);

    pair<int, U16> Min_value(Board& b, int depth, int alpha, int beta, Engine* e);

    pair<int, U16> Max_value(Board& b, int depth, int alpha, int beta, Engine* e);

    int evaluate_function(const Board& b);
    int eval2(Board& b, U16 move);



    void undoMove(Board& b, U16 move, U8 dead, int deadidx) {
        U8 p0 = getp0(move);
        U8 p1 = getp1(move);
        U8 promo = getpromo(move);

        U8 piecetype = b.data.board_0[p1];
        U8 deadpiece = b.data.last_killed_piece;
        b.data.last_killed_piece = dead;

        // scan and get piece from coord
        U8 *pieces = (U8*)(&(b.data));
        for (int i=0; i<2*b.data.n_pieces; i++) {
            if (pieces[i] == p1) {
                pieces[i] = p0;
                break;
            }
        }
        if (b.data.last_killed_piece_idx >= 0) {
            pieces[b.data.last_killed_piece_idx] = p1;
        }
        b.data.last_killed_piece_idx = deadidx;

        if (promo == PAWN_ROOK) {
            piecetype = ((piecetype & (WHITE | BLACK)) ^ ROOK) | PAWN;
        }
        else if (promo == PAWN_BISHOP) {
            piecetype = ((piecetype & (WHITE | BLACK)) ^ BISHOP) | PAWN;
        }

        b.data.board_0  [b.data.transform_array[0][p1]] = deadpiece;
        b.data.board_90 [b.data.transform_array[1][p1]] = deadpiece;
        b.data.board_180[b.data.transform_array[2][p1]] = deadpiece;
        b.data.board_270[b.data.transform_array[3][p1]] = deadpiece;

        b.data.board_0  [b.data.transform_array[0][p0]] = piecetype;
        b.data.board_90 [b.data.transform_array[1][p0]] = piecetype;
        b.data.board_180[b.data.transform_array[2][p0]] = piecetype;
        b.data.board_270[b.data.transform_array[3][p0]] = piecetype;

    }

    
    void flip_player_(Board &b) {
        b.data.player_to_play = (PlayerColor)(b.data.player_to_play ^ (WHITE | BLACK));
    }

};











#define cw_180_pos_1(p) cw_180_7x7[((int)p)%64]
#define acw_90_pos_1(p) acw_90_7x7[((int)p)%64]
#define cw_90_pos_1(p) cw_90_7x7[((int)p)%64]

int engine_b1::get_pawn_score_white(U8 P)
{
    return pawn_scores[P];
}

int engine_b1::get_pawn_score_black(U8 P1)
{
    // int idx = (int) P1;
    U8 P = cw_180_pos_1(P1);
    if (getx(P) == 5)
    {
        if (gety(P) == 6)
            return 6;
        if (gety(P) == 5)
            return 6;
    }
    return get_pawn_score_white(P);
}

int engine_b1::get_bishop_score(U8 P)
{


    if (gety(P) <= 1 && getx(P) >= 1 && getx(P) <= 5)
    {
        return bishop_scores[P];
    }

    U8 P1 = cw_90_pos_1(P);
    U8 P2 = cw_180_pos_1(P);
    U8 P3 = acw_90_pos_1(P);
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

int engine_b1::get_rook_score_white(U8 P)
{
    if ((gety(P) == 0 && getx(P) >= 0) || (gety(P) == 1 && getx(P) <= 6 && getx(P) >= 0))
    {
        return rook_scores[P];
    }

    U8 P1 = cw_90_pos_1(P);
    U8 P2 = cw_180_pos_1(P);
    U8 P3 = acw_90_pos_1(P);
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

int engine_b1::get_rook_score_black(U8 P)
{
    U8 P1 = cw_180_pos_1(P);
    return get_rook_score_white(P1);
}

int engine_b1::get_king_score_white(U8 P)
{
    if ((gety(P) == 0 && getx(P) >= 1) || (gety(P) == 1 && getx(P) <= 5 && getx(P) >= 2))
    {
        return king_scores[P];
    }

    U8 P1 = cw_90_pos_1(P);
    U8 P2 = cw_180_pos_1(P);
    U8 P3 = acw_90_pos_1(P);
    if ((gety(P1) == 0 && getx(P1) >= 1) || (gety(P1) == 1 && getx(P1) <= 5 && getx(P1) >= 2))
    {
        return king_scores[P1]-1;
    }
    if ((gety(P2) == 0 && getx(P2) >= 1) || (gety(P2) == 1 && getx(P2) <= 5 && getx(P2) >= 2))
    {
        return king_scores[P2]-1;
    }
    if ((gety(P3) == 0 && getx(P3) >= 1) || (gety(P3) == 1 && getx(P3) <= 5 && getx(P3) >= 2))
    {
        return king_scores[P3]+1;
    }

    cout<<"no matching position for bishop";
    return -1;
}

int engine_b1::get_king_score_black(U8 P)
{
    U8 P1 = cw_180_pos_1(P);
    return get_king_score_white(P1);
}


// chrono::high_resolution_clock::time_point start_time;
// int time_left_to_match;
// this is the main function !
bool engine_b1::isTimeValid() {
    this_thread::sleep_for(std::chrono::milliseconds(1950));
    return chrono::high_resolution_clock::now() - start_time < chrono::milliseconds(2000);
}

int engine_b1::calculate_material(const Board& b) {
    int material = 0;

    if (b.data.w_pawn_1 != DEAD)
    {
        if (b.data.board_0[b.data.w_pawn_1] == (PAWN | WHITE))
            material += 1;
        else
        {
            if (b.data.board_0[b.data.w_pawn_1] == (ROOK | WHITE))
                material += 5;
            else if (b.data.board_0[b.data.w_pawn_1] == (BISHOP | WHITE))
                material += 3;
            else if (b.data.board_0[b.data.w_pawn_1] == (KNIGHT | WHITE))
                material += 4;
        }
    }
    if (b.data.w_pawn_2 != DEAD)
    {
        if (b.data.board_0[b.data.w_pawn_2] == (PAWN | WHITE))
            material += 1;
        else
        {
            if (b.data.board_0[b.data.w_pawn_2] == (ROOK | WHITE))
                material += 5;
            else if (b.data.board_0[b.data.w_pawn_2] == (BISHOP | WHITE))
                material += 3;
            else if (b.data.board_0[b.data.w_pawn_2] == (KNIGHT | WHITE))
                material += 4;
        }
    }

    if (b.data.w_bishop != DEAD)
        material += 3;
    if (b.data.w_rook_1 != DEAD)
        material += 5;
    if (b.data.w_rook_2 != DEAD)
        material += 5;


    if (b.data.b_pawn_1 != DEAD)
    {
        if (b.data.board_0[b.data.b_pawn_1] == (PAWN | BLACK))
            material -= 1;
        else
        {
            if (b.data.board_0[b.data.b_pawn_1] == (ROOK | BLACK))
                material -= 5;
            else if (b.data.board_0[b.data.b_pawn_1] == (BISHOP | BLACK))
                material -= 3;
            else if (b.data.board_0[b.data.b_pawn_1] == (KNIGHT | BLACK))
                material -= 4;
        }
    }
    if (b.data.b_pawn_2 != DEAD)
    {
        if (b.data.board_0[b.data.b_pawn_2] == (PAWN | BLACK))
            material -= 1;
        else
        {
            if (b.data.board_0[b.data.b_pawn_2] == (ROOK | BLACK))
                material -= 5;
            else if (b.data.board_0[b.data.b_pawn_2] == (BISHOP | BLACK))
                material -= 3;
            else if (b.data.board_0[b.data.b_pawn_2] == (KNIGHT | BLACK))
                material -= 4;
        }
    }

    if (b.data.b_bishop != DEAD)
        material -= 3;

    if (b.data.b_rook_1 != DEAD)
        material -= 5;
    if (b.data.b_rook_2 != DEAD)
        material -= 5;

    return material;
}


int engine_b1::calc_check_score(const Board& b)
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


int engine_b1::calc_positional_score(const Board &b){
    int positional_score = 0;

    if (b.data.w_pawn_1 != DEAD)
    {
        if (b.data.board_0[b.data.w_pawn_1] == (PAWN | WHITE))
            positional_score += 2*(get_pawn_score_white(b.data.w_pawn_1)+3);
        else
        {
            if (b.data.board_0[b.data.w_pawn_1] == (ROOK | WHITE))
                positional_score += get_rook_score_white(b.data.w_pawn_1);
            else if (b.data.board_0[b.data.w_pawn_1] == (BISHOP | WHITE))
                positional_score += get_bishop_score(b.data.w_pawn_1);
        }
    }
    if (b.data.w_pawn_2 != DEAD)
    {
        if (b.data.board_0[b.data.w_pawn_2] == (PAWN | WHITE))
            positional_score += 2*(get_pawn_score_white(b.data.w_pawn_2)+3);
        else
        {
            if (b.data.board_0[b.data.w_pawn_2] == (ROOK | WHITE))
                positional_score += get_rook_score_white(b.data.w_pawn_2);
            else if (b.data.board_0[b.data.w_pawn_2] == (BISHOP | WHITE))
                positional_score += get_bishop_score(b.data.w_pawn_2);
        }
    }

    if (b.data.w_bishop != DEAD)
        positional_score += 0.5*get_bishop_score(b.data.w_bishop);
    if (b.data.w_rook_1 != DEAD)
        positional_score += get_rook_score_white(b.data.w_rook_1);
    if (b.data.w_rook_2 != DEAD)
        positional_score += get_rook_score_white(b.data.w_rook_2);

    if (b.data.b_pawn_1 != DEAD)
    {
        if (b.data.board_0[b.data.b_pawn_1] == (PAWN | BLACK))
            positional_score -= 2*(get_pawn_score_black(b.data.b_pawn_1)+3);
        else
        {
            if (b.data.board_0[b.data.b_pawn_1] == (ROOK | BLACK))
                positional_score -= get_rook_score_black(b.data.b_pawn_1);
            else if (b.data.board_0[b.data.b_pawn_1] == (BISHOP | BLACK))
                positional_score -= get_bishop_score(b.data.b_pawn_1);
        }
    }
    if (b.data.b_pawn_2 != DEAD)
    {
        if (b.data.board_0[b.data.b_pawn_2] == (PAWN | BLACK))
            positional_score -= 2*(get_pawn_score_black(b.data.b_pawn_2)+3);
        else
        {
            if (b.data.board_0[b.data.b_pawn_2] == (ROOK | BLACK))
                positional_score -= get_rook_score_black(b.data.b_pawn_2);
            else if (b.data.board_0[b.data.b_pawn_2] == (BISHOP | BLACK))
                positional_score -= get_bishop_score(b.data.b_pawn_2);
        }
    }

    if (b.data.b_bishop != DEAD)
        positional_score -= 0.5*get_bishop_score(b.data.b_bishop);

    if (b.data.b_rook_1 != DEAD)
        positional_score -= get_rook_score_black(b.data.b_rook_1);
    if (b.data.b_rook_2 != DEAD)
        positional_score -= get_rook_score_black(b.data.b_rook_2);

    positional_score += get_king_score_white(b.data.w_king);
    positional_score -= get_king_score_black(b.data.b_king);

    return positional_score;
}


int engine_b1::pawn_closeness_score_white(const Board& b)
{
    int score = 12;

    auto pos1 = b.data.w_pawn_1;

    auto pos2 = b.data.w_pawn_2;

    if (pos1 != DEAD && pos2 != DEAD && b.data.board_0[pos2] == (WHITE|PAWN) && b.data.board_0[pos1] == (WHITE|PAWN))
    {
        score -= abs(gety(pos1) - gety(pos2));
        if (gety(pos1) == 5 || gety(pos1) == 6 || gety(pos2) == 5 || gety(pos2) == 6)
            score -= abs(getx(pos1) - getx(pos2));
    }
    else
    {
        score = 0;
    }
    return score;
}


int engine_b1::pawn_closeness_score_black(const Board& b)
{
    int score = 12;

    auto pos1 = b.data.b_pawn_1;

    auto pos2 = b.data.b_pawn_2;

    if (pos1 != DEAD && pos2 != DEAD && b.data.board_0[pos2] == (BLACK|PAWN) && b.data.board_0[pos1] == (BLACK|PAWN))
    {
        score -= abs(gety(pos1) - gety(pos2));
        if (gety(pos1) == 5 || gety(pos1) == 6 || gety(pos2) == 5 || gety(pos2) == 6)
            score -= abs(getx(pos1) - getx(pos2));
    }
    else
    {
        score = 0;
    }

    return score;
}


int engine_b1::evaluate_function(const Board& b)
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
    int w1 = 110;

    int pawn_score = 0; // count_pawn_score(b); // range -20 to +20
    int w2 = 6;


    int check_score = calc_check_score(b); // -10 or 10
    int w3 = 3;


    // wrong code for count_pawn_score, uses b.data.w_pawn_ws and b.data.w_pawn_bs
    int protected_score = 0;  // how_many_protected_score(b);  // -1 to 1
    int w4 = 8;


    int positional_score = calc_positional_score(b); // range -20/-13 to +20/+13 (for rook)
    // -9 to 9 for pawn, -8 to 8 for bishop
    int w5 = 3;

    int pawn_closeness_score = pawn_closeness_score_white(b) - pawn_closeness_score_black(b);
    int w6 = 2;

    int final_score = (w1*material) + (w2*pawn_score) + (w3*check_score) + (w4*protected_score) + (w5*positional_score) + (w6*pawn_closeness_score);
    return final_score;
}


int engine_b1::eval2(Board& b, U16 move){
    // Board b_copy = Board(b);
    // b_copy.do_move_(move);

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

    U8 deadpiece = b.data.last_killed_piece;
    int deadidx = b.data.last_killed_piece_idx;
    b.do_move_(move);


    int material = calculate_material(b);  // range -5 to +5
    int w1 = 110;

    int check_score = calc_check_score(b); // -10 or 10
    int w3 = 3;

    int positional_score = calc_positional_score(b); // range -20/-13 to +20/+13 (for rook)
    // -9 to 9 for pawn, -8 to 8 for bishop
    int w5 = 3;

    int pawn_closeness_score = pawn_closeness_score_white(b) - pawn_closeness_score_black(b);
    int w6 = 2;

    int final_score = (w1*material) + (w3*check_score) + (w5*positional_score) + (w6*pawn_closeness_score);
    
    undoMove(b, move, deadpiece, deadidx);
    flip_player_(b);
    return final_score;
}


pair<int, U16> engine_b1::Max_value(Board& b, int depth, int alpha, int beta, Engine* e)
{

    if (depth > MAX_DEPTH)
    {
        return make_pair(evaluate_function(b), 0);
    }
    else
    {
        U16 best_move = 0;
        auto moveset = b.get_legal_moves();
        if (moveset.empty()) {
            if (b.in_check()) {
                if (b.data.player_to_play == WHITE)
                    return make_pair(-100000, U16(e->best_move));
                else
                    return make_pair(100000, U16(e->best_move));
            }
            else
                return make_pair(0, best_move);
        }
        else if (time->get_elapsed_time() < timeAllotted){

            int max_value = -100050;
            vector<pair<int,  U16> > temp;
            for (auto move: moveset) {
                temp.push_back(make_pair(eval2(b, move), move));
            }
            sort(temp.begin(), temp.end());
            reverse(temp.begin(), temp.end());
            for (auto m1: temp) {
                U16 m = m1.second;
                // Board b_copy = Board(b); // Copy constructor, make move and undo move instead??
                // b_copy.do_move_(m);

                U8 deadpiece = b.data.last_killed_piece;
                int deadidx = b.data.last_killed_piece_idx;
                b.do_move_(m);
                auto min_ans = Min_value(b, depth + 1, alpha, beta, e);
                alpha = max(alpha, min_ans.first);
                undoMove(b, m, deadpiece, deadidx);
                flip_player_(b);
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
        else {
            return make_pair(-100050, best_move);
        }
    }
}

pair<int, U16> engine_b1::Min_value(Board& b, int depth, int alpha, int beta, Engine* e)
{

    if (depth > MAX_DEPTH) {
        return make_pair(evaluate_function(b), U16(e->best_move));
    }
    else {
        U16 best_move = 0;
        auto moveset = b.get_legal_moves();
        if (moveset.empty()) {
            if (b.in_check()) {
                if (b.data.player_to_play == WHITE)
                    return make_pair(-100000, U16(e->best_move));
                else
                    return make_pair(100000, U16(e->best_move));
            }
            else
                return make_pair(0, U16(e->best_move));
        }
        else if (time->get_elapsed_time() < timeAllotted){
            int min_value = 100050;
            vector<pair<int,  U16> > temp;
            for (auto move: moveset) {
                temp.push_back(make_pair(eval2(b, move), move));
            }
            sort(temp.begin(), temp.end());
            for (auto m1: temp) {
                U16 m = m1.second;

                // Board b_copy = Board(b);
                // b_copy.do_move_(m);
                U8 deadpiece = b.data.last_killed_piece;
                int deadidx = b.data.last_killed_piece_idx;
                b.do_move_(m);
                auto max_ans = Max_value(b, depth + 1, alpha, beta, e);
                undoMove(b, m, deadpiece, deadidx);
                flip_player_(b);
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
        else {
            return make_pair(100050, best_move);
        }
    }
}


pair<int, U16> engine_b1::MiniMax(Board& b, PlayerColor colour, Engine* e)
{
    auto moveset = b.get_legal_moves();

    if (moveset.empty()) {
        return make_pair(0, 0);
    }
    else {
        auto ans = make_pair(0, 0);
        if (colour == BLACK)
        {
            ans = Min_value(b, 1, -100000, 100000, e);
//            cout<<"\n\n\n";
//            cout<< "final evaluation is, "<<ans.first<< " at MAX_DEPTH: "<<MAX_DEPTH<<'\n';
//            cout<<"best move at this depth is: "<<move_to_str(ans.second)<<"\n";
//            cout<<"\n\n\n";
            return ans;
        }
        else
        {
            ans = engine_b1::Max_value(b, 1, -100000, 100000, e);
//            cout<<"\n\n\n";
//            cout<< "final evaluation is, "<<ans.first<< " at MAX_DEPTH: "<<MAX_DEPTH<<'\n';
//            cout<<"best move at this depth is: "<<move_to_str(ans.second)<<"\n";
//            cout<<"\n\n\n";
            return ans;
        }
    }
}


U16 engine_b1::return_best_move(const Board &b1, Engine *e) {
    time = new Time();
    Board b = Board(b1);
    start_time = chrono::high_resolution_clock::now();
    time_left_to_match =  e->time_left.count()/1e3;
    
    if (time_left_to_match > 90){
        timeAllotted = 5.0;
    }
    else if (time_left_to_match < 90 && time_left_to_match > 50){
        timeAllotted = 4.0;
    }
    else if (time_left_to_match <50 && time_left_to_match > 20){
        timeAllotted = min((float) 3.5, time_left_to_match/15);
    }
    else if (time_left_to_match > 2){
        timeAllotted = time_left_to_match/15;
    }
    else {
        auto moveset = b.get_legal_moves();
        for (auto m : moveset){
            return m;
        }
    }


    cout << "time left to match: " << time_left_to_match << endl;
    auto moveset = b.get_legal_moves();
    if (moveset.size() == 1)
    {
        for (auto m : moveset)
            return m;
    }
    auto colour = b.data.player_to_play;

    vector<pair<int, pair<U16, Board>>> moveset_boards;

    MAX_DEPTH = 1;
    while (time->get_elapsed_time() < timeAllotted)
    {

        if ((b.data.player_to_play == WHITE) && ( b.data.board_0[pos(2, 1)] == (WHITE|PAWN)) && (b.data.board_0[pos(1, 2)] == EMPTY) ) {
            e->best_move = (pos(2, 1) << 8) | (pos(1, 2));
            cout << move_to_str(e->best_move) << endl;
            return e->best_move;
        }

        if ((b.data.player_to_play == BLACK) && ( b.data.board_0[pos(1, 2)] == (WHITE|PAWN)) && (b.data.board_0[pos(2, 1)] == EMPTY) ) {

            if (( b.data.board_0[pos(4, 5)] == (BLACK|PAWN)) && (b.data.board_0[pos(5, 4)] == EMPTY) ) {

                e->best_move = (pos(4, 5) << 8) | (pos(5, 4));
                cout << move_to_str(e->best_move) << endl;
                return e->best_move;
            }
        }

        auto p = MiniMax(b, colour, e);

        if (time->get_elapsed_time() >= timeAllotted)
            break;

        e->best_move = p.second;
        if ((p.first == 100000) && (b.data.player_to_play == WHITE))
            break;

        if ((p.first == -100000) && (b.data.player_to_play == BLACK))
            break;

        cout<<"\n\n";
        cout<< "final evaluation is, "<<p.first<< " at MAX_DEPTH: "<<MAX_DEPTH<<'\n';
        cout<<"best move at this depth is: "<<move_to_str(p.second)<<"\n";
        cout<<"\n\n";

        if (MAX_DEPTH == 1)
            MAX_DEPTH++;
        else
            MAX_DEPTH += 1;
    }

    return e->best_move;
}





























class Time1{
    chrono::high_resolution_clock::time_point start_time;
    public:
    Time1(){
        start_time = chrono::high_resolution_clock::now();
    }

    float get_elapsed_time(){
        auto end_time = chrono::high_resolution_clock::now();
        auto time_taken = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
        return time_taken/1e3;
    }
};

class engine_b2
{
public:
    int MAX_DEPTH = 4;
    float time_left_to_match;

    // void initialise();
    Time1* time;
    float timeAllotted = 6.2;
    chrono::high_resolution_clock::time_point start_time;
    
    map<U8, int> pawn_scores = {
            {pos(5, 0), 0}, {pos(5, 1), 0},/*                                                                                     */{pos(5, 6), 14}, {pos(5, 7), 14},
            {pos(4, 0), 3}, {pos(4, 1), 1},/*                                                                                     */{pos(4, 6), 13}, {pos(4, 7), 13},
            {pos(3, 0), 1}, {pos(3, 1), 2},/*                                                                                     */{pos(3, 6), 11}, {pos(3, 7), 11},
            {pos(2, 0), 1}, {pos(2, 1), 2},/*                                                                                     */{pos(2, 6), 10}, {pos(2, 7), 10},
            {pos(1, 0), 1}, {pos(1, 1), 3}, {pos(1, 2), 4}, {pos(1, 3), 5}, {pos(1, 4), 6}, {pos(1, 5), 7}, {pos(1, 6), 8}, {pos(1, 7), 8},
            {pos(0, 0), 1}, {pos(0, 1), 3}, {pos(0, 2), 4}, {pos(0, 3), 5}, {pos(0, 4), 6}, {pos(0, 5), 7}, {pos(0, 6), 8}, {pos(0, 7), 8},

    };

    map<U8, int> bishop_scores = {
                       {pos(2, 1), 5}, {pos(3, 1), 6}, {pos(4, 1), 5}, {pos(5, 1), 6}, {pos(6, 1), 6},
        {pos(1, 0), 3}, {pos(2, 0), 4}, {pos(3, 0), 3}, {pos(4, 0), 3}, {pos(5, 0), 3}, {pos(6, 0), 3}, {pos(7, 0), 1}
    };

    map<U8, int> rook_scores = {
                       {pos(2, 1), 4}, {pos(3, 1), 5}, {pos(4, 1), 6}, {pos(5, 1), 7}, {pos(6, 1), 9},
        {pos(1, 0), 16}, {pos(2, 0), 11}, {pos(3, 0), 12}, {pos(4, 0), 13}, {pos(5, 0), 14}, {pos(6, 0), 15}, {pos(7, 0), 15}
    };

    map<U8, int> king_scores = {
                       {pos(2, 1), 4}, {pos(3, 1), 4}, {pos(4, 1), 4}, {pos(5, 1), 4}, {pos(6, 1), 4},
       {pos(1, 0), 1}, {pos(2, 0), 1}, {pos(3, 0), 1}, {pos(4, 0), 1}, {pos(5, 0), 1}, {pos(6, 0), 1}, {pos(7, 0), 1}
    };


    int get_pawn_score_white(U8 P);
    int get_pawn_score_black(U8 P);
    int get_bishop_score(U8 P);
    int get_rook_score_white(U8 P);
    int get_rook_score_black(U8 P);
    int get_king_score_white(U8 P);
    int get_king_score_black(U8 P);
    U16 return_best_move(const Board& b, Engine *e);

    int calc_check_score(const Board& b);
    int calc_positional_score(const Board& b);
    int calculate_material(const Board& b);
    int pawn_closeness_score_white(const Board& b);
    int pawn_closeness_score_black(const Board& b);

    bool isTimeValid();

    pair<int, U16> MiniMax(Board& b, PlayerColor colour, Engine* e);

    pair<int, U16> Min_value(Board& b, int depth, int alpha, int beta, Engine* e);

    pair<int, U16> Max_value(Board& b, int depth, int alpha, int beta, Engine* e);

    int evaluate_function(const Board& b);
    int eval2(Board& b, U16 move);

    void undoMove(Board& b, U16 move, U8 dead, int deadidx) {
        U8 p0 = getp0(move);
        U8 p1 = getp1(move);
        U8 promo = getpromo(move);

        U8 piecetype = b.data.board_0[p1];
        U8 deadpiece = b.data.last_killed_piece;
        b.data.last_killed_piece = dead;

        // scan and get piece from coord
        U8 *pieces = (U8*)(&(b.data));
        for (int i=0; i<2*b.data.n_pieces; i++) {
            if (pieces[i] == p1) {
                pieces[i] = p0;
                break;
            }
        }
        if (b.data.last_killed_piece_idx >= 0) {
            pieces[b.data.last_killed_piece_idx] = p1;
        }
        b.data.last_killed_piece_idx = deadidx;

        if (promo == PAWN_ROOK) {
            piecetype = ((piecetype & (WHITE | BLACK)) ^ ROOK) | PAWN;
        }
        else if (promo == PAWN_BISHOP) {
            piecetype = ((piecetype & (WHITE | BLACK)) ^ BISHOP) | PAWN;
        }

        b.data.board_0  [b.data.transform_array[0][p1]] = deadpiece;
        b.data.board_90 [b.data.transform_array[1][p1]] = deadpiece;
        b.data.board_180[b.data.transform_array[2][p1]] = deadpiece;
        b.data.board_270[b.data.transform_array[3][p1]] = deadpiece;

        b.data.board_0  [b.data.transform_array[0][p0]] = piecetype;
        b.data.board_90 [b.data.transform_array[1][p0]] = piecetype;
        b.data.board_180[b.data.transform_array[2][p0]] = piecetype;
        b.data.board_270[b.data.transform_array[3][p0]] = piecetype;

    }

    
    void flip_player_(Board &b) {
        b.data.player_to_play = (PlayerColor)(b.data.player_to_play ^ (WHITE | BLACK));
    }

};



#define cw_180_pos_2(p) cw_180_8x8[((int)p)%64]
#define acw_90_pos_2(p) acw_90_8x8[((int)p)%64]
#define cw_90_pos_2(p) cw_90_8x8[((int)p)%64]


//@ShlokDotPande this is the function you need to edit
int engine_b2::get_pawn_score_white(U8 P)
{
    return pawn_scores[P];
}

int engine_b2::get_pawn_score_black(U8 P1)
{
    // int idx = (int) P1;
    U8 P = cw_180_pos_2(P1);

    return get_pawn_score_white(P);
}

int engine_b2::get_bishop_score(U8 P)
{
//    bishop_scores[pos(6, 1)] = 3;

    if (bishop_scores.find(P) != bishop_scores.end())
    {
        return bishop_scores[P];
    }

    U8 P1 = cw_90_pos_2(P);
    U8 P2 = cw_180_pos_2(P);
    U8 P3 = acw_90_pos_2(P);
    if (bishop_scores.find(P1) != bishop_scores.end())
    {
        return bishop_scores[P1];
    }
    if (bishop_scores.find(P2) != bishop_scores.end())
    {
        return bishop_scores[P2];
    }
    if (bishop_scores.find(P3) != bishop_scores.end())
    {
        return bishop_scores[P3];
    }

    cout<<"no matching position for bishop\n";
    cout<<getx(P)<<" "<<gety(P)<<endl;
    cout<<'\n';
    return -1;
}

int engine_b2::get_rook_score_white(U8 P)
{

    if (rook_scores.find(P) != rook_scores.end())
    {
        return rook_scores[P];
    }

    U8 P1 = cw_90_pos_2(P);
    U8 P2 = cw_180_pos_2(P);
    U8 P3 = acw_90_pos_2(P);
    if (rook_scores.find(P1) != rook_scores.end())
    {
        return rook_scores[P1];
    }
    if (rook_scores.find(P2) != rook_scores.end())
    {
        return rook_scores[P2]+4;
    }
    if (rook_scores.find(P3) != rook_scores.end())
    {
        return rook_scores[P3]+4;
    }

    cout<<"no matching position for rook";
    return -1;
}

int engine_b2::get_rook_score_black(U8 P)
{
    U8 P1 = cw_180_pos_2(P);
    return get_rook_score_white(P1);
}

int engine_b2::get_king_score_white(U8 P)
{
    if ((gety(P) == 1 && getx(P) >= 2 && getx(P) <= 6) || (gety(P) == 0 && getx(P) >= 1 && getx(P) <= 7))
    {
        return king_scores[P];
    }

    U8 P1 = cw_90_pos_2(P);
    U8 P2 = cw_180_pos_2(P);
    U8 P3 = acw_90_pos_2(P);
    if ((gety(P1) == 1 && getx(P1) >= 2 && getx(P1) <= 6) || (gety(P1) == 0 && getx(P1) >= 1 && getx(P1) <= 7))
    {
        return king_scores[P1]-1;
    }
    if ((gety(P2) == 1 && getx(P2) >= 2 && getx(P2) <= 6) || (gety(P2) == 0 && getx(P2) >= 1 && getx(P2) <= 7))
    {
        return king_scores[P2]-1;
    }
    if ((gety(P3) == 1 && getx(P3) >= 2 && getx(P3) <= 6) || (gety(P3) == 0 && getx(P3) >= 1 && getx(P3) <= 7))
    {
        return king_scores[P3]+1;
    }

    cout<<"no matching position for king x, y:- " << getx(P) << " " << gety(P) << endl;
    return -1;
}


int engine_b2::get_king_score_black(U8 P)
{
    U8 P1 = cw_180_pos_2(P);
    return get_king_score_white(P1);
}


// chrono::high_resolution_clock::time_point start_time;
// int time_left_to_match;
// this is the main function !
bool engine_b2::isTimeValid() {
    this_thread::sleep_for(std::chrono::milliseconds(1950));
    return chrono::high_resolution_clock::now() - start_time < chrono::milliseconds(2000);
}


int engine_b2::calculate_material(const Board& b) {
    int material = 0;

    if (b.data.w_pawn_1 != DEAD)
    {
        if (b.data.board_0[b.data.w_pawn_1] == (PAWN | WHITE))
            material += 1;
        else
        {
            if (b.data.board_0[b.data.w_pawn_1] == (ROOK | WHITE))
                material += 5;
            else if (b.data.board_0[b.data.w_pawn_1] == (BISHOP | WHITE))
                material += 3;
            else if (b.data.board_0[b.data.w_pawn_1] == (KNIGHT | WHITE))
                material += 4;
        }
    }
    if (b.data.w_pawn_2 != DEAD)
    {
        if (b.data.board_0[b.data.w_pawn_2] == (PAWN | WHITE))
            material += 1;
        else
        {
            if (b.data.board_0[b.data.w_pawn_2] == (ROOK | WHITE))
                material += 5;
            else if (b.data.board_0[b.data.w_pawn_2] == (BISHOP | WHITE))
                material += 3;
            else if (b.data.board_0[b.data.w_pawn_2] == (KNIGHT | WHITE))
                material += 4;
        }
    }
    if (b.data.w_pawn_3 != DEAD)
    {
        if (b.data.board_0[b.data.w_pawn_3] == (PAWN | WHITE))
            material += 1;
        else
        {
            if (b.data.board_0[b.data.w_pawn_3] == (ROOK | WHITE))
                material += 5;
            else if (b.data.board_0[b.data.w_pawn_3] == (BISHOP | WHITE))
                material += 3;
            else if (b.data.board_0[b.data.w_pawn_3] == (KNIGHT | WHITE))
                material += 4;
        }
    }
    if (b.data.w_pawn_4 != DEAD)
    {
        if (b.data.board_0[b.data.w_pawn_4] == (PAWN | WHITE))
            material += 1;
        else
        {
            if (b.data.board_0[b.data.w_pawn_4] == (ROOK | WHITE))
                material += 5;
            else if (b.data.board_0[b.data.w_pawn_4] == (BISHOP | WHITE))
                material += 3;
            else if (b.data.board_0[b.data.w_pawn_4] == (KNIGHT | WHITE))
                material += 4;
        }
    }
    if (b.data.w_bishop != DEAD)
        material += 3;
//    if (b.data.w_knight_1 != DEAD)
//        material += 4;
//    if (b.data.w_knight_2 != DEAD)
//        material += 4;
    if (b.data.w_rook_1 != DEAD)
        material += 5;
    if (b.data.w_rook_2 != DEAD)
        material += 5;


    if (b.data.b_pawn_1 != DEAD)
    {
        if (b.data.board_0[b.data.b_pawn_1] == (PAWN | BLACK))
            material -= 1;
        else
        {
            if (b.data.board_0[b.data.b_pawn_1] == (ROOK | BLACK))
                material -= 5;
            else if (b.data.board_0[b.data.b_pawn_1] == (BISHOP | BLACK))
                material -= 3;
            else if (b.data.board_0[b.data.b_pawn_1] == (KNIGHT | BLACK))
                material -= 4;
        }
    }
    if (b.data.b_pawn_2 != DEAD)
    {
        if (b.data.board_0[b.data.b_pawn_2] == (PAWN | BLACK))
            material -= 1;
        else
        {
            if (b.data.board_0[b.data.b_pawn_2] == (ROOK | BLACK))
                material -= 5;
            else if (b.data.board_0[b.data.b_pawn_2] == (BISHOP | BLACK))
                material -= 3;
            else if (b.data.board_0[b.data.b_pawn_2] == (KNIGHT | BLACK))
                material -= 4;
        }
    }
    if (b.data.b_pawn_3 != DEAD)
    {
        if (b.data.board_0[b.data.b_pawn_3] == (PAWN | BLACK))
            material -= 1;
        else
        {
            if (b.data.board_0[b.data.b_pawn_3] == (ROOK | BLACK))
                material -= 5;
            else if (b.data.board_0[b.data.b_pawn_3] == (BISHOP | BLACK))
                material -= 3;
            else if (b.data.board_0[b.data.b_pawn_3] == (KNIGHT | BLACK))
                material -= 4;
        }
    }
    if (b.data.b_pawn_4 != DEAD)
    {
        if (b.data.board_0[b.data.b_pawn_4] == (PAWN | BLACK))
            material -= 1;
        else
        {
            if (b.data.board_0[b.data.b_pawn_4] == (ROOK | BLACK))
                material -= 5;
            else if (b.data.board_0[b.data.b_pawn_4] == (BISHOP | BLACK))
                material -= 3;
            else if (b.data.board_0[b.data.b_pawn_4] == (KNIGHT | BLACK))
                material -= 4;
        }
    }

    if (b.data.b_bishop != DEAD)
        material -= 3;

//    if (b.data.b_knight_1 != DEAD)
//        material -= 4;
//    if (b.data.b_knight_2 != DEAD)
//        material -= 4;

    if (b.data.b_rook_1 != DEAD)
        material -= 5;
    if (b.data.b_rook_2 != DEAD)
        material -= 5;

    return material;
}


int engine_b2::calc_check_score(const Board& b)
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


int engine_b2::calc_positional_score(const Board &b){
    int positional_score = 0;

    if (b.data.w_pawn_1 != DEAD)
    {
        if (b.data.board_0[b.data.w_pawn_1] == (PAWN | WHITE))
            positional_score += 5*(get_pawn_score_white(b.data.w_pawn_1)+2);
        else
        {
            if (b.data.board_0[b.data.w_pawn_1] == (ROOK | WHITE))
                positional_score += get_rook_score_white(b.data.w_pawn_1);
            else if (b.data.board_0[b.data.w_pawn_1] == (BISHOP | WHITE))
                positional_score += 0.5*get_bishop_score(b.data.w_pawn_1);
        }
    }
    if (b.data.w_pawn_2 != DEAD)
    {
        if (b.data.board_0[b.data.w_pawn_2] == (PAWN | WHITE))
            positional_score += 5*(get_pawn_score_white(b.data.w_pawn_2)+2);
        else
        {
            if (b.data.board_0[b.data.w_pawn_2] == (ROOK | WHITE))
                positional_score += get_rook_score_white(b.data.w_pawn_2);
            else if (b.data.board_0[b.data.w_pawn_2] == (BISHOP | WHITE))
                positional_score += 0.5*get_bishop_score(b.data.w_pawn_2);
        }
    }
    if (b.data.w_pawn_3 != DEAD)
    {
        if (b.data.board_0[b.data.w_pawn_3] == (PAWN | WHITE))
            positional_score += 5*(get_pawn_score_white(b.data.w_pawn_3)+2);
        else
        {
            if (b.data.board_0[b.data.w_pawn_3] == (ROOK | WHITE))
                positional_score += get_rook_score_white(b.data.w_pawn_3);
            else if (b.data.board_0[b.data.w_pawn_3] == (BISHOP | WHITE))
                positional_score += 0.5*get_bishop_score(b.data.w_pawn_3);
        }
    }
    if (b.data.w_pawn_4 != DEAD)
    {
        if (b.data.board_0[b.data.w_pawn_4] == (PAWN | WHITE))
            positional_score += 5*(get_pawn_score_white(b.data.w_pawn_4)+2);
        else
        {
            if (b.data.board_0[b.data.w_pawn_4] == (ROOK | WHITE))
                positional_score += get_rook_score_white(b.data.w_pawn_4);
            else if (b.data.board_0[b.data.w_pawn_4] == (BISHOP | WHITE))
                positional_score += 0.5*get_bishop_score(b.data.w_pawn_4);
        }
    }

    if (b.data.w_bishop != DEAD)
        positional_score += 0.5*get_bishop_score(b.data.w_bishop);
    if (b.data.w_rook_1 != DEAD)
        positional_score += get_rook_score_white(b.data.w_rook_1);
    if (b.data.w_rook_2 != DEAD)
        positional_score += get_rook_score_white(b.data.w_rook_2);

    if (b.data.b_pawn_1 != DEAD)
    {
        if (b.data.board_0[b.data.b_pawn_1] == (PAWN | BLACK))
            positional_score -= 5*(get_pawn_score_black(b.data.b_pawn_1)+2);
        else
        {
            if (b.data.board_0[b.data.b_pawn_1] == (ROOK | BLACK))
                positional_score -= get_rook_score_black(b.data.b_pawn_1);
            else if (b.data.board_0[b.data.b_pawn_1] == (BISHOP | BLACK))
                positional_score -= 0.5*get_bishop_score(b.data.b_pawn_1);
        }
    }
    if (b.data.b_pawn_2 != DEAD)
    {
        if (b.data.board_0[b.data.b_pawn_2] == (PAWN | BLACK))
            positional_score -= 5*(get_pawn_score_black(b.data.b_pawn_2)+2);
        else
        {
            if (b.data.board_0[b.data.b_pawn_2] == (ROOK | BLACK))
                positional_score -= get_rook_score_black(b.data.b_pawn_2);
            else if (b.data.board_0[b.data.b_pawn_2] == (BISHOP | BLACK))
                positional_score -= 0.5*get_bishop_score(b.data.b_pawn_2);
        }
    }
    if (b.data.b_pawn_3 != DEAD)
    {
        if (b.data.board_0[b.data.b_pawn_3] == (PAWN | BLACK))
            positional_score -= 5*(get_pawn_score_black(b.data.b_pawn_3)+2);
        else
        {
            if (b.data.board_0[b.data.b_pawn_3] == (ROOK | BLACK))
                positional_score -= get_rook_score_black(b.data.b_pawn_3);
            else if (b.data.board_0[b.data.b_pawn_3] == (BISHOP | BLACK))
                positional_score -= 0.5*get_bishop_score(b.data.b_pawn_3);
        }
    }
    if (b.data.b_pawn_4 != DEAD)
    {
        if (b.data.board_0[b.data.b_pawn_4] == (PAWN | BLACK))
            positional_score -= 5*(get_pawn_score_black(b.data.b_pawn_4)+2);
        else
        {
            if (b.data.board_0[b.data.b_pawn_4] == (ROOK | BLACK))
                positional_score -= get_rook_score_black(b.data.b_pawn_4);
            else if (b.data.board_0[b.data.b_pawn_4] == (BISHOP | BLACK))
                positional_score -= 0.5*get_bishop_score(b.data.b_pawn_4);
        }
    }

    if (b.data.b_bishop != DEAD)
        positional_score -= 0.5*get_bishop_score(b.data.b_bishop);

    if (b.data.b_rook_1 != DEAD)
        positional_score -= get_rook_score_black(b.data.b_rook_1);
    if (b.data.b_rook_2 != DEAD)
        positional_score -= get_rook_score_black(b.data.b_rook_2);

    positional_score += get_king_score_white(b.data.w_king);
    positional_score -= get_king_score_black(b.data.b_king);

    return positional_score;
}


int engine_b2::pawn_closeness_score_white(const Board& b)
{
    int score1 = 12;

    auto pos1 = b.data.w_pawn_1;

    auto pos2 = b.data.w_pawn_2;

    if (pos1 != DEAD && pos2 != DEAD && b.data.board_0[pos2] == (WHITE|PAWN) && b.data.board_0[pos1] == (WHITE|PAWN))
    {
        score1 -= abs(gety(pos1) - gety(pos2));
        if (gety(pos1) == 6 || gety(pos1) == 7 || gety(pos2) == 6 || gety(pos2) == 7)
            score1 -= abs(getx(pos1) - getx(pos2));

    }
    else
        score1 = 0;

    pos1 = b.data.w_pawn_3;

    pos2 = b.data.w_pawn_4;
    int score2 = 12;
    if (pos1 != DEAD && pos2 != DEAD && b.data.board_0[pos2] == (WHITE|PAWN) && b.data.board_0[pos1] == (WHITE|PAWN))
    {
        score2 -= abs(gety(pos1) - gety(pos2));
        if (gety(pos1) == 6 || gety(pos1) == 7 || gety(pos2) == 6 || gety(pos2) == 7)
            score2 -= abs(getx(pos1) - getx(pos2));
    }
    else
        score2 = 0;

    return score1 + score2;
}


int engine_b2::pawn_closeness_score_black(const Board& b)
{
    int score1 = 12;

    auto pos1 = b.data.b_pawn_1;

    auto pos2 = b.data.b_pawn_2;

    if (pos1 != DEAD && pos2 != DEAD && b.data.board_0[pos2] == (BLACK|PAWN) && b.data.board_0[pos1] == (BLACK|PAWN))
    {
        score1 -= abs(gety(pos1) - gety(pos2));
        if (gety(pos1) == 6 || gety(pos1) == 7 || gety(pos2) == 6 || gety(pos2) == 7)
            score1 -= abs(getx(pos1) - getx(pos2));
    }
    else
        score1 = 0;

    pos1 = b.data.b_pawn_3;

    pos2 = b.data.b_pawn_4;
    int score2 = 12;
    if (pos1 != DEAD && pos2 != DEAD && b.data.board_0[pos2] == (BLACK|PAWN) && b.data.board_0[pos1] == (BLACK|PAWN))
    {
        score2 -= abs(gety(pos1) - gety(pos2));
        if (gety(pos1) == 6 || gety(pos1) == 7 || gety(pos2) == 6 || gety(pos2) == 7)
            score1 -= abs(getx(pos1) - getx(pos2));
    }
    else
        score2 = 0;

    return score1 + score2;
}


int engine_b2::evaluate_function(const Board& b)
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
    int w1 = 110;

    int pawn_score = 0; // count_pawn_score(b); // range -20 to +20
    int w2 = 6;
    
    
    int check_score = calc_check_score(b); // -10 or 10
    int w3 = 3;


    // wrong code for count_pawn_score, uses b.data.w_pawn_ws and b.data.w_pawn_bs
    int protected_score = 0;  // how_many_protected_score(b);  // -1 to 1
    int w4 = 8;


    int positional_score = calc_positional_score(b); // range -20/-13 to +20/+13 (for rook)
    // -9 to 9 for pawn, -8 to 8 for bishop
    int w5 = 3;

    int pawn_closeness_score = pawn_closeness_score_white(b) - pawn_closeness_score_black(b);
    int w6 = 2;

    int final_score = (w1*material) + (w2*pawn_score) + (w3*check_score) + (w4*protected_score) + (w5*positional_score) + (w6*pawn_closeness_score);
    return final_score;
}


int engine_b2::eval2(Board& b, U16 move){
    // Board b_copy = Board(b);
    // b_copy.do_move_(move);

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

    U8 deadpiece = b.data.last_killed_piece;
    int deadidx = b.data.last_killed_piece_idx;
    b.do_move_(move);

    
    int material = calculate_material(b);  // range -5 to +5
    int w1 = 110;

    int check_score = calc_check_score(b); // -10 or 10
    int w3 = 3;

    int positional_score = calc_positional_score(b); // range -20/-13 to +20/+13 (for rook)
    // -9 to 9 for pawn, -8 to 8 for bishop
    int w5 = 3;

    int pawn_closeness_score = pawn_closeness_score_white(b) - pawn_closeness_score_black(b);
    int w6 = 2;

    int final_score = (w1*material) + (w3*check_score) + (w5*positional_score) + (w6*pawn_closeness_score);
    undoMove(b, move, deadpiece, deadidx);
    flip_player_(b);
    return final_score;
}
//pair<int, U16> engine_b2::Min_value(Board b, int depth, int alpha, int beta, Engine* e);

pair<int, U16> engine_b2::Max_value(Board& b, int depth, int alpha, int beta, Engine* e)
{

    if (depth > MAX_DEPTH)
    {
        return make_pair(evaluate_function(b), 0);
    }
    else
    {
        U16 best_move = 0;
        auto moveset = b.get_legal_moves();
        if (moveset.empty()) {
            if (b.in_check()) {
                if (b.data.player_to_play == WHITE)
                    return make_pair(-100000, U16(e->best_move));
                else
                    return make_pair(100000, U16(e->best_move));
            }
            else
                return make_pair(0, best_move);
        }
        else {

            int max_value = -100050;
            if (time->get_elapsed_time() < timeAllotted){
                vector<pair<int,  U16> > temp;
                for (auto move: moveset) {
                    temp.push_back(make_pair(eval2(b, move), move));
                }
                sort(temp.begin(), temp.end());
                reverse(temp.begin(), temp.end());
                for (auto m1: temp) {
                    U16 m = m1.second;
                    // Board b_copy = Board(b); // create a copy constructor??
                    // b_copy.do_move_(m);

                    U8 deadpiece = b.data.last_killed_piece;
                    int deadidx = b.data.last_killed_piece_idx;
                    b.do_move_(m);
                    auto min_ans = Min_value(b, depth + 1, alpha, beta, e);
                    undoMove(b, m, deadpiece, deadidx);
                    flip_player_(b);
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

            return make_pair(max_value, best_move);
        }
    }
}

pair<int, U16> engine_b2::Min_value(Board& b, int depth, int alpha, int beta, Engine* e)
{

    if (depth > MAX_DEPTH) {
        return make_pair(evaluate_function(b), U16(e->best_move));
    }
    else {
        U16 best_move = 0;
        auto moveset = b.get_legal_moves();
        if (moveset.empty()) {
            if (b.in_check()) {
                if (b.data.player_to_play == WHITE)
                    return make_pair(-100000, U16(e->best_move));
                else
                    return make_pair(100000, U16(e->best_move));
            }
            else
                return make_pair(0, U16(e->best_move));
        }
        else {
            int min_value = 100050;
            if (time->get_elapsed_time() < timeAllotted){
                vector<pair<int,  U16> > temp;
                for (auto move: moveset) {
                    temp.push_back(make_pair(eval2(b, move), move));
                }
                sort(temp.begin(), temp.end());
                for (auto m1: temp) {
                    U16 m = m1.second;

                    // Board b_copy = Board(b);
                    // b_copy.do_move_(m);


                    U8 deadpiece = b.data.last_killed_piece;
                    int deadidx = b.data.last_killed_piece_idx;
                    b.do_move_(m);
                    auto max_ans = Max_value(b, depth + 1, alpha, beta, e);
                    undoMove(b, m, deadpiece, deadidx);
                    flip_player_(b);
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

            return make_pair(min_value, best_move);
        }
    }
}


pair<int, U16> engine_b2::MiniMax(Board& b, PlayerColor colour, Engine* e)
{
    auto moveset = b.get_legal_moves();

    if (moveset.empty()) {
        return make_pair(0, 0);
    }
    else {
        auto ans = make_pair(0, 0);
        if (colour == BLACK)
        {
            ans = Min_value(b, 1, -100000, 100000, e);
//            cout<<"\n\n\n";
//            cout<< "final evaluation is, "<<ans.first<< " at MAX_DEPTH: "<<MAX_DEPTH<<'\n';
//            cout<<"best move at this depth is: "<<move_to_str(ans.second)<<"\n";
//            cout<<"\n\n\n";
            return ans;
        }
        else
        {
            ans = engine_b2::Max_value(b, 1, -100000, 100000, e);
//            cout<<"\n\n\n";
//            cout<< "final evaluation is, "<<ans.first<< " at MAX_DEPTH: "<<MAX_DEPTH<<'\n';
//            cout<<"best move at this depth is: "<<move_to_str(ans.second)<<"\n";
//            cout<<"\n\n\n";
            return ans;
        }
    }
}


U16 engine_b2::return_best_move(const Board &b1, Engine *e) {
    time = new Time1();
    Board b = Board(b1);
    start_time = chrono::high_resolution_clock::now();
    time_left_to_match =  e->time_left.count()/1e3;
    
    if (time_left_to_match > 90){
        timeAllotted = 5.0;
    }
    else if (time_left_to_match < 90 && time_left_to_match > 50){
        timeAllotted = 4.0;
    }
    else if (time_left_to_match <50 && time_left_to_match > 20){
        timeAllotted = min((float) 3.5, time_left_to_match/15);
    }
    else if (time_left_to_match > 2){
        timeAllotted = time_left_to_match/15;
    }
    else {
        auto moveset = b.get_legal_moves();
        for (auto m : moveset){
            return m;
        }
    }

    cout << "time left to match: " << time_left_to_match << endl;
    auto moveset = b.get_legal_moves();

    if (moveset.size() == 1)
    {
        for (auto m : moveset)
        {
            return m;
        }
    }

    auto colour = b.data.player_to_play;

    vector<pair<int, pair<U16, Board>>> moveset_boards;

    MAX_DEPTH = 1;
    while (time->get_elapsed_time() < timeAllotted)
    {

        if ((b.data.player_to_play == WHITE) && ( b.data.board_0[pos(2, 1)] == (WHITE|PAWN)) && (b.data.board_0[pos(1, 2)] == EMPTY) ) {
            e->best_move = (pos(2, 1) << 8) | (pos(1, 2));
            cout << move_to_str(e->best_move) << endl;
            return e->best_move;
        }

        auto p = MiniMax(b, colour, e);

        if (time->get_elapsed_time() >= timeAllotted)
            break;

        e->best_move = p.second;
        if ((p.first == 100000) && (b.data.player_to_play == WHITE))
            break;

        if ((p.first == -100000) && (b.data.player_to_play == BLACK))
            break;

        cout<<"\n\n";
        cout<< "final evaluation is, "<<p.first<< " at MAX_DEPTH: "<<MAX_DEPTH<<'\n';
        cout<<"best move at this depth is: "<<move_to_str(p.second)<<"\n";
        cout<<"\n\n";

        if (MAX_DEPTH == 1)
            MAX_DEPTH++;
        else
            MAX_DEPTH += 1;

    }
    return e->best_move;

}















class Time2{
    chrono::high_resolution_clock::time_point start_time;
    public:
    Time2(){
        start_time = chrono::high_resolution_clock::now();
    }

    float get_elapsed_time(){
        auto end_time = chrono::high_resolution_clock::now();
        auto time_taken = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
        return time_taken/1e3;
    }
};


class engine_b3
{
public:

    int MAX_DEPTH = 3;
    float time_left_to_match;
    Time2* time;
    float timeAllotted;
    chrono::high_resolution_clock::time_point start_time;
    
    map<U8, int> pawn_scores = {
       {pos(5, 0), 0}, {pos(5, 1), 0}, {pos(5, 2), 0},/*                                useless  */ // {pos(5, 5), 13}, {pos(5, 6), 13}, {pos(5, 7), 13},
       {pos(4, 0), 1}, {pos(4, 1), 2}, {pos(4, 2), 2},/*                                         */{pos(4, 5), 12}, {pos(4, 6), 12}, {pos(4, 7), 12},
       {pos(3, 0), 2}, {pos(3, 1), 3}, {pos(3, 2), 3},/*                                         */{pos(3, 5), 11}, {pos(3, 6), 11}, {pos(3, 7), 11},
       {pos(2, 0), 3}, {pos(2, 1), 4}, {pos(2, 2), 5}, {pos(2, 3), 6}, {pos(2, 4), 7}, {pos(2, 5), 8}, {pos(2, 6), 9}, {pos(2, 7), 9},
       {pos(1, 0), 3}, {pos(1, 1), 4}, {pos(1, 1), 5}, {pos(1, 3), 6}, {pos(1, 4), 7}, {pos(1, 5), 8}, {pos(1, 6), 8}, {pos(1, 7), 8},
       {pos(0, 0), 1}, {pos(0, 1), 2}, {pos(0, 0), 3}, {pos(0, 3), 4}, {pos(0, 4), 5}, {pos(0, 5), 6}, {pos(0, 6), 6}, {pos(0, 7), 6},
    };

    map<U8, int> bishop_scores = {


                                        {pos(3, 2), 9}, {pos(4, 2), 6}, {pos(5, 2), 7},
                        {pos(2, 1), 10}, {pos(3, 1), 11}, {pos(4, 1), 8}, {pos(5, 1), 5}, {pos(6, 1), 7},
        {pos(1, 0), 4}, {pos(2, 0), 8}, {pos(3, 0), 8}, {pos(4, 0), 8}, {pos(5, 0), 5}, {pos(6, 0), 5}, {pos(7, 0), 4}
    };

    map<U8, int> rook_scores = {
                                        {pos(3, 2), 5}, {pos(4, 2), 6}, {pos(5, 2), 7},
                        {pos(2, 1), 5}, {pos(3, 1), 6}, {pos(4, 1), 7}, {pos(5, 1), 8}, {pos(6, 1), 9},
        {pos(1, 0), 15}, {pos(2, 0), 14}, {pos(3, 0), 12}, {pos(4, 0), 13}, {pos(5, 0), 14}, {pos(6, 0), 15}, {pos(7, 0), 16}
    };

    map<U8, int> knight_scores = {
                                        {pos(3, 2), 8}, {pos(4, 2), 8}, {pos(5, 2), 7},
                        {pos(2, 1), 6}, {pos(3, 1), 7}, {pos(4, 1), 6}, {pos(5, 1), 6}, {pos(6, 1), 5},
        {pos(1, 0), 2}, {pos(2, 0), 3}, {pos(3, 0), 3}, {pos(4, 0), 3}, {pos(5, 0), 3}, {pos(6, 0), 2}, {pos(7, 0), 1}
    };

    map<U8, int> king_scores = {
                                        {pos(3, 2), 4}, {pos(4, 2), 4}, {pos(5, 2), 4},
                        {pos(2, 1), 4}, {pos(3, 1), 4}, {pos(4, 1), 4}, {pos(5, 1), 4}, {pos(6, 1), 4},
        {pos(1, 0), 2}, {pos(2, 0), 2}, {pos(3, 0), 2}, {pos(4, 0), 2}, {pos(5, 0), 2}, {pos(6, 0), 2}, {pos(7, 0), 2}
    };


    void check(){
        for (int i = 0; i<64; i++){
            U8 P = (U8) i;
            cout << getx(P) << " " << gety(P) << " " << get_knight_score_white(P) << endl;
        }
    }


    int get_pawn_score_white(U8 P);
    int get_pawn_score_black(U8 P);
    int get_bishop_score(U8 P);
    int get_knight_score_white(U8 P);
    int get_knight_score_black(U8 P);
    int get_rook_score_white(U8 P);
    int get_rook_score_black(U8 P);
    int get_king_score_white(U8 P);
    int get_king_score_black(U8 P);
    int pawn_closeness_score_white(const Board& b);
    int pawn_closeness_score_black(const Board& b);


    U16 return_best_move(const Board& b, Engine *e);


    int calc_check_score(const Board& b);
    int calc_positional_score(const Board& b);
    int calculate_material(const Board& b);

    bool isTimeValid();

    pair<int, U16> MiniMax(Board& b, PlayerColor colour, Engine* e);

    pair<int, U16> Min_value(Board& b, int depth, int alpha, int beta, Engine* e);

    pair<int, U16> Max_value(Board& b, int depth, int alpha, int beta, Engine* e);

    int evaluate_function(const Board& b);
    int eval2(Board& b, U16 move);

    void undoMove(Board& b, U16 move, U8 dead, int deadidx) {
        U8 p0 = getp0(move);
        U8 p1 = getp1(move);
        U8 promo = getpromo(move);

        U8 piecetype = b.data.board_0[p1];
        U8 deadpiece = b.data.last_killed_piece;
        b.data.last_killed_piece = dead;

        // scan and get piece from coord
        U8 *pieces = (U8*)(&(b.data));
        for (int i=0; i<2*b.data.n_pieces; i++) {
            if (pieces[i] == p1) {
                pieces[i] = p0;
                break;
            }
        }
        if (b.data.last_killed_piece_idx >= 0) {
            pieces[b.data.last_killed_piece_idx] = p1;
        }
        b.data.last_killed_piece_idx = deadidx;

        if (promo == PAWN_ROOK) {
            piecetype = ((piecetype & (WHITE | BLACK)) ^ ROOK) | PAWN;
        }
        else if (promo == PAWN_BISHOP) {
            piecetype = ((piecetype & (WHITE | BLACK)) ^ BISHOP) | PAWN;
        }

        b.data.board_0  [b.data.transform_array[0][p1]] = deadpiece;
        b.data.board_90 [b.data.transform_array[1][p1]] = deadpiece;
        b.data.board_180[b.data.transform_array[2][p1]] = deadpiece;
        b.data.board_270[b.data.transform_array[3][p1]] = deadpiece;

        b.data.board_0  [b.data.transform_array[0][p0]] = piecetype;
        b.data.board_90 [b.data.transform_array[1][p0]] = piecetype;
        b.data.board_180[b.data.transform_array[2][p0]] = piecetype;
        b.data.board_270[b.data.transform_array[3][p0]] = piecetype;

    }

    
    void flip_player_(Board &b) {
        b.data.player_to_play = (PlayerColor)(b.data.player_to_play ^ (WHITE | BLACK));
    }

};

#define cw_180_pos_3(p) cw_180_8x8[((int)p)%64]
#define acw_90_pos_3(p) acw_90_8x8[((int)p)%64]
#define cw_90_pos_3(p) cw_90_8x8[((int)p)%64]


// isme reflection ki bt ho sakti hai !! @ShlokDotPande
int engine_b3::get_pawn_score_white(U8 P)
{
    return pawn_scores[P];
}

int engine_b3::get_pawn_score_black(U8 P1)
{
    // int idx = (int) P1;
    U8 P = cw_180_pos_3(P1);
    return get_pawn_score_white(P);
}

int engine_b3::get_bishop_score(U8 P)
{
    if (bishop_scores.find(P) != bishop_scores.end())
    {
        return bishop_scores[P];
    }

    U8 P1 = cw_90_pos_3(P);
    U8 P2 = cw_180_pos_3(P);
    U8 P3 = acw_90_pos_3(P);
    if (bishop_scores.find(P1) != bishop_scores.end())
    {
        return bishop_scores[P1];
    }
    if (bishop_scores.find(P2) != bishop_scores.end())
    {
        return bishop_scores[P2];
    }
    if (bishop_scores.find(P3) != bishop_scores.end())
    {
        return bishop_scores[P3];
    }
    cout << getx(P) << " " << gety(P) ;
    cout<<" no matching position for bishop" << endl;
    return -1;
}

int engine_b3::get_rook_score_white(U8 P)
{

    U8 P1 = cw_90_pos_3(P);
    U8 P2 = cw_180_pos_3(P);
    U8 P3 = acw_90_pos_3(P);
    if (rook_scores.find(P) != rook_scores.end())
    {
        return rook_scores[P];
    }

    else if (rook_scores.find(P1) != rook_scores.end())
    {
        return rook_scores[P1];
    }
    else if (rook_scores.find(P2) != rook_scores.end())
    {
        return rook_scores[P2];
    }
    else if (rook_scores.find(P3) != rook_scores.end())
    {
        return rook_scores[P3];
    }

    cout<<"no matching position for rook";
    return -1;
}

int engine_b3::get_rook_score_black(U8 P)
{
    U8 P1 = cw_180_pos_3(P);
    return get_rook_score_white(P1);
}

int engine_b3::get_king_score_white(U8 P)
{
    if ((gety(P) == 2 && getx(P) >= 3 && getx(P) <= 5) ||
            (gety(P) == 1 && getx(P) >= 2 && getx(P) <= 6) ||
                (gety(P) == 0 && getx(P) >= 1 && getx(P) <= 7))
    {
        return king_scores[P];
    }

    U8 P1 = cw_90_pos_3(P);
    U8 P2 = cw_180_pos_3(P);
    U8 P3 = acw_90_pos_3(P);
    if ((gety(P1) == 2 && getx(P1) >= 3 && getx(P1) <= 5) ||
        (gety(P1) == 1 && getx(P1) >= 2 && getx(P1) <= 6) ||
        (gety(P1) == 0 && getx(P1) >= 1 && getx(P1) <= 7))
    {
        return king_scores[P1]-1;
    }
    if ((gety(P2) == 2 && getx(P2) >= 3 && getx(P2) <= 5) ||
        (gety(P2) == 1 && getx(P2) >= 2 && getx(P2) <= 6) ||
        (gety(P2) == 0 && getx(P2) >= 1 && getx(P2) <= 7))
    {
        return king_scores[P2]-1;
    }
    if ((gety(P3) == 2 && getx(P3) >= 3 && getx(P3) <= 5) ||
        (gety(P3) == 1 && getx(P3) >= 2 && getx(P3) <= 6) ||
        (gety(P3) == 0 && getx(P3) >= 1 && getx(P3) <= 7))
    {
        return king_scores[P3]+1;
    }

    cout<<"no matching position for king";
    return -1;
}

int engine_b3::get_king_score_black(U8 P)
{
    U8 P1 = cw_180_pos_3(P);
    return get_king_score_white(P1);
}

// @ShlokDotPande knight scores is how many it can attack !!
int engine_b3::get_knight_score_white(U8 P)
{
    U8 P1 = cw_90_pos_3(P);
    U8 P2 = cw_180_pos_3(P);
    U8 P3 = acw_90_pos_3(P);
    if (knight_scores.find(P) != knight_scores.end())
    {
        return knight_scores[P];
    }
    else if (knight_scores.find(P1) != knight_scores.end())
    {
        return knight_scores[P1];
    }
    else if (knight_scores.find(P2) != knight_scores.end())
    {
        return knight_scores[P2];
    }
    else if (knight_scores.find(P3) != knight_scores.end())
    {
        return knight_scores[P3];
    }

    cout<<"no matching position for knight";
    return -1;
}

int engine_b3::get_knight_score_black(U8 P)
{
    U8 P1 = cw_180_pos_3(P);
    return get_knight_score_white(P1);
}

// chrono::high_resolution_clock::time_point start_time;
// int time_left_to_match;
// this is the main function !
bool engine_b3::isTimeValid() {
    this_thread::sleep_for(std::chrono::milliseconds(1950));
    return chrono::high_resolution_clock::now() - start_time < chrono::milliseconds(2000);
}


int engine_b3::calculate_material(const Board& b) {
    int material = 0;

    if (b.data.w_pawn_1 != DEAD)
    {
        if (b.data.board_0[b.data.w_pawn_1] == (PAWN | WHITE))
            material += 1;
        else
        {
            if (b.data.board_0[b.data.w_pawn_1] == (ROOK | WHITE))
                material += 5;
            else if (b.data.board_0[b.data.w_pawn_1] == (BISHOP | WHITE))
                material += 3;
            else if (b.data.board_0[b.data.w_pawn_1] == (KNIGHT | WHITE))
                material += 4;
        }
    }
    if (b.data.w_pawn_2 != DEAD)
    {
        if (b.data.board_0[b.data.w_pawn_2] == (PAWN | WHITE))
            material += 1;
        else
        {
            if (b.data.board_0[b.data.w_pawn_2] == (ROOK | WHITE))
                material += 5;
            else if (b.data.board_0[b.data.w_pawn_2] == (BISHOP | WHITE))
                material += 3;
            else if (b.data.board_0[b.data.w_pawn_2] == (KNIGHT | WHITE))
                material += 4;
        }
    }
    if (b.data.w_pawn_3 != DEAD)
    {
        if (b.data.board_0[b.data.w_pawn_3] == (PAWN | WHITE))
            material += 1;
        else
        {
            if (b.data.board_0[b.data.w_pawn_3] == (ROOK | WHITE))
                material += 5;
            else if (b.data.board_0[b.data.w_pawn_3] == (BISHOP | WHITE))
                material += 3;
            else if (b.data.board_0[b.data.w_pawn_3] == (KNIGHT | WHITE))
                material += 4;
        }
    }
    if (b.data.w_pawn_4 != DEAD)
    {
        if (b.data.board_0[b.data.w_pawn_4] == (PAWN | WHITE))
            material += 1;
        else
        {
            if (b.data.board_0[b.data.w_pawn_4] == (ROOK | WHITE))
                material += 5;
            else if (b.data.board_0[b.data.w_pawn_4] == (BISHOP | WHITE))
                material += 3;
            else if (b.data.board_0[b.data.w_pawn_4] == (KNIGHT | WHITE))
                material += 4;
        }
    }
    if (b.data.w_bishop != DEAD)
        material += 3;
    if (b.data.w_knight_1 != DEAD)
        material += 4;
    if (b.data.w_knight_2 != DEAD)
        material += 4;
    if (b.data.w_rook_1 != DEAD)
        material += 5;
    if (b.data.w_rook_2 != DEAD)
        material += 5;


    if (b.data.b_pawn_1 != DEAD)
    {
        if (b.data.board_0[b.data.b_pawn_1] == (PAWN | BLACK))
            material -= 1;
        else
        {
            if (b.data.board_0[b.data.b_pawn_1] == (ROOK | BLACK))
                material -= 5;
            else if (b.data.board_0[b.data.b_pawn_1] == (BISHOP | BLACK))
                material -= 3;
            else if (b.data.board_0[b.data.b_pawn_1] == (KNIGHT | BLACK))
                material -= 4;
        }
    }
    if (b.data.b_pawn_2 != DEAD)
    {
        if (b.data.board_0[b.data.b_pawn_2] == (PAWN | BLACK))
            material -= 1;
        else
        {
            if (b.data.board_0[b.data.b_pawn_2] == (ROOK | BLACK))
                material -= 5;
            else if (b.data.board_0[b.data.b_pawn_2] == (BISHOP | BLACK))
                material -= 3;
            else if (b.data.board_0[b.data.b_pawn_2] == (KNIGHT | BLACK))
                material -= 4;
        }
    }
    if (b.data.b_pawn_3 != DEAD)
    {
        if (b.data.board_0[b.data.b_pawn_3] == (PAWN | BLACK))
            material -= 1;
        else
        {
            if (b.data.board_0[b.data.b_pawn_3] == (ROOK | BLACK))
                material -= 5;
            else if (b.data.board_0[b.data.b_pawn_3] == (BISHOP | BLACK))
                material -= 3;
            else if (b.data.board_0[b.data.b_pawn_3] == (KNIGHT | BLACK))
                material -= 4;
        }
    }
    if (b.data.b_pawn_4 != DEAD)
    {
        if (b.data.board_0[b.data.b_pawn_4] == (PAWN | BLACK))
            material -= 1;
        else
        {
            if (b.data.board_0[b.data.b_pawn_4] == (ROOK | BLACK))
                material -= 5;
            else if (b.data.board_0[b.data.b_pawn_4] == (BISHOP | BLACK))
                material -= 3;
            else if (b.data.board_0[b.data.b_pawn_4] == (KNIGHT | BLACK))
                material -= 4;
        }
    }

    if (b.data.b_bishop != DEAD)
        material -= 3;

    if (b.data.b_knight_1 != DEAD)
        material -= 4;
    if (b.data.b_knight_2 != DEAD)
        material -= 4;

    if (b.data.b_rook_1 != DEAD)
        material -= 5;
    if (b.data.b_rook_2 != DEAD)
        material -= 5;

    return material;
}


int engine_b3::calc_check_score(const Board& b)
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


int engine_b3::calc_positional_score(const Board &b){
    int positional_score = 0;

    if (b.data.w_pawn_1 != DEAD)
    {
        if (b.data.board_0[b.data.w_pawn_1] == (PAWN | WHITE))
            positional_score += 5*(get_pawn_score_white(b.data.w_pawn_1)+3);
        else
        {
            if (b.data.board_0[b.data.w_pawn_1] == (ROOK | WHITE))
                positional_score += get_rook_score_white(b.data.w_pawn_1);
            else if (b.data.board_0[b.data.w_pawn_1] == (BISHOP | WHITE))
                positional_score += 0.5*get_bishop_score(b.data.w_pawn_1);
        }
    }
    if (b.data.w_pawn_2 != DEAD)
    {
        if (b.data.board_0[b.data.w_pawn_2] == (PAWN | WHITE))
            positional_score += 5*(get_pawn_score_white(b.data.w_pawn_2)+3);
        else
        {
            if (b.data.board_0[b.data.w_pawn_2] == (ROOK | WHITE))
                positional_score += get_rook_score_white(b.data.w_pawn_2);
            else if (b.data.board_0[b.data.w_pawn_2] == (BISHOP | WHITE))
                positional_score += 0.5*get_bishop_score(b.data.w_pawn_2);
        }
    }
    if (b.data.w_pawn_3 != DEAD)
    {
        if (b.data.board_0[b.data.w_pawn_3] == (PAWN | WHITE))
            positional_score += 5*(get_pawn_score_white(b.data.w_pawn_3)+3);
        else
        {
            if (b.data.board_0[b.data.w_pawn_3] == (ROOK | WHITE))
                positional_score += get_rook_score_white(b.data.w_pawn_3);
            else if (b.data.board_0[b.data.w_pawn_3] == (BISHOP | WHITE))
                positional_score += 0.5*get_bishop_score(b.data.w_pawn_3);
        }
    }
    if (b.data.w_pawn_4 != DEAD)
    {
        if (b.data.board_0[b.data.w_pawn_4] == (PAWN | WHITE))
            positional_score += 5*(get_pawn_score_white(b.data.w_pawn_4)+3);
        else
        {
            if (b.data.board_0[b.data.w_pawn_4] == (ROOK | WHITE))
                positional_score += get_rook_score_white(b.data.w_pawn_4);
            else if (b.data.board_0[b.data.w_pawn_4] == (BISHOP | WHITE))
                positional_score += 0.5*get_bishop_score(b.data.w_pawn_4);
        }
    }

    if (b.data.w_bishop != DEAD)
        positional_score += 0.5*get_bishop_score(b.data.w_bishop);
    if (b.data.w_knight_1 != DEAD)
        positional_score += get_knight_score_white(b.data.w_knight_1);
    if (b.data.w_knight_2 != DEAD)
        positional_score += get_knight_score_white(b.data.w_knight_2);
    if (b.data.w_rook_1 != DEAD)
        positional_score += get_rook_score_white(b.data.w_rook_1);
    if (b.data.w_rook_2 != DEAD)
        positional_score += get_rook_score_white(b.data.w_rook_2);

    if (b.data.b_pawn_1 != DEAD)
    {
        if (b.data.board_0[b.data.b_pawn_1] == (PAWN | BLACK))
            positional_score -= 5*(get_pawn_score_black(b.data.b_pawn_1)+3);
        else
        {
            if (b.data.board_0[b.data.b_pawn_1] == (ROOK | BLACK))
                positional_score -= get_rook_score_black(b.data.b_pawn_1);
            else if (b.data.board_0[b.data.b_pawn_1] == (BISHOP | BLACK))
                positional_score -= 0.5*get_bishop_score(b.data.b_pawn_1);
        }
    }
    if (b.data.b_pawn_2 != DEAD)
    {
        if (b.data.board_0[b.data.b_pawn_2] == (PAWN | BLACK))
            positional_score -= 5*(get_pawn_score_black(b.data.b_pawn_2)+3);
        else
        {
            if (b.data.board_0[b.data.b_pawn_2] == (ROOK | BLACK))
                positional_score -= get_rook_score_black(b.data.b_pawn_2);
            else if (b.data.board_0[b.data.b_pawn_2] == (BISHOP | BLACK))
                positional_score -= 0.5*get_bishop_score(b.data.b_pawn_2);
        }
    }
    if (b.data.b_pawn_3 != DEAD)
    {
        if (b.data.board_0[b.data.b_pawn_3] == (PAWN | BLACK))
            positional_score -= 5*(get_pawn_score_black(b.data.b_pawn_3)+3);
        else
        {
            if (b.data.board_0[b.data.b_pawn_3] == (ROOK | BLACK))
                positional_score -= get_rook_score_black(b.data.b_pawn_3);
            else if (b.data.board_0[b.data.b_pawn_3] == (BISHOP | BLACK))
                positional_score -= 0.5*get_bishop_score(b.data.b_pawn_3);
        }
    }
    if (b.data.b_pawn_4 != DEAD)
    {
        if (b.data.board_0[b.data.b_pawn_4] == (PAWN | BLACK))
            positional_score -= 5*(get_pawn_score_black(b.data.b_pawn_4)+3);
        else
        {
            if (b.data.board_0[b.data.b_pawn_4] == (ROOK | BLACK))
                positional_score -= get_rook_score_black(b.data.b_pawn_4);
            else if (b.data.board_0[b.data.b_pawn_4] == (BISHOP | BLACK))
                positional_score -= 0.5*get_bishop_score(b.data.b_pawn_4);
        }
    }

    if (b.data.b_bishop != DEAD)
        positional_score -= 0.5*get_bishop_score(b.data.b_bishop);
    if (b.data.b_knight_1 != DEAD)
        positional_score -= get_knight_score_black(b.data.b_knight_1);
    if (b.data.b_knight_2 != DEAD)
        positional_score -= get_knight_score_black(b.data.b_knight_2);
    if (b.data.b_rook_1 != DEAD)
        positional_score -= get_rook_score_black(b.data.b_rook_1);
    if (b.data.b_rook_2 != DEAD)
        positional_score -= get_rook_score_black(b.data.b_rook_2);

    positional_score += get_king_score_white(b.data.w_king);
    positional_score -= get_king_score_black(b.data.b_king);

    return positional_score;
}


int engine_b3::pawn_closeness_score_white(const Board& b)
{
    int score1 = 12;

    auto pos1 = b.data.w_pawn_1;

    auto pos2 = b.data.w_pawn_2;

    if (pos1 != DEAD && pos2 != DEAD && b.data.board_0[pos2] == (WHITE|PAWN) && b.data.board_0[pos1] == (WHITE|PAWN))
    {
        score1 -= abs(gety(pos1) - gety(pos2));
        if (gety(pos1) == 6 || gety(pos1) == 7 || gety(pos2) == 6 || gety(pos2) == 7)
            score1 -= abs(getx(pos1) - getx(pos2));

    }
    else
        score1 = 0;

    pos1 = b.data.w_pawn_3;

    int score2 = 12;
    if (pos1 != DEAD && pos2 != DEAD && b.data.board_0[pos2] == (WHITE|PAWN) && b.data.board_0[pos1] == (WHITE|PAWN))
    {
        score2 -= abs(gety(pos1) - gety(pos2));
        if (gety(pos1) == 6 || gety(pos1) == 7 || gety(pos2) == 6 || gety(pos2) == 7)
            score2 -= abs(getx(pos1) - getx(pos2));
    }
    else
        score2 = 0;

    return score1 + score2;
}


int engine_b3::pawn_closeness_score_black(const Board& b)
{
    int score1 = 12;

    auto pos1 = b.data.b_pawn_1;

    auto pos2 = b.data.b_pawn_2;

    if (pos1 != DEAD && pos2 != DEAD && b.data.board_0[pos2] == (BLACK|PAWN) && b.data.board_0[pos1] == (BLACK|PAWN))
    {
        score1 -= abs(gety(pos1) - gety(pos2));
        if (gety(pos1) == 6 || gety(pos1) == 7 || gety(pos2) == 6 || gety(pos2) == 7)
            score1 -= abs(getx(pos1) - getx(pos2));
    }
    else
        score1 = 0;

    pos1 = b.data.b_pawn_3;

    int score2 = 12;
    if (pos1 != DEAD && pos2 != DEAD && b.data.board_0[pos2] == (BLACK|PAWN) && b.data.board_0[pos1] == (BLACK|PAWN))
    {
        score2 -= abs(gety(pos1) - gety(pos2));
        if (gety(pos1) == 6 || gety(pos1) == 7 || gety(pos2) == 6 || gety(pos2) == 7)
            score1 -= abs(getx(pos1) - getx(pos2));
    }
    else
        score2 = 0;

    return score1 + score2;
}


int engine_b3::evaluate_function(const Board& b)
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
    int w1 = 112;

    int pawn_score = 0; // count_pawn_score(b); // range -20 to +20
    int w2 = 6;
    
    
    int check_score = calc_check_score(b); // -10 or 10
    int w3 = 3;


    // wrong code for count_pawn_score, uses b.data.w_pawn_ws and b.data.w_pawn_bs
    int protected_score = 0;  // how_many_protected_score(b);  // -1 to 1
    int w4 = 8;


    int positional_score = calc_positional_score(b); // range -20/-13 to +20/+13 (for rook)
    // -9 to 9 for pawn, -8 to 8 for bishop
    int w5 = 3;


    int pawn_closeness_score = pawn_closeness_score_white(b) - pawn_closeness_score_black(b);
    int w6 = 2;

    int final_score = (w1*material) + (w2*pawn_score) + (w3*check_score) + (w4*protected_score) + (w5*positional_score) + (w6*pawn_closeness_score);
    return final_score;
}


int engine_b3::eval2(Board& b, U16 move){
    // Board b_copy = Board(b);
    // b_copy.do_move_(move);
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

    U8 deadpiece = b.data.last_killed_piece;
    int deadidx = b.data.last_killed_piece_idx;
    b.do_move_(move);
    int material = calculate_material(b);  // range -5 to +5
    int w1 = 112;

    int check_score = calc_check_score(b); // -10 or 10
    int w3 = 3;

    int positional_score = calc_positional_score(b); // range -20/-13 to +20/+13 (for rook)
    // -9 to 9 for pawn, -8 to 8 for bishop
    int w5 = 3;

    int pawn_closeness_score = pawn_closeness_score_white(b) - pawn_closeness_score_black(b);
    int w6 = 2;

    int final_score = (w1*material) + (w3*check_score) + (w5*positional_score) + (w6*pawn_closeness_score);
    undoMove(b, move, deadpiece, deadidx);
    flip_player_(b);
    return final_score;
}

//pair<int, U16> engine_b3::Min_value(Board b, int depth, int alpha, int beta, Engine* e);

pair<int, U16> engine_b3::Max_value(Board& b, int depth, int alpha, int beta, Engine* e)
{

    if (depth > MAX_DEPTH)
    {
        return make_pair(evaluate_function(b), 0);
    }
    else
    {
        U16 best_move = 0;
        auto moveset = b.get_legal_moves();
        if (moveset.empty()) {
            if (b.in_check()) {
                if (b.data.player_to_play == WHITE)
                    return make_pair(-100000, U16(e->best_move));
                else
                    return make_pair(100000, U16(e->best_move));
            }
            else
                return make_pair(0, best_move);
        }
        else {

            int max_value = -100050;
            if (time->get_elapsed_time() < timeAllotted){
                vector<pair<int,  U16> > temp;
                for (auto move: moveset) {
                    temp.push_back(make_pair(eval2(b, move), move));
                }
                sort(temp.begin(), temp.end());
                reverse(temp.begin(), temp.end());
                for (auto m1: temp) {
                    U16 m = m1.second;
                    // Board b_copy = Board(b); // create a copy constructor??
                    // b_copy.do_move_(m);


                    U8 deadpiece = b.data.last_killed_piece;
                    int deadidx = b.data.last_killed_piece_idx;
                    b.do_move_(m);
                    auto min_ans = Min_value(b, depth + 1, alpha, beta, e);
                    undoMove(b, m, deadpiece, deadidx);
                    flip_player_(b);
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

            return make_pair(max_value, best_move);
        }
    }
}

pair<int, U16> engine_b3::Min_value(Board& b, int depth, int alpha, int beta, Engine* e)
{

    if (depth > MAX_DEPTH) {
        return make_pair(evaluate_function(b), U16(e->best_move));
    }
    else {
        U16 best_move = 0;
        auto moveset = b.get_legal_moves();
        if (moveset.empty()) {
            if (b.in_check()) {
                if (b.data.player_to_play == WHITE)
                    return make_pair(-100000, U16(e->best_move));
                else
                    return make_pair(100000, U16(e->best_move));
            }
            else
                return make_pair(0, U16(e->best_move));
        }
        else {
            int min_value = 100050;
            if (time->get_elapsed_time() < timeAllotted){
                vector<pair<int,  U16> > temp;
                for (auto move: moveset) {
                    temp.push_back(make_pair(eval2(b, move), move));
                }
                sort(temp.begin(), temp.end());
                for (auto m1: temp) {
                    U16 m = m1.second;

                    // Board b_copy = Board(b);
                    // b_copy.do_move_(m);


                    U8 deadpiece = b.data.last_killed_piece;
                    int deadidx = b.data.last_killed_piece_idx;
                    b.do_move_(m);
                    auto max_ans = Max_value(b, depth + 1, alpha, beta, e);
                    undoMove(b, m, deadpiece, deadidx);
                    flip_player_(b);
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

    //            cout<<"\n";
    //            cout<<"Black final eval:"<<min_value<<move_to_str(best_move);
    //            cout<<"corresponding white best move"<<move_to_str(white_best_move);
    //            cout<<"\n";
            }
            return make_pair(min_value, best_move);
        }
    }
}


pair<int, U16> engine_b3::MiniMax(Board& b, PlayerColor colour, Engine* e)
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
//            cout<<"\n\n";
//            cout<< "final evaluation is, "<<ans.first<< " at MAX_DEPTH: "<<MAX_DEPTH<<'\n';
//            cout<<"best move at this depth is: "<<move_to_str(ans.second)<<"\n";
//            cout<<"\n\n";
            return ans;
        }
        else
        {
            ans = engine_b3::Max_value(b, 0, -100000, 100000, e);
//            cout<<"\n\n";
//            cout<< "final evaluation is, "<<ans.first<< " at MAX_DEPTH: "<<MAX_DEPTH<<'\n';
//            cout<<"best move at this depth is: "<<move_to_str(ans.second)<<"\n";
//            cout<<"\n\n";
            return ans;
        }
    }
}


U16 engine_b3::return_best_move(const Board &b1, Engine *e) {
    time = new Time2();
    Board b = Board(b1);
    start_time = chrono::high_resolution_clock::now();
    time_left_to_match =  e->time_left.count()/1e3;
    
    if (time_left_to_match > 90){
        timeAllotted = 5.0;
    }
    else if (time_left_to_match < 90 && time_left_to_match > 50){
        timeAllotted = 4.0;
    }
    else if (time_left_to_match <50 && time_left_to_match > 20){
        timeAllotted = min((float) 3.5, time_left_to_match/15);
    }
    else if (time_left_to_match > 2){
        timeAllotted = time_left_to_match/15;
    }
    else {
        auto moveset = b.get_legal_moves();
        for (auto m : moveset){
            return m;
        }
    }

    cout << "time left to match: " << time_left_to_match << endl;
    auto moveset = b.get_legal_moves();

    if (moveset.size() == 1)
    {
        for (auto m : moveset)
            return m;
    }

    auto colour = b.data.player_to_play;
    vector<pair<int, pair<U16, Board>>> moveset_boards;

    MAX_DEPTH = 1;
    while (time->get_elapsed_time() < timeAllotted)
    {

        if ((b.data.player_to_play == WHITE) && ( b.data.board_0[pos(2, 0)] == (WHITE|PAWN)) && (b.data.board_0[pos(1, 1)] == EMPTY) ) {
            e->best_move = (pos(2, 0) << 8) | (pos(1, 1));
            cout << move_to_str(e->best_move) << endl;
            return e->best_move;
        }

        auto p = MiniMax(b, colour, e);

        if (time->get_elapsed_time() >= timeAllotted)
            break;

        e->best_move = p.second;
        if ((p.first == 100000) && (b.data.player_to_play == WHITE))
        {
            cout<<"here\n";
            break;
        }

        if ((p.first == -100000) && (b.data.player_to_play == BLACK))
        {
            cout<<"there\n";
            break;
        }


        if (MAX_DEPTH == 1)
            MAX_DEPTH++;
        else
            MAX_DEPTH += 1;

        cout<<"\n\n";
        cout<< "final evaluation is, "<<p.first<< " at MAX_DEPTH: "<<MAX_DEPTH<<'\n';
        cout<<"best move at this depth is: "<<move_to_str(p.second)<<"\n";
        cout<<"\n\n";

    }

    return e->best_move;
}





engine_b1 E1;
engine_b2 E2;
engine_b3 E3;

void Engine::find_best_move(const Board& b) {

    auto moveset = b.get_legal_moves();
    // check functions for debugging
    // E3.check();
    if (moveset.size() == 0) {
        std::cout << "Could not get any moves from board!\n";
        std::cout << board_to_str(&b.data);
        this->best_move = 0;
    }

    else {

        if (b.data.board_type == 1){
            this->best_move = E1.return_best_move(b, this);
        }
        else if (b.data.board_type == 2){
            this->best_move = E2.return_best_move(b, this);
        }
        else if (b.data.board_type == 3){
            this->best_move = E3.return_best_move(b, this);
        }

    }

    // std::this_thread::sleep_for(std::chrono::milliseconds(2000));

//        std::vector<U16> moves;
//        std::cout << show_moves(&b.data, moveset) << std::endl;
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

    // just for debugging, to slow down the moves
}
