//
// Created by mjluv on 6/1/2020.
//
#pragma once

#include "model.hxx"
#include <vector>


class View
{
public:
    explicit View(Model const&); //make this model constant after i test one
    // thing

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
    int screen_to_board(ge211::Position) const;

private:
    Model const& model_;

    //stores the location of the outermost (the clickable) pieces. dark jail
    // is index 26, light jail is index 27
    std::vector<ge211::Position> outermost_pieces_{28, {-2, -2}};

    ge211::Image_sprite
            board_sprite_    {"board.jpg"};

    ge211::Circle_sprite
            dark_sprite_     {22, {120, 80, 25}};

    ge211::Circle_sprite
            light_sprite_    {22, {210, 210, 210}};

    ge211::Rectangle_sprite
            d_endzone_sprite_ {{50,13}, {120, 80, 25}};

    ge211::Rectangle_sprite
            l_endzone_sprite_ {{50,13}, {210, 210, 210}};

    ge211::Image_sprite
            dice_1_sprite_    {"side1.png"};

    ge211::Image_sprite
            dice_2_sprite_    {"side2.png"};

    ge211::Image_sprite
            dice_3_sprite_    {"side3.png"};

    ge211::Image_sprite
            dice_4_sprite_    {"side4.png"};

    ge211::Image_sprite
            dice_5_sprite_    {"side5.png"};

    ge211::Image_sprite
            dice_6_sprite_    {"side6.png"};

    //helper to place the dice
    void render_dice(ge211::Sprite_set&, int);



};


