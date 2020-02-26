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
    this->actualNode = false;
}
Node::Node(int x, int y, int distanceTraveled, int distToGoal) {
    this->x = x;
    this->y = y;
    this->distanceTraveled = distanceTraveled;
    this->weight = distanceTraveled + distToGoal;
    this->childExists = false;
    this->child = nullptr;
    this->actualNode = true;
}
Node::Node(int x, int y, int distanceTraveled, int toGoal, Node child)
{
    this->x = x;
    this->y = y;
    this->distanceTraveled = distanceTraveled;
    this->weight = distanceTraveled + toGoal;
    this->child = &child;
    this->childExists = true;
    this->actualNode = true;
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
void Node::setWeight(float boat) {
    this->weight = boat;
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
bool Node::isNode(Node check)
{
    return check.actualNode;
}
int Node::getWeight() {
    return weight;
};
