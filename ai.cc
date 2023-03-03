#include "ai.h"
#include "game.h"

#include <random>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

int FirstOpenAgent(const Board &board, int ai_player) {
  // To keep the interface for AIs consistent.
  (void)ai_player;

  // Loop over board and find the first place that is not occupied.
  for (int row = 0; row < 6; ++row) {
    for (int col = 0; col < 7; ++col) {
      if (board[row][col] == 0) {
        return col;
      }
    }
  }

  // This can only happen if game is broken.
  throw;
}

int RandomAgent(const Board &board, int ai_player) {
  // To keep the interface for AIs consistent.
  (void)ai_player;

  std::vector<int> open_columns;

  // Create list of columns that has an open space
  for (int row = 0; row < 6; ++row) {
    for (int col = 0; col < 7; ++col) {
      if (board[row][col] == 0) {
        open_columns.push_back(col);
      }
    }
  }

  // Randomly select from this list. An empty list implies the board is full.
  // This should not happen.
  if (open_columns.empty()) {
    throw;
  }

  size_t choice = rand() % open_columns.size();
  return open_columns[choice];
}

PlayerType GetPlayerType(const std::string &arg) {
  if (arg == "human") {
    return PlayerType::kHuman;
  } else if (arg == "random") {
    return PlayerType::kRandom;
  } else if (arg == "first") {
    return PlayerType::kFirst;
  } else if (arg == "nolebot") {
    return PlayerType::kNoleBot;
  } else {
    return PlayerType::kInvalid;
  }
}

// Functions for use by NoleBot to determine various types of victorys and how to block or embrace them.
int nRow_Win(int &player1_count, int &player2_count, const Board &board, int bestColumn, bool NoleBotUsage) {
  
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
        if ((player1_count == 1) || (player2_count == 1)) {
          if (i == 5) {
            if ((j <= 3) && (board.at(i).at(j) == 1) && (board.at(i).at(j + 2) == 1) && (board.at(i).at(j + 3) == 1) && (board.at(i).at(j + 1) == 0)) {
              bestColumn = j + 1;
              return bestColumn;
            } else if ((j <= 3) && (board.at(i).at(j) == 2) && (board.at(i).at(j + 2) == 2) && (board.at(i).at(j + 3) == 2) && (board.at(i).at(j + 1) == 0)) {
              bestColumn = j + 1;
              return bestColumn;
            } else {
              bestColumn = -1;
            }
          } else {
            if ((j <= 3) && (board.at(i).at(j) == 1) && (board.at(i).at(j + 2) == 1) && (board.at(i).at(j + 3) == 1) && (board.at(i).at(j + 1) == 0) && (board.at(i + 1).at(j + 1) != 0)) {
              bestColumn = j + 1;
              return bestColumn;
            } else if ((j <= 3) && (board.at(i).at(j) == 2) && (board.at(i).at(j + 2) == 2) && (board.at(i).at(j + 3) == 2) && (board.at(i).at(j + 1) == 0) && (board.at(i + 1).at(j + 1) != 0)) {
              bestColumn = j + 1;
              return bestColumn;
            } else {
              bestColumn = -1;
            }
          }
        }

        if ((player1_count == 2) || (player2_count == 2)) {
          if (i == 5) {
            if ((j <= 4) && (board.at(i).at(j) == 1) && (board.at(i).at(j + 2) == 1) && (board.at(i).at(j - 1) == 1) && (board.at(i).at(j + 1) == 0)) {
              bestColumn = j + 1;
              return bestColumn;
            } else if ((j <= 4) && (board.at(i).at(j) == 2) && (board.at(i).at(j + 2) == 2) && (board.at(i).at(j - 1) == 2) && (board.at(i).at(j + 1) == 0)) {
              bestColumn = j + 1;
              return bestColumn;
            } else if ((j + 1 <= 6) && (j - 2 >= 0)) {
              if (board.at(i).at(j + 1) == 0) {
                bestColumn = j + 1;
                return bestColumn;
              } else if (j - 1 == 0) {
                bestColumn = j - 1;
                return bestColumn;
              } else {
                bestColumn = -1;
              }
            } else {
              bestColumn = -1;
            }
          } else {
            if ((j <= 4) && (board.at(i).at(j) == 1) && (board.at(i).at(j + 2) == 1) && (board.at(i).at(j - 1) == 1) && (board.at(i).at(j + 1) == 0) && (board.at(i + 1).at(j + 1) != 0)) {
              bestColumn = j + 1;
              return bestColumn;
            } else if ((j <= 4) && (board.at(i).at(j) == 2) && (board.at(i).at(j + 2) == 2) && (board.at(i).at(j - 1) == 2) && (board.at(i).at(j + 1) == 0) && (board.at(i + 1).at(j + 1) != 0)) {
              bestColumn = j + 1;
              return bestColumn;
            } else {
              bestColumn = -1;
            }
          }
        }

        if ((player1_count == 3) || (player2_count == 3)) {
          if (i == 5) { // First-row scenario
            if ((j + 1 < 7) && (j - 3 >= 0)) {
              if (board.at(i).at(j + 1) == 0) {
                bestColumn = j + 1;
              } else if (board.at(i).at(j - 3) == 0) {
                bestColumn = j - 3;
              } else {
                bestColumn = -1;
              }
            } else if (j - 3 < 0) {
              if (board.at(i).at(j + 1) == 0) {
                bestColumn = j + 1;
              } else {
                bestColumn = -1;
              }
            } else if (j + 1 >= 7) {
              if (board.at(i).at(j - 3) == 0) {
                bestColumn = j - 3;
              } else {
                bestColumn = -1;
              }
            }            
          } else { // Other-rows scenario
            if ((j + 1 < 7) && (j - 3 >= 0)) {
              if ((board.at(i).at(j + 1) == 0) && (board.at(i + 1).at(j + 1) != 0)) {
                bestColumn = j + 1;
              } else if ((board.at(i).at(j - 3) == 0) && (board.at(i + 1).at(j - 3) != 0)) {
                bestColumn = j - 3;
              } else {
                bestColumn = -1;
              }
            } else if (j - 3 < 0) {
              if ((board.at(i).at(j + 1) == 0) && (board.at(i + 1).at(j + 1) != 0)) {
                bestColumn = j + 1;
              } else {
                bestColumn = -1;
              }
            } else if (j + 1 >= 7) {
              if ((board.at(i).at(j - 3) == 0) && (board.at(i + 1).at(j - 3) != 0)) {
                bestColumn = j - 3;
              } else {
                bestColumn = -1;
              }
            } 
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
  return -1;
}

int nCol_Win(int &player1_count, int &player2_count, const Board &board, int bestColumn, bool NoleBotUsage) {
  if (bestColumn != -1) {
      return bestColumn;
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
        if ((player1_count == 3) || (player2_count == 3)) {
          if (i - 2 > 0) {
            if (board.at(i - 3).at(j) == 0) {
              bestColumn = j;
            } else if (j < 6) {
              break;
            } else {
              bestColumn = -1;
            }
          } else {
            if (j < 6) {
              break;
            } else {
              bestColumn = -1;
            }
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
  return -1;
}

int nDiagonal_Pos1 (int row, int &player1_count, int &player2_count, const Board &board, int bestColumn, bool NoleBotUsage) {
    if (bestColumn != -1) {
      return bestColumn;
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
        if ((player1_count == 1) || (player2_count == 1)) {
          if ((row == 3) && (i == 0)) {
            if ((board.at(row - i).at(i) == 1) && (board.at(row - i - 2).at(i + 2) == 1) && (board.at(row - i - 3).at(i + 3) == 1) && (board.at(row - i).at(i + 1) != 0) && (board.at(row - i - 1).at(i + 1) == 0)) {
              bestColumn = i + 1;
              return bestColumn;
            } else if ((board.at(row - i).at(i) == 2) && (board.at(row - i - 2).at(i + 2) == 2) && (board.at(row - i - 3).at(i + 3) == 2) && (board.at(row - i).at(i + 1) != 0) && (board.at(row - i - 1).at(i + 1) == 0)) {
              bestColumn = i + 1;
              return bestColumn;
            } else {
              bestColumn = -1;
            }
          } else if ((row == 4) && ((i == 0) || (i == 1))) {
            if ((board.at(row - i).at(i) == 1) && (board.at(row - i - 2).at(i + 2) == 1) && (board.at(row - i - 3).at(i + 3) == 1) && (board.at(row - i).at(i + 1) != 0) && (board.at(row - i - 1).at(i + 1) == 0)) {
              bestColumn = i + 1;
              return bestColumn;
            } else if ((board.at(row - i).at(i) == 2) && (board.at(row - i - 2).at(i + 2) == 2) && (board.at(row - i - 3).at(i + 3) == 2) && (board.at(row - i).at(i + 1) != 0) && (board.at(row - i - 1).at(i + 1) == 0)) {
              bestColumn = i + 1;
              return bestColumn;
            } else {
              bestColumn = -1;
            }
          } else if ((row == 5) && ((i == 0) || (i == 1) || (i == 2))) {
            if ((board.at(row - i).at(i) == 1) && (board.at(row - i - 2).at(i + 2) == 1) && (board.at(row - i - 3).at(i + 3) == 1) && (board.at(row - i).at(i + 1) != 0) && (board.at(row - i - 1).at(i + 1) == 0)) {
              bestColumn = i + 1;
              return bestColumn;
            } else if ((board.at(row - i).at(i) == 2) && (board.at(row - i - 2).at(i + 2) == 2) && (board.at(row - i - 3).at(i + 3) == 2) && (board.at(row - i).at(i + 1) != 0) && (board.at(row - i - 1).at(i + 1) == 0)) {
              bestColumn = i + 1;
              return bestColumn;
            } else {
              bestColumn = -1;
            }
          }
        }

        if ((player1_count == 2) || (player2_count == 2)) {
          if ((row == 3) && (i == 1)) {
            if ((board.at(row - i).at(i) == 1) && (board.at(row - i - 2).at(i + 2) == 1) && (board.at(row - i + 1).at(i - 1) == 1) && (board.at(row - i).at(i + 1) != 0) && (board.at(row - i - 1).at(i + 1) == 0)) {
              bestColumn = i + 1;
              return bestColumn;
            } else if ((board.at(row - i).at(i) == 2) && (board.at(row - i - 2).at(i + 2) == 2) && (board.at(row - i + 1).at(i - 1) == 2) && (board.at(row - i).at(i + 1) != 0) && (board.at(row - i - 1).at(i + 1) == 0)) {
              bestColumn = i + 1;
              return bestColumn;
            } else {
              bestColumn = -1;
            }
          } else if ((row == 4) && ((i == 1) || (i == 2))) {
            if ((board.at(row - i).at(i) == 1) && (board.at(row - i - 2).at(i + 2) == 1) && (board.at(row - i + 1).at(i - 1) == 1) && (board.at(row - i).at(i + 1) != 0) && (board.at(row - i - 1).at(i + 1) == 0)) {
              bestColumn = i + 1;
              return bestColumn;
            } else if ((board.at(row - i).at(i) == 2) && (board.at(row - i - 2).at(i + 2) == 2) && (board.at(row - i + 1).at(i - 1) == 2) && (board.at(row - i).at(i + 1) != 0) && (board.at(row - i - 1).at(i + 1) == 0)) {
              bestColumn = i + 1;
              return bestColumn;
            } else {
              bestColumn = -1;
            }
          } else if ((row == 5) && ((i == 1) || (i == 2) || (i == 3))) {
            if ((board.at(row - i).at(i) == 1) && (board.at(row - i - 2).at(i + 2) == 1) && (board.at(row - i + 1).at(i - 1) == 1) && (board.at(row - i).at(i + 1) != 0) && (board.at(row - i - 1).at(i + 1) == 0)) {
              bestColumn = i + 1;
              return bestColumn;
            } else if ((board.at(row - i).at(i) == 2) && (board.at(row - i - 2).at(i + 2) == 2) && (board.at(row - i + 1).at(i - 1) == 2) && (board.at(row - i).at(i + 1) != 0) && (board.at(row - i - 1).at(i + 1) == 0)) {
              bestColumn = i + 1;
              return bestColumn;
            } else {
              bestColumn = -1;
            }
          }
        }

        if ((player1_count == 3) || (player2_count == 3)) {
          if ((i - 3 >= 0) && (row - i >= 1)) {
            if ((row - i + 4) > 5) {
              if (board.at(row - i).at(i + 1) != 0 && board.at(row - i - 1).at(i + 1) == 0) {
                bestColumn = i + 1;
              } else if (board.at(row - i + 3).at(i - 3) == 0) {
                bestColumn = i - 3;
              } else {
                bestColumn = -1;
              }
            } else {
              if (board.at(row - i).at(i + 1) != 0) {
                bestColumn = i + 1;
              } else if (board.at(row - i + 4).at(i - 3) != 0) {
                bestColumn = i - 3;
              } else {
                bestColumn = -1;
              }
            }
          } else if (i - 2 == 0) {
            if (board.at(row - i).at(i + 1) != 0 && board.at(row - i - 1).at(i + 1) == 0) {
                bestColumn = i + 1;
            } else {
              bestColumn = -1;
            }
          } else if (row - i == 0) {
            if (board.at(row - i + 4).at(i - 3) != 0 && board.at(row - i + 3).at(i - 3) == 0) {
              bestColumn = i - 3;
            } else {
              bestColumn = -1;
            }
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

    return -1;
}

int nDiagonal_Pos2 (int row, int &player1_count, int &player2_count, const Board &board, int bestColumn, bool NoleBotUsage) {
  if (bestColumn != -1) {
      return bestColumn;
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
      if ((player1_count == 1) || (player2_count == 1)) {
        if ((row == 0) && ((6 - i == 6) || (6 - i == 5) || ((6 - i) == 4))) {
          if ((board.at(row + i).at(6 - i) == 1) && (board.at(row + i + 2).at(4 - i) == 1) && (board.at(row + i + 3).at(3 - i) == 1) && (board.at(row + i + 2).at(5 - i) != 0) && (board.at(row + i + 1).at(5 - i) == 0)) {
            bestColumn = 5 - i;
            return bestColumn;
          } else if ((board.at(row + i).at(6 - i) == 2) && (board.at(row + i + 2).at(4 - i) == 2) && (board.at(row + i + 3).at(3 - i) == 2) && (board.at(row + i + 2).at(5 - i) != 0) && (board.at(row + i + 1).at(5 - i) == 0)) {
            bestColumn = 5 - i;
            return bestColumn;
          } else {
            bestColumn = -1;
          }
        } else if ((row == 1) && ((6 - i == 6) || (6 - i == 5))) {
          if ((board.at(row + i).at(6 - i) == 1) && (board.at(row + i + 2).at(4 - i) == 1) && (board.at(row + i + 3).at(3 - i) == 1) && (board.at(row + i + 2).at(5 - i) != 0) && (board.at(row + i + 1).at(5 - i) == 0)) {
            bestColumn = 5 - i;
            return bestColumn;
          } else if ((board.at(row + i).at(6 - i) == 2) && (board.at(row + i + 2).at(4 - i) == 2) && (board.at(row + i + 3).at(3 - i) == 2) && (board.at(row + i + 2).at(5 - i) != 0) && (board.at(row + i + 1).at(5 - i) == 0)) {
            bestColumn = 5 - i;
            return bestColumn;
          } else {
            bestColumn = -1;
          }
        } else if ((row == 2) && (6 - i == 6)) {
          if ((board.at(row + i).at(6 - i) == 1) && (board.at(row + i + 2).at(4 - i) == 1) && (board.at(row + i + 3).at(3 - i) == 1) && (board.at(row + i + 2).at(5 - i) != 0) && (board.at(row + i + 1).at(5 - i) == 0)) {
            bestColumn = 5 - i;
            return bestColumn;
          } else if ((board.at(row + i).at(6 - i) == 2) && (board.at(row + i + 2).at(4 - i) == 2) && (board.at(row + i + 3).at(3 - i) == 2) && (board.at(row + i + 2).at(5 - i) != 0) && (board.at(row + i + 1).at(5 - i) == 0)) {
            bestColumn = 5 - i;
            return bestColumn;
          } else {
            bestColumn = -1;
          }
        }
      }

      if ((player1_count == 2) || (player2_count == 2)) {
        if ((row == 0) && ((6 - i == 5) || (6 - i == 4) || ((6 - i) == 3))) {
          if ((board.at(row + i).at(6 - i) == 1) && (board.at(row + i + 2).at(4 - i) == 1) && (board.at(row + i - 1).at(7 - i) == 1) && (board.at(row + i + 2).at(5 - i) != 0) && (board.at(row + i + 1).at(5 - i) == 0)) {
            bestColumn = 5 - i;
            return bestColumn;
          } else if ((board.at(row + i).at(6 - i) == 2) && (board.at(row + i + 2).at(4 - i) == 2) && (board.at(row + i - 1).at(7 - i) == 2) && (board.at(row + i + 2).at(5 - i) != 0) && (board.at(row + i + 1).at(5 - i) == 0)) {
            bestColumn = 5 - i;
            return bestColumn;
          } else {
            bestColumn = -1;
          }
        } else if ((row == 1) && ((6 - i == 5) || (6 - i == 4))) {
          if ((board.at(row + i).at(6 - i) == 1) && (board.at(row + i + 2).at(4 - i) == 1) && (board.at(row + i - 1).at(7 - i) == 1) && (board.at(row + i + 2).at(5 - i) != 0) && (board.at(row + i + 1).at(5 - i) == 0)) {
            bestColumn = 5 - i;
            return bestColumn;
          } else if ((board.at(row + i).at(6 - i) == 2) && (board.at(row + i + 2).at(4 - i) == 2) && (board.at(row + i - 1).at(7 - i) == 2) && (board.at(row + i + 2).at(5 - i) != 0) && (board.at(row + i + 1).at(5 - i) == 0)) {
            bestColumn = 5 - i;
            return bestColumn;
          } else {
            bestColumn = -1;
          }
        } else if ((row == 2) && (6 - i == 5)) {
          if ((board.at(row + i).at(6 - i) == 1) && (board.at(row + i + 2).at(4 - i) == 1) && (board.at(row + i - 1).at(7 - i) == 1) && (board.at(row + i + 2).at(5 - i) != 0) && (board.at(row + i + 1).at(5 - i) == 0)) {
            bestColumn = 5 - i;
            return bestColumn;
          } else if ((board.at(row + i).at(6 - i) == 2) && (board.at(row + i + 2).at(4 - i) == 2) && (board.at(row + i - 1).at(7 - i) == 2) && (board.at(row + i + 2).at(5 - i) != 0) && (board.at(row + i + 1).at(5 - i) == 0)) {
            bestColumn = 5 - i;
            return bestColumn;
          } else {
            bestColumn = -1;
          }
        }
      }

      if ((player1_count == 3) || (player2_count == 3)) {
        if ((row + i + 1 < 6) && (9 - i < 7)) {
          if (row + i + 2 < 6) {
            if ((board.at(row + i + 2).at(5 - i) != 0) && board.at(row + i + 1).at(5 - i) == 0) {
              bestColumn = 5 - i;
            } else if ((board.at(row + i - 2).at(9 - i) != 0) && (board.at(row + i - 3).at(9 - i) == 0)) {
              bestColumn = 9 - i;
            } else {
              bestColumn = -1;
            }
          } else if (row + i + 1 == 5) {
            if (board.at(row + i + 1).at(5 - i) == 0) {
              bestColumn = 5 - i;
            } else if ((board.at(row + i - 2).at(9 - i) != 0) && (board.at(row + i - 3).at(9 - i) == 0)) {
              bestColumn = 9 - i;
            } else {
              bestColumn = -1;
            }
          }
        } else if (row + i == 5) {
          if ((board.at(row + i - 2).at(9 - i) != 0) && (board.at(row + i - 3).at(9 - i) == 0)) {
            bestColumn = 9 - i;
          } else {
            bestColumn = -1;
          }
        } else if (4 - i == 6) {
          if (row + i + 2 < 6) {
            if ((board.at(row + i + 2).at(5 - i) != 0) && board.at(row + i + 1).at(5 - i) == 0) {
              bestColumn = 5 - i;
            } else {
              bestColumn = -1;
            }
          } else if (row + i + 1 == 5) {
            if (board.at(row + i + 1).at(5 - i) == 0) {
              bestColumn = 5 - i;
            } else {
              bestColumn = -1;
            }
          }
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
  return -1;
}

int nDiagonal_Neg1 (int row, int &player1_count, int &player2_count, const Board &board, int bestColumn, bool NoleBotUsage) {
  if (bestColumn != -1) {
      return bestColumn;
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
      if ((player1_count == 1) || (player2_count == 1)) {
        if ((row == 0) && ((i == 0) || (i == 1) || (i == 2))) {
          if ((board.at(row + i).at(i) == 1) && (board.at(row + i + 2).at(i + 2) == 1) && (board.at(row + i + 3).at(i + 3) == 1) && (board.at(row + i + 2).at(i + 1) != 0) && (board.at(row + i + 1).at(i + 1) == 0)) {
            bestColumn = i + 1;
            return bestColumn;
          } else if ((board.at(row + i).at(i) == 2) && (board.at(row + i + 2).at(i + 2) == 2) && (board.at(row + i + 3).at(i + 3) == 2) && (board.at(row + i + 2).at(i + 1) != 0) && (board.at(row + i + 1).at(i + 1) == 0)) {
            bestColumn = i + 1;
            return bestColumn;
          } else {
            bestColumn = -1;
          }
        } else if ((row == 1) && ((i == 0) || (i == 1))) {
          if ((board.at(row + i).at(i) == 1) && (board.at(row + i + 2).at(i + 2) == 1) && (board.at(row + i + 3).at(i + 3) == 1) && (board.at(row + i + 2).at(i + 1) != 0) && (board.at(row + i + 1).at(i + 1) == 0)) {
            bestColumn = i + 1;
            return bestColumn;
          } else if ((board.at(row + i).at(i) == 2) && (board.at(row + i + 2).at(i + 2) == 2) && (board.at(row + i + 3).at(i + 3) == 2) && (board.at(row + i + 2).at(i + 1) != 0) && (board.at(row + i + 1).at(i + 1) == 0)) {
            bestColumn = i + 1;
            return bestColumn;
          } else {
            bestColumn = -1;
          }
        } else if ((row == 2) && (i == 0)) {
          if ((board.at(row + i).at(i) == 1) && (board.at(row + i + 2).at(i + 2) == 1) && (board.at(row + i + 3).at(i + 3) == 1) && (board.at(row + i + 2).at(i + 1) != 0) && (board.at(row + i + 1).at(i + 1) == 0)) {
            bestColumn = i + 1;
            return bestColumn;
          } else if ((board.at(row + i).at(i) == 2) && (board.at(row + i + 2).at(i + 2) == 2) && (board.at(row + i + 3).at(i + 3) == 2) && (board.at(row + i + 2).at(i + 1) != 0) && (board.at(row + i + 1).at(i + 1) == 0)) {
            bestColumn = i + 1;
            return bestColumn;
          } else {
            bestColumn = -1;
          }
        }
      }

      if ((player1_count == 2) || (player2_count == 2)) {
        if ((row == 0) && ((i == 1) || (i == 2) || (i == 3))) {
          if ((board.at(row + i).at(i) == 1) && (board.at(row + i + 2).at(i + 2) == 1) && (board.at(row + i - 1).at(i - 1) == 1) && (board.at(row + i + 2).at(i + 1) != 0) && (board.at(row + i + 1).at(i + 1) == 0)) {
            bestColumn = i + 1;
            return bestColumn;
          } else if ((board.at(row + i).at(i) == 2) && (board.at(row + i + 2).at(i + 2) == 2) && (board.at(row + i - 1).at(i - 1) == 2) && (board.at(row + i + 2).at(i + 1) != 0) && (board.at(row + i + 1).at(i + 1) == 0)) {
            bestColumn = i + 1;
            return bestColumn;
          } else {
            bestColumn = -1;
          }
        } else if ((row == 1) && ((i == 1) || (i == 2))) {
          if ((board.at(row + i).at(i) == 1) && (board.at(row + i + 2).at(i + 2) == 1) && (board.at(row + i - 1).at(i - 1) == 1) && (board.at(row + i + 2).at(i + 1) != 0) && (board.at(row + i + 1).at(i + 1) == 0)) {
            bestColumn = i + 1;
            return bestColumn;
          } else if ((board.at(row + i).at(i) == 2) && (board.at(row + i + 2).at(i + 2) == 2) && (board.at(row + i - 1).at(i - 1) == 2) && (board.at(row + i + 2).at(i + 1) != 0) && (board.at(row + i + 1).at(i + 1) == 0)) {
            bestColumn = i + 1;
            return bestColumn;
          } else {
            bestColumn = -1;
          }
        } else if ((row == 2) && (i == 1)) {
          if ((board.at(row + i).at(i) == 1) && (board.at(row + i + 2).at(i + 2) == 1) && (board.at(row + i - 1).at(i - 1) == 1) && (board.at(row + i + 2).at(i + 1) != 0) && (board.at(row + i + 1).at(i + 1) == 0)) {
            bestColumn = i + 1;
            return bestColumn;
          } else if ((board.at(row + i).at(i) == 2) && (board.at(row + i + 2).at(i + 2) == 2) && (board.at(row + i - 1).at(i - 1) == 2) && (board.at(row + i + 2).at(i + 1) != 0) && (board.at(row + i + 1).at(i + 1) == 0)) {
            bestColumn = i + 1;
            return bestColumn;
          } else {
            bestColumn = -1;
          }
        }
      }

      if ((player1_count == 3) || (player2_count == 3)) {
        if ((row + i + 1 < 6) && (i - 2 != 0)) {
          if (row + i < 4) {
            if ((board.at(row + i + 2).at(i + 1) != 0) && (board.at(row + i + 1).at(i + 1) == 0)) {
              bestColumn = i + 1;
            } else if ((board.at(row + i - 2).at(i - 3) != 0) && (board.at(row + i - 3).at(i - 3) == 0)) {
              bestColumn = i - 3;
            } else {
              bestColumn = -1;
            }
          } else if (row + i == 4) {
            if (board.at(row + i + 1).at(i + 1) == 0) {
              bestColumn = i + 1;
            } else if ((board.at(row + i - 2).at(i - 3) != 0) && (board.at(row + i - 3).at(i - 3) == 0)) {
              bestColumn = i - 3;
            } else {
              bestColumn = -1;
            }
          }
        } else if (i - 2 == 0) {
          if (row + i < 4) {
            if ((board.at(row + i + 2).at(i + 1) != 0) && (board.at(row + i + 1).at(i + 1) == 0)) {
              bestColumn = i + 1;
            } else {
              bestColumn = -1;
            }
          } else if (row + i == 4) {
            if (board.at(row + i + 1).at(i + 1) == 0) {
              bestColumn = i + 1;
            } else {
              bestColumn = -1;
            }
          }
        } else if (row + i == 5) {
          if ((board.at(row + i - 2).at(i - 3) != 0) && (board.at(row + i - 3).at(i - 3) == 0)) {
            bestColumn = i - 3;
          } else {
            bestColumn = -1;
          }
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
  return -1;
}

int nDiagonal_Neg2 (int row, int &player1_count, int &player2_count, const Board &board, int bestColumn, bool NoleBotUsage) {
  if (bestColumn != -1) {
      return bestColumn;
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
      if ((player1_count == 1) || (player2_count == 1)) {
        if ((row == 5) && ((6 - i == 6) || (6 - i == 5) || (6 - i == 4))) {
          if ((board.at(row - i).at(6 - i) == 1) && (board.at(row - i - 2).at(4 - i) == 1) && (board.at(row - i - 3).at(3 - i) == 1) && (board.at(row - i).at(5 - i) != 0) && (board.at(row - i - 1).at(5 - i) == 0)) {
            bestColumn = 5 - i;
            return bestColumn;
          } else if ((board.at(row - i).at(6 - i) == 2) && (board.at(row - i - 2).at(4 - i) == 2) && (board.at(row - i - 3).at(3 - i) == 2) && (board.at(row - i).at(5 - i) != 0) && (board.at(row - i - 1).at(5 - i) == 0)) {
            bestColumn = 5 - i;
            return bestColumn;
          } else {
            bestColumn = -1;
          }
        } else if ((row == 4) && ((6 - i == 6) || (6 - i == 5))) {
          if ((board.at(row - i).at(6 - i) == 1) && (board.at(row - i - 2).at(4 - i) == 1) && (board.at(row - i - 3).at(3 - i) == 1) && (board.at(row - i).at(5 - i) != 0) && (board.at(row - i - 1).at(5 - i) == 0)) {
            bestColumn = 5 - i;
            return bestColumn;
          } else if ((board.at(row - i).at(6 - i) == 2) && (board.at(row - i - 2).at(4 - i) == 2) && (board.at(row - i - 3).at(3 - i) == 2) && (board.at(row - i).at(5 - i) != 0) && (board.at(row - i - 1).at(5 - i) == 0)) {
            bestColumn = 5 - i;
            return bestColumn;
          } else {
            bestColumn = -1;
          }
        } else if ((row == 3) && (6 - i == 6)) {
          if ((board.at(row - i).at(6 - i) == 1) && (board.at(row - i - 2).at(4 - i) == 1) && (board.at(row - i - 3).at(3 - i) == 1) && (board.at(row - i).at(5 - i) != 0) && (board.at(row - i - 1).at(5 - i) == 0)) {
            bestColumn = 5 - i;
            return bestColumn;
          } else if ((board.at(row - i).at(6 - i) == 2) && (board.at(row - i - 2).at(4 - i) == 2) && (board.at(row - i - 3).at(3 - i) == 2) && (board.at(row - i).at(5 - i) != 0) && (board.at(row - i - 1).at(5 - i) == 0)) {
            bestColumn = 5 - i;
            return bestColumn;
          } else {
            bestColumn = -1;
          }
        }
      }

      if ((player1_count == 2) || (player2_count == 2)) {
        if ((row == 5) && ((6 - i == 5) || (6 - i == 4) || (6 - i == 3))) {
          if ((board.at(row - i).at(6 - i) == 1) && (board.at(row - i - 2).at(4 - i) == 1) && (board.at(row - i + 1).at(7 - i) == 1) && (board.at(row - i).at(5 - i) != 0) && (board.at(row - i - 1).at(5 - i) == 0)) {
            bestColumn = 5 - i;
            return bestColumn;
          } else if ((board.at(row - i).at(6 - i) == 2) && (board.at(row - i - 2).at(4 - i) == 2) && (board.at(row - i + 1).at(7 - i) == 2) && (board.at(row - i).at(5 - i) != 0) && (board.at(row - i - 1).at(5 - i) == 0)) {
            bestColumn = 5 - i;
            return bestColumn;
          } else {
            bestColumn = -1;
          }
        } else if ((row == 4) && ((6 - i == 5) || (6 - i == 4))) {
          if ((board.at(row - i).at(6 - i) == 1) && (board.at(row - i - 2).at(4 - i) == 1) && (board.at(row - i + 1).at(7 - i) == 1) && (board.at(row - i).at(5 - i) != 0) && (board.at(row - i - 1).at(5 - i) == 0)) {
            bestColumn = 5 - i;
            return bestColumn;
          } else if ((board.at(row - i).at(6 - i) == 2) && (board.at(row - i - 2).at(4 - i) == 2) && (board.at(row - i + 1).at(7 - i) == 2) && (board.at(row - i).at(5 - i) != 0) && (board.at(row - i - 1).at(5 - i) == 0)) {
            bestColumn = 5 - i;
            return bestColumn;
          } else {
            bestColumn = -1;
          }
        } else if ((row == 3) && (6 - i == 5)) {
          if ((board.at(row - i).at(6 - i) == 1) && (board.at(row - i - 2).at(4 - i) == 1) && (board.at(row - i + 1).at(7 - i) == 1) && (board.at(row - i).at(5 - i) != 0) && (board.at(row - i - 1).at(5 - i) == 0)) {
            bestColumn = 5 - i;
            return bestColumn;
          } else if ((board.at(row - i).at(6 - i) == 2) && (board.at(row - i - 2).at(4 - i) == 2) && (board.at(row - i + 1).at(7 - i) == 2) && (board.at(row - i).at(5 - i) != 0) && (board.at(row - i - 1).at(5 - i) == 0)) {
            bestColumn = 5 - i;
            return bestColumn;
          } else {
            bestColumn = -1;
          }
        }
      }

      if ((player1_count == 3) || (player2_count == 3)) {
        if ((row - i > 0) && (8 - i < 6)) {
          if (row - i + 2 == 4) {
            if (board.at(row - i + 3).at(9 - i) == 0) {
              bestColumn = 9 - i;
            } else if ((board.at(row - i).at(5 - i) != 0) && (board.at(row - i - 1).at(5 - i) == 0)) {
              bestColumn = 5 - i;
            } else {
              bestColumn = -1;
            }
          } else {
            if ((board.at(row - i + 4).at(9 - i) != 0) && (board.at(row - i + 4).at(9 - i) == 0)) {
              bestColumn = 9 - i;
            } else if ((board.at(row - i).at(5 - i) != 0) && (board.at(row - i - 1).at(5 - i) == 0)) {
              bestColumn = 5 - i;
            } else {
              bestColumn = -1;
            }
          }
        } else if (row - i == 0) {
          if ((board.at(row - i + 4).at(9 - i) != 0) && (board.at(row - i + 4).at(9 - i) == 0)) {
            bestColumn = 9 - i;
          } else {
            bestColumn = -1;
          }
        } else if (6 - i == 6) {
          if ((board.at(row - i).at(5 - i) != 0) && (board.at(row - i - 1).at(5 - i) == 0)) {
            bestColumn = 5 - i;
          } else {
            bestColumn = -1;
          }
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
  return -1;
}

/* 
This is NoleBot, my hand-crafted AI. Its strategy is to prefer to go in the center column and, once the center is filled, to go in
the surrounding columns. If NoleBot sees that its opponent can win, it will block them. If NoleBot sees that it can win, it will prefer
winning over blocking the opponent. NoleBot is capable of sensing all three types of victories: Type 1: 000x, Type 2: 0x00, Type 3: 00x0; 
'x' represents where NoleBot can go to prevent a victory.

To use NoleBot, use the key-phrase 'nolebot' in the command line!
*/
int NoleBotAgent(const Board &board, int ai_player) {
  // To keep the interface for AIs consistent.
  (void)ai_player;


  // Check to see if NoleBot can win and, if so, win. Check to see if opponent can win and, if so, stop them from winning.
  int bestColumn = -1;
  int player1_count = 0;
  int player2_count = 0;

  // Status: COMPLETE
  bestColumn = nRow_Win(player1_count, player2_count, board, bestColumn, true);

  // Status: COMPLETE
  bestColumn = nCol_Win(player1_count, player2_count, board, bestColumn, true);

  // Status: COMPLETE
  bestColumn = nDiagonal_Pos1(3, player1_count, player2_count, board, bestColumn, true);
  bestColumn = nDiagonal_Pos1(4, player1_count, player2_count, board, bestColumn, true);
  bestColumn = nDiagonal_Pos1(5, player1_count, player2_count, board, bestColumn, true);

  // Status: COMPLETE
  bestColumn = nDiagonal_Pos2(2, player1_count, player2_count, board, bestColumn, true);
  bestColumn = nDiagonal_Pos2(1, player1_count, player2_count, board, bestColumn, true);
  bestColumn = nDiagonal_Pos2(0, player1_count, player2_count, board, bestColumn, true);

  // Status: COMPLETE
  bestColumn = nDiagonal_Neg1(2, player1_count, player2_count, board, bestColumn, true);
  bestColumn = nDiagonal_Neg1(1, player1_count, player2_count, board, bestColumn, true);
  bestColumn = nDiagonal_Neg1(0, player1_count, player2_count, board, bestColumn, true);

  // Status: COMPLETE
  bestColumn = nDiagonal_Neg2(3, player1_count, player2_count, board, bestColumn, true);
  bestColumn = nDiagonal_Neg2(4, player1_count, player2_count, board, bestColumn, true);
  bestColumn = nDiagonal_Neg2(5, player1_count, player2_count, board, bestColumn, true);

  // Return a column number 0-6
  if (bestColumn != -1) {
    if (board.at(0).at(bestColumn) != 0) {
      bestColumn = -1;
    }
  }

  if ((bestColumn != -1) && (board.at(0).at(bestColumn) == 0)) {
    return bestColumn;
  } else {
      if (board.at(0).at(3) == 0) {
        return 3;
      } else if (board.at(0).at(2) == 0) {
        return 2;
      } else if (board.at(0).at(4) == 0) {
        return 4;
      } else if (board.at(0).at(1) == 0) {
        return 1;
      } else if (board.at(0).at(5) == 0) {
        return 5;
      } else if (board.at(0).at(0) == 0) {
        return 0;
      } else if (board.at(0).at(6) == 0) {
        return 6;
      }
  }

  // This can only happen if game is broken.
  throw;
}