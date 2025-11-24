#ifndef BLACK_PAWN_HPP
#define BLACK_PAWN_HPP

#include <cstdint>
#include <bit>

#include "MoveUtils.hpp"
#include "Board.hpp"
#include "BitOperation.hpp"

/*
* Basic Lowest Level Component of Legal Moves
*/

/*
* This class considers:
* 1. pawn pushes
* 2. double pawn pushes
* 3. pawn attacks
* 4. En passant strikes
*/

class BlackPawnMap {
    private:
    //----------------------------
    // Helpers for attacking moves
    //----------------------------
    static constexpr uint64_t notAFile =  0xFEFEFEFEFEFEFEFE;
    static constexpr uint64_t notHFile =  0x7F7F7F7F7F7F7F7F;
    static constexpr uint64_t notLast2Ranks = 0x00FFFFFFFFFFFFFF;
    static constexpr uint64_t rank7 =         0x00FF000000000000;

    public:
    //--------------------
    // Initilizers
    //--------------------
    
    BlackPawnMap() = delete;

    //--------------------
    // Moves defined by Chess rules
    //--------------------

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getPushTargets(const int originSq, const uint64_t fullBoard) { return bitBoardSet(originSq) >> 8 & MoveUtils::empty(fullBoard); }
   
    [[nodiscard("PURE FUN")]] static constexpr uint64_t getDblPushTargets(const int originSq, const uint64_t fullBoard) 
    { 
        if ( ((minBitSet << originSq) & rank7) && getPushTargets(originSq, fullBoard) )
        {
            return bitBoardSet(originSq) >> 16 & MoveUtils::empty(fullBoard); 
        }
        return 0ULL;
    }

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getWestAttackTargets(const int originSq, const uint64_t oponentPieces) { return ((bitBoardSet(originSq) & notAFile) >> 9) & oponentPieces; }

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getEastAttackTargets(const int originSq, const uint64_t oponentPieces) { return ((bitBoardSet(originSq) & notHFile) >> 7) & oponentPieces; }

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getAnyAttackTargets(const int originSq, const uint64_t oponentPieces) 
    { 
        return getEastAttackTargets(originSq, oponentPieces) | getWestAttackTargets(originSq, oponentPieces);
    }

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getEpAttackTarget(const int originSq, const int ep)
    {
        if (0 == ep) return 0ULL;
        return getAnyAttackTargets(originSq, bitBoardSet(ep));
    }

    // may be used in future by evaluation function
    // TODO[Low prior]: get to know what author meant above

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getDblAttackTargets(const uint64_t originSq, const uint64_t oponentPieces) { return getEastAttackTargets(originSq, oponentPieces) & getWestAttackTargets(originSq, oponentPieces); }

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getSingleAttackTargets(const uint64_t originSq, const uint64_t oponentPieces) { return getEastAttackTargets(originSq, oponentPieces) ^ getWestAttackTargets(originSq, oponentPieces); }
    
    //--------------------------
    // Main API function - moves
    //--------------------------

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
            if (pos & notLast2Ranks)
            {
                res[i] = ((pos & notAFile) << 7) | ((pos & notHFile) << 9);
            }
        }

        return res;
    }();
};

#endif 
