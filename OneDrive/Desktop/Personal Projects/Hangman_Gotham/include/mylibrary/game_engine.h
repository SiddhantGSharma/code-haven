//
// Created by SIDDHANT on 4/27/2020.
//

#ifndef FINALPROJECT_GAME_ENGINE_H
#define FINALPROJECT_GAME_ENGINE_H

#include "read_file.h"

namespace hangman {

const std::string kVowels = "AEIOUaeiou";

class Engine {
 public:
  Engine();

  // Executes the steps in a round.
  void Step();

  // Start the game over.
  void Reset();

  // Creates a vector with all movie names from a .txt file.
  void CreateList();

  // Gets each movie from the vector of movie names.
  void GetMovieFromList();

  // If user input is a correct guess, it adds it to the blank, else,
  // it calls PopulateIncorrectGuessList().
  void PlayRound();

  // Returns if the user guess of character is correct or not.
  bool IsCorrectGuess(char user_guess);

  // If a guess is incorrect, it adds the character to a vector.
  void PopulateIncorrectGuessList();

  // For testing purposes.
  const std::vector<std::string>& getMoviesList() const;

  const std::string& getMovie() const;
  void setMovie(const std::string& movie);
  void setMovieName(const std::vector<char>& movieName);

  // For testing purposes.
  const std::vector<char>& getMovieName() const;

  const std::vector<char>& getIncompleteMovieName() const;
  void setIncompleteMovieName(const std::vector<char>& incompleteMovieName);
  char getUserGuess() const;
  void setUserGuess(char userGuess);
  const std::vector<char>& getIncorrectGuesses() const;
  int getScore() const;
  void setRoundOver(bool roundOver);
  bool isRoundOver() const;
  bool isGameOver() const;

  // Returns if a character is a vowel or not.
  // Made public for testing purposes.
  static bool isVowel(char c);

 private:
  std::vector<std::string> movies_list_;
  std::string movie_;
  std::vector<char> movie_name_;
  std::vector<char> incomplete_movie_name_;
  char user_guess_{};
  std::vector<char> incorrect_guesses_;
  int score_;
  bool round_over_;
  bool game_over_;
};

} // namespace hangman

#endif  // FINALPROJECT_GAME_ENGINE_H
