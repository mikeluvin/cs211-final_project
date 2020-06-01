#pragma once

#include "dice.hxx"
#include "board.hxx"
#include "player.hxx"
#include <ge211.hxx>
#include <vector>

//
// TODO: Sketch the structure of your model and declare its operations.
//

//added to own file
/*
//even though there's no ties in Backgammon, we'll assign Player::neither when
// the game starts (maybe?) and when the game is over
enum class Player
{
    dark,
    light,
    neither,
};
//function that will go in player.hxx
Player other_player(Player);
 */

//removed from project
/*
//a class to store information about a single piece on the board
class Piece
{
public:
//constructs a piece at the given position for the given player.
    explicit Piece(int pos, Player player);

private:
//position from 0-23
    int pos_;

//the player this piece belongs to
    Player player_;

//whether this piece is in jail
    bool jailed?_;

//whether this piece is in its endzone
    bool endzoned?_;
};
 */

//added to own file
/*
class Board
{
public:
//Constructs the default board, with all positions having 0 pieces
// if init is false, and with the proper initial setup if init is
// true
    Board(bool init);

//returns the number of pieces at the given position on the board.
    int num_pieces(int pos);

//returns the player at the given position on the board.
    Player player(int pos);

//clears the given position, making num_pieces == 0 and Player ==
// Player::neither.
    void clear_pos(int pos);

//removes a piece from the given position on the board.
    void remove_piece(int pos);

//adds a piece to the given position for the given player.
    void add_piece(int pos, Player);

//sends a single piece at the given position to jail
    void send_to_jail(int pos);

//removes piece of player from jail and adds to board at position pos
    void remove_from_jail(int pos, Player player);

//sends a single piece at the given position to its endzone
    void send_to_endzone(int pos);

//returns number of jailed pieces for given player
    int num_jailed(Player player);

//returns number of endzoned pieces for given player
    int num_endzoned(Player player);


private:

//stores the number of each type of piece in jail
    struct jail
    {
        int num_dark;
        int num_light;
    };

//the jail for the game
    static jail board_jail_;

//stores the number of pieces and player
    struct pos_info
    {
        int num_pieces;
        Player player;
//initialized to {0, Player::neither}
    };

//endzones
//make endzones positions 0 and 25 instead? i think it would make more sense
//pos_info dark_endzone_;
//pos_info light_endzone_;

//a vector which stores a pos_info struct in each position. The array
// index corresponds to the position on the board, 0-23
    std::vector<pos_info> positions_{26, {0, Player::neither}};

//vector of all 15 dark pieces
    std::vector<Piece> dark_pieces_;

//vector of all 15 light pieces
    std::vector<Piece> light_pieces_;
};
 */

//added to own class
/*
//dice for dice rolls
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
//randomly generated ints from 1-6
    int num_1_;
    int num_2_;
};
 */


class Model
{
public:

//constructs the default model with the pieces properly intialized at the
// starting locations
    Model();

//returns whether the game is finished. This is true when one player has all
//their pieces in their endzone (and turn_ == Player::neither).
    bool is_game_over() const;

//returns the current board, which we can use board's public functions on
    Board board() const
    { return board_; }

//returns the current turn
    Player turn() const
    { return turn_ ; }

//returns the winner
    Player winner() const
    { return winner_; }

//attempts to play the move at the given position. If successful, advances
// the state of the game to the correct player or game over.
// I think we also want to check here if there's no possible moves
    void play_move(int pos_from, int pos_to);

private:
    Player turn_;
    Player winner_ = Player::neither;
    Board board_ = Board(true);
    Dice dice_;

    //we may want to make a class that contains the positions that have moves
    // after the dice are rolled?

//
///HELPER FUNCTIONS
//

//checks whether all of the current Player's pieces are in the final section
//(helper for evaluate_position_)
    bool all_in_final_();

//checks if the dice are greater than or equal to the Player's pieces are
// away from the endzone
//(helper for evaluate_position, to be used after all_in_final_ is checked)
    bool leq_dice_();

//determines whether the current Player can move their piece from pos_from
// to the given position pos_to. Returns true if they can, false otherwise
//(helper for find_moves_)
    bool evaluate_position_(int pos_from, int pos_to);

    //we need a function that checks if the current player has a piece in
    // jail, then does all the necessary moves to get all the pieces out of
    // jail. this is going to have to be a helper for play_move, as the jail
    // does not have an int position

    // helper for find_moves_
    std::vector<int> find_moves_helper_(int pos_start, int dir)

//finds the possible moves for a piece at the given position for the
// current player. Returns a vector of the possible positions, or empty
// if there's no moves
//(helper for play_move)
    std::vector<int> const find_moves_(int pos);

    // helper for advance_turn_
    bool no_next_moves?_();

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

//Test access
    friend struct Test_access;

};