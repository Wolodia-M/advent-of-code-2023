#include "algorithm"
#include "numeric"
#include "string"
#include "sys/resource.h"
#include "vector"
#ifndef UTILS_HPP
#define UTILS_HPP
inline std::string string_replace(std::string in, std::string token,
                                  std::string data) {
  std::string out = "";
  while (in.find(token) != std::string::npos) {
    in.replace(in.find(token), token.size(), data);
  }
  return in;
}
inline int sum_of_vec(std::vector<int> *vec) {
  return std::accumulate(vec->begin(), vec->end(), 0);
}
template <class T>
inline int sum_of_vec_op(std::vector<T> *vec, int (*op)(T val)) {
  int sum = 0;
  auto lambda = [&sum, &op](T val) { sum += op(val); };
  std::ranges::for_each(*vec, lambda);
  return sum;
}
inline void stack_1gb() {
  size_t kStackSize = 1024L * 1024L * 1024L;
  struct rlimit rl;
  int result;
  result = getrlimit(RLIMIT_STACK, &rl);
  {
    if (rl.rlim_cur < kStackSize) {
      rl.rlim_cur = kStackSize;
      result = setrlimit(RLIMIT_STACK, &rl);
      if (result != 0) {
        printf("setrlimit returned result = %d\n", result);
        fflush(stdout);
      }
    }
  }
}
void dfs_explore(std::vector<std::vector<int>> *nodes,
                 std::vector<std::vector<bool>> *visited, size_t stx,
                 size_t sty, bool (*isFloor)(int elem)) {
  auto isValid = [](int row, int col, int numRows, int numCols) -> bool {
    return (row >= 0 && row < numRows && col >= 0 && col < numCols);
  };
  visited->at(sty).at(stx) = true;
  static const int dr[] = {-1, 0, 1, 0};
  static const int dc[] = {0, 1, 0, -1};
  for (int i = 0; i < 4; ++i) { // Explore in all directions
    int y = sty + dr[i];
    int x = stx + dc[i];
    printf("x: %d, y: %d\n", x, y);
    if (isValid(y, x, nodes->size(), nodes->at(0).size()) &&
        !visited->at(y).at(x) && isFloor(nodes->at(sty).at(stx))) {
      dfs_explore(nodes, visited, x, y, isFloor);
    }
  }
};
#endif // UTILS_HPP