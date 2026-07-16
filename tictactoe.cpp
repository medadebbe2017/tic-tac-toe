// ============================================================
//  tictactoe.cpp  —  Tic-Tac-Toe Game Engine
//  Author : Mohamed
//  Build  : g++ -std=c++17 -o tictactoe tictactoe.cpp
//  Usage  : ./tictactoe
// ============================================================

#include <iostream>
#include <array>
#include <string>
#include <sstream>
#include <limits>

// ── Cell state ───────────────────────────────────────────────
enum class Cell { Empty, X, O };

// ── Game result ──────────────────────────────────────────────
enum class GameResult { Ongoing, WinX, WinO, Draw };

// ── Win-line (which three cells formed the win) ──────────────
struct WinLine {
    int a, b, c;   // indices 0-8 into the flat board array
    bool valid;    // false = no winner yet
};

// ────────────────────────────────────────────────────────────
//  Board  — owns all game logic
// ────────────────────────────────────────────────────────────
class Board {
public:
    static constexpr int SIZE = 9;

    // All eight possible winning combinations
    static constexpr std::array<std::array<int,3>, 8> WIN_LINES = {{
        {0,1,2}, {3,4,5}, {6,7,8},   // rows
        {0,3,6}, {1,4,7}, {2,5,8},   // columns
        {0,4,8}, {2,4,6}             // diagonals
    }};

    Board() { reset(); }

    // Reset board to initial state
    void reset() {
        cells_.fill(Cell::Empty);
        move_count_ = 0;
    }

    // Attempt a move; returns true if the cell was empty
    bool place(int index, Cell player) {
        if (index < 0 || index >= SIZE) return false;
        if (cells_[index] != Cell::Empty) return false;
        cells_[index] = player;
        ++move_count_;
        return true;
    }

    // Check current result
    GameResult result() const {
        WinLine w = winning_line();
        if (w.valid) {
            return (cells_[w.a] == Cell::X) ? GameResult::WinX : GameResult::WinO;
        }
        if (move_count_ == SIZE) return GameResult::Draw;
        return GameResult::Ongoing;
    }

    // Return which three cells form a win (invalid if none)
    WinLine winning_line() const {
        for (auto& line : WIN_LINES) {
            Cell c = cells_[line[0]];
            if (c != Cell::Empty &&
                c == cells_[line[1]] &&
                c == cells_[line[2]]) {
                return { line[0], line[1], line[2], true };
            }
        }
        return { 0, 0, 0, false };
    }

    // Read a cell
    Cell cell_at(int index) const { return cells_[index]; }

    // Pretty-print the board to stdout
    void print() const {
        std::cout << "\n";
        for (int i = 0; i < SIZE; ++i) {
            std::cout << " " << cell_char(cells_[i]);
            if (i % 3 != 2) std::cout << " │ ";
            else if (i != SIZE - 1) std::cout << "\n───┼───┼───\n";
        }
        std::cout << "\n\n";
    }

    int move_count() const { return move_count_; }

private:
    std::array<Cell, SIZE> cells_;
    int move_count_;

    static char cell_char(Cell c) {
        switch (c) {
            case Cell::X: return 'X';
            case Cell::O: return 'O';
            default:      return ' ';
        }
    }
};

// ────────────────────────────────────────────────────────────
//  ScoreTracker  — persists across rounds in one session
// ────────────────────────────────────────────────────────────
struct ScoreTracker {
    int wins_x   = 0;
    int wins_o   = 0;
    int draws    = 0;
    int rounds   = 0;

    void record(GameResult r) {
        ++rounds;
        if      (r == GameResult::WinX) ++wins_x;
        else if (r == GameResult::WinO) ++wins_o;
        else if (r == GameResult::Draw) ++draws;
    }

    void print() const {
        std::cout << "┌─────────────────────────┐\n";
        std::cout << "│        SCOREBOARD        │\n";
        std::cout << "├─────────────────────────┤\n";
        std::cout << "│  player_X  wins : " << wins_x
                  << std::string(6 - std::to_string(wins_x).size(), ' ') << "│\n";
        std::cout << "│  player_O  wins : " << wins_o
                  << std::string(6 - std::to_string(wins_o).size(), ' ') << "│\n";
        std::cout << "│  draws          : " << draws
                  << std::string(6 - std::to_string(draws).size(), ' ') << "│\n";
        std::cout << "│  rounds         : " << rounds
                  << std::string(6 - std::to_string(rounds).size(), ' ') << "│\n";
        std::cout << "└─────────────────────────┘\n\n";
    }
};

// ────────────────────────────────────────────────────────────
//  Game  — orchestrates a single round
// ────────────────────────────────────────────────────────────
class Game {
public:
    Game(ScoreTracker& tracker) : tracker_(tracker) {
        board_.reset();
        current_player_ = Cell::X;
    }

    void run() {
        print_header();
        while (true) {
            board_.print();
            print_status();

            int move = get_move();
            if (move == -1) {           // quit signal
                std::cout << "\n>> Session ended by player.\n";
                return;
            }

            if (!board_.place(move, current_player_)) {
                std::cout << "// error: cell " << (move+1)
                          << " is already occupied — try again.\n";
                continue;
            }

            GameResult r = board_.result();
            if (r != GameResult::Ongoing) {
                board_.print();
                announce(r);
                tracker_.record(r);
                tracker_.print();
                return;
            }

            // Switch player
            current_player_ = (current_player_ == Cell::X) ? Cell::O : Cell::X;
        }
    }

private:
    Board        board_;
    ScoreTracker& tracker_;
    Cell         current_player_;

    void print_header() const {
        std::cout << "============================================\n";
        std::cout << "  TIC-TAC-TOE  //  two-player mode\n";
        std::cout << "  Cells numbered 1-9 (left→right, top→bot)\n";
        std::cout << "  Enter 0 to quit.\n";
        std::cout << "============================================\n";
    }

    void print_status() const {
        std::string p = (current_player_ == Cell::X) ? "player_X" : "player_O";
        std::cout << ">> turn: " << p << "  |  move #"
                  << (board_.move_count() + 1) << "\n";
    }

    // Returns 0-based index, or -1 to quit
    int get_move() const {
        int input = 0;
        while (true) {
            std::cout << "   enter cell [1-9] or 0 to quit: ";
            if (!(std::cin >> input)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "// error: invalid input.\n";
                continue;
            }
            if (input == 0)           return -1;
            if (input >= 1 && input <= 9) return input - 1;
            std::cout << "// error: enter a number between 1 and 9.\n";
        }
    }

    void announce(GameResult r) const {
        std::cout << "============================================\n";
        if      (r == GameResult::WinX) std::cout << ">> RESULT: player_X wins!\n";
        else if (r == GameResult::WinO) std::cout << ">> RESULT: player_O wins!\n";
        else                            std::cout << ">> RESULT: draw — board full.\n";
        std::cout << "============================================\n\n";
    }
};

// ────────────────────────────────────────────────────────────
//  main
// ────────────────────────────────────────────────────────────
int main() {
    ScoreTracker tracker;

    std::cout << "\n[tictactoe] compiled with C++17\n\n";

    while (true) {
        Game game(tracker);
        game.run();

        std::cout << ">> play again? [y/n]: ";
        char again;
        std::cin >> again;
        if (again != 'y' && again != 'Y') break;
        std::cout << "\n";
    }

    std::cout << "\n[tictactoe] session over. final scores:\n\n";
    tracker.print();
    std::cout << ">> goodbye.\n\n";
    return 0;
}
