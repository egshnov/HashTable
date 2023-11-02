#include <iostream>
#include <string>
#include <vector>
#include "DLinkedList.hpp"
#include "HashTable.hpp"
#include <list>
#include <unordered_map>

template<class Key, class T>
struct TableUnit {
    Key key;
    T val;

    bool operator==(TableUnit const &node) {
        return key == node.key;
    }

    bool operator==(const Key &key_) {
        return key == key_;
    }

    bool operator!=(TableUnit const &node) {
        return key != node.key;
    }

    TableUnit &operator=(T const &value) {
        val = value;
        return *this;
    }
};

//Валгринд фейлится на гуглтесте или на хэш таблице?
int main() {
    std::vector<std::vector<int>> keys = {
            {1,       2,   3,      4,       5,     6,   7,   8,   9,    10},
            {-1,      -2,  -3,     -4,      -5},
            {101,     201, 301,    450,     580,   630, 710, 820, 1900, 10000},
            {18,      5,   1,      7,       6,     11},
            {3914914, 5,   281240, 1249124, 21412, 12412},
    };
    std::vector<std::vector<std::string>> values = {
            {"1",       "2",             "3",      "4",       "5",     "6", "7", "8", "9", "10"},
            {"Vasya",   "Petya",         "Sasha",  "Oleg",    "Dima"},

            {"1",       "2",             "3",      "4",       "5",     "6", "7", "8", "9", "10"},

            {"3",       "9",             "28",     "124",     "21",    "112"},

            {"3914914", "9011914-90000", "281240", "1249124", "21412", "12412"}
    };
    structures::HashTable<int, std::string> table;
    for (int i = 0; i < keys[1].size(); i++) {
        table.Insert(keys[1][i], values[1][i]);
    }

    for (int i: keys[1]) {
        std::cout << "{" << i << ":" << table.At(i) << "} ";
    }
    std::cout << std::endl;
    std::cout << "start by iter" << std::endl;
    for (auto &it: table) {
        std::cout << "{" << it.key << ":" << it.val << "}" << " " << std::endl;
    }
    std::cout << "stop by iter" << std::endl;
    std::cout << std::endl;

    for (auto it = table.begin(); it != table.end(); it++) {
        std::cout << "{" << it->key << ":" << it->val << "}" << " ";
    }
    std::cout << std::endl;
    for (auto it = --table.end(); it != table.begin(); it--) {
        std::cout << "{" << it->key << ":" << it->val << "}" << " ";
    }
//    structures::HashTable<int, std::string> table2(table);
//    structures::HashTable<int, std::string> table3(table); //= std::move(table2);
//    table3.begin()->key = 10;
//    auto it1 = table.begin();
//    auto it2 = table3.begin();
//    std::cout << std::endl;
//    while (it1 != table.end() && it2 != table3.end()) {
//        std::cout << "it1: " << "{" << it1->key << ":" << it1->val << "}" << " it2:" << "{" << it2->key << ":"
//                  << it2->val << "}" << std::endl;
//        it1++;
//        it2++;
//    }

//    for (auto &it: table) {
//        std::cout << "{" << it.key << ":" << it.val << "}" << " ";
//    }

//    std::cout << '\n';
//    for (auto it = (--table.cend()); it != table.cbegin(); it--) {
//        std::cout << "{" << it->key << ":" << it->val << "}" << " ";
//    }
//
//    structures::DLinkedList<int> list;
//    for (int i = 0; i < 10; i++) {
//        list.PushBack(i);
//    }

}