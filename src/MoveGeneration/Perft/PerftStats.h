#ifndef PERFT_STATS_H
#define PERFT_STATS_H

#include <cstdint>
#include <iostream>
#include <iomanip>

struct PerftStats 
{
    uint64_t nodes = 0;
    uint64_t captures = 0;
    uint64_t promotions = 0;
    uint64_t castles = 0;
    uint64_t enPassantCaptures = 0;

    // these below are updated in ChessRules class by reference member to this structure
    uint64_t checks = 0;
    uint64_t discovery_checks = 0;
    uint64_t double_checks = 0;
    uint64_t check_mates = 0;

    void reset() {
        nodes = captures = promotions = castles = enPassantCaptures = 
            checks = discovery_checks = double_checks = check_mates = 0;
    }

    void print(std::ostream& os = std::cout) const 
    {
        os << "=== Perft Stats ===\n";

        auto printLine = [&](const char* name, uint64_t value) {
            os << "  " << std::left << std::setw(20) << name << ": " << value << "\n";
        };

        printLine("Nodes", nodes);
        printLine("Captures", captures);
        printLine("Promotions", promotions);
        printLine("Castles", castles);
        printLine("En-passant", enPassantCaptures);

        printLine("Checks", checks);
        printLine("Discovery checks", discovery_checks);
        printLine("Double checks", double_checks);
        printLine("Checkmates", check_mates);

        os << "====================\n";
    }
};


#endif
