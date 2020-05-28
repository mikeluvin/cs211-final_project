//
// Created by Anton de Lesseps on 5/27/20.
//

#include "dice.hxx"

Dice::Dice()
        : random_ (ge211::Abstract_game::get_random())
        , num_1_ (random.between(1, 6))
        , num_2_ (random.between(1, 6))
{}

void roll()
{
    num_1_ = random.between(1, 6);
    num_2_ = random.between(1, 6);
}

int num_1()
{
    return num_1_;
}

int num_2()
{
    return num_2;
}