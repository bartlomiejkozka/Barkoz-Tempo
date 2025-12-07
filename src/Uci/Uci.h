// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// Main search engine
/*************************************************/

#ifndef UCI_H
#define UCI_H

#include "Board.hpp"
#include "Engine/Search.h"
#include "MoveGeneration/MoveGenerator.h"
#include "MoveGeneration/Move.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <thread>


// TODO: Change this module to use predefined variables not hard-coded values

class UCI
{
public:
    Board &rules;
    Search &engine;
    std::thread searchThread;

    void loop()
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
                engine.clear();
            }
            else if (token == "position") 
            {
                parsePosition(ss);
            }
            else if (token == "go") {
                parseGo(ss);
            } 
            else if (token == "stop") {
                stopSearch = true;
                if (searchThread.joinable()) searchThread.join();
            } 
            else if (token == "quit") {
                stopSearch = true;
                if (searchThread.joinable()) searchThread.join();
                break;
            }
        }
    }

private:
    void parsePosition(std::istringstream& ss) 
    {
        std::string token;
        std::string fen;

        ss >> token;

        if (token == "startpos") 
        {
            rules._board.init();
            ss >> token; // wczytaj ewentualne "moves"
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
                int from = SimpleParser::parseMoveString(token).from;
                int to = SimpleParser::parseMoveString(token).to;
                char promotion = SimpleParser::parseMoveString(token).promotion;
                MoveType moveType;
                std::array<Move, 256> moves;
                MoveGen::generateLegalMoves(rules, moves.data());
                for (auto move : moves)
                {
                    if (move.OriginSq() == from && move.TargetSq() == to)
                    {
                        moveType = move.getType();
                    }
                }
                rules._board.makeMove(Move{from, to, moveType});
            }
        }
    }

    void parseGo(std::istringstream& ss) {
        std::string token;
        int depth = -1;
        int wtime = 0, btime = 0, winc = 0, binc = 0;
        int movetime = -1;

        // Parsowanie parametrów czasu
        while (ss >> token) {
            if (token == "wtime") ss >> wtime;
            else if (token == "btime") ss >> btime;
            else if (token == "winc") ss >> winc;
            else if (token == "binc") ss >> binc;
            else if (token == "depth") ss >> depth;
            else if (token == "movetime") ss >> movetime;
        }

        // Proste zarządzanie czasem (DLA PRZYKŁADU):
        // Zakładamy, że gramy zawsze jako strona, której czas jest analizowany
        // W prawdziwym silniku musisz wiedzieć, czyj jest ruch (SideToMove)
        int timeForMove = 0;
        
        // Jeśli podano movetime, używamy go
        if (movetime != -1) {
            timeForMove = movetime;
        } 
        // Jeśli gramy na czas (wtime/btime), obliczamy np. 1/30 pozostałego czasu
        else if (wtime > 0) { 
            // Tu uproszczenie: bierzemy wtime, ale silnik powinien sprawdzać czyj ruch
             timeForMove = wtime / 30; 
        }

        // Ważne: Search musi być w osobnym wątku, żeby "stop" zadziałało
        if (searchThread.joinable()) searchThread.join();
        
        // Kopiujemy planszę, żeby search nie mieszał w głównej planszy GUI podczas myślenia
        // (chyba że masz pewność że GUI nie wyśle komend w trakcie 'go')
        Board boardForSearch = board; 
        
        searchThread = std::thread(runSearch, depth, timeForMove, boardForSearch);
    }
};

#endif
