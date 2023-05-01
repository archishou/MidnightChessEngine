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
Thanks to [@Alex2262](https://github.com/Alex2262) for helping with lots of small improvements in my engine.

Thanks to [@Ciekce](https://github.com/Ciekce) for helping me become a better C++ dev. He helped me find code that produced undefined behavior and is responsible for helping me fix my Makefile numerous times.

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
- Midnight v5
  - Search Changes
    - Adds Transposition Table entries for Q search
    - Add soft/hard limits
    - Adds History Gravity
    - Adds incremental selection sort to sort moves
    - Updates LMR with tuning
    - Add Late Move Pruning
  - Move Ordering Changes
    - Updates definition of quiet to include to castling and double pushes
  - Evaluation Changes
    - King Ring Danger Eval
    - Pawn Eval Attack
    - New Score Style with S(x, y)
    - Mobility includes XRAY squares
    - Passed Pawn / Blocked Pawn PSTS
- Midnight v6
  - [New LMR Constants](https://chess.swehosting.se/test/8/)
  - [New LMR Constants](https://chess.swehosting.se/test/13/)
  - [Tempo Bonus](https://chess.swehosting.se/test/33/)
  - [Aspiration Windows](https://chess.swehosting.se/test/54/)
  - [Threat Eval](https://chess.swehosting.se/test/57/)
  - [Refactor Movegen](https://chess.swehosting.se/test/68/)
  - [Generate Promotions in Q Search](https://chess.swehosting.se/test/73/)
  - [Static Exchange Evaluation in Move Ordering](https://chess.swehosting.se/test/91/)
  - [Static Exchange Pruning in Q Search](https://chess.swehosting.se/test/110/)
  - [Static Exchange Pruning in PVS](https://chess.swehosting.se/test/125/)
  - [Move RFP to before NMP](https://chess.swehosting.se/test/168/)
  - [Only do NMP if our Evaluation is greater than their best value](https://chess.swehosting.se/test/177/)
  - [Prefetch TT Entry](https://chess.swehosting.se/test/190/)
  - [Bonus for controlling board center](https://chess.swehosting.se/test/197/)
  - [Pawn Phalanx Evaluation](https://chess.swehosting.se/test/200/)
  - [Evaluate Candidate Passed Pawns](https://chess.swehosting.se/test/208/)
  - [Add LMP for Quiet Moves](https://chess.swehosting.se/test/227/)
  - [Update History when Alpha is Raised](https://chess.swehosting.se/test/230/)
  - [Add Futility Pruning](https://chess.swehosting.se/test/248/)
  - [Add History Pruning](https://chess.swehosting.se/test/270/)
  - [Add Continuation History](https://chess.swehosting.se/test/309/)
  - [Add Capture History](https://chess.swehosting.se/test/318/)
  - [More Aggressive LMR](https://chess.swehosting.se/test/320/)
  - [Singular Extensions](https://chess.swehosting.se/test/329/)
  - [Multi-cut and negative extensions](https://chess.swehosting.se/test/355/)

[commits-badge]:https://img.shields.io/github/commits-since/archishou/MidnightChessEngine/latest?style=for-the-badge
[commits-link]:https://github.com/archishou/MidnightChessEngine/commits/master
[release-badge]:https://img.shields.io/github/v/release/archishou/MidnightChessEngine?style=for-the-badge&label=official%20release
[release-link]:https://github.com/archishou/MidnightChessEngine/releases/latest
[license-badge]:https://img.shields.io/github/license/archishou/MidnightChessEngine?style=for-the-badge&label=license&color=success
[license-link]:https://github.com/archishou/MidnightChessEngine/blob/master/LICENSE