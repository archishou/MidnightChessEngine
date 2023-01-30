#
# Web based GUI for BBC chess engine
#

# packages
import re
from flask import Flask
from flask import render_template
from flask import request
import logging
import chess
import chess.engine
import chess.pgn
import io
import random
from flask import jsonify
from flask import Response
from flask_pymongo import PyMongo
from datetime import datetime
import json

# create web app instance
app = Flask(__name__)
'''
log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)
'''

# probe book move
def probe_book(pgn):
    # open book file
    with open('./engine/book.txt') as f:
        # read book games
        book = f.read()

        # init board
        board = chess.Board()

        # define response moves
        response_moves = []

        # loop over book lines
        for line in book.split('\n')[0:-1]:
            # define variation
            variation = []

            # loop over line moves
            for move in line.split():
                variation.append(chess.Move.from_uci(move))

            # parse variation to SAN
            san = board.variation_san(variation)

            # match book line
            if pgn in san:
                try:
                    # black move
                    if san.split(pgn)[-1].split()[0][0].isdigit():
                        response_moves.append(san.split(pgn)[-1].split()[1])

                    # white move
                    else:
                        response_moves.append(san.split(pgn)[-1].split()[0])

                except:
                    pass

            # engine makes first move
            if pgn == '':
                response_moves.append(san.split('1. ')[-1].split()[0])

        # return random response move
        if len(response_moves):
            move = random.choice(response_moves)
            print('BOOK MOVE:', pgn, move)
            return move

        else:
            return 0


# root(index) route
@app.route('/')
def root():
    return render_template('bbc.html')


# make move API
@app.route('/make_move', methods=['POST'])
def make_move():
    # extract FEN string from HTTP POST request body
    pgn = request.form.get('pgn')
    book_response = probe_book(pgn)
    if book_response:
        return {
            'score': 0,
            'best_move': book_response,
        }


    # read game moves from PGN
    game = chess.pgn.read_game(io.StringIO(pgn))

    # init board
    board = game.board()

    # loop over moves in game
    for move in game.mainline_moves():
        # make move on chess board
        board.push(move)

    # create chess engine instance
    engine_location = '/Users/archishmaan/Documents/CodeProjects/chess-engine/ChessEngine/ChessEngine_tests' \
                      '/cutechess_tests/latest_master/master'
    engine = chess.engine.SimpleEngine.popen_uci(engine_location)
    result = engine.play(board, chess.engine.Limit(time=1))
    best_move = result.move
    board.push(result.move)
    # terminate engine process
    engine.quit()

    return {
        'fen': board.fen(),
        'best_move': str(best_move),
        'score': 0,
        'depth': 0,
        'pv': ' ',
        'nodes': 0,
        'time': 0,
    }


@app.route('/analytics')
def analytics():
    return render_template('stats.html')


@app.route('/analytics/api/post', methods=['POST'])
def post():
    response = Response('')
    response.headers['Access-Control-Allow-Origin'] = '*'

    stats = {
        'Date': request.form.get('date'),
        'Url': request.form.get('url'),
        'Agent': request.headers.get('User-Agent')
    }

    if request.headers.getlist("X-Forwarded-For"):
        stats['Ip'] = request.headers.getlist("X-Forwarded-For")[0]
    else:
        stats['Ip'] = request.remote_addr

    if request.headers.get('Origin'):
        stats['Origin'] = request.headers.get('Origin')
    else:
        stats['Origin'] = 'N/A'

    if request.headers.get('Referer'):
        stats['Referer'] = request.headers.get('Referer')
    else:
        stats['Referer'] = 'N/A'

    with open('stats.json', 'a') as f:
        f.write(json.dumps(stats, indent=2) + '\n\n')
    return response


@app.route('/analytics/api/get')
def get():
    stats = []

    with open('stats.json') as f:
        for entry in f.read().split('\n\n'):
            try:
                stats.append(json.loads(entry))
            except:
                pass

    return jsonify({'data': stats})


# main driver
if __name__ == '__main__':
    # start HTTP server
    app.run(debug=True, threaded=True)
