<div align="center">

# Midnight
[![License][license-badge]][license-link]
[![Release][release-badge]][release-link]
[![Commits][commits-badge]][commits-link]

</div>
A free and open source UCI chess engine written in C++.

# Strength

| Version      | CCRL 40/15 | CCRL Blitz         | MCERL        |
|--------------|------------|--------------------| ------------ |
| Midnight 5.0 |            | [2828±15 [#146]](http://ccrl.chessdom.com/ccrl/404/cgi/engine_details.cgi?print=Details&each_game=1&eng=Midnight%205%2064-bit#Midnight_5_64-bit) |              |

# Compilation
```
git clone https://github.com/archishou/MidnightChessEngine
cd MidnightChessEngine
make
```

# Credits
Thanks to [@Alex2262](https://github.com/Alex2262) for helping me debug a lot of the small bugs I had when I first started and then giving me validating ideas I had as my engine grew in strength.

Thanks to [@Ciekce](https://github.com/Ciekce) for helping me become a better C++ dev. He helped identify many of the places of undefined behavior in my code and generally pointed out many of the bad coding practices prevalent in my code.

# Features
- Search
  - Principle Variation Search
  - Alpha Beta Pruning
  - Transposition Table
  - Null Move Pruning
  - Reverse Futility Pruning
  - Late Move Reductions
  - Late Move Pruning
- Move Ordering
  - Killer Moves
  - History indexed by color-to-from
- Evaluation
  - Piece Material
  - Piece Square Tables
  - Tempo bonus
  - Bishop Pair Bonus
  - Piece Mobility
  - Piece supported by pawn
  - Piece attacked by pawn
  - Rook/Queen/King on open/semi open files
  - King pawn shield
  - Threats from minor to minor/rook/queen
  - Threats from rook to queen
  - King Ring attack Bonus
  - King check bonus
  - Doubled pawns
  - Isolated pawns
  - Passed pawns
  - Blocked passed pawns
  - Pawns supported by pawns

# Elo History
- Midnight Rating 1500
  - AlphaBeta pruning
  - Move ordering: MVV-LVA, opponent pawn control, promotions
  - Evaluation: Material, PSTQ, Mopup for endgame
  - Transposition table
- Midnight v2 Rating 1935
  - Adds History Heuristic and Killer Moves
  - Reset Transposition Table only at game start
  - Adds Null Move Pruning
  - Switches from Negamax to Principle Variation Search
- Midnight v3 Rating 2500 
  - Search Changes
    - Adds a first draft of Reverse Futility Pruning
    - Adds a first draft of Late Move Reductions
    - Adds Null Move Pruning Reductions based on Static Evaluation
    - Adds use of the Transposition Table in QSearch
  - Move Ordering Changes
    - Fixes History Heuristic [Previously not a global variable]
  - Evaluation Changes
    - Adds tuned PSQT for all pieces based on Texel tuning
    - Removes Mop Up Evaluation
    - Bonus for Passed Pawns
    - Isolated Pawn Penalty
    - Paired Bishop Bonus
    - Mobility Bonus for all pieces
    - Rook/Queen/King Bonuses and Penalties for Open/Semi Open file
    - Pieces protected by pawns (including pawns) receive a bonus
    - King Shield Pawn bonus
- Midnight v4 Rating 2500
  - No functional changes just adds better tech stack.

[commits-badge]:https://img.shields.io/github/commits-since/archishou/MidnightChessEngine/latest?style=for-the-badge
[commits-link]:https://github.com/archishou/MidnightChessEngine/commits/master
[release-badge]:https://img.shields.io/github/v/release/archishou/MidnightChessEngine?style=for-the-badge&label=official%20release
[release-link]:https://github.com/archishou/MidnightChessEngine/releases/latest
[license-badge]:https://img.shields.io/github/license/archishou/MidnightChessEngine?style=for-the-badge&label=license&color=success
[license-link]:https://github.com/archishou/MidnightChessEngine/blob/master/LICENSE