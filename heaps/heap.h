//
// Created by USER on 20/12/2021.
//

#ifndef DATA_STRUCTURES_HEAP_H
#define DATA_STRUCTURES_HEAP_H


#include <vector>
using std::vector;

template<class T>
class Heap {
    std::vector<T *> heap;
    int heap_size = 0;

    constexpr static int CAPACITY = 100;

public:
    Heap();

    ~Heap();

    //insertion takes O(1) running time BUT we have to make sure that the
    //heap properties are not violated (it takes O(logN) because of the fixUp() method)
    void insert(const T &new_item);

    //we return the root node. Because it is a max heap the root is the max item
    //of course because of the array representation it takes O(1) time
    //this is the peek() method

    const T &getMax() const;

    T getMax();

    //it returns the maximum item + removes it from the heap
    //note: we just do not care about that item - O(logN)
    T poll();

    //we have N items and we want to sort them with a heap
    //every poll() operation takes O(logN) time because of the fixDown() method thats why
    //the overall running time complexity is O(NlogN) for heapsort
    void heapSort();

private:
    //we consider the last item and checks whether swaps are needed or not
    //running time: O(logN)
    void fixUp(int index);

    //we have a given item in the heap and we consider all the item BELOW and check
    //whether the heap properties are violated or not
    void fixDown(int index);

    //checks whether the heap is full or not
    //note: we are not able to insert more items than the CAPACITY
    bool isHeapFull() const;

    //basically its poll with out deleting
    void sort();
};

template<class T>
Heap<T>::Heap() {
    heap.resize(CAPACITY, 0);
}

template<class T>
Heap<T>::~Heap() {
    for (int i = 0; i < heap_size; ++i) {
        delete heap[i];
    }
}

template<class T>
void Heap<T>::insert(const T &new_item) {
    if (isHeapFull()) {
        throw std::overflow_error("Heap is full...");
    }
    heap[heap_size] = new T(new_item);
    //we insert the item to the last position of the array: of course the heap
    //properties may be violated so we have to fix it if necessary
    fixUp(heap_size++);

}

template<class T>
bool Heap<T>::isHeapFull() const {
    return heap_size == CAPACITY;
}

template<class T>
void Heap<T>::fixUp(int index) {
    int parent_index = (index - 1) / 2;

    if (index > 0 && *heap[index] > *heap[parent_index]) {
        std::swap(heap[index], heap[parent_index]);
        fixUp(parent_index);
    }
}

template<class T>
T Heap<T>::getMax() {
    if (heap_size == 0) {
        throw std::length_error("Heap is empty");
    }

    return *heap[0];
}

template<class T>
const T &Heap<T>::getMax() const {
    if (heap_size == 0) {
        throw std::length_error("Heap is empty");
    }

    return *heap[0];
}

template<class T>
void Heap<T>::fixDown(int index) {
    //every node have  children: left child and right child
    //in the array the node i has left child with index *i+1 and right child with index 2*i+1
    int index_left = 2 * index + 1;
    int index_right = 2 * index + 2;
    //max heap so the parent node is always greater than the children
    int index_largest = index;

    //if the left child is greater than the parent: largest is the left node
    if (index_left < heap_size && *heap[index] < *heap[index_left]) {
        index_largest = index_left;
    }
    //if the right child is greater than the left child: largest is the right node
    if (index_right < heap_size && *heap[index_largest] < *heap[index_right]) {
        index_largest = index_right;
    }
    //we do not want to swap items with themselves
    if (index != index_largest) {
        std::swap(heap[index], heap[index_largest]);
        fixDown(index_largest);
    }
}

template<class T>
T Heap<T>::poll() {
    T max = getMax();

    delete heap[0];
    heap[0] = nullptr;

    std::swap(heap[0], heap[--heap_size]);

    fixDown(0);
    return max;
}

template<class T>
void Heap<T>::heapSort() {
    //we decrease the size of the heap in the poll() method so we have to store it (!!!)
    int size = heap_size;

    //we call the poll operation as many times as the number of items in the heap
    for (int i = 0; i < size; ++i) {
        sort();
    }
    heap_size = size;

    for (int i = 0; i < size; ++i) {
        std::cout << *heap[i] << ' ';
    }
    std::cout <<std::endl;
}

template<class T>
void Heap<T>::sort() {
    T max = getMax();

    std::swap(heap[0], heap[--heap_size]);

    fixDown(0);
}

#endif //DATA_STRUCTURES_HEAP_H
