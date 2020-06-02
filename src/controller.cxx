#include "controller.hxx"

Controller::Controller()
        : model_()
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