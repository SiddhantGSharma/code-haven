//
// Created by SIDDHANT on 4/27/2020.
//

#ifndef FINALPROJECT_PLAYER_H
#define FINALPROJECT_PLAYER_H

#include <string>

namespace hangman {

struct Player {
  Player(const std::string& name, size_t score) : name(name), score(score) {}
  std::string name;
  size_t score;
};

} // namespace hangman

#endif  // FINALPROJECT_PLAYER_H
