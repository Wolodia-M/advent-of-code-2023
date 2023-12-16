#include "../loader.hpp"
#include "../utils.hpp"
#include "atomic"
#include "cstdio"
#include "cstdlib"
#include "mutex"
#include "sstream"
#include "string"
#include "thread"
#include "tsqueue.hpp"
#include "vector"
// TODO not working at all, halts
typedef enum state {
  SEEDS,
  SEED_TO_SOIL,
  SOIL_TO_FERTILIZE,
  FERTILIZE_TO_WATER,
  WATER_TO_LIGHT,
  LIGHT_TO_TEMPERATURE,
  TEMPERATURE_TO_HUMIDITY,
  HUMIDITY_TO_LOCATION
} state;
typedef struct map_entry {
  unsigned long int dest;
  unsigned long int src;
  unsigned long int range;
} map_entry;
typedef struct seed_range {
  unsigned long int start;
  unsigned long int range;
} seed_range;
unsigned long int perform_map(unsigned long int val,
                              std::vector<map_entry> *map) {
  for (auto entry : *map) {
    if ((val >= entry.src) && (val < entry.src + entry.range)) {
      return entry.dest + val - entry.src;
    }
  }
  return val;
}
std::vector<map_entry> seed_to_soil_map;
std::vector<map_entry> soil_to_fertilize_map;
std::vector<map_entry> fertilize_to_water_map;
std::vector<map_entry> water_to_light_map;
std::vector<map_entry> light_to_temperature_map;
std::vector<map_entry> temperature_to_humidity_map;
std::vector<map_entry> humidity_to_location_map;
std::vector<seed_range> seeds;
int main(int argc, char **argv) {
  std::string FILE_DATA = load_file("test9_10");
  std::stringstream FILE_DATA_STREAM(FILE_DATA);
  // Parse file
  std::string line, line1;
  state st = SEEDS;
  while (std::getline(FILE_DATA_STREAM, line, '\n')) {
    if (line == std::string("")) {
      continue;
    }
    // printf("%s\n", line.c_str());
    switch (st) {
    case SEEDS: {
      if (line == std::string("seed-to-soil map:")) {
        printf("seed-to-soil map found\n");
        st = SEED_TO_SOIL;
        continue;
      }
      line = line.substr(7);
      std::stringstream line_stream(line);
      while (std::getline(line_stream, line1, ' ')) {
        if (std::isdigit(line1[0])) {
          printf(".");
          fflush(stdout);
          std::string line2;
          std::getline(line_stream, line2, ' ');
          unsigned long int rng = std::strtoul(line2.c_str(), NULL, 10);
          unsigned long int st = std::strtoul(line1.c_str(), NULL, 10);
          seeds.push_back({.start = st, .range = rng});
        }
      }
    } break;
    case SEED_TO_SOIL: {
      if (line == std::string("soil-to-fertilizer map:")) {
        printf("soil-to-fertilizer map found\n");
        st = SOIL_TO_FERTILIZE;
        continue;
      }
      std::stringstream line_stream(line);
      map_entry me;
      line_stream >> line1;
      me.dest = std::strtoul(line1.c_str(), NULL, 10);
      line_stream >> line1;
      me.src = std::strtoul(line1.c_str(), NULL, 10);
      line_stream >> line1;
      me.range = std::strtoul(line1.c_str(), NULL, 10);
      seed_to_soil_map.push_back(me);
    } break;
    case SOIL_TO_FERTILIZE: {
      if (line == std::string("fertilizer-to-water map:")) {
        printf("fertilizer-to-water map found\n");
        st = FERTILIZE_TO_WATER;
        continue;
      }
      std::stringstream line_stream(line);
      map_entry me;
      line_stream >> line1;
      me.dest = std::strtoul(line1.c_str(), NULL, 10);
      line_stream >> line1;
      me.src = std::strtoul(line1.c_str(), NULL, 10);
      line_stream >> line1;
      me.range = std::strtoul(line1.c_str(), NULL, 10);
      soil_to_fertilize_map.push_back(me);
    } break;
    case FERTILIZE_TO_WATER: {
      if (line == std::string("water-to-light map:")) {
        printf("water-to-light map found\n");
        st = WATER_TO_LIGHT;
        continue;
      }
      std::stringstream line_stream(line);
      map_entry me;
      line_stream >> line1;
      me.dest = std::strtoul(line1.c_str(), NULL, 10);
      line_stream >> line1;
      me.src = std::strtoul(line1.c_str(), NULL, 10);
      line_stream >> line1;
      me.range = std::strtoul(line1.c_str(), NULL, 10);
      fertilize_to_water_map.push_back(me);
    } break;
    case WATER_TO_LIGHT: {
      if (line == std::string("light-to-temperature map:")) {
        printf("light-to-temperature map found\n");
        st = LIGHT_TO_TEMPERATURE;
        continue;
      }
      std::stringstream line_stream(line);
      map_entry me;
      line_stream >> line1;
      me.dest = std::strtoul(line1.c_str(), NULL, 10);
      line_stream >> line1;
      me.src = std::strtoul(line1.c_str(), NULL, 10);
      line_stream >> line1;
      me.range = std::strtoul(line1.c_str(), NULL, 10);
      water_to_light_map.push_back(me);
    } break;
    case LIGHT_TO_TEMPERATURE: {
      if (line == std::string("temperature-to-humidity map:")) {
        printf("temperature-to-humidity map found\n");
        st = TEMPERATURE_TO_HUMIDITY;
        continue;
      }
      std::stringstream line_stream(line);
      map_entry me;
      line_stream >> line1;
      me.dest = std::strtoul(line1.c_str(), NULL, 10);
      line_stream >> line1;
      me.src = std::strtoul(line1.c_str(), NULL, 10);
      line_stream >> line1;
      me.range = std::strtoul(line1.c_str(), NULL, 10);
      light_to_temperature_map.push_back(me);
    } break;
    case TEMPERATURE_TO_HUMIDITY: {
      if (line == std::string("humidity-to-location map:")) {
        printf("humidity-to-location map found\n");
        st = HUMIDITY_TO_LOCATION;
        continue;
      }
      std::stringstream line_stream(line);
      map_entry me;
      line_stream >> line1;
      me.dest = std::strtoul(line1.c_str(), NULL, 10);
      line_stream >> line1;
      me.src = std::strtoul(line1.c_str(), NULL, 10);
      line_stream >> line1;
      me.range = std::strtoul(line1.c_str(), NULL, 10);
      temperature_to_humidity_map.push_back(me);
    } break;
    case HUMIDITY_TO_LOCATION: {
      std::stringstream line_stream(line);
      map_entry me;
      line_stream >> line1;
      me.dest = std::strtoul(line1.c_str(), NULL, 10);
      line_stream >> line1;
      me.src = std::strtoul(line1.c_str(), NULL, 10);
      line_stream >> line1;
      me.range = std::strtoul(line1.c_str(), NULL, 10);
      humidity_to_location_map.push_back(me);
    } break;
    }
  }
  printf("seed-to-soil\n");
  for (auto elem : seed_to_soil_map) {
    printf("Src: %lu, dest: %lu, range: %lu\n", elem.src, elem.dest,
           elem.range);
  }
  printf("soil-to-fertilizer\n");
  for (auto elem : soil_to_fertilize_map) {
    printf("Src: %lu, dest: %lu, range: %lu\n", elem.src, elem.dest,
           elem.range);
  }
  printf("fertilize-to-water\n");
  for (auto elem : fertilize_to_water_map) {
    printf("Src: %lu, dest: %lu, range: %lu\n", elem.src, elem.dest,
           elem.range);
  }
  printf("water-to-light\n");
  for (auto elem : water_to_light_map) {
    printf("Src: %lu, dest: %lu, range: %lu\n", elem.src, elem.dest,
           elem.range);
  }
  printf("light-to-temperature\n");
  for (auto elem : light_to_temperature_map) {
    printf("Src: %lu, dest: %lu, range: %lu\n", elem.src, elem.dest,
           elem.range);
  }
  printf("temperature-to-humidity\n");
  for (auto elem : temperature_to_humidity_map) {
    printf("Src: %lu, dest: %lu, range: %lu\n", elem.src, elem.dest,
           elem.range);
  }
  printf("humidity-to-location\n");
  for (auto elem : humidity_to_location_map) {
    printf("Src: %lu, dest: %lu, range: %lu\n", elem.src, elem.dest,
           elem.range);
  }
  printf("Seeds: \n");
  for (auto elem : seeds) {
    printf("Start: %lu, length: %lu\n", elem.start, elem.range);
  }
  // Ceating tasks
  // Preprocess ramges
  ThreadSafeQueue<seed_range> tasks;
  for (auto elem : seeds) {
    if (elem.range <= 10000) {
      tasks.push(elem);
    } else {
      for (unsigned long int i = elem.start; i < elem.start + elem.range;
           i += 10000) {
        tasks.push({.start = i,
                    .range = static_cast<unsigned long>(
                        ((i + 10000) > (elem.start + elem.range))
                            ? ((elem.start + elem.range) - i)
                            : 10000)});
      }
    }
  }
  //   tasks.run();
  //   seed_range elem;
  //   printf("Working ranges: \n");
  //   while (tasks.try_pop(elem)) {
  // printf("Start: %lu, range: %lu\n", elem.start, elem.range);
  //   }
  int num_threads = 8;
  std::vector<std::thread> threads;
  unsigned long int thread_min[num_threads];
  std::mutex end_mutex[num_threads];
  bool end[num_threads];
  for (int i = 0; i < num_threads; i++) {
    unsigned long int *min_ptr = &thread_min[i];
    *min_ptr = 0xFFFFFFFFFFFFFFFF;
    bool *end_ptr = &end[i];
    *end_ptr = false;
    auto mtx_ptr = &end_mutex[i];
    threads.push_back(std::thread([min_ptr, end_ptr, &mtx_ptr, &tasks]() {
      seed_range task;
      auto available = tasks.try_pop(task);
      if (!available) {
        {
          std::unique_lock<std::mutex> lock(*mtx_ptr);
          *end_ptr = true;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return;
      }
      for (int i = task.start; i < task.start + task.range; i++) {
        unsigned int seed = i;
        unsigned long int soil = perform_map(seed, &seed_to_soil_map);
        unsigned long int fertilize = perform_map(soil, &soil_to_fertilize_map);
        unsigned long int water =
            perform_map(fertilize, &fertilize_to_water_map);
        unsigned long int light = perform_map(water, &water_to_light_map);
        unsigned long int temperature =
            perform_map(light, &light_to_temperature_map);
        unsigned long int humidity =
            perform_map(temperature, &temperature_to_humidity_map);
        unsigned long int location =
            perform_map(humidity, &humidity_to_location_map);
        if (location < *min_ptr) {
          *min_ptr = location;
        }
      }
    }));
  }
  int stopped_threads = 0;
  while (true) {
    for (int i = 0; i < num_threads; i++) {
      std::unique_lock<std::mutex> lock(end_mutex[i]);
      auto *elem = &end[i];
      if (*elem == true) {
        threads.at(i).join();
        stopped_threads++;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if (stopped_threads == num_threads) {
      unsigned long int min = 0xFFFFFFFFFFFFFFFF;
      for (int thmin : thread_min) {
        if (thmin < min) {
          min = thmin;
        }
      }
      printf("Min: %lu\n", min);
      return 0;
    }
  }
}