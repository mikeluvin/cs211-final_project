#include "model.hxx"

Model::Model()
{

}

bool Model::is_game_over() const
{

    return board().num_pieces(0) == 15 || board().num_pieces(25) == 15;
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
    std::vector<int> pos_vec = board().pos_final(turn());
    int final_count = 0;

    for (int i : pos_vec)
        final_count += board().num_pieces(i);

    final_count += board().num_endzoned(turn());

    return final_count == 15;
}

//checks if the dice are greater than the Player's pieces are away from
// the endzone
//(helper for evaluate_position, to be used after all_in_final_ is checked)
bool Model::leq_dice_()
{
    std::vector<int> pos_vec = board().pos_final(turn());

    //the pos_vec is formatted such that the farthest space away from the
    // endzone is the last element in the vector
    // if both dice are greater than the last space, then there's no move
    if (turn() == Player::dark) {
        return (dice_.num_1_active() && dice_.num_1() >= pos_vec.back()) ||
        (dice_.num_2_active() && dice_.num_2() >= pos_vec.back());
    } else if (turn() == Player::light) {
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
    if (pos_from == -1 && board().num_jailed(turn()) > 0) {
        if (turn() == Player::dark && !(pos_to >= 19 && pos_to <= 24)) {
            return false;
        } else if (turn() == Player::light && !(pos_to >= 1 && pos_to <= 6)) {
            return false;
        } else if (board_.player(pos_to) == other_player(turn()) &&
        board_.num_pieces(pos_to) > 1) {
            return false;
        } else {
            return true;
        }
    }
    //check that move is in correct direction
    else if (turn() == Player::dark && pos_to >= pos_from) {
        return false;
    } else if (turn() == Player::light && pos_to <= pos_from) {
        return false;
    }
    //check if we're even clicking on a valid piece for the given player
    else if (board().player(pos_from) == turn() && board().num_jailed(turn())
    == 0) {

        //negate dice since dark goes from 24 -> 0
        if (turn() == Player::dark)
            dice_.invert();

        //if the other player has more than once piece in pos_to, return false
        if (board_.player(pos_to) == other_player(turn()) && board_
        .num_pieces(pos_to) > 1) {
            return false;
        }

        //check if all are in final first, otherwise it's normal gameplay
        if (all_in_final_()) {
            if (leq_dice_()) {
                // can only play piece farthest from endzone
                // not necessary to match die value
                if (turn() == Player::dark && pos_to == 0 && pos_from ==
                board_.pos_final(turn()).back()) {
                    return true;
                } else if (turn() == Player::light && pos_to == 25 && pos_from
                == board_.pos_final(turn()).back()) {
                    return true;
                } else {
                    return false;
                }
            } else {
                // can only play pieces equal to or higher than dice
                // difference between positions must be die value
                if (turn() == Player::dark) {
                    if (dice_.num_1_active() && pos_from >= dice_.num_1() &&
                    pos_to - pos_from == -dice_.num_1) {
                        return true;
                    } else if (dice_.num_2_active() && pos_from >= dice_.num_2()
                    && pos_to - pos_from == -dice_.num_2) {
                        return true;
                    } else {
                        return false;
                    }
                } else if (turn() == Player::light) {
                    if (dice_.num_1_active() && pos_from <= dice_.num_1() &&
                    pos_to - pos_from == dice_.num_1) {
                        return true;
                    } else if (dice_.num_2_active() && pos_from <= dice_.num_2()
                    && pos_to - pos_from == dice_.num_2) {
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
            if (dice_.num_1_active() && dice_.num_2_active())
                return pos_from + dice_.num_1() == pos_to || pos_from + dice_
                .num_2() == pos_to;
            else if (dice_.num_1_active())
                return pos_from + dice_.num_1() == pos_to;
            else if (dice_.num_2_active())
                return pos_from + dice_.num_2() == pos_to;
        }
    }
    return false;
}