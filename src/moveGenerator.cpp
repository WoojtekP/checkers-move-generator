#include "moveGenerator.hpp"

CheckersMoveGenerator::CheckersMoveGenerator() : whiteTurn(false)
{
    board.resetBoard();
}

void CheckersMoveGenerator::resetState()
{
    board.resetBoard();
    kingMovesCounter = 0;
    whiteTurn = false;
}

void CheckersMoveGenerator::setState(const gameState& state)
{
    board.setWhiteMan(state.white);
    board.setBlackMan(state.black);
    board.setKings(state.kings);
    kingMovesCounter = state.counter;
    whiteTurn = state.whiteTurn;
}

gameState CheckersMoveGenerator::getState()
{
    return { board.getWhitePieces(), board.getBlackPieces(), board.getKings(), kingMovesCounter, whiteTurn };
}

std::vector<uint32_t> CheckersMoveGenerator::getMovesList()
{
    std::vector<uint32_t> moves;
    if (whiteTurn)
    {
        auto jumpers = board.getWhiteJumpers();
        if (jumpers)
            moves =  board.getWhiteJumpList(jumpers);
        else
            moves = board.getWhiteMoveList();
        
    }
    else
    {
        auto jumpers = board.getBlackJumpers();
        if (jumpers)
            moves = board.getBlackJumpList(jumpers);
        else 
            moves = board.getBlackMoveList();
    }
    return moves;
}

bool CheckersMoveGenerator::isDraw()
{
    return kingMovesCounter >= 20;
}

void CheckersMoveGenerator::applyMove(const uint32_t& mv)
{
    auto white = board.getWhitePieces();
    auto black = board.getBlackPieces();
    auto kings = board.getKings();
    if (whiteTurn)
    {
        board.applyWhiteMove(mv);
        if ((mv & white & kings) && ((mv & black) == 0))
        {
            kingMovesCounter++;
        }
        else
        {
            kingMovesCounter = 0;
        }
    }
    else
    {
        board.applyBlackMove(mv);
        if ((mv & black & kings) && ((mv & white) == 0))
        {
            kingMovesCounter++;
        }
        else
        {
            kingMovesCounter = 0;
        }
    }
    whiteTurn = !whiteTurn;
}

std::ostream& operator<< (std::ostream& out, const CheckersMoveGenerator& board)
{
    auto white = board.board.getWhitePieces();
    auto black = board.board.getBlackPieces();
    for (int row = 7; row >= 0; --row)
    {
        if (row % 2 == 1)
            out << ' ';
        for (int col = 0; col < 4; ++col)
        {
            if ((white >> (row*4 + col)) & 1)
            {
                out << 'W';
            }
            else if((black >> (row*4 + col)) & 1)
            {
                out << 'B';
            }
            else
            {
                out << '_';
            }
            out << ' ';
        }
        out << std::endl;
    }
    out << "white: " << std::hex << white << std::endl << "black: " << black << std::endl;
    return out;
}
