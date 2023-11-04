#include <algorithm>
#include <random>
#include <iostream>
#include <thread>

#include "board.hpp"
#include "engine.hpp"
#include "butils.hpp"
#include "engine_b1.hpp"
#include "engine_b2.hpp"
#include "engine_b3.hpp"

BoardType board_type;

Board1 board1;
Board2 board2;
Board3 board3;

void Engine::find_best_move(const Board& b) {


    auto moveset = b.get_legal_moves();
    if (moveset.size() == 0) {
        std::cout << "Could not get any moves from board!\n";
        std::cout << board_to_str(&b.data);
        this->best_move = 0;
    }

    else {

        if (b.data.board_type == 1){
            this->best_move = board1.return_best_move(b);
        }
        else if (b.data.board_type == 2){
            this->best_move = board2.return_best_move(b);
        }
        else if (b.data.board_type == 3){
            this->best_move = board3.return_best_move(b);
        }


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
    }

    // just for debugging, to slow down the moves
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}
