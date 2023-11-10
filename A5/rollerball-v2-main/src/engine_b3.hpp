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

    int MAX_DEPTH = 4;
    int time_left_to_match;
    chrono::high_resolution_clock::time_point start_time;
    int get_pawn_score_white(U8 P);
    int get_pawn_score_black(U8 P);
    int get_bishop_score(U8 P);
    int get_knight_score(U8 P);
    int get_rook_score_white(U8 P);
    int get_rook_score_black(U8 P);
    int get_king_score_white(U8 P);
    int get_king_score_black(U8 P);


    U16 return_best_move(const Board& b, Engine *e);

    int calculate_material(const Board& b);

    bool isTimeValid();

    pair<int, U16> MiniMax(Board b, PlayerColor colour, Engine* e);

    pair<int, U16> Min_value(Board b, int depth, int alpha, int beta, Engine* e);

    pair<int, U16> Max_value(Board b, int depth, int alpha, int beta, Engine* e);

    int evaluate_function(const Board& b);

};
