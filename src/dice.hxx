//
// Created by Anton de Lesseps on 5/27/20.
//
#pragma once

#include <ge211.hxx>
#include <random>


class Dice
{
public:
    //constructs a dice class by randomly generating two ints between 1 and
    // 6, and setting both to active
    explicit Dice(ge211::Random&);

    //constructs a Dice class with dice with numbers die_1 and die_2. ONLY
    // used in testing.
    Dice(ge211::Random& rand, int die_1, int die_2);

    //produces new random ints between 1 and 6
    void roll();

    //returns the 1st die number
    int num_1() const;

    //returns whether or not the 1st die is active
    bool num_1_active() const;

    //returns the 2nd die number
    int num_2() const;

    //returns whether or not the 2nd die is active
    bool num_2_active() const;

    //sets the give die (1 or 2) inactive, or both (3)
    void set_inactive(int dice_num);

private:

    struct dice_num {
        int num;
        bool active;
    };
    //randomly generated ints from 1-6
    dice_num num_1_;
    dice_num num_2_;

    ge211::Random& random_;
};