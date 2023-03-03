// The main game loop for Connect 4.

#include <ncurses.h>

#include <iostream>
#include <random>
#include <utility>

#include "ai.h"
#include "game.h"

using namespace std;

// *** Utility functions only used in this file. ***

// Switches from player 1 to player 2 and vice-versa.
int SwitchPlayer(int player);

// Get a move from the human. Updates UI to show column selection.
bool GetPlayerMove(const Board &board, int &move);

// Plays a single game.
int PlayGame(std::pair<PlayerType, PlayerType> players, bool silent);

int main(int argc, char **argv) {
  srand(time(0));

  std::pair<PlayerType, PlayerType> players;
  if (argc == 3) {
    players = make_pair(GetPlayerType(argv[1]), GetPlayerType(argv[2]));
  } else if (argc == 1) {
    players = make_pair(PlayerType::kHuman, PlayerType::kRandom);
  } else {
    cerr << "Usage: ./connect4 [player1] [player2]" << endl;
    return 1;
  }

  // Show the screen if either player is a human.
  bool silent = true;
  if (players.first == PlayerType::kHuman ||
      players.second == PlayerType::kHuman) {
    silent = false;
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
  }
  
  if (silent) {
    // The pit. Two AIs fighting each other.
    int player1_wins = 0;
    int player2_wins = 0;
    int draws = 0;

    for (int i = 0; i < 1000; ++i) {
      int winner = PlayGame(players, silent);
      if (winner == 1) {
        player1_wins += 1;
      } else if (winner == 2) {
        player2_wins += 1;
      } else if (winner == 0) {
        draws++;
      }
    }

    cout << "Player 1 wins: " << player1_wins << endl;
    cout << "Player 2 wins: " << player2_wins << endl;
    cout << "Draws: " << draws << endl;
  } else {
    // Play a single game Human vs. AI.
    PlayGame(players, silent);
  }

  // Wait for any key press.
  if (!silent) {
    getch();
    endwin();
  }

  return 0;
}

// Returns false if the user wants to exit.
bool GetPlayerMove(const Board &board, int &move) {
  int ch;

  while (true) {
    // User Move
    ch = getch();

    if (ch == KEY_RIGHT && move <= 5) {
      move += 1;
    } else if (ch == KEY_LEFT && move > 0) {
      move -= 1;
    } else if (ch == ' ') {
      return true;
    } else if (ch == 'q') {
      return false;
    }

    DrawBoard(board, move);
  }
}

int SwitchPlayer(int player) {
  if (player == 1) return 2;
  if (player == 2) return 1;
  throw;
}

int PlayGame(std::pair<PlayerType, PlayerType> players, bool silent) {
  Board board;

  int current_player = 1;

  InitializeBoard(board);

  int player_move = 0;

  // Loop ends if the game ends or GetPlayerMove returns false.
  while (true) {
    if (!silent) DrawBoard(board, player_move);
    if (Winner(board) != 0) {
      break;
    }

    PlayerType player_type;

    if (current_player == 1) {
      player_type = players.first;
    } else if (current_player == 2) {
      player_type = players.second;
    } else {
      throw;
    }

    // Kept separate from player_move to avoid updating UI.
    int move_to_make;

    switch (player_type) {
      case PlayerType::kHuman:
        if (!GetPlayerMove(board, player_move)) {
          printw("Game over.");
          return Winner(board);
        }
        move_to_make = player_move;
        break;
      case PlayerType::kRandom:
        move_to_make = RandomAgent(board, current_player);
        break;
      case PlayerType::kFirst:
        move_to_make = FirstOpenAgent(board, current_player);
        break;
      case PlayerType::kNoleBot:
        move_to_make = NoleBotAgent(board, current_player);
        break;
      case PlayerType::kInvalid:
      default:
        throw;
    }

    bool result = Drop(board, current_player, move_to_make);
    if (result) current_player = SwitchPlayer(current_player);
  }

  int winner = Winner(board);

  if (!silent) {
    if (winner == 1) {
      printw("Player 1 wins!");
    } else if (winner == 2) {
      printw("Player 2 wins!");
    } else if (winner == -1) {
      printw("Draw.");
    } else {
      printw("Game over.");
    }
  }

  return winner;
}