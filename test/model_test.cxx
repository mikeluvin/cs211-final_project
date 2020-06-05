#include "model.hxx"
#include <catch.hxx>
#include <ge211.hxx>
#include <algorithm>

//
// TODO: Write preliminary model tests.
//
// These tests should demonstrate at least six of the functional
// requirements.
//

struct Random_test_access
{
    ge211::Random random;
};
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

//requirement 1
TEST_CASE("Board initialization")
{
    Board board = Board(1);
    //makes sure the board is properly initialized
    CHECK( board.num_pieces(6) == 5 );
    CHECK( board.num_pieces(8) == 3 );
    CHECK( board.num_pieces(13) == 5 );
    CHECK( board.num_pieces(24) == 2 );

    CHECK( board.num_pieces(1) == 2 );
    CHECK( board.num_pieces(12) == 5 );
    CHECK( board.num_pieces(17) == 3 );
    CHECK( board.num_pieces(19) == 5 );

    int num_dark = 0;
    int num_light = 0;
    for (int i = 0; i <= 25; ++i) {
        if (board.player(i) == Player::dark) {
            num_dark += board.num_pieces(i);
        } else if (board.player(i) == Player::light) {
            num_light += board.num_pieces(i);
        }
    }
    num_dark += board.num_jailed(Player::dark);
    num_light += board.num_jailed(Player::light);

    CHECK( num_dark == 15 );
    CHECK( num_light == 15 );
}

//requirement 11
TEST_CASE("Dice roll")
{
    Random_test_access r;
    Dice mydice = Dice(r.random);
    mydice.roll();
    //makes sure dice produce ints between 1 and 6
    CHECK( mydice.num_1() >= 1);
    CHECK( mydice.num_1() <= 6);
    CHECK( mydice.num_2() >= 1);
    CHECK( mydice.num_1() <= 6);

}

//requirements 2 and 4
TEST_CASE("Model::evaluate_position()")
{
    Random_test_access r;
    Model model{r.random, 1, 5, Player::dark};
    Test_access t{model};

    //dark goes CCW, light goes CW

    //making sure dark can only go CCW
    CHECK_FALSE( t.evaluate_position(13, 14 ));
    //dark tries to play where dark already has >1 piece, a valid move
    CHECK( t.evaluate_position(13, 8));
    //tries to play at a position not indicated on the dice
    CHECK_FALSE( t.evaluate_position(6, 4));
    //tries to play where light already has >1 piece, an invalid move
    //(requirement 4)
    CHECK_FALSE( t.evaluate_position(13, 12));

    //light only CW
    t.turn() = Player::light;
    //value not indicated on the dice
    CHECK_FALSE( t.evaluate_position(12, 14));
    //dark >1 piece
    CHECK_FALSE( t.evaluate_position(12, 13));
    //valid move
    CHECK( t.evaluate_position(17, 18));

}

//requirement 5
TEST_CASE("Jail")
{
    Board board = Board(true);

    //player::dark is initially at 12
    //checks that a piece is sent to jail
    board.send_to_jail(13);
    CHECK( board.num_jailed(Player::dark) == 1 );
    CHECK( board.num_pieces(13) == 4 );

    //checks that a piece can be removed from jail
    board.remove_from_jail(13, Player::dark);
    CHECK( board.num_jailed(Player::dark) == 0 );
    CHECK( board.num_pieces(13) == 5 );
}

//requirement 7
TEST_CASE("all pieces in final section")
{
    Random_test_access r;
    Model model = Model(r.random);
    Test_access t{model};

    t.turn() = Player::dark;
    CHECK_FALSE(t.all_in_final());
    t.turn() = Player::light;
    CHECK_FALSE(t.all_in_final());

    //given an initial state where dark is one move away from winning,
    Model model2 = Model(r.random, false);
    Test_access t2{model2};
    t2.turn() = Player::dark;
    CHECK(t2.all_in_final());
    //all of light's pieces are in the final section too
    t2.turn() = Player::light;
    CHECK(t2.all_in_final());
}

//requirement 10
TEST_CASE("advance a turn")
{
    Random_test_access r;
    Model model = Model(r.random, 1, 3, Player::dark);
    Test_access t{model};

    //not a valid move (backwards)
    t.play_move(13, 12);
    CHECK( t.turn() == Player::dark);

    //a valid move
    t.play_move(24, 21);
    //still has one more move
    CHECK( t.turn() == Player::dark);
    t.play_move(21, 20);
    CHECK( t.turn() == Player::light);
}