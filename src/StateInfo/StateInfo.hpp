#ifndef STATE_INFO_HPP
#define STATE_INFO_HPP

#include <cstdint>
#include <stack>
#include <stdexcept>


struct posInfo {
    // stockFish also use pawnKey hash, materialKey hash pliesFromNull
    uint64_t hash;
    uint64_t from;
    uint64_t to;
    uint8_t castlingRights;
    uint8_t enPassant;
    uint8_t halfMoveClock;
    uint8_t capturedPiece;
};


class StateInfo
{
private:
    std::stack<posInfo> stateStack;

public:
    //--------Contructors-----------
    StateInfo() = default;
    ~StateInfo() = default;
    StateInfo(StateInfo &other) = default;
    StateInfo& operator=(const StateInfo& other) = default;

    //---------Methods---------------
    void addPos(const posInfo &positionInfo) { stateStack.push(positionInfo); }
    
    posInfo getPos() 
    { 
        posInfo tempPos;
        if (!stateStack.empty())
        { 
            tempPos = stateStack.top();
            stateStack.pop();
        }
        return tempPos;
    }

};

#endif