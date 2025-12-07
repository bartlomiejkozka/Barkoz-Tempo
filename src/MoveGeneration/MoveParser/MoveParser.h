#ifndef SIMPLEPARSER_HPP
#define SIMPLEPARSER_HPP

#include "MoveGeneration/Move.hpp"

#include <cstdint>
#include <string>
#include <optional>

class SimpleParser {
public:
    struct MoveParts {
        uint16_t from = 0;
        uint16_t to = 0;
        char promotion = '\0'; // 'q','r','b','n' or '\0'
        bool valid = false;
    };

    // Convert "a1".."h8" -> 0..63
    static std::optional<uint16_t> squareFromString(const std::string &sqstr);

    // Convert 0..63 -> "a1".."h8"
    static std::string squareToString(uint16_t sq);

    // Parse UCI move ("a2a4", "e7e8q", case-insensitive)
    static MoveParts parseMoveString(const std::string &moveStr);

    // Convert structured move to UCI string
    static std::string moveToString(uint16_t from, uint16_t to, char promotion = '\0');

    static MoveType promotionStringToType(char promotion);
};

#endif // SIMPLEPARSER_HPP
