#ifndef BLACK_PAWN_HPP
#define BLACK_PAWN_HPP

#include <cstdint>
#include "MoveUtils.hpp"


/*
* Basic Lowest Level Component of Legal Moves
*/

/*
* This class considers:
* 1. pawn pushes
* 2. double pawn pushes
* 3. pawn attacks
* 4. handling en passant pawn moves
*/

class BlackPawnMap {
    public:
    //--------------------
    // Initilizers
    //--------------------
    
    BlackPawnMap() = delete;

    //--------------------
    // Moves defined by Chess rules
    //--------------------

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getPushTargets(const uint64_t wPawns, const uint64_t fullBoard) { return wPawns >> 8 & MoveUtils::empty(fullBoard); }

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getDblPushTargets(const uint64_t wPawns, const uint64_t fullBoard) { return wPawns >> 16 & MoveUtils::empty(fullBoard); }

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getWestAttackTargets(const uint64_t wPawns, const uint64_t oponentPieces) { return wPawns & notAFile >> 9 & oponentPieces; }

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getEastAttackTargets(const uint64_t wPawns, const uint64_t oponentPieces) { return wPawns & notHFile >> 7 & oponentPieces; }

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getAnyAttackTargets(const uint64_t wPawns, const uint64_t oponentPieces) 
    { 
        return getEastAttackTargets(wPawns, oponentPieces) | getWestAttackTargets(wPawns, oponentPieces); 
    }

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getEpAttackTarget(const uint64_t wPawns, const uint64_t ep) { return (( wPawns >> 9) | (wPawns >> 7)) & (static_cast<uint64_t>(1) << ep); }

    // may be used in future by evaluation function

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getDblAttackTargets(const uint64_t wPawns, const uint64_t oponentPieces) { return getEastAttackTargets(wPawns, oponentPieces) & getWestAttackTargets(wPawns, oponentPieces); }

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getSingleAttackTargets(const uint64_t wPawns, const uint64_t oponentPieces) { return getEastAttackTargets(wPawns, oponentPieces) ^ getWestAttackTargets(wPawns, oponentPieces); }
    
    //------------------
    // Main API function
    //------------------

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getMoves(const uint64_t wPawns, const uint64_t fullBoard, const uint64_t oponentPieces, const uint64_t ep)
    {
        return getPushTargets(wPawns, fullBoard) | getDblPushTargets(wPawns, fullBoard) | getAnyAttackTargets(wPawns, oponentPieces) | getEpAttackTarget(wPawns, ep);
    }


    private:
    //--------------------
    // Helpers for attacking moves
    //--------------------

    static constexpr uint64_t notAFile = 0x7F7F7F7F7F7F7F7F;
    static constexpr uint64_t notHFile = 0xFEFEFEFEFEFEFEFE;
};

#endif 
