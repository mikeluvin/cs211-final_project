//
// Created by Anton de Lesseps on 5/27/20.
//
#pragma once
#include <ge211.hxx>

#ifndef FINAL_PROJECT_PLAYER_HXX
#define FINAL_PROJECT_PLAYER_HXX

#endif //FINAL_PROJECT_PLAYER_HXX

//even though there's no ties in Backgammon, we'll assign Player::neither when
// the game starts (maybe?) and when the game is over
enum class Player
{
    dark,
    light,
    neither,
};

//switch player
Player other_player(Player);