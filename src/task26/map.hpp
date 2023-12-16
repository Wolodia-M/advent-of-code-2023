#include "../loader.hpp"
#include "algorithm"
#include "cstdio"
#include "cstdlib"
#include "cstring"
#include "ranges"
#include "sstream"
#include "string"
#include "string_view"
#include "vector"

class Map {
private:
  std::vector<char *> data;
  std::vector<size_t> rows;
  std::vector<size_t> columns;
  size_t idx(size_t x, size_t y, size_t id) {
    return x + y * this->columns[id];
  }

public:
  Map(std::string file) {
    std::string FILE_DATA = load_file(file);
    size_t pattern_id = 0;
    for (const auto pattern :
         std::views::split(FILE_DATA, std::string("\n\n"))) {

      std::string PATTERN_STR = std::string(pattern.begin(), pattern.end());
      if (PATTERN_STR != std::string("")) {
        printf("Pattern %lu:\n%s\n", pattern_id, PATTERN_STR.c_str());
        this->data.push_back(NULL);
        this->rows.push_back(0);
        this->columns.push_back(0);
        this->rows[pattern_id] = std::ranges::count(PATTERN_STR, '\n') + 1;
        this->columns[pattern_id] =
            PATTERN_STR.substr(0, PATTERN_STR.find('\n')).size();
        printf("Rows: %lu, columns: %lu\n", this->rows[pattern_id],
               this->columns[pattern_id]);
        this->data[pattern_id] =
            (char *)malloc(this->rows[pattern_id] * this->columns[pattern_id]);
        std::stringstream PATTERN_STR_STREAM(PATTERN_STR);
        std::string line;
        size_t y = 0;
        fflush(stdout);
        while (std::getline(PATTERN_STR_STREAM, line, '\n')) {
          memcpy(&this->data[pattern_id][this->idx(0, y, pattern_id)],
                 line.c_str(), this->columns[pattern_id]);
          y++;
        }
      }
      pattern_id++;
    }
    for (int id = 0; id < this->data.size(); id++) {
      printf("ID: %d\n", id);
      for (int i = 0; i < this->rows[id]; i++) {
        for (int j = 0; j < this->columns[id]; j++) {
          printf("%c", this->data[id][this->idx(j, i, id)]);
        }
        printf("\n");
      }
    }
  }
  size_t patterns_count() { return this->data.size(); }
  size_t get_pattern_refrection(size_t pattern_id) {
    size_t ret = 0;
    auto ptr = this->data[pattern_id];
    auto cols = this->columns[pattern_id];
    auto rows = this->rows[pattern_id];
    struct comparition_result {
      bool fully_match;
      bool one_error;
      size_t err_cnt;
    };
    auto string_comparator = [](std::string a,
                                std::string b) -> comparition_result {
      comparition_result res = {
          .fully_match = false, .one_error = false, .err_cnt = 0};
      if (a == b) {
        res.fully_match = true;
        res.one_error = false;
        res.err_cnt = 0;
      } else {
        res.fully_match = false;
        size_t errs = 0;
        if (a.size() != b.size()) {
          res.one_error = false;
          res.err_cnt = 0xFFFFFFFFFFFFFFFF;
        } else {
          for (int i = 0; i < a.size(); i++) {
            if (a[i] != b[i]) {
              errs++;
            }
          }
          res.err_cnt = errs;
          if (errs == 1) {
            res.one_error = true;
          }
        }
      }
      return res;
    };
    auto compare_line = [this, &pattern_id, &ptr, &cols, &string_comparator](
                            size_t line1, size_t line2) -> comparition_result {
      printf("Line %lu == line %lu\n", line1, line2);
      std::string l1(ptr + this->idx(0, line1, pattern_id), cols);
      std::string l2(ptr + this->idx(0, line2, pattern_id), cols);
      printf("'%s' == '%s'\n", l1.c_str(), l2.c_str());
      fflush(stdout);
      return string_comparator(l1, l2);
    };
    auto compare_col = [this, &pattern_id, &ptr, &cols, &rows,
                        &string_comparator](size_t col1,
                                            size_t col2) -> comparition_result {
      printf("Col %lu == Col %lu\n", col1, col2);
      std::string c1, c2;
      for (int i = 0; i < rows; i++) {
        c1.push_back(ptr[this->idx(col1, i, pattern_id)]);
      }
      for (int i = 0; i < rows; i++) {
        c2.push_back(ptr[this->idx(col2, i, pattern_id)]);
      }
      printf("'%s' == '%s'\n", c1.c_str(), c2.c_str());
      fflush(stdout);
      return string_comparator(c1, c2);
    };
    for (int i = 0; i < rows - 1; i++) {
      printf("Trying to find mirror on line %d\n", i);
      bool match = true;
      bool fixed = false;
      for (int j = 0; j <= i; j++) {
        if ((i + j + 1) >= rows) {
          break;
        }
        auto res = compare_line(i - j, i + j + 1);
        if (res.one_error && !fixed) {
          fixed = true;
          continue;
        } else if (res.fully_match) {
        } else {
          match = false;
        }
      }
      if (match && fixed) {
        printf("Found\n");
        fflush(stdout);
        ret += 100 * (i + 1);
      }
    }
    for (int i = 0; i < cols - 1; i++) {
      printf("Trying to find mirror on column %d\n", i);
      bool match = true;
      bool fixed = false;
      for (int j = 0; j <= i; j++) {
        if ((i + j + 1) >= cols) {
          break;
        }

        auto res = compare_col(i - j, i + j + 1);
        printf("Errors: %lu\n", res.err_cnt);
        if (res.one_error && !fixed) {
          fixed = true;
          continue;
        } else if (res.fully_match) {
        } else {
          match = false;
        }
      }
      if (match & fixed) {
        printf("Found\n");
        fflush(stdout);
        ret += i + 1;
      }
    }
    return ret;
  }
  ~Map() {
    for (auto ptr : this->data) {
      free(ptr);
    }
  }
};