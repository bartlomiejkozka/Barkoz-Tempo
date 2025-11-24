#ifndef PERFT_FUNCTIONS_H
#define PERFT_FUNCTIONS_H

#include "PerftStats.h"
#include "MoveGeneration/ChessRules.hpp"

#include <array>
#include <cstdint>


uint64_t Perft(int depth, ChessRules &rules, PerftStats &stats);

uint64_t PerftDivide(int depth, ChessRules &rules, PerftStats &stats);


#endif
