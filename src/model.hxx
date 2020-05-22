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

    //sends a single piece at the given position to its endzone
    void send_to_endzone(int pos);

    //returns number of jailed pieces for given player
    int num_jailed(Player player);

    //returns number of endzoned pieces for given player
    int num_endzoned(Player player);

    //removes piece of player from jail to position pos
    void remove_from_jail(int pos, Player player);

private:

    //stores the number of each type of piece in jail
    struct jail
    {
        int num_dark;
        int num_light;
    };

    //the jail for the game
    static jail board_jail;

    //stores the number of pieces and player
    struct pos_info
    {
        int num_pieces;
        Player player;
        //initialized to {0, Player::neither}
    };

    //endzones
    pos_info dark_endzone_;
    pos_info light_endzone_;

    //a vector which stores a pos_info struct in each position. The array
    // index corresponds to the position on the board, 0-23
    std::vector<pos_info> positions_{24, {0, Player::neither}};

    //vector of all 15 dark pieces
    std::vector<Piece> dark_pieces_;

    //vector of all 15 light pieces
    std::vector<Piece> light_pieces_;
};

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
    Dice dice_;

    //
    ///HELPER FUNCTIONS
    //

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