//
// Created by mjluv on 6/1/2020.
//
#pragma once

#include "model.hxx"
#include <vector>


class View
{
public:
    explicit View(Model const&);

    //pass in the mouse position of the piece we want to move, and the
    // position we want to move it to
    void draw(ge211::Sprite_set&, int from, int to);

    void show_winner(ge211::Sprite_set&);

    ge211::Dimensions initial_window_dimensions() const;

    std::string initial_window_title() const;

    //takes position on the board and converts it to
    // screen position. returns the upper left hand corner of the
    // screen location of the sprite.
    ge211::Position board_to_screen(int) const;

    //takes a given screen position and maps it to its location on the board
    int screen_to_board(ge211::Position) const;

private:
    int board_start_x_ = 86;
    int board_top_y_ = 21;
    int board_bot_y_ = 528;
    int token_rad_ = 22;
    int token_spacing_x_ = 50;
    int token_diameter_ = 44; //diameter of tokens
    int board_rhs_x_ = 401 + token_diameter_/2;
    int jail_x_ = 400 - token_diameter_/2;
    int jail_y_ = 255;
    int endzone_x_ = 736;
    int l_end_y_ = 227;
    int d_end_y_ = 351;
    int endzone_spacing_ = 14;
    int endzone_width_ = 50;
    double dice_scale_ = 0.4;
    int highlight_width_ = 44;
    int highlight_height_ = 15;

    bool win_sprite_updated_ = false;

    ge211::Position dice_1_pos_ = {17, 250};
    ge211::Position dice_2_pos_ = {17, 300};

    Model const& model_;

    //stores the location of the outermost (the clickable) pieces. dark jail
    // is index 26, light jail is index 27
    std::vector<ge211::Position> outermost_pieces_{28, {-2, -2}};

    //helper to place the dice
    void render_dice(ge211::Sprite_set&, int);

    ge211::Image_sprite
            board_sprite_    {"board.jpg"};

    ge211::Circle_sprite
            dark_sprite_     {token_rad_, {120, 80, 25}};

    ge211::Circle_sprite
            light_sprite_    {token_rad_, {210, 210, 210}};

    ge211::Rectangle_sprite
            d_endzone_sprite_ {{50,13}, {120, 80, 25}};

    ge211::Rectangle_sprite
            l_endzone_sprite_ {{50,13}, {210, 210, 210}};

    //to show possible moves
    ge211::Rectangle_sprite
            highlight_sprite_ {{highlight_width_, highlight_height_},
                               {100,255,100}};

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

    ge211::Rectangle_sprite
            msg_box_sprite_ {{252, 40},
                               {0,0,0}};

    ge211::Font sans72 {"sans.ttf", 72};

    ge211::Text_sprite win_sprite_;

    ge211::Text_sprite::Builder builder_;
    ge211::Color builder_color_ = {204, 204, 255};


    ge211::Font sans20 {"sans.ttf", 20};
    ge211::Text_sprite turn_sprite_ = ge211::Text_sprite("Turn:", sans20);
    ge211::Text_sprite no_moves_sprite_ = ge211::Text_sprite("No more moves, "
                                                             "next player",
                                                             sans20);
    ge211::Text_sprite skipped_sprite_ = ge211::Text_sprite("Other player turn "
                                                            "skipped",
                                                             sans20);
};