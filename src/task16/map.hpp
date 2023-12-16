#include "../loader.hpp"
#include "cstdio"
#include "cstdlib"
#include "sstream"
#include "string"
#include "vector"
struct MapEntry {
  char name[3];
  char left_id[3];
  char right_id[3];
  MapEntry *left;
  MapEntry *right;
  MapEntry &operator=(const MapEntry &other) {
    this->right = other.right;
    this->left = other.left;
    this->name[0] = other.name[0];
    this->name[1] = other.name[1];
    this->name[2] = other.name[2];
    this->left_id[0] = other.left_id[0];
    this->left_id[1] = other.left_id[1];
    this->left_id[2] = other.left_id[2];
    this->right_id[0] = other.right_id[0];
    this->right_id[1] = other.right_id[1];
    this->right_id[2] = other.right_id[2];
    return *this;
  }
};
enum Direction { Right = 'R', Left = 'L' };
class Map {
public:
  const char START[3] = {'A', 'A', 'A'};
  const char END[3] = {'Z', 'Z', 'Z'};

private:
  MapEntry *data = NULL;
  volatile size_t len = 0; // Number of elems, last used index + 1
  volatile size_t allocated = 0;
  inline bool compare_node_id(const char lhs[3], const char rhs[3]) {
    return ((lhs[0] == rhs[0]) && (lhs[1] == rhs[1]) && (lhs[2] == rhs[2]));
  }

public:
  Map(std::string file) {
    std::string FILE_DATA = load_file(file);
    std::stringstream FILE_DATA_STREAM(FILE_DATA);
    std::string line;
    std::getline(FILE_DATA_STREAM, line, '\n');
    std::getline(FILE_DATA_STREAM, line, '\n');
    while (std::getline(FILE_DATA_STREAM, line, '\n')) {
      if (len == allocated) {
        this->allocated = (this->allocated * 2 + 1);
        this->data = (MapEntry *)std::realloc(this->data, this->allocated *
                                                              sizeof(MapEntry));
      }
      this->data[this->len] =
          (MapEntry){.name = {line[0], line[1], line[2]},
                     .left_id = {line[7], line[8], line[9]},
                     .right_id = {line[12], line[13], line[14]},
                     .left = NULL,
                     .right = NULL};
      this->len++;
    }
    for (int i = 0; i < this->len; i++) {
      for (int j = 0; j < this->len; j++) {
        if (this->compare_node_id(this->data[i].left_id, this->data[j].name)) {
          this->data[i].left = &(this->data[j]);
        }
        if (this->compare_node_id(this->data[i].right_id, this->data[j].name)) {
          this->data[i].right = &(this->data[j]);
        }
        if ((this->data[i].right != NULL) && (this->data[i].left != NULL)) {
          break;
        }
      }
    }
    printf("Map: \n");
    for (int i = 0; i < this->len; i++) {
      printf("Node '%c%c%c' -> R: '%c%c%c', L: '%c%c%c'\n",
             this->data[i].name[0], this->data[i].name[1],
             this->data[i].name[2], this->data[i].right->name[0],
             this->data[i].right->name[1], this->data[i].right->name[2],
             this->data[i].left->name[0], this->data[i].left->name[1],
             this->data[i].left->name[2]);
    }
  }
  std::vector<MapEntry *> get_start_node() {
    std::vector<MapEntry *> ret;
    for (int i = 0; i < this->len; i++) {
      if (this->data[i].name[2] == 'A') {
        ret.push_back(&(this->data[i]));
      }
    }
    return ret;
  }
  bool is_end_node(const MapEntry *const node) {
    return (node->name[2] == 'Z');
  }
  MapEntry *traverse_map(const MapEntry *const node, Direction dir) {
    switch (dir) {
    case Right:
      return node->right;
      break;
    case Left:
      return node->left;
      break;
    }
  }
};