# Elo History
- Midnight Rating 1500 | 1479/1517: 
  - AlphaBeta pruning
  - Move ordering: MVV-LVA, opponent pawn control, promotions
  - Evaluation: Material, PSTQ, Mopup for endgame
  - Transposition table
- Midnight v2 Rating 1935 |
  - Adds History Heuristic and Killer Moves
  - Reset Transposition Table only at game start
  - Adds Null Move Pruning
  - Switches from Negamax to Principle Variation Search

# File structure

- /ChessEngine contains the C++ chess engine. It produces moves in uci format.
- /ChessEngine/ChessEngine_lib contains the core library including move search and evaluation.
- /ChessEngine/ChessEngine_tests contains the unit tests to verify that function in the lib work as expected. 
- /uci-gui contains a python module that produces a chess gui that a human can play with and interfaces with the chess engine built. Hosted on localhost and built using flask. Most of this is code I took from [here](https://github.com/maksimKorzh/uci-gui). I take no credit for it, just something so that I can play against my engine!

# Todo 

- (Medium Importance) extern in types.h needed?
- (Medium Importance) Try to use something other than std::vector or at least pre-define memory requirements so we don't resize array every time.
- Implement Kind end game pstq
- Fixup History Table, not a global variable [most likely a bug somewhere here]
