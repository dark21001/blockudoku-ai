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
.........       ->      ###......       ->      ..#......
#........               #........               .........
...#.....               ......#..               .##...#..
...##....               .........               .........
...##....               .........               .........

#   ###   #             #    ##   #
##   #  ###             ###  ##   ##
#    #                            #
```
Each screen shows the board state and the 3 available pieces to place.

## How strong is Blockudoku-AI?
The AI reaches turn 5000 (about 200,000 points) more than half the games it plays. It takes a string of bad luck for the game to end.

## How does Blockudoku-AI work?
The algorithm can be divided into evaluation and search. 

The board state and pieces are represented as [BitBoards](https://en.wikipedia.org/wiki/Bitboard) to make evaluation and search fast. 
- The evaluation function is about 300 assembly instructions when compiled with `gcc -O3`.
- 

### Evaluation
The evaluation function `GameState::simpleEvalImpl` estimates how good a game state is. Better game states have fewer points.

Generally speaking, the evaluation function tries to keep the board clear, set up future clears, and avoid patterns that are difficult to clear.

The evaluation function gives points for:
   - each occupied square
   - each row/column that contains an occupied square
   - each 3x3 block that contains an occupied square
   - each empty square that has 2 or more occupied cardinally adjacent squares

### Searching
`AI::makeMoveSimple` picks the best next board state that can result from placing the 3 pieces. The best state is determined using the evaluation function. This search method evaluates considers up to about 100,000 positions per turn.

`AI::makeMoveLookahead`, the code actually used, goes one level deeper by considering the placement of a hypothetical 4th. The best state is determined by the average score after placing the 4th piece.

## Why did I build this?
A certain mermaid was better at Blockudoku than me. And cause my phone refused to let me play the game anymore. I hate this game :P !
