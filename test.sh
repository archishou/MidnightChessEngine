# cd cutechess dir
cd /Users/archishmaan/Documents/cutechess/build/

# Engine Paths and Names
ENGINE_DEV=(midnight-dev /Users/archishmaan/Documents/CodeProjects/chess-engine/ChessEngine/cmake-build-debug/ChessEngine_run)
ENGINE_V2=(midnight-v2 /Users/archishmaan/Documents/CodeProjects/chess-engine/ChessEngine/releases/v2/master)
ENGINE_LATEST=(midnight-v2 /Users/archishmaan/Documents/CodeProjects/chess-engine/ChessEngine/releases/vlatest/master)

# Opening Books
SUITE_1=/Users/archishmaan/Documents/CodeProjects/chess-engine/ChessEngine/testing/books-master/8moves_v3.pgn

# Testing Folder
TEST_ROOT=/Users/archishmaan/Documents/CodeProjects/chess-engine/ChessEngine/testing

# Test Name
TEST_NAME=test1

# Test Variables
ENGINE_1=(${ENGINE_DEV[*]})
ENGINE_2=(${ENGINE_V2[*]})
TEST_OPENING_BOOK=$SUITE_1
TIME_CONTROL=5+0.05
CONCURRENCY=5

mkdir -p $TEST_ROOT/$TEST_NAME

echo Engine 1 Name ${ENGINE_1[0]}
echo Engine 1 Path ${ENGINE_1[1]}
echo Engine 2 Name ${ENGINE_2[0]}
echo Engine 2 Path ${ENGINE_2[1]}

./cutechess-cli \
    -engine name=${ENGINE_1[0]} proto=uci cmd=${ENGINE_1[1]} stderr=$TEST_ROOT/$TEST_NAME/stderr/${ENGINE_1[0]}_stderr.txt \
    -engine name=${ENGINE_2[0]} proto=uci cmd=${ENGINE_2[1]} stderr=$TEST_ROOT/$TEST_NAME/stderr/${ENGINE_2[0]}_stderr.txt\
    -repeat -each tc=8+0.08 \
    -openings file=$TEST_OPENING_BOOK format=pgn order=random \
    -concurrency $CONCURRENCY -ratinginterval 1 -games 50000 \
    -draw movenumber=200 movecount=0 score=0 \
    -sprt elo0=0 elo1=10 alpha=0.05 beta=0.05 \
    -pgnout $TEST_ROOT/$TEST_NAME/test.pgn \
    -recover \


    # -debug \
