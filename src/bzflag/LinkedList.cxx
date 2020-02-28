/************************************************
Modified from https://www.geeksforgeeks.org/data-structures/linked-list/

*************************************************/


#include "Node.h"
#include "LinkedList.h"
#include "playing.h"
#include <iostream>
#include <string>

using namespace std;

LinkedList::LinkedList()
{
    this->Head = nullptr;
    this->Tail = nullptr;
    this->length = 0;
}

bool LinkedList::isEmpty() {
    if (Head != nullptr) {
        return false;
    }
    return true;
}

bool LinkedList::contains(Node check)
{
    Node* current = this->Head;
    while (current != nullptr) {
        if (check.x == current->x && check.y == current->y) {
            return true;
        }
        else {
            current = current->child;
        }
    }
    return false;
}
void LinkedList::add(int x, int y, int distanceTraveled, int distanceToGoal) {
    Node temp;
    if (Head == nullptr) {
        Head = &temp;
        Head->x = x;
        Head->y = y;
        Head->distanceTraveled = distanceTraveled;
        Head->distanceToGoal = distanceToGoal;
        Head->weight = distanceToGoal + distanceTraveled;
        Tail = Head;
        return;
    }

    /* 2. allocate new node */
    Node* new_node = &temp;

    /* 3. put in the data  */
    new_node->x = x;
    new_node->y = y;
    new_node->distanceTraveled = distanceTraveled;
    new_node->distanceToGoal = distanceToGoal;
    new_node->weight = distanceToGoal + distanceTraveled;

    /*4. attach to Head*/
    Head->parent = new_node;
    new_node->child = Head;
    new_node->parent = nullptr;
    Head = new_node;
}
/* Given data, insert into the list based on weight */
void LinkedList::insertInOrder(int x, int y, int distanceTraveled, int distanceToGoal)
{
    Node temp;
    char buffer[128];
    sprintf(buffer, "I am Being Called");
    controlPanel->addMessage(buffer);
    memset(buffer, 0, sizeof(buffer));
    /*1. check if the given prev_node is NULL */
    if (Head == nullptr)
    {
        Head = &temp;
        Head->x = x;
        Head->y = y;
        Head->distanceTraveled = distanceTraveled;
        Head->distanceToGoal = distanceToGoal;
        Head->weight = distanceToGoal + distanceTraveled;
        Tail = Head;
        return;
    }

     /* 2. allocate new node */
    Node* new_node = &temp;

    /* 3. put in the data  */
    new_node->x = x;
    new_node->y = y;
    new_node->distanceTraveled = distanceTraveled;
    new_node->distanceToGoal = distanceToGoal;
    new_node->weight = distanceToGoal + distanceTraveled;

    //check if head is < new node
    if (Head->weight > new_node->weight) {
        new_node->child = Head;
        new_node->child->parent = new_node;
        Head = new_node;
        //exit
        return;
    }

    /* 4. Find place in list based on weight */
    Node* prev_node = Head;
    Node* current = Head;
    while (current->weight < new_node->weight) {
        prev_node = current;
        current = current->child;
    }
    /*5. set new_node child to prev nodes child*/
    new_node->child = prev_node->child;
    new_node->child->parent = new_node;

    /* 6. set the child of previous node to new_node */
    prev_node->child = new_node;
    new_node->parent = prev_node;
}

void LinkedList::printList()
{
    char buffer[128];
    //sprintf(buffer, "Robot(%d) found a flag at (%f, %f, %f)",
    //    getId(), location[0], location[1], location[2]);
    //controlPanel->addMessage(buffer);
    Node* node = Head;
    while (node != nullptr)
    {
        //cout << endl;
        //cout << "X: " << node->x << endl;
        //cout << "Y: " << node->y << endl;
        //cout << "DistTraveled: " << node->distanceTraveled << endl;
        //cout << "DistGoal: " << node->distanceToGoal << endl;
        //cout << "Weigth: " << node->weight << endl;
        sprintf(buffer, "(%d,%d,%d,%d,$d)", node->x, node->y, node->distanceTraveled, node->distanceToGoal, node->weight);
        controlPanel->addMessage(buffer);
        memset(buffer, 0, sizeof(buffer));
        node = node->child;
    }
    //if (node == nullptr) {
    //    sprintf(buffer, "no nodes in this list");
    //    controlPanel->addMessage(buffer);
    //    memset(buffer, 0, sizeof(buffer));
    //}
}
void LinkedList::printHead()
{
    cout << endl;
    cout << "X: " << Head->x << endl;
    cout << "Y: " << Head->y << endl;
    cout << "DistTraveled: " << Head->distanceTraveled << endl;
    cout << "DistGoal: " << Head->distanceToGoal << endl;
    cout << "Weigth: " << Head->weight << endl;
}
void LinkedList::printTail()
{
    cout << endl;
    cout << "X: " << Tail->x << endl;
    cout << "Y: " << Tail->y << endl;
    cout << "DistTraveled: " << Tail->distanceTraveled << endl;
    cout << "DistGoal: " << Tail->distanceToGoal << endl;
    cout << "Weigth: " << Tail->weight << endl;
}

Node* LinkedList::pop() {
    Node* ret = Head;
    Head = Head->child;
    return ret;
}
void LinkedList::print(Node* print)
{
    char buffer[128];
    sprintf(buffer, "(%d,%d,%d,%d,$d)", print->x, print->y, print->distanceTraveled, print->distanceToGoal, print->weight);
    controlPanel->addMessage(buffer);
    memset(buffer, 0, sizeof(buffer));
    //cout << endl;
    //cout << "X: " << print->x << endl;
    //cout << "Y: " << print->y << endl;
    //cout << "DistTraveled: " << print->distanceTraveled << endl;
    //cout << "DistGoal: " << print->distanceToGoal << endl;
    //cout << "Weigth: " << print->weight << endl;
}
bool LinkedList::remove(int x, int y)
{
    bool found = false;
    if (Head->x == x && Head->y == y)
    {
        found = true;
        Head = Head->child;
    }
    Node* current = Head;
    Node* previous = Head;
    while (!found) {
        if (current == nullptr) {
            found = false;
            break;
        }
        else if (current->x == x && current->y == y) {
            previous->child = current->child;
            found = true;
        }
        else {
            previous = current;
            current = current->child;
        }
    }
    return found;
}
;
