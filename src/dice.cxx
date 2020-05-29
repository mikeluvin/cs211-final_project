//
// Created by Anton de Lesseps on 5/27/20.
//

#include "dice.hxx"


Dice::Dice()
        : generator_ ()
        , num_1_ (random_(generator_))
        , num_2_ (random_(generator_))
{}

void Dice::roll()
{
    num_1_ = random_(generator_);
    num_2_ = random_(generator_);

}

int Dice::num_1()
{
    return num_1_;
}

int Dice::num_2()
{
    return num_2_;
}