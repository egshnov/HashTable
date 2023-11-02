#include "gtest/gtest.h"
#include "HashTable.hpp"
#include <vector>
#include <string>
#include "initializer_list"
#include <random>
#include <unordered_map>

namespace st = structures;

template<class Key, class T>
void tstConstr(std::initializer_list<Key> keys_, std::initializer_list<T> values_) {
    std::vector<Key> keys(keys_);
    std::vector<T> values(values_);
    st::HashTable<Key, T> table1;
    EXPECT_EQ(keys.size(), values.size());
    for (int i = 0; i < keys.size(); i++) {
        table1.Insert(keys[i], values[i]);
    }

    for (int i = 0; i < keys.size(); i++) {
        EXPECT_EQ(table1.At(keys[i]), values[i]);
    }

    st::HashTable<Key, T> table2(table1);
    for (int i = 0; i < keys.size(); i++) {
        EXPECT_EQ(table2.At(keys[i]), table1.At(keys[i]));
    }
    st::HashTable<Key, T> table3(std::move(table2));
    for (int i = 0; i < keys.size(); i++) {
        EXPECT_EQ(table3.At(keys[i]), table1.At(keys[i]));
    }

    st::HashTable<Key, T> table4 = std::move(table3);
    for (int i = 0; i < keys.size(); i++) {
        EXPECT_EQ(table1.At(keys[i]), table4.At(keys[i]));
    }
    table4.Resize(20);
    auto it1 = table1.begin();

    while (it1 != table1.end()) {
        EXPECT_EQ(it1->val, table4.At(it1->key));
        it1++;
    }
}

TEST(TestHashTable, TestConstructors) {
    tstConstr({1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
              {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
    tstConstr({-1, -2, -3, -4, -5}, {"Vasya", "Petya", "Sasha", "Oleg", "Dima"});
    tstConstr({101, 201, 301, 450, 580, 630, 710, 820, 1900, 10000},
              {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"});
    tstConstr({18, 5, 1, 7, 6, 11},
              {"3", "9", "28", "124", "21", "112"});
    tstConstr({3914914, 5, 281240, 1249124, 21412, 12412},
              {"3914914", "9011914-90000", "281240", "1249124", "21412", "12412"});
    tstConstr({"3914914", "5", "281240", "1249124", "21412", "12412"},
              {"3914914", "9011914-90000", "281240", "1249124", "21412", "12412"});
}

template<class Key, class T>
void tstIter(std::initializer_list<Key> keys_, std::initializer_list<T> values_) {
    std::vector<Key> keys(keys_);
    std::vector<T> values(values_);
    st::HashTable<Key, T> table1;
    EXPECT_EQ(keys.size(), values.size());
    for (int i = 0; i < keys.size(); i++) {
        table1.Insert(keys[i], values[i]);
    }
    st::HashTable<Key, T> const const_table(table1);

    {
        auto it = table1.begin();
        int i = 0;
        while (it != table1.end()) {
            EXPECT_EQ(it->key, keys[i]);
            EXPECT_EQ(it->val, values[i]);
            it++;
            i++;
        }
    }

    {
        auto itc = table1.cbegin();
        int i = 0;
        while (itc != table1.cend()) {
            EXPECT_EQ(itc->key, keys[i]);
            EXPECT_EQ(itc->val, values[i]);
            itc++;
            i++;
        }
    }

    {
        auto it2 = const_table.begin();
        int i2 = 0;
        while (it2 != const_table.end()) {
            EXPECT_EQ(it2->key, keys[i2]);
            EXPECT_EQ(it2->val, values[i2]);
            it2++;
            i2++;
        }
    }

    {
        auto it2 = const_table.cbegin();
        int i = 0;
        while (it2 != const_table.cend()) {
            EXPECT_EQ(it2->key, keys[i]);
            EXPECT_EQ(it2->val, values[i]);
            it2++;
            i++;
        }
    }
}


//TEST(TestHashTable, TestIter) {
//    tstIter({1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
//            {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
//    tstIter({-1, -2, -3, -4, -5}, {"Vasya", "Petya", "Sasha", "Oleg", "Dima"});
//    tstIter({101, 201, 301, 450, 580, 630, 710, 820, 1900, 10000},
//            {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"});
//    tstIter({18, 5, 1, 7, 6, 11},
//            {"3", "9", "28", "124", "21", "112"});
//    tstIter({3914914, 5, 281240, 1249124, 21412, 12412},
//            {"3914914", "9011914-90000", "281240", "1249124", "21412", "12412"});
//}

static int GetRandomNumber(int min, int max) {
    srand(time(NULL));
    int num = min + rand() % (max - min + 1);

    return num;
}

template<class Key, class T>
void testDelInsert(std::initializer_list<Key> keys_, std::initializer_list<T> values_) {
    std::vector<Key> keys(keys_);
    std::vector<T> values(values_);
    EXPECT_EQ(keys.size(), values.size());
    st::HashTable<Key, T> table;
    for (int i = 0; i < keys.size(); i++) {
        table.Insert(keys[i], values[i]);
    }
    table.Del(keys[0]);
    keys.erase(keys.begin());
    values.erase(values.begin());
    table.Del(*(--keys.end()));
    keys.pop_back();
    values.pop_back();
    for (int i = 0; i < keys.size(); i++) {
        EXPECT_EQ(table.At(keys[i]), values[i]);
    }

    for (int i = 0; i < keys.size() / 2; i++) {
        int ind = GetRandomNumber(0, keys.size() - 1);
        auto itk = keys.begin();
        auto itv = values.begin();
        std::advance(itk, ind);
        std::advance(itv, ind);
        table.Del(*itk);
        keys.erase(itk);
        values.erase(itv);
        std::vector<T> tmp;
        for (int j = 0; j < keys.size(); j++) {
            EXPECT_EQ(table.At(keys[j]), values[j]);
        }
    }
}

TEST(TestHashTable, TestDelInsert) {
    testDelInsert({1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
                  {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
    testDelInsert({-1, -2, -3, -4, -5}, {"Vasya", "Petya", "Sasha", "Oleg", "Dima"});
    testDelInsert({101, 201, 301, 450, 580, 630, 710, 820, 1900, 10000},
                  {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"});
    testDelInsert({18, 5, 1, 7, 6, 11},
                  {"3", "9", "28", "124", "21", "112"});
    testDelInsert({3914914, 5, 281240, 1249124, 21412, 12412},
                  {"3914914", "9011914-90000", "281240", "1249124", "21412", "12412"});
    testDelInsert({"3914914", "5", "281240", "1249124", "21412", "12412"},
                  {"3914914", "9011914-90000", "281240", "1249124", "21412", "12412"});
}

template<class Key, class T>
void testDelEmplace(std::initializer_list<Key> keys_, std::initializer_list<T> values_) {
    std::vector<Key> keys(keys_);
    std::vector<T> values(values_);
    EXPECT_EQ(keys.size(), values.size());
    st::HashTable<Key, T> table;
    for (int i = 0; i < keys.size(); i++) {
        table.Emplace(keys[i], values[i]);
    }
    table.Del(keys[0]);
    keys.erase(keys.begin());
    values.erase(values.begin());
    table.Del(*(--keys.end()));
    keys.pop_back();
    values.pop_back();
    for (int i = 0; i < keys.size(); i++) {
        EXPECT_EQ(table.At(keys[i]), values[i]);
    }

    for (int i = 0; i < keys.size() / 2; i++) {
        int ind = GetRandomNumber(0, keys.size() - 1);
        auto itk = keys.begin();
        auto itv = values.begin();
        std::advance(itk, ind);
        std::advance(itv, ind);
        table.Del(*itk);
        keys.erase(itk);
        values.erase(itv);
        std::vector<T> tmp;
        for (int j = 0; j < keys.size(); j++) {
            EXPECT_EQ(table.At(keys[j]), values[j]);
        }
    }
}

TEST(TestHashTable, TestDelEmplace) {
    testDelEmplace({1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
                   {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
    testDelEmplace({-1, -2, -3, -4, -5}, {"Vasya", "Petya", "Sasha", "Oleg", "Dima"});
    testDelEmplace({101, 201, 301, 450, 580, 630, 710, 820, 1900, 10000},
                   {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"});
    testDelEmplace({18, 5, 1, 7, 6, 11},
                   {"3", "9", "28", "124", "21", "112"});
    testDelEmplace({3914914, 5, 281240, 1249124, 21412, 12412},
                   {"3914914", "9011914-90000", "281240", "1249124", "21412", "12412"});
    testDelEmplace({"3914914", "5", "281240", "1249124", "21412", "12412"},
                   {"3914914", "9011914-90000", "281240", "1249124", "21412", "12412"});

}

template<class Key, class T>
void testOperator(std::initializer_list<Key> keys_, std::initializer_list<T> values_) {
    std::vector<Key> keys(keys_);
    std::vector<T> values(values_);
    EXPECT_EQ(keys.size(), values.size());
    st::HashTable<Key, T> table;
    for (int i = 0; i < keys.size(); i++) {
        table[keys[i]] = values[i];
    }
    for (int i = 0; i < keys.size(); i++) {
        EXPECT_EQ(table[keys[i]], values[i]);
    }
    for (int i = 0; i < keys.size(); i++) {
        table[keys[i]] = values[keys.size() - i - 1];
    }
    for (int i = 0; i < keys.size(); i++) {
        EXPECT_EQ(table[keys[i]], values[keys.size() - i - 1]);
    }
}

TEST(TestHashTable, TestOperator) {
    testOperator({1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
                 {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
    testOperator({-1, -2, -3, -4, -5}, {"Vasya", "Petya", "Sasha", "Oleg", "Dima"});
    testOperator({101, 201, 301, 450, 580, 630, 710, 820, 1900, 10000},
                 {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"});
    testOperator({18, 5, 1, 7, 6, 11},
                 {"3", "9", "28", "124", "21", "112"});
    testOperator({3914914, 5, 281240, 1249124, 21412, 12412},
                 {"3914914", "9011914-90000", "281240", "1249124", "21412", "12412"});
    testOperator({"3914914", "5", "281240", "1249124", "21412", "12412"},
                 {"3914914", "9011914-90000", "281240", "1249124", "21412", "12412"});
}

class MyHashFunction {
public:
    size_t operator()(const std::string &p) const {
        size_t sum = 0;
        for (char i: p) {
            sum += i;
        }
        return sum;
    }
};
//
//template<class Key, class T>
//void testMyHash(std::initializer_list<Key> keys_, std::initializer_list<T> values_) {
//    auto my_hash = [](std::string const &foo) {
//        size_t sum = 0;
//        for (char i: foo) {
//            sum += i;
//        }
//        return sum;
//    };
//    std::vector<Key> keys(keys_);
//    std::vector<T> values(values_);
//    EXPECT_EQ(keys.size(), values.size());
//    //st::HashTable<Key, T, decltype(my_hash)> table;
//    std::unordered_map<Key,T, my_hash> map;
//    for (int i = 0; i < keys.size(); i++) {
//        table.Insert(keys[i], values[i]);
//    }
//    table.Del(keys[0]);
//    keys.erase(keys.begin());
//    values.erase(values.begin());
//    table.Del(*(--keys.end()));
//    keys.pop_back();
//    values.pop_back();
//    for (int i = 0; i < keys.size(); i++) {
//        EXPECT_EQ(table.At(keys[i]), values[i]);
//    }
//
//    for (int i = 0; i < keys.size() / 2; i++) {
//        int ind = GetRandomNumber(0, keys.size() - 1);
//        auto itk = keys.begin();
//        auto itv = values.begin();
//        std::advance(itk, ind);
//        std::advance(itv, ind);
//        table.Del(*itk);
//        keys.erase(itk);
//        values.erase(itv);
//        std::vector<T> tmp;
//        for (int j = 0; j < keys.size(); j++) {
//            EXPECT_EQ(table.At(keys[j]), values[j]);
//        }
//    }
//}
//
//TEST(TestHashTable, TestHash) {
//    testMyHash({"3914914", "5", "281240", "1249124", "21412", "12412"},
//               {"3914914", "9011914-90000", "281240", "1249124", "21412", "12412"});
//    testMyHash({"3914914", "5", "281240", "1249124", "21412", "12412"},
//               {"3914914", "9011914-90000", "281240", "1249124", "21412", "12412"});
//}
