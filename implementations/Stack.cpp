// Author: Peh Yu Xiang, 2020-02-16
// For CS2040C AY19/20 Semester 2 Lab Group B08
//
// Sample code to demonstrate tester

template<typename T>
class Stack {
private:
    struct node {
        T item;
        node *next = nullptr;
    };
    node *head = nullptr;
    unsigned int length = 0;

private:
    // O(n) time, to use somehow
    void reverse() {
        node *p = head;
        node *prev = nullptr;
        while (p != nullptr) {
            node *next = p->next;
            p->next = prev;
            prev = p;
            p = next;
        }
        head = prev;
    }

public:
    Stack() {}
    ~Stack() {
        node *p = head;
        while (p != nullptr) {
            node *next = p->next;
            delete p;
            p = next;
        }
        head = nullptr;
    }

    void push(T item) {
        length += 1;
        node *p = new node();
        p->next = head;
        p->item = item;
        head = p;
    }

    void pop() {
        length -= 1;
        node *p = head;
        head = head->next;
        delete p;
    }

    T top() {
        return head->item;
    }

    bool empty() {
        return head == nullptr;
    }

    unsigned int size() {
        return length;
    }
};
