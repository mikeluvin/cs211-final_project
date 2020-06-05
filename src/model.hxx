#pragma once

#include "dice.hxx"
#include "board.hxx"
#include "player.hxx"
#include <ge211.hxx>
#include <vector>

//
// TODO: Sketch the structure of your model and declare its operations.
//

class Model
{
public:

//constructs the default model with the pieces properly intialized at the
// starting locations
    explicit Model(ge211::Random&);

//returns whether the game is finished. This is true when one player has all
//their pieces in their endzone (and turn_ == Player::neither).
    bool is_game_over() const;

//returns the current board, which we can use board's public functions on
    Board board() const
    { return board_; }

//returns the current turn
    Player turn() const
    { return turn_; }

    //returns the dice
    Dice dice() const
    { return dice_; }

//returns the winner
    Player winner() const
    { return winner_; }

    //finds the possible moves for a piece at the given position for the
    // current player. Returns a vector of the possible positions, or empty
    // if there's no moves
    //(helper for play_move)
    //**I made this public because it will be helpful to show potential moves
    std::vector<int> find_moves_(int pos) const;

//attempts to play the move at the given position. If successful, advances
// the state of the game to the correct player or game over.
// I think we also want to check here if there's no possible moves
    void play_move(int pos_from, int pos_to);

private:
    Player turn_;
    Player winner_ = Player::neither;
    Board board_ = Board(true);
    Dice dice_;

//
///HELPER FUNCTIONS
//

//checks whether all of the current Player's pieces are in the final section
//(helper for evaluate_position_)
    bool all_in_final_() const;

//checks if the dice are greater than or equal to the Player's pieces are
// away from the endzone
//(helper for evaluate_position, to be used after all_in_final_ is checked)
    bool leq_die_(int dice_num) const;

//determines whether the current Player can move their piece from pos_from
// to the given position pos_to. Returns true if they can, false otherwise
//(helper for find_moves_)
    bool evaluate_position_(int pos_from, int pos_to) const;

    //we need a function that checks if the current player has a piece in
    // jail, then does all the necessary moves to get all the pieces out of
    // jail. this is going to have to be a helper for play_move, as the jail
    // does not have an int position

    // helper for find_moves_
    std::vector<int> find_moves_helper_(int pos_start, int dir) const;

    // helper for advance_turn_
    bool no_next_moves_();

    //advances the turn by setting the turn_ to other_player(turn_),
    // checking for game over, rolling the dice
    void advance_turn_();

//checks to see if a player has all 15 pieces in their endzone, and if
// true, sets that player to the winner and turn_ to Player::neither
//(helper for really_play_move)
    void set_game_over_();

//actually executes the move by setting the relevant board positions
// (including moving a player to jail), setting the used die to inactive, and
// then advancing the turn and checking for the game to be over
//(helper for play_move)
    void really_play_move_(int pos_from, int pos_to);

    //sets the correct die to inactive when a move is played.
    //(helper for play_move)
    void inactivate_die(int pos_from, int pos_to);

//Test access
    friend struct Test_access;
};