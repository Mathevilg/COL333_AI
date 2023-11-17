#include "engine_b2.hpp"

using namespace std;

// editing trial

// int MAX_DEPTH = 3;

#define cw_180_pos_2(p) cw_180_8x8[((int)p)%64]
#define acw_90_pos_2(p) acw_90_8x8[((int)p)%64]
#define cw_90_pos_2(p) cw_90_8x8[((int)p)%64]


//@ShlokDotPande this is the function you need to edit
int engine_b2::get_pawn_score_white(U8 P)
{
    return pawn_scores[P];
}

int engine_b2::get_pawn_score_black(U8 P1)
{
    // int idx = (int) P1;
    U8 P = cw_180_pos_2(P1);

    return get_pawn_score_white(P);
}

int engine_b2::get_bishop_score(U8 P)
{
//    bishop_scores[pos(6, 1)] = 3;

    if (bishop_scores.find(P) != bishop_scores.end())
    {
        return bishop_scores[P];
    }

    U8 P1 = cw_90_pos_2(P);
    U8 P2 = cw_180_pos_2(P);
    U8 P3 = acw_90_pos_2(P);
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

    cout<<"no matching position for bishop\n";
    cout<<getx(P)<<" "<<gety(P)<<endl;
    cout<<'\n';
    return -1;
}

int engine_b2::get_rook_score_white(U8 P)
{

    if (rook_scores.find(P) != rook_scores.end())
    {
        return rook_scores[P];
    }

    U8 P1 = cw_90_pos_2(P);
    U8 P2 = cw_180_pos_2(P);
    U8 P3 = acw_90_pos_2(P);
    if (rook_scores.find(P1) != rook_scores.end())
    {
        return rook_scores[P1];
    }
    if (rook_scores.find(P2) != rook_scores.end())
    {
        return rook_scores[P2]+4;
    }
    if (rook_scores.find(P3) != rook_scores.end())
    {
        return rook_scores[P3]+4;
    }

    cout<<"no matching position for rook";
    return -1;
}

int engine_b2::get_rook_score_black(U8 P)
{
    U8 P1 = cw_180_pos_2(P);
    return get_rook_score_white(P1);
}

int engine_b2::get_king_score_white(U8 P)
{
    if ((gety(P) == 1 && getx(P) >= 2 && getx(P) <= 6) || (gety(P) == 0 && getx(P) >= 1 && getx(P) <= 7))
    {
        return king_scores[P];
    }

    U8 P1 = cw_90_pos_2(P);
    U8 P2 = cw_180_pos_2(P);
    U8 P3 = acw_90_pos_2(P);
    if ((gety(P1) == 1 && getx(P1) >= 2 && getx(P1) <= 6) || (gety(P1) == 0 && getx(P1) >= 1 && getx(P1) <= 7))
    {
        return king_scores[P1]-1;
    }
    if ((gety(P2) == 1 && getx(P2) >= 2 && getx(P2) <= 6) || (gety(P2) == 0 && getx(P2) >= 1 && getx(P2) <= 7))
    {
        return king_scores[P2]-1;
    }
    if ((gety(P3) == 1 && getx(P3) >= 2 && getx(P3) <= 6) || (gety(P3) == 0 && getx(P3) >= 1 && getx(P3) <= 7))
    {
        return king_scores[P3]+1;
    }

    cout<<"no matching position for king x, y:- " << getx(P) << " " << gety(P) << endl;
    return -1;
}


int engine_b2::get_king_score_black(U8 P)
{
    U8 P1 = cw_180_pos_2(P);
    return get_king_score_white(P1);
}


// chrono::high_resolution_clock::time_point start_time;
// int time_left_to_match;
// this is the main function !
bool engine_b2::isTimeValid() {
    this_thread::sleep_for(std::chrono::milliseconds(1950));
    return chrono::high_resolution_clock::now() - start_time < chrono::milliseconds(2000);
}


int engine_b2::calculate_material(const Board& b) {
    int material = 0;

    if (b.data.w_pawn_1 != DEAD)
    {
        if (b.data.board_0[b.data.w_pawn_1] == (PAWN | WHITE))
            material += 1;
        else
        {
            if (b.data.board_0[b.data.w_pawn_1] == (ROOK | WHITE))
                material += 5;
            else if (b.data.board_0[b.data.w_pawn_1] == (BISHOP | WHITE))
                material += 3;
            else if (b.data.board_0[b.data.w_pawn_1] == (KNIGHT | WHITE))
                material += 4;
        }
    }
    if (b.data.w_pawn_2 != DEAD)
    {
        if (b.data.board_0[b.data.w_pawn_2] == (PAWN | WHITE))
            material += 1;
        else
        {
            if (b.data.board_0[b.data.w_pawn_2] == (ROOK | WHITE))
                material += 5;
            else if (b.data.board_0[b.data.w_pawn_2] == (BISHOP | WHITE))
                material += 3;
            else if (b.data.board_0[b.data.w_pawn_2] == (KNIGHT | WHITE))
                material += 4;
        }
    }
    if (b.data.w_pawn_3 != DEAD)
    {
        if (b.data.board_0[b.data.w_pawn_3] == (PAWN | WHITE))
            material += 1;
        else
        {
            if (b.data.board_0[b.data.w_pawn_3] == (ROOK | WHITE))
                material += 5;
            else if (b.data.board_0[b.data.w_pawn_3] == (BISHOP | WHITE))
                material += 3;
            else if (b.data.board_0[b.data.w_pawn_3] == (KNIGHT | WHITE))
                material += 4;
        }
    }
    if (b.data.w_pawn_4 != DEAD)
    {
        if (b.data.board_0[b.data.w_pawn_4] == (PAWN | WHITE))
            material += 1;
        else
        {
            if (b.data.board_0[b.data.w_pawn_4] == (ROOK | WHITE))
                material += 5;
            else if (b.data.board_0[b.data.w_pawn_4] == (BISHOP | WHITE))
                material += 3;
            else if (b.data.board_0[b.data.w_pawn_4] == (KNIGHT | WHITE))
                material += 4;
        }
    }
    if (b.data.w_bishop != DEAD)
        material += 3;
//    if (b.data.w_knight_1 != DEAD)
//        material += 4;
//    if (b.data.w_knight_2 != DEAD)
//        material += 4;
    if (b.data.w_rook_1 != DEAD)
        material += 5;
    if (b.data.w_rook_2 != DEAD)
        material += 5;


    if (b.data.b_pawn_1 != DEAD)
    {
        if (b.data.board_0[b.data.b_pawn_1] == (PAWN | BLACK))
            material -= 1;
        else
        {
            if (b.data.board_0[b.data.b_pawn_1] == (ROOK | BLACK))
                material -= 5;
            else if (b.data.board_0[b.data.b_pawn_1] == (BISHOP | BLACK))
                material -= 3;
            else if (b.data.board_0[b.data.b_pawn_1] == (KNIGHT | BLACK))
                material -= 4;
        }
    }
    if (b.data.b_pawn_2 != DEAD)
    {
        if (b.data.board_0[b.data.b_pawn_2] == (PAWN | BLACK))
            material -= 1;
        else
        {
            if (b.data.board_0[b.data.b_pawn_2] == (ROOK | BLACK))
                material -= 5;
            else if (b.data.board_0[b.data.b_pawn_2] == (BISHOP | BLACK))
                material -= 3;
            else if (b.data.board_0[b.data.b_pawn_2] == (KNIGHT | BLACK))
                material -= 4;
        }
    }
    if (b.data.b_pawn_3 != DEAD)
    {
        if (b.data.board_0[b.data.b_pawn_3] == (PAWN | BLACK))
            material -= 1;
        else
        {
            if (b.data.board_0[b.data.b_pawn_3] == (ROOK | BLACK))
                material -= 5;
            else if (b.data.board_0[b.data.b_pawn_3] == (BISHOP | BLACK))
                material -= 3;
            else if (b.data.board_0[b.data.b_pawn_3] == (KNIGHT | BLACK))
                material -= 4;
        }
    }
    if (b.data.b_pawn_4 != DEAD)
    {
        if (b.data.board_0[b.data.b_pawn_4] == (PAWN | BLACK))
            material -= 1;
        else
        {
            if (b.data.board_0[b.data.b_pawn_4] == (ROOK | BLACK))
                material -= 5;
            else if (b.data.board_0[b.data.b_pawn_4] == (BISHOP | BLACK))
                material -= 3;
            else if (b.data.board_0[b.data.b_pawn_4] == (KNIGHT | BLACK))
                material -= 4;
        }
    }

    if (b.data.b_bishop != DEAD)
        material -= 3;

//    if (b.data.b_knight_1 != DEAD)
//        material -= 4;
//    if (b.data.b_knight_2 != DEAD)
//        material -= 4;

    if (b.data.b_rook_1 != DEAD)
        material -= 5;
    if (b.data.b_rook_2 != DEAD)
        material -= 5;

    return material;
}


int engine_b2::calc_check_score(const Board& b)
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


int engine_b2::calc_positional_score(const Board &b){
    int positional_score = 0;

    if (b.data.w_pawn_1 != DEAD)
    {
        if (b.data.board_0[b.data.w_pawn_1] == (PAWN | WHITE))
            positional_score += 5*(get_pawn_score_white(b.data.w_pawn_1)+2);
        else
        {
            if (b.data.board_0[b.data.w_pawn_1] == (ROOK | WHITE))
                positional_score += get_rook_score_white(b.data.w_pawn_1);
            else if (b.data.board_0[b.data.w_pawn_1] == (BISHOP | WHITE))
                positional_score += 0.5*get_bishop_score(b.data.w_pawn_1);
        }
    }
    if (b.data.w_pawn_2 != DEAD)
    {
        if (b.data.board_0[b.data.w_pawn_2] == (PAWN | WHITE))
            positional_score += 5*(get_pawn_score_white(b.data.w_pawn_2)+2);
        else
        {
            if (b.data.board_0[b.data.w_pawn_2] == (ROOK | WHITE))
                positional_score += get_rook_score_white(b.data.w_pawn_2);
            else if (b.data.board_0[b.data.w_pawn_2] == (BISHOP | WHITE))
                positional_score += 0.5*get_bishop_score(b.data.w_pawn_2);
        }
    }
    if (b.data.w_pawn_3 != DEAD)
    {
        if (b.data.board_0[b.data.w_pawn_3] == (PAWN | WHITE))
            positional_score += 5*(get_pawn_score_white(b.data.w_pawn_3)+2);
        else
        {
            if (b.data.board_0[b.data.w_pawn_3] == (ROOK | WHITE))
                positional_score += get_rook_score_white(b.data.w_pawn_3);
            else if (b.data.board_0[b.data.w_pawn_3] == (BISHOP | WHITE))
                positional_score += 0.5*get_bishop_score(b.data.w_pawn_3);
        }
    }
    if (b.data.w_pawn_4 != DEAD)
    {
        if (b.data.board_0[b.data.w_pawn_4] == (PAWN | WHITE))
            positional_score += 5*(get_pawn_score_white(b.data.w_pawn_4)+2);
        else
        {
            if (b.data.board_0[b.data.w_pawn_4] == (ROOK | WHITE))
                positional_score += get_rook_score_white(b.data.w_pawn_4);
            else if (b.data.board_0[b.data.w_pawn_4] == (BISHOP | WHITE))
                positional_score += 0.5*get_bishop_score(b.data.w_pawn_4);
        }
    }

    if (b.data.w_bishop != DEAD)
        positional_score += 0.5*get_bishop_score(b.data.w_bishop);
    if (b.data.w_rook_1 != DEAD)
        positional_score += get_rook_score_white(b.data.w_rook_1);
    if (b.data.w_rook_2 != DEAD)
        positional_score += get_rook_score_white(b.data.w_rook_2);

    if (b.data.b_pawn_1 != DEAD)
    {
        if (b.data.board_0[b.data.b_pawn_1] == (PAWN | BLACK))
            positional_score -= 5*(get_pawn_score_black(b.data.b_pawn_1)+2);
        else
        {
            if (b.data.board_0[b.data.b_pawn_1] == (ROOK | BLACK))
                positional_score -= get_rook_score_black(b.data.b_pawn_1);
            else if (b.data.board_0[b.data.b_pawn_1] == (BISHOP | BLACK))
                positional_score -= 0.5*get_bishop_score(b.data.b_pawn_1);
        }
    }
    if (b.data.b_pawn_2 != DEAD)
    {
        if (b.data.board_0[b.data.b_pawn_2] == (PAWN | BLACK))
            positional_score -= 5*(get_pawn_score_black(b.data.b_pawn_2)+2);
        else
        {
            if (b.data.board_0[b.data.b_pawn_2] == (ROOK | BLACK))
                positional_score -= get_rook_score_black(b.data.b_pawn_2);
            else if (b.data.board_0[b.data.b_pawn_2] == (BISHOP | BLACK))
                positional_score -= 0.5*get_bishop_score(b.data.b_pawn_2);
        }
    }
    if (b.data.b_pawn_3 != DEAD)
    {
        if (b.data.board_0[b.data.b_pawn_3] == (PAWN | BLACK))
            positional_score -= 5*(get_pawn_score_black(b.data.b_pawn_3)+2);
        else
        {
            if (b.data.board_0[b.data.b_pawn_3] == (ROOK | BLACK))
                positional_score -= get_rook_score_black(b.data.b_pawn_3);
            else if (b.data.board_0[b.data.b_pawn_3] == (BISHOP | BLACK))
                positional_score -= 0.5*get_bishop_score(b.data.b_pawn_3);
        }
    }
    if (b.data.b_pawn_4 != DEAD)
    {
        if (b.data.board_0[b.data.b_pawn_4] == (PAWN | BLACK))
            positional_score -= 5*(get_pawn_score_black(b.data.b_pawn_4)+2);
        else
        {
            if (b.data.board_0[b.data.b_pawn_4] == (ROOK | BLACK))
                positional_score -= get_rook_score_black(b.data.b_pawn_4);
            else if (b.data.board_0[b.data.b_pawn_4] == (BISHOP | BLACK))
                positional_score -= 0.5*get_bishop_score(b.data.b_pawn_4);
        }
    }

    if (b.data.b_bishop != DEAD)
        positional_score -= 0.5*get_bishop_score(b.data.b_bishop);

    if (b.data.b_rook_1 != DEAD)
        positional_score -= get_rook_score_black(b.data.b_rook_1);
    if (b.data.b_rook_2 != DEAD)
        positional_score -= get_rook_score_black(b.data.b_rook_2);

    positional_score += get_king_score_white(b.data.w_king);
    positional_score -= get_king_score_black(b.data.b_king);

    return positional_score;
}


int engine_b2::pawn_closeness_score_white(const Board& b)
{
    int score1 = 12;

    auto pos1 = b.data.w_pawn_1;

    auto pos2 = b.data.w_pawn_2;

    if (pos1 != DEAD && pos2 != DEAD && b.data.board_0[pos2] == (WHITE|PAWN) && b.data.board_0[pos1] == (WHITE|PAWN))
    {
        score1 -= abs(gety(pos1) - gety(pos2));
        if (gety(pos1) == 6 || gety(pos1) == 7 || gety(pos2) == 6 || gety(pos2) == 7)
            score1 -= abs(getx(pos1) - getx(pos2));

    }
    else
        score1 = 0;

    pos1 = b.data.w_pawn_3;

    pos2 = b.data.w_pawn_4;
    int score2 = 12;
    if (pos1 != DEAD && pos2 != DEAD && b.data.board_0[pos2] == (WHITE|PAWN) && b.data.board_0[pos1] == (WHITE|PAWN))
    {
        score2 -= abs(gety(pos1) - gety(pos2));
        if (gety(pos1) == 6 || gety(pos1) == 7 || gety(pos2) == 6 || gety(pos2) == 7)
            score2 -= abs(getx(pos1) - getx(pos2));
    }
    else
        score2 = 0;

    return score1 + score2;
}


int engine_b2::pawn_closeness_score_black(const Board& b)
{
    int score1 = 12;

    auto pos1 = b.data.b_pawn_1;

    auto pos2 = b.data.b_pawn_2;

    if (pos1 != DEAD && pos2 != DEAD && b.data.board_0[pos2] == (BLACK|PAWN) && b.data.board_0[pos1] == (BLACK|PAWN))
    {
        score1 -= abs(gety(pos1) - gety(pos2));
        if (gety(pos1) == 6 || gety(pos1) == 7 || gety(pos2) == 6 || gety(pos2) == 7)
            score1 -= abs(getx(pos1) - getx(pos2));
    }
    else
        score1 = 0;

    pos1 = b.data.b_pawn_3;

    pos2 = b.data.b_pawn_4;
    int score2 = 12;
    if (pos1 != DEAD && pos2 != DEAD && b.data.board_0[pos2] == (BLACK|PAWN) && b.data.board_0[pos1] == (BLACK|PAWN))
    {
        score2 -= abs(gety(pos1) - gety(pos2));
        if (gety(pos1) == 6 || gety(pos1) == 7 || gety(pos2) == 6 || gety(pos2) == 7)
            score1 -= abs(getx(pos1) - getx(pos2));
    }
    else
        score2 = 0;

    return score1 + score2;
}


int engine_b2::evaluate_function(const Board& b)
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
    int w3 = 3;


    // wrong code for count_pawn_score, uses b.data.w_pawn_ws and b.data.w_pawn_bs
    int protected_score = 0;  // how_many_protected_score(b);  // -1 to 1
    int w4 = 8;


    int positional_score = calc_positional_score(b); // range -20/-13 to +20/+13 (for rook)
    // -9 to 9 for pawn, -8 to 8 for bishop
    int w5 = 3;

    int pawn_closeness_score = pawn_closeness_score_white(b) - pawn_closeness_score_black(b);
    int w6 = 2;

    int final_score = (w1*material) + (w2*pawn_score) + (w3*check_score) + (w4*protected_score) + (w5*positional_score) + (w6*pawn_closeness_score);
    return final_score;
}


int engine_b2::eval2(const Board& b, U16 move){
    Board b_copy = Board(b);
    b_copy.do_move_(move);
    int material = calculate_material(b_copy);  // range -5 to +5
    int w1 = 110;

    int check_score = calc_check_score(b_copy); // -10 or 10
    int w3 = 3;

    int positional_score = calc_positional_score(b_copy); // range -20/-13 to +20/+13 (for rook)
    // -9 to 9 for pawn, -8 to 8 for bishop
    int w5 = 3;

    int pawn_closeness_score = pawn_closeness_score_white(b_copy) - pawn_closeness_score_black(b_copy);
    int w6 = 2;

    int final_score = (w1*material) + (w3*check_score) + (w5*positional_score) + (w6*pawn_closeness_score);
    return final_score;
}
//pair<int, U16> engine_b2::Min_value(Board b, int depth, int alpha, int beta, Engine* e);

pair<int, U16> engine_b2::Max_value(const Board& b, int depth, int alpha, int beta, Engine* e)
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
            vector<pair<int,  U16> > temp;
            for (auto move: moveset) {
                temp.push_back(make_pair(eval2(b, move), move));
            }
            sort(temp.begin(), temp.end());
            reverse(temp.begin(), temp.end());
            for (auto m1: temp) {
                U16 m = m1.second;
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

pair<int, U16> engine_b2::Min_value(const Board& b, int depth, int alpha, int beta, Engine* e)
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
            vector<pair<int,  U16> > temp;
            for (auto move: moveset) {
                temp.push_back(make_pair(eval2(b, move), move));
            }
            sort(temp.begin(), temp.end());
            for (auto m1: temp) {
                U16 m = m1.second;

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


pair<int, U16> engine_b2::MiniMax(const Board& b, PlayerColor colour, Engine* e)
{
    auto moveset = b.get_legal_moves();

    if (moveset.empty()) {
        return make_pair(0, 0);
    }
    else {
        auto ans = make_pair(0, 0);
        if (colour == BLACK)
        {
            ans = Min_value(b, 1, -100000, 100000, e);
//            cout<<"\n\n\n";
//            cout<< "final evaluation is, "<<ans.first<< " at MAX_DEPTH: "<<MAX_DEPTH<<'\n';
//            cout<<"best move at this depth is: "<<move_to_str(ans.second)<<"\n";
//            cout<<"\n\n\n";
            return ans;
        }
        else
        {
            ans = engine_b2::Max_value(b, 1, -100000, 100000, e);
//            cout<<"\n\n\n";
//            cout<< "final evaluation is, "<<ans.first<< " at MAX_DEPTH: "<<MAX_DEPTH<<'\n';
//            cout<<"best move at this depth is: "<<move_to_str(ans.second)<<"\n";
//            cout<<"\n\n\n";
            return ans;
        }
    }
}


U16 engine_b2::return_best_move(const Board &b, Engine *e) {
    start_time = chrono::high_resolution_clock::now();
    time_left_to_match = (int) e->time_left.count();
    cout << "time left to match: " << time_left_to_match << endl;
    auto moveset = b.get_legal_moves();

    if (moveset.size() == 1)
    {
        for (auto m : moveset)
        {
            return m;
        }
    }

    auto colour = b.data.player_to_play;
    auto curr_time = chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(curr_time - start_time);

    // Convert the duration to seconds (as a floating-point number)
    double elapsed_time = duration.count() / 1e6;

    vector<pair<int, pair<U16, Board>>> moveset_boards;

    MAX_DEPTH = 1;
    while (MAX_DEPTH <= 6)
    {

        auto p = MiniMax(b, colour, e);
        e->best_move = p.second;
        if ((p.first == 100000) && (b.data.player_to_play == WHITE))
            break;

        if ((p.first == -100000) && (b.data.player_to_play == BLACK))
            break;

        cout<<"\n\n";
        cout<< "final evaluation is, "<<p.first<< " at MAX_DEPTH: "<<MAX_DEPTH<<'\n';
        cout<<"best move at this depth is: "<<move_to_str(p.second)<<"\n";
        cout<<"\n\n";

        if (MAX_DEPTH == 1)
            MAX_DEPTH++;
        else
            MAX_DEPTH += 1;
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



