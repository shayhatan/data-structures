//
// Created by USER on 02/11/2021.
//

#ifndef EX2_MTM_SORTEDLIST_H
#define EX2_MTM_SORTEDLIST_H

#include <iostream>
#include <exception>

/*
            My implementation for double sorted linked list
                                                                        */


namespace mtm {
    template<class T>
    class SortedList {
    private:
        struct Item;
        Item *head;
        Item *tail;

        static void insertCurrent(const T &data, Item *here);

    public:
        SortedList() : head(new Item), tail(new Item) {
            head->next = tail;
            tail->prev = head;
        }

        ~SortedList() {
            delete head;
        }

        class const_iterator;

        // the cast insert (head->next)& into class const_iterator
        const_iterator begin() const {
            return (const_iterator) head->next;
        }

        // the cast insert (head->next)& into class const_iterator
        const_iterator end() const {
            return (const_iterator) tail;
        }

        SortedList(const SortedList &other_list);

        SortedList &operator=(const SortedList &other_list);

        int length() const;

        void insert(const T &data);

        void remove(const_iterator iter) const;

        template<class Func>
        SortedList<T> apply(Func &f) const;

        template<class Bool>
        SortedList<T> filter(Bool &f) const;


    };


    template<class T>
    struct SortedList<T>::Item {
        Item *next;
        Item *prev;
        T *_data;

        explicit Item(T *data = nullptr) : _data(data), next(nullptr), prev(nullptr) {}

        Item(const Item &a) = default;

        Item &operator=(const Item &a) = default;

        ~Item() {
            delete _data;
            delete next;
        }
    };


    template<class T>
    class SortedList<T>::const_iterator {
        friend class SortedList<T>;

        Item *p;
    private:
        explicit const_iterator(Item *pt = nullptr) : p(pt) {}

    public:
        const_iterator(const const_iterator &iter) = default;

        const_iterator &operator=(const const_iterator &iter) = default;

        int operator!=(const const_iterator itr) const {
            return (p != itr.p);
        }

        int operator==(const const_iterator itr) const {
            return (p == itr.p);
        }

        const_iterator &operator++() {
            if (p == nullptr) {
                throw std::out_of_range("");
            }
            p = p->next;
            return *this;
        }

        const T &operator*() const {
            if (p == nullptr || p->_data == nullptr) {
                throw std::out_of_range("");
            }
            return *(p->_data);
        }


    };

    template<class T>
    void SortedList<T>::insertCurrent(const T &data, SortedList::Item *here) {
        Item *to_insert = new Item(new T(data));
        to_insert->next = here;
        to_insert->prev = here->prev;
        here->prev->next = to_insert;
        here->prev = to_insert;
    }

    template<class T>
    SortedList<T>::SortedList(const SortedList &other_list) :
            head(new Item), tail(new Item) {
        head->next = tail;
        tail->prev = head;
        const_iterator done = other_list.end();
        const_iterator it = other_list.begin();
        for (it != done; ++it) {
            insertCurrent((*it), tail);
        }
    }

    template<class T>
    SortedList<T> &SortedList<T>::operator=(const SortedList &other_list) {

        if (this == &other_list) {
            return *this;
        }
        auto *a = new SortedList<T>;
        delete this->head; // must try allocate first to avoid leak possibility encase of exception
        head = a->head;
        tail = a->tail;
        const_iterator done = other_list.end();
        const_iterator it = other_list.begin();
        for (it != done; ++it) {
            insertCurrent((*it), tail);
        }
        return *this;
    }

    template<class T>
    int SortedList<T>::length() const {
        int size = 0;
        const_iterator done = end();
        const_iterator it = begin();

        for (it; it != done; ++it) {
            ++size;
        }
        return size;
    }

    template<class T>
    void SortedList<T>::insert(const T &data) {
        const_iterator done = end();
        const_iterator it = begin();

        for (it != done; ++it) {
            if (data < (*it)) {
                insertCurrent(data, (it.p));
                return;
            }

        }
        insertCurrent(data, it.p);
    }

    template<class T>
    void SortedList<T>::remove(const SortedList::const_iterator iter) const {
        Item *temp = iter.p->next;
        iter.p->prev->next = temp;
        temp->prev = iter.p->prev;
        iter.p->next = nullptr;
        delete iter.p;
    }

    template<class T>
    template<class Func>
    SortedList<T> SortedList<T>::apply(Func &f) const {
        const_iterator done = end();
        const_iterator it = begin();
        SortedList<T> new_list;
        for (it; it != done; ++it) {
            new_list.insert(f(*it));
        }
        return new_list;
    }

    template<class T>
    template<class Bool>
    SortedList<T> SortedList<T>::filter(Bool &f) const {
        const_iterator done = end();
        const_iterator it = begin();
        SortedList<T> new_list;
        for (it; it != done; ++it) {
            if (f(*it)) {
                new_list.insert(*it);
            }
        }
        return new_list;
    }


}
#endif //EX2_MTM_SORTEDLIST_H
