#include <algorithm>
#include <random>
#include <iostream>
#include <thread>

#include "board.hpp"
#include "engine.hpp"
#include "butils.hpp"

class Board1
{
public:

    U16 return_best_move(const Board& b);


    int calculate_material(const Board& b);

};
