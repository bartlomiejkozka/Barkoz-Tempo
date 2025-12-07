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
            std::cout << "uciok" << std::endl;
        }
        else if (token == "isready") 
        {
            std::cout << "readyok" << std::endl;
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

            MoveType moveType;
            std::array<Move, 256> moves;
            MoveGen::generateLegalMoves(rules, moves.data());
            bool moveFound = false;

            for (auto move : moves)
            {
                if (move.OriginSq() == from && move.TargetSq() == to)
                {
                    if (promotion != '\0')
                    {
                        MoveType promCap = SimpleParser::promotionStringToType<true>(promotion);
                        MoveType prom = SimpleParser::promotionStringToType<false>(promotion);
                        
                        if (move.getType() == promCap || move.getType() == prom)
                        {
                            moveType = move.getType();
                            moveFound = true;
                            break;
                        }
                    }
                    else
                    {
                        moveType = move.getType();
                        moveFound = true;
                        break;
                    }
                }
            }
            if (!moveFound) std::unreachable();

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

    while (ss >> token) 
    {
        if (token == "wtime") ss >> wtime;
        else if (token == "btime") ss >> btime;
        else if (token == "winc") ss >> winc;
        else if (token == "binc") ss >> binc;
        else if (token == "depth") ss >> depth;
        else if (token == "movetime") ss >> movetime;
    }

    int timeForMove = 0;
    
    if (movetime != -1) 
    {
        timeForMove = movetime;
    } 
    else if (wtime > 0 || btime > 0)
    {
        bool isWhiteToMove = (rules._board.sideToMove == pColor::White);
        
        int myTime = isWhiteToMove ? wtime : btime;
        int myInc  = isWhiteToMove ? winc  : binc;

        timeForMove = (myTime / 30) + myInc - 50;
        
        if (timeForMove < 50) timeForMove = 50;
    }

    // set default depth, when time and depth not passed
    if (depth == -1 && timeForMove == 0) depth = 6;

    // if we play for time, depth should be INF
    if (timeForMove > 0 && depth == -1) depth = 64;

    // search thread handling

    if (searchThread.joinable()) searchThread.join();

    searchEngine.stopRequest = false; 

    ChessRules rulesForThread = rules; 

    searchThread = std::thread([this, rulesForThread, depth, timeForMove]() mutable 
    {
        this->searchEngine.searchPosition(rulesForThread, depth, timeForMove);
    });
}
