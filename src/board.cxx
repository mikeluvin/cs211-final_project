//
// Created by Anton de Lesseps on 5/27/20.
//

#include "board.hxx"
#include "player.hxx"

Board::Board(bool init)
        : jail_ {0, 0}
{
    if (init) {
        positions_[6] = {5, Player::dark};
        positions_[8] = {3, Player::dark};
        positions_[13] = {5, Player::dark};
        positions_[24] = {2, Player::dark};

        positions_[19] = {5, Player::light};
        positions_[17] = {3, Player::light};
        positions_[12] = {5, Player::light};
        positions_[1] = {2, Player::light};
    }
}

int num_pieces(int pos)
{
    returns positions_[pos].num_pieces;
}

//returns the player at the given position on the board.
Player player(int pos)
{
    return positions_[pos].player;
}

//clears the given position, making num_pieces == 0 and Player ==
// Player::neither.
void clear_pos(int pos)
{
    positions_[pos] = {0, Player::neither};
}

//removes a piece from the given position on the board.
void remove_piece(int pos)
{
    if (num_pieces(pos) <= 1) {
        clear_pos(pos);
    } else {
        positions_[pos].num_pieces -= 1;
    }
}

//adds a piece to the given position for the given player.
void add_piece(int pos, Player player) {
    if (player(pos) == player) {
        positions_[pos].num_pieces += 1;
    } else if (player(pos) == Player::neither) {
        positions_[pos].player = player;
        positions_[pos].num_pieces = 1;
    } else {
        throw Client_logic_error("Can't add piece here");
    }
}

//sends a single piece at the given position to jail
void send_to_jail(int pos)
{
    Player thisplayer = player(pos);
    remove_piece(pos);
    if (thisplayer == Player::dark) {
        jail_.num_dark += 1;
    } else if (thisplayer == Player::light) {
        jail_.num_light += 1;
    }
}

//removes piece of player from jail and adds to board at position pos
void remove_from_jail(int pos, Player player)
{
    if (player == Player::dark) {
        if (jail_.num_dark <= 1) {
            jail_.num_dark = 0;
        } else {
            jail_.num_dark -= 1;
        }
    } else if (player == Player::light) {
        if (jail_.num_light <= 1) {
            jail_.num_light = 0;
        } else {
            jail_.num_light -= 1;
        }
    }

    add_piece(pos, player);
}

//sends a single piece at the given position to its endzone
void send_to_endzone(int pos)
{
    Player thisplayer = player(pos);
    remove_piece(pos);
    if (thisplayer == Player::dark && pos >= 19 && pos <= 24) {
        add_piece(25, thisplayer);
    } else if (thisplayer == Player::light && pos <= 6 && pos >= 1) {
        add_piece(0, thisplayer);
    } else {
        throw Client_logic_error("Can't send this piece to endzone");
    }
}

//returns number of jailed pieces for given player
int num_jailed(Player player)
{
    if (player == Player::dark) {
        return jail_.num_dark;
    } else if (player == Player::light) {
        return jail_.num_light;
    }
}

//returns number of endzoned pieces for given player
int num_endzoned(Player player)
{
    if (player == Player::dark) {
        return num_pieces(25);
    } else if (player == Player::light) {
        return num_pieces(0);
    }
}