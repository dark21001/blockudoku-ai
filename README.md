**[See online demo](https://gary-z.github.io/blokie/)**.

Blockudoku-AI is a powerful engine for **Blockudoku**, a puzzle game by [EasyBrain](https://easybrain.com/) based on Sodoku and Tetris.
- [Blokudoku on Google Play](https://play.google.com/store/apps/details?id=com.easybrain.block.puzzle.games)
- [Blokudoku on Apple Store](https://apps.apple.com/us/app/blockudoku-block-puzzle-game/id1452227871)

`blockudoku-solver.cpp` is a terminal program that shows turns of Blockudoku played by the AI.
<p><img style="width: 25%; height: 15%" src="/demo.gif?raw=true"/></p>

Each screen shows the board state and the 3 available pieces to place.

## How strong/fast is the engine?
The engine reaches turn 5000 (about equivalent to 200,000 points) more than half the games it plays. The engine takes about 1 second per move when compiled with `gcc -O3` in 2021.

## How does the engine work?
The algorithm can be divided into evaluation and search. 

The board state and pieces are represented as [BitBoards](https://en.wikipedia.org/wiki/Bitboard) to make evaluation and search fast. For instance, the evaluation function is about 300 assembly instructions when optimized.

### Evaluation
The evaluation function `GameState::simpleEvalImpl` estimates how good a game state is. Generally speaking, the evaluation function tries to keep the board clear, set up future clears, and avoid patterns that are difficult to clear.

Better game states have fewer points. The evaluation function gives points for:
   - each occupied square
   - each row/column that contains an occupied square
   - each 3x3 block that contains an occupied square
   - each empty square that has 2 or more occupied cardinally adjacent squares

### Searching
`AI::makeMoveSimple` picks the best next board state that can result from placing the 3 pieces. The best state is determined using the evaluation function. This search method evaluates about 100,000 positions per turn.

`AI::makeMoveLookahead`, the code actually used, goes one level deeper by considering the placement of a hypothetical 4th. The best state is determined by the average score after placing the 4th piece. This search method typically evaluates about 100,000,000 positions per turn.

## Why did I build this?
Because a certain mermaid is better at Blockudoku than me. And cause my phone refused to let me play the game anymore. I hate this game <3 
