#pragma once

#include <utility>
#include <iterator>
#include <iostream>

#include <gtest/gtest.h>


namespace structures {
    template<class Key, class Type, class Hash>
    class HashTable;

    template<class T>
    class DLinkedList {
        template<class Key, class Type, class Hash>
        friend
        class HashTable;

    public:

        bool IsEmpty() const {
            return sentinel_->next == sentinel_;
        }

    private:
        struct Node {
            Node() : next(this), prev(this) {}

            virtual ~Node() = default; // looks bad, try to fix it

            Node *next;
            Node *prev;
        };

        struct DNode : public Node {
            T value;

            explicit DNode(T value) : Node(), value{value} {};

            template<typename... Args>
            explicit DNode(Args &&...args):Node(), value(std::forward<Args>(args)...) {}

//            explicit DNode(T &&data) : Node(), value(std::forward<T>(data)) {}
//
//            template<typename... Args>
//            explicit DNode(Args &&...args):Node(), value(std::forward<Args>(args)...) {}
        };


        template<typename ValueType>
        struct IteratorBase {
            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = ValueType;
            using pointer = ValueType *;
            using reference = ValueType &;
            Node *ptr;

            IteratorBase(Node *node) : ptr(node) {}


            bool operator==(IteratorBase const &rhs) const {
                return ptr == rhs.ptr;
            }

            bool operator!=(IteratorBase const &rhs) const {
                return !(*this == rhs);
            }

            IteratorBase &operator++() { // prefix
                ptr = ptr->next;
                return *this;
            }

            IteratorBase &operator--() { //prefix
                ptr = ptr->prev;
                return *this;
            }

            IteratorBase operator++(int) { // postfix
                IteratorBase result(*this);
                ++(*this);
                return result;
            }

            IteratorBase operator--(int) { //postfix
                IteratorBase result(*this);
                --(*this);
                return result;
            }

            reference operator*() const {
                return static_cast<DNode *>(ptr)->value;
            }

            pointer operator->() {
                return &(static_cast<DNode *>(ptr)->value);
            }
        };

        Node *sentinel_ = new Node();

        //sentinel_->next = head
        //sentinel_->prev = tail
        //sentinel = end()
        void BackChaining(DNode *target) {
//            if (IsEmpty()) {
//                sentinel_->next = target;
//                sentinel_->prev = target;
//                target->prev = sentinel_;
//                target->next = sentinel_;
//            } else {
            sentinel_->prev->next = target;
            target->prev = sentinel_->prev; // target->next is already nullptr
            sentinel_->prev = target;
            target->next = sentinel_;
            //}
        }

        void FrontChaining(DNode *target) {
//            if (IsEmpty()) {


//                sentinel_->next = target;
//                sentinel_->prev = target;
//                target->prev = sentinel_;
//                target->next = sentinel_;
//            } else {
            sentinel_->next->prev = target;
            target->next = sentinel_->next;
            sentinel_->next = target;
            target->prev = sentinel_;
            // }
        }

    public:
        using Iterator = IteratorBase<T>;
        using ConstIterator = IteratorBase<const T>;

        void PushBack(T const &value) {
            BackChaining(new DNode(value));
        }

        void PushFront(T const &value) {
            FrontChaining(new DNode(value));
        }

        void PushBack(T &&value) {
            BackChaining(new DNode(std::move(value)));
        }

        void PushFront(T &&value) {
            FrontChaining(new DNode(std::move(value)));
        }

        // Emplacing value into list
        template<typename... Args>
        void EmplaceBack(Args &&...args) {
            BackChaining(new DNode(std::forward<Args>(args)...));
        }

        template<typename... Args>
        void EmplaceFront(Args &&...args) {
            FrontChaining(new DNode(std::forward<Args>(args)...));
        }

        DLinkedList() = default;

        DLinkedList(DLinkedList const &list) { //: DLinkedList() {
            auto it = list.sentinel_->next;
            while (it != list.sentinel_) {
                PushBack(static_cast<DNode *>(it)->value);
                it = it->next;
            }

        }

        DLinkedList &operator=(DLinkedList const &list) {
            if (this != &list) {
                DLinkedList tmp(list);
                std::swap(sentinel_, tmp.sentinel_);
            }
            return *this;
        }

        DLinkedList(DLinkedList &&list) noexcept { // :DLinkedList() {
            std::swap(sentinel_, list.sentinel_);
        }

        DLinkedList &operator=(DLinkedList &&list) noexcept {
            std::swap(sentinel_, list.sentinel_);
            return *this;
        }

        void Clear() {
            // explicit type, see google style guide on type deduction for rationale
            auto ptr = sentinel_->next;
            while (ptr != sentinel_) {
                auto tmp = ptr;
                ptr = ptr->next;
                delete tmp;
            }
            sentinel_->next = sentinel_;
            sentinel_->prev = sentinel_;
        }

        ~DLinkedList() {
            Clear();
            delete sentinel_;
        }


        Iterator begin() {
            return Iterator(sentinel_->next);
        }

        Iterator end() {
            return Iterator(sentinel_);
        }

        ConstIterator begin() const {
            return ConstIterator(sentinel_->next);
        }

        ConstIterator end() const {
            return ConstIterator(sentinel_);
        }

        ConstIterator cbegin() const {
            return ConstIterator(sentinel_->next);
        }

        ConstIterator cend() const {
            return ConstIterator(sentinel_);
        }


        Node *InsertBeforePointer(Node *next_node, T const &value) { //camel case
            if (next_node == nullptr) throw std::out_of_range("nullptr doesn't belong to the list");

            // redundant
            if (next_node == sentinel_) { // means we are inserting to the end of the list;
                PushBack(value);
                return sentinel_->prev;
            }
            if (next_node == sentinel_->next) { // means we are inserting to the front of the list;
                PushFront(value);
                return sentinel_->next;
            }

            auto target = new DNode(value);
            target->next = next_node;
            target->prev = next_node->prev;
            next_node->prev->next = target;
            next_node->prev = target;
            return target;
        }

        // don't accept iterators by reference
        Iterator InsertBefore(Iterator &it, T const &value) {
            return Iterator(InsertBeforePointer(it.ptr, value));
        }

        ConstIterator InsertBefore(ConstIterator &it, T const &value) {
            return ConstIterator(InsertBeforePointer(it.ptr, value));
        }

        Iterator InsertBefore(size_t ind, T const &value) { //
            auto it = begin();
            std::advance(it, ind);
            return InsertBefore(it, value);
        }

        void PopFront() {
            if (IsEmpty()) throw std::out_of_range("cannot pop out of empty list");
            auto target = sentinel_->next;
            target->next->prev = target->prev;
            target->prev->next = target->next;
            delete target;
        }

        void PopBack() {
            if (IsEmpty()) throw std::out_of_range("cannot pop out of empty list");
            auto target = sentinel_->prev;
            target->next->prev = target->prev;
            target->prev->next = target->next;
            delete target;
        }

        void DelByPointer(Node *target) {
            if (IsEmpty()) throw std::out_of_range("cannot delete out of empty list");
            target->next->prev = target->prev;
            target->prev->next = target->next;
            delete target;
        }

        void Del(Iterator const &it) {
            DelByPointer(it.ptr);
        }

        void Del(size_t ind) {
            auto it = begin();
            std::advance(it, ind);
            Del(it);
        }

        Iterator find(const T &value) {
            return std::find(begin(), end(), value);
        }

        ConstIterator find(const T &value) const {
            return std::find(cbegin(), cend(), value);
        }
    };

    template<class T>
    bool operator==(DLinkedList<T> const &l1, DLinkedList<T> const &l2) {
        auto it1 = l1.begin();
        auto it2 = l2.begin();

        while (it1 != l1.end() && it2 != l2.end()) {
            if (*it1 != *it2) {
                return false;
            }
            it1++;
            it2++;
        }
        return it1 == l1.end() && it2 == l2.end();
    }

    template<class T>
    bool operator!=(DLinkedList<T> const &l1, DLinkedList<T> const &l2) {
        return !(l1 == l2);
    }

}