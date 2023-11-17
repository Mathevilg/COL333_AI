#include <algorithm>
#include <random>
#include <iostream>
#include <thread>

#include "board.hpp"
#include "engine.hpp"
#include "butils.hpp"

using namespace std;

class engine_b1
{
public:
    int MAX_DEPTH = 4;
    int time_left_to_match;
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

    pair<int, U16> MiniMax(const Board& b, PlayerColor colour, Engine* e);

    pair<int, U16> Min_value(const Board& b, int depth, int alpha, int beta, Engine* e);

    pair<int, U16> Max_value(const Board& b, int depth, int alpha, int beta, Engine* e);

    int evaluate_function(const Board& b);
    int eval2(const Board& b, U16 move);

};
