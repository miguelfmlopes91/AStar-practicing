/*
 Sample code from https://www.redblobgames.com/pathfinding/a-star/
 Copyright 2014 Red Blob Games <redblobgames@gmail.com>
 
 Feel free to use this code in your own projects, including commercial projects
 License: Apache v2.0 <http://www.apache.org/licenses/LICENSE-2.0.html>
 */

#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <array>
#include <vector>
#include <utility>
#include <queue>
#include <tuple>
#include <algorithm>
#include <cstdlib>

#include "map.hpp"


constexpr int ORTHOGONAL_COST = 14;
constexpr int DIAGONAL_COST = 10;

typedef std::vector<std::vector<char>> Grid;
typedef std::pair<Node, double> pair;


namespace std {//TODO CHECK IF NEEDED
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
    Node{1, 0}, Node{1, -1}, Node{0, -1}, Node{-1, -1},
    Node{-1, 0}, Node{-1, 1}, Node{0, 1}, Node{1, 1}
};

inline std::vector<Node> GetNeighbours(Node id, const Grid& grid ){
    std::vector<Node> results;
    for (Node dir : DIRS) {
        Node next{id.x + dir.x, id.y + dir.y};
        if (next.InBounds() && ( grid[next.x][next.y] != '*')){ //check if it's not water
            //std::cout<<"next:" <<next <<std::endl;
            results.push_back(next);
        }//TODO::do else and print for water to validare
    }
    return results;
}

inline double distance( Node source, Node target)
{
    int dx = std::abs(source.x - target.x);
    int dy = std::abs(source.y - target.y) / 2;
    
    return ORTHOGONAL_COST * (dx + dy) + (DIAGONAL_COST - 2 * ORTHOGONAL_COST) * std::min(dx, dy);
}

inline double heuristic(Node node, Node goal) {
    //return std::abs(a.x - b.x) + std::abs(a.y - b.y);
    double dx = std::abs(node.x - goal.x);
    double dy = std::abs(node.y - goal.y);
    return DIAGONAL_COST * (dx + dy) + (ORTHOGONAL_COST - 2 * DIAGONAL_COST) * std::min(dx, dy);
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

void a_star_search(Grid graph,
                   Node start,
                   Node goal,
                   std::unordered_map<Node, Node>& came_from,
                   std::unordered_map<Node,
                   double>& cost_so_far){
    
    std::priority_queue<Node> openList;

    openList.emplace(start);
    
    came_from[start] = start;
    cost_so_far[start] = 0;//could change this to actualy cost.
    
    while (!openList.empty()) {
        Node current = openList.top();
        openList.pop();
        std::cout << "current :"<< current << std::endl;
        
        if (current == goal) {
            break;
        }
        auto neighbours = GetNeighbours(current, graph);
        for (auto next : neighbours) {
            std::cout << "next :"<< next << std::endl;
            std::cout << "next cost :"<< GetCost(graph[next.x][next.y]) << std::endl;

            double new_cost = cost_so_far[current] + distance(current, next) + GetCost(graph[next.x][next.y]); //g
            std::cout << "new_cost:"<< new_cost << std::endl;

            if (cost_so_far.find(next) == cost_so_far.end()
                || new_cost < cost_so_far[next]) {
                cost_so_far[next] = new_cost;
                
                double priority = new_cost + heuristic(next, goal);//f = g + h
                std::cout << "priority :"<< priority << std::endl;
                next.SetPriority(priority);
                openList.emplace(next);

                came_from[next] = current;
            }
        }
    }
}

std::vector<Node> reconstruct_path(Node start,
                                           Node goal,
                                           std::unordered_map<Node,
                                           Node> came_from) {
    std::vector<Node> path;
    Node current = goal;
    while (current != start) {
        path.push_back(current);
        current = came_from[current];
    }
    path.push_back(start); // optional
    std::reverse(path.begin(), path.end());
    return path;
}

int main() {
    
    auto map = std::make_unique<Map>("AStarMap.txt");
    Grid grid;
    
    grid = map->GetMap();
    
    Node start{30, 10};
    Node goal{120, 10};
    
    //Start position: (20, 20)
    //End position:   (75, 89)
    //Start position: (30, 10)
    //End position:   (120, 10)
    std::unordered_map<Node, Node> came_from;
    std::unordered_map<Node, double> cost_so_far;
    
    
    auto startClock = std::chrono::high_resolution_clock::now();
    a_star_search(grid, start, goal, came_from, cost_so_far);
    auto endClocl = std::chrono::high_resolution_clock::now();

    std::cout << "real time to calculate the route (ms): "<<
    std::chrono::duration<double, std::milli>(endClocl-startClock).count() << std::endl;

    
    std::vector<Node> path = reconstruct_path(start, goal, came_from);
    std::cout << "START : "<< start<<std::endl;
    std::cout << "GOAL: : "<< goal <<std::endl;
//    for (auto p : path) {
//        std::cout << p <<std::endl;
//    }
    
    map->PrintMap(path);

}
