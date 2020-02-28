#pragma once
class Node
{
public:
    int x, y;
    int distanceTraveled;
    int distanceToGoal;
    int weight;
    Node* child;
};
