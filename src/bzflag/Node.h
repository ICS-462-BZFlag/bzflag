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
