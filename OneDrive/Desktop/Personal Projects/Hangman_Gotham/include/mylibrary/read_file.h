//
// Created by SIDDHANT on 4/27/2020.
//

#ifndef FINALPROJECT_READ_FILE_H
#define FINALPROJECT_READ_FILE_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

namespace hangman {

class ReadFile {
 public:
  // Read text from a .txt file and store it into a string vector
  static std::vector<std::string> ReadTxtFile(const std::string &file_path);
};

} // namespace hangman

#endif  // FINALPROJECT_READ_FILE_H
