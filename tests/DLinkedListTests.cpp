#include "gtest/gtest.h"
#include "DLinkedList.hpp"
#include <vector>
#include <random>
#include "initializer_list"

namespace st = structures;

template<typename T>
void testConstruct(const std::initializer_list<T> &vec) {
    st::DLinkedList<T> l1;
    for (auto i: vec) {
        l1.PushBack(i);
    }

    EXPECT_EQ(l1.IsEmpty(), false);
    st::DLinkedList<T> l2 = l1;
    EXPECT_EQ(l1, l2);
    st::DLinkedList<T> l2_2(l2);
    EXPECT_EQ(l2_2, l2);

    l2 = l2_2;
    EXPECT_EQ(l2_2, l2);

    st::DLinkedList<T> l3(std::move(l2));
    EXPECT_EQ(l1, l3);
    st::DLinkedList<T> l4 = std::move(l3);
    EXPECT_EQ(l1, l4);

    st::DLinkedList<T> l5;
    for (auto i: vec) {
        l5.EmplaceBack(i);
    }
    EXPECT_EQ(l5, l1);
    st::DLinkedList<T> const l6(std::move(l5));
    EXPECT_EQ(l6, l1);

}

TEST(TestLinkedList, Construct) {

    testConstruct({1, 2, 3, 4, 5, 6, 7, 7, 8, 9, 10});
    testConstruct({0, 0, 0, 0, 0, 0, 0, 0,});
    testConstruct({-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -12, -13});
    testConstruct({"asdsf", "adfa", "afafas", "fadfasf", "afasfaf", "gagaaad", "asfasg"});
    testConstruct({"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "10", "134134"});
    testConstruct({"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "10", "134134"});
    testConstruct({std::make_pair(1, 1), std::make_pair(2, 3), std::make_pair(3, 10)});
    testConstruct({std::make_pair('a', 'b'), std::make_pair('c', 'd'), std::make_pair('e', 'f')});
    testConstruct({std::make_pair("one", "two"), std::make_pair("three", "four"), std::make_pair("five", "six")});

}

template<typename T>
void testIter(const std::initializer_list<T> &list) {
    st::DLinkedList<T> l1;
    EXPECT_EQ(l1.begin(), l1.end());
    for (auto i: list) {
        l1.PushBack(i);
    }
    std::vector<T> vec1(list);
    auto it1 = l1.begin();
    auto it2 = vec1.begin();
    while (it1 != l1.end() && it2 != vec1.end()) {
        EXPECT_EQ(*it1, *it2);
        it1++;
        it2++;
    }
    auto itl = --l1.end();
    auto itv = --vec1.end();
    while (itl != l1.begin() && itv != vec1.end()) {
        EXPECT_EQ(*itl, *itv);
        --itl;
        --itv;
    }

    std::vector<T> vec2{};
    std::vector<T> vec3{};
    for (auto i = l1.begin(); i != l1.end(); i++) {
        vec2.push_back(*i);
    }
    EXPECT_EQ(vec1, vec2);

    for (auto i = l1.cbegin(); i != l1.cend(); i++) {
        vec3.push_back(*i);
    }
    EXPECT_EQ(vec1, vec3);

}

TEST(TestLinkedList, Iter) {

    testIter({1, 2, 3, 4, 5, 6, 7, 7, 8, 9, 10});
    testIter({0, 0, 0, 0, 0, 0, 0, 0,});
    testIter({-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -12, -13});
    testIter({"asdsf", "adfa", "afafas", "fadfasf", "afasfaf", "gagaaad", "asfasg"});
    testIter({"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "10", "134134"});
    testIter({"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "10", "134134"});
    testIter({std::make_pair(1, 1), std::make_pair(2, 3), std::make_pair(3, 10)});
    testIter({std::make_pair('a', 'b'), std::make_pair('c', 'd'), std::make_pair('e', 'f')});
    testIter({std::make_pair("one", "two"), std::make_pair("three", "four"), std::make_pair("five", "six")});
}

int GetRandomNumber(int min, int max) {
    srand(time(NULL));
    int num = min + rand() % (max - min + 1);

    return num;
}

template<typename T>
void testDelInsert(const std::initializer_list<T> &list) {
    std::vector<T> ideal(list);
    st::DLinkedList<T> l1;
    for (auto i: list) {
        l1.PushBack(i);
    }

    auto it = ideal.begin();
    ideal.insert(it, ideal[0]);
    l1.InsertBefore(0, ideal[0]);
    l1.InsertBefore(ideal.size() - 1, ideal[0]);
    it = ideal.begin();
    std::advance(it, ideal.size() - 1);
    ideal.insert(it, ideal[0]);
    {
        std::vector<T> tmp;
        for (auto i = l1.begin(); i != l1.end(); i++) {
            tmp.push_back(*i);
        }
        EXPECT_EQ(tmp, ideal);
    }


    for (int i = 0; i < 1000; i++) {
        int ind = GetRandomNumber(0, ideal.size() - 1);
        int after = GetRandomNumber(0, ideal.size() - 1);
        it = ideal.begin();
        std::advance(it, after);
        ideal.insert(it, ideal[ind]);
        l1.InsertBefore(after, ideal[ind]);
    }

    {
        std::vector<T> tmp;
        for (auto i = l1.begin(); i != l1.end(); i++) {
            tmp.push_back(*i);
        }
        EXPECT_EQ(tmp, ideal);
    }
    l1.PopFront();
    l1.PopBack();
    ideal.erase(ideal.begin());
    ideal.pop_back();
    {
        std::vector<T> tmp;
        for (auto i = l1.begin(); i != l1.end(); i++) {
            tmp.push_back(*i);
        }
        EXPECT_EQ(tmp, ideal);
    }


    for (int i = 0; i < ideal.size(); i++) {
        int ind = GetRandomNumber(0, ideal.size() - 1);
        it = ideal.begin();
        std::advance(it, ind);
        ideal.erase(it);
        l1.Del(ind);
        std::vector<T> tmp;
        for (auto itl = l1.begin(); itl != l1.end(); itl++) {
            tmp.push_back(*itl);
        }
        EXPECT_EQ(tmp, ideal);
    }

}

TEST(TestLinkedList, DelInsert) {
    testDelInsert({1, 2, 3, 4, 5, 6, 7, 7, 8, 9, 10});
    testDelInsert({0, 0, 0, 0, 0, 0, 0, 0,});
    testDelInsert({-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -12, -13});
    testDelInsert({"asdsf", "adfa", "afafas", "fadfasf", "afasfaf", "gagaaad", "asfasg"});
    testDelInsert({"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "10", "134134"});
    testDelInsert({"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "10", "134134"});
    testDelInsert({std::make_pair(1, 1), std::make_pair(2, 3), std::make_pair(3, 10)});
    testDelInsert({std::make_pair('a', 'b'), std::make_pair('c', 'd'), std::make_pair('e', 'f')});
    testDelInsert({std::make_pair("one", "two"), std::make_pair("three", "four"), std::make_pair("five", "six")});
}

template<typename T>
void testInsertConstIter(const std::initializer_list<T> &list) {
    std::vector<T> ideal(list);
    st::DLinkedList<T> l1;
    for (auto i: list) {
        l1.PushBack(i);
    }
    st::DLinkedList<T> const l2(l1);
    {
        auto itc = l2.cbegin();
        auto it = l1.begin();
        while (it != l1.end() && itc != l2.end()) {
            EXPECT_EQ(*itc, *it);
            itc++;
            it++;
        }
    }
    {
        auto it = ideal.begin();
        ideal.insert(it, ideal[0]);
        auto itl = l1.cbegin();
        l1.InsertBefore(itl, ideal[0]);
        l1.InsertBefore(--l1.cend(), ideal[0]);
        it = ideal.begin();
        std::advance(it, ideal.size() - 1);
        ideal.insert(it, ideal[0]);
    }
    {
        std::vector<T> tmp;
        for (auto i = l1.begin(); i != l1.end(); i++) {
            tmp.push_back(*i);
        }
        EXPECT_EQ(tmp, ideal);
    }
    for (int i = 0; i < 100; i++) {
        int ind = GetRandomNumber(0, ideal.size() - 1);
        int after = GetRandomNumber(0, ideal.size() - 1);
        auto it = ideal.begin();
        std::advance(it, after);
        ideal.insert(it, ideal[ind]);
        auto itc = l1.cbegin();
        std::advance(itc, after);
        l1.InsertBefore(itc, ideal[ind]);
        std::vector<T> tmp;
        for (auto j = l1.begin(); j != l1.end(); j++) {
            tmp.push_back(*j);
        }
        EXPECT_EQ(tmp, ideal);
    }


}

TEST(TestLinkedList, InsertConstIter) {
    testInsertConstIter({1, 2, 3, 4, 5, 6, 7, 7, 8, 9, 10});
    testInsertConstIter({0, 0, 0, 0, 0, 0, 0, 0,});
    testInsertConstIter({-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -12, -13});
    testInsertConstIter({"asdsf", "adfa", "afafas", "fadfasf", "afasfaf", "gagaaad", "asfasg"});
    testInsertConstIter({"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "10", "134134"});
    testInsertConstIter({"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "10", "134134"});
    testInsertConstIter({std::make_pair(1, 1), std::make_pair(2, 3), std::make_pair(3, 10)});
    testInsertConstIter({std::make_pair('a', 'b'), std::make_pair('c', 'd'), std::make_pair('e', 'f')});
    testInsertConstIter({std::make_pair("one", "two"), std::make_pair("three", "four"), std::make_pair("five", "six")});
}

template<typename T>
void testFind(const std::initializer_list<T> &list, T val, bool res) {
    std::vector<T> ideal(list);
    st::DLinkedList<T> l1;
    for (auto i: list) {
        l1.PushBack(i);
    }
    EXPECT_EQ(l1.find(val) != l1.end(), res);
}

TEST(TestLinkedList, find) {
    testFind({1, 2, 3, 4, 5, 6, 7, 7, 8, 9, 10}, 1, true);
    testFind({0, 0, 0, 0, 0, 0, 0, 0,}, 1, false);
    testFind({0, 0, 0, 0, 0, 0, 0, 0,}, 0, true);
    testFind({-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -12, -13}, -13, true);
    testFind({"asdsf", "adfa", "afafas", "fadfasf", "afasfaf", "gagaaad", "asfasg"}, "fadfasf", true);
    testFind({"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "10", "134134"}, "", false);
    testFind({"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "10", "134134"}, "134134", true);
    testFind({std::make_pair(1, 1), std::make_pair(1, 1), std::make_pair(2, 3), std::make_pair(3, 10)},
             std::make_pair(3, 10), true);
    testFind({std::make_pair('a', 'b'), std::make_pair('c', 'd'), std::make_pair('e', 'f')},
             std::make_pair('0', '0'),
             false);
    testFind({std::make_pair("one", "two"), std::make_pair("three", "four"), std::make_pair("five", "six")},
             std::make_pair("three", "four"), true);
}
