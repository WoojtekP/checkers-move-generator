#include <chrono>
#include <iostream>
#include <vector>
#include <random>
#include <unistd.h>
#include "moveGenerator.hpp"


int main(int argc, char *argv[])
{
    int games = 100000;
    int opt;
    while ((opt = getopt(argc, argv, "n:")) != -1)
    {
        if (opt == 'n')
        {
            games = atoi(optarg);
        }
    }
    std::cout << "benchmark with " << games << " random games started...\n";
    CheckersMoveGenerator moveGenerator;
    long long rounds = 0;
    std::random_device device;
    std::mt19937 numberGenerator(device());
    auto start = std::chrono::steady_clock::now();
    int whiteWins = 0, blackWins = 0, draws = 0;
    for (int i = 0; i < games; ++i) 
    {
        while(1)
        {
            if (moveGenerator.isDraw())
            {
                draws++;
                break;
            }
            auto moves = moveGenerator.getMovesList();
            int n = moves.size();
            if (n == 0)
            {
                if (moveGenerator.isDraw())
                    draws++;
                else if (moveGenerator.whiteTurn)
                    whiteWins++;
                else
                    blackWins++;
                break;
            }
            std::uniform_int_distribution<int> distribution(0,n-1);
            auto mv = moves[distribution(numberGenerator)];
            moveGenerator.applyMove(mv);
            rounds++;
        }
        moveGenerator.resetState();
    }
    auto end = std::chrono::steady_clock::now();
    double seconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000.0;
    std::cout << "time: " << seconds << "s" << std::endl;
    std::cout << "moves: " << rounds << std::endl;
    std::cout << "games/s: " << games / seconds << std::endl;
    std::cout << "moves/s: " << rounds / seconds << std::endl;
    std::cout << "white/black win ratio " << whiteWins * 1.0 / blackWins << std::endl;
    std::cout << "draws/games ratio " << draws * 1.0 / games << std::endl;
    return 0;
}