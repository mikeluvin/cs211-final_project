//
// Created by mjluv on 6/1/2020.
//

#include "view.hxx"

View::View(Model const& model)
        : model_(model)
{ }

ge211::Dimensions View::initial_window_dimensions() const
{
    return {800, 593};
}

std::string View::initial_window_title() const
{
    return "Backgammon";
}


void View::draw(ge211::Sprite_set& set, int from, int to)
{
    set.add_sprite(board_sprite_, {0,0});
}
