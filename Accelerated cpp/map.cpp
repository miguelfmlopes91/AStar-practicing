//
//  map.cpp
//  Accelerated cpp
//
//  Created by Miguel Lopes on 07/10/2019.
//  Copyright © 2019 Miguel Lopes. All rights reserved.
//

#include "map.hpp"
#include <fstream>
#include <iostream>

#define MAPSIZE 126

inline void remove_carriage_return(std::string& line){
    if (*line.rbegin() == '\r')
        line.erase(line.length() - 1);
}


Map::Map(const std::string& filepath){
    std::ifstream file(filepath);

    _map.reserve(MAPSIZE);
    std::string line;
    
    if (file.is_open()) {
        while (std::getline(file, line)) {
            remove_carriage_return(line);//we ignore the carriage return
            std::vector<char> temp (line.begin(),line.end());
            _map.emplace_back(line.begin(),line.end());
        }
        file.close();//TODO::if not possible to read file,give warning, end program. Wrap this with a try-catch
    }
}

void Map::PrintMap(const std::vector<Node>& path){
    for (const auto& g : path) {
        _map[g.x][g.y] = '+';
    }
    for (int i = 0; i < MAPSIZE; i++){
        for (int j = 0; j < MAPSIZE; j++){
            std::cout << _map[i][j];
        }
        std::cout<<std::endl;
    }
}
