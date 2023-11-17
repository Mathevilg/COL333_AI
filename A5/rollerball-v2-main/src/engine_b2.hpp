#include <algorithm>
#include <random>
#include <iostream>
#include <thread>

#include "board.hpp"
#include "engine.hpp"
#include "butils.hpp"

using namespace std;


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
