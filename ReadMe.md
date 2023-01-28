# Elo History
- Midnight Rating: 1356.2 - 1438.6 | 1397.4 (1/27/23)

# File structure

- /ChessEngine contains the C++ chess engine. It produces moves in uci format.
- /ChessEngine/ChessEngine_lib contains the core library including move search and evaluation.
- /ChessEngine/ChessEngine_tests contains the unit tests to verify that function in the lib work as expected. 
- /uci-gui contains a python module that produces a chess gui that a human can play with and interfaces with the chess engine built. Hosted on localhost and built using flask. Most of this is code I took from [here](https://github.com/maksimKorzh/uci-gui). I take no credit for it, just something so that I can play against my engine!

# Todo 

- (HIGH IMPORTANCE) Avoid king shuffling, encourage checkmate without check penalty in eval
- (Medium Importance) extern in types.h needed?
- (Medium Importance) Try to use something other than std::vector or at least pre-define memory requirements so we don't resize array every time.
- History might not be reset correctly in board object
- Implement Kind end game pstq
- Tests for evaluation on more pstq positions
- [Completed!] (HIGH IMPORTANCE) Q search, mis-evaluating many positions.
