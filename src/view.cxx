//
// Created by mjluv on 6/1/2020.
//

#include "view.hxx"
#include <algorithm>

/*
int board_start_x = 86;
int board_top_y = 21;
int board_bot_y = 528;
int token_spacing_x = 50;
int token_diameter = 44; //diameter of tokens
int board_rhs_x = 401 + token_diameter/2;
int jail_x = 400 - token_diameter/2;
int jail_y = 255;
int endzone_x = 736;
int l_end_y = 227;
int d_end_y = 351;
int endzone_spacing = 14;
int endzone_width = 50;
double dice_scale = 0.4;
ge211::Position dice_1_pos = {17, 250};
ge211::Position dice_2_pos = {17, 300};
 */

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

    //use model_.board().num_pieces(pos) to draw the correct number of pieces
    // at those positions. We'll iterate over the entire board from 1 to 24.
    // The jail and the endzones will be treated separately.

    //the top and the bottom need to be treated separately.
    //bottom row
    for (int i = 1; i <= 12; ++i) {
        //see which sprite to use. I tried doing this without repeating the
        // for loop, but it just wouldn't work, so we're doing it this way.
        int num_pieces = model_.board().num_pieces(i);
        ge211::Position screen_pos = board_to_screen(i);

        //if (model_.board().player(i) == Player::neither) {
            //continue;
        // } else
        if (model_.board().player(i) == Player::dark) {
            for (int j = 0; j < num_pieces; ++j) {
                screen_pos.y = board_bot_y_ - j * token_diameter_;
                set.add_sprite(dark_sprite_, screen_pos,1);
            }
        } else if (model_.board().player(i) == Player::light) {
            for (int j = 0; j < num_pieces; ++j) {
                screen_pos.y = board_bot_y_ - j * token_diameter_;
                set.add_sprite(light_sprite_, screen_pos,1);
            }
        }
        //if there's no player at the location, then fill the vector with the
        // first vacant position, which is what screen_pos is initialized as
        outermost_pieces_[i] = screen_pos;
    }

    //top row
    for (int i = 13; i <= 24; ++i) {

        int num_pieces = model_.board().num_pieces(i);
        ge211::Position screen_pos = board_to_screen(i);

        //if (model_.board().player(i) == Player::neither) {
          // continue;
        //}
        if (model_.board().player(i) == Player::dark) {
            for (int j = 0; j < num_pieces; ++j) {
                screen_pos.y = board_top_y_ + j * token_diameter_;
                set.add_sprite(dark_sprite_, screen_pos, 1);
            }
        } else if (model_.board().player(i) == Player::light) {
            for (int j = 0; j < num_pieces; ++j) {
                screen_pos.y = board_top_y_ + j * token_diameter_;
                set.add_sprite(light_sprite_, screen_pos, 1);
            }
        }
        outermost_pieces_[i] = screen_pos;
    }

    //each endzoned piece will be represented by a rectangle in the respective
    // endzone
    //light's endzone is the top right, dark's is bottom right
    for (int i = 0; i < model_.board().num_endzoned(Player::dark); ++i) {
        set.add_sprite(d_endzone_sprite_, {endzone_x_, d_end_y_ + i *
          endzone_spacing_}, 1);
    }
    for (int i = 0; i < model_.board().num_endzoned(Player::light); ++i) {
        set.add_sprite(l_endzone_sprite_, {endzone_x_, l_end_y_ - i *
          endzone_spacing_}, 1);
    }

    //jail
    int light_jailed = model_.board().num_jailed(Player::light);
    int dark_jailed = model_.board().num_jailed(Player::dark);

    ge211::Position screen_pos = board_to_screen(-1);
    //additional dark jail pieces will be added above the first one
    if (dark_jailed > 0) {
        for (int j = 0; j < dark_jailed; ++j) {
            screen_pos.y = jail_y_ - j * token_diameter_;
            set.add_sprite(dark_sprite_, screen_pos, 1);
        }
        outermost_pieces_[26] = screen_pos;
    }
    //additional light jail pieces will be added below the first one
    if (light_jailed > 0) {
        for (int j = 0; j < light_jailed; ++j) {
            screen_pos.y = jail_y_ + (j + 1) * token_diameter_;
            set.add_sprite(light_sprite_, screen_pos, 1);
        }
        outermost_pieces_[27] = screen_pos;
    }

    //the dice
    if (model_.dice().num_1_active()) {
        render_dice(set, 1);
    }
    if (model_.dice().num_2_active()) {
        render_dice(set, 2);
    }

    //todo: make it so that when we click on a piece, we see the available
    // moves for that piece. I made Model::find_moves_ public so we can do this

    if (from != -2 && to == -2) {
        for (int move : model_.find_moves_(from)) {
            if (move >= 13 && move <= 24) {
                set.add_sprite(highlight_sprite_, {board_to_screen(move).x,
                                                   board_to_screen(move).y -
                                                   highlight_height_}, 1);
            } else if (move >= 1 && move <= 12) {
                set.add_sprite(highlight_sprite_, {board_to_screen(move).x,
                                                   board_to_screen(move).y +
                                                   token_diameter_}, 1);
            } else if (move == 25) {
                set.add_sprite(highlight_sprite_, {board_to_screen(move).x,
                                                   board_to_screen(move - 1).y -
                                                   highlight_height_}, 1);
            } else if (move == 0) {
                set.add_sprite(highlight_sprite_, {board_to_screen(move).x,
                                                   board_to_screen(move + 1).y +
                                                   token_diameter_}, 1);
            }
        }
    }
}

ge211::Position View::board_to_screen(int b_pos) const
{
    //position 1, bottom right: {board_rhs_x+token_spacing_x * 5, 524}
    //position 2: {board_rhs_x+token_spacing_x * 4, 524}
    //lower right quadrant

    if (b_pos >= 1 && b_pos <= 6) {
        return {board_rhs_x_ + token_spacing_x_ * (6 - b_pos), board_bot_y_};
    } else if (b_pos >= 7 && b_pos <= 12) {
        return {board_start_x_ + token_spacing_x_ * (12 - b_pos), board_bot_y_};
    } else if (b_pos >= 13 && b_pos <= 18) {
        //position 13: {board_start_x, board_top_y}
        return {board_start_x_ + token_spacing_x_ * (b_pos - 13), board_top_y_};
    } else if (b_pos >= 19 && b_pos <= 24) {
        //position 13: {board_start_x, board_top_y}
        return {board_rhs_x_ + token_spacing_x_ * (b_pos - 19), board_top_y_};
    } else if (b_pos == -1) {
        //jail case
        return {jail_x_, jail_y_};
    } else if (b_pos == 0) {
        return {endzone_x_, d_end_y_};
    } else if (b_pos == 25) {
        return {endzone_x_, l_end_y_};
    }

    return {-2, -2};
}

int View::screen_to_board(ge211::Position s_pos) const
{
    //if the mouse isn't clicking a valid position, return -2 (not a valid
    // position)
    //empty positions have {-2, -2} stored in them
    //currently both endzones are left empty (will be fixed)
    int result = -2;

    //need to have access to screen position of the outermost piece
    for (int i = 1; i <= 24; ++i) {
        ge211::Position piece_pos = outermost_pieces_[i];
        //checks if screen position is within the bounds of the token sprite
        if (s_pos.x >= piece_pos.x && s_pos.y >= piece_pos.y && s_pos.x <=
        piece_pos.x + token_diameter_ && s_pos.y <= piece_pos.y +
        token_diameter_) {
            return i;
        }
    }

    //we want the entire endzone to be clickable
    //light endzone (upper right)
    if (s_pos.x >= endzone_x_ && s_pos.y <= l_end_y_ && s_pos.x <= endzone_x_ +
    endzone_width_ && s_pos.y >= l_end_y_ - 14 * endzone_spacing_) {
        return 25;
    }
    //dark endzone
    if (s_pos.x >= endzone_x_ && s_pos.y >= d_end_y_ && s_pos.x <= endzone_x_ +
    endzone_width_ && s_pos.y <= d_end_y_ + 15 * endzone_spacing_) {
        return 0;
    }

    //jail case
    //todo not sure how to do this yet
    ge211::Position d_jail = outermost_pieces_[26];
    ge211::Position l_jail = outermost_pieces_[27];
    if (s_pos.x >= l_jail.x && s_pos.y >= l_jail.y && s_pos.x <= l_jail.x +
    token_diameter_ && s_pos.y <= l_jail.y +token_diameter_) {
        return -1;
    }
    if (s_pos.x >= d_jail.x && s_pos.y >= d_jail.y && s_pos.x <= d_jail.x +
    token_diameter_ && s_pos.y <= d_jail.y +token_diameter_) {
        return -1;
    }

    return result;
}

void View::render_dice(ge211::Sprite_set& set, int dice_num)
{
    ge211::Position dice_pos = {-1, -1};
    int actual_dice_num = 0;

    if (dice_num == 1) {
        actual_dice_num = model_.dice().num_1();
        dice_pos = dice_1_pos_;
    } else if (dice_num == 2) {
        actual_dice_num = model_.dice().num_2();
        dice_pos = dice_2_pos_;
    } else
        return;

    //need to scale them down cause they're too large (112x112)
    ge211::Transform dice_transform = ge211::Transform{}.set_scale(dice_scale_);
    if (actual_dice_num == 1)
        set.add_sprite(dice_1_sprite_, dice_pos, 1, dice_transform);
    else if (actual_dice_num == 2)
        set.add_sprite(dice_2_sprite_, dice_pos, 1, dice_transform);
    else if (actual_dice_num == 3)
        set.add_sprite(dice_3_sprite_, dice_pos, 1, dice_transform);
    else if (actual_dice_num == 4)
        set.add_sprite(dice_4_sprite_, dice_pos, 1, dice_transform);
    else if (actual_dice_num == 5)
        set.add_sprite(dice_5_sprite_, dice_pos, 1, dice_transform);
    else if (actual_dice_num == 6)
        set.add_sprite(dice_6_sprite_, dice_pos, 1, dice_transform);
}