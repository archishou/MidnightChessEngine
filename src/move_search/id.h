//
// Created by Archishmaan Peyyety on 1/1/23.
//
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#include "position.h"
#include "alphabeta.h"

struct IDResults {
    Move bestMove;
    int depthSearched;
    double timeSearched;
};

template<Color color>
IDResults iterativeDeepening(Position& board, int maxTimeMilliseconds) {
    struct IDResults idResults;
    std::chrono::time_point endTime = std::chrono::system_clock::now() + std::chrono::milliseconds(maxTimeMilliseconds);

    std::clock_t start;
    start = std::clock();

    for (int subDepth = 1; subDepth <= DEPTH; subDepth++) {
        struct AlphaBetaResults abResults = alphaBetaRoot<color, std::chrono::system_clock>(board, subDepth, endTime);
        if (abResults.searchCompleted) {
            idResults.bestMove = abResults.bestMove;
            idResults.depthSearched = subDepth;
        }
    }

    idResults.timeSearched = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    return idResults;
}
