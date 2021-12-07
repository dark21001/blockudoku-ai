Blockudoku-AI is a powerful engine for **Blockudoku**, a puzzle game by [EasyBrain](https://easybrain.com/) based on Sodoku and Tetris.
- [Blokudoku on Google Play](https://play.google.com/store/apps/details?id=com.easybrain.block.puzzle.games)
- [Blokudoku on Apple Store](https://apps.apple.com/us/app/blockudoku-block-puzzle-game/id1452227871)

`blockudoku-solver.cpp` is a terminal program that shows turns of Blockudoku played by the AI. Example output:
```
===== TURN 5327         ===== TURN 5328         ===== TURN 5329
.........               .........               .........
.#.#.....               ##.#.....               ##.#..#..
...###...               ##.###...               .........
.#.......               ###......               ..##.....
.........               ###......               ..#......
#........               #........               .........
...#.....               ......#..               .##...#..
...##....               .........               .........
...##....               .........               .........

#   ###   #             #    ##   #
##   #  ###             ###  ##   ##              . . .
#    #                            #
```
Each screen shows the board state and the 3 available pieces to place.

## How strong is Blockudoku-AI?

## How does Blockudoku-AI work?
The algorithm can be divided into Evaluation and Search.

### Evaluation
The evaluation function `GameState::simpleEvalImpl` estimates how good a game state is. Better game states have fewer points.

Generally speaking, the evaluation function tries to keep the board clear, set up future clears, and avoid patterns that are difficult to clear.

The entire evaluation function is:
   - 1 point for each occupied square.
   - 1 point for each row/column that contains an occupied square.
   - 3 points for each 3x3 block that contains an occupied square.
   - up to 4 points for each empty square that has occupied squares in 2 adjacent cardinal squares.
   - up to 2 points for each empty square that has no empty squares in 2 opposite cardinal directions.

### Searching
The AI searches for the best next board state that can result from placing the 3 pieces.

The search function `AI::makeMoveLookahead` can be described as follows.
```
best_score = INF
best_board_state = NULL
foreach board_state in possible_next_board_states(current_board_state, piece_1, piece_2, piece_3):
  
  foreach piece_4 in get_all_pieces():
    foreach board_state in possible_next_board_states(current_board_state, piece_4):
      

```

## How is the AI so fast?
