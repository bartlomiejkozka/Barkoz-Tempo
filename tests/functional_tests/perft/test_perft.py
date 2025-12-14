import pytest
import PyPerft

# ---------------------------------------------------------------------------
# Perft Test Suite
# Each function tests one position from depth 1 to N.
# Results (node counts) are sourced from the standard Chess Programming Wiki database.
# ---------------------------------------------------------------------------

def test_position_1_initial():
    """
    Initial position.
    As requested, using an empty string "" as FEN for the start position.
    Max depth: 6
    """
    fen = "" 
    # Map: {depth: expected_node_count}
    expected_results = {
        1: 20,
        2: 400,
        3: 8902,
        4: 197281,
        5: 4865609,
        6: 119060324,
        # 7: 3195901860
    }

    # Loop from 1 to 6 inclusive
    for depth in range(1, len(expected_results)+1):
        result = PyPerft.run_perft(fen, depth)
        assert result == expected_results[depth], \
            f"Error for Start Pos at depth {depth}. Expected {expected_results[depth]}, got {result}"


def test_position_2_kiwipete():
    """
    Position 2 (aka 'KiwiPete').
    Great for detecting move generation bugs (especially captures).
    Max depth: 5
    """
    fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
    expected_results = {
        1: 48,
        2: 2039,
        3: 97862,
        4: 4085603,
        5: 193690690,
        # 6: 8031647685
    }

    for depth in range(1, len(expected_results)+1):
        result = PyPerft.run_perft(fen, depth)
        assert result == expected_results[depth], \
            f"Error for KiwiPete at depth {depth}. Expected {expected_results[depth]}, got {result}"


def test_position_3_endgame_promotions():
    """
    Position 3.
    Tests promotions, en passant captures, and endgame checks.
    Max depth: 7
    """
    fen = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1"
    expected_results = {
        1: 14,
        2: 191,
        3: 2812,
        4: 43238,
        5: 674624,
        6: 11030083,
        7: 178633661,
        # 8: 3009794393
    }

    for depth in range(1, len(expected_results)+1):
        result = PyPerft.run_perft(fen, depth)
        assert result == expected_results[depth], \
            f"Error for Pos 3 at depth {depth}. Expected {expected_results[depth]}, got {result}"


def test_position_4_checks_and_pins():
    """
    Position 4.
    Complex middlegame position, lots of pins and discovered checks.
    Max depth: 5
    """
    fen = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"
    expected_results = {
        1: 6,
        2: 264,
        3: 9467,
        4: 422333,
        5: 15833292,
        # 6: 706045033
    }

    for depth in range(1, len(expected_results)+1):
        result = PyPerft.run_perft(fen, depth)
        assert result == expected_results[depth], \
            f"Error for Pos 4 at depth {depth}. Expected {expected_results[depth]}, got {result}"


def test_position_5_endgame_nasty():
    """
    Position 5.
    Known for bugs regarding promotions and strange king interactions.
    Max depth: 5
    """
    fen = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"
    expected_results = {
        1: 44,
        2: 1486,
        3: 62379,
        4: 2103487,
        5: 89941194
    }

    for depth in range(1, 6):
        result = PyPerft.run_perft(fen, depth)
        assert result == expected_results[depth], \
            f"Error for Pos 5 at depth {depth}. Expected {expected_results[depth]}, got {result}"


def test_position_6_middle_game():
    """
    Position 6.
    Standard middle game.
    Max depth: 5
    """
    fen = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"
    expected_results = {
        1: 46,
        2: 2079,
        3: 89890,
        4: 3894594,
        5: 164075551,
        # 6: 6923051137
    }

    for depth in range(1, len(expected_results)+1):
        result = PyPerft.run_perft(fen, depth)
        assert result == expected_results[depth], \
            f"Error for Pos 6 at depth {depth}. Expected {expected_results[depth]}, got {result}"
