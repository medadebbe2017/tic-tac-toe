Building
Just needs g++ (or clang) with C++17.
g++ -std=c++17 -o tictactoe tictactoe.cpp
./tictactoe
macOS: brew install gcc
Ubuntu/Debian: sudo apt install g++
That’s it — no other dependencies. The browser version (tictactoe.html) doesn’t need building, the JS logic just mirrors the C++ and runs on its own.
