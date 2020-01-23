#define CATCH_CONFIG_MAIN
#include <algorithm>
#include <vector>
#include <catch.hpp>
#include <bitboard.hpp>

using namespace Catch::Matchers;

uint32_t generateBitboard(const std::vector<int>& pieces)
{
    uint32_t board = 0;
    for (const auto& p : pieces)
    {
        board |= 1 << p;
    }
    return board;
}

TEST_CASE("Checkers bitboard should", "")
{
    CheckersBitboard bitboard;

    SECTION("generate correct list of movers from beginning position")
    {
        bitboard.resetBoard();
        REQUIRE(bitboard.getWhiteMovers() == generateBitboard({8,9,10,11}));
        REQUIRE(bitboard.getBlackMovers() == generateBitboard({20,21,22,23}));
    }

    SECTION("generate correct list of jumpers from beginning position")
    {
        bitboard.resetBoard();
        REQUIRE(bitboard.getWhiteJumpers() == 0);
        REQUIRE(bitboard.getBlackJumpers() == 0);
    }

    SECTION("generate correct list of white man which can move left")
    {
        bitboard.setBlackMan(0);
        bitboard.setWhiteMan(generateBitboard({7,15,23}));
        REQUIRE(bitboard.getWhiteMovers() == generateBitboard({7,15,23}));
        REQUIRE(bitboard.getWhiteJumpers() == 0);

        bitboard.setWhiteMan(generateBitboard({7,19,23}));
        REQUIRE(bitboard.getWhiteMovers() == generateBitboard({7,19,23}));

        bitboard.setWhiteMan(generateBitboard({15,19,23}));
        REQUIRE(bitboard.getWhiteMovers() == generateBitboard({19,23}));

        bitboard.setWhiteMan(generateBitboard({7,15,23,31,11,19,27}));
        REQUIRE(bitboard.getWhiteMovers() == generateBitboard({11,19,27}));
    }

    SECTION("generate correct list of white man which can move right")
    {
        bitboard.setBlackMan(0);
        bitboard.setWhiteMan(generateBitboard({0,8,16}));
        REQUIRE(bitboard.getWhiteMovers() == generateBitboard({0,8,16}));

        bitboard.setWhiteMan(generateBitboard({0,12,16}));
        REQUIRE(bitboard.getWhiteMovers() == generateBitboard({0,12,16}));

        bitboard.setWhiteMan(generateBitboard({8,12,16}));
        REQUIRE(bitboard.getWhiteMovers() == generateBitboard({12,16}));

        bitboard.setWhiteMan(generateBitboard({0,8,16,24,4,12,20}));
        REQUIRE(bitboard.getWhiteMovers() == generateBitboard({4,12,20,24}));
    }

    SECTION("generate correct list of possible moves for white and black pieces from starting position")
    {
        //  B B B B 28-31
        // B B B B  24-27
        //  B B B B 20-23
        // _ _ _ _  16-19
        //  _ _ _ _ 12-15
        // W W W W  8 -11
        //  W W W W 4 - 7
        // W W W W  0 - 3
        bitboard.resetBoard();

        REQUIRE_THAT(bitboard.getWhiteMoveList(), UnorderedEquals(std::vector<uint32_t>{
            generateBitboard({8,12}),
            generateBitboard({9,12}),
            generateBitboard({9,13}),
            generateBitboard({10,13}),
            generateBitboard({10,14}),
            generateBitboard({11,14}),
            generateBitboard({11,15})
        }));

        REQUIRE_THAT(bitboard.getBlackMoveList(), UnorderedEquals(std::vector<uint32_t>{
            generateBitboard({20,16}),
            generateBitboard({20,17}),
            generateBitboard({21,17}),
            generateBitboard({21,18}),
            generateBitboard({22,18}),
            generateBitboard({22,19}),
            generateBitboard({23,19})
        }));
    }

    SECTION("generate correct list of white man which can move in board without black pieces")
    {
        bitboard.setBlackMan(0);
        // white:
        // 22, 23   + +
        // 18, 19   + -
        // 12, 13   + +
        // 09, 11   - +
        // 06       +
        // 02       +;
        bitboard.setWhiteMan(generateBitboard({22,23,18,19,12,13,9,11,6,2}));
        REQUIRE(bitboard.getWhiteMovers() == generateBitboard({22,23,18,12,13,11,6,2}));
    }

    SECTION("generate proper move list for white pieces")
    {
        bitboard.setBlackMan(0);
        bitboard.setWhiteMan(generateBitboard({0}));
        auto moves = bitboard.getWhiteMoveList();
        REQUIRE(moves.size() == 1);
        REQUIRE(moves[0] == generateBitboard({0,4}));
        bitboard.applyWhiteMove(moves[0]);
        REQUIRE(bitboard.getWhiteMovers() == generateBitboard({4}));
        
        bitboard.setWhiteMan(0x2dff);
        bitboard.setBlackMan(0xffb40000);
        bitboard.setKings(0);
        auto jumpers = bitboard.getWhiteJumpers();
        REQUIRE(jumpers == generateBitboard({13}));
        REQUIRE_THAT(bitboard.getWhiteJumpList(jumpers), UnorderedEquals(std::vector<uint32_t>{
            generateBitboard({13,18,22})
        }));
    }

    SECTION("generate proper move list for kings")
    {
        //  K _ _ _ 28-31
        // _ _ K _  24-27
        //  _ _ _ K 20-23
        // _ K _ _  16-19
        //  _ _ K _ 12-15
        // K _ _ _  8 -11
        //  _ K _ _ 4 - 7
        // _ _ _ K  0 - 3
        bitboard.resetBoard();
        bitboard.setWhiteMan(generateBitboard({3,5,8,14,17,23,26,28}));
        bitboard.setBlackMan(0);
        bitboard.setKings(bitboard.getWhitePieces());

        REQUIRE(bitboard.getWhitePieces() == generateBitboard({3,5,8,14,17,23,26,28}));
        REQUIRE(bitboard.getWhiteMovers() == bitboard.getWhitePieces());
        REQUIRE_THAT(bitboard.getWhiteMoveList(), UnorderedEquals(std::vector<uint32_t>{
            generateBitboard({3,6}),
            generateBitboard({3,7}),

            generateBitboard({5,1}),
            generateBitboard({5,2}),
            generateBitboard({5,9}),
            generateBitboard({5,10}),

            generateBitboard({8,4}),
            generateBitboard({8,12}),
            
            generateBitboard({14,10}),
            generateBitboard({14,11}),
            generateBitboard({14,18}),
            generateBitboard({14,19}),

            generateBitboard({17,12}),
            generateBitboard({17,13}),
            generateBitboard({17,20}),
            generateBitboard({17,21}),

            generateBitboard({23,19}),
            generateBitboard({23,27}),

            generateBitboard({26,21}),
            generateBitboard({26,22}),
            generateBitboard({26,29}),
            generateBitboard({26,30}),

            generateBitboard({28,24}),
            generateBitboard({28,25})
        }));

    }

    SECTION("apply moves without changing state of opponent's pieces")
    {
        bitboard.resetBoard();
        bitboard.applyWhiteMove(generateBitboard({8,12}));
        REQUIRE(bitboard.getWhitePieces() == generateBitboard({0,1,2,3,4,5,6,7,9,10,11,12}));
        REQUIRE(bitboard.getBlackPieces() == generateBitboard({20,21,22,23,24,25,26,27,28,29,30,31}));
        REQUIRE(bitboard.getKings() == 0);
    }

    SECTION("promote white piece after move to last row")
    {
        bitboard.setBlackMan(0);
        bitboard.setWhiteMan(generateBitboard({24}));
        auto moves = bitboard.getWhiteMoveList();
        REQUIRE(moves.size() == 1);
        REQUIRE(moves[0] == generateBitboard({24,28}));
        bitboard.applyWhiteMove(moves[0]);
        REQUIRE(bitboard.getWhiteMovers() == generateBitboard({28}));
        REQUIRE(bitboard.getWhitePieces() == generateBitboard({28}));
        REQUIRE(bitboard.getKings() == generateBitboard({28}));
    }

    SECTION("promote black piece after move to last row")
    {
        bitboard.setBlackMan(generateBitboard({7}));
        bitboard.setWhiteMan(0);
        auto moves = bitboard.getBlackMoveList();
        REQUIRE(moves.size() == 1);
        REQUIRE(moves[0] == generateBitboard({7,3}));
        bitboard.applyBlackMove(moves[0]);
        REQUIRE(bitboard.getBlackMovers() == generateBitboard({3}));
        REQUIRE(bitboard.getBlackPieces() == generateBitboard({3}));
        REQUIRE(bitboard.getKings() == generateBitboard({3}));
    }

    SECTION("generate correct list of white man which can jump")
    {
        // jump right from odd row
        bitboard.setWhiteMan(generateBitboard({0,2,17,19}));
        bitboard.setBlackMan(generateBitboard({4,6,21,24}));
        REQUIRE(bitboard.getWhiteJumpers() == generateBitboard({0,2,17}));

        // jump left from odd row
        bitboard.setWhiteMan(generateBitboard({1,3,16,18}));
        bitboard.setBlackMan(generateBitboard({4,6,21,24}));
        REQUIRE(bitboard.getWhiteJumpers() == generateBitboard({1,3,18}));

        // jump right from even row
        bitboard.setWhiteMan(generateBitboard({4,6,21,24}));
        bitboard.setBlackMan(generateBitboard({9,11,26,28}));
        REQUIRE(bitboard.getWhiteJumpers() == generateBitboard({4,6,21}));

        // jump left from even row
        bitboard.setWhiteMan(generateBitboard({5,7,20,22}));
        bitboard.setBlackMan(generateBitboard({9,11,24,26}));
        REQUIRE(bitboard.getWhiteJumpers() == generateBitboard({5,7,22}));
    }

    SECTION("generate proper jump list for white pieces with singe and double jumps")
    {
        //  _ _ _ _ 28-31
        // _ _ _ _  24-27
        //  _ B B _ 20-23
        // _ _ W _  16-19
        //  B B _ W 12-15
        // _ W W _  8 -11
        //  _ W W W 4 - 7
        // _ _ _ _  0 - 3
        bitboard.setWhiteMan(generateBitboard({5,6,7,9,10,15,18}));
        bitboard.setBlackMan(generateBitboard({12,13,21,22}));
        bitboard.setKings(0);
        
        REQUIRE_THAT(bitboard.getWhiteJumpList(bitboard.getWhiteJumpers()), UnorderedEquals(std::vector<uint32_t>{
            generateBitboard({9,12,16}),
            generateBitboard({10,13,21,26}),
            generateBitboard({18,21,25}),
            generateBitboard({18,22,27}),
        }));
    }

    SECTION("generate proper jump list for white pieces with multiple branchy jumps")
    {
        //  _ _ _ _ 28-31
        // _ _ _ _  24-27
        //  B B B _ 20-23
        // _ _ _ _  16-19
        //  _ B _ _ 12-15
        // _ W W _  8 -11
        //  _ _ _ _ 4 - 7
        // _ _ _ _  0 - 3
        bitboard.setWhiteMan(generateBitboard({9,10}));
        bitboard.setBlackMan(generateBitboard({13,20,21,22}));
        bitboard.setKings(0);
        
        REQUIRE_THAT(bitboard.getWhiteJumpList(bitboard.getWhiteJumpers()), UnorderedEquals(std::vector<uint32_t>{
            generateBitboard({9,13,22,27}),
            generateBitboard({9,13,21,25}),
            generateBitboard({10,13,20,24}),
            generateBitboard({10,13,21,26})
        }));
    }

    SECTION("apply white multiple jumps properly and promote")
    {
        //  _ _ _ _ 28-31
        // _ _ B _  24-27
        //  _ _ _ _ 20-23
        // _ B _ _  16-19
        //  _ _ _ _ 12-15
        // _ B _ _  8 -11
        //  _ W _ _ 4 - 7
        // _ _ _ _  0 - 3
        bitboard.setWhiteMan(generateBitboard({5}));
        bitboard.setBlackMan(generateBitboard({9,17,26}));
        bitboard.setKings(0);
        std::cout << "genearting move list...\n";
        auto moveList = bitboard.getWhiteJumpList(bitboard.getWhiteJumpers());
        REQUIRE(moveList.size() == 1);
        auto move = moveList.front();
        REQUIRE(move == generateBitboard({5,9,17,26,30}));
        bitboard.applyWhiteMove(move);
        REQUIRE(bitboard.getWhitePieces() == generateBitboard({30}));
        REQUIRE(bitboard.getBlackPieces() == 0);
        REQUIRE(bitboard.getKings() == generateBitboard({30}));
    }

    SECTION("generate proper down jumps for white king")
    {
        //  _ _ _ _ 28-31
        // _ _ _ _  24-27
        //  _ _ _ _ 20-23
        // _ W _ _  16-19
        //  _ B _ _ 12-15
        // _ _ _ _  8 -11
        //  _ B _ _ 4 - 7
        // _ _ _ _  0 - 3
        bitboard.setWhiteMan(generateBitboard({17}));
        bitboard.setBlackMan(generateBitboard({13,5}));
        bitboard.setKings(generateBitboard({17}));
        
        REQUIRE_THAT(bitboard.getWhiteJumpList(bitboard.getWhiteJumpers()), UnorderedEquals(std::vector<uint32_t>{
            generateBitboard({17,13,5,1})
        }));
    }

    SECTION("generate proper jump list for white kings with multiple jumps")
    {
        //  _ _ _ _ 28-31
        // _ _ _ _  24-27
        //  _ _ _ _ 20-23
        // _ _ _ _  16-19
        //  _ B B _ 12-15
        // _ W _ _  8 -11
        //  _ _ B _ 4 - 7
        // _ _ _ _  0 - 3
        bitboard.setWhiteMan(generateBitboard({9}));
        bitboard.setBlackMan(generateBitboard({6,13,14}));
        bitboard.setKings(generateBitboard({9}));
        
        REQUIRE_THAT(bitboard.getWhiteJumpList(bitboard.getWhiteJumpers()), UnorderedEquals(std::vector<uint32_t>{
            generateBitboard({9,13,14,6,2})
        }));
    }

    SECTION("generate proper jump list for white kings with end at start point")
    {
        //  _ _ _ _ 28-31
        // _ _ _ _  24-27
        //  _ _ _ _ 20-23
        // _ _ _ _  16-19
        //  _ B B _ 12-15
        // _ W _ _  8 -11
        //  _ B B _ 4 - 7
        // _ _ _ _  0 - 3
        bitboard.setWhiteMan(generateBitboard({9}));
        bitboard.setBlackMan(generateBitboard({5,6,13,14}));
        bitboard.setKings(generateBitboard({9}));
        
        REQUIRE_THAT(bitboard.getWhiteJumpList(bitboard.getWhiteJumpers()), UnorderedEquals(std::vector<uint32_t>{
            generateBitboard({5,6,13,14}),
            generateBitboard({5,6,13,14})
        }));
    }

    SECTION("generate proper jump list for white kings with multiple branchy jumps")
    {
        //  _ _ _ _ 28-31
        // _ B B B  24-27
        //  W _ _ _ 20-23
        // _ B _ B  16-19
        //  _ _ _ _ 12-15
        // _ B B B  8 -11
        //  _ _ _ _ 4 - 7
        // _ _ _ _  0 - 3
        bitboard.setWhiteMan(generateBitboard({20}));
        bitboard.setBlackMan(generateBitboard({9,10,11,17,19,25,26,27}));
        bitboard.setKings(generateBitboard({20}));
        
        REQUIRE_THAT(bitboard.getWhiteJumpList(bitboard.getWhiteJumpers()), UnorderedEquals(std::vector<uint32_t>{
            generateBitboard({20,25,26,27,31}),
            generateBitboard({25,26,19,11,10,17}),
            generateBitboard({25,26,19,11,10,17}),
            generateBitboard({20,25,26,19,11,10,9,4}),
            generateBitboard({20,17,9,4}),
            generateBitboard({20,17,10,11,19,27,31})
        }));

        bitboard.applyWhiteMove(generateBitboard({25,26,19,11,10,17}));
        REQUIRE(bitboard.getWhitePieces() == generateBitboard({20}));
        REQUIRE(bitboard.getBlackPieces() == generateBitboard({9,27}));
    }

    SECTION("remove black man from the board when white man jumps")
    {
        bitboard.setBlackMan(generateBitboard({5}));
        bitboard.setWhiteMan(generateBitboard({1}));
        uint32_t move = generateBitboard({1,5,10});
        bitboard.applyWhiteMove(move);
        REQUIRE(bitboard.getBlackPieces() == 0);
        REQUIRE(bitboard.getWhitePieces() == generateBitboard({10}));
        REQUIRE(bitboard.getKings() == 0);
    }

    SECTION("remove white man from the board when black man jumps and promote")
    {
        bitboard.setBlackMan(generateBitboard({9}));
        bitboard.setWhiteMan(generateBitboard({5}));
        uint32_t move = generateBitboard({9,5,2});
        bitboard.applyBlackMove(move);
        REQUIRE(bitboard.getBlackPieces() == generateBitboard({2}));
        REQUIRE(bitboard.getWhitePieces() == 0);
        REQUIRE(bitboard.getKings() == bitboard.getBlackPieces());
    }

    SECTION("remove black man from the board when white king jumps")
    {
        bitboard.setBlackMan(generateBitboard({10}));
        bitboard.setWhiteMan(generateBitboard({13}));
        bitboard.setKings(generateBitboard({13}));
        uint32_t move = generateBitboard({13,10,6});
        bitboard.applyWhiteMove(move);
        REQUIRE(bitboard.getBlackPieces() == 0);
        REQUIRE(bitboard.getWhitePieces() == generateBitboard({6}));
        REQUIRE(bitboard.getKings() == bitboard.getWhitePieces());
    }

    SECTION("remove black king from the board when white king jumps")
    {
        bitboard.setBlackMan(generateBitboard({10}));
        bitboard.setWhiteMan(generateBitboard({13}));
        bitboard.setKings(generateBitboard({10,13}));
        uint32_t move = generateBitboard({13,10,6});
        bitboard.applyWhiteMove(move);
        REQUIRE(bitboard.getBlackPieces() == 0);
        REQUIRE(bitboard.getWhitePieces() == generateBitboard({6}));
        REQUIRE(bitboard.getKings() == bitboard.getWhitePieces());
    }
}

