// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// Main search engine
/*************************************************/

#ifndef SEARCH_H
#define SEARCH_H

#include "MoveGeneration/ChessRules.hpp"


[[nodiscard]] int negaMax(int depth, ChessRules &rules, int alpha, int beta);

#endif
