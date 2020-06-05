#include "model.hxx"
#include <iostream>

Model::Model(ge211::Random & rand)
        : dice_ (rand)
        , board_ (1)
{
    if (dice_.num_1() >= dice_.num_2()) {
        turn_ = Player::dark;
    } else if (dice_.num_1() < dice_.num_2()) {
        turn_ = Player::light;
    }
}


Model::Model(ge211::Random& rand, int board_state)
        : dice_ (rand)
        , board_ (board_state)
{
    if (board_state == 2){
        //need this for the one initialization case
        turn_ = Player::dark;
    } else if (dice_.num_1() >= dice_.num_2()) {
        turn_ = Player::dark;
    } else if (dice_.num_1() < dice_.num_2()) {
        turn_ = Player::light;
    }
}

Model::Model(ge211::Random& rand, int die_1, int die_2, Player player)
        : dice_ (rand, die_1, die_2)
        , turn_ (player)
        , board_ (true)
{ }


bool Model::is_game_over() const
{
    return board().num_pieces(0) == 15 || board().num_pieces(25) == 15;
}

// pos represents the piece that we're starting with
std::vector<int> Model::find_moves_(int pos) const
{
    if (pos == -1 && board_.num_jailed(turn_) > 0) {
        if (turn_ == Player::dark) {
            return find_moves_helper_(-1, -1);
        } else if (turn_ == Player::light) {
            return find_moves_helper_(-1, 1);
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

void Model::play_move(int pos_from, int pos_to)
{
    if (winner_ == Player::neither && evaluate_position_(pos_from, pos_to)) {
        really_play_move_(pos_from, pos_to);

        //set the correct die to inactive
        // this is being called
        inactivate_die(pos_from, pos_to);

        // this is not being called
        // here is the first thing that isn't being called
        // something's happening with inactivate die
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
bool Model::all_in_final_() const
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
bool Model::leq_die_(int dice_num) const
{
    std::vector<int> pos_vec = board_.pos_final(turn_);

    int num_on_die;
    if (dice_num == 1) {
        num_on_die = dice_.num_1();
    } else if (dice_num == 2) {
        num_on_die = dice_.num_2();
    }

    //the pos_vec is formatted such that the farthest space away from the
    // endzone is the last element in the vector
    // if both dice are greater than the last space, then there's no move
    if (turn_ == Player::dark) {
        if (pos_vec.empty()) {
            return true;
        } else {
            return num_on_die > pos_vec.back();
        }
    } else if (turn_ == Player::light) {
        // map 19 to 6, 24 to 1 so we can correctly compare the dice to the
        // positions
        if (pos_vec.empty()) {
            return true;
        } else {
            return num_on_die > std::abs(pos_vec.back() - 25);
        }
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
bool Model::evaluate_position_(int pos_from, int pos_to) const
{

    //can't move a piece out of an endzone or (manually) into jail
    if (pos_from == 0 || pos_from == 25){
        return false;
    }

    if (pos_to == -1){
        return false;
    }

    // -1 represents the position of the jail (but isn't actually the position
    // of the jail)
    if (board_.num_jailed(turn_) > 0) {
        //if there's a piece in jail, you must play that piece first
        if (pos_from != -1 || (board_.player(pos_to) == other_player(turn_) &&
                               board_.num_pieces(pos_to) > 1)) {
            return false;
        }
        if (turn_ == Player::dark) {
            if (dice_.num_1_active() && dice_.num_2_active()) {
                return 25 - dice_.num_1() == pos_to || 25 - dice_.num_2() ==
                pos_to;
            } else if (dice_.num_1_active()) {
                return 25 - dice_.num_1() == pos_to;
            } else if (dice_.num_2_active()) {
                return 25 - dice_.num_2() == pos_to;
            }
        } else if (turn_ == Player::light) {
            if (dice_.num_1_active() && dice_.num_2_active()) {
                return dice_.num_1() == pos_to || dice_.num_2() == pos_to;
            } else if (dice_.num_1_active()) {
                return dice_.num_1() == pos_to;
            } else if (dice_.num_2_active()) {
                return dice_.num_2() == pos_to;
            }
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
    else if (board_.player(pos_from) == turn_ && board_.num_jailed(turn_)== 0) {

        //if the other player has more than once piece in pos_to, return false
        if (board_.player(pos_to) == other_player(turn_) && board_
        .num_pieces(pos_to) > 1) {
            return false;
        }

        //check if all are in final first, otherwise it's normal gameplay
        if (all_in_final_()) {
            //both dice active
            if (dice_.num_1_active() && dice_.num_2_active()) {
                if (leq_die_(1) && leq_die_(2)) {
                    if (turn_ == Player::dark && pos_to == 0 && pos_from ==
                            board_.pos_final(turn_).back()) {
                        return true;
                    } else if (turn_ == Player::light && pos_to == 25 &&
                            pos_from == board_.pos_final(turn_).back()) {
                        return true;
                    }
                } else if (leq_die_(1)) {
                    //both active, only dice 1 leq
                    //implies dice 1 > dice 2
                    //**when only one dice is leq and BOTH are active, the
                    // die that is NOT leq MUST be played first!! So this
                    // case should just be
                    if (turn_ == Player::dark) {
                        return pos_to - pos_from == -dice_.num_2();
                    } else if (turn_ == Player::light) {
                        return pos_to - pos_from == dice_.num_2();
                    }

                } else if (leq_die_(2)) {
                    //both active, only dice 2 leq
                    if (turn_ == Player::dark) {
                        return pos_to - pos_from == -dice_.num_1();
                    } else if (turn_ == Player::light) {
                        return pos_to - pos_from == dice_.num_1();
                    }

                } else {
                    //both active, neither leq
                    if (turn_ == Player::dark) {
                        return pos_to - pos_from == -dice_.num_1() || pos_to -
                        pos_from == -dice_.num_2();
                    } else if (turn_ == Player::light) {
                        return pos_to - pos_from == dice_.num_1() || pos_to -
                        pos_from == dice_.num_2();
                    }
                }
            } else if (dice_.num_1_active()) {
                //only dice 1 active
                if (leq_die_(1)) {
                    //only dice 1 active and leq
                    if (turn_ == Player::dark) {
                        return pos_to == 0 && pos_from == board_.pos_final
                                                                        (turn_)
                                                                .back();
                    } else if (turn_ == Player::light) {
                        return pos_to == 25 && pos_from == board_.pos_final
                                                                         (turn_)
                                                                 .back();
                    }
                } else {
                    //only dice 1 active and dice 1 not leq
                    if (turn_ == Player::dark) {
                        return pos_to - pos_from == -dice_.num_1();
                    } else if (turn_ == Player::light) {
                        return pos_to - pos_from == dice_.num_1();
                    }
                }
            } else if (dice_.num_2_active()) {
                //only dice 2 active
                if (leq_die_(2)) {
                    //only dice 2 active and leq
                    if (turn_ == Player::dark) {
                        return pos_to == 0 && pos_from == board_.pos_final
                        (turn_).back();
                    } else if (turn_ == Player::light) {
                        return pos_to == 25 && pos_from == board_.pos_final
                        (turn_).back();
                    }
                } else {
                    //only dice 2 active and dice 2 not leq
                    if (turn_ == Player::dark) {
                        return pos_to - pos_from == -dice_.num_2();
                    } else if (turn_ == Player::light) {
                        return pos_to - pos_from == dice_.num_2();
                    }
                }
            } else {
                //neither active
                return false;
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
                return pos_from + dice_.num_1() * dir == pos_to || pos_from +
                dice_.num_2() * dir == pos_to;
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
std::vector<int> Model::find_moves_helper_(int pos_start, int dir) const
{
    std::vector<int> result;

    if (dir == 1) {
        for (int i = pos_start; i <= 25; i++) {
            if (evaluate_position_(pos_start, i)) {
                result.push_back(i);
            }
        }
    } else if (dir == -1) {
        int j = pos_start;
        //need to do this so the while loop below will work
        if (pos_start == -1) {
            j = 24;
        }

        while (j >= 0) {
            if (evaluate_position_(pos_start, j)) {
                result.push_back(j);
            }
            --j;
        }
    } else result = {};

    return result;
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
    //set the no more moves/skipped turn flags back to false
    no_more_moves_ = false;
    skipped_turn_ = false;
    if (is_game_over()) {
        set_game_over_();
        //throw ge211::Client_logic_error("Game over");
    } else if (!(dice_.num_1_active()) && !(dice_.num_2_active())) {
        turn_ = other_player(turn_);
        dice_.roll();
    } else if (no_next_moves_()) {
        //todo message here too: 'player turn_ had no more moves!'
        no_more_moves_ = true;
        turn_ = other_player(turn_);
        dice_.roll();
    }
    if (no_next_moves_()) {
        //advance_turn_();
        //todo: we should show a message saying that someone's turn was
        // skipped since they had no moves. otherwise, you can only tell that
        // this happened due to the fact that the same player ends up being
        // able to move again
        // 'player turn_ had no available moves!'
        skipped_turn_ = true;
        turn_ = other_player(turn_);
        dice_.roll();
    }
}

// we assume move is valid
void Model::really_play_move_(int pos_from, int pos_to)
{
    //piece in jail case
    if (pos_from == -1) {
        if (board_.player(pos_to) == turn_ || board_.player(pos_to) ==
        Player::neither) {
            board_.remove_from_jail(pos_to, turn_);
        } else if (board_.player(pos_to) == other_player(turn_)) {
            board_.send_to_jail(pos_to);
            board_.remove_from_jail(pos_to, turn_);
        }
    } else {
        if (board_.player(pos_to) == turn_ || board_.player(pos_to) ==
        Player::neither) {
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
    } else if (all_in_final_()) {
        if (dice_.num_1_active() && dice_.num_2_active()) {
            if (leq_die_(1) && leq_die_(2)) {
                dice_.set_inactive(1);
            } else if (leq_die_(1)) {
                if (turn_ == Player::dark) {
                    if (pos_to - pos_from == -dice_.num_2()) {
                        dice_.set_inactive(2);
                    } else {
                        dice_.set_inactive(1);
                    }
                } else if (turn_ == Player::light) {
                    if (pos_to - pos_from == dice_.num_2()) {
                        dice_.set_inactive(2);
                    } else {
                        dice_.set_inactive(1);
                    }
                }
            } else if (leq_die_(2)) {
                if (turn_ == Player::dark) {
                    if (pos_to - pos_from == -dice_.num_1()) {
                        dice_.set_inactive(1);
                    } else {
                        dice_.set_inactive(2);
                    }
                } else if (turn_ == Player::light) {
                    if (pos_to - pos_from == dice_.num_1()) {
                        dice_.set_inactive(1);
                    } else {
                        dice_.set_inactive(2);
                    }
                }
            } else {
                //both active, not leq for either
                if (turn_ == Player::dark) {
                    if (pos_to - pos_from == -dice_.num_1()) {
                        dice_.set_inactive(1);
                    } else if (pos_to - pos_from == -dice_.num_2()) {
                        dice_.set_inactive(2);
                    }
                } else if (turn_ == Player::light) {
                    if (pos_to - pos_from == dice_.num_1()) {
                        dice_.set_inactive(1);
                    } else if (pos_to - pos_from == dice_.num_2()) {
                        dice_.set_inactive(2);
                    }
                }
            }
        } else if (dice_.num_1_active()) {
            dice_.set_inactive(1);
        } else if (dice_.num_2_active()) {
            dice_.set_inactive(2);
        }
    } else if (dice_.num_1_active() && dice_.num_2_active()) {
        if (turn_ == Player::dark) {
            if (pos_to - pos_from == -dice_.num_1()) {
                dice_.set_inactive(1);
            } else if (pos_to - pos_from == -dice_.num_2()) {
                dice_.set_inactive(2);
            }
        } else if (turn_ == Player::light) {
            if (pos_to - pos_from == dice_.num_1()) {
                dice_.set_inactive(1);
            } else if (pos_to - pos_from == dice_.num_2()) {
                dice_.set_inactive(2);
            }
        }
    } else if (dice_.num_2_active()) {
        dice_.set_inactive(2);
    } else if (dice_.num_1_active()) {
        dice_.set_inactive(1);
    }
}


