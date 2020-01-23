#include <algorithm>
#include <iostream>
#include <vector>
#include <moveGenerator.hpp>

// generates results for all levels form depth to 1
void perft_all(CheckersMoveGenerator &generator, std::vector<unsigned long> &result, int depth)
{
    auto moveList = generator.getMovesList();
    result[depth-1] += moveList.size();
    if (depth == 1)
    {
        return;
    }
    depth--;
    auto init = generator.getState();
    for (const auto& move : moveList)
    {
        generator.applyMove(move);
        perft_all(generator, result, depth);
        generator.setState(init);
    }
}

// generates number of possible moves on given depth
unsigned long perft(CheckersMoveGenerator &generator, int depth)
{
    if (depth == 0) 
    {
        return 1;
    }
    unsigned long nodes = 0;
    auto moveList = generator.getMovesList();
    auto init = generator.getState();
    for (const auto mv : moveList) 
    {
        generator.applyMove(mv);
        nodes += perft(generator, depth - 1);
        generator.setState(init);
    }
    return nodes;
}

int main()
{
    CheckersMoveGenerator generator;
    int depth = 10;
    std::cout << "depth: ";
    std::cin >> depth;
    
    std::vector<unsigned long> result(depth, 0);
    perft_all(generator, result, depth);
    std::reverse(result.begin(), result.end());
    for (int i = 0; i < depth; ++i)
    {
        std::cout << "depth " << i + 1 << " positions " << result[i] << std::endl;
    }

    // std::cout << std::endl;
    // for (int i = 1; i <= depth; ++i)
    // {
    //     generator.resetState();
    //     std::cout << "depth " << i << " positions " << perft(generator, i) << std::endl;
    // }
}