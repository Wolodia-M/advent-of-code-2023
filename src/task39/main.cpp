#include "../loader.hpp"
#include "algorithm"
#include "ranges"
#include "signals.hpp"
#include "sstream"
#include "string"
std::vector<Module *> modules;
const int BUTTON_ID = 0;
const int BROADCASTER_ID = 1;
const int FREE_SLOTS_START = 2;
int MODULES_COUNT = 2;
int main(int argc, char **argv) {
  std::string FILE_DATA = load_file("task39_40");
  for (int i = 0; i < 1 + std::ranges::count(FILE_DATA, '\n'); i++) {
    modules.push_back(NULL);
  }
  modules.at(BUTTON_ID) = new BUTTON("button");
  std::stringstream FILE_DATA_STREAM(FILE_DATA);
  std::string line;
  int push_idx = FREE_SLOTS_START;
  while (std::getline(FILE_DATA_STREAM, line, '\n')) {
    if (line == std::string("")) {
      continue;
    }
    if (line.find("broadcaster") == 0) {
      modules.at(BROADCASTER_ID) = new BROADCASTER("broadcaster");
      modules.at(BUTTON_ID)->add_output(modules[BROADCASTER_ID]);
      line = line.substr(15);
      line += ',';
      line += ' ';
      std::stringstream line_stream(line);
      for (auto out : std::ranges::split_view(line, std::string(", "))) {
        modules.at(BROADCASTER_ID)
            ->allocate_output(std::string(out.begin(), out.end()));
      }
    } else if (line[0] == '%') {
      modules.at(push_idx) = new FLIP_FLOP(line.substr(1, line.find(' ') - 1));
      line = line.substr(line.find('>') + 2);
      line += ',';
      line += ' ';
      std::stringstream line_stream(line);
      for (auto out : std::ranges::split_view(line, std::string(", "))) {
        modules.at(push_idx)->allocate_output(
            std::string(out.begin(), out.end()));
      }
      MODULES_COUNT++;
      push_idx++;
    } else if (line[0] == '&') {
      modules.at(push_idx) =
          new CONJUNCTION(line.substr(1, line.find(' ') - 1));
      line = line.substr(line.find('>') + 2);
      line += ',';
      line += ' ';
      std::stringstream line_stream(line);
      for (auto out : std::ranges::split_view(line, std::string(", "))) {
        modules.at(push_idx)->allocate_output(
            std::string(out.begin(), out.end()));
      }
      MODULES_COUNT++;
      push_idx++;
    }
  }
  for (int i = 0; i < MODULES_COUNT; i++) {
    for (auto out : *modules[i]->get_allocated_outputs()) {
      if (out != std::string("")) {
        bool found = false;
        for (int j = 0; j < MODULES_COUNT; j++) {
          if (*modules[j]->get_id() == out) {
            modules[i]->add_output(modules[j]);
            found = true;
          }
        }
        if (!found) {
          modules.push_back(NULL);
          modules.at(push_idx) = new OUTPUT(out);
          modules[i]->add_output(modules.at(push_idx));
          MODULES_COUNT++;
          push_idx++;
        }
      }
    }
  }
  printf("Modules: %d\n", push_idx);
  for (int i = 0; i < MODULES_COUNT; i++) {
    auto mod = modules[i];
    printf("%s '%s' -> ", typeid(*modules[i]).name(), mod->get_id()->c_str());
    for (auto out : *mod->get_outputs()) {
      printf("%s, ", out->get_id()->c_str());
    }
    printf("\n");
  }
  PulseCount signals_transmitted = {0, 0};
  for (int i = 0; i < 1000; i++) {
    signals_transmitted += Sheduler::shedule(&modules);
  }
  printf("Total high signals count: %lu\nTotal low signals count: %lu\n",
         signals_transmitted.high, signals_transmitted.low);
  printf("Total: %lu\n", signals_transmitted.high * signals_transmitted.low);
  return 0;
}