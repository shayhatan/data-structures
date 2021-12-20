//
// Created by USER on 20/12/2021.
//

#ifndef DATA_STRUCTURES_HASHTABLE_H
#define DATA_STRUCTURES_HASHTABLE_H
#ifndef HashTable_hpp
#define HashTable_hpp

#include <vector>
#include <list>
#include <exception>

class EmptyException : std::exception {
public:
    EmptyException() = default;

    const char *what() const noexcept override {
        return "Item doesn't exist";
    }
};

template<class T>
class HashTable {

    //the maximum item we can store in the map
    static constexpr int TABLE_SIZE = 10;

    //every item we insert is a (key,value) pair
    struct HashItem;

    //a single array index may store more item: we store these items in a list
    std::vector<std::list<HashItem *>> hash_table;

public:

    HashTable();

    ~HashTable();

    //O(1) if the hash function is perfect
    T get(int key);

    //O(1) if the hash function is perfect
    void put(int key, T value);

private:
    static int hash(int key);
};

template<class T>
struct HashTable<T>::HashItem {
    int key = 0;
    T *value = 0;

    HashItem(int _key, T *_value) : key(_key), value(_value) {
    }

    ~HashItem() {
        delete value;
    }
};

template<class T>
HashTable<T>::HashTable() {
    hash_table.resize(TABLE_SIZE);
}

template<class T>
T HashTable<T>::get(int key) {
    int array_index = hash(key);
    std::list<HashItem *> &items = hash_table[array_index];
    if (items.empty()) {
        throw EmptyException{};
    }
    for (HashItem *const &item : items) {
        //check if item already inserted
        if (item->key == key) {
            return *(item->value);
        }
    }
    throw EmptyException{};
}


// should use more complicated hash function
template<class T>
int HashTable<T>::hash(int key) {
    return key % TABLE_SIZE;
}

template<class T>
void HashTable<T>::put(int key, T value) {
    int hash_array_index = hash(key);

    std::list<HashItem *> &items = hash_table[hash_array_index];

    if (!items.empty()) {
        for (HashItem *const &item : items) {
            //check if item already inserted
            if (item->key == key) {
                return;
            }
        }
    }
    HashItem *item = new HashItem(key, new T(value));
    items.push_back(item);
}

template<class T>
HashTable<T>::~HashTable() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        if (!hash_table[i].empty()) {
            auto it = hash_table[i].begin();
            while (it != hash_table[i].end()) {
                delete hash_table[i].front();
                hash_table[i].erase(it++);
            }
        }
    }
}

#endif
#endif //DATA_STRUCTURES_HASHTABLE_H
