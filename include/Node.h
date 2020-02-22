#pragma once
class Node
{
public:
    Node(int x, int y, int distance, int toGoal, Node child);
    bool operator==(Node check);
    bool operator!=(Node check);
    Node(int x, int y, int distance, int toGoal);
    Node();
    int getX();
    int getY();
    void setChild(Node child);
    Node* getChild();
    bool hasChild();
    int getDistanceTraveled();
    int getWeight();
    int getDistanceToGoal();


private:
    int x, y;
    bool childExists;
    int distanceTraveled;
    int weight;
    Node* child;

};
Node::Node() {
    this->x = 0;
    this->y = 0;
    this->child = NULL;
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
    this->child = NULL;
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
bool Node::operator==(Node check){
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
    this->child = &child;
    this->childExists = true;
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

