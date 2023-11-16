#include "engine_b3.hpp"

using namespace std;


#define cw_180_pos_3(p) cw_180_8x8[((int)p)%64]
#define acw_90_pos_3(p) acw_90_8x8[((int)p)%64]
#define cw_90_pos_3(p) cw_90_8x8[((int)p)%64]


// isme reflection ki bt ho sakti hai !! @ShlokDotPande
int engine_b3::get_pawn_score_white(U8 P)
{
    return pawn_scores[P];
}

int engine_b3::get_pawn_score_black(U8 P1)
{
    // int idx = (int) P1;
    U8 P = cw_180_pos_3(P1);
    return get_pawn_score_white(P);
}

int engine_b3::get_bishop_score(U8 P)
{

    U8 P1 = cw_90_pos_3(P);
    U8 P2 = cw_180_pos_3(P);
    U8 P3 = acw_90_pos_3(P);
    if (bishop_scores.find(P) != bishop_scores.end())
    {
        return bishop_scores[P];
    }
    if (bishop_scores.find(P1) != bishop_scores.end())
    {
        return bishop_scores[P1];
    }
    if (bishop_scores.find(P2) != bishop_scores.end())
    {
        return bishop_scores[P2];
    }
    if (bishop_scores.find(P3) != bishop_scores.end())
    {
        return bishop_scores[P3];
    }
    cout << getx(P) << " " << gety(P) ;
    cout<<" no matching position for bishop" << endl;
    return -1;
}

int engine_b3::get_rook_score_white(U8 P)
{

    U8 P1 = cw_90_pos_3(P);
    U8 P2 = cw_180_pos_3(P);
    U8 P3 = acw_90_pos_3(P);
    if (rook_scores.find(P) != rook_scores.end())
    {
        return rook_scores[P];
    }

    else if (rook_scores.find(P1) != rook_scores.end())
    {
        return rook_scores[P1];
    }
    else if (rook_scores.find(P2) != rook_scores.end())
    {
        return rook_scores[P2];
    }
    else if (rook_scores.find(P3) != rook_scores.end())
    {
        return rook_scores[P3];
    }

    cout<<"no matching position for rook";
    return -1;
}

int engine_b3::get_rook_score_black(U8 P)
{
    U8 P1 = cw_180_pos_3(P);
    return get_rook_score_white(P1);
}

int engine_b3::get_king_score_white(U8 P)
{
    return 10;
}

int engine_b3::get_king_score_black(U8 P)
{
    U8 P1 = cw_180_pos_3(P);
    return get_king_score_white(P1);
}

// @ShlokDotPande knight scores is how many it can attack !!
int engine_b3::get_knight_score_white(U8 P)
{
    U8 P1 = cw_90_pos_3(P);
    U8 P2 = cw_180_pos_3(P);
    U8 P3 = acw_90_pos_3(P);
    if (knight_scores.find(P) != knight_scores.end())
    {
        return knight_scores[P];
    }
    else if (knight_scores.find(P1) != knight_scores.end())
    {
        return knight_scores[P1];
    }
    else if (knight_scores.find(P2) != knight_scores.end())
    {
        return knight_scores[P2];
    }
    else if (knight_scores.find(P3) != knight_scores.end())
    {
        return knight_scores[P3];
    }

    cout<<"no matching position for knight";
    return -1;
}

int engine_b3::get_knight_score_black(U8 P)
{
    U8 P1 = cw_180_pos_3(P);
    return get_knight_score_white(P1);
}

// chrono::high_resolution_clock::time_point start_time;
// int time_left_to_match;
// this is the main function !
bool engine_b3::isTimeValid() {
    this_thread::sleep_for(std::chrono::milliseconds(1950));
    return chrono::high_resolution_clock::now() - start_time < chrono::milliseconds(2000);
}

int engine_b3::calculate_material(const Board& b) {
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
            default:
                break;
        }
    }
    return material;
}


int engine_b3::calc_check_score(Board b)
{
    if (b.in_check())
    {
        if (b.data.player_to_play == WHITE)
            return -10;
        else
            return 10;
    }
    else
        return 0;
}


int engine_b3::calc_positional_score(const Board &b){
    int positional_score = 0;
    for (int i=0; i<64; i++){
        if (b.data.board_0[i] == EMPTY)
            continue;
        else{
            switch (b.data.board_0[i])
            {
            case WHITE | PAWN:
                positional_score += (get_pawn_score_white(i)+3)*2;
                break;
            case BLACK | PAWN:
                positional_score -= (get_pawn_score_black(i)+3)*2;
                break;
            case WHITE | BISHOP:
                positional_score += get_bishop_score(i);
                break;
            case BLACK | BISHOP:
                positional_score -= get_bishop_score(i);
                break;
            case WHITE | ROOK:
                positional_score += get_rook_score_white(i);
                break;
            case BLACK | ROOK:
                positional_score -= get_rook_score_black(i);
                break;
            case WHITE | KING:
                positional_score += get_king_score_white(i);
                break;
            case BLACK | KING:
                positional_score -= get_king_score_black(i);
                break;
            case WHITE | KNIGHT:
                positional_score += get_knight_score_white(i);
                break;
            case BLACK | KNIGHT:
                positional_score -= get_knight_score_black(i);
                break;
            default:
                break;
            }
        }
    }
    return positional_score;
}


int engine_b3::evaluate_function(const Board& b)
{

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

    int material = calculate_material(b);  // range -5 to +5
    int w1 = 110;

    int pawn_score = 0; // count_pawn_score(b); // range -20 to +20
    int w2 = 6;
    
    
    int check_score = calc_check_score(b); // -10 or 10
    int w3 = 6;


    // wrong code for count_pawn_score, uses b.data.w_pawn_ws and b.data.w_pawn_bs
    int protected_score = 0;  // how_many_protected_score(b);  // -1 to 1
    int w4 = 8;


    int positional_score = calc_positional_score(b); // range -20/-13 to +20/+13 (for rook)
    // -9 to 9 for pawn, -8 to 8 for bishop
    int w5 = 3;

    int final_score = (w1*material) + (w2*pawn_score) + (w3*check_score) + (w4*protected_score) + (w5*positional_score);
    return final_score;
}

//pair<int, U16> engine_b3::Min_value(Board b, int depth, int alpha, int beta, Engine* e);

pair<int, U16> engine_b3::Max_value(Board b, int depth, int alpha, int beta, Engine* e)
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

pair<int, U16> engine_b3::Min_value(Board b, int depth, int alpha, int beta, Engine* e)
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
            U16 white_best_move = 0;
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
                    white_best_move = max_ans.second;
                    best_move = m;
//                    if (depth == 0)
//                        e->best_move = m;
                }
            }

            cout<<"\n";
            cout<<"Black final eval:"<<min_value<<move_to_str(best_move);
            cout<<"corresponding white best move"<<move_to_str(white_best_move);
            cout<<"\n";
            return make_pair(min_value, best_move);
        }
    }
}


pair<int, U16> engine_b3::MiniMax(Board b, PlayerColor colour, Engine* e)
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
            cout<<"\n\n";
            cout<< "final evaluation is, "<<ans.first<< " at MAX_DEPTH: "<<MAX_DEPTH<<'\n';
            cout<<"best move at this depth is: "<<move_to_str(ans.second)<<"\n";
            cout<<"\n\n";
            return ans;
        }
        else
        {
            ans = engine_b3::Max_value(b, 0, -100000, 100000, e);
            cout<<"\n\n";
            cout<< "final evaluation is, "<<ans.first<< " at MAX_DEPTH: "<<MAX_DEPTH<<'\n';
            cout<<"best move at this depth is: "<<move_to_str(ans.second)<<"\n";
            cout<<"\n\n";
            return ans;
        }
    }
}


U16 engine_b3::return_best_move(const Board &b, Engine *e) {
    start_time = chrono::high_resolution_clock::now();
    time_left_to_match = (int) e->time_left.count();
    cout << "time left to match: " << time_left_to_match << endl;
    auto moveset = b.get_legal_moves();
    auto colour = b.data.player_to_play;
    auto curr_time = chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(curr_time - start_time);

    // Convert the duration to seconds (as a floating-point number)
    double elapsed_time = duration.count() / 1e6;

    vector<pair<int, pair<U16, Board>>> moveset_boards;

    MAX_DEPTH = 1;
    while (MAX_DEPTH <= 4)
    {

        auto p = MiniMax(b, colour, e);
        e->best_move = p.second;
        if ((p.first == 100000) && (b.data.player_to_play == WHITE))
            break;

        if ((p.first == -100000) && (b.data.player_to_play == BLACK))
            break;

        if (MAX_DEPTH == 1)
            MAX_DEPTH++;
        else
            MAX_DEPTH += 2;
        //    MAX_DEPTH++;
        curr_time = chrono::high_resolution_clock::now();

        duration = std::chrono::duration_cast<std::chrono::microseconds>(curr_time - start_time);

        // Convert the duration to seconds (as a floating-point number)
        elapsed_time = duration.count() / 1e6;
    }

    return e->best_move;
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



