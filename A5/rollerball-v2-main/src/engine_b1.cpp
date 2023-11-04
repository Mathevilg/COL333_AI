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
            case ROOK | BLACK:
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


// #define cw_90_pos(p) (cw_90_7x7[p])
#define cw_180_pos(p) cw_180_7x7[((int)p)%64]
#define acw_90_pos(p) acw_90_7x7[((int)p)%64]
#define cw_90_pos(p) cw_90_7x7[((int)p)%64]

int get_pawn_score_white(U8 P)
{
    map<U8, int> pawn_scores;
    pawn_scores[pos(2, 0)] = 0;
    pawn_scores[pos(2, 1)] = 0;

    pawn_scores[pos(0, 0)] = 1;
    pawn_scores[pos(1, 0)] = 1;

    pawn_scores[pos(0, 1)] = 2;
    pawn_scores[pos(1, 1)] = 2;


    for (int i = 2; i < 6; i++)
    {
        pawn_scores[pos(0, i)] = i+3;
        pawn_scores[pos(1, i)] = i+3;
    }
    pawn_scores[pos(0, 6)] = 8;
    pawn_scores[pos(1, 6)] = 9;

    for (int i = 2; i <= 4; i++)
    {
        pawn_scores[pos(i, 5)] = 8+i;
        pawn_scores[pos(i, 6)] = 8+i;
    }

    pawn_scores[pos(3, 6)] = 14;
    pawn_scores[pos(3, 5)] = 14;
    return pawn_scores[P];
}

int get_pawn_score_black(U8 P1)
{
    // int idx = (int) P1;
    U8 P = cw_180_pos(P1);

    return get_pawn_score_white(P);
}


chrono::high_resolution_clock::time_point start_time;
int time_left_to_match;
// this is the main function !
bool engine_b1::isTimeValid() {
    this_thread::sleep_for(std::chrono::milliseconds(1950));
    return chrono::high_resolution_clock::now() - start_time < chrono::milliseconds(2000);
}

U16 engine_b1::return_best_move(const Board &b, Engine *e) {
    start_time = chrono::high_resolution_clock::now();
    time_left_to_match = e->time_left.count();
    cout << "time left to match: " << time_left_to_match << endl;
    auto moveset = b.get_legal_moves();
    if (moveset.size() == 0) {
        std::cout << "Could not get any moves from board!\n";
        std::cout << board_to_str(&b.data);
        return 0;
    }
    else{
        std::vector<U16> moves;
        while (isTimeValid()){
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
        }
        return moves[0];
    }
}



