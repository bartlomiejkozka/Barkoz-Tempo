#ifndef WHITE_PAWN_HPP
#define WHITE_PAWN_HPP

#include <cstdint>
#include <bit>

#include "MoveUtils.hpp"


// TODO:
// 1. Consider change this to be more abstractive, but not at the sacrifice of perf -> refactor to static polymorphism


/*
* Class conception is the same as BlackPawn see there to get more info
* For now two similar classes to have the best perf
*/

class WhitePawnMap {
    private:
    //--------------------
    // Helpers for attacking moves
    //--------------------

    static constexpr uint64_t notAFile =  0xFEFEFEFEFEFEFEFE;
    static constexpr uint64_t notHFile =  0x7F7F7F7F7F7F7F7F;
    static constexpr uint64_t notFirst2Ranks = 0xFFFFFFFFFFFFFF00;
    static constexpr uint64_t rank2 =          0x000000000000FF00;


    public:
    //--------------------
    // Initilizers
    //--------------------
    
    WhitePawnMap() = delete;

    //--------------------
    // Moves defined by Chess rules
    //--------------------

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getPushTargets(const int originSq, const uint64_t fullBoard) { return bitBoardSet(originSq) << 8 & MoveUtils::empty(fullBoard); }

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getDblPushTargets(const int originSq, const uint64_t fullBoard) 
    { 
        if ( ((minBitSet << originSq) & rank2) && getPushTargets(originSq, fullBoard) )
        {
            return bitBoardSet(originSq) << 16 & MoveUtils::empty(fullBoard); 
        }
        return 0ULL;
    }

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getWestAttackTargets(const int originSq, const uint64_t oponentPieces) { return ((bitBoardSet(originSq) & notAFile) << 7) & oponentPieces; }

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getEastAttackTargets(const int originSq, const uint64_t oponentPieces) { return ((bitBoardSet(originSq) & notHFile) << 9) & oponentPieces; }

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getAnyAttackTargets(const int originSq, const uint64_t oponentPieces) 
    { 
        return getEastAttackTargets(originSq, oponentPieces) | getWestAttackTargets(originSq, oponentPieces);
    }

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getEpAttackTarget(const int originSq, const int ep) 
    { 
        if (-1 == ep) return 0ULL;
        return getAnyAttackTargets(originSq, bitBoardSet(ep));
    }

    // may be used in future by evaluation function

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getDblAttackTargets(const uint64_t originSq, const uint64_t oponentPieces) { return getEastAttackTargets(originSq, oponentPieces) & getWestAttackTargets(originSq, oponentPieces); }
 
    [[nodiscard("PURE FUN")]] static constexpr uint64_t getSingleAttackTargets(const uint64_t originSq, const uint64_t oponentPieces) { return getEastAttackTargets(originSq, oponentPieces) ^ getWestAttackTargets(originSq, oponentPieces); }
    
    //------------------s
    // Main API function
    //------------------
    
    [[nodiscard("PURE FUN")]] static constexpr uint64_t getMoves(const uint64_t originSq, const uint64_t fullBoard, const uint64_t oponentPieces, const uint64_t ep)
    {
        return getPushTargets(originSq, fullBoard) | getDblPushTargets(originSq, fullBoard) | getAnyAttackTargets(originSq, oponentPieces) | getEpAttackTarget(originSq, ep);
    }

    //--------------------------
    // Main API function - table
    //--------------------------

    // Ep exluded -> in general case (King check) Ep not used
    static constexpr std::array<uint64_t, Board::boardSize> attacksTo = [] constexpr
    {
        std::array<uint64_t, Board::boardSize> res{};

        for (int i = 0; i < Board::boardSize; ++i)
        {
            uint64_t pos = 1ULL << i;
            if (pos & notFirst2Ranks)
            {
                res[i] = ((pos & notHFile) >> 7) | ((pos & notAFile) >> 9);
            }
        }

        return res;
    }();
};

#endif 
