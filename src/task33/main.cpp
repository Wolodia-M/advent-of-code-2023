#include "graph.hpp"
int main(int argc, char **argv) {
  Graph graph("test33_34");
  printf("Distance: %lu\n", graph.lowest_heat_loss(0, 0, graph.get_cols() - 1,
                                                   graph.get_rows() - 1));
  return 0;
}