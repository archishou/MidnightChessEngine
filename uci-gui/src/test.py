import asyncio
import chess
import chess.engine


async def main() -> None:
    transport, engine = await chess.engine.popen_uci("/Users/Archish/Documents/CodeProjects/C/ChessEngine/cmake-build-debug/ChessEngine")

    board = chess.Board()
    i = 0
    while not board.is_game_over() and i < 200:
        result = await engine.play(board, chess.engine.Limit(time=0.1))
        board.push(result.move)
        i += 1

    await engine.quit()

for i in range(10000):
    print(i / 10000, end='\r')
    asyncio.set_event_loop_policy(chess.engine.EventLoopPolicy())
    asyncio.run(main())