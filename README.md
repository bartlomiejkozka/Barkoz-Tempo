## Barkoz Tempo
#### chess engine

![CI](https://github.com/bartlomiejkozka/Barkoz-Tempo/actions/workflows/ci.yml/badge.svg)

Barkoz Tempo is a chess engine under development.

### Current Status

The estimated ELO rating of the engine is currently around **1200**.

### Development Roadmap

- [ ] Improve the evaluation function.
- [ ] Add an opening book.
- [ ] Expand tests to cover all possible remaining positions/cases.
- [ ] Improve time management.

### Key Features

- **Board Representation** <br>
The board is represented using **bitboards**. Each piece type for each color has its own 64-bit integer, which allows for efficient move generation and board state updates.
- **Board Evaluation** <br> 
The evaluation function calculates a score based on two main factors: **material balance** and **piece mobility** (the number of legal moves available for each side).
- **Best Move Search** <br> 
The engine uses **Iterative Deepening** combined with an **Alpha-Beta pruning** algorithm. This allows the engine to manage its time effectively and provide a good move even if the search is interrupted before completion.
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
