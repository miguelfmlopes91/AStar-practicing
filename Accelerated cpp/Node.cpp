//
//  Node.cpp
//  
//
//  Created by Miguel Lopes on 08/10/2019.
//
#include "Node.h"

constexpr int WITDH = 126;
constexpr int HEIGTH = 126;

Node::Node(int a, int b): x(a),y(b){
    //nothing to do here
}

double Node::Priority(){
    return _priority;
}

void Node::SetPriority(double p){
    _priority = p;
}

bool Node::InBounds() {
    return 0 <= x && x < WITDH
    && 0 <= y && y < HEIGTH;
}
