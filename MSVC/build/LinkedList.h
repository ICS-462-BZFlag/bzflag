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
private:
    Node* Head;
    Node* Tail;
};
LinkedList::LinkedList(Node start){
    this->Head = &start;
    this->Tail = &start;
}
Node LinkedList::lowestSearch() {
    Node current = *Head;
    Node lowest = *Head;
    while (current.hasChild()) {
        if (lowest.getWeight() > current.getWeight()) {
            lowest = current;
        }
        current = *current.getChild();
    }
    return lowest;
}
void LinkedList::addNode(Node add) {
   Tail->setChild(add);
    Tail = &add;
}
Node LinkedList::remNode(Node rem) {
    Node current = *Head;
    while (current != rem) {
        current = *current.getChild();
    }
}
bool LinkedList::isEmpty() {
    if (Head != NULL) {
        return false;
    }
    return true;
}
;
