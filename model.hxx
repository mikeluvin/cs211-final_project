#pragma once

#include <ge211.hxx>

//
// TODO: Sketch the structure of your model and declare its operations.
//

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

//***I actually don't think we even need a piece class based on how i
// constructed Board***
/*
class Piece
{
public:
    //constructs a piece at the given position for the given player.
    explicit Piece(int pos, Player player);

private:
    //position from 0-23(?), (what to make jail/endzone? maybe jail = 25,
    // endzone = 24?)
    int pos_;

    //the player this piece belongs to
    Player player_;

};
*/

class Board
{
public:
    //Constructs the default board, with all positions having 0 pieces and
    // with Player::neither
    // we should also make a constructor that makes the initial state of the
    // board
    Board();

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

private:

    //stores the number of pieces and player
    struct pos_info
    {
        int num_pieces;
        Player player;
        //initialized to {0, Player::neither}
    };

    //a vector which stores a pos_info struct in each position. The array
    // index corresponds to the position on the board, 0-25
    //**this is open to change, just how i thought to go about it
    std::vector<pos_info> positions_{26, {0, Player::neither}};

};


//idk if this is how you were thinking of doing this
class Dice
{
public:
    //constructs a dice class by randomly generating two ints between 1 and 6
    Dice();
    //returns the 1st die number
    int num_1();
    //returns the 2nd die number
    int num_2();
private:
    //randomly generated ints from 1-6
    int num_1_;
    int num_2_;

};


class Model
{

public:
    //returns whether the game is finished. This is true when one player has all
    //their pieces in their endzone (and turn_ == Player::neither).
    bool is_game_over() const;

    //returns the current turn
    Player turn() const;

    //returns the winner
    Player winner() const;

private:
    Player turn_;
    Player winner_;
    Board board_;

    //
    ///HELPER FUNCTIONS
    //

    //sends the piece at the given position to jail
    void go_to_jail_(int pos);

    //determines whether the given Player can move their piece to the given
    // position. Returns true if they can, false otherwise
    //(helper for find_moves_)
    bool evaluate_position_(int pos, Player);

    //finds the possible moves for a piece at the given position. returns a
    // vector of the possible positions
    std::vector<int> const find_moves_(int pos, Player);

    //Test access
    friend struct Test_access;

};