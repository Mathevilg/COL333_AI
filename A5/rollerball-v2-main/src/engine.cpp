#include <algorithm>
#include <random>
#include <iostream>
#include <thread>

#include "board.hpp"
#include "engine.hpp"
#include "butils.hpp"
#include "engine_b1.cpp"
#include "engine_b2.cpp"
#include "engine_b3.cpp"

using namespace std;

BoardType board_type;


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
