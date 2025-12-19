## Barkoz Tempo
#### chess engine

![CI](https://github.com/bartlomiejkozka/Barkoz-Tempo/actions/workflows/ci.yml/badge.svg)

Barkoz Tempo is a chess engine under development.

### Current Status

The estimated ELO rating of the engine is currently over **1800**.

Ranking is estimated by performing tournaments with other chess engines. \
(For now performing against StockFish with limitedElo to 1320-2000)

### Development Roadmap

- [ ] Add an opening book.
- [ ] Improve the evaluation function.
- [ ] Improve time management.

### Key Features

- **Board Representation** <br>
The board is represented using **bitboards**. Each piece type for each color has its own 64-bit integer, which allows for efficient move generation and board state updates.
- **Board Evaluation** <br> 
    - **Material Balance**: The raw value of pieces on the board.
    - **Piece-Square Tables (PST) with Tapered Evaluation**: Pre-calculated tables that evaluate the positional quality of a piece's location. The engine utilizes Tapered Evaluation, interpolating linearly between two distinct sets of tables (Middlegame and Endgame) based on the current game phase.
    - **Piece Mobility**: The number of legal moves available for each side, rewarding active play.
- **Best Move Search** <br> 
The engine uses **Iterative Deepening** combined with an **Alpha-Beta** pruning algorithm. To mitigate the "horizon effect," a **Quiescence Search** is performed at leaf nodes. This extends the search by examining only capture moves until a "quiet" position is reached, ensuring tactical sequences are fully resolved before evaluation.
- **Move Ordering** <br> 
To improve search efficiency, moves are sorted using several techniques. The primary heuristic is **MVV-LVA (Most Valuable Victim - Least Valuable Attacker)**. Additionally, the best move found in the previous iteration of iterative deepening is always searched first.
- **UCI (Universal Chess Interface)** <br>
The engine communicates using the UCI protocol. Key supported commands include `uci`, `isready`, `position`, and `go`.
- **Time Management** <br> 
Basic time management is implemented, allowing the engine to adjust its search depth based on the time remaining for the game (`wtime`/`btime`) and `movetime` constraints.

### How to Run

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/bartlomiejkozka/Barkoz-Tempo.git
    cd Barkoz-Tempo
    ```

2.  **Build the project:**
    The project uses CMake. To build it, create a `build` directory and run CMake.
    ```bash
    mkdir build && cd build
    cmake ..
    cmake --build .
    ```

3.  **Run the engine:**
    After building, the executable will be in the `build` directory. You can start communicating with it via the UCI protocol.

    The engine can also be connected to any UCI-compatible chess GUI, such as Cute Chess.

    > **Note:** Not all UCI commands are fully supported yet.

### Testing

The correctness of the move generator is primarily verified using **Perft (Performance Testing)**. This involves counting the total number of leaf nodes in the move generation tree up to a certain depth to ensure it matches known correct values.

To run tests, you need to compile the project with the `BUILD_PYTHON_BINDINGS=ON`
