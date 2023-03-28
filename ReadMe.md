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

Credits
Shoutout to @Antares for donating his big brain and helping with a lot of ideas on this engine.