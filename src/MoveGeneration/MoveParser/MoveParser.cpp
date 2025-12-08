#include "MoveParser.h"
#include <algorithm>
#include <cctype>

//
// Convert "a1" -> 0 .. "h8" -> 63
//
std::optional<uint16_t> SimpleParser::squareFromString(const std::string &sqstr) 
{
    if (sqstr.size() != 2)
        return std::nullopt;

    char f = std::tolower(static_cast<unsigned char>(sqstr[0]));
    char r = sqstr[1];

    if (f < 'a' || f > 'h')
        return std::nullopt;
    if (r < '1' || r > '8')
        return std::nullopt;

    int file = f - 'a';      // 0..7
    int rank = r - '1';      // 0..7

    uint16_t sq = static_cast<uint16_t>(rank * 8 + file);
    return sq;
}

//
// Convert 0..63 -> "a1".."h8"
//
std::string SimpleParser::squareToString(uint16_t sq) 
{
    if (sq >= 64)
        return {};

    int file = sq % 8;
    int rank = sq / 8;

    std::string s;
    s.reserve(2);
    s.push_back(static_cast<char>('a' + file));
    s.push_back(static_cast<char>('1' + rank));
    return s;
}

//
// Parse UCI move "a2a4" or "e7e8q"
// Returns MoveParts.valid = true on success
//
SimpleParser::MoveParts SimpleParser::parseMoveString(const std::string &moveStr) 
{
    MoveParts out;
    std::string s = moveStr;

    // remove any whitespace
    s.erase(std::remove_if(s.begin(), s.end(),
                           [](unsigned char c){ return std::isspace(c); }),
            s.end());

    // lowercase for uniform processing
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return static_cast<char>(std::tolower(c)); });

    if (s.size() < 4)
        return out;

    auto fromOpt = squareFromString(s.substr(0,2));
    auto toOpt   = squareFromString(s.substr(2,2));

    if (!fromOpt.has_value() || !toOpt.has_value())
        return out;

    out.from = *fromOpt;
    out.to   = *toOpt;

    // Promotion piece
    if (s.size() >= 5) {
        char p = s[4];
        if (p=='q' || p=='r' || p=='b' || p=='n')
            out.promotion = p;
        else
            return out; // invalid promotion
    }

    out.valid = true;
    return out;
}

//
// Convert structured move into UCI "a2a4" or "e7e8q"
//
std::string SimpleParser::moveToString(uint16_t from, uint16_t to, char promotion) 
{
    if (from >= 64 || to >= 64)
        return {};

    std::string s;
    s.reserve(5);

    s += squareToString(from);
    s += squareToString(to);

    if (promotion != '\0') 
    {
        char p = static_cast<char>(std::tolower(static_cast<unsigned char>(promotion)));
        if (p == 'q' || p == 'r' || p == 'b' || p == 'n')
            s.push_back(p);
        else
            return {}; // invalid promotion character
    }

    return s;
}
