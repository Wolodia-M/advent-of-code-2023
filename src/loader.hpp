#include "cstdlib"
#include "fstream"
#include "string"
#ifndef FILE_LOADER_HPP
#define FILE_LOADER_HPP
#define BASE "data/";
inline std::string load_file(std::string file) {
  std::fstream f;
  std::string ret;
  f.open("data/" + file);
  std::getline(f, ret, '\0');
  f.close();
  return ret;
}
#endif // FILE_LOADER_HPP