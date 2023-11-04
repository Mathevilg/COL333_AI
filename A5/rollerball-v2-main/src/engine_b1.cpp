#include "engine_b1.hpp"

using namespace std;


int engine_b1::calculate_material(const Board& b) {
    int material = 0;
    for (int i = 0; i < 64; i++) {
        switch (b.data.board_0[i]) {
            case PAWN | WHITE:
                material += 1;
                break;
            case KNIGHT | WHITE:
                material += 3;
                break;
            case BISHOP | WHITE:
                material += 3;
                break;
            case ROOK | WHITE:
                material += 5;
                break;
            case PAWN | BLACK:
                material -= 1;
                break;
            case KNIGHT | BLACK:
                material -= 3;
                break;
            case BISHOP | BLACK:
                material -= 3;
                break;
            case BISHOP | ROOK:
                material -= 5;
                break;
            // case B_QUEEN:
            //     material -= 9;
            //     break;
            default:
                break;
        }
    }
    return material;
}


U16 engine_b1::return_best_move(const Board &b, Engine *e) {
    auto moveset = b.get_legal_moves();
    if (moveset.size() == 0) {
        std::cout << "Could not get any moves from board!\n";
        std::cout << board_to_str(&b.data);
        return 0;
    }
    else{
        std::vector<U16> moves;
        std::cout << show_moves(&b.data, moveset) << std::endl;
        for (auto m : moveset) {
            std::cout << move_to_str(m) << " ";
        }
        std::cout << std::endl;
        std::sample(
            moveset.begin(),
            moveset.end(),
            std::back_inserter(moves),
            1,
            std::mt19937{std::random_device{}()}
        );
        return moves[0];
    }
}



