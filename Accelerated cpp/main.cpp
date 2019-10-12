#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <array>
#include <vector>
#include <utility>
#include <queue>
#include <algorithm>
#include <cstdlib>
#include <math.h>


#include "map.hpp"
#include "fibonacci.h"


constexpr int ORTHOGONAL_COST = 14;
constexpr int DIAGONAL_COST = 10;
//constexpr double epsilon = 0.001f;

typedef std::vector<std::vector<char>> Grid;
typedef std::pair<Node, double> pair;

inline double Tolerance(double val){
    return std::round(val * 1000) / 1000;
}

namespace std {
    /* implement hash function so we can put Node into an unordered_map */
    template <> struct hash<Node> {
        typedef Node argument_type;
        typedef std::size_t result_type;
        std::size_t operator()(const Node& id) const noexcept {
            return std::hash<int>()(id.x ^ (id.y << 4));
        }
    };
}

std::array<Node, 8> DIRS {
    Node(1, 0), Node(1, -1), Node(0, -1), Node(-1, -1),
    Node(-1, 0), Node(-1, 1), Node(0, 1), Node(1, 1),
};

inline std::vector<Node> GetNeighbours(Node id, const Grid& grid ){
    std::vector<Node> results;
    for (Node dir : DIRS) {
        Node next{id.x + dir.x, id.y + dir.y};
        if (next.InBounds() && ( grid[next.x][next.y] != '*')){ //check if it's not water
            results.push_back(next);
        }
    }
    return results;
}

inline double distance( Node source, Node target){
    double dx = std::abs(source.x - target.x);
    double dy = std::abs(source.y - target.y);

    return Tolerance(ORTHOGONAL_COST * (dx + dy) + (DIAGONAL_COST - 2 * ORTHOGONAL_COST) * std::min(dx, dy));
}

inline double heuristic(Node node, Node goal) {
    double dx = std::abs(node.x - goal.x);
    double dy = std::abs(node.y - goal.y);
    //return dx + dy; Manhattan - perfect for 4-way
    //return UNIFORM_COST * std::max(dx,y); Diagonal distance (8 way with uniform cost)
    
    //Diagonal Distance, with diagon cost different from ortogonal
    /*
     ORTHOGONAL_COST = 1
     DIAGONAL_COST = ORTHOGONAL_COST * 1.414
     d_max = std::max(dx, dy);
     d_min = std::min(dx, dy);
     return DIAGONAL_COST * d_min + ORTHOGONAL_COST *(d_max−d_min)
     */
    
    // h(n)2 =(n.x−goal.x)2​​ +(n.y−goal.y)2;    Euclidean distance
    
    return Tolerance(DIAGONAL_COST * (dx + dy) + (ORTHOGONAL_COST - 2 * DIAGONAL_COST) * std::min(dx, dy));
}

inline double GetCost(char val){
    double value = 0.f;
    switch(val) {
        case '.' ://plain
            value = 1.0f;
            break;
        case '*' ://water
            value = MAXFLOAT;
            break;
        case '-' ://swamps
            value = 1.5f;
            break;
        case '^' ://mountains
            value = 2.0f;
            break;
    }
    return value;
}

bool a_star_search(Grid graph,
                   Node start,
                   Node goal,
                   std::unordered_map<Node, Node>& closedList,
                   std::unordered_map<Node,
                   double>& costList){
    
    std::priority_queue<Node> openList;
    //FibonacciHeap<Node> openList;
    openList.emplace(start);
    //openList.insert(start);

    
    closedList[start] = start;
    costList[start] = 0;//could change this to actualy cost.
    
    while (!openList.empty()) {
        //Node current = openList.removeMinimum();
        Node current = openList.top();
        openList.pop();
        
        if (current == goal) {
            return true;
        }
        auto neighbours = GetNeighbours(current, graph);
        for (auto next : neighbours) {
            double new_cost = costList[current] + distance(current, next) + GetCost(graph[next.x][next.y]); //g

            if (costList.find(next) == costList.end()
                || new_cost < costList[next]) {
                costList[next] = new_cost;
                
                double priority = new_cost + heuristic(next, goal);//f = g + h
                next.SetPriority(priority);
                openList.emplace(next);
                //openList.insert(next);

                
                closedList[next] = current;
            }
        }
    }
    return false;
}

std::vector<Node> reconstructPath(Node start,
                                   Node goal,
                                   std::unordered_map<Node,
                                   Node> closedList) {
    std::vector<Node> path;
    Node current = goal;
    while (current != start) {
        path.push_back(current);
        current = closedList[current];
    }
    path.push_back(start); // optional
    std::reverse(path.begin(), path.end());
    return path;
}

int main(int argc, char* argv[]) {
    
//    char* MapName   = argv[1];
//    int startX      = std::atoi(argv[2]);
//    int startY      = std::atoi(argv[3]);
//    int endX        = std::atoi(argv[4]);
//    int endY        = std::atoi(argv[5]);
    
    auto map = std::make_unique<Map>("AStarMap.txt");

    Grid grid;
    bool wasFound;
    grid = map->GetMap();
    
    Node start(30, 10);
    Node goal(120, 10);

    std::unordered_map<Node, Node> closedList;
    std::unordered_map<Node, double> costList;
    
    auto startClock = std::chrono::high_resolution_clock::now();
    wasFound = a_star_search(grid, start, goal, closedList, costList);
    auto endClocl = std::chrono::high_resolution_clock::now();
    double totalCost = costList[goal];

    std::vector<Node> path = reconstructPath(start, goal, closedList);
    std::cout << "Start position:   ("<< start.x <<", "<<start.y<<")"<<std::endl;
    std::cout << "End position:     ("<< goal.x <<", "<<goal.y<<")"<<std::endl;

    std::cout << "Path found:       "<< std::boolalpha<< wasFound <<std::endl;
    std::cout << "Path cost:        "<< (wasFound?totalCost:0.0f) <<std::endl;


    std::cout << "Total duration:   "<<
    std::chrono::duration<double, std::milli>(endClocl-startClock).count() << "ms" << std::endl;

    if (wasFound) {
        map->PrintMap(path);
    }
}

