/************************************************
*Node functions from Node.h
* includes constructors, getter/setters, and hasChild
*
*************************************************/


#include "Node.h"
Node::Node() {
    this->x = 0;
    this->y = 0;
    this->child = nullptr;
    this->childExists = false;
    this->distanceTraveled = 0;
    this->weight = 0;
}
Node::Node(int x, int y, int distanceTraveled, int distToGoal) {
    this->x = x;
    this->y = y;
    this->distanceTraveled = distanceTraveled;
    this->weight = distanceTraveled + distToGoal;
    this->childExists = false;
    this->child = nullptr;
}
Node::Node(int x, int y, int distance, int toGoal, Node child)
{
    this->x = x;
    this->y = y;
    this->distanceTraveled = distance;
    this->weight = distance + toGoal;
    this->child = &child;
    this->childExists = true;
}
bool Node::operator==(Node check) {
    if (x = check.getX()) {
        if (y = check.getX()) {
            if (child = check.getChild()) {
                if (distanceTraveled = check.getDistanceTraveled()) {
                    if (weight = check.getWeight()) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
bool Node::operator!=(Node check) {
    if (x = check.getX()) {
        if (y = check.getX()) {
            if (child = check.getChild()) {
                if (distanceTraveled = check.getDistanceTraveled()) {
                    if (weight = check.getWeight()) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}
bool Node::hasChild() {
    return childExists;
}
int Node::getX()
{
    return x;
}

int Node::getY() {
    return y;
}
void Node::setChild(Node child) {
    if (&child == nullptr) {
        childExists = false;
        this->child = false;
    }
    else {
        this->child = &child;
        this->childExists = true;
    }
}
Node* Node::getChild() {
    return child;
}
int Node::getDistanceTraveled() {
    return distanceTraveled;
}
int Node::getDistanceToGoal() {
    return weight - distanceTraveled;
}
int Node::getWeight() {
    return weight;
};
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

LinkedList::LinkedList(Node start) {
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
        return results;
    }
}
;
