#ifndef __HPP__HOUSE__
#define __HPP__HOUSE__

#include <iostream>
#include <vector>
#include <map>
#include "npc.hpp"

class Location {
    private:
        std::string name;
        std::string description;
        bool visited;
        std::map<std::string, Location*> neighbors;
        std::vector<NPC> npcs;
        std::vector<std::string> items;
    public:
        Location(std::string locationName, std::string description)
        : name(locationName), description(description), visited(false){}

};


#endif