#include <pybind11/pybind11.h>

#include "MoveGeneration/Perft/PerftFunctions.h"
#include "MoveGeneration/Perft/PerftStats.h"
#include "Board.hpp"
#include "MoveGeneration/ChessRules.hpp"
#include "PieceMap.hpp"


namespace py = pybind11;


uint64_t run_perft_simple(const std::string fen, int depth) 
{    
    static bool is_global_init = false;
    if (!is_global_init) 
    {
        PieceMap::init();
        is_global_init = true;
    }

    Board board{};
    PerftStats stats{};
    ChessRules rules{board, stats};

    board.init();
    board.loadFromFEN(fen);

    return Perft(depth, rules, stats);
}

PYBIND11_MODULE( PyPerft, m, py::mod_gil_not_used() ) 
{
    m.doc() = "Perft Wrapper";
    
    m.def(
        "run_perft", 
        &run_perft_simple, 
        "return nodes number",
        py::arg("fen"), py::arg("depth")
    );
}
