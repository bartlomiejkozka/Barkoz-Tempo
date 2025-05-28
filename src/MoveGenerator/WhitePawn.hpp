#ifndef BLACK_PAWN_HPP
#define BLACK_PAWN_HPP

#include <cstdint>
#include "MoveUtils.hpp"


// TODO:
// 1. Consider change this to be more abstractive, but not at the sacrifice of perf


/*
* Class conception is the same as BlackPawn see there to get more info
* For now two similar classes to have the best perf
*/

class WhitePawn {
    public:
    //--------------------
    // Initilizers
    //--------------------
    
    WhitePawn() = delete;s

    //--------------------
    // Moves defined by Chess rules
    //--------------------

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getPushTargets(const uint64_t bPawns, const uint64_t fullBoard) { return bPawns << 8 & MoveUtils::empty(fullBoard); }

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getDblPushTargets(const uint64_t bPawns, const uint64_t fullBoard) { return bPawns << 16 & MoveUtils::empty(fullBoard); }

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getWestAttackTargets(const uint64_t bPawns, const uint64_t oponentPieces) { return bPawns & notAFile << 9 & oponentPieces; }

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getEastAttackTargets(const uint64_t bPawns, const uint64_t oponentPieces) { return bPawns & notHFile << 7 & oponentPieces; }

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getAnyAttackTargets(const uint64_t bPawns, const uint64_t oponentPieces) 
    { 
        return getEastAttackTargets(oponentPieces) | getWestAttackTargets(oponentPieces); 
    }

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getEpAttackTarget(const uint64_t bPawns, const uint64_t ep) { return (( bPawns << 9) | (bPawns << 7)) & (static_cast<uint64_t>(1) << ep); }

    // may be used in future by evaluation function

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getDblAttackTargets(const uint64_t bPawns, const uint64_t oponentPieces) { return getEastAttackTargets & getWestAttackTargets; }

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getSingleAttackTargets(const uint64_t bPawns, const uint64_t oponentPieces) { return getEastAttackTargets ^ getWestAttackTargets; }
    
    //------------------
    // Main API function
    //------------------
    
    [[nodiscard("PURE FUN")]] static constexpr uint64_t getMoves(const uint64_t bPawns, const uint64_t fullBoard, const uint64_t oponentPieces, const uint64_t ep)
    {
        return getPushTargets(bPawns, fullBoard) | getDblPushTargets(bPawns, fullBoard) | getAnyAttackTargets(bPawns, oponentPieces) | getEpAttackTarget(bPawns, ep);
    }


    private:
    //--------------------
    // Helpers for attacking moves
    //--------------------

    static constexpr uint64_t notAFile = 0x7F7F7F7F7F7F7F7F;
    static constexpr uint64_t notHFile = 0xFEFEFEFEFEFEFEFE;
};

#endif 
