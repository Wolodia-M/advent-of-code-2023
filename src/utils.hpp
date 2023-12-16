#include "algorithm"
#include "numeric"
#include "string"
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
#endif // UTILS_HPP