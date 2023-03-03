// The main logic of the Connect 4 game. The game rules are in these functions.

#ifndef GAME_H_
#define GAME_H_

#include <array>

#include <iostream>
using namespace std;

const int kNumRows = 6;
const int kNumCols = 7;
const int kColWidth = 10;
const int kRowHeight = 4;

enum class PlayerType { kHuman, kRandom, kFirst, kNoleBot, kInvalid };

typedef std::array<std::array<int, kNumCols>, kNumRows> Board;

// Create an empty board.
void InitializeBoard(Board &board);

// Draws the game board
void DrawBoard(const Board &board, int column);

// Play a piece by player at column.
// Returns false if the move is invalid.
bool Drop(Board &board, int player, int column);

int Row_Win(int &player1_count, int &player2_count, const Board &board, int bestColumn, const int &victory, bool NoleBotUsage);
int Col_Win(int &player1_count, int &player2_count, const Board &board, int bestColumn, const int &victory, bool NoleBotUsage);
int Diagonal_Pos1 (int row, int &player1_count, int &player2_count, const Board &board, const int &victory, int bestColumn, bool NoleBotUsage);
int Diagonal_Pos2 (int row, int &player1_count, int &player2_count, const Board &board, const int &victory, int bestColumn, bool NoleBotUsage);
int Diagonal_Neg1 (int row, int &player1_count, int &player2_count, const Board &board, const int &victory, int bestColumn, bool NoleBotUsage);
int Diagonal_Neg2 (int row, int &player1_count, int &player2_count, const Board &board, const int &victory, int bestColumn, bool NoleBotUsage);

// Returns 0 if no winner yet.
// Returns -1 if draw.
// Returns 1 if player 1 has won.
// Returns 2 if player 2 has won.
int Winner(const Board &board);

#endif