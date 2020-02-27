#pragma once
#include "Node.h"
class LinkedList {
public:
    LinkedList();
    LinkedList(Node start);
    void lowestSearch(Node lowest);
    bool isEmpty();
    void addNode(Node add);
    void addAtHead(Node add);
    void remNode(Node rem);
    bool contains(Node check);
    int getLength();
    Node popHead();
private:
    Node* Head;
    Node* Tail;
    int length;
};


