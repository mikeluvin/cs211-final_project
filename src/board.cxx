//
// Created by Anton de Lesseps on 5/27/20.
//

#include "board.hxx"
#include "player.hxx"

Board::Board(int board_state)
        : jail_ {0, 0}
{
    if (board_state == 2) {
        //for testing the 'skipped turn' messages
        jail_ = {0, 2};
        positions_[1] = {2, Player::dark};
        positions_[2] = {2, Player::dark};
        positions_[3] = {2, Player::dark};
        positions_[4] = {2, Player::dark};
        positions_[5] = {2, Player::dark};
        positions_[6] = {2, Player::dark};
        positions_[7] = {3, Player::dark};

        positions_[10] = {6, Player::light};
        positions_[13] = {7, Player::light};

    } else if (board_state == 0) {
        // for testing logic at end of game
        positions_[0] = {14, Player::dark};
        positions_[1] = {1, Player::dark};

        positions_[24] = {5, Player::light};
        positions_[23] = {10, Player::light};

    } else {
        //default setup
        positions_[6]  = {5, Player::dark};
        positions_[8]  = {3, Player::dark};
        positions_[13] = {5, Player::dark};
        positions_[24] = {2, Player::dark};

        positions_[19] = {5, Player::light};
        positions_[17] = {3, Player::light};
        positions_[12] = {5, Player::light};
        positions_[1]  = {2, Player::light};
    }
}

int Board::num_pieces(int pos) const
{
    return positions_[pos].num_pieces;
}

//returns the player at the given position on the board.
Player Board::player(int pos) const
{
    return positions_[pos].player;
}

//clears the given position, making num_pieces == 0 and Player ==
// Player::neither.
void Board::clear_pos(int pos)
{
    positions_[pos] = {0, Player::neither};
}

//removes a piece from the given position on the board.
void Board::remove_piece(int pos)
{
    if (num_pieces(pos) <= 1) {
        clear_pos(pos);
    } else {
        positions_[pos].num_pieces -= 1;
    }
}

//adds a piece to the given position for the given player.
void Board::add_piece(int pos, Player thisplayer)
{
    if (player(pos) == thisplayer) {
        positions_[pos].num_pieces += 1;
    } else if (player(pos) == Player::neither) {
        positions_[pos].player = thisplayer;
        positions_[pos].num_pieces = 1;
    } else {
        throw ge211::Client_logic_error("Can't add piece here");
    }
}

//sends a single piece at the given position to jail
void Board::send_to_jail(int pos)
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
void Board::remove_from_jail(int pos, Player thisplayer)
{
    if (thisplayer == Player::dark) {
        if (jail_.num_dark <= 1) {
            jail_.num_dark = 0;
        } else {
            jail_.num_dark -= 1;
        }
    } else if (thisplayer == Player::light) {
        if (jail_.num_light <= 1) {
            jail_.num_light = 0;
        } else {
            jail_.num_light -= 1;
        }
    }

    add_piece(pos, thisplayer);
}

//sends a single piece at the given position to its endzone
void Board::send_to_endzone(int pos)
{
    Player thisplayer = player(pos);
    remove_piece(pos);
    if (thisplayer == Player::light && pos >= 19 && pos <= 24) {
        add_piece(25, thisplayer);
    } else if (thisplayer == Player::dark && pos <= 6 && pos >= 1) {
        add_piece(0, thisplayer);
    } else {
        throw ge211::Client_logic_error("Can't send this piece to endzone");
    }
}

//returns number of jailed pieces for given player
int Board::num_jailed(Player thisplayer) const
{
    if (thisplayer == Player::dark) {
        return jail_.num_dark;
    } else if (thisplayer == Player::light) {
        return jail_.num_light;
    } else {
        return 0; //shouldn't be passing player::neither into here
    }
}

//returns number of endzoned pieces for given player
int Board::num_endzoned(Player thisplayer) const
{
    if (thisplayer == Player::dark) {
        return num_pieces(0);
    } else if (thisplayer == Player::light) {
        return num_pieces(25);
    } else {
        return 0;
    }
}

//returns a vector of the positions of the pieces in the player's final stretch
// for the given player
std::vector<int> Board::pos_final(Player thisplayer) const
{
    std::vector<int> result{};

    if (thisplayer == Player::dark) {
        for (int i = 1; i <= 6; ++i) {
            if (player(i) == thisplayer && num_pieces(i) > 0) {
                result.push_back(i);
            }
        }

    } else if (thisplayer == Player::light) {
        //doing this loop this way so that we start at one space away from
        // light's endzone and go up to 6 spaces away.
        for (int i = 24; i >= 19; --i) {
            if (player(i) == thisplayer && num_pieces(i) > 0) {
                result.push_back(i);
            }
        }
    }

    return result;
}