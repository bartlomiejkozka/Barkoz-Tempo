// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// UCI protocol interface
/*************************************************/

#include "Uci.h"
#include "MoveGeneration/MoveGenerator.h"
#include "MoveGeneration/Move.hpp"
#include "MoveParser.h"


void UCI::loop()
{
    std::string line;
    std::string token;
    
    std::cout.setf(std::ios::unitbuf);  // have to be, std::endl does not invoke flush in pipes case

    while ( std::getline(std::cin, line) )
    {
        std::istringstream ss(line);
        ss >> token;

        if (token == "uci") 
        {
            std::cout << "id name Barkoz-Tempo" << std::endl;
            std::cout << "id author BartlomiejKozka" << std::endl;
            std::cout << "option name Move Overhead type spin default 0 min 0 max 5000" << std::endl;
            std::cout << "option name Threads type spin default 1 min 1 max 4" << std::endl;            // not implemented
            std::cout << "option name Hash type spin default 16 min 1 max 1024" << std::endl;
            std::cout << "option name SyzygyPath type string default <empty>" << std::endl;             // not implemented
            std::cout << "option name UCI_ShowWDL type check default false" << std::endl;               // not implemented
            
            std::cout << "option name Ponder type check default false" << std::endl;                    // not implemented
            std::cout << "option name UCI_Chess960 type check default false" << std::endl;              // not implemented
            std::cout << "uciok" << std::endl;
        }
        else if (token == "isready") 
        {
            std::cout << "readyok" << std::endl;
        }
        else if (token == "setoption")
        {
            parseSetOption(ss);
        }
        else if (token == "ucinewgame")
        {
            searchEngine.clear();
        }
        else if (token == "position") 
        {
            parsePosition(ss);
        }
        else if (token == "go") 
        {
            parseGo(ss);
        } 
        else if (token == "stop")
        {
            searchEngine.stopRequest = true;
            if (searchThread.joinable()) searchThread.join();
        } 
        else if (token == "quit") 
        {
            searchEngine.stopRequest = true;
            if (searchThread.joinable()) searchThread.join();
            break;
        }
    }
}

void UCI::parseSetOption(std::istringstream& ss)
{
    std::string token, name, value;
    
    ss >> token;
    
    while (ss >> token && token != "value") {
        if (!name.empty()) name += " ";
        name += token;
    }

    if (name == "SyzygyPath") 
    {
        std::getline(ss, value); 
        if (!value.empty() && value[0] == ' ') 
        {
            value.erase(0, 1);
        }
        return; 
    }

    ss >> value;

    if (name == "Move Overhead") 
    {
        try {
            moveOverhead = std::stoi(value);
        } catch (...) {
            // ignore
        }
    }
    else if (name == "Threads" || name == "UCI_ShowWDL" || 
                name == "Ponder" || name == "UCI_Chess960")
    {
        // ignore, for now only 1 thread for search
    }
    else if (name == "Hash") 
    {
        searchEngine._TT.resize(std::stoi(value));
    }
}

void UCI::parsePosition(std::istringstream& ss) 
{
    std::string token;
    std::string fen;

    ss >> token;

    if (token == "startpos") 
    {
        rules._board.init();
        ss >> token;    // read remaning "moves"
    } 
    else if (token == "fen")
    {
        while (ss >> token && token != "moves") 
        {
            fen += token + " ";
        }
        rules._board.loadFromFEN(fen);
    }

    if (token == "moves") 
    {
        while (ss >> token) 
        {
            auto parsedMove = SimpleParser::parseMoveString(token);
            int from = parsedMove.from;
            int to = parsedMove.to;
            char promotion = parsedMove.promotion;

            MoveType moveType = MoveType::QUIET;
            std::array<Move, 256> moves;
            int movesCount = MoveGen::generateLegalMoves(rules, moves.data());
            bool moveFound = false;

            for (int i = 0; i < movesCount; ++i)
            {
                if (moves[i].OriginSq() == from && moves[i].TargetSq() == to)
                {
                    if (promotion != '\0')
                    {
                        MoveType promCap = SimpleParser::promotionStringToType<true>(promotion);
                        MoveType prom = SimpleParser::promotionStringToType<false>(promotion);
                        
                        if (moves[i].getType() == promCap || moves[i].getType() == prom)
                        {
                            moveType = moves[i].getType();
                            moveFound = true;
                            break;
                        }
                    }
                    else
                    {
                        moveType = moves[i].getType();
                        moveFound = true;
                        break;
                    }
                }
            }
            if (!moveFound) 
            {
                std::cerr << "Blad: Otrzymano nielegalny ruch UCI: " << token << std::endl;
                break;
            }

            Move mv = Move{from, to, moveType};
            rules._board.makeMove(mv);
        }
    }
}

void UCI::parseGo(std::istringstream& ss) 
{
    std::string token;
    int depth = -1;
    int wtime = 0, btime = 0, winc = 0, binc = 0;
    int movetime = -1;
    int movestogo = -1;

    while (ss >> token) 
    {
        if (token == "wtime") ss >> wtime;
        else if (token == "btime") ss >> btime;
        else if (token == "winc") ss >> winc;
        else if (token == "binc") ss >> binc;
        else if (token == "depth") ss >> depth;
        else if (token == "movetime") ss >> movetime;
        else if (token == "movestogo") ss >> movestogo;
    }

    int timeForMove = 0;
    
    if (movetime != -1) 
    {
        timeForMove = movetime - moveOverhead - 10; 
        if (timeForMove < 10) timeForMove = 10;
    } 
    else if (wtime > 0 || btime > 0)
    {
        bool isWhiteToMove = (rules._board.sideToMove == pColor::White);
        
        int myTime = isWhiteToMove ? wtime : btime;
        int myInc  = isWhiteToMove ? winc  : binc;

        myTime = myTime - moveOverhead;
        if (myTime < 0) myTime = 0;

        if (movestogo != -1)
        {
            int movesLeft = (movestogo > 30) ? 30 : (movestogo + 1); 
            timeForMove = (myTime / movesLeft) + myInc;
        }
        else
        {
            timeForMove = (myTime / 30) + myInc;
        }
        
        timeForMove -= 50; 
        if (timeForMove < 50) timeForMove = 50;
    }

    if (depth == -1 && timeForMove == 0) depth = 6;
    if (timeForMove > 0 && depth == -1) depth = 64;

    if (searchThread.joinable()) searchThread.join();

    searchEngine.stopRequest = false; 

    ChessRules rulesForThread = rules; 

    searchThread = std::thread([this, rulesForThread, depth, timeForMove]() mutable 
    {
        this->searchEngine.searchPosition(rulesForThread, depth, timeForMove);
    });
}
