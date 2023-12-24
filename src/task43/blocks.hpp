#pragma once
#include "../loader.hpp"
#include "cstddef"
#include "cstdint"
#include "grid_container.hpp"
#include "sstream"
#include "vector"
const size_t MAX_X = 512;
const size_t MAX_Y = 512;
const size_t MAX_Z = 512;
union Position {
  uint8_t flags;
  struct {
    unsigned ocupied : 1 = 0;
    unsigned falling : 1 = 0;
  };
};
struct Block {
  uint16_t x1;
  uint16_t y1;
  uint16_t z1;
  uint16_t x2;
  uint16_t y2;
  uint16_t z2;
  bool in_grid = false;
};
class Blocks {
private:
  Grid<Position, MAX_X, MAX_Y, MAX_Z> grid;
  std::vector<Block> blocks;

public:
  Blocks(std::string file) {
    {
      Position pos;
      pos.falling = 0;
      pos.ocupied = 0;
      for (int i = 0; i < MAX_X; i++) {
        for (int j = 0; j < MAX_X; j++) {
          for (int k = 0; k < MAX_Z; k++) {
            this->grid.set(i, j, k, pos);
          }
        }
      }
    }
    {
      Position pos;
      pos.falling = 0;
      pos.ocupied = 1;
      for (int i = 0; i < MAX_X; i++) {
        for (int j = 0; j < MAX_X; j++) {
          this->grid.set(i, j, 0, pos);
        }
      }
    }
    {
      std::string FILE_DATA = load_file(file);
      std::stringstream FILE_DATA_STREAM(FILE_DATA);
      std::string line;
      while (std::getline(FILE_DATA_STREAM, line, '\n')) {
        Block b;
        // printf("L: %s\n", line.c_str());
        b.x1 = std::atoi(line.substr(0, line.find(',')).c_str());
        line = line.substr(line.find(',') + 1);
        // printf("L: %s\n", line.c_str());
        b.y1 = std::atoi(line.substr(0, line.find(',')).c_str());
        line = line.substr(line.find(',') + 1);
        // printf("L: %s\n", line.c_str());
        b.z1 = std::atoi(line.substr(0, line.find('~')).c_str());
        line = line.substr(line.find('~') + 1);
        // printf("L: %s\n", line.c_str());
        b.x2 = std::atoi(line.substr(0, line.find(',')).c_str());
        line = line.substr(line.find(',') + 1);
        // printf("L: %s\n", line.c_str());
        b.y2 = std::atoi(line.substr(0, line.find(',')).c_str());
        line = line.substr(line.find(',') + 1);
        // printf("L: %s\n", line.c_str());
        b.z2 = std::atoi(line.c_str());
        printf("Block: {%d, %d, %d},{%d, %d, %d}\n", b.x1, b.y1, b.z1, b.x2,
               b.y2, b.z2);
        this->blocks.push_back(b);
      }
    }
    {
      Position p;
      p.ocupied = 1;
      p.falling = 1;
      for (auto block : this->blocks) {
        for (int x = block.x1; x <= block.x2; x++) {
          for (int y = block.y1; y <= block.y2; y++) {
            for (int z = block.z1; z <= block.z2; z++) {
              this->grid.set(x, y, z, p);
            }
          }
        }
      }
    }
    printf("Init of the grid completed!\n");
  }
};