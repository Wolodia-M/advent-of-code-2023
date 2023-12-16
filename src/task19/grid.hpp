#include "../loader.hpp"
#include "algorithm"
#include "sstream"
#include "string"
#include "vector"
enum Pipes {
  VERTICAL = '|',
  HORIZONTAL = '-',
  UP_RIGHT_TURN = 'L',
  UP_LEFT_TURN = 'J',
  BOTTOM_RIGHT_TURN = 'F',
  BOTTOM_LEFT_TURN = '7',
  START_POS = 'S',
  NONE = '.',
};
enum Direction { ERR, UP, RIGHT, DOWN, LEFT };
class PipeGrid {
private:
  std::vector<std::vector<Pipes>> grid; // .at(y).at(x)
  size_t start_x, start_y;
  void write(size_t x, size_t y, Pipes pipe) { grid.at(y).at(x) = pipe; }
  Pipes read(size_t x, size_t y) { return grid.at(y).at(x); }
  void alloc_line() { this->grid.push_back({}); }
  void alloc_char(size_t num) {
    for (size_t i = 0; i < num; i++) {
      this->grid.back().push_back(Pipes::NONE);
    }
  }
  size_t lines() { return this->grid.size(); }
  size_t columns() { return this->grid.at(0).size(); }
  bool pipe_connector_down(Pipes pipe) {
    return ((pipe == Pipes::VERTICAL) || (pipe == Pipes::BOTTOM_LEFT_TURN) ||
            (pipe == Pipes::BOTTOM_RIGHT_TURN));
  }
  bool pipe_connector_up(Pipes pipe) {
    return ((pipe == Pipes::VERTICAL) || (pipe == Pipes::UP_LEFT_TURN) ||
            (pipe == Pipes::UP_RIGHT_TURN));
  }
  bool pipe_connector_left(Pipes pipe) {
    return ((pipe == Pipes::HORIZONTAL) || (pipe == Pipes::BOTTOM_LEFT_TURN) ||
            (pipe == Pipes::UP_LEFT_TURN));
  }
  bool pipe_connector_right(Pipes pipe) {
    return ((pipe == Pipes::HORIZONTAL) || (pipe == Pipes::BOTTOM_RIGHT_TURN) ||
            (pipe == Pipes::UP_RIGHT_TURN));
  }
  std::vector<Direction> get_connected_pipes(Pipes pipe) {
    std::vector<Direction> ret;
    if (this->pipe_connector_down(pipe)) {
      ret.push_back(Direction::DOWN);
    }
    if (this->pipe_connector_up(pipe)) {
      ret.push_back(Direction::UP);
    }
    if (this->pipe_connector_left(pipe)) {
      ret.push_back(Direction::LEFT);
    }
    if (this->pipe_connector_right(pipe)) {
      ret.push_back(Direction::RIGHT);
    }
    return ret;
  }
  size_t get_uncanceled_direction(Direction prev, std::vector<Direction> curr) {
    size_t ret;
    switch (curr.at(0)) {
    case ERR:
      break;
    case UP:
      if (prev != DOWN) {
        ret = 0;
      }
      break;
    case RIGHT:
      if (prev != LEFT) {
        ret = 0;
      }
      break;
    case DOWN:
      if (prev != UP) {
        ret = 0;
      }
      break;
    case LEFT:
      if (prev != RIGHT) {
        ret = 0;
      }
      break;
    }
    switch (curr.at(1)) {
    case ERR:
      break;
    case UP:
      if (prev != DOWN) {
        ret = 1;
      }
      break;
    case RIGHT:
      if (prev != LEFT) {
        ret = 1;
      }
      break;
    case DOWN:
      if (prev != UP) {
        ret = 1;
      }
      break;
    case LEFT:
      if (prev != RIGHT) {
        ret = 1;
      }
      break;
    }
    return ret;
  }

public:
  PipeGrid(std::string path) {
    std::string FILE_DATA = load_file(path);
    std::stringstream FILE_DATA_STREAM(FILE_DATA);
    std::string line;
    size_t y = 0;
    size_t x = 0;
    while (std::getline(FILE_DATA_STREAM, line, '\n')) {
      this->alloc_line();
      for (char c : line) {
        // printf("%c", c);
        this->alloc_char(1);
        this->write(x, y, static_cast<Pipes>(c));
        x++;
      }
      y++;
      x = 0;
      // printf("\n");
    }
    for (size_t i = 0; i < this->lines(); i++) {
      for (size_t j = 0; j < this->columns(); j++) {
        Pipes pipe = this->read(j, i);
        if (pipe == Pipes::START_POS) {
          this->start_x = j;
          this->start_y = i;
          Pipes up = Pipes::NONE;
          Pipes down = Pipes::NONE;
          Pipes right = Pipes::NONE;
          Pipes left = Pipes::NONE;
          try {
            up = this->read(j, i - 1);
          } catch (std::exception &e) {
          }
          try {
            down = this->read(j, i + 1);
          } catch (std::exception &e) {
          }
          try {
            right = this->read(j + 1, i);
          } catch (std::exception &e) {
          }
          try {
            left = this->read(j - 1, i);
          } catch (std::exception &e) {
          }
          if (this->pipe_connector_down(up) && this->pipe_connector_up(down)) {
            this->write(j, i, Pipes::VERTICAL);
          } else if (this->pipe_connector_left(right) &&
                     this->pipe_connector_right(left)) {
            this->write(j, i, Pipes::HORIZONTAL);
          } else if (this->pipe_connector_down(up) &&
                     this->pipe_connector_left(right)) {
            this->write(j, i, Pipes::UP_RIGHT_TURN);
          } else if (this->pipe_connector_down(up) &&
                     this->pipe_connector_right(left)) {
            this->write(j, i, Pipes::UP_LEFT_TURN);
          } else if (this->pipe_connector_up(down) &&
                     this->pipe_connector_left(right)) {
            this->write(j, i, Pipes::BOTTOM_RIGHT_TURN);
          } else if (this->pipe_connector_up(down) &&
                     this->pipe_connector_right(left)) {
            this->write(j, i, Pipes::BOTTOM_LEFT_TURN);
          }
        }
      }
    }
    printf("Pipe grid:\n");
    for (size_t i = 0; i < this->lines(); i++) {
      for (size_t j = 0; j < this->columns(); j++) {
        printf("%c", static_cast<char>(this->read(j, i)));
      }
      printf("\n");
    }
    printf("Start: x: %lu, y: %lu, pipe: %c\n", this->start_x, this->start_y,
           this->read(this->start_x, this->start_y));
  }
  size_t find_largest_distance() {
    std::vector<size_t> counter = {0, 0};
    std::vector<size_t> x = {this->start_x, this->start_x};
    std::vector<size_t> y = {this->start_y, this->start_y};
    std::vector<size_t> prev_used_dir = {0, 1};
    std::vector<std::vector<Direction>> prev_dirs = {{ERR, ERR}, {ERR, ERR}};
    std::vector<std::vector<Direction>> curr_dirs = {{ERR, ERR}, {ERR, ERR}};
    curr_dirs[0] = this->get_connected_pipes(this->read(x[0], y[0]));
    curr_dirs[1] = this->get_connected_pipes(this->read(x[1], y[1]));
    printf("%d, %d, %d, %d\n", curr_dirs[0][0], curr_dirs[0][1],
           curr_dirs[1][0], curr_dirs[1][1]);
    switch (curr_dirs[0][0]) {
    case ERR:
      throw std::exception();
    case UP:
      y[0]--;
      break;
    case RIGHT:
      x[0]++;
      break;
    case DOWN:
      y[0]++;
      break;
    case LEFT:
      x[0]--;
      break;
    }
    switch (curr_dirs[1][1]) {
    case ERR:
      throw std::exception();
    case UP:
      y[1]--;
      break;
    case RIGHT:
      x[1]++;
      break;
    case DOWN:
      y[1]++;
      break;
    case LEFT:
      x[1]--;
      break;
    }
    prev_dirs = curr_dirs;
    bool br = false;
    while (true) {
      printf("0: %c, 1: %c\n", this->read(x[0], y[0]), this->read(x[1], y[1]));
      curr_dirs[0] = this->get_connected_pipes(this->read(x[0], y[0]));
      curr_dirs[1] = this->get_connected_pipes(this->read(x[1], y[1]));
      // printf("Pd: %lu, Pd[0]:%lu, Pd[1]:%lu, Cd:%lu, Cd[0]:%lu, Cd[1]:%lu, "
      //        "Pud:%lu, Pud[0]%lu, Pud[1]:%lu\n",
      //        prev_dirs.size(), prev_dirs[0].size(), prev_dirs[1].size(),
      //        curr_dirs.size(), curr_dirs[0].size(), curr_dirs[1].size(),
      //        prev_used_dir.size(), prev_used_dir[0], prev_used_dir[1]);
      fflush(stdout);
      size_t dir1 = this->get_uncanceled_direction(
          prev_dirs[0][prev_used_dir[0]], curr_dirs[0]);
      size_t dir2 = this->get_uncanceled_direction(
          prev_dirs[1][prev_used_dir[1]], curr_dirs[1]);
      prev_used_dir[0] = dir1;
      prev_used_dir[1] = dir2;
      switch (curr_dirs[0][dir1]) {
      case ERR:
        break;
      case UP:
        y[0]--;
        break;
      case RIGHT:
        x[0]++;
        break;
      case DOWN:
        y[0]++;
        break;
      case LEFT:
        x[0]--;
        break;
      }
      switch (curr_dirs[1][dir2]) {
      case ERR:
        break;
      case UP:
        y[1]--;
        break;
      case RIGHT:
        x[1]++;
        break;
      case DOWN:
        y[1]++;
        break;
      case LEFT:
        x[1]--;
        break;
      }
      counter[0]++;
      counter[1]++;
      if (br) {
        break;
      }
      if ((x[0] == x[1]) && (y[0] == y[1])) {
        br = true;
      }
      prev_dirs = curr_dirs;
    }
    return std::max(counter[0], counter[1]);
  }
};