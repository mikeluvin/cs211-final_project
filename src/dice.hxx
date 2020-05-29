//
// Created by Anton de Lesseps on 5/27/20.
//
#pragma once

#include <ge211.hxx>
#include <random>
/*
#ifndef FINAL_PROJECT_DICE_HXX
#define FINAL_PROJECT_DICE_HXX

#endif //FINAL_PROJECT_DICE_HXX
*/

class Dice
{
public:
    //constructs a dice class by randomly generating two ints between 1 and
    // 6, and setting both to active
    Dice();

    //produces new random ints between 1 and 6
    void roll();

    //inverts the dice to -1 to -6 (useful when player is Dark)
    void invert();

    //returns the 1st die number
    int num_1() const;

    //returns whether or not the 1st die is active
    bool num_1_active() const;

    //returns the 2nd die number
    int num_2() const;

    //returns whether or not the 2nd die is active
    bool num_2_active() const;

    //sets the give die (1 or 2) inactive, or both (3)
    void set_inactive(int);

private:
    std::default_random_engine generator_;
    std::uniform_int_distribution<int> random_{1, 6};

    struct dice_num {
        int num;
        bool active;
    };
    //randomly generated ints from 1-6
    dice_num num_1_;
    dice_num num_2_;
};