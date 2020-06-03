#include "model.hxx"

Model::Model(ge211::Random& rand)
        : dice_ (rand)
{
    if (dice_.num_1() >= dice_.num_2()) {
        turn_ = Player::dark;
    } else if (dice_.num_1() < dice_.num_2()) {
        turn_ = Player::light;
    }
}

bool Model::is_game_over() const
{
    return board().num_pieces(0) == 15 || board().num_pieces(25) == 15;
}

void Model::play_move(int pos_from, int pos_to)
{
    if (evaluate_position_(pos_from, pos_to)) {
        really_play_move_(pos_from, pos_to);
        //set the correct die to inactive
        inactivate_die(pos_from, pos_to);
        advance_turn_();
    }
}
//
// HELPER FUNCTIONS
//

//we need to implement a function to determine who goes first. Simple to
// write, but idk where exactly it belongs, cause it's a special scenario at
// the beginning of the game

//checks whether all of the current Player's pieces are in the final section
//(helper for evaluate_position_)
bool Model::all_in_final_()
{
    std::vector<int> pos_vec = board_.pos_final(turn_);
    int final_count = 0;

    for (int i : pos_vec)
        final_count += board_.num_pieces(i);

    final_count += board_.num_endzoned(turn_);

    return final_count == 15;
}

//checks if the dice are greater than the Player's pieces are away from
// the endzone
//(helper for evaluate_position, to be used after all_in_final_ is checked)
bool Model::leq_dice_()
{
    std::vector<int> pos_vec = board_.pos_final(turn_);

    //the pos_vec is formatted such that the farthest space away from the
    // endzone is the last element in the vector
    // if both dice are greater than the last space, then there's no move
    if (turn_ == Player::dark) {
        return (dice_.num_1_active() && dice_.num_1() >= pos_vec.back()) ||
        (dice_.num_2_active() && dice_.num_2() >= pos_vec.back());
    } else if (turn_ == Player::light) {
        // map 19 to 6, 24 to 1 so we can correctly compare the dice to the
        // positions
        return (dice_.num_1_active() && dice_.num_1() >=
        std::abs(pos_vec.back() - 25)) || (dice_.num_2_active() &&
        dice_.num_2() >= std::abs(pos_vec.back() - 25));
    }
    return false;
}

//determines whether the current Player can move their piece from pos_from
// to the given position pos_to. Returns true if they can, false otherwise
//(helper for find_moves_)
//
//we're going to need some variable in the controller to store the position
// of the first click (the selected piece) and the position of the second
// click (where the selected piece is being moved to)
bool Model::evaluate_position_(int pos_from, int pos_to)
{
    // -1 represents the position of the jail (but isn't actually the position
    // of the jail)
    if (pos_from == -1 && board_.num_jailed(turn_) > 0) {
        if (turn_ == Player::dark && !(pos_to >= 19 && pos_to <= 24)) {
            return false;
        } else if (turn_ == Player::light && !(pos_to >= 1 && pos_to <= 6)) {
            return false;
        } else if (board_.player(pos_to) == other_player(turn_) &&
        board_.num_pieces(pos_to) > 1) {
            return false;
        } else {
            return true;
        }
    } else if (pos_from == -1 && board_.num_jailed(turn_) == 0) {
        return false;
    }
    //check that move is in correct direction
    else if (turn_ == Player::dark && pos_to >= pos_from) {
        return false;
    } else if (turn_ == Player::light && pos_to <= pos_from) {
        return false;
    }
    //check if we're even clicking on a valid piece for the given player
    else if (board_.player(pos_from) == turn_ && board_.num_jailed(turn_)
    == 0) {

        //if the other player has more than once piece in pos_to, return false
        if (board_.player(pos_to) == other_player(turn_) && board_
        .num_pieces(pos_to) > 1) {
            return false;
        }

        //check if all are in final first, otherwise it's normal gameplay
        if (all_in_final_()) {
            if (leq_dice_()) {
                // can only play piece farthest from endzone
                // not necessary to match die value
                if (turn_ == Player::dark && pos_to == 0 && pos_from ==
                board_.pos_final(turn_).back()) {
                    return true;
                } else if (turn_ == Player::light && pos_to == 25 && pos_from
                == board_.pos_final(turn_).back()) {
                    return true;
                } else {
                    return false;
                }
            } else {
                // can only play pieces equal to or higher than dice
                // difference between positions must be die value
                if (turn_ == Player::dark) {
                    //todo: i dont think these conditions are correct. When i
                    // got to this stage of the game, i could no longer move
                    // the pieces and the game was stuck. We need to create a
                    // model initializer that automatically puts both
                    // players' pieces in their final stretch so we can
                    // properly test this.
                    if (dice_.num_1_active() && pos_from >= dice_.num_1() &&
                    pos_to - pos_from == -dice_.num_1()) {
                        return true;
                    } else if (dice_.num_2_active() && pos_from >= dice_.num_2()
                    && pos_to - pos_from == -dice_.num_2()) {
                        return true;
                    } else {
                        return false;
                    }
                } else if (turn_ == Player::light) {
                    if (dice_.num_1_active() && pos_from <= dice_.num_1() &&
                    pos_to - pos_from == dice_.num_1()) {
                        return true;
                    } else if (dice_.num_2_active() && pos_from <= dice_.num_2()
                    && pos_to - pos_from == dice_.num_2()) {
                        return true;
                    } else {
                        return false;
                    }
                }
            }
        } else {
            //normal gameplay when not all are in final, no one in jail, and
            // the spot has 1 or less of the other player's pieces
            //return true if either of the dice result in a valid move
            int dir;
            if (turn_ == Player::dark) {
                dir = -1;
            } else if (turn_ == Player::light) {
                dir = 1;
            }
            //if they try to play in an endzone, return false
            if (pos_to == 0 || pos_to == 25)
                return false;

            if (dice_.num_1_active() && dice_.num_2_active())
                return pos_from + dice_.num_1() * dir == pos_to || pos_from + dice_
                .num_2() * dir == pos_to;
            else if (dice_.num_1_active())
                return pos_from + dice_.num_1() * dir == pos_to;
            else if (dice_.num_2_active())
                return pos_from + dice_.num_2() * dir == pos_to;
        }
    }
    return false;
}

// dir is 1 (for light) or -1 (for dark)
// pos_start is the piece's position before any move
std::vector<int> Model::find_moves_helper_(int pos_start, int dir)
{
    std::vector<int> result;
    if (dice_.num_1_active() && dice_.num_2_active()) {
        if (board_.player(pos_start + dice_.num_1() * dir) == turn_ ||
        board_.num_pieces(pos_start + dice_.num_1() * dir) <= 1) {
            result.push_back(pos_start + dice_.num_1() * dir);
        }
        if (board_.player(pos_start + dice_.num_2() * dir) == turn_ ||
        board_.num_pieces(pos_start + dice_.num_2() * dir) <= 1) {
            result.push_back(pos_start + dice_.num_2() * dir);
        }
    } else if (dice_.num_1_active()) {
        if (board_.player(pos_start + dice_.num_1() * dir) == turn_ ||
        board_.num_pieces(pos_start + dice_.num_1() * dir) <= 1) {
            result.push_back(pos_start + dice_.num_1() * dir);
        }
    } else if (dice_.num_2_active()) {
        if (board_.player(pos_start + dice_.num_2() * dir) == turn_ ||
        board_.num_pieces(pos_start + dice_.num_2() * dir) <= 1) {
            result.push_back(pos_start + dice_.num_2() * dir);
        }
    } else {
        result = {};
    }

    for (int i = 0; i < result.size(); ++i) {
        if (dir == -1 && result[i] < 0) {
            result[i] = 0;
        } else if (dir == 1 && result[i] > 25) {
            result[i] = 25;
        }
    }

    return result;
}

// pos represents the piece that we're starting with
std::vector<int> Model::find_moves_(int pos)
{
    if (pos == -1 && board_.num_jailed(turn_) > 0) {
        if (turn_ == Player::dark) {
            return find_moves_helper_(25, -1);
        } else if (turn_ == Player::light) {
            return find_moves_helper_(0, 1);
        }
    } else if (pos == -1 && board_.num_jailed(turn_) == 0) {
        return {};
    } else if (board_.player(pos) != turn_) {
        return {};
    } else {
        if (turn_ == Player::dark) {
            return find_moves_helper_(pos, -1);
        } else if (turn_ == Player::light) {
            return find_moves_helper_(pos, 1);
        }
    }
    return {};
}

void Model::set_game_over_()
{
    turn_ = Player::neither;
    if (board_.num_endzoned(Player::dark) == 15) {
        winner_ = Player::dark;
    } else if (board_.num_endzoned(Player::light) == 15) {
        winner_ = Player::light;
    }
}

bool Model::no_next_moves_()
{
    //std::vector<int> temp_moves;
    if (board_.num_jailed(turn_) > 0) {
        return find_moves_(-1).empty();
    }

    for (int i = 1; i <= 24; ++i) {
        if (!(find_moves_(i).empty())) {
            return false;
        }
    }
    return true;
}

void Model::advance_turn_()
{
    if (is_game_over()) {
        set_game_over_();
        throw ge211::Client_logic_error("Game over");
    } else if (!(dice_.num_1_active()) && !(dice_.num_2_active())) {
        turn_ = other_player(turn_);
        dice_.roll();
    } else if (no_next_moves_()) {
        turn_ = other_player(turn_);
        dice_.roll();
    }
    if (no_next_moves_()) {
        //advance_turn_();
        //todo: we should show a message saying that someone's turn was
        // skipped since they had no moves. otherwise, you can only tell that
        // this happened due to the fact that the same player ends up being
        // able to move again
        turn_ = other_player(turn_);
        dice_.roll();
    }
}

// we assume move is valid
void Model::really_play_move_(int pos_from, int pos_to)
{
    //piece in jail case
    if (pos_from == -1) {
        if (board_.player(pos_to) == turn_ || board_.player(pos_to) == Player::neither) {
            board_.remove_from_jail(pos_to, turn_);
        } else if (board_.player(pos_to) == other_player(turn_)) {
            board_.send_to_jail(pos_to);
            board_.remove_from_jail(pos_to, turn_);
        }
    } else {
        if (board_.player(pos_to) == turn_ || board_.player(pos_to) == Player::neither) {
            board_.remove_piece(pos_from);
            board_.add_piece(pos_to, turn_);
        } else if (board_.player(pos_to) == other_player(turn_)) {
            board_.send_to_jail(pos_to);
            board_.remove_piece(pos_from);
            board_.add_piece(pos_to, turn_);
        }
    }
}

void Model::inactivate_die(int pos_from, int pos_to)
{
    //todo: i forgot to consider the scenario when all_in_final and leq are
    // true, so those need to be added

    if (pos_from == -1) {
        if (turn_ == Player::dark) {
            if (dice_.num_1_active() && dice_.num_1() == std::abs(pos_to -
                                                                  25)) {
                dice_.set_inactive(1);
            } else if (dice_.num_2_active() && dice_.num_2() == std::abs
                    (pos_to - 25)){
                dice_.set_inactive(2);
            }
        } else if (turn_ == Player::light) {
            if (dice_.num_1_active() && dice_.num_1() == pos_to) {
                dice_.set_inactive(1);
            } else if (dice_.num_2_active() && dice_.num_2() == pos_to){
                dice_.set_inactive(2);
            }
        }
    } else if (dice_.num_1_active() && dice_.num_1() == std::abs(pos_to -
                                                                 pos_from)) {
        dice_.set_inactive(1);
    } else if (dice_.num_2_active() && dice_.num_2() == std::abs(pos_to -
                                                                 pos_from)) {
        dice_.set_inactive(2);
    }
}
