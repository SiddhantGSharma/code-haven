//
// Created by SIDDHANT on 4/27/2020.
//

#include <mylibrary/game_engine.h>

namespace hangman {

Engine::Engine() {
  score_ = 0;
  round_over_ = false;
  game_over_ = false;
}

void Engine::CreateList() {
  movies_list_ = hangman::ReadFile::ReadTxtFile("C:\\Users\\SIDDHANT\\cinder_0.9.2_vc2015\\my-projects\\final-project-SiddhantGSharma\\assets\\movie_list.txt");

  GetMovieFromList();
}

void Engine::GetMovieFromList() {
  // game is over when all movies are read from file
  if (movies_list_.empty()) {
    game_over_ = true;
    return;
  }

  std::string movie = movies_list_.back();
  movies_list_.pop_back();
  setMovie(movie);
  std::vector<char> temp(movie.begin(), movie.end());
  std::vector<char> temp_two;

  // creating incomplete movie name for guessing
  for (char i : temp) {
    if (isVowel(i) || i == ' ') {
      temp_two.push_back(i);
    } else {
      temp_two.push_back('-');
    }
  }

  setMovieName(temp);
  setIncompleteMovieName(temp_two);

  // incorrect guesses vector should reset for each movie name
  while (!incorrect_guesses_.empty()) {
    incorrect_guesses_.pop_back();
  }

  // round starts
  setRoundOver(false);
}

void Engine::Step() {
  if (movie_name_ == incomplete_movie_name_) {
    score_ += movie_name_.size();
    round_over_= true;
  } else {
    PlayRound();
  }
}

void Engine::PlayRound() {
  if (IsCorrectGuess(getUserGuess())) {
    // populate all instances of the guessed character in movie name blanks
    for (size_t i = 0; i < movie_name_.size(); i++) {
      if (movie_name_.at(i) == user_guess_) {
        incomplete_movie_name_.at(i) = user_guess_;
      }
    }
  } else {
    // guess is incorrect
    PopulateIncorrectGuessList();
  }
}

bool Engine::IsCorrectGuess(char user_guess) {
  for (size_t i = 0; i < movie_name_.size(); i++) {
    // Guess is correct if character is there in movie_name_ and not there
    // in incomplete_movie_name_ so it can be filled.
    if (user_guess == movie_name_.at(i) &&
        user_guess != incomplete_movie_name_.at(i)) {
      return true;
    }
  }

  return false;
}

void Engine::PopulateIncorrectGuessList() {
  if (isalpha(user_guess_) || isdigit(user_guess_)) {
    for (char character : movie_name_) {
      if (user_guess_ == character) return;
    }

    if (incorrect_guesses_.empty()) {
      incorrect_guesses_.push_back(user_guess_);
      return;
    }

    // to avoid repetition of a single character in a round.
    for (char incorrect_guess : incorrect_guesses_) {
      if (user_guess_ == incorrect_guess) {
        return;
      }
    }

    incorrect_guesses_.push_back(user_guess_);
  }

  if (incorrect_guesses_.size() == 8) {
    game_over_ = true;
  }
}

bool Engine::isVowel(char c) {
  for (char vowel : kVowels) {
    if (vowel == c) {
      return true;
    }
  }

  return false;
}

const std::vector<std::string>& Engine::getMoviesList() const {
  return movies_list_;
}

const std::string& Engine::getMovie() const { return movie_; }

void Engine::setMovie(const std::string& movie) { Engine::movie_ = movie; }

void Engine::setMovieName(const std::vector<char>& movieName) {
  movie_name_ = movieName;
}

const std::vector<char>& Engine::getMovieName() const { return movie_name_; }

const std::vector<char>& Engine::getIncompleteMovieName() const {
  return incomplete_movie_name_;
}

void Engine::setIncompleteMovieName(
    const std::vector<char>& incompleteMovieName) {
  incomplete_movie_name_ = incompleteMovieName;
}

char Engine::getUserGuess() const { return user_guess_; }

void Engine::setUserGuess(char userGuess) { user_guess_ = userGuess; }

const std::vector<char>& Engine::getIncorrectGuesses() const {
  return incorrect_guesses_;
}

int Engine::getScore() const { return score_; }

void Engine::setRoundOver(bool roundOver) { round_over_ = roundOver; }

bool Engine::isRoundOver() const { return round_over_; }

bool Engine::isGameOver() const { return game_over_; }

void Engine::Reset() {
  score_ = 0;
  round_over_ = false;
  user_guess_ = '\0';
  game_over_ = false;

  while (!incorrect_guesses_.empty()) {
    incorrect_guesses_.pop_back();
  }

  while (!movies_list_.empty()) {
    movies_list_.pop_back();
  }

  while (!movie_name_.empty()) {
    movie_name_.pop_back();
  }

  while (!incomplete_movie_name_.empty()) {
    incomplete_movie_name_.pop_back();
  }
}

} // namespace hangman