cmake_minimum_required(VERSION 3.3)
project(backgammon CXX)
include(.cs211/cmake/CMakeLists.txt)

set(MODEL_SRC
        src/player.cxx
        src/dice.cxx
        src/board.cxx
        src/model.cxx)

add_program(main
    src/main.cxx
    src/controller.cxx
    src/view.cxx
    ${MODEL_SRC})
target_link_libraries(main ge211)

add_test_program(model_test
    test/model_test.cxx
    ${MODEL_SRC})
target_link_libraries(model_test ge211)

# vim: ft=cmake
