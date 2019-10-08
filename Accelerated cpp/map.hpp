//
//  map.hpp
//  Accelerated cpp
//
//  Created by Miguel Lopes on 07/10/2019.
//  Copyright © 2019 Miguel Lopes. All rights reserved.
//

#pragma once
#include <string>
#include <vector>
#include "Node.h"
//class GridLocation;

class Map {
public:
    Map() = delete;
    Map(const std::string& filepath);
    ~Map() = default;
    
    std::vector<std::vector<char>> GetMap(){return _map;}
    void PrintMap(const std::vector<Node>& path);
private:
    std::vector<std::vector<char>> _map;
};
