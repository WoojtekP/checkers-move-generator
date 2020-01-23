#include "bitboard.hpp"

void CheckersBitboard::resetBoard()
{
    BlackPieces = 0xFFF00000;
    WhitePieces = 0xFFF;
    Kings = 0x0;
}

void CheckersBitboard::setWhiteMan(const uint32_t& pieces)
{
    WhitePieces = pieces;
    Kings &= BlackPieces;
}

void CheckersBitboard::setBlackMan(const uint32_t& pieces)
{
    BlackPieces = pieces;
    Kings &= WhitePieces;
}

void CheckersBitboard::setKings(const uint32_t& pieces)
{
    Kings = pieces & (WhitePieces | BlackPieces);
}

uint32_t CheckersBitboard::getWhitePieces() const
{
    return WhitePieces;
}

uint32_t CheckersBitboard::getBlackPieces() const
{
    return BlackPieces;
}

uint32_t CheckersBitboard::getKings() const
{
    return Kings;
}

uint32_t CheckersBitboard::getWhiteMovers() const
{
    return getMoversUp(WhitePieces) | getMoversDown(WhitePieces & Kings);
}

uint32_t CheckersBitboard::getBlackMovers() const
{
    return getMoversDown(BlackPieces) | getMoversUp(BlackPieces & Kings);
}

uint32_t CheckersBitboard::getWhiteJumpers() const
{
    return getJumpersUp(WhitePieces, BlackPieces) | getJumpersDown(WhitePieces & Kings, BlackPieces);
}

uint32_t CheckersBitboard::getBlackJumpers() const
{
    return getJumpersDown(BlackPieces, WhitePieces) | getJumpersUp(BlackPieces & Kings, WhitePieces);
}

std::vector<uint32_t> CheckersBitboard::getWhiteMoveList() const
{
    std::vector<uint32_t> moves;
    moves.reserve(32);
    auto movers = getWhiteMovers();
    getMoveListUp(movers, moves);
    getMoveListDown(movers & Kings, moves);
    return moves;
}

std::vector<uint32_t> CheckersBitboard::getBlackMoveList() const
{
    std::vector<uint32_t> moves;
    moves.reserve(32);
    auto movers = getBlackMovers();
    getMoveListDown(movers, moves);
    getMoveListUp(movers & Kings, moves);
    return moves;
}

std::vector<uint32_t> CheckersBitboard::getWhiteJumpList(uint32_t jumpers) const
{
    std::vector<uint32_t> jumps;
    jumps.reserve(32);
    uint32_t empty = ~(WhitePieces | BlackPieces);
    getJumpListUp(jumpers & ~Kings, BlackPieces, empty, jumps);
    getJumpListTwoSides(jumpers & Kings, BlackPieces, empty, jumps);
    return jumps;
}

std::vector<uint32_t> CheckersBitboard::getBlackJumpList(uint32_t jumpers) const
{
    std::vector<uint32_t> jumps;
    jumps.reserve(32);
    uint32_t empty = ~(WhitePieces | BlackPieces);
    getJumpListDown(jumpers & ~Kings, WhitePieces, empty, jumps);
    getJumpListTwoSides(jumpers & Kings, WhitePieces, empty, jumps);
    return jumps;
}

void CheckersBitboard::applyWhiteMove(const uint32_t& mv)
{
    WhitePieces ^= mv & ~BlackPieces;  // move white piece
    BlackPieces &= ~mv;  // remove black pieces
    if (mv & Kings)
        Kings ^= mv & WhitePieces;  // move king if piece=king
    Kings |= mv & WhitePieces & 0xF0000000;  // promote white
    Kings &= BlackPieces | WhitePieces;  // remove black kings (if white jumps over king)
}

void CheckersBitboard::applyBlackMove(const uint32_t& mv)
{
    BlackPieces ^= mv & ~WhitePieces;
    WhitePieces &= ~mv;
    if (mv & Kings)
        Kings ^= mv & BlackPieces;
    Kings |= mv & BlackPieces & 0xF;
    Kings &= WhitePieces | BlackPieces;

}

inline uint32_t CheckersBitboard::getMoversDown(const uint32_t& pieces) const
{
    if (pieces == 0)
        return 0;
    uint32_t empty = ~(WhitePieces | BlackPieces);
    uint32_t movers = (empty << 4) & pieces;
    movers |= ((empty & 0x0E0E0E0E) << 3) & pieces;
    movers |= ((empty & 0x00707070) << 5) & pieces;
    return movers;
}

inline uint32_t CheckersBitboard::getMoversUp(const uint32_t& pieces) const
{
    if (pieces == 0)
        return 0;
    uint32_t empty = ~(WhitePieces | BlackPieces);
    uint32_t movers = (empty >> 4) & pieces;
    movers |= ((empty & 0x70707070) >> 3) & pieces;
    movers |= ((empty & 0x0E0E0E00) >> 5) & pieces;
    return movers;
}

inline uint32_t CheckersBitboard::getJumpersDown(const uint32_t& jumpers, const uint32_t& pieces) const
{
    uint32_t empty = ~(WhitePieces | BlackPieces);
    uint32_t jumps = jumpers & 0x07070700 & (empty << 7) & (pieces << 4);  // odd -> right
    jumps |= jumpers & 0x0E0E0E00 & (empty << 9) & (pieces << 5);  // odd -> left
    jumps |= jumpers & 0x70707000 & (empty << 7) & (pieces << 3);  // even -> right
    jumps |= jumpers & 0xE0E0E000 & (empty << 9) & (pieces << 4);  // even -> left
    return jumps;
}

inline uint32_t CheckersBitboard::getJumpersUp(const uint32_t& jumpers, const uint32_t& pieces) const
{
    uint32_t empty = ~(WhitePieces | BlackPieces);
    uint32_t jumps = jumpers & 0x00070707 & (empty >> 9) & (pieces >> 4);  // odd -> right
    jumps |= jumpers & 0x000E0E0E & (empty >> 7) & (pieces >> 3);  // odd -> left
    jumps |= jumpers & 0x00707070 & (empty >> 9) & (pieces >> 5);  // even -> right
    jumps |= jumpers & 0x00E0E0E0 & (empty >> 7) & (pieces >> 4);  // even -> left
    return jumps;
}

inline void CheckersBitboard::getMoveListDown(uint32_t pieces, std::vector<uint32_t>& moves) const
{
    uint32_t empty = ~(WhitePieces | BlackPieces);
    while(pieces)
    {
        uint32_t piece = msb(pieces);
        uint32_t mv = (empty << 4) & piece;
        if (mv)
            moves.push_back(piece | (piece >> 4));
        
        mv = ((empty & 0x0E0E0E0E) << 3) & piece;
        if (mv)
            moves.push_back(piece | (piece >> 3));
        
        mv = ((empty & 0x00707070) << 5) & piece;
        if (mv)
            moves.push_back(piece | (piece >> 5));
        
        pieces ^= piece;
    }
}

inline void CheckersBitboard::getMoveListUp(uint32_t pieces, std::vector<uint32_t>& moves) const
{
    uint32_t empty = ~(WhitePieces | BlackPieces);
    while(pieces)
    {
        uint32_t piece = msb(pieces);
        uint32_t mv = (empty >> 4) & piece;
        if (mv)
            moves.push_back(piece | (piece << 4));
        
        mv = ((empty & 0x70707070) >> 3) & piece;
        if (mv)
            moves.push_back(piece | (piece << 3));
        
        mv = ((empty & 0x0E0E0E00) >> 5) & piece;
        if (mv)
            moves.push_back(piece | (piece << 5));
        
        pieces ^= piece;
    }
}

void CheckersBitboard::getJumpListDown(uint32_t jumpers, uint32_t pieces, uint32_t empty, std::vector<uint32_t>& jumps) const
{
    static constexpr jumpMask bits[4] = {
        {0x70707000, 7, 3}, // even rows
        {0xE0E0E000, 9, 4}, // even rows
        {0x07070700, 7, 4}, // odd rows
        {0x0E0E0E00, 9, 5}  // odd rows
    };
    while(jumpers)
    {
        uint32_t jumper = msb(jumpers);
        int offset = (jumper & 0x0F0F0F00) ? 2 : 0;  // offset for odd rows
        for (int i = offset; i < 2 + offset; ++i)
        {
            bool isJump = bits[i].mask & (empty << bits[i].empty) & (pieces << bits[i].enemy) & jumper;
            if (isJump)
            {
                uint32_t jump = jumper | (jumper >> bits[i].empty) | (jumper >> bits[i].enemy);
                std::vector<uint32_t> nextJumps;
                nextJumps.reserve(4);
                getJumpListDown(jumper >> bits[i].empty, pieces, empty ^ (jumper >> bits[i].enemy), nextJumps);
                if (!nextJumps.empty())
                {
                    for (const auto& next : nextJumps)
                    {
                        jumps.push_back(jump ^ next);
                    }
                }
                else
                {
                    jumps.push_back(jump);
                }
            }
        }
        jumpers ^= jumper;
    }
}

void CheckersBitboard::getJumpListUp(uint32_t jumpers, uint32_t pieces, uint32_t empty, std::vector<uint32_t>& jumps) const
{
    static constexpr jumpMask bits[4] = {
        {0x00707070, 9, 5}, // even rows
        {0x00E0E0E0, 7, 4}, // even rows
        {0x00070707, 9, 4}, // odd rows
        {0x000E0E0E, 7, 3}  // odd rows
    };
    while(jumpers)
    {
        uint32_t jumper = msb(jumpers);
        int offset = (jumper & 0x00F0F0F0) ? 0 : 2;  // offset for odd rows
        for (int i = offset; i < 2 + offset; ++i)
        {
            bool isJump = bits[i].mask & (empty >> bits[i].empty) & (pieces >> bits[i].enemy) & jumper;
            if (isJump)
            {
                uint32_t jump = jumper | (jumper << bits[i].empty) | (jumper << bits[i].enemy);
                std::vector<uint32_t> nextJumps;
                nextJumps.reserve(4);
                getJumpListUp(jumper << bits[i].empty, pieces, empty ^ (jumper << bits[i].enemy), nextJumps);
                if (!nextJumps.empty())
                {
                    for (const auto& next : nextJumps)
                    {
                        jumps.push_back(jump ^ next);
                    }
                }
                else
                {
                    jumps.push_back(jump);
                }
            }
        }
        jumpers ^= jumper;
    }
}

void CheckersBitboard::getJumpListTwoSides(uint32_t jumpers, uint32_t pieces, uint32_t empty, std::vector<uint32_t>& jumps) const
{
    static constexpr jumpMask bits[8] = {
        {0x00707070, 9, 5}, // even up
        {0x00E0E0E0, 7, 4}, // even up
        {0x70707000, 7, 3}, // even down
        {0xE0E0E000, 9, 4}, // even down
        {0x00070707, 9, 4}, // odd up
        {0x000E0E0E, 7, 3}, // odd up
        {0x07070700, 7, 4}, // odd down
        {0x0E0E0E00, 9, 5}  // odd down
    };
    while(jumpers)
    {
        uint32_t jumper = msb(jumpers);
        int offset = (jumper & 0x0F0F0F0F) ? 4 : 0;  // offset for odd rows
        for (int i = offset; i < 2 + offset; ++i)
        {
            // jumps up
            bool isJump = bits[i].mask & (empty >> bits[i].empty) & (pieces >> bits[i].enemy) & jumper;
            if (isJump)
            {
                uint32_t jump = jumper | (jumper << bits[i].empty) | (jumper << bits[i].enemy);
                std::vector<uint32_t> nextJumps;
                nextJumps.reserve(8);
                getJumpListTwoSides(jumper << bits[i].empty, pieces ^ (jumper << bits[i].enemy), empty ^ jumper, nextJumps);
                if (!nextJumps.empty())
                {
                    for (const auto& next : nextJumps)
                    {
                        jumps.push_back(jump ^ next);
                    }
                }
                else
                {
                    jumps.push_back(jump);
                }
            }
            // jumps down
            isJump = bits[i+2].mask & (empty << bits[i+2].empty) & (pieces << bits[i+2].enemy) & jumper;
            if (isJump)
            {
                uint32_t jump = jumper | (jumper >> bits[i+2].empty) | (jumper >> bits[i+2].enemy);
                std::vector<uint32_t> nextJumps;
                nextJumps.reserve(8);
                getJumpListTwoSides(jumper >> bits[i+2].empty, pieces ^ (jumper >> bits[i+2].enemy), empty ^ jumper, nextJumps);
                if (!nextJumps.empty())
                {
                    for (const auto& next : nextJumps)
                    {
                        jumps.push_back(jump ^ next);
                    }
                }
                else
                {
                    jumps.push_back(jump);
                }
            }
        }
        jumpers ^= jumper;
    }
}


inline uint32_t CheckersBitboard::msb(uint32_t pieces) const
{
    uint64_t n = pieces;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n += 1;
    return n >> 1;
}