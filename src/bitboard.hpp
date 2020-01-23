#include <cstdint>
#include <vector>

struct jumpMask
{
    uint32_t mask;
    uint8_t empty;
    uint8_t enemy;
};

class CheckersBitboard
{
public:
    void resetBoard();
    void setWhiteMan(const uint32_t&);
    void setBlackMan(const uint32_t&);
    void setKings(const uint32_t&);

    uint32_t getWhitePieces() const;
    uint32_t getBlackPieces() const;
    uint32_t getKings() const;

    uint32_t getWhiteMovers() const;
    uint32_t getBlackMovers() const;
    uint32_t getWhiteJumpers() const;
    uint32_t getBlackJumpers() const;

    std::vector<uint32_t> getWhiteMoveList() const;
    std::vector<uint32_t> getBlackMoveList() const;

    std::vector<uint32_t> getWhiteJumpList(uint32_t) const;
    std::vector<uint32_t> getBlackJumpList(uint32_t) const;
    
    void applyWhiteMove(const uint32_t&);
    void applyBlackMove(const uint32_t&);

private:
    inline uint32_t getMoversDown(const uint32_t&) const;
    inline uint32_t getMoversUp(const uint32_t&) const;
    inline uint32_t getJumpersDown(const uint32_t&, const uint32_t&) const;
    inline uint32_t getJumpersUp(const uint32_t&, const uint32_t&) const;
    inline void getMoveListDown(uint32_t, std::vector<uint32_t>&) const;
    inline void getMoveListUp(uint32_t, std::vector<uint32_t>&) const;
    void getJumpListDown(uint32_t, uint32_t, uint32_t, std::vector<uint32_t>&) const;
    void getJumpListUp(uint32_t, uint32_t, uint32_t, std::vector<uint32_t>&) const;
    void getJumpListTwoSides(uint32_t, uint32_t, uint32_t, std::vector<uint32_t>&) const;
    inline uint32_t msb(uint32_t) const;
    uint32_t WhitePieces;
    uint32_t BlackPieces;
    uint32_t Kings;
};
