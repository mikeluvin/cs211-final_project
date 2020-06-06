#pragma once

#include "model.hxx"
#include "view.hxx"


class Controller : public ge211::Abstract_game
{
public:
    //takes the board initializer value.
    // board_state = 1 is the normal game
    // board_state = 0 is with dark being one piece away from winning
    // board_state = 2 is to show the skipped turn message (when a player has
    // no moves).
    explicit Controller(int board_state);

protected:

    void on_mouse_up(ge211::Mouse_button, ge211::Position) override;

    //delegate to the view
    void draw(ge211::Sprite_set&) override;
    ge211::Dimensions initial_window_dimensions() const override;
    std::string initial_window_title() const override;

private:
    Model    model_;
    View     view_;

    //holds the position of the piece that is clicked and the position to
    // which that piece is trying to be moved
    int from_pos_ = -2;
    int to_pos_ = -2;
};