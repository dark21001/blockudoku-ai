Blockudoku-AI is a powerful engine for **Blockudoku**, a puzzle game by [EasyBrain](https://easybrain.com/) based on Sodoku and Tetris.
- [Blokudoku on Google Play](https://play.google.com/store/apps/details?id=com.easybrain.block.puzzle.games)
- [Blokudoku on Apple Store](https://apps.apple.com/us/app/blockudoku-block-puzzle-game/id1452227871)

`blockudoku-solver.cpp` is a terminal program that shows turns game of Blockudoku played by the AI. Example output:
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
Each turn is a board state + the set of 3 pieces available.

## What is the AI's algorithm?


## How is the AI so fast?
