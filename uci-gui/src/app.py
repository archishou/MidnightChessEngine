#
# Web based GUI for BBC chess engine
#

# packages
import re
from flask import Flask
from flask import render_template
from flask import request
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

# probe book move

# root(index) route
@app.route('/')
def root():
    return render_template('bbc.html')

# make move API
@app.route('/make_move', methods=['POST'])
def make_move():
    # extract FEN string from HTTP POST request body
    pgn = request.form.get('pgn')

    # read game moves from PGN
    game = chess.pgn.read_game(io.StringIO(pgn))    
    
    # init board
    board = game.board()
    
    # loop over moves in game
    for move in game.mainline_moves():
        # make move on chess board
        board.push(move)
    
    # create chess engine instance
    engine_location = "/Users/Archish/Documents/CodeProjects/C/ChessEngine/cmake-build-debug/ChessEngine"
    engine = chess.engine.SimpleEngine.popen_uci(engine_location)
    result = engine.play(board, chess.engine.Limit(time=0.1))
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
        'Agent':request.headers.get('User-Agent')
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
    
    with open('stats.json', 'a') as f: f.write(json.dumps(stats, indent=2) + '\n\n')
    return response


@app.route('/analytics/api/get')
def get():
    stats = []
    
    with open('stats.json') as f:
        for entry in f.read().split('\n\n'):
            try: stats.append(json.loads(entry))
            except: pass
              
    return jsonify({'data': stats})

# main driver
if __name__ == '__main__':
    # start HTTP server
    app.run(debug=True, threaded=True)
