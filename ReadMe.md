<div align="center">

# Midnight
[![License][license-badge]][license-link]
[![Release][release-badge]][release-link]
[![Commits][commits-badge]][commits-link]

</div>
A free and open source UCI chess engine written in C++.

# Strength

| Version      | CCRL 40/15                                                                                                                                                                      | CCRL Blitz                                                                                                                                                                     |
|--------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Midnight 8.0 | [3257±21 [#42]](http://computerchess.org.uk/ccrl/4040/cgi/engine_details.cgi?match_length=30&each_game=0&print=Details&each_game=0&eng=Midnight%208%2064-bit#Midnight_8_64-bit) | [3373±17 [#44]](http://computerchess.org.uk/ccrl/404/cgi/engine_details.cgi?match_length=30&each_game=1&print=Details&each_game=1&eng=Midnight%208%2064-bit#Midnight_8_64-bit) |
| Midnight 7.0 | [3020±21 [#93]](http://computerchess.org.uk/ccrl/4040/cgi/engine_details.cgi?match_length=30&each_game=0&print=Details&each_game=0&eng=Midnight%207%2064-bit#Midnight_7_64-bit) | [3113±17 [#89]](http://computerchess.org.uk/ccrl/404/cgi/engine_details.cgi?match_length=30&each_game=1&print=Details&each_game=1&eng=Midnight%207%2064-bit#Midnight_7_64-bit) |              
| Midnight 6.0 | [2919±30 [#110]](https://ccrl.chessdom.com/ccrl/4040/cgi/engine_details.cgi?print=Details&each_game=0&eng=Midnight%206%2064-bit#Midnight_6_64-bit)                              | [3055±19 [#92]](https://ccrl.chessdom.com/ccrl/404/cgi/engine_details.cgi?print=Details&each_game=1&eng=Midnight%206%2064-bit#Midnight_6_64-bit)                               |
| Midnight 5.0 |                                                                                                                                                                                 | [2828±15 [#146]](http://ccrl.chessdom.com/ccrl/404/cgi/engine_details.cgi?print=Details&each_game=1&eng=Midnight%205%2064-bit#Midnight_5_64-bit)                               |

# Compilation
```
git clone https://github.com/archishou/MidnightChessEngine
cd MidnightChessEngine
make
```

# Credits
Thanks to [@Alex2262](https://github.com/Alex2262) for helping with lots of small improvements in my engine.

Thanks to [@Ciekce](https://github.com/Ciekce) for helping me become a better C++ dev. He helped me find code that produced undefined behavior and is responsible for helping me fix my Makefile numerous times.

[commits-badge]:https://img.shields.io/github/commits-since/archishou/MidnightChessEngine/latest?style=for-the-badge
[commits-link]:https://github.com/archishou/MidnightChessEngine/commits/master
[release-badge]:https://img.shields.io/github/v/release/archishou/MidnightChessEngine?style=for-the-badge&label=official%20release
[release-link]:https://github.com/archishou/MidnightChessEngine/releases/latest
[license-badge]:https://img.shields.io/github/license/archishou/MidnightChessEngine?style=for-the-badge&label=license&color=success
[license-link]:https://github.com/archishou/MidnightChessEngine/blob/master/LICENSE