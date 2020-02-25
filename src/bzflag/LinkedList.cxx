#pragma once
#include "Node.h"
#include "LinkedList.h"
LinkedList::LinkedList(Node start) {
    this->Head = &start;
    this->Tail = &start;
    this->length = 1;
}
LinkedList::LinkedList() {
    this->Head = nullptr;
    this->Tail = nullptr;
    this->length = 0;
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
    if (Head == nullptr) {
        Head = &add;
        Tail = &add;
    }
    else {
        Tail->setChild(add);
        Tail = &add;
    }
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

bool LinkedList::contains(Node check)
{
    Node* current = this->Head;
    while (current != nullptr) {
        if (check.getX() == current->getX() && check.getY() == current->getY()) {
            return true;
        }
        else {
            current = current->getChild();
        }
    }
    return false;
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
