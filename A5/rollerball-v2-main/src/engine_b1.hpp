#include <algorithm>
#include <random>
#include <iostream>
#include <thread>

#include "board.hpp"
#include "engine.hpp"
#include "butils.hpp"

using namespace std;


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
