#include "../loader.hpp"
#include "../utils.hpp"
#include "algorithm"
#include "cmath"
#include "cstdio"
#include "cstdlib"
#include "sstream"
#include "string"
#include "vector"
enum Direction {
  UP = '3',
  RIGHT = '0',
  DOWN = '1',
  LEFT = '2',
};
struct DigEntry {
  Direction dir;
  double len;
};
struct Vertex {
  double x;
  double y;
  std::strong_ordering operator<=>(const Vertex &other) const {
    double this_theta = 0;
    if (this->x != 0) {
      double this_theta = atan(this->y / this->x);
    } else {
      this_theta = (this->y > 0) ? M_PI / 2 : M_PI * 3;
    }
    double other_theta = 0;
    if (other.x != 0) {
      other_theta = atan(other.y / other.x);
    } else {
      this_theta = (this->y > 0) ? M_PI / 2 : M_PI * 3;
    }
    if (this_theta < other_theta) {
      return std::strong_ordering::less;
    } else if (this_theta > other_theta) {
      return std::strong_ordering::greater;
    }
    return std::strong_ordering::equal;
  }
};
class Grid {
private:
  std::vector<DigEntry> commands;
  std::vector<Vertex> poligon;

public:
  Grid(std::string file) {
    std::string FILE_DATA = load_file(file);
    std::stringstream FILE_DATA_STREAM(FILE_DATA);
    std::string line;
    std::string elem;
    while (std::getline(FILE_DATA_STREAM, line, '\n')) {
      std::stringstream line_stream(line);
      int step = 0;
      DigEntry dge;
      std::string elem = line.substr(line.find_first_of('#') + 1, 6);
      dge.dir = static_cast<Direction>(elem.back());
      dge.len = std::strtoul(elem.substr(0, 5).c_str(), NULL, 16);
      printf(
          "%s %f\n",
          [&dge]() -> std::string {
            switch (dge.dir) {
            case UP:
              return "U";
              break;
            case RIGHT:
              return "R";
              break;
            case DOWN:
              return "D";
              break;
            case LEFT:
              return "L";
              break;
            }
          }()
                          .c_str(),
          dge.len);
      this->commands.push_back(dge);
    }
  }
  void perform_digging() {
    double x = 0;
    double y = 0;
    for (auto dge : this->commands) {
      switch (dge.dir) {
      case UP:
        y += dge.len;
        break;
      case RIGHT:
        x += dge.len;
        break;
      case DOWN:
        y -= dge.len;
        break;
      case LEFT:
        x -= dge.len;
        break;
      }
      this->poligon.push_back({.x = x, .y = y});
    }
  }
  // Done using "Shoelace formula for polygonal area"
  double get_area() {
    // std::ranges::sort(this->poligon,
    //                   [](Vertex a, Vertex b) -> bool { return a < b; });
    for (auto vertex : this->poligon) {
      printf("x: %f, y: %f\n", vertex.x, vertex.y);
    }
    double ret = 0;
    // abs( (sum(x[0]*y[1] + ... x[n-1]*y[n]) + x[N]*y[0]) -
    //   (sum(x[1]*y[0] + ... x[n]*y[n-1]) + x[0]*y[N])
    //  ) / 2
    for (int n = 1; n < this->poligon.size() - 1; n++) {
      ret += this->poligon.at(n - 1).x * this->poligon.at(n).y;
    }
    ret += this->poligon.back().x * this->poligon.front().y;
    for (int n = 1; n < this->poligon.size() - 1; n++) {
      ret -= this->poligon.at(n).x * this->poligon.at(n - 1).y;
    }
    ret -= this->poligon.front().x * this->poligon.back().y;
    ret = std::abs(ret) / 2;
    for (auto dge : this->commands) {
      ret += dge.len / 2;
    }
    ret += 1;
    return ret;
  }
};