//
// Created by Anton de Lesseps on 5/27/20.
//

#include "dice.hxx"


Dice::Dice(ge211::Random& rand)
        : random_(rand)
        , num_1_ ({rand.between(1,6), true})
        , num_2_ ({rand.between(1,6), true})
{}

void Dice::roll()
{
    num_1_ = {random_.between(1, 6), true};
    num_2_ = {random_.between(1, 6), true};
}

int Dice::num_1() const
{
    return num_1_.num;
}

int Dice::num_2() const
{
    return num_2_.num;
}

void Dice::invert()
{
    num_1_.num *= -1;
    num_2_.num *= -1;
}

bool Dice::num_1_active() const
{
    return num_1_.active;
}

bool Dice::num_2_active() const
{
    return num_2_.active;
}

void Dice::set_inactive(int dice_num)
{
    if (dice_num == 1) {
        num_1_.active = false;
    } else if (dice_num == 2) {
        num_2_.active = false;
    } else if (dice_num == 3) {
        num_1_.active = false;
        num_2_.active = false;
    }
}