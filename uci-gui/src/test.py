import asyncio
import chess
import chess.engine


async def main() -> None:
    transport, engine = await chess.engine.popen_uci("/Users/Archish/Documents/CodeProjects/C/ChessEngine/cmake-build-debug/ChessEngine")

    board = chess.Board()
    for _ in range(100):
        result = await engine.play(board, chess.engine.Limit(time=0.1))
        board.push(result.move)
        print(board.fen())

    print(board)
    await engine.quit()

asyncio.set_event_loop_policy(chess.engine.EventLoopPolicy())
asyncio.run(main())