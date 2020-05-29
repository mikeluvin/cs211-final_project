//
// Created by Anton de Lesseps on 5/27/20.
//
#pragma once

#include <ge211.hxx>
/*
#ifndef FINAL_PROJECT_DICE_HXX
#define FINAL_PROJECT_DICE_HXX

#endif //FINAL_PROJECT_DICE_HXX
*/

class Dice
{
public:
//constructs a dice class by randomly generating two ints between 1 and 6
    Dice();

//produces new random ints between 1 and 6
    void roll();

//returns the 1st die number
    int num_1();

//returns the 2nd die number
    int num_2();

private:
    std::default_random_engine generator_;
    std::uniform_int_distribution<int> random_{1, 6};
//randomly generated ints from 1-6
    int num_1_;
    int num_2_;
};