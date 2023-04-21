# Changelog 

### Add History Heuristic
```
Score of midnight-dev vs midnight-latest: 449 - 276 - 275 [0.587]
...      midnight-dev playing White: 219 - 155 - 126  [0.564] 500
...      midnight-dev playing Black: 230 - 121 - 149  [0.609] 500
...      White vs Black: 340 - 385 - 275  [0.477] 1000
Elo difference: 60.7 +/- 18.5, LOS: 100.0 %, DrawRatio: 27.5 %
1000 of 1000 games finished.
```

### History with tighter bounds
```
Score of midnight-dev vs midnight-history: 382 - 320 - 298 [0.531]
...      midnight-dev playing White: 196 - 159 - 145  [0.537] 500
...      midnight-dev playing Black: 186 - 161 - 153  [0.525] 500
...      White vs Black: 357 - 345 - 298  [0.506] 1000
Elo difference: 21.6 +/- 18.1, LOS: 99.0 %, DrawRatio: 29.8 %
1000 of 1000 games finished.
```

### Adds killer heuristic
```
Score of midnight-dev vs vhistory-bounds: 372 - 322 - 306 [0.525]
...      midnight-dev playing White: 162 - 180 - 158  [0.482] 500
...      midnight-dev playing Black: 210 - 142 - 148  [0.568] 500
...      White vs Black: 304 - 390 - 306  [0.457] 1000
Elo difference: 17.4 +/- 17.9, LOS: 97.1 %, DrawRatio: 30.6 %
1000 of 1000 games finished.
```

### Adds NMP
```
Score of midnight-dev vs midnight-latest: 382 - 304 - 314 [0.539]
...      midnight-dev playing White: 176 - 161 - 163  [0.515] 500
...      midnight-dev playing Black: 206 - 143 - 151  [0.563] 500
...      White vs Black: 319 - 367 - 314  [0.476] 1000
Elo difference: 27.2 +/- 17.9, LOS: 99.9 %, DrawRatio: 31.4 %
1000 of 1000 games finished.
```

### NMP Fixup
```
Score of midnight-dev vs midnight-v1.5: 435 - 293 - 272 [0.571]
...      midnight-dev playing White: 273 - 96 - 131  [0.677] 500
...      midnight-dev playing Black: 162 - 197 - 141  [0.465] 500
...      White vs Black: 470 - 258 - 272  [0.606] 1000
Elo difference: 49.7 +/- 18.5, LOS: 100.0 %, DrawRatio: 27.2 %
1000 of 1000 games finished.
```

### Adaptive NMP
```
Score of midnight-dev vs midnight-latest: 384 - 342 - 274 [0.521]
...      midnight-dev playing White: 194 - 164 - 142  [0.530] 500
...      midnight-dev playing Black: 190 - 178 - 132  [0.512] 500
...      White vs Black: 372 - 354 - 274  [0.509] 1000
Elo difference: 14.6 +/- 18.3, LOS: 94.0 %, DrawRatio: 27.4 %
1000 of 1000 games finished.
```

### Adds pv_node check for nmp and ttable checks
```
Score of midnight-dev vs midnight-latest: 400 - 333 - 267 [0.533]
...      midnight-dev playing White: 176 - 187 - 137  [0.489] 500
...      midnight-dev playing Black: 224 - 146 - 130  [0.578] 500
...      White vs Black: 322 - 411 - 267  [0.456] 1000
Elo difference: 23.3 +/- 18.5, LOS: 99.3 %, DrawRatio: 26.7 %
1000 of 1000 games finished.
```

### Don't reset TT Every Move
```
Score of midnight-dev vs vtt-reset-every-time: 421 - 309 - 270 [0.556]
...      midnight-dev playing White: 262 - 107 - 131  [0.655] 500
...      midnight-dev playing Black: 159 - 202 - 139  [0.457] 500
...      White vs Black: 464 - 266 - 270  [0.599] 1000
Elo difference: 39.1 +/- 18.5, LOS: 100.0 %, DrawRatio: 27.0 %
1000 of 1000 games finished.
```

### Net Gains of V2
```
Score of midnight-dev vs Lynx: 926 - 26 - 48 [0.950]
...      midnight-dev playing White: 485 - 0 - 15  [0.985] 500
...      midnight-dev playing Black: 441 - 26 - 33  [0.915] 500
...      White vs Black: 511 - 441 - 48  [0.535] 1000
Elo difference: 511.5 +/- 43.4, LOS: 100.0 %, DrawRatio: 4.8 %
1000 of 1000 games finished.
Lynx ELO: 1558
Midnight ELO: 2069.5

Score of midnight-dev vs snowy0.2: 321 - 506 - 173 [0.407]
...      midnight-dev playing White: 170 - 227 - 103  [0.443] 500
...      midnight-dev playing Black: 151 - 279 - 70  [0.372] 500
...      White vs Black: 449 - 378 - 173  [0.535] 1000
Elo difference: -65.0 +/- 19.8, LOS: 0.0 %, DrawRatio: 17.3 %
1000 of 1000 games finished.
Snowy ELO 2000
Midnight ELO 1935
```


### PESTO PSTs
```
Score of midnight-dev vs midnight-latest: 108 - 18 - 11  [0.828] 137
...      midnight-dev playing White: 57 - 9 - 3  [0.848] 69
...      midnight-dev playing Black: 51 - 9 - 8  [0.809] 68
...      White vs Black: 66 - 60 - 11  [0.522] 137
Elo difference: 273.6 +/- 73.9, LOS: 100.0 %, DrawRatio: 8.0 %
SPRT: llr 2.96 (100.5%), lbound -2.94, ubound 2.94 - H1 was accepted
```

### Remove Mop Up [Thank god!]
```
Score of midnight-dev vs midnight-latest: 136 - 126 - 135 [0.513]
...      midnight-dev playing White: 74 - 58 - 66  [0.540] 198
...      midnight-dev playing Black: 62 - 68 - 69  [0.485] 199
...      White vs Black: 142 - 120 - 135  [0.528] 397
Elo difference: 8.8 +/- 27.8, LOS: 73.2 %, DrawRatio: 34.0 %
402 of 1000 games finished.
```

### Rank * 10 Bonus for Passed Pawns
```
Score of midnight-dev vs midnight-latest: 417 - 340 - 243 [0.538]
...      midnight-dev playing White: 193 - 194 - 113  [0.499] 500
...      midnight-dev playing Black: 224 - 146 - 130  [0.578] 500
...      White vs Black: 339 - 418 - 243  [0.461] 1000
Elo difference: 26.8 +/- 18.8, LOS: 99.7 %, DrawRatio: 24.3 %
1000 of 1000 games finished.
```

### Isolated Pawn Penalty
```
Score of midnight-dev vs midnight-latest: 1006 - 893 - 678  [0.522] 2577
...      midnight-dev playing White: 535 - 412 - 342  [0.548] 1289
...      midnight-dev playing Black: 471 - 481 - 336  [0.496] 1288
...      White vs Black: 1016 - 883 - 678  [0.526] 2577
Elo difference: 15.2 +/- 11.5, LOS: 99.5 %, DrawRatio: 26.3 %
SPRT: llr 2.97 (100.8%), lbound -2.94, ubound 2.94 - H1 was accepted
```

### Bishop Bonus
```
Score of midnight-dev vs midnight-latest: 4697 - 4442 - 3307  [0.510] 12446
...      midnight-dev playing White: 2425 - 2160 - 1638  [0.521] 6223
...      midnight-dev playing Black: 2272 - 2282 - 1669  [0.499] 6223
...      White vs Black: 4707 - 4432 - 3307  [0.511] 12446
Elo difference: 7.1 +/- 5.2, LOS: 99.6 %, DrawRatio: 26.6 %
SPRT: llr 2.98 (101.1%), lbound -2.94, ubound 2.94 - H1 was accepted
```

### Tuned PSTs and Other penalties etc.
```
Score of midnight-dev vs midnight-pesto: 875 - 677 - 448 [0.549]
...      midnight-dev playing White: 453 - 322 - 225  [0.566] 1000
...      midnight-dev playing Black: 422 - 355 - 223  [0.533] 1000
...      White vs Black: 808 - 744 - 448  [0.516] 2000
Elo difference: 34.5 +/- 13.4, LOS: 100.0 %, DrawRatio: 22.4 %

```
### Mobility Bonus
```
Score of midnight-dev vs midnight-latest: 399 - 308 - 234  [0.548] 941
...      midnight-dev playing White: 200 - 155 - 116  [0.548] 471
...      midnight-dev playing Black: 199 - 153 - 118  [0.549] 470
...      White vs Black: 353 - 354 - 234  [0.499] 941
Elo difference: 33.7 +/- 19.3, LOS: 100.0 %, DrawRatio: 24.9 %
SPRT: llr 2.98 (101.1%), lbound -2.94, ubound 2.94 - H1 was accepted
```

### Blocked Passed Pawn Eval
```
Score of midnight-dev vs midnight-latest: 773 - 670 - 541  [0.526] 1984
...      midnight-dev playing White: 400 - 315 - 278  [0.543] 993
...      midnight-dev playing Black: 373 - 355 - 263  [0.509] 991
...      White vs Black: 755 - 688 - 541  [0.517] 1984
Elo difference: 18.1 +/- 13.0, LOS: 99.7 %, DrawRatio: 27.3 %
SPRT: llr 2.95 (100.2%), lbound -2.94, ubound 2.94 - H1 was accepted
```

### Rook on open file
```
Score of midnight-dev vs midnight-latest: 613 - 516 - 429  [0.531] 1558
...      midnight-dev playing White: 304 - 266 - 210  [0.524] 780
...      midnight-dev playing Black: 309 - 250 - 219  [0.538] 778
...      White vs Black: 554 - 575 - 429  [0.493] 1558
Elo difference: 21.7 +/- 14.7, LOS: 99.8 %, DrawRatio: 27.5 %
SPRT: llr 2.97 (100.8%), lbound -2.94, ubound 2.94 - H1 was accepted
```

### Queen and king on open/semi-open files
```
Score of midnight-dev vs midnight-latest: 2246 - 2085 - 1708  [0.513] 6039
...      midnight-dev playing White: 1183 - 1019 - 818  [0.527] 3020
...      midnight-dev playing Black: 1063 - 1066 - 890  [0.500] 3019
...      White vs Black: 2249 - 2082 - 1708  [0.514] 6039
Elo difference: 9.3 +/- 7.4, LOS: 99.3 %, DrawRatio: 28.3 %
SPRT: llr 2.98 (101.0%), lbound -2.94, ubound 2.94 - H1 was accepted
```

### Protected Pawns
```
Score of midnight-dev vs midnight-latest: 514 - 423 - 398  [0.534] 1335
...      midnight-dev playing White: 266 - 197 - 204  [0.552] 667
...      midnight-dev playing Black: 248 - 226 - 194  [0.516] 668
...      White vs Black: 492 - 445 - 398  [0.518] 1335
Elo difference: 23.7 +/- 15.6, LOS: 99.9 %, DrawRatio: 29.8 %
SPRT: llr 2.95 (100.2%), lbound -2.94, ubound 2.94 - H1 was accepted
```

### King Shield Pawns
```
Score of midnight-dev vs midnight-latest: 1163 - 1044 - 767  [0.520] 2974
...      midnight-dev playing White: 582 - 521 - 385  [0.520] 1488
...      midnight-dev playing Black: 581 - 523 - 382  [0.520] 1486
...      White vs Black: 1105 - 1102 - 767  [0.501] 2974
Elo difference: 13.9 +/- 10.8, LOS: 99.4 %, DrawRatio: 25.8 %
SPRT: llr 2.96 (100.5%), lbound -2.94, ubound 2.94 - H1 was accepted
```

### RFP MaxDepth 4 MarginPerDepth = 200
```
Score of midnight-dev vs midnight-latest: 238 - 156 - 149  [0.576] 543
...      midnight-dev playing White: 118 - 79 - 74  [0.572] 271
...      midnight-dev playing Black: 120 - 77 - 75  [0.579] 272
...      White vs Black: 195 - 199 - 149  [0.496] 543
Elo difference: 52.9 +/- 25.1, LOS: 100.0 %, DrawRatio: 27.4 %
SPRT: llr 2.97 (101.0%), lbound -2.94, ubound 2.94 - H1 was accepted
```

### RFP MD 9 MPR 75
```
Score of midnight-dev vs midnight-latest: 520 - 429 - 409  [0.534] 1358
...      midnight-dev playing White: 266 - 201 - 212  [0.548] 679
...      midnight-dev playing Black: 254 - 228 - 197  [0.519] 679
...      White vs Black: 494 - 455 - 409  [0.514] 1358
Elo difference: 23.3 +/- 15.5, LOS: 99.8 %, DrawRatio: 30.1 %
SPRT: llr 2.95 (100.2%), lbound -2.94, ubound 2.94 - H1 was accepted
```

### Eval in NMP Depth Reduction
```
Score of midnight-dev vs midnight-latest: 153 - 82 - 121  [0.600] 356
...      midnight-dev playing White: 89 - 33 - 56  [0.657] 178
...      midnight-dev playing Black: 64 - 49 - 65  [0.542] 178
...      White vs Black: 138 - 97 - 121  [0.558] 356
Elo difference: 70.2 +/- 29.6, LOS: 100.0 %, DrawRatio: 34.0 %
SPRT: llr 2.95 (100.1%), lbound -2.94, ubound 2.94 - H1 was accepted
```

### LMR V1
```
Score of midnight-dev vs midnight-latest: 125 - 54 - 83  [0.635] 262
...      midnight-dev playing White: 64 - 28 - 39  [0.637] 131
...      midnight-dev playing Black: 61 - 26 - 44  [0.634] 131
...      White vs Black: 90 - 89 - 83  [0.502] 262
Elo difference: 96.6 +/- 35.6, LOS: 100.0 %, DrawRatio: 31.7 %
SPRT: llr 2.96 (100.5%), lbound -2.94, ubound 2.94 - H1 was accepted
```

### Transposition Table in QSearch
```
Score of midnight-dev vs midnight-latest: 337 - 265 - 496  [0.533] 1098
...      midnight-dev playing White: 189 - 127 - 233  [0.556] 549
...      midnight-dev playing Black: 148 - 138 - 263  [0.509] 549
...      White vs Black: 327 - 275 - 496  [0.524] 1098
Elo difference: 22.8 +/- 15.2, LOS: 99.8 %, DrawRatio: 45.2 %
SPRT: llr 2.96 (100.7%), lbound -2.94, ubound 2.94 - H1 was accepted
```

### Improve Time Management
```
Score of midnight-dev vs midnight-latest: 148 - 89 - 223  [0.564] 460
...      midnight-dev playing White: 80 - 41 - 109  [0.585] 230
...      midnight-dev playing Black: 68 - 48 - 114  [0.543] 230
...      White vs Black: 128 - 109 - 223  [0.521] 460
Elo difference: 44.8 +/- 22.8, LOS: 100.0 %, DrawRatio: 48.5 %
SPRT: llr 2.99 (101.6%), lbound -2.94, ubound 2.94 - H1 was accepted
```

### Doubled Pawns
```
Score of midnight-dev vs midnight-latest: 2772 - 2589 - 4423  [0.509] 9784
...      midnight-dev playing White: 1453 - 1179 - 2261  [0.528] 4893
...      midnight-dev playing Black: 1319 - 1410 - 2162  [0.491] 4891
...      White vs Black: 2863 - 2498 - 4423  [0.519] 9784
Elo difference: 6.5 +/- 5.1, LOS: 99.4 %, DrawRatio: 45.2 %
SPRT: llr 2.96 (100.5%), lbound -2.94, ubound 2.94 - H1 was accepted
```

### Antares makes my LMR a lot better
```
Score of midnight-dev vs midnight-latest: 93 - 39 - 123  [0.606] 255
...      midnight-dev playing White: 50 - 18 - 60  [0.625] 128
...      midnight-dev playing Black: 43 - 21 - 63  [0.587] 127
...      White vs Black: 71 - 61 - 123  [0.520] 255
Elo difference: 74.7 +/- 30.7, LOS: 100.0 %, DrawRatio: 48.2 %
SPRT: llr 2.98 (101.1%), lbound -2.94, ubound 2.94 - H1 was accepted
```

### Update History Heuristic
```
Score of midnight-dev vs midnight-latest: 639 - 552 - 1044  [0.519] 2235
...      midnight-dev playing White: 352 - 256 - 510  [0.543] 1118
...      midnight-dev playing Black: 287 - 296 - 534  [0.496] 1117
...      White vs Black: 648 - 543 - 1044  [0.523] 2235
Elo difference: 13.5 +/- 10.5, LOS: 99.4 %, DrawRatio: 46.7 %
SPRT: llr 2.97 (100.8%), lbound -2.94, ubound 2.94 - H1 was accepted
```

### Profile Code and added incremental sel sort + slower time checks
```
Score of midnight-dev vs midnight-latest: 134 - 75 - 183  [0.575] 392
...      midnight-dev playing White: 74 - 33 - 89  [0.605] 196
...      midnight-dev playing Black: 60 - 42 - 94  [0.546] 196
...      White vs Black: 116 - 93 - 183  [0.529] 392
Elo difference: 52.7 +/- 25.2, LOS: 100.0 %, DrawRatio: 46.7 %
SPRT: llr 2.96 (100.6%), lbound -2.94, ubound 2.94 - H1 was accepted
```

### Late Move Pruning
```
Score of midnight-dev vs midnight-latest: 309 - 239 - 458  [0.535] 1006
...      midnight-dev playing White: 163 - 104 - 235  [0.559] 502
...      midnight-dev playing Black: 146 - 135 - 223  [0.511] 504
...      White vs Black: 298 - 250 - 458  [0.524] 1006
Elo difference: 24.2 +/- 15.8, LOS: 99.9 %, DrawRatio: 45.5 %
SPRT: llr 2.95 (100.2%), lbound -2.94, ubound 2.94 - H1 was accepted
```

### King Ring Danger Eval
```
Score of midnight-dev vs midnight-latest: 297 - 225 - 385  [0.540] 907
...      midnight-dev playing White: 166 - 112 - 176  [0.559] 454
...      midnight-dev playing Black: 131 - 113 - 209  [0.520] 453
...      White vs Black: 279 - 243 - 385  [0.520] 907
Elo difference: 27.6 +/- 17.2, LOS: 99.9 %, DrawRatio: 42.4 %
SPRT: llr 2.97 (100.7%), lbound -2.94, ubound 2.94 - H1 was accepted
```

### Pawn Attack Eval
```
Score of midnight-dev vs midnight-latest: 686 - 597 - 1159  [0.518] 2442
...      midnight-dev playing White: 388 - 264 - 569  [0.551] 1221
...      midnight-dev playing Black: 298 - 333 - 590  [0.486] 1221
...      White vs Black: 721 - 562 - 1159  [0.533] 2442
Elo difference: 12.7 +/- 10.0, LOS: 99.4 %, DrawRatio: 47.5 %
SPRT: llr 2.96 (100.4%), lbound -2.94, ubound 2.94 - H1 was accepted
```

### Change Score Style ~300KNps Quicker than previous implementation.
```
Score of midnight-dev vs midnight-latest: 258 - 193 - 440  [0.536] 891
...      midnight-dev playing White: 133 - 89 - 223  [0.549] 445
...      midnight-dev playing Black: 125 - 104 - 217  [0.524] 446
...      White vs Black: 237 - 214 - 440  [0.513] 891
Elo difference: 25.4 +/- 16.2, LOS: 99.9 %, DrawRatio: 49.4 %
SPRT: llr 2.99 (101.5%), lbound -2.94, ubound 2.94 - H1 was accepted
```

### New Mobility Style, Kinda hate it tho
```
Score of midnight-dev vs midnight-latest: 883 - 783 - 1334  [0.517] 3000
...      midnight-dev playing White: 458 - 360 - 683  [0.533] 1501
...      midnight-dev playing Black: 425 - 423 - 651  [0.501] 1499
...      White vs Black: 881 - 785 - 1334  [0.516] 3000
Elo difference: 11.6 +/- 9.3, LOS: 99.3 %, DrawRatio: 44.5 %
SPRT: llr 2.95 (100.2%), lbound -2.94, ubound 2.94 - H1 was accepted
```

### Passed Pawn Blocked Pawn PST
```
Score of midnight-dev vs midnight-latest: 479 - 400 - 759  [0.524] 1638
...      midnight-dev playing White: 289 - 163 - 367  [0.577] 819
...      midnight-dev playing Black: 190 - 237 - 392  [0.471] 819
...      White vs Black: 526 - 353 - 759  [0.553] 1638
Elo difference: 16.8 +/- 12.3, LOS: 99.6 %, DrawRatio: 46.3 %
SPRT: llr 2.98 (101.3%), lbound -2.94, ubound 2.94 - H1 was accepted
```

### Update Move Ordering def of quiet to include castling/double pushes and Check Extensions
```
Score of midnight-dev vs midnight-latest: 427 - 352 - 716  [0.525] 1495
...      midnight-dev playing White: 241 - 170 - 337  [0.547] 748
...      midnight-dev playing Black: 186 - 182 - 379  [0.503] 747
...      White vs Black: 423 - 356 - 716  [0.522] 1495
Elo difference: 17.4 +/- 12.7, LOS: 99.6 %, DrawRatio: 47.9 %
SPRT: llr 2.96 (100.7%), lbound -2.94, ubound 2.94 - H1 was accepted
```

### New LMR Constants: Base 1.0 Divisor 2.00
https://engineprogramming.pythonanywhere.com/test/8/
```
ELO   | 7.57 +- 6.08 (95%)
SPRT  | 5.0+0.05s Threads=1 Hash=64MB
LLR   | 2.97 (-2.94, 2.94) [0.00, 10.00]
GAMES | N: 6792 W: 1915 L: 1767 D: 3110
```

### New LMR Constants: Base 1.2 Divisor 1.8
https://engineprogramming.pythonanywhere.com/test/13/
```
ELO   | 5.36 +- 4.35 (95%)
SPRT  | 5.0+0.05s Threads=1 Hash=64MB
LLR   | 2.71 (-2.94, 2.94) [0.00, 5.00]
GAMES | N: 13352 W: 3752 L: 3546 D: 6054
```

### Tempo Bonus
https://engineprogramming.pythonanywhere.com/test/33/
```
ELO   | 23.88 +- 10.74 (95%)
SPRT  | 5.0+0.05s Threads=1 Hash=64MB
LLR   | 2.97 (-2.94, 2.94) [0.00, 5.00]
GAMES | N: 2288 W: 728 L: 571 D: 989
```

### Aspiration Windows
https://engineprogramming.pythonanywhere.com/test/54/
```
ELO   | 49.93 +- 15.54 (95%)
SPRT  | 8.0+0.08s Threads=1 Hash=64MB
LLR   | 2.99 (-2.94, 2.94) [0.00, 5.00]
GAMES | N: 1016 W: 338 L: 193 D: 485
```

### Threat Eval
https://engineprogramming.pythonanywhere.com/test/57/
```
ELO   | 31.10 +- 12.32 (95%)
SPRT  | 8.0+0.08s Threads=1 Hash=64MB
LLR   | 3.00 (-2.94, 2.94) [0.00, 5.00]
GAMES | N: 1736 W: 568 L: 413 D: 755
```

### Refactor Q MoveGen
https://engineprogramming.pythonanywhere.com/test/68/
```
ELO   | 2.32 +- 9.27 (95%)
SPRT  | 8.0+0.08s Threads=1 Hash=64MB
LLR   | 2.15 (-2.94, 2.94) [-10.00, 0.00]
GAMES | N: 2848 W: 763 L: 744 D: 1341
```

### Generate Promotions in Q Search
https://engineprogramming.pythonanywhere.com/test/73/
```
ELO | 6.46 +- 4.74 (95%)
SPRT | 8.0+0.08s Threads=1 Hash=64MB
LLR | 3.00 (-2.94, 2.94) [0.00, 5.00]
GAMES | N: 10968 W: 3028 L: 2824 D: 5116
```

### Static Exchange Evaluation
https://engineprogramming.pythonanywhere.com/test/91/
```
ELO   | 15.08 +- 8.19 (95%)
SPRT  | 8.0+0.08s Threads=1 Hash=64MB
LLR   | 2.95 (-2.94, 2.94) [0.00, 5.00]
GAMES | N: 3712 W: 1078 L: 917 D: 1717
```

### Static Exchange Evaluation Pruning in Q Search
https://engineprogramming.pythonanywhere.com/test/110/
```
ELO   | 25.97 +- 11.08 (95%)
SPRT  | 8.0+0.08s Threads=1 Hash=64MB
LLR   | 2.95 (-2.94, 2.94) [0.00, 5.00]
GAMES | N: 2024 W: 616 L: 465 D: 943
```

### Static Exchange Evaluation Pruning in PVS
https://engineprogramming.pythonanywhere.com/test/125/
```
ELO | 26.39 +- 10.86 (95%)
SPRT | 8.0+0.08s Threads=1 Hash=64MB
LLR | 2.99 (-2.94, 2.94) [0.00, 5.00]
GAMES | N: 1952 W: 557 L: 409 D: 986
```

### Move RFP to before NMP because it's cheaper to prune
https://engineprogramming.pythonanywhere.com/test/168/
```
ELO   | 9.60 +- 6.09 (95%)
SPRT  | 8.0+0.08s Threads=1 Hash=64MB
LLR   | 3.00 (-2.94, 2.94) [0.00, 5.00]
GAMES | N: 6264 W: 1660 L: 1487 D: 3117
```

### Only NMP if our evaluation is greater than their best value
https://engineprogramming.pythonanywhere.com/test/177/
```
ELO   | 11.79 +- 7.06 (95%)
SPRT  | 8.0+0.08s Threads=1 Hash=64MB
LLR   | 2.97 (-2.94, 2.94) [0.00, 5.00]
GAMES | N: 4984 W: 1421 L: 1252 D: 2311
```