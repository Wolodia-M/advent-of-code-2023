#include "../loader.hpp"
#include "../utils.hpp"
#include "cstdio"
#include "cstdlib"
#include "sstream"
#include "string"
#include "vector"
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
      // line = line.substr(7);
      // std::stringstream line_stream(line);
      // printf("Seed processing");
      // while (std::getline(line_stream, line1, ' ')) {
      // if (std::isdigit(line1[0])) {
      // printf(".");
      // fflush(stdout);
      // std::string line2;
      // std::getline(line_stream, line2, ' ');
      // unsigned long int rng = std::strtoul(line2.c_str(), NULL, 10);
      // unsigned long int st = std::strtoul(line1.c_str(), NULL, 10);
      // for (unsigned long int i = st; i < st + rng; i++) {
      // seeds.push_back(i);
      // }
      // }
      // }
      // printf("\n");
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
  // Perform mappings
  printf("Performing mapping:\n");
  unsigned long int min = 0xFFFFFFFFFFFFFFFF;
  std::stringstream SEED_STREAM(FILE_DATA.substr(0, FILE_DATA.find('\n')));
  printf("%s\n", SEED_STREAM.str().c_str());
  while (std::getline(SEED_STREAM, line, ' ')) {
    if (std::isdigit(line[0])) {
      std::getline(SEED_STREAM, line1, ' ');
      unsigned long int st = std::strtoul(line.c_str(), NULL, 10);
      unsigned long int rng = std::strtoul(line1.c_str(), NULL, 10);
      printf("New seed range");
      fflush(stdout);
      for (unsigned long int i = st; i < st + rng; i++) {
        if (i % 1000000 == 0) {
          printf(".");
          fflush(stdout);
        }
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
        if (location < min) {
          min = location;
        }
      }
      printf("\n");
    }
  }
  printf("Lowest location id: %lu\n", min);
}