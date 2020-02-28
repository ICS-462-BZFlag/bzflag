/************************************************
*Node functions from Node.h
* includes constructors, getter/setters, and hasChild
*
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

/* Given data, insert into the list based on weight */
void LinkedList::insertInOrder(int x, int y, int distanceTraveled, int distanceToGoal)
{
    /*1. check if the given prev_node is NULL */
    if (Head == nullptr)
    {
        Head = new Node();
        Head->x = x;
        Head->y = y;
        Head->distanceTraveled = distanceTraveled;
        Head->distanceToGoal = distanceToGoal;
        Head->weight = distanceToGoal + distanceTraveled;
        Tail = Head;
        return;
    }

    /* 2. allocate new node */
    Node* new_node = new Node();

    /* 3. put in the data  */
    new_node->x = x;
    new_node->y = y;
    new_node->distanceTraveled = distanceTraveled;
    new_node->distanceToGoal = distanceToGoal;
    new_node->weight = distanceToGoal + distanceTraveled;
    //check if head is < new node
    if (Head->weight > new_node->weight) {
        new_node->child = Head;
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

    /* 6. set the child of previous node to new_node */
    prev_node->child = new_node;
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
        sprintf(buffer, "(Position X, Position Y, DisTrav, DistGoal, Weight)\n (%d,%d,%d,%d,$d)", node->x, node->y, node->distanceTraveled, node->distanceToGoal, node->weight);
        controlPanel->addMessage(buffer);
        memset(buffer, 0, sizeof(buffer));
        node = node->child;
    }
    if (node == nullptr) {
        sprintf(buffer, "no nodes in this list");
        controlPanel->addMessage(buffer);
        memset(buffer, 0, sizeof(buffer));
    }
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
    cout << endl;
    cout << "X: " << print->x << endl;
    cout << "Y: " << print->y << endl;
    cout << "DistTraveled: " << print->distanceTraveled << endl;
    cout << "DistGoal: " << print->distanceToGoal << endl;
    cout << "Weigth: " << print->weight << endl;
}
bool LinkedList::remove(int x, int y, int distTraveled, int distGoal)
{
    bool found = false;
    if (Head->x == x && Head->y == y && Head->distanceTraveled == distTraveled && Head->distanceToGoal == distGoal)
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
        else if (current->x == x && current->y == y && current->distanceTraveled == distTraveled && current->distanceToGoal == distGoal) {
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
