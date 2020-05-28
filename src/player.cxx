//
// Created by Anton de Lesseps on 5/27/20.
//
#include "player.hxx"

Player other_player(Player player)
{
    switch (player) {
        case Player::light: return Player::dark;
        case Player::dark: return Player::light;
        default: return Player::neither;
    }
}