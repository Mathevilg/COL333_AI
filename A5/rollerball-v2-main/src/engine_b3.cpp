#include <bits/stdc++.h>
#include <string>
#include <iostream>
#include "board.hpp"
#include "butils.hpp"
#include "constants.hpp"
#include <cstring>

using namespace std;

class Board3
{
public:

    U16 return_best_move(const Board& b);


    int calculate_material(const Board& b)
    {
        return 10;
    }

};

