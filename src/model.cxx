#include "model.hxx"

Model::Model()
{

}

bool Model::is_game_over() const
{

    return board().num_pieces(0) == 15 || board().num_pieces(25) == 15;
}


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
bool Model::lower_than_dice_()
{
    std::vector<int> pos_vec = board().pos_final(turn());

    //the pos_vec is formatted such that the farthest space away from the
    // endzone is the last element in the vector
    // if both dice are greater than the last space, then there's no move
    if (turn() == Player::dark) {
        return dice_.num_1() > pos_vec.back() && dice_.num_1() > pos_vec.back();
    } else if (turn() == Player::light) {
        // map 19 to 6, 24 to 1 so we can correctly compare the dice to the
        // positions
        return dice_.num_1() > std::abs(pos_vec.back() - 25) && dice_.num_1() >
        std::abs(pos_vec.back() - 25);
    }
    return false;
}
