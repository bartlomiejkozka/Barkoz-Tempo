// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// UCI protocol interface
/*************************************************/

#ifndef UCI_H
#define UCI_H

#include "Board.hpp"
#include "Engine/Search.h"

#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <utility>


// TODO: Change this module to use predefined variables not hard-coded values

class UCI
{
public:
    ChessRules &rules;
    Search &searchEngine;
    std::thread searchThread;

    // -----------------------
    // Initialization
    // -----------------------
    
    UCI(ChessRules &r, Search &s)
        : rules{r}, searchEngine{s} {}
    
    void loop();

private:
    void parsePosition(std::istringstream& ss);

    void parseGo(std::istringstream& ss);
};

#endif
