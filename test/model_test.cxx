#include "model.hxx"
#include <catch.hxx>
#include <ge211.hxx>

//
// TODO: Write preliminary model tests.
//
// These tests should demonstrate at least six of the functional
// requirements.
//
struct Test_access
{
    Model& model_;

    Board& board()
    {
        return model_.board_;
    }

    Player& turn()
    {
        return model_.turn_;
    }

    Dice& dice()
    {
        return model_.dice_;
    }

    bool evaluate_position(int pos_from, int pos_to)
    {
        return model_.evaluate_position_(pos_from, pos_to);
    }

    bool all_in_final()
    {
        return model_.all_in_final_();
    }

    void play_move(int pos_from, int pos_to) {
        model_.play_move(pos_from, pos_to);
    }

};
TEST_CASE("Board initialization")
{
    Board board = Board(true);
    //makes sure the board is properly initialized
    CHECK( board.num_pieces(5) == 5 );
    CHECK( board.num_pieces(7) == 3 );
    CHECK( board.num_pieces(12) == 5 );
    CHECK( board.num_pieces(23) == 2 );
}

//requirement 11
TEST_CASE("Dice roll")
{
    Dice mydice = Dice(ge211::Abstract_game::get_random());
    mydice.roll();
    //makes sure dice produce ints between 1 and 6
    CHECK( mydice.num_1() >= 1 && mydice.num_1() <= 6 );
    CHECK( mydice.num_2() >= 1 && mydice.num_2() <= 6 );
}

//requirements 2 and 4
TEST_CASE("Model::evaluate_position()")
{
    Model model{};
    Test_access t{model};

    //for now just assuming dark goes clockwise, light goes counterclockwise
    t.turn() = Player::dark;

    //making sure dark can only go CW
    CHECK_FALSE( t.evaluate_position(12, 10 ) );
    CHECK( t.evaluate_position(12, 13) );

    //light only CCW
    t.turn() = Player::light;
    CHECK_FALSE( t.evaluate_position(11, 13) );
    CHECK( t.evaluate_position(11, 9) );

    //checks to make sure you can't move where the other player has more than
    // one piece (requirement 4)
    CHECK_FALSE(t.evaluate_position(18, 12));
}

//requirement 5
TEST_CASE("Jail")
{
    Board board = Board(true);

    //player::dark is initially at 12
    //checks that a piece is sent to jail
    board.send_to_jail(12);
    CHECK( board.num_jailed(Player::dark) == 1 );
    CHECK( board.num_pieces(12) == 4 );

    //checks that a piece can be removed from jail
    board.remove_from_jail(12, Player::dark);
    CHECK( board.num_jailed(Player::dark) == 0 );
    CHECK( board.num_pieces(12) == 5 );
}

//requirement 7
TEST_CASE("all pieces in final section")
{
    Model model = Model();
    Test_access t{model};

    CHECK_FALSE(t.all_in_final());
}

//requirement 10
TEST_CASE("advance a turn")
{
    Model model = Model();
    Test_access t{model};

    t.turn() = Player::dark;
    t.play_move(13);
    CHECK( t.turn() == Player::light);
}