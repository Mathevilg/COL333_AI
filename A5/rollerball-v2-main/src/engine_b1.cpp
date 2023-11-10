#include "engine_b1.hpp"

using namespace std;

// editing trial

// int MAX_DEPTH = 3;


#define cw_180_pos(p) cw_180_7x7[((int)p)%64]
#define acw_90_pos(p) acw_90_7x7[((int)p)%64]
#define cw_90_pos(p) cw_90_7x7[((int)p)%64]

int engine_b1::get_pawn_score_white(U8 P)
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

int engine_b1::get_pawn_score_black(U8 P1)
{
    // int idx = (int) P1;
    U8 P = cw_180_pos(P1);

    return get_pawn_score_white(P);
}

int engine_b1::get_bishop_score(U8 P)
{
    map<U8, int> bishop_scores;
    bishop_scores[pos(1, 0)] = 3;
    bishop_scores[pos(2, 1)] = 8;
    bishop_scores[pos(3, 0)] = 2;
    bishop_scores[pos(4, 1)] = 6;
    bishop_scores[pos(5, 0)] = 3;
//    bishop_scores[pos(6, 1)] = 3;

    if (gety(P) <= 1 && getx(P) >= 1 && getx(P) <= 5)
    {
        return bishop_scores[P];
    }

    U8 P1 = cw_90_pos(P);
    U8 P2 = cw_180_pos(P);
    U8 P3 = acw_90_pos(P);
    if (gety(P1) <= 1 && getx(P1) >= 1 && getx(P1) <= 5)
    {
        return bishop_scores[P1];
    }
    if (gety(P2) <= 1 && getx(P2) >= 1 && getx(P2) <= 5)
    {
        return bishop_scores[P2];
    }
    if (gety(P3) <= 1 && getx(P3) >= 1 && getx(P3) <= 5)
    {
        return bishop_scores[P3];
    }

    cout<<"no matching position for bishop";
    return -1;
}

int engine_b1::get_rook_score_white(U8 P)
{
    map<U8, int> rook_scores;
    rook_scores[pos(1, 0)] = 17;
    for (int i = 2; i <= 5; i++)
    {
        rook_scores[pos(i, 0)] = 10+ i;
    }
    rook_scores[pos(6, 0)] = 16;

    for (int i = 2; i <= 4; i++)
    {
        rook_scores[pos(i, 1)] = i+ 2;
    }
    rook_scores[pos(5, 1)] = 9;

    if ((gety(P) == 0 && getx(P) >= 0) || (gety(P) == 1 && getx(P) <= 6 && getx(P) >= 0))
    {
        rook_scores[pos(6, 0)] = 5;
        rook_scores[pos(5, 0)] = 7;
        rook_scores[pos(4, 0)] = 7;
        rook_scores[pos(3, 0)] = 10;
        rook_scores[pos(2, 0)] = 11;
        rook_scores[pos(1, 0)] = 16;
        rook_scores[pos(0, 0)] = 16;

        rook_scores[pos(6, 1)] = 2;
        rook_scores[pos(5, 1)] = 2;
        rook_scores[pos(4, 1)] = 4;
        rook_scores[pos(3, 1)] = 6;
        rook_scores[pos(2, 1)] = 7;
        rook_scores[pos(1, 1)] = 15;
        rook_scores[pos(0, 1)] = 15;

        return rook_scores[P];
    }

    U8 P1 = cw_90_pos(P);
    U8 P2 = cw_180_pos(P);
    U8 P3 = acw_90_pos(P);
    if ((gety(P1) == 0 && getx(P1) >= 1) || (gety(P1) == 1 && getx(P1) <= 5 && getx(P1) >= 2))
    {
        return rook_scores[P1];
    }
    if ((gety(P2) == 0 && getx(P2) >= 1) || (gety(P2) == 1 && getx(P2) <= 5 && getx(P2) >= 2))
    {
        return rook_scores[P2]+4;
    }
    if ((gety(P3) == 0 && getx(P3) >= 1) || (gety(P3) == 1 && getx(P3) <= 5 && getx(P3) >= 2))
    {
        return rook_scores[P3]+4;
    }

    cout<<"no matching position for bishop";
    return -1;
}

int engine_b1::get_rook_score_black(U8 P)
{
    U8 P1 = cw_180_pos(P);
    return get_rook_score_white(P1);
}

int engine_b1::get_king_score_white(U8 P)
{
//    map<U8, int> king_scores;
//
//    king_scores[pos(3, 1)] = 0;
//    king_scores[pos(3, 0)] = 0;
//
//    if ((getx(P) == 3) || getx)
//    {
//        return king_scores[P] = 0;
//    }

    return 10;
}

int engine_b1::get_king_score_black(U8 P)
{
    U8 P1 = cw_180_pos(P);
    return get_king_score_white(P1);
}


// chrono::high_resolution_clock::time_point start_time;
// int time_left_to_match;
// this is the main function !
bool engine_b1::isTimeValid() {
    this_thread::sleep_for(std::chrono::milliseconds(1950));
    return chrono::high_resolution_clock::now() - start_time < chrono::milliseconds(2000);
}

int engine_b1::calculate_material(const Board& b)
{
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

int engine_b1::evaluate_function(const Board& b)
{
    int final_score = calculate_material(b);

    if (b.get_legal_moves().empty())
    {
        if (b.in_check()) {
            if (b.data.player_to_play == WHITE)
                return -100000;
            else
                return 100000;
        }
        else
            return 0;
    }

    return final_score;
}

//pair<int, U16> engine_b1::Min_value(Board b, int depth, int alpha, int beta, Engine* e);

pair<int, U16> engine_b1::Max_value(Board b, int depth, int alpha, int beta, Engine* e)
{

    if (depth > MAX_DEPTH)
    {
        return make_pair(evaluate_function(b), 0);
    }
    else
    {
        U16 best_move = 0;
        auto moveset = b.get_legal_moves();
        if (moveset.empty()) {
            if (b.in_check()) {
                if (b.data.player_to_play == WHITE)
                    return make_pair(-100000, U16(e->best_move));
                else
                    return make_pair(100000, U16(e->best_move));
            }
            else
                return make_pair(0, best_move);
        }
        else {

            int max_value = -100050;
            for (auto m : moveset) {

                if (m & (1 << 6))
                {
                    continue;
                }

                Board b_copy = Board(b); // create a copy constructor??
                b_copy.do_move_(m);
                auto min_ans = Min_value(b_copy, depth + 1, alpha, beta, e);
                alpha = max(alpha, min_ans.first);
                if (alpha>=beta)
                    return make_pair(min_ans.first, m);


                if (min_ans.first > max_value)
                {
                    max_value = min_ans.first;
                    best_move = m;
//                    if (depth == 0)
//                        e->best_move = m;
                }
            }

            return make_pair(max_value, best_move);
        }
    }
}

pair<int, U16> engine_b1::Min_value(Board b, int depth, int alpha, int beta, Engine* e)
{

    if (depth > MAX_DEPTH) {
        return make_pair(evaluate_function(b), U16(e->best_move));
    }
    else {
        U16 best_move = 0;
        auto moveset = b.get_legal_moves();
        if (moveset.empty()) {
            if (b.in_check()) {
                if (b.data.player_to_play == WHITE)
                    return make_pair(-100000, U16(e->best_move));
                else
                    return make_pair(100000, U16(e->best_move));
            }
            else
                return make_pair(0, U16(e->best_move));
        }
        else {
            int min_value = 100050;
            for (auto m: moveset) {

                if (m & (1 << 6))
                {
                    continue;
                }

                Board b_copy = Board(b);
                b_copy.do_move_(m);
                auto max_ans = Max_value(b_copy, depth + 1, alpha, beta, e);
                beta = min(beta, max_ans.first);
                if (alpha>=beta)
                    return make_pair(max_ans.first, m);
                if (max_ans.first < min_value) {
                    min_value = max_ans.first;
                    best_move = m;
//                    if (depth == 0)
//                        e->best_move = m;
                }
            }

            return make_pair(min_value, best_move);
        }
    }
}


pair<int, U16> engine_b1::MiniMax(Board b, PlayerColor colour, Engine* e)
{
    auto moveset = b.get_legal_moves();

    if (moveset.empty()) {
        return make_pair(0, 0);
    }
    else {
        auto ans = make_pair(0, 0);
        if (colour == BLACK)
        {
            ans = Min_value(b, 0, -100000, 100000, e);
            cout<<"\n\n\n";
            cout<< "final evaluation is, "<<ans.first<< " at MAX_DEPTH: "<<MAX_DEPTH<<'\n';
            cout<<"best move at this depth is: "<<move_to_str(ans.second)<<"\n";
            cout<<"\n\n\n";
            return ans;
        }
        else
        {
            ans = engine_b1::Max_value(b, 0, -100000, 100000, e);
            cout<<"\n\n\n";
            cout<< "final evaluation is, "<<ans.first<< " at MAX_DEPTH: "<<MAX_DEPTH<<'\n';
            cout<<"best move at this depth is: "<<move_to_str(ans.second)<<"\n";
            cout<<"\n\n\n";
            return ans;
        }
    }
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

        auto ans = MiniMax(b, b.data.player_to_play, e);

        return ans.second;
//        while (isTimeValid()){
//            std::cout << show_moves(&b.data, moveset) << std::endl;
//            for (auto m : moveset) {
//                std::cout << move_to_str(m) << " ";
//            }
//            std::cout << std::endl;
//            std::sample(
//                moveset.begin(),
//                moveset.end(),
//                std::back_inserter(moves),
//                1,
//                std::mt19937{std::random_device{}()}
//            );
//        }
//        return moves[0];
    }
}



