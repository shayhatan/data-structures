//
// Created by USER on 20/02/2022.
//

#ifndef DATA_STRUCTURES_STACK_H
#define DATA_STRUCTURES_STACK_H

template<class T>
class Stack {
    int capacity;
    T *stack;
    int count;

public:

    //at the beginning the array size is 1
    Stack(): capacity(10), stack(new T[10]), count(0){
    }

    //we have to delete the array in the end
    ~Stack() {
        delete[] stack;
    }

    //we push a given item onto the stack
    //O(1) if no need to resize
    void push(const T &item) {

        //sometimes we have to resize the array  [O(N)]
        if (count == capacity) {
            resize(2 * capacity);
        }

        //insert the the given item into the array
        stack[count++] = item;
    }

    //O(1) if no need to resize
    T pop() {

        //the stack may be empty
        if (isEmpty())
            throw std::out_of_range("Stack is empty.");

        //the item we want to pop
        T item_to_pop = stack[--count];

        //if we have popped too many items: we have to resize the array [O(N)]
        if (count > 0 && count == capacity / 4) {
            resize(capacity / 2);
        }

        //return the item itself
        return item_to_pop;
    }

    //O(1) operation
    bool isEmpty() const {
        return count == 0;
    }

    //O(1) operation
    int size() const {
        return count;
    }

    // O(n) because we have to copy the items one by one
    void resize(int _capacity) {

        T *stack_copy = new T[_capacity];

        for (int i = 0; i < count; i++) {
            stack_copy[i] = stack[i];
        }

        delete[] stack;
        stack = stack_copy;
        capacity = _capacity;
    }
};

#endif //DATA_STRUCTURES_STACK_H
