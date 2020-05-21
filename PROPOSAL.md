# Proposal: Backgammon

## Synopsis

We hope to make a two-player (no computer player) digital version of backgammon.

### Objective
Each player's objective is to move all of their pieces to their endzone before
the other player.

### Elements

#### Board
The board has 24 total positions (0 through 23 clockwise). Positions 0 through
11 are across from 12 through 23, where 0 is across from 23, 1 across from 22,
etc. Each set of 12 positions is split into two adjacent sections of six. The
board also has two endzones (positioned before 0 and after 23) and a jail (in
the center of the board, but out of play).

#### Pieces
Each player has 15 pieces, which only they can move. Player 1 can move pieces
only clockwise, and player 2 can move pieces only counterclockwise. One piece
can have only one position at a given time, but multiple pieces of the same
player may occupy the same position at any time. Any number of pieces of one
player may occupy that player's endzone, and any number of pieces from both
players may occupy the jail. Each position on the board may be occupied by
pieces from one color, the other, or neither, but never both.

#### Dice
As in the real game, each turn players will roll two six-sided dice, the sum of
which is how many spaces they must move their pieces. If the dice show the same
number, the player must move their pieces the number of spaces given by two
times the sum of the dice.

### Gameplay

#### Setup
Player 1 initially has five pieces at position 5, three pieces at position 7,
five pieces at position 12, and two pieces at position 23. Player 2 initially
has five pieces at position 18, three pieces at position 16, five pieces at
position 11, and two pieces at position 0. At the start of the game, each player
rolls one die. The player with the higher roll goes first, using the numbers
shown by the dice to move their pieces. If the players' rolls are equal, they
roll again.

#### Normal play
Players alternate turns. At each turn, the player must move their pieces a total
of the number they roll, where each move is a number shown by a die. For
example, if a player rolls three and four, they must move either one piece three
positions and another piece four positions, or one piece seven positions. It is
important to not that, for the example described, the player must be able to 
occupy a space at three and/or four positions away in order to be able to move
the seven spaces. In other words, each die's move is considered separately. All
moves are in the same direction (clockwise for player 1, counterclockwise for
player 2). A player's piece can land on a position occupied by their pieces, no
pieces, or only one of the other player's pieces, in which case that player's
piece is placed in jail. If a player cannot move all pieces the number shown by
the dice, the uncompletable moves are skipped.

#### Jail
On a player's turn, if they have any pieces in jail, they cannot move non-jailed
pieces until all pieces are out of jail. When they roll, they can move jailed
pieces into spots that are both available, shown by the dice, and in the first
section of that player's path.

#### Endzone
Once a player has all pieces in the final section of their path, they can start
to move pieces into their endzone, with some restrictions. For example, if
player 1 has a piece in position 18 and a piece in position 20, and they must
move a piece five positions, they must move the piece from position 18 to
position 23. Another case is when a player rolls a number that is greater than
their position away from the endzone. For example, if you have pieces in 
positions 20 and 22, and you roll a 5 and 6, then you must move all pieces from
position 22 before moving any from 20. Also, if a player's piece is put in jail,
they must get that piece back to the final section before continuing to move 
pieces to the endzone.

## Functional requirements

1. All 15 of each player's pieces are on the board, and the game tracks each of
their positions (on board, in jail, or in endzone).
2. The players can move their pieces in only one direction.
3. The players can move their pieces only the distances shown on the dice.
4. The players cannot place their pieces where the other player already has more
than one piece.
5. The pieces are appropriately removed from the board and put in jail.
6. The players select both the piece to move and the position to move it to 
using mouse clicks.
7. The game tracks when all of each player's pieces are in their final section.
8. The pieces are appropriately moved from the board into the correct endzone.
9. The game recognizes when no move can be made.
10. The game alternates turns between players.
11. The dice randomly select two numbers from 1 to 6.
12. The game recognizes when a player has won, and will indicate the winner on 
the screen.

## Open questions

How can we represent piece and position selection (i.e. highlight
piece/position, select piece only from top of stack, select position only at
edge of board, etc.)?\
In one turn, a player may use the same mouse to select two pieces and two
positions, in the order piece, position, piece, position. How can we track what
the mouse is currently being used for?\
In normal backgammon, the dice double on a double roll. Would this be too much
functionality to add?\
How do we show timed (~2 seconds) error messages on the screen (i.e. "Player 1
has no move", "Game over: Player 2 wins", "You cannot move there - try again")?
