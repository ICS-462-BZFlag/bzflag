#pragma once
#include "Node.h"
class LinkedList {
public:
    LinkedList();
    LinkedList(Node start);
    Node* lowestSearch();
    bool isEmpty();
    void addNode(Node add);
    void addAtHead(Node add);
    Node* remNode(Node rem);
    bool contains(Node check);
    int getLength();
    Node popHead();
private:
    Node* Head;
    Node* Tail;
    int length;
};


