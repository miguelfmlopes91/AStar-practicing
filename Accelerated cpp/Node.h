//
//  Node.h
//  
//
//  Created by Miguel Lopes on 08/10/2019.
//
#pragma once
#include<iostream>

class Node {
public:
    Node() = default;
    Node(int a, int b);
    ~Node() = default;
    
    double Priority();
    void SetPriority(double p);
    bool InBounds();
    
    int x, y;//should be encapsulated
private:
    double _priority;
    //// Helpers for Node class
    //
    friend bool operator == (Node a, Node b) {
        return a.x == b.x && a.y == b.y;
    }
    
    friend bool operator != (Node a, Node b) {
        return !(a == b);
    }
    
    friend bool operator < (Node a, Node b) {
        return a._priority > b._priority;
    }
    friend bool operator > (Node a, Node b) {
        return a._priority > b._priority;
    }
    
    friend std::basic_iostream<char>::basic_ostream& operator<<(std::basic_iostream<char>::basic_ostream& out, const Node& loc) {
        out << '(' << loc.x << ',' << loc.y << ')';
        return out;
    }
    
};
