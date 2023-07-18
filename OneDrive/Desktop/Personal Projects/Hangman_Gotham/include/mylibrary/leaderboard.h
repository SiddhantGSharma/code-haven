//
// Created by SIDDHANT on 4/27/2020.
//

#ifndef FINALPROJECT_LEADERBOARD_H
#define FINALPROJECT_LEADERBOARD_H

#include "player.h"

#include <sqlite_modern_cpp.h>

#include <string>
#include <vector>

namespace hangman {

class LeaderBoard {
 public:
  // Creates a new leaderboard table if it doesn't already exist.
  explicit LeaderBoard(const std::string& database_path);

  // Adds a player to the leaderboard.
  void AddScoreToLeaderBoard(const Player&);

  // Returns a list of the players with the highest scores, in decreasing order.
  // The size of the list should be no greater than `limit`.
  std::vector<Player> RetrieveHighScores(const size_t limit);

 private:
  sqlite::database database_;
};

} // namespace hangman

#endif  // FINALPROJECT_LEADERBOARD_H
