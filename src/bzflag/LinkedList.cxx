#pragma once
#include "Node.h"
#include "LinkedList.h" 
#include <string>

using namespace std;
LinkedList::LinkedList(Node start) {
    this->Head = &start;
    this->Tail = &start;
    this->length = 1;
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
    length++;
}

Node LinkedList::remNode(Node rem) {
    Node current = *Head;
    Node prev;
    while (current != rem) {
        prev = current;
        current = *current.getChild();
    }
    prev.setChild(*current.getChild());
    length--;
    return current;
}

int LinkedList::getLength()
{
    return length;
}

bool LinkedList::isEmpty() {
    if (Head != NULL) {
        return false;
    }
    return true;
}
;
