#ifndef __HPP__HOUSE__
#define __HPP__HOUSE__
#include "npc.hpp"

#include <iostream>
#include <map>

class Location {
    private:
        std::string name;
        std::string description;
        std::bool visited;
        std::map<std::string, Location*> neighbors;
        std::vector<NPC> npcs;
        std::vector<std::string> items;
    public:
        Location(std::string locationName, std::description)
        : name(locationName), description(description), visited(false){}

};


#endif