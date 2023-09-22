#include <algorithm>
#include <random>
#include <iostream>

#include "board.hpp"
#include "engine.hpp"
// move = 0 means defeated !

// maximise for me minimise for other


#define maxDepth = 2;



// should return a positive value for the board configuration !
long long eval(const Board &b){
    return b.data.b_bishop + b.data.b_king + b.data.b_pawn_bs + b.data.b_pawn_ws + b.data.b_rook_bs + b.data.b_rook_ws
    - (b.data.w_bishop + b.data.w_king + b.data.w_pawn_bs + b.data.w_pawn_ws + b.data.w_rook_bs + b.data.w_rook_ws);
}


long long minValue(const Board &b, int depth);

long long maxValue(Board& b, int depth){
    auto moves = b.get_legal_moves();
    if (! moves.size() ) return -1e11; 
    else if (!depth) return eval(b);
    else {
        long long temp = LLONG_MIN;
        // auto ans = 0;
        for (auto move : moves){
            Board* temp_b = b.copy();
            temp_b->do_move(move);
            long long temp_value = minValue(*temp_b, depth - 1);
            if ( temp_value >= temp) {
                // ans = move;
                temp = temp_value;
            }
        }
        return temp;
    }
}

long long minValue(const Board &b, int depth){
    auto moves = b.get_legal_moves();
    if (!moves.size()) return 1e11;
    else if (!depth) return -eval(b);
    else {
        long long temp = LLONG_MAX;
        for (auto move : moves){
            Board* temp_b = b.copy();
            temp_b->do_move(move);
            long long temp_value = maxValue(*temp_b, depth-1);
            if (temp_value <= temp){
                temp = temp_value;
            }
        }
        return temp;
    }
}





void Engine::find_best_move(const Board& b) {

    // pick a random move
    
    auto moveset = b.get_legal_moves();
    if (moveset.size() == 0) {
        this->best_move = 0;
    }
    else {
        std::vector<U16> moves;
        std::cout << all_boards_to_str(b) << std::endl;
        for (auto m : moveset) {
            std::cout << move_to_str(m) << " ";
        }
        std::cout << std::endl;
        // std::sample(
        //     moveset.begin(),
        //     moveset.end(),
        //     std::back_inserter(moves),
        //     1,
        //     std::mt19937{std::random_device{}()}
        // );
        long long ans = LLONG_MIN;
        auto move = 0;
        for (auto it: moveset){
            Board* tmp = b.copy();
            tmp->do_move(it);
            long long tmp_val = maxValue(*tmp, 2);
            if (tmp_val >= ans){
                ans = tmp_val;
                move = it;
            }

        }
        this->best_move = move;
    }
}
