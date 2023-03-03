#include <ncurses.h>

#include <iostream>
using namespace std;

#include "game.h"
#include "ai.h"

void DrawPiece(int row, int col, int player);

void InitializeBoard(Board &board) {
  // Initialize board
  for (int row = 0; row < kNumRows; ++row) {
    for (int col = 0; col < kNumCols; ++col) {
      board.at(row).at(col) = 0;
    }
  }
}

void DrawBoard(const Board &board, int column) {
  clear();

  // Draw column lines
  for (int j = 1; j <= kNumRows * kRowHeight; ++j) {
    for (int i = 0; i <= kNumCols; ++i) {
      mvaddch(j, kColWidth * i, '|');
    }
  }

  // Draw Row lines
  for (int i = kRowHeight; i < kNumRows * kRowHeight; i += 4) {
    for (int j = 1; j < kNumCols * kColWidth; ++j) {
      if (j % 10 == 0) {
        continue;
      }
      mvaddch(i, j, '-');
    }
  }

  // Draw pieces
  for (int row = 0; row < 6; row++) {
    for (int col = 0; col < 7; col++) {
      if (board[row][col] != 0) {
        DrawPiece(row, col, board[row][col]);
      }
    }
  }

  int col_pos = (kColWidth / 2) + (kColWidth * column);
  mvaddch(0, col_pos, '!');

  refresh();
}

void DrawPiece(int row, int col, int player) {
  int start_row = (row * kRowHeight) + 1;
  int start_col = (col * kColWidth) + 1;

  char ch;
  if (player == 1) {
    ch = '#';
  } else {
    ch = ':';
  }

  for (int r = start_row; r < start_row + (kRowHeight - 1); r++) {
    for (int c = start_col; c < start_col + (kColWidth - 1); c++) {
      mvaddch(r, c, ch);
    }
  }
}

bool Drop(Board &board, int player, int column) { 
  for(int i = 0; i < 6; ++i) {
    if (board.at(i).at(column) != 0) {
      if (i - 1 < 0) {
        return false;
      }
      if (player == 1) {
        board.at(i - 1).at(column) = 1;
        return true;
      }
      if (player == 2) {
        board.at(i - 1).at(column) = 2;
        return true;
      }
    }
  }

  // If column is empty, fill bottom row.
  if (player == 1) {
    board.at(5).at(column) = 1;
  } else if (player == 2) {
    board.at(5).at(column) = 2;
  }
  return true;
}

// Functions for use within Winner().

int Row_Win(int &player1_count, int &player2_count, const Board &board, int bestColumn, const int &victory, bool NoleBotUsage) {
  if (victory != 0) {
      return victory;
    }
  
  for(int i = 0; i < 6; ++i) {
    player1_count = 0;
    player2_count = 0;

    for(int j = 0; j < 7; ++j) {

      if (board.at(i).at(j) == 0) {
        player1_count = 0;
        player2_count = 0;
      } else if (board.at(i).at(j) == 1) {
        ++player1_count;
        player2_count = 0;
      } else if (board.at(i).at(j) == 2) {
        ++player2_count;
        player1_count = 0;
      }

      if (NoleBotUsage) {
        if ((player1_count == 3) || (player2_count == 3)) {
            bestColumn = j + 1;
            if (bestColumn > 5) {
              bestColumn = j - 1;
            }
            return bestColumn;
        }
      }

      if (player1_count == 4) {
        return 1;
      }
      if (player2_count == 4) {
        return 2;
      }

    }

  }
  return 0;
}

int Col_Win(int &player1_count, int &player2_count, const Board &board, int bestColumn, const int &victory, bool NoleBotUsage) {
  
  if (victory != 0) {
      return victory;
    }
  
  for(int j = 0; j < 7; ++j) {
    player1_count = 0;
    player2_count = 0;

    for(int i = 0; i < 6; ++i) {

      if (board.at(i).at(j) == 0) {
        player1_count = 0;
        player2_count = 0;
      } else if (board.at(i).at(j) == 1) {
        ++player1_count;
        player2_count = 0;
      } else if (board.at(i).at(j) == 2) {
        ++player2_count;
        player1_count = 0;
      }

      if (NoleBotUsage) {
        /*if ((player1_count == 3) || (player2_count == 3)) {
            bestColumn = j;
        }*/
        cout << "Best column: " << bestColumn << endl;
        return bestColumn;
      }

      if (player1_count == 4) {
        return 1;
      }
      if (player2_count == 4) {
        return 2;
      }

    }

  }
  return 0;
}

int Diagonal_Pos1 (int row, int &player1_count, int &player2_count, const Board &board, const int &victory, int bestColumn, bool NoleBotUsage) {
    if (victory != 0) {
      return victory;
    }

    player1_count = 0;
    player2_count = 0;
    for (int i = 0; i < (row + 1); ++i) {
      if (board.at(row - i).at(i) == 0) {
        player1_count = 0;
        player2_count = 0;
      } else if (board.at(row - i).at(i) == 1) {
        ++player1_count;
        player2_count = 0;
      } else if (board.at(row - i).at(i) == 2) {
        ++player2_count;
        player1_count = 0;
      }

      if (NoleBotUsage) {
        if ((player1_count == 3) || (player2_count == 3)) {
          bestColumn = i + 1;
          if (bestColumn > 5) {
            bestColumn = i - 1;
          }
          return bestColumn;
        }
      }

      if (player1_count == 4) {
        return 1;
      }
      if (player2_count == 4) {
        return 2;
      }
    }

    return 0;
}

int Diagonal_Pos2 (int row, int &player1_count, int &player2_count, const Board &board, const int &victory, int bestColumn, bool NoleBotUsage) {
  if (victory != 0) {
      return victory;
  }
  
  player1_count = 0;
  player2_count = 0;
  for (int i = 0; i < (6 - row); ++i) {
    if (board.at(row + i).at(6 - i) == 0) {
      player1_count = 0;
      player2_count = 0;
    } else if (board.at(row + i).at(6 - i) == 1) {
      ++player1_count;
      player2_count = 0;
    } else if (board.at(row + i).at(6 - i) == 2) {
      ++player2_count;
      player1_count = 0;
    }

    if (NoleBotUsage) {
      if ((player1_count == 3) || (player2_count == 3)) {
            bestColumn = i + 1;
            if (bestColumn > 5) {
              bestColumn = i - 1;
            }
            return bestColumn;
          }
    }

    if (player1_count == 4) {
      return 1;
    }
    if (player2_count == 4) {
      return 2;
    }
  }
  return 0;
}

int Diagonal_Neg1 (int row, int &player1_count, int &player2_count, const Board &board, const int &victory, int bestColumn, bool NoleBotUsage) {
  if (victory != 0) {
      return victory;
  }

  player1_count = 0;
  player2_count = 0;
  for (int i = 0; i < (6 - row); ++i) {
    if (board.at(row + i).at(i) == 0) {
      player1_count = 0;
      player2_count = 0;
    } else if (board.at(row + i).at(i) == 1) {
      ++player1_count;
      player2_count = 0;
    } else if (board.at(row + i).at(i) == 2) {
      ++player2_count;
      player1_count = 0;
    }

    if (NoleBotUsage) {
      if ((player1_count == 3) || (player2_count == 3)) {
            bestColumn = i + 1;
            if (bestColumn > 5) {
              bestColumn = i - 1;
            }
            return bestColumn;
          }
    }

    if (player1_count == 4) {
      return 1;
    }
    if (player2_count == 4) {
      return 2;
    }
  }
  return 0;
}

int Diagonal_Neg2 (int row, int &player1_count, int &player2_count, const Board &board, const int &victory, int bestColumn, bool NoleBotUsage) {
  if (victory != 0) {
      return victory;
  }

  player1_count = 0;
  player2_count = 0;
  for (int i = 0; i < (row + 1); ++i) {
    if (board.at(row - i).at(6 - i) == 0) {
      player1_count = 0;
      player2_count = 0;
    } else if (board.at(row - i).at(6 - i) == 1) {
      ++player1_count;
      player2_count = 0;
    } else if (board.at(row - i).at(6 - i) == 2) {
      ++player2_count;
      player1_count = 0;
    }

    if (NoleBotUsage) {
      if ((player1_count == 3) || (player2_count == 3)) {
            bestColumn = i + 1;
            if (bestColumn > 5) {
              bestColumn = i - 1;
            }
            return bestColumn;
      }
    }

    if (player1_count == 4) {
      return 1;
    }
    if (player2_count == 4) {
      return 2;
    }
  }
  return 0;
}

int Winner(const Board &board) {
  int player1_count;
  int player2_count;
  int victory = 0;
  int bestColumn = 3;

  // Check for a row win.

  // Check for a column win.

  // Check for a diagonal positive-slope win.
  victory = Row_Win(player1_count, player2_count, board, bestColumn, victory, false);

  victory = Col_Win(player1_count, player2_count, board, bestColumn, victory, false);

  victory = Diagonal_Pos1(3, player1_count, player2_count, board, victory, bestColumn, false);
  victory = Diagonal_Pos1(4, player1_count, player2_count, board, victory, bestColumn, false);
  victory = Diagonal_Pos1(5, player1_count, player2_count, board, victory, bestColumn, false);

  victory = Diagonal_Pos2(2, player1_count, player2_count, board, victory, bestColumn, false);
  victory = Diagonal_Pos2(1, player1_count, player2_count, board, victory, bestColumn, false);
  victory = Diagonal_Pos2(0, player1_count, player2_count, board, victory, bestColumn, false);

  victory = Diagonal_Neg1(2, player1_count, player2_count, board, victory, bestColumn, false);
  victory = Diagonal_Neg1(1, player1_count, player2_count, board, victory, bestColumn, false);
  victory = Diagonal_Neg1(0, player1_count, player2_count, board, victory, bestColumn, false);

  victory = Diagonal_Neg2(3, player1_count, player2_count, board, victory, bestColumn, false);
  victory = Diagonal_Neg2(4, player1_count, player2_count, board, victory, bestColumn, false);
  victory = Diagonal_Neg2(5, player1_count, player2_count, board, victory, bestColumn, false);

  if (victory == 1) {
    return 1;
  } else if (victory == 2) {
    return 2;
  } else {
    return 0;
  }
}