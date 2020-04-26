// Author: Peh Yu Xiang, 2020-02-16
// For CS2040C AY19/20 Semester 2 Lab Group B08
//
// Sample code to demonstrate tester

template<typename T>
class PriorityQueue {
    T value;
public:
    bool empty() { return true; }
    unsigned int size() { return 0; }
    void push(T item) { value = item; }
    T top() { return value; }
    void pop() { return; }
};
