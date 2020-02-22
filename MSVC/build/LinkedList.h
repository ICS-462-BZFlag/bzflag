#pragma once
#include "Node.h"
#include <string>
class LinkedList {
public:
    Node lowestSearch();
    LinkedList(Node start);
    bool isEmpty();
    void addNode(Node add);
    Node remNode(Node rem);
    std::string printList();
private:
    Node* Head;
    Node* Tail;
};

