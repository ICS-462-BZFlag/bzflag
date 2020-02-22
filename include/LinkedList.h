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
    Node prev;
    while (current != rem) {
        prev = current;
        current = *current.getChild();
    }
    prev.setChild(*current.getChild());
    return current;
}

bool LinkedList::isEmpty() {
    if (Head != NULL) {
        return false;
    }
    return true;
}
std::string LinkedList::printList() {
    std::string results = "";
    Node current = *Head;
    while (current.hasChild()) {
        results += "(" + current.getX + "," + current.getY() + "," + current.getDistanceToGoal() +  "," + current.getWeight() + ")";
    }
}
;
