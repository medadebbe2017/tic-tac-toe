# tictactoe.cpp

A Tic-Tac-Toe game I built in C++ for practice with classes and game logic. The core engine (`tictactoe.cpp`) runs as a CLI game, and I mirrored the same logic in JavaScript so it's also playable in the browser with a matching look to the rest of my portfolio.

## Play it

[Live demo](https://medadebbe2017.github.io/tictactoe/)

## Features

- Two players, custom names
- Win/draw detection with a highlighted winning line
- Score tracking across rounds
- Restart round or reset the whole session
- The HTML page also has a tab to view the C++ source with basic syntax highlighting

## Running the C++ version

You need g++ with C++17 support.

```bash
g++ -std=c++17 -o tictactoe tictactoe.cpp
./tictactoe
```

## How it's structured

- `Board` — handles the 3x3 grid, placing moves, checking for a winner
- `ScoreTracker` — keeps wins/draws/rounds across a session
- `Game` — runs a single round, switches turns between players

The JS in `tictactoe.html` follows the same structure so the browser version behaves identically to the CLI one.

## Tech

C++17, and plain HTML/CSS/JS for the browser version — no frameworks.

## License

MIT — see [LICENSE](./LICENSE)
