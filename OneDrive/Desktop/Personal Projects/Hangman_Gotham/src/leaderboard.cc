//
// Created by SIDDHANT on 4/27/2020.
//

#include <mylibrary/leaderboard.h>
#include <mylibrary/player.h>
#include <sqlite_modern_cpp.h>

#include <string>
#include <vector>

namespace hangman {

using std::string;
using std::vector;

LeaderBoard::LeaderBoard(const string& database_path) : database_{database_path} {
  database_ << "CREATE TABLE if not exists leaderboard (\n"
         "  name  TEXT NOT NULL,\n"
         "  score INTEGER NOT NULL\n"
         ");";
}

void LeaderBoard::AddScoreToLeaderBoard(const Player& player) {
  database_ << "insert into leaderboard (name,score) values (?,?);"
      << player.name
      << player.score;
}

vector<Player> GetPlayers(sqlite::database_binder* rows) {
  vector<Player> players;

  for (auto&& row : *rows) {
    string name;
    size_t score;
    row >> name >> score;
    Player player = {name, score};
    players.push_back(player);
  }

  return players;
}

vector<Player> LeaderBoard::RetrieveHighScores(const size_t limit) {
  auto rows = database_ << "SELECT \n"
                     "  name, \n"
                     "  score \n"
                     "FROM \n"
                     "  leaderboard \n"
                     "ORDER BY \n"
                     "  score DESC \n"
                     "LIMIT \n"
                     "  ?;" << limit;
  return GetPlayers(&rows);
}

}  // namespace hangman
