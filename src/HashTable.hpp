#pragma once

#include "DLinkedList.hpp"

namespace structures {
    template<class Key, class T, class Hash = std::hash<Key>>
    class HashTable {
    private:
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

            // && operations

            TableUnit(const Key &key_, const T &val_) : key(key_), val(val_) {}

            template<typename... Args>
            explicit TableUnit(const Key &key_, Args &&...args):key(key_), val(std::forward<Args>(args)...) {}
        };

        DLinkedList<TableUnit> *buckets_maintainer_;
        size_t num_of_buckets_;
        size_t ind_of_first_not_empty_;

        size_t get_ind(const Key &key) {
            return Hash{}(key) % num_of_buckets_;
        }

        template<typename ValueType>
        struct IteratorBase {
            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = ValueType;
            using pointer = ValueType *;
            using reference = ValueType &;
            using ListIterator = typename DLinkedList<TableUnit>::template IteratorBase<ValueType>;

            IteratorBase(DLinkedList<TableUnit> *buckets_maintainer_, size_t cur_bucket_, size_t num_of_buckets_,
                         ListIterator it_) :
                    buckets_maintainer(buckets_maintainer_), cur_bucket(cur_bucket_), num_of_buckets(num_of_buckets_),
                    ptr(it_) {}

            reference operator*() const { return ptr.operator*(); }

            pointer operator->() { return ptr.operator->(); }

            IteratorBase &operator++() { //skips empty_buckets;
                ptr++;
                if (ptr == buckets_maintainer[cur_bucket].end() && cur_bucket != num_of_buckets - 1) {
                    cur_bucket++;
                    while (cur_bucket < num_of_buckets - 1 && buckets_maintainer[cur_bucket].IsEmpty()) {
                        cur_bucket++;
                    }
                    ptr = buckets_maintainer[cur_bucket].begin();
                }
                return *this;
            }

            IteratorBase operator++(int) {
                auto tmp = *this;
                ++(*this);
                return tmp;
            }

            IteratorBase &operator--() {
                if (ptr == buckets_maintainer[cur_bucket].begin()) {
                    cur_bucket--;
                    while (cur_bucket >= 0 && buckets_maintainer[cur_bucket].IsEmpty()) {
                        cur_bucket--;
                    }
                    ptr = --buckets_maintainer[cur_bucket].end();
                } else {
                    ptr--;
                }
                return *this;
            }

            IteratorBase operator--(int) {
                auto tmp = *this;
                --(*this);
                return tmp;
            }

            bool operator==(IteratorBase const &it) {
                return cur_bucket == it.cur_bucket && ptr == it.ptr;
            }

            bool operator!=(IteratorBase const &it) {
                return !(*this == it);
            }


        private:
            ListIterator ptr;
            size_t num_of_buckets;
            size_t cur_bucket;
            DLinkedList<TableUnit> *buckets_maintainer;
        };

    public:
        using Iterator = IteratorBase<TableUnit>;
        using ConstIterator = IteratorBase<const TableUnit>;

        Iterator begin() {
            return Iterator(buckets_maintainer_, ind_of_first_not_empty_, num_of_buckets_,
                            buckets_maintainer_[ind_of_first_not_empty_].begin());
        }

        Iterator end() {
            return Iterator(buckets_maintainer_, num_of_buckets_ - 1, num_of_buckets_,
                            buckets_maintainer_[num_of_buckets_ - 1].end());
        }

        ConstIterator begin() const {
            return ConstIterator(buckets_maintainer_, ind_of_first_not_empty_, num_of_buckets_,
                                 buckets_maintainer_[ind_of_first_not_empty_].cbegin());
        }

        ConstIterator end() const {
            return ConstIterator(buckets_maintainer_, num_of_buckets_ - 1, num_of_buckets_,
                                 buckets_maintainer_[num_of_buckets_ - 1].cend());
        }

        ConstIterator cbegin() const {
            return ConstIterator(buckets_maintainer_, ind_of_first_not_empty_, num_of_buckets_,
                                 buckets_maintainer_[ind_of_first_not_empty_].cbegin());
        }

        ConstIterator cend() const {
            return ConstIterator(buckets_maintainer_, num_of_buckets_ - 1, num_of_buckets_,
                                 buckets_maintainer_[num_of_buckets_ - 1].cend());
        }

        explicit HashTable(size_t num_of_buckets = 17) : num_of_buckets_(num_of_buckets),
                                                         ind_of_first_not_empty_(num_of_buckets_) {
            buckets_maintainer_ = new DLinkedList<TableUnit>[num_of_buckets_];
            //std::cout << "default constructor" << std::endl;
        }

        HashTable(HashTable const &table) : num_of_buckets_(table.num_of_buckets_), ind_of_first_not_empty_(
                table.ind_of_first_not_empty_) {
            buckets_maintainer_ = new DLinkedList<TableUnit>[num_of_buckets_];
            for (int i = 0; i < num_of_buckets_; i++) {
                buckets_maintainer_[i] = table.buckets_maintainer_[i];
            }
        }

        HashTable &operator=(HashTable const &table) {
            if (this != &table) {
                HashTable tmp(table);
                std::swap(tmp.num_of_buckets_, this->num_of_buckets_);
                std::swap(tmp.buckets_maintainer_, this->buckets_maintainer_);
                std::swap(tmp.ind_of_first_not_empty_, this->ind_of_first_not_empty_);
            }
            return *this;
        }

        HashTable(HashTable &&table) noexcept {
            buckets_maintainer_ = nullptr;
            num_of_buckets_ = 0;
            ind_of_first_not_empty_ = 0;
            std::swap(buckets_maintainer_, table.buckets_maintainer_);
            std::swap(num_of_buckets_, table.num_of_buckets_);
            std::swap(ind_of_first_not_empty_, table.ind_of_first_not_empty_);
        }

        HashTable &operator=(HashTable &&table) noexcept {
            std::swap(buckets_maintainer_, table.buckets_maintainer_);
            std::swap(num_of_buckets_, table.num_of_buckets_);
            std::swap(ind_of_first_not_empty_, table.ind_of_first_not_empty_);
            return *this;
        }

        ~HashTable() {
            delete[] buckets_maintainer_;
        }

        void Insert(const Key &key, const T &value) {
            size_t ind = get_ind(key);
            if (ind < ind_of_first_not_empty_) {
                ind_of_first_not_empty_ = ind;
            }
            auto it = std::find(buckets_maintainer_[ind].begin(), buckets_maintainer_[ind].end(), key);
            if (it != buckets_maintainer_[ind].end()) {
                *it = value;
            } else {
                buckets_maintainer_[ind].PushFront({key, value});
            }
        }

        template<typename... Args>
        void Emplace(Key key, Args &&...args) {
            size_t ind = get_ind(key);
            if (ind < ind_of_first_not_empty_) {
                ind_of_first_not_empty_ = ind;
            }
            auto it = std::find(buckets_maintainer_[ind].begin(), buckets_maintainer_[ind].end(), key);
            if (it != buckets_maintainer_[ind].end()) {
                *it = TableUnit(key, std::forward<Args>(args)...); //?
            } else {
                buckets_maintainer_[ind].EmplaceFront(TableUnit(key, std::forward<Args>(args)...));
            }
        }

        T At(const Key &key) {
            size_t ind = get_ind(key);
            auto it = std::find(buckets_maintainer_[ind].begin(), buckets_maintainer_[ind].end(), key);
            if (it != buckets_maintainer_[ind].end()) {
                return it->val;
            } else {
                throw std::out_of_range("given key doesn't belong to the hashmap");
            }
        }

        void Del(const Key &key) {
            size_t ind = get_ind(key);
            auto it = std::find(buckets_maintainer_[ind].begin(), buckets_maintainer_[ind].end(), key);
            if (it != buckets_maintainer_[ind].end()) {
                buckets_maintainer_[ind].Del(it);
            } else {
                throw std::out_of_range("given key doesn't belong to the hashmap");
            }
            if (ind_of_first_not_empty_ == ind && buckets_maintainer_[ind_of_first_not_empty_++].IsEmpty()) {
                while (ind_of_first_not_empty_ < num_of_buckets_ &&
                       buckets_maintainer_[ind_of_first_not_empty_].IsEmpty()) {
                    ++ind_of_first_not_empty_;
                }
            }
        }

        //T must have a default constructor
        T &operator[](Key const &key) {
            size_t ind = get_ind(key);
            auto it = std::find(buckets_maintainer_[ind].begin(), buckets_maintainer_[ind].end(), key);
            if (it == buckets_maintainer_[ind].end()) {
//                buckets_maintainer_[ind].PushFront(TableUnit(key, T()));
//                it = buckets_maintainer_[ind].begin();
                it = buckets_maintainer_[ind].InsertBefore(it, TableUnit(key, T()));
            }
            return it->val;

        }

        void Resize(size_t new_num_of_buckets) {
            auto *new_buckets_maintainer = new DLinkedList<TableUnit>[new_num_of_buckets];
            auto it = begin();
            while (it != end()) {
                size_t ind = Hash{}(it->key) % new_num_of_buckets;
                new_buckets_maintainer[ind].PushFront(*it);
                ++it;
            }
            delete[] buckets_maintainer_;
            num_of_buckets_ = new_num_of_buckets;
            buckets_maintainer_ = new_buckets_maintainer;
        }

    };

}