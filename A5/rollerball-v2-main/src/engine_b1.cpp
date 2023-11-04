#include <engine_b1.hpp>

using namespace std;

class Board1
{
public:

    U16 return_best_move(const Board& b);


    int calculate_material(const Board& b)
    {
        return 10;
    }

};

