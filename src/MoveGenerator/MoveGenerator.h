// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// Class for moves generation - used by ChessRules, which is considered as Higher Level Interface
/*************************************************/

#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include "Move.hpp"
#include "Board.hpp"

#include <utility>


enum class Gen : size_t
{
    Captures,
    Quiets,
    Evasions,
    All
}

class MoveGen
{
public:
    // -------------------------
    // Main API move generation.
    // -------------------------

    template<Gen GenMode>
    [[nodiscard]] Move* generate(Board &_board, Move *moves)
    {
        if constexpr (GenMode == Gen::Captures)
        {
            return genCaptures(Board &_board, Move *moves);
        }
        
        if constexpr (GenMode == Gen::Quiets)
        {
            return genQuiets(Board &_board, Move *moves);
        }

        if constexpr (GenMode == Gen::Evasions)
        {
            return genEvasions(Board &_board, Move *moves);
        }

        if constexpr (GenMode == Gen::All)
        {
            return genAll(Board &_board, Move *moves);
        }

        std::unreachable();
        return moves;
    }

private:
    // -------------------------
    // Piece Types Helpers
    // -------------------------

    [[nodiscard]] Move* getKingMove(Board &_board, Move *moves);

    [[nodiscard]] Move* getKnightMove(Board &_board, Move *moves);

    // -------------------------
    // Gen Types Helpers
    // -------------------------

    [[nodiscard]] Move* genCaptures(Board &_board, Move *moves);

    [[nodiscard]] Move* genQuiets(Board &_board, Move *moves);

    [[nodiscard]] Move* genEvasions(Board &_board, Move *moves);

    [[nodiscard]] Move* genAll(Board &_board, Move *moves);
};

#endif  // MOVE_GENERATOR_H
