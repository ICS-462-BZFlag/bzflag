#pragma once
class Node
{
public:
    Node(int x, int y, Node child);
    Node(int x, int y);
    int getX();
    int getY();
    void setChild(Node child);
    Node* getChild();
    bool hasChild();

private:
    int x, y;
    bool childExists;
    Node* child;
};
Node::Node(int x, int y) {
    this->x = x;
    this->y = y;
    this->childExists = false;
}
Node::Node(int x, int y, Node child)
{
    this->x = x;
    this->y = y;
    this->child = &child;
    this->childExists = true;
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
};

