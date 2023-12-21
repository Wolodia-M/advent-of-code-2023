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
std::map<std::string, Workflow> workflows;
std::vector<Part> parts;
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
      printf("Part: '%s'\n", line.data());
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
  size_t sum = 0;
  for (Part p : parts) {
    auto res = workflows["in"](p);
    while (res.accepted != true && res.rejected != true) {
      res = workflows[res.next_workflow](p);
    }
    if (res.accepted) {
      sum += p.x;
      sum += p.m;
      sum += p.a;
      sum += p.s;
    }
  }
  printf("Sum: %lu\n", sum);
}