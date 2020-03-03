#pragma once
class Node
{
public:
    int x, y;
    float distanceTraveled;
    float distanceToGoal;
    float weight;
    Node* child;
    Node* parent;
    bool operator< (const Node& other) const
    {
        return weight > other.weight;
    }
    //bool operator== (const Node& other) const
    //{
    //    return weight == other.weight;
    //}
};


