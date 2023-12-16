#include "../loader.hpp"
#include "../utils.hpp"
#include "sstream"
#include "stdio.h"
#include "string"
#include "vector"
enum CARD {
  C2 = 0,
  C3 = 1,
  C4 = 2,
  C5 = 3,
  C6 = 4,
  C7 = 5,
  C8 = 6,
  C9 = 7,
  CT = 8,
  CJ = 9,
  CQ = 10,
  CK = 11,
  CA = 12,
};
enum HAND_TYPE {
  FIVE_OF_A_KIND = 7,
  FOUR_OF_A_KIND = 6,
  FULL_HOUSE = 5,
  THREE_OF_A_KIND = 4,
  TWO_PAIR = 3,
  ONE_PAIR = 2,
  HIGH_CARD = 1,
  NONE = 0,
};
struct hand {
  CARD cards[5];
  HAND_TYPE type;
  unsigned long int bid;
};
std::vector<hand> hands;
int main(int argc, char **argv) {
  std::string FILE_DATA = load_file("test13_14");
  std::stringstream FILE_DATA_STREAM(FILE_DATA);
  std::string line;
  auto iscard = [](char c) -> bool {
    if (c == '2') {
      return true;
    } else if (c == '3') {
      return true;
    } else if (c == '4') {
      return true;
    } else if (c == '5') {
      return true;
    } else if (c == '6') {
      return true;
    } else if (c == '7') {
      return true;
    } else if (c == '8') {
      return true;
    } else if (c == '9') {
      return true;
    } else if (c == 'T') {
      return true;
    } else if (c == 'J') {
      return true;
    } else if (c == 'Q') {
      return true;
    } else if (c == 'K') {
      return true;
    } else if (c == 'A') {
      return true;
    } else {
      return false;
    }
  };
  auto decode_card = [](std::string cards, int idx) -> CARD {
    char c = cards[idx];
    if (c == '2') {
      return C2;
    } else if (c == '3') {
      return C3;
    } else if (c == '4') {
      return C4;
    } else if (c == '5') {
      return C5;
    } else if (c == '6') {
      return C6;
    } else if (c == '7') {
      return C7;
    } else if (c == '8') {
      return C8;
    } else if (c == '9') {
      return C9;
    } else if (c == 'T') {
      return CT;
    } else if (c == 'J') {
      return CJ;
    } else if (c == 'Q') {
      return CQ;
    } else if (c == 'K') {
      return CK;
    } else if (c == 'A') {
      return CA;
    }
  };
  auto compare_range = [](std::ranges::range auto &&r, size_t how_many,
                          size_t start) -> bool {
    auto first = r[start];
    for (size_t i = start + 1; i < start + how_many; i++) {
      if (r[i] != first) {
        return false;
      }
    }
    return true;
  };
  auto decode_hand_type = [&compare_range](hand *h) {
    if (compare_range(h->cards, 5, 0)) {
      h->type = FIVE_OF_A_KIND;
      return;
    }
    std::vector<int> c(std::begin(h->cards), std::end(h->cards));
    std::ranges::sort(c, std::ranges::greater());
    if (compare_range(c, 4, 0) || compare_range(c, 4, 1)) {
      h->type = FOUR_OF_A_KIND;
      return;
    }
    if ((compare_range(c, 3, 0) && compare_range(c, 2, 3)) ||
        (compare_range(c, 2, 0) && compare_range(c, 3, 2))) {
      h->type = FULL_HOUSE;
      return;
    }
    if (compare_range(c, 3, 0) || compare_range(c, 3, 1) ||
        compare_range(c, 3, 2)) {
      h->type = THREE_OF_A_KIND;
      return;
    }
    // XXYY. .XXYY XX.YY YYXX. .YYXX YY.XX
    if ((compare_range(c, 2, 0) && compare_range(c, 2, 2)) ||
        (compare_range(c, 2, 0) && compare_range(c, 2, 3)) ||
        (compare_range(c, 2, 1) && compare_range(c, 2, 3))) {
      h->type = TWO_PAIR;
      return;
    }
    if (compare_range(c, 2, 0) || compare_range(c, 2, 1) ||
        compare_range(c, 2, 2) || compare_range(c, 2, 3)) {
      h->type = ONE_PAIR;
      return;
    }
    // printf("Cards: %s\n",
    //    [&c]() -> std::string {
    //  std::string ret = "";
    //  for (int C : c) {
    //    switch (C) {
    //    case C2:
    //  ret += '2';
    //  break;
    //    case C3:
    //  ret += '3';
    //  break;
    //    case C4:
    //  ret += '4';
    //  break;
    //    case C5:
    //  ret += '5';
    //  break;
    //    case C6:
    //  ret += '6';
    //  break;
    //    case C7:
    //  ret += '7';
    //  break;
    //    case C8:
    //  ret += '8';
    //  break;
    //    case C9:
    //  ret += '9';
    //  break;
    //    case CT:
    //  ret += 'T';
    //  break;
    //    case CJ:
    //  ret += 'J';
    //  break;
    //    case CQ:
    //  ret += 'Q';
    //  break;
    //    case CK:
    //  ret += 'K';
    //  break;
    //    case CA:
    //  ret += 'A';
    //  break;
    //    }
    //  }
    //  return ret;
    //    }()
    //  .c_str());
    h->type = HIGH_CARD;
    return;
  };
  while (std::getline(FILE_DATA_STREAM, line, '\n')) {
    if (iscard(line[0])) {
      std::string cards = line.substr(0, 5);
      std::string bid_str = line.substr(6);
      hands.push_back({.cards =
                           {
                               decode_card(cards, 0),
                               decode_card(cards, 1),
                               decode_card(cards, 2),
                               decode_card(cards, 3),
                               decode_card(cards, 4),
                           },
                       .type = NONE,
                       .bid = std::strtoul(bid_str.c_str(), NULL, 10)});
      decode_hand_type(&hands.back());
      auto hand = &hands.back();
      //   printf(
      //       "Hand: cards - '%s', bid - '%lu', type - %s\n",
      //       [&hand]() -> std::string {
      //         std::string ret = "";
      //         for (CARD c : hand->cards) {
      //           switch (c) {
      //           case C2:
      //             ret += '2';
      //             break;
      //           case C3:
      //             ret += '3';
      //             break;
      //           case C4:
      //             ret += '4';
      //             break;
      //           case C5:
      //             ret += '5';
      //             break;
      //           case C6:
      //             ret += '6';
      //             break;
      //           case C7:
      //             ret += '7';
      //             break;
      //           case C8:
      //             ret += '8';
      //             break;
      //           case C9:
      //             ret += '9';
      //             break;
      //           case CT:
      //             ret += 'T';
      //             break;
      //           case CJ:
      //             ret += 'J';
      //             break;
      //           case CQ:
      //             ret += 'Q';
      //             break;
      //           case CK:
      //             ret += 'K';
      //             break;
      //           case CA:
      //             ret += 'A';
      //             break;
      //           }
      //         }
      //         return ret;
      //       }()
      //                        .c_str(),
      //       hand->bid,
      //       [&hand]() -> std::string {
      //         switch (hand->type) {
      //         case FIVE_OF_A_KIND:
      //           return "five of a kind";
      //           break;
      //         case FOUR_OF_A_KIND:
      //           return "four of a kind";
      //           break;
      //         case FULL_HOUSE:
      //           return "full house";
      //           break;
      //         case THREE_OF_A_KIND:
      //           return "three of a kind";
      //           break;
      //         case TWO_PAIR:
      //           return "two pairs";
      //           break;
      //         case ONE_PAIR:
      //           return "one pair";
      //           break;
      //         case HIGH_CARD:
      //           return "hign card";
      //           break;
      //         case NONE:
      //           return "err";
      //           break;
      //         }
      //       }()
      //                        .c_str());
    }
  }
  auto hand_comparator = [](const hand &a, const hand &b) -> bool {
    int bigger = 0; // -1 -> a>b, +1 -> a<b
    int type_a = a.type;
    int type_b = b.type;
    if (type_a > type_b) {
      bigger = -1;
    } else if (type_a < type_b) {
      bigger = +1;
    } else {
      for (int i = 0; i < 5; i++) {
        if (a.cards[i] > b.cards[i]) {
          bigger = -1;
          break;
        } else if (a.cards[i] < b.cards[i]) {
          bigger = +1;
          break;
        }
      }
    }
    return (bigger > 0);
  };
  printf("Sorted hands:\n");
  std::ranges::sort(hands, hand_comparator);
  unsigned long int money = 0;
  for (int i = 1; i <= hands.size(); i++) {
    auto hand = &hands.at(i - 1);
    printf(
        "Hand: cards - '%s', bid - '%lu', type - %s\n",
        [&hand]() -> std::string {
          std::string ret = "";
          for (CARD c : hand->cards) {
            switch (c) {
            case C2:
              ret += '2';
              break;
            case C3:
              ret += '3';
              break;
            case C4:
              ret += '4';
              break;
            case C5:
              ret += '5';
              break;
            case C6:
              ret += '6';
              break;
            case C7:
              ret += '7';
              break;
            case C8:
              ret += '8';
              break;
            case C9:
              ret += '9';
              break;
            case CT:
              ret += 'T';
              break;
            case CJ:
              ret += 'J';
              break;
            case CQ:
              ret += 'Q';
              break;
            case CK:
              ret += 'K';
              break;
            case CA:
              ret += 'A';
              break;
            }
          }
          return ret;
        }()
                         .c_str(),
        hand->bid,
        [&hand]() -> std::string {
          switch (hand->type) {
          case FIVE_OF_A_KIND:
            return "five of a kind";
            break;
          case FOUR_OF_A_KIND:
            return "four of a kind";
            break;
          case FULL_HOUSE:
            return "full house";
            break;
          case THREE_OF_A_KIND:
            return "three of a kind";
            break;
          case TWO_PAIR:
            return "two pairs";
            break;
          case ONE_PAIR:
            return "one pair";
            break;
          case HIGH_CARD:
            return "hign card";
            break;
          case NONE:
            return "err";
            break;
          }
        }()
                         .c_str());
    money += (hands.at(i - 1).bid * i);
  }
  printf("Resulting money: %lu\n", money);
  return 0;
}