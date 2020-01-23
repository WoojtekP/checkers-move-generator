#include <algorithm>
#include <iostream>
#include <vector>
#include "bitboard.hpp"

struct gameState
{
    uint32_t white;
    uint32_t black;
    uint32_t kings;
    uint8_t counter;
    bool whiteTurn;
};

class CheckersMoveGenerator
{
public:
    CheckersMoveGenerator();
    void resetState();
    void setState(const gameState&);
    gameState getState();
    std::vector<uint32_t> getMovesList();
    bool isDraw();
    void applyMove(const uint32_t&);
    friend std::ostream& operator<< (std::ostream&, const CheckersMoveGenerator&);
    bool whiteTurn;
private:
    std::vector<uint32_t> removeDuplicates(std::vector<uint32_t>&) const;
    CheckersBitboard board;
    uint8_t kingMovesCounter = 0;
};