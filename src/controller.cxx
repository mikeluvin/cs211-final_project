#include "controller.hxx"

Controller::Controller()
        : model_(get_random())
        , view_(model_)
{ }

void Controller::draw(ge211::Sprite_set& sprites)
{
    view_.draw(sprites, from_pos_, to_pos_);
}

ge211::Dimensions Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

std::string Controller::initial_window_title() const
{
    return view_.initial_window_title();
}

void Controller::on_mouse_move(ge211::Position)
{
    //todo, not even sure we need this one actually
}

void Controller::on_mouse_up(ge211::Mouse_button, ge211::Position pos)
{
    //todo, definitely need this one

    int b_pos = view_.screen_to_board(pos);


    //this means that this is the first click of the sequence
    if (from_pos_ == -2 && to_pos_ == -2) {
        //if there's no moves to be had there, you can't click on it
        if (model_.find_moves_(b_pos).empty()) {
            return;
        //otherwise, there's possible moves, so set from_pos to b_pos
        } else {
            from_pos_ = b_pos;
        }
    } else if (to_pos_ == -2) {
        to_pos_ = b_pos;
        model_.play_move(from_pos_, to_pos_);
        from_pos_ = -2;
        to_pos_ = -2;
    }
}



