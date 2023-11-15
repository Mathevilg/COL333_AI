#include <algorithm>
#include <random>
#include <iostream>
#include <thread>

#include "board.hpp"
#include "engine.hpp"
#include "butils.hpp"


using namespace std;

class engine_b3
{
public:

    int MAX_DEPTH = 3;
    int time_left_to_match;
    chrono::high_resolution_clock::time_point start_time;
    
    map<U8, int> pawn_scores = {
       {pos(5, 0), 0}, {pos(5, 1), 0}, {pos(5, 2), 0},
       {pos(4, 0), 1}, {pos(4, 1), 1}, {pos(4, 2), 1},/*                             */{pos(4, 5), 11}, {pos(4, 6), 11}, {pos(4, 7), 11},
       {pos(3, 0), 1}, {pos(3, 1), 2}, {pos(3, 2), 2},/*                             */{pos(3, 5), 10}, {pos(3, 6), 10}, {pos(3, 7), 10},
       {pos(2, 0), 1}, {pos(2, 1), 2}, {pos(2, 2), 3}, {pos(2, 3), 4}, {pos(2, 4), 5}, {pos(2, 5), 6}, {pos(2, 6), 8}, {pos(2, 7), 9},
       {pos(1, 0), 1}, {pos(1, 1), 2}, {pos(1, 1), 3}, {pos(1, 3), 4}, {pos(1, 4), 5}, {pos(1, 5), 6}, {pos(1, 6), 8}, {pos(1, 7), 8},
       {pos(0, 0), 1}, {pos(0, 1), 2}, {pos(0, 0), 3}, {pos(0, 3), 4}, {pos(0, 4), 5}, {pos(0, 5), 6}, {pos(0, 6), 8}, {pos(0, 7), 8}, 
    };

    map<U8, int> bishop_scores = {


                                        {pos(3, 2), 9}, {pos(4, 2), 6}, {pos(5, 2), 7},
                        {pos(2, 1), 10}, {pos(3, 1), 11}, {pos(4, 1), 8}, {pos(5, 1), 5}, {pos(6, 1), 7},
        {pos(1, 0), 4}, {pos(2, 0), 8}, {pos(3, 0), 8}, {pos(4, 0), 8}, {pos(5, 0), 5}, {pos(6, 0), 5}, {pos(7, 0), 4}
    };

    map<U8, int> rook_scores = {
                                        {pos(3, 2), 5}, {pos(4, 2), 6}, {pos(5, 2), 7},
                        {pos(2, 1), 5}, {pos(3, 1), 6}, {pos(4, 1), 7}, {pos(5, 1), 8}, {pos(6, 1), 9},
        {pos(1, 0), 10}, {pos(2, 0), 11}, {pos(3, 0), 12}, {pos(4, 0), 13}, {pos(5, 0), 14}, {pos(6, 0), 15}, {pos(7, 0), 15}
    };

    map<U8, int> knight_scores = {
                                        {pos(3, 2), 7}, {pos(4, 2), 7}, {pos(5, 2), 6},
                        {pos(2, 1), 5}, {pos(3, 1), 5}, {pos(4, 1), 5}, {pos(5, 1), 5}, {pos(6, 1), 4},
        {pos(1, 0), 3}, {pos(2, 0), 4}, {pos(3, 0), 4}, {pos(4, 0), 4}, {pos(5, 0), 4}, {pos(6, 0), 3}, {pos(7, 0), 2}
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


    U16 return_best_move(const Board& b, Engine *e);


    int calc_check_score(Board b);
    int calc_positional_score(const Board& b);
    int calculate_material(const Board& b);

    bool isTimeValid();

    pair<int, U16> MiniMax(Board b, PlayerColor colour, Engine* e);

    pair<int, U16> Min_value(Board b, int depth, int alpha, int beta, Engine* e);

    pair<int, U16> Max_value(Board b, int depth, int alpha, int beta, Engine* e);

    int evaluate_function(const Board& b);

};
