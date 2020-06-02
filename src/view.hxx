//
// Created by mjluv on 6/1/2020.
//
#pragma once

#include "model.hxx"


class View
{
public:
    explicit View(Model const&);

    //pass in the mouse position of the piece we want to move, and the
    // position we want to move it to
    void draw(ge211::Sprite_set&, int from, int to);

    ge211::Dimensions initial_window_dimensions() const;

    std::string initial_window_title() const;

    //takes position on the board and converts it to
    // screen position. returns the upper left hand corner of the
    // screen location of the sprite.
    ge211::Position board_to_screen(int) const;

    //takes a given screen position and maps it to its location on the board
    ge211::Position screen_to_board(int) const;

private:
    Model const& model_;

    ge211::Image_sprite
            board_sprite_    {"board.jpg"};

    ge211::Image_sprite
            dark_sprite_     {"name of image"};

    ge211::Image_sprite
            light_sprite_    {"name of image"};


};


