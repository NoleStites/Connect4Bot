// AI agents for connect 4.
// Each agent corresponds a function. AI functions assume that the board 
// is not full and may crash the game if given an invalid board state.
// This file also defines the main types of players (including human),
// and the strings used in command-line flags to pick player types.

#ifndef AI_H_
#define AI_H_

#include <string>
#include <iostream>

using namespace std;

#include "game.h"

// AI chooses first open space.
// Corresponds to kFirst player type.
int FirstOpenAgent(const Board &board, int ai_player);

// AI chooses a random open space.
// Corresponds to kRandom player type.
int RandomAgent(const Board &board, int ai_player);

// AI strategy HERE.
// Corresponds to kNoleBot player type.
int NoleBotAgent(const Board &board, int ai_player);

// String names of player types.
PlayerType GetPlayerType(const std::string &arg);

#endif