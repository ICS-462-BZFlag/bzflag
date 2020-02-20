#pragma once
class Node
{
public:
    Node(int x, int y, Node child);

private:
    int x, y;
    Node* child;
};

Node::Node(int x, int y, Node child)
{
    this->x = x;
    this->y = y;
    this->child = &child;
};

