#include "../loader.hpp"
#include "cstdio"
#include "map"
#include "sstream"
#include "string"
#include "vector"
struct Part {
  int x;
  int m;
  int a;
  int s;
};
enum comparision_rule : char {
  GREATER = '>',
  LESS = '<',
};
enum Param : char {
  X = 'x',
  M = 'm',
  A = 'a',
  S = 's',
};
struct result {
  int accepted = 0; // -1 -> false, 1 -> true
  int rejected = 0; // -1 -> false, 1 -> true
  std::string next_workflow;
};
struct Comparision {
  comparision_rule comp;
  int comp_val;
  Param param;
  result action;
  bool operator()(const Part &p) {
    switch (this->comp) {
    case GREATER: {
      switch (this->param) {
      case X:
        return p.x > this->comp_val;
        break;
      case M:
        return p.m > this->comp_val;
        break;
      case A:
        return p.a > this->comp_val;
        break;
      case S:
        return p.s > this->comp_val;
        break;
      }
    } break;
    case LESS: {
      switch (this->param) {
      case X:
        return p.x < this->comp_val;
        break;
      case M:
        return p.m < this->comp_val;
        break;
      case A:
        return p.a < this->comp_val;
        break;
      case S:
        return p.s < this->comp_val;
        break;
      }
    } break;
    }
  }
};
struct Workflow {
  std::vector<Comparision> comps;
  result last;
  result operator()(const Part &p) {
    for (auto comp : this->comps) {
      if (comp(p)) {
        return comp.action;
      }
    }
    return this->last;
  }
};
struct Range {
  int start;
  int end;
};
struct PartsRange {
  Range x;
  Range m;
  Range a;
  Range s;
  bool invalid = false;
  ;
};
std::map<std::string, Workflow> workflows;
std::vector<Part> parts;
size_t get_range_span(PartsRange prng) {
  if (prng.invalid) {
    return 0;
  }
  size_t ret = (prng.a.end - prng.a.start) * (prng.m.end - prng.m.start) *
               (prng.a.end - prng.a.start) * (prng.s.end - prng.s.start);
  printf("RET: %lu\n", ret);
  return ret;
}
PartsRange get_subrange(PartsRange prng, Comparision comp) {
  PartsRange ret = prng;
  ret.invalid = true;
  if (comp.comp == comparision_rule::GREATER) {
    switch (comp.param) {
    case X:
      if (prng.x.end > comp.comp_val) {
        ret.x = {.start = std::max(comp.comp_val + 1, prng.x.start),
                 .end = prng.x.end};
        ret.invalid = false;
      }
      break;
    case M:
      if (prng.m.end > comp.comp_val) {
        ret.x = {.start = std::max(comp.comp_val + 1, prng.m.start),
                 .end = prng.m.end};
        ret.invalid = false;
      }
      break;
    case A:
      if (prng.a.end > comp.comp_val) {
        ret.x = {.start = std::max(comp.comp_val + 1, prng.a.start),
                 .end = prng.a.end};
        ret.invalid = false;
      }
      break;
    case S:
      if (prng.s.end > comp.comp_val) {
        ret.x = {.start = std::max(comp.comp_val + 1, prng.s.start),
                 .end = prng.s.end};
        ret.invalid = false;
      }
      break;
    }
  } else if (comp.comp == comparision_rule::LESS) {
    switch (comp.param) {
    case X:
      if (prng.x.start < comp.comp_val) {
        ret.x = {.start = prng.x.start,
                 .end = std::min(prng.x.end, comp.comp_val - 1)};
        ret.invalid = false;
      }
      break;
    case M:
      if (prng.m.start < comp.comp_val) {
        ret.m = {.start = prng.m.start,
                 .end = std::min(prng.m.end, comp.comp_val - 1)};
        ret.invalid = false;
      }
      break;
    case A:
      if (prng.a.start < comp.comp_val) {
        ret.a = {.start = prng.a.start,
                 .end = std::min(prng.a.end, comp.comp_val - 1)};
        ret.invalid = false;
      }
      break;
    case S:
      if (prng.s.start < comp.comp_val) {
        ret.s = {.start = prng.s.start,
                 .end = std::min(prng.s.end, comp.comp_val - 1)};
        ret.invalid = false;
      }
      break;
    }
  }
  return ret;
}
// size_t searcher(PartsRange prng, std::string workflow) {
std::vector<PartsRange> searcher(PartsRange prng, std::string workflow) {
  std::vector<PartsRange> ret = {};
  // size_t ret = 0;
  for (auto comp : workflows[workflow].comps) {
    if (comp.action.accepted) {
      auto r = get_subrange(prng, comp);
      ret.push_back(r);
      // ret += get_range_span(get_subrange(prng, comp));
    } else if (!comp.action.rejected) {
      auto r = searcher(get_subrange(prng, comp), comp.action.next_workflow);
      ret.insert(ret.end(), r.begin(), r.end());
      // ret += searcher(get_subrange(prng, comp), comp.action.next_workflow);
    }
  }
  Comparision null = {.comp = GREATER,
                      .comp_val = -1,
                      .param = X,
                      .action = workflows[workflow].last};
  if (null.action.accepted) {
    auto r = get_subrange(prng, null);
    ret.push_back(r);
    // ret += get_range_span(get_subrange(prng, null));
  } else if (!null.action.rejected) {
    auto r = searcher(get_subrange(prng, null), null.action.next_workflow);
    ret.insert(ret.end(), r.begin(), r.end());
    // ret += searcher(get_subrange(prng, null), null.action.next_workflow);
  }
  return ret;
}
int main(int argc, char **argv) {
  std::string FILE_DATA = load_file("test37_38");
  std::stringstream FILE_DATA_STREAM(FILE_DATA);
  std::string line;
  bool parts_description = false;
  while (std::getline(FILE_DATA_STREAM, line, '\n')) {
    if (line == std::string("")) {
      parts_description = true;
      continue;
    }
    if (!parts_description) {
      printf("Workflow: '%s'\n", line.data());
      std::string name = line.substr(0, line.find('{'));
      Workflow w;
      line = line.substr(line.find('{') + 1, line.size() - name.size() - 2);
      std::stringstream line_stream(line);
      std::string comp;
      while (std::getline(line_stream, comp, ',')) {
        if (comp.find('>') != std::string::npos ||
            comp.find('<') != std::string::npos) {
          printf("Comp: %s\n", comp.c_str());
          w.comps.push_back(
              {.comp = [&comp]() -> comparision_rule {
                 if (comp.find('>') != std::string::npos) {
                   return comparision_rule::GREATER;
                 } else if (comp.find('<') != std::string::npos) {
                   return comparision_rule::LESS;
                 }
               }(),
               .comp_val = [&comp]() -> int {
                 return std::atoi(comp.substr(2, comp.size() - 2).c_str());
               }(),
               .param = static_cast<Param>(comp[0]),
               .action = {.accepted = comp.find('A') != std::string::npos,
                          .rejected = comp.find('R') != std::string::npos,
                          .next_workflow = comp.substr(comp.find(':') + 1)}});
        } else {
          printf("Last: %s\n", comp.c_str());
          w.last = {.accepted = (comp == std::string("A")),
                    .rejected = (comp == std::string("R")),
                    .next_workflow = comp};
        }
      }
      workflows.emplace(std::make_pair(name, w));
    } else {
      // printf("Part: '%s'\n", line.data());
      line = line.substr(1, line.size() - 2);
      std::stringstream line_stream(line);
      std::string num;
      Part p = {-1, -1, -1, -1};
      while (std::getline(line_stream, num, ',')) {
        num = num.substr(2, num.size() - 2);
        if (p.x == -1) {
          p.x = std::atoi(num.c_str());
        } else if (p.m == -1) {
          p.m = std::atoi(num.c_str());
        } else if (p.a == -1) {
          p.a = std::atoi(num.c_str());
        } else if (p.s == -1) {
          p.s = std::atoi(num.c_str());
        }
      }
      parts.push_back(p);
    }
  }
  for (auto w : workflows) {
    printf("Workflow '%s': [", w.first.c_str());
    for (auto comp : w.second.comps) {
      printf("{%c %c %d => %s},", static_cast<char>(comp.param),
             static_cast<char>(comp.comp), comp.comp_val,
             [&comp]() -> std::string {
               if (comp.action.accepted) {
                 return "part accepted";
               }
               if (comp.action.rejected) {
                 return "part rejected";
               }
               return std::string("go to workflow '") +
                      comp.action.next_workflow + std::string("'");
             }()
                              .c_str());
    }
    printf("%s]\n",
           [&w]() -> std::string {
             if (w.second.last.accepted) {
               return "part accepted";
             }
             if (w.second.last.rejected) {
               return "part rejected";
             }
             return std::string("go to workflow '") +
                    w.second.last.next_workflow + std::string("'");
           }()
                         .c_str());
  }
  // for (int x = 1; x <= 4000; x++) {
  //   for (int m = 1; m <= 4000; m++) {
  //     for (int a = 1; a < 4000; a++) {
  //       for (int s = 1; s <= 4000; s++) {
  //         Part p = {.x = x, .m = m, .a = a, .s = s};
  //         auto res = workflows["in"](p);
  //         while (res.accepted != true && res.rejected != true) {
  //           res = workflows[res.next_workflow](p);
  //         }
  //         sum++;
  //       }
  //       printf(".");
  //       fflush(stdout);
  //     }
  //     printf("*");
  //     fflush(stdout);
  //   }
  //   printf("#");
  //   fflush(stdout);
  // }
  auto ranges = searcher({{1, 4000}, {1, 4000}, {1, 4000}, {1, 4000}}, "in");
  std::vector<PartsRange> non_overlaping_ranges = {ranges[0]};
  for (auto rng : ranges) {
    // Process ranges and get non-overlapping parts
  }
  for (auto rng : ranges) {
    printf("x[%d;%d],m[%d;%d],a[%d;%d],s[%d;%d]\n", rng.x.start, rng.x.end,
           rng.m.start, rng.m.end, rng.a.start, rng.a.end, rng.s.start,
           rng.s.end);
  }
  for (auto rng : non_overlaping_ranges) {
    printf("x[%d;%d],m[%d;%d],a[%d;%d],s[%d;%d]\n", rng.x.start, rng.x.end,
           rng.m.start, rng.m.end, rng.a.start, rng.a.end, rng.s.start,
           rng.s.end);
  }
  size_t sum = 0;
  // Do get_range_span() for needed ranges
  printf("Sum: %lu\n", sum);
}