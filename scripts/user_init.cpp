/**
 * @file main.cpp
 * @author WolodiaM (w_melnyk@outlook.com)
 * @brief Main user file of CBuild
 * @version 1.0
 * @date 2023-01-18
 *
 * @license GPL v3.0 or later
 *
 * Copyright (C) 2023  WolodiaM
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
// CBuild includes
#include "CBuild/CBuild_defs.hpp"
#include "CBuild/build/g++.hpp"
#include "CBuild/filesystem++.hpp"
#include "CBuild/print.hpp"
#include "CBuild/register.hpp"
#include "CBuild/task/Task.hpp"
// task TODO
class TODO : public CBuild::Task {
private:
  std::string text;

public:
  TODO(int id, std::string text)
      : CBuild::Task("TODO" + std::to_string(id), {}) {
    this->text = text;
  }
  virtual void call(std::vector<std::string> args) override {
    CBuild::print("Not done!!!!", CBuild::RED);
    CBuild::print(text, CBuild::YELLOW);
  }
};
// Tasks
CBuild::GXX challenge1("challenge1");
CBuild::GXX challenge2("challenge2");
CBuild::GXX challenge3("challenge3");
CBuild::GXX challenge4("challenge4");
CBuild::GXX challenge5("challenge5");
CBuild::GXX challenge6("challenge6");
CBuild::GXX challenge7("challenge7");
CBuild::GXX challenge8("challenge8");
CBuild::GXX challenge9("challenge9");
CBuild::GXX challenge10("challenge10");
CBuild::GXX challenge10mt("challenge10mt");
CBuild::GXX challenge11("challenge11");
CBuild::GXX challenge12("challenge12");
CBuild::GXX challenge13("challenge13");
CBuild::GXX challenge14("challenge14");
CBuild::GXX challenge15("challenge15");
CBuild::GXX challenge16("challenge16");
CBuild::GXX challenge17("challenge17");
CBuild::GXX challenge18("challenge18");
CBuild::GXX challenge19("challenge19");
CBuild::GXX challenge20("challenge20");
CBuild::GXX challenge21("challenge21");
CBuild::GXX challenge22("challenge22");
CBuild::GXX challenge23("challenge23");
CBuild::GXX challenge24("challenge24");
TODO todo24(24, "Matcher not work.");
CBuild::GXX challenge25("challenge25");
CBuild::GXX challenge26("challenge26");
CBuild::GXX challenge27("challenge27");
CBuild::GXX challenge28("challenge28");
TODO todo28(28, "Caching not work.");
CBuild::GXX challenge29("challenge29");
CBuild::GXX challenge30("challenge30");
CBuild::GXX challenge31("challenge31");
CBuild::GXX challenge32("challenge32");
CBuild::GXX challenge33("challenge33");
CBuild::GXX challenge35("challenge35");
TODO todo35(35, "Do tasks 35 and 26.");
CBuild::GXX challenge36("challenge36");
void init() {
  challenge1.add_folder("src/task1/");
  challenge1.set_standart("c++20");
  challenge1.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge1);
  challenge2.add_folder("src/task2/");
  challenge2.set_standart("c++20");
  challenge2.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge2);
  challenge3.add_folder("src/task3/");
  challenge3.set_standart("c++20");
  challenge3.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge3);
  challenge4.add_folder("src/task4/");
  challenge4.set_standart("c++20");
  challenge4.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge4);
  challenge5.add_folder("src/task5/");
  challenge5.set_standart("c++20");
  challenge5.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge5);
  challenge6.add_folder("src/task6/");
  challenge6.set_standart("c++20");
  challenge6.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge6);
  challenge7.add_folder("src/task7/");
  challenge7.set_standart("c++20");
  challenge7.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge7);
  challenge8.add_folder("src/task8/");
  challenge8.set_standart("c++20");
  challenge8.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge8);
  challenge9.add_folder("src/task9/");
  challenge9.set_standart("c++20");
  challenge9.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge9);
  challenge10.add_folder("src/task10/");
  challenge10.set_standart("c++20");
  challenge10.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge10);
  challenge10mt.add_folder("src/task10mt/");
  challenge10mt.set_standart("c++20");
  challenge10mt.add_link_arg("-lstdc++");
  challenge10mt.multithreaded_target();
  CBuild::Registry::RegisterTarget(&challenge10mt);
  challenge11.add_folder("src/task11/");
  challenge11.set_standart("c++20");
  challenge11.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge11);
  challenge12.add_folder("src/task12/");
  challenge12.set_standart("c++20");
  challenge12.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge12);
  challenge13.add_folder("src/task13/");
  challenge13.set_standart("c++20");
  challenge13.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge13);
  challenge14.add_folder("src/task14/");
  challenge14.set_standart("c++20");
  challenge14.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge14);
  challenge15.add_folder("src/task15/");
  challenge15.set_standart("c++20");
  challenge15.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge15);
  challenge16.add_folder("src/task16/");
  challenge16.set_standart("c++20");
  challenge16.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge16);
  challenge17.add_folder("src/task17/");
  challenge17.set_standart("c++20");
  challenge17.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge17);
  challenge18.add_folder("src/task18/");
  challenge18.set_standart("c++20");
  challenge18.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge18);
  challenge19.add_folder("src/task19/");
  challenge19.set_standart("c++20");
  challenge19.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge19);
  challenge20.add_folder("src/task20/");
  challenge20.set_standart("c++20");
  challenge20.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge20);
  challenge21.add_folder("src/task21/");
  challenge21.set_standart("c++20");
  challenge21.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge21);
  challenge22.add_folder("src/task22/");
  challenge22.set_standart("c++20");
  challenge22.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge22);
  challenge23.add_folder("src/task23/");
  challenge23.set_standart("c++20");
  challenge23.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge23);
  challenge24.add_folder("src/task24/");
  challenge24.set_standart("c++20");
  challenge24.add_link_arg("-lstdc++");
  challenge24.add_requirment("TODO24", CBuild::PRE);
  CBuild::Registry::RegisterTarget(&challenge24);
  CBuild::Registry::RegisterTask(&todo24);
  challenge25.add_folder("src/task25/");
  challenge25.set_standart("c++20");
  challenge25.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge25);
  challenge26.add_folder("src/task26/");
  challenge26.set_standart("c++20");
  challenge26.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge26);
  challenge27.add_folder("src/task27/");
  challenge27.set_standart("c++20");
  challenge27.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge27);
  challenge28.add_folder("src/task28/");
  challenge28.set_standart("c++20");
  challenge28.add_link_arg("-lstdc++");
  challenge28.add_requirment("TODO28");
  CBuild::Registry::RegisterTarget(&challenge28);
  CBuild::Registry::RegisterTask(&todo28);
  challenge29.add_folder("src/task29/");
  challenge29.set_standart("c++20");
  challenge29.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge29);
  challenge30.add_folder("src/task30/");
  challenge30.set_standart("c++20");
  challenge30.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge30);
  challenge31.add_folder("src/task31/");
  challenge31.set_standart("c++20");
  challenge31.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge31);
  challenge32.add_folder("src/task32/");
  challenge32.set_standart("c++20");
  challenge32.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge32);
  challenge33.add_folder("src/task33/");
  challenge33.set_standart("c++20");
  challenge33.add_link_arg("-lstdc++");
  CBuild::Registry::RegisterTarget(&challenge33);
  challenge35.add_folder("src/task35/");
  challenge35.set_standart("c++20");
  challenge35.add_link_arg("-lstdc++");
  challenge35.add_requirment("TODO35");
  CBuild::Registry::RegisterTarget(&challenge35);
  CBuild::Registry::RegisterTask(&todo35);
  challenge36.add_folder("src/task36/");
  challenge36.set_standart("c++20");
  challenge36.add_link_arg("-lstdc++");
  challenge36.add_link_arg("-lm");
  CBuild::Registry::RegisterTarget(&challenge36);
}