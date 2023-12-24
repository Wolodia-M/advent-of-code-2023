#include "../loader.hpp"
#include "../utils.hpp"
#include "sstream"
#include "string"
#include "vector"
enum Tile : int { STARTING_POS = 0, PLOT = 1, ROCK = 2 };
int main(int argc, char **argv) {
  std::vector<std::vector<int>> tilemap;
  std::vector<std::vector<bool>> visited;
  std::vector<std::vector<bool>> last;
  std::string FILE_DATA = load_file("test41_42");
  std::stringstream FILE_DATA_STREAM(FILE_DATA);
  std::string line;
  int y = 0;
  while (std::getline(FILE_DATA_STREAM, line, '\n')) {
    tilemap.push_back({});
    visited.push_back({});
    last.push_back({});
    for (char c : line) {
      if (c == 'S') {
        tilemap.at(y).push_back(Tile::STARTING_POS);
      } else if (c == '.') {
        tilemap.at(y).push_back(Tile::PLOT);
      } else if (c == '#') {
        tilemap.at(y).push_back(Tile::ROCK);
      }
      visited.at(y).push_back(false);
      last.at(y).push_back(false);
    }
    y++;
  }
  int stx = 0;
  int sty = 0;
  printf("Map: \nRows: %lu, columns: %lu\n", tilemap.size(),
         tilemap.at(0).size());
  for (int i = 0; i < tilemap.size(); i++) {
    for (int j = 0; j < tilemap.at(0).size(); j++) {
      printf("%c", [](int t) -> char {
        switch (t) {
        case STARTING_POS:
          return 'S';
        case PLOT:
          return '.';
        case ROCK:
          return '#';
        }
      }(tilemap.at(i).at(j)));
      if (tilemap.at(i).at(j) == Tile::STARTING_POS) {
        stx = j;
        sty = i;
      }
    }
    printf("\n");
  }
  printf("Map data:\n");
  for (int i = 0; i < tilemap.size(); i++) {
    for (int j = 0; j < tilemap.at(0).size(); j++) {
      printf("%d", tilemap.at(i).at(j));
    }
    printf("\n");
  }
  printf("DFS\n");
  stack_1gb();
  dfs_explore(
      &tilemap, &visited, stx, sty,
      [](const int tile) -> bool { return tile != 2; }, 0,
      [&last](bool floor, const size_t deep, const size_t x,
              const size_t y) -> void {
        if (floor && (deep % 2 == 0) && (deep <= 6)) {
          last.at(y).at(x) = true;
        }
      });
  size_t reachable_tiles = 0;
  printf("Map after dfs:\n");
  for (int i = 0; i < tilemap.size(); i++) {
    for (int j = 0; j < tilemap.at(0).size(); j++) {
      if (last.at(i).at(j) == true) {
        printf("O");
        reachable_tiles++;
      } else if (visited.at(i).at(j) == true) {
        printf("o");
      } else {
        printf("%c", [](int t) -> char {
          switch (t) {
          case STARTING_POS:
            return 'S';
          case PLOT:
            return '.';
          case ROCK:
            return '#';
          }
        }(tilemap.at(i).at(j)));
      }
    }
    printf("\n");
  }
  printf("Reachable plots: %lu\n", reachable_tiles);
}