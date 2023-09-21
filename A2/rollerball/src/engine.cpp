#include <algorithm>
#include <random>
#include <iostream>

#include "board.hpp"
#include "engine.hpp"

int move_number = 0;
using namespace std;

# define MAX_DEPTH 3
// actual depth is this value + 1. So if MAX_DEPTH = 3, then depth = 4

int calculate_material(const Board& b)
{
    int material = 0;
    string board_str = board_to_str(b.data.board_0);
//    cout<<"\n";
//    cout<<board_str;
//    cout<<"\n";

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
    }
    return material;
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
    int material = calculate_material(b);
    int w1 = 10;
    int pawn_score = count_pawn_score(b);
    int w2 = 5;
    int check_score = calc_check_score(b);
    int w3 = 5;
    return (w1*material + w2*pawn_score + w3*check_score);
}

pair<int, U16> Min_value(Board b, int depth, int alpha, int beta, Engine* e);

pair<int, U16> Max_value(Board b, int depth, int alpha, int beta, Engine* e)
{
    if (depth > MAX_DEPTH)
    {
        return make_pair(evaluate_function(b), 0);
    }
    else
    {
        U16 best_move = 0;
        auto moveset = b.get_legal_moves();
        if (moveset.size() == 0) {
            return make_pair(0, best_move);
        }
        else {

            int max_value = -100000;
            for (auto m : moveset) {
                Board b_copy = *b.copy();
                b_copy.do_move(m);
                auto min_ans = Min_value(b_copy, depth + 1, alpha, beta, e);
                alpha = max(alpha, min_ans.first);
                if (alpha>=beta)
                    return make_pair(alpha, m);

                if (min_ans.first > max_value)
                {
                    max_value = min_ans.first;
                    best_move = m;
                    if (depth == 0)
                        e->best_move = m;
                }
            }

            return make_pair(max_value, best_move);
        }
    }
}

pair<int, U16> Min_value(Board b, int depth, int alpha, int beta, Engine* e)
{
    if (depth > MAX_DEPTH) {
        return make_pair(evaluate_function(b), 0);
    }
    else {
        U16 best_move = 0;
        auto moveset = b.get_legal_moves();
        if (moveset.size() == 0) {
            return make_pair(0, best_move);
        } else {

            int min_value = 100000;
            for (auto m: moveset) {
                Board b_copy = *b.copy();
                b_copy.do_move(m);
                auto max_ans = Max_value(b_copy, depth + 1, alpha, beta, e);
                beta = min(beta, max_ans.first);
                if (alpha>=beta)
                    return make_pair(beta, m);
                if (max_ans.first < min_value) {
                    min_value = max_ans.first;
                    best_move = m;
                    if (depth == 0)
                        e->best_move = m;
                }
            }

            return make_pair(min_value, best_move);
        }
    }
}


U16 MiniMax(Board b, PlayerColor colour, Engine* e)
{
    auto moveset = b.get_legal_moves();
    if (moveset.size() == 0) {
        return 0;
    }
    else {
        auto ans = make_pair(0, 0);
        if (colour == BLACK)
        {
            ans = Min_value(b, 0, -100000, 100000, e);
            return ans.second;
        }
        else
        {
            ans = Max_value(b, 0, -100000, 100000, e);
            return ans.second;
        }
    }
}


void Engine::find_best_move(const Board& b) {

    auto colour = b.data.player_to_play;

    if (this->search) {
        this->best_move = MiniMax(b, colour, this);
        return;
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
}
