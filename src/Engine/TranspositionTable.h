// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// Main search engine
/*************************************************/

#ifndef TRANSPOSITION_TABLE_H
#define TRANSPOSITION_TABLE_H

#include "MoveGeneration/Move.hpp"

#include <vector>
#include <cstdint>
#include <iostream>
#include <algorithm>


class TranspositionTable 
{
public:
    struct Entry {
        enum class Type
        {
            EXACT,
            LOWERBOUND,
            UPPERBOUND
        };
        
        uint64_t key = 0;
        Move move = Move{0};
        int score = 0;
        int depth = 0;
        Type type = Type::EXACT;

        bool isValid() const { return key != 0; }
    };

    // ------------------------
    // Initialization
    // ------------------------

    explicit TranspositionTable(int sizeInMB = 64)
    {
        resize(sizeInMB);
    }

    // ------------------------
    // Modification
    // ------------------------

    // !!! Have to be called after/in TT contructor to init size of TT
    void resize(int sizeInMB)
    {
        size_t entrySize = sizeof(Entry);        
        size_t count = (sizeInMB * 1024 * 1024) / entrySize;    // 64MB ~ 1 milion idx's

        size_t powerOf2 = 1;
        while (powerOf2 * 2 <= count) {
            powerOf2 *= 2;
        }

        table.resize(powerOf2);
        table.shrink_to_fit();
        
        // 5. Maska do indeksowania (zamiast modulo)
        // Np. dla rozmiaru 4096 (binarnie 1000000000000)
        // maska to 4095 (binarnie 0111111111111)
        mask = powerOf2 - 1;

        std::cout << "TT initialized with " << powerOf2 << " entries (" 
                  << (powerOf2 * entrySize) / (1024*1024) << " MB)" << std::endl;
        
        clear();
    }

    void clear() 
    {
        std::fill(table.begin(), table.end(), Entry{});
    }

    void save(uint64_t key, int depth, int score, Entry::Type type, Move move)
    {
        size_t index = key & mask;

        Entry& entry = table[index];

        bool replace = (entry.key == 0) || 
                       (entry.depth <= depth); 

        if (replace) 
        {
            if (static_cast<uint16_t>(move.getPackedMove()) == 0)
            {
                move = entry.move;
            }

            entry.key = key;
            entry.depth = depth;
            entry.score = score;
            entry.type = type;
            entry.move = move;
        }
    }

    Entry probe(uint64_t key) 
    {
        size_t index = key & mask;
        Entry e = table[index];

        if (e.key == key) {
            return e;
        }

        return Entry{}; 
    }

private:
    std::vector<Entry> table;
    size_t mask = 0;
};

#endif
