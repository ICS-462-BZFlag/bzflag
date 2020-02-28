#pragma once
#include "Node.h"
class LinkedList {
public:
    LinkedList();
    bool isEmpty();
    bool contains(Node check);
    void insertInOrder(int x, int y, int distanceTraveled, int distanceToGoal);
    void printList();
    void printHead();
    void printTail();
    Node* pop();
    void print(Node* print);
    bool remove(int x, int y, int distTraveled, int distGoal);
private:
    Node* Head;
    Node* Tail;
    int length;

};


