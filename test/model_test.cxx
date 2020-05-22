#include "model.hxx"
#include <catch.hxx>

//
// TODO: Write preliminary model tests.
//
// These tests should demonstrate at least six of the functional
// requirements.
//

TEST_CASE("Board initialization")
{
    Board board = Board(true);
    CHECK( board.num_pieces(5) == 5 );
    CHECK( board.num_pieces(7) == 3 );
    CHECK( board.num_pieces(12) == 5 );
    CHECK( board.num_pieces(23) == 2 );
}

TEST_CASE("Dice roll")
{
    Dice dice = Dice();
    myDice.roll();
    CHECK( dice.num_1() >= 1 && dice.num_1() <= 6 );
    CHECK( dice.num_2() >= 1 && dice.num_2() <= 6 );
}

TEST_CASE("Model::evaluate_position()")
{
    Model model = Model();
    //for now just assuming dark goes clockwise, light goes counterclockwise
    CHECK_FALSE( model.evaluate_position(12, dark) );
    CHECK( model.evaluate_position(12, light) );
    CHECK_FALSE( model.evaluate_position(11, light) );
    CHECK( model.evaluate_position(11, dark) );
}

TEST_CASE("Jail")
{
    Board board = Board(true);
    board.send_to_jail(12);
    CHECK( board.num_jailed() == 1 );
    CHECK( board.num_pieces(12) == 4 );
    board.remove_from_jail(12, light);
    CHECK( board.num_jailed() == 0 );
    CHECK( board.num_pieces(12) == 5 );
}