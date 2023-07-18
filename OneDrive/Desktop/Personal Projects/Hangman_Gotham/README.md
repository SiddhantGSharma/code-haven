# Hangman Gotham

[![license](https://img.shields.io/badge/license-MIT-green)](LICENSE)
[![docs](https://img.shields.io/badge/docs-yes-brightgreen)](docs/README.md)

Read [this document](https://cliutils.gitlab.io/modern-cmake/chapters/basics/structure.html) to understand the project
layout.

Author: Siddhant Sharma - [`sharma63@illinois.edu`](mailto:sharma63@illinois.edu)

---

The Batman themed **Hangman** game gives the 
player a movie name to guess which has only the vowels provided. The player has 
to guess an alphabet or digit of the name. With each correct guess, all 
instances of that alphabet or digit in the game is populated. With a wrong guess, 
a silhouette of a hanging man starts drawing. However, once a guess is wrong, 
the same character would not be counted as a wrong guess the second time. Hence,
if you accidentally press a key twice, if it is a wrong guess, only one part of 
the hangman would be drawn. The game is over in two case:
1) The player correctly guesses the name of all the movies.
2) The hangman is completely drawn (in which case the player loses the game)

In my version, the puzzles would be given by Joker and you, as Batman, would
have to solve each puzzle to save the civilian or the civilian would be hung.
Your current game score is displayed in the end along with the top 3 scores.