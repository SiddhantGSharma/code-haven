// Copyright (c) 2020 [Siddhant Sharma]. All rights reserved.

#include "hangman_app.h"

#include <cinder/app/App.h>


namespace hangman_app {

using cinder::app::KeyEvent;
using cinder::Color;
using cinder::app::getWindowCenter;
using cinder::TextBox;
using cinder::ColorA;
using cinder::Rectf;

const char kNormalFont[] = "Helvetica";
const std::string kPlayerName = "Siddhant Sharma";
const char kDbPath[] = "hangman.db";
const int kLimit = 3;

cinder::audio::VoiceRef openingSound;

Hangman::Hangman()
  : state_{GameState::kPlaying},
    movie_name_{""},
    printed_game_over_{false},
    leaderboard_{cinder::app::getAssetPath(kDbPath).string()},
    game_over_text_{"\"I'll get you next time, Batman.\""} {};

void Hangman::setup() {
  engine_.CreateList();
  movie_name_ = engine_.getMovie();

  // for background music
  cinder::audio::SourceFileRef sourceFile = cinder::audio::load(
      cinder::app::loadAsset("Backgroung Music.mp3"));
  openingSound = cinder::audio::Voice::create(sourceFile);
  openingSound->start();
}

void Hangman::update() {
  if (engine_.isGameOver()) {
    // if game gets over after player runs out of guesses
    if (engine_.getIncorrectGuesses().size() == 8) {
      game_over_text_ = "\"I guess you were too slow, Batman.\"";
    }

    state_ = GameState::kGameOver;
  }

  if (state_ == GameState::kGameOver) {
    openingSound->stop();

    if (top_players_.empty()) {
      leaderboard_.AddScoreToLeaderBoard(
          {kPlayerName,static_cast<size_t>(engine_.getScore())});
      top_players_ = leaderboard_.RetrieveHighScores(kLimit);
      // It is crucial that this vector be populated, given that `kLimit` > 0.
      assert(!top_players_.empty());
    }

    return;
  };

  if (engine_.isRoundOver()) {
    engine_.GetMovieFromList();
    movie_name_ = engine_.getMovie();
    return;
  }

  engine_.Step();
}

void Hangman::draw() {
  cinder::gl::enableAlphaBlending();

  if (state_ == GameState::kGameOver) {
    if (!printed_game_over_) cinder::gl::clear(Color(
         1, 0, 0));
    DrawGameOver();
    return;
  }

  cinder::gl::clear(Color(1,1,1));
  DrawBackground();
  DrawMovieName();
  DrawHangman();
}

template <typename C>
void PrintText(const std::string& text, const C& color,
    const cinder::ivec2& size, const cinder::vec2& loc) {
  cinder::gl::color(color);

  auto box = TextBox()
      .alignment(TextBox::CENTER)
      .font(cinder::Font(kNormalFont, 55))
      .size(size)
      .color(color)
      .backgroundColor(ColorA(0, 0, 0, 0))
      .text(text);

  const auto box_size = box.getSize();
  const cinder::vec2 locp = {loc.x - box_size.x / 2,
                             loc.y - box_size.y / 2};
  const auto surface = box.render();
  const auto texture = cinder::gl::Texture::create(surface);
  cinder::gl::draw(texture, locp);
}

// Only for printing incorrect guesses on the screen.
template <typename C>
void PrintIncorrectGuessText(const std::string& text, const C& color,
               const cinder::ivec2& size, const cinder::vec2& loc) {
  cinder::gl::color(color);

  auto box = TextBox()
      .alignment(TextBox::CENTER)
      .font(cinder::Font(kNormalFont, 35))
      .size(size)
      .color(color)
      .backgroundColor(ColorA(0, 0, 0, 0))
      .text(text);

  const auto box_size = box.getSize();
  const cinder::vec2 locp = {loc.x - box_size.x / 2,
                             loc.y - box_size.y / 2};
  const auto surface = box.render();
  const auto texture = cinder::gl::Texture::create(surface);
  cinder::gl::draw(texture, locp);
}

void Hangman::DrawBackground() {
  // for personalised background image
  cinder::gl::color(1,1,1);
  cinder::gl::Texture2dRef texture = cinder::gl::Texture::create(
      cinder::loadImage(loadAsset(
          "Background Image.jpg")));
  cinder::gl::draw(texture);
}

void Hangman::DrawMovieName() {
  const cinder::vec2 center = getWindowCenter();
  const cinder::ivec2 size = {800, 50};
  const Color color = Color::black();
  std::vector<char> to_convert = engine_.getIncompleteMovieName();
  // converting vector<char> to string for printing
  std::string to_print(to_convert.begin(), to_convert.end());

  PrintText("\"Guess this movie of yours, Batman!\"", color, size,
      {center.x, center.y - 160});
  PrintText(to_print, color, size, {center.x, center.y - 100});
}

void Hangman::DrawHangman() {
  const cinder::vec2 center = getWindowCenter();
  int incorrect_guesses = engine_.getIncorrectGuesses().size();

  //nothing to draw
  if (incorrect_guesses == 0) {
    return;
  }

  // to print incorrect guesses of the user
  const cinder::ivec2 size = {800, 50};
  const Color color = Color::black();
  size_t row = 1;
  PrintIncorrectGuessText("Incorrect Guesses : ", color, size,
      {center.x + 225,center.y - 50});

  // to print each incorrectly guessed character
  for (char character : engine_.getIncorrectGuesses()) {
    std::string to_print;
    to_print += character;
    PrintIncorrectGuessText(to_print, color, size,
              {center.x + 225, center.y - 50 + (++row) * 30});
  }

  // to draw hangman at a specific location
  Rectf drawRect(center.x - 200, center.y - 60, center.x + 200,
                 center.y + 400);

  if (incorrect_guesses == 1) {
    cinder::gl::Texture2dRef texture = cinder::gl::Texture::create(
        cinder::loadImage(loadAsset(
            "Hangman 1.png")));
    cinder::gl::draw(texture, drawRect);
  }

  if (incorrect_guesses == 2) {
    cinder::gl::Texture2dRef texture = cinder::gl::Texture::create(
        cinder::loadImage(loadAsset(
            "Hangman 2.png")));
    cinder::gl::draw(texture, drawRect);
  }

  if (incorrect_guesses == 3) {
    cinder::gl::Texture2dRef texture = cinder::gl::Texture::create(
        cinder::loadImage(loadAsset(
            "Hangman 3.png")));
    cinder::gl::draw(texture, drawRect);
  }

  if (incorrect_guesses == 4) {
    cinder::gl::Texture2dRef texture = cinder::gl::Texture::create(
        cinder::loadImage(loadAsset(
            "Hangman 4.png")));
    cinder::gl::draw(texture, drawRect);
  }

  if (incorrect_guesses == 5) {
    cinder::gl::Texture2dRef texture = cinder::gl::Texture::create(
        cinder::loadImage(loadAsset(
            "Hangman 5.png")));
    cinder::gl::draw(texture, drawRect);
  }

  if (incorrect_guesses == 6) {
    cinder::gl::Texture2dRef texture = cinder::gl::Texture::create(
        cinder::loadImage(loadAsset(
            "Hangman 6.png")));
    cinder::gl::draw(texture, drawRect);
  }

  if (incorrect_guesses == 7) {
    cinder::gl::Texture2dRef texture = cinder::gl::Texture::create(
        cinder::loadImage(loadAsset(
            "Hangman 7.png")));
    cinder::gl::draw(texture, drawRect);
  }

  if (incorrect_guesses == 8) {
    cinder::gl::Texture2dRef texture = cinder::gl::Texture::create(
        cinder::loadImage(loadAsset(
            "Hangman 8.png")));
    cinder::gl::draw(texture, drawRect);
  }
}

void Hangman::DrawGameOver() {
  // for background image
  cinder::gl::color(1,1,1);
  cinder::gl::Texture2dRef texture = cinder::gl::Texture::create(
      cinder::loadImage(loadAsset(
          "Game Over Image.jpg")));
  cinder::gl::draw(texture);

  //for printing text on screen.
  const cinder::vec2 center = getWindowCenter();
  const cinder::ivec2 size = {800, 50};
  const Color color = Color::black();
  std::string score = std::to_string(engine_.getScore());
  size_t row = 0;

  PrintText(":: Game Over ::", color, size, {center.x + 225,
                                       center.y + (++row) * 50});
  PrintText(game_over_text_ , color, size,
      {center.x + 225,center.y + (++row) * 50});
  PrintText("Current Score : " + score, color, size,
      {center.x + 225,center.y + (++row) * 50});
  PrintText("Highest Scores :", color, size, {center.x + 225,
                                              center.y + (++row) * 50});

  for (const hangman::Player& player : top_players_) {
    std::stringstream ss;
    ss << player.name << " - " << player.score;
    PrintText(ss.str(), color, size,
        {center.x + 225, center.y + (++row) * 50});
  }

  printed_game_over_ = true;
}

void Hangman::keyDown(KeyEvent event) {
  // to take user input as alphabet or digit
  char character = event.getChar();

  if (isalpha(character) || isdigit(character)) {
    engine_.setUserGuess(character);
  }
}

void Hangman::keyUp(KeyEvent event) {
  // to avoid alphabet/digit input when key is released.
  char character = event.getChar();

  if (isalpha(character) || isdigit(character)) {
    engine_.setUserGuess(' ');
  }
}

void Hangman::Reset() {
  engine_.Reset();
  state_ = GameState::kPlaying;
  movie_name_ = "";
  printed_game_over_ = false;
  game_over_text_ = "\"I'll get you next time, Batman.\"";
}

}  // namespace hangman_app
