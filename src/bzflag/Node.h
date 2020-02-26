#pragma once
class Node
{
public:
    Node(int x, int y, int distTraveled, int toGoal, Node child);
    bool operator==(Node check);
    bool operator!=(Node check);
    Node(int x, int y, int distTraveled, int toGoal);
    Node();
    int getX();
    int getY();
    void setChild(Node child);
    void setWeight(int boat);
    Node* getChild();
    bool hasChild();
    int getDistanceTraveled();
    int getWeight();
    int getDistanceToGoal();
    bool isNode(Node check);


private:
    int x, y;
    bool childExists;
    int distanceTraveled;
    int weight;
    Node* child;
    bool actualNode;
};
