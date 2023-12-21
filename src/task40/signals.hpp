#include "map"
#include "string"
#include "vector"
enum class SIGNAL { HIGH, LOW };
struct PulseCount {
  size_t low;
  size_t high;
  PulseCount operator+=(const PulseCount &other) {
    this->low += other.low;
    this->high += other.high;
    return *this;
  }
};
struct call {
  std::string this_id;
  void *out;
  SIGNAL sig;
};
struct ret {
  std::vector<call> outs;
  PulseCount pc;
  bool received = false;
  ret operator+=(const ret &other) {
    this->pc += other.pc;
    this->outs.insert(this->outs.end(), other.outs.begin(), other.outs.end());
    return *this;
  }
};
class Module {
protected:
  std::string id;
  std::vector<Module *> outputs;
  std::vector<std::string> pending_outputs;

public:
  Module(std::string id) { this->id = id; }
  virtual void add_output(Module *mod) {
    this->outputs.push_back(mod);
    mod->setup_channel(this->id);
  };
  void allocate_output(std::string id) { this->pending_outputs.push_back(id); }
  std::vector<std::string> *const get_allocated_outputs() {
    return &this->pending_outputs;
  }
  const std::string *const get_id() { return &this->id; }
  const std::vector<Module *> *get_outputs() { return &this->outputs; }
  virtual ret input(std::string sender, SIGNAL signal) = 0;
  virtual void setup_channel(std::string sender) = 0;
};
class FLIP_FLOP : public Module {
private:
  bool state;

public:
  FLIP_FLOP(std::string id) : Module(id) { this->state = false; }
  virtual ret input(std::string sender, SIGNAL signal) override {
    ret ret = {{}, {0, 0}};
    if (signal == SIGNAL::LOW) {
      if (this->state == false) {
        this->state = true;
        for (auto mod : this->outputs) {
          ret.pc.high += 1;
          ret.outs.push_back({this->id, mod, SIGNAL::HIGH});
          // mod->input(this->id, SIGNAL::HIGH);
        }
      } else if (this->state == true) {
        this->state = false;
        for (auto mod : this->outputs) {
          ret.pc.low += 1;
          ret.outs.push_back({this->id, mod, SIGNAL::LOW});
          // mod->input(this->id, SIGNAL::LOW);
        }
      }
    }
    return ret;
  }
  virtual void setup_channel(std::string sender) override {}
};
class CONJUNCTION : public Module {
private:
  std::map<std::string, SIGNAL> memory;

public:
  CONJUNCTION(std::string id) : Module(id) { this->memory = {}; }
  virtual ret input(std::string sender, SIGNAL signal) override {
    ret ret = {{}, {0, 0}};
    this->memory.at(sender) = signal;
    bool send_lo = true;
    for (auto elem : this->memory) {
      if (elem.second != SIGNAL::HIGH) {
        send_lo = false;
      }
    }
    if (send_lo) {
      for (auto mod : this->outputs) {
        ret.pc.low += 1;
        ret.outs.push_back({this->id, mod, SIGNAL::LOW});
        // ret += mod->input(this->id, SIGNAL::LOW);
      }
    } else {
      for (auto mod : this->outputs) {
        ret.pc.high += 1;
        ret.outs.push_back({this->id, mod, SIGNAL::HIGH});
        // ret += mod->input(this->id, SIGNAL::HIGH);
      }
    }
    return ret;
  }
  virtual void setup_channel(std::string sender) override {
    this->memory.emplace(std::make_pair(sender, SIGNAL::LOW));
  }
};
class BROADCASTER : public Module {
public:
  BROADCASTER(std::string id) : Module(id) {}
  virtual ret input(std::string sender, SIGNAL signal) override {
    ret ret = {{}, {0, 0}};
    for (auto mod : this->outputs) {
      if (signal == SIGNAL::HIGH) {
        ret.pc.high += 1;
      } else if (signal == SIGNAL::LOW) {
        ret.pc.low += 1;
      }
      ret.outs.push_back({this->id, mod, signal});
      // ret += mod->input(this->id, signal);
    }
    return ret;
  }
  virtual void setup_channel(std::string sender) override {}
};
class OUTPUT : public Module {
public:
  OUTPUT(std::string id) : Module(id) {}
  virtual ret input(std::string sender, SIGNAL signal) override {
    return {{}, {0, 0}};
  }
  virtual void setup_channel(std::string sender) override {}
};
class BUTTON : public Module {
public:
  BUTTON(std::string id) : Module(id) {}
  virtual ret input(std::string sender, SIGNAL signal) override {
    ret ret = {{}, {0, 0}};
    for (auto mod : this->outputs) {
      ret.pc.low += 1;
      ret.outs.push_back({this->id, mod, SIGNAL::LOW});
      // ret += mod->input(this->id, SIGNAL::LOW);
    }
    return ret;
  }
  virtual void setup_channel(std::string sender) override {}
};
class RECEIVER : public Module {
public:
  RECEIVER(std::string id) : Module(id) {}
  virtual ret input(std::string sender, SIGNAL signal) override {
    if (signal == SIGNAL::LOW) {
      return {{}, {0, 0}, true};
    } else {
      return {{}, {0, 0}, false};
    }
  }
  virtual void setup_channel(std::string sender) override {}
};
class Sheduler {
public:
  static bool shedule(std::vector<Module *> *modules) {
    ret r = {{{"", modules->at(0), SIGNAL::LOW}}, {0, 0}};
    while (!r.outs.empty()) {
      ret new_r = {{}, {0, 0}};
      for (auto mod : r.outs) {
        //   printf("%s -> '%s' => %s\n", mod.this_id.c_str(),
        //          mod.sig == SIGNAL::HIGH ? "HIGH" : "LOW",
        //          ((Module *)mod.out)->get_id()->c_str());
        auto tmp = ((Module *)mod.out)->input(mod.this_id, mod.sig);
        if (tmp.received == true) {
          return true;
        }
        new_r += tmp;
      }
      r.outs = new_r.outs;
      r.pc += new_r.pc;
      r.received = new_r.received;
    }
    return r.received;
  }
};
