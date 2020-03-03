#pragma once
class Node
{
public:
    int x, y;
    int distanceTraveled;
    int distanceToGoal;
    int weight;
    Node* child;
    Node* parent;
    bool operator< (const Node& other) const
    {
        return weight < other.weight;
    }
    bool operator> (const Node& other) const
    {
        return weight < other.weight;
    }
    bool operator== (const Node& other) const
    {
        return weight == other.weight;
    }
};

