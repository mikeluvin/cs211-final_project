//
// Created by Anton de Lesseps on 5/27/20.
//
#pragma once

#include "player.hxx"
#include <ge211.hxx>
#include <vector>


class Board
{
public:
//Constructs the default board, with all positions having 0 pieces
// if init is false, and with the proper initial setup if init is
// true
    Board(int board_state);

//returns the number of pieces at the given position on the board.
    int num_pieces(int pos) const;

//returns the player at the given position on the board.
    Player player(int pos) const;

//clears the given position, making num_pieces == 0 and Player ==
// Player::neither.
    void clear_pos(int pos);

//removes a piece from the given position on the board.
    void remove_piece(int pos);

//adds a piece to the given position for the given player.
    void add_piece(int pos, Player player);

//sends a single piece at the given position to jail
    void send_to_jail(int pos);

//removes piece of player from jail and adds to board at position pos
    void remove_from_jail(int pos, Player player);

//sends a single piece at the given position to its endzone
    //void send_to_endzone(int pos);

//returns number of jailed pieces for given player
    int num_jailed(Player player) const;

//returns number of endzoned pieces for given player
    int num_endzoned(Player player) const;

//returns a vector of the positions of the pieces in the player's final stretch
// for the given player
    std::vector<int> pos_final(Player) const;

private:

//stores the number of each type of piece in jail
    struct jail
    {
        int num_dark;
        int num_light;
    };

//the jail for the game
    jail jail_;

//stores the number of pieces and player
    struct pos_info
    {
        int num_pieces;
        Player player;
    };

//endzones
//make endzones positions 0 and 25 instead? i think it would make more sense
//pos_info dark_endzone_;
//pos_info light_endzone_;

//a vector which stores a pos_info struct in each position. The array
// index corresponds to the position on the board, 0-23
// dark's endzone is position 0, light's is 25
    std::vector<pos_info> positions_{26, {0, Player::neither}};
};