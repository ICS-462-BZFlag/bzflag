#pragma once
#include "Node.h"
class LinkedList {
public:
    LinkedList();
    LinkedList(Node start);
    Node lowestSearch();
    bool isEmpty();
    void addNode(Node add);
    Node remNode(Node rem);
    bool contains(Node check);
    int getLength();
private:
    Node* Head;
    Node* Tail;
    int length;
};


