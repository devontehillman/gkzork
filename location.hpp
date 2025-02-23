#ifndef __HPP__LOCATION__
#define __HPP__LOCATION__

#include <iostream>
#include <vector>
#include <map>
#include "npc.hpp"
#include "Item.hpp"

class Location {
    private:
        std::string name;
        std::string description;
        bool visited;
        std::map<std::string, Location*> neighbors;
        std::vector<NPC> npcs;
        std::vector<Item> items;
    public:
        Location(std::string locationName, std::string description)
        : name(locationName), description(description), visited(false) {}

};


#endif