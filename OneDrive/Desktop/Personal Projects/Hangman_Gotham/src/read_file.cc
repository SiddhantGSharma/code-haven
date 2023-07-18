//
// Created by SIDDHANT on 4/27/2020.
//

#include <mylibrary/read_file.h>

namespace hangman {

std::vector<std::string> ReadFile::ReadTxtFile(const std::string &file_path) {
  std::vector<std::string> movies_list;
  std::fstream movies_list_file;
  movies_list_file.open(file_path);
  std::string line;

  while (getline(movies_list_file, line)) {
    movies_list.push_back(line);
  }

  return movies_list;
}

} // namespace hangman