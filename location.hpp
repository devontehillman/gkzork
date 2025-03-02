/**
 * @file location.hpp
 * @brief This file contains the definition of the Location class, which represents a location in the game.
 * 
 * The Location class includes attributes such as the name, description, visited status, neighboring locations,
 * NPCs, and items present in the location. It provides methods to retrieve neighboring locations and add new locations.
 * 
 * @author Devonte H. & Horacio V.
 * @date Winter 2025
 */

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
    //default constructor
    Location() : name(""), description("") {}
    Location(std::string locationName, std::string description)
        : name(locationName), description(description), visited(false) {}

    //might not need the * here of in the instantiation of the variable XX
    std::map<std::string, Location*> get_locations(){
        return neighbors;
    }

    //this is named add_location in the instructions 
    void add_neighbor(std::string direction, Location* newLocation){
        // add new location into the map with the provided direction
        
        // //check if direction(key) is valid 1-5 or n,s,e,w + 'through' XX I could use this or get rid of it
        // if (neighbors.find(direction) != neighbors.end()) {
        //     std::cout << "There is a location to the north.\n";
        // } else {
        //     std::cout << "No location exists in that direction.\n";
        // }

        //check if their is already a location for that direction
        if(neighbors.find(direction) != neighbors.end()) {
            std::cerr << "Error: '" << direction << "' There is already a location for that location.\n";
            return;
        }

        //check if the the location(value) is already in the map 
        for(const auto& pair : neighbors) {
            //iterate through the map and make sure the location is not already in the map
            if(pair.second == newLocation) {
                std::cerr << "Error: this location has already been assigned to another direction.\n";
                return;
            }
        }
        // add to neighbor map
        neighbors[direction] = newLocation;


    }

    //adding an npc into the vector of the npc
    void add_npc(NPC npc){
        npcs.push_back(npc);
    }

    //return the vector of npcs for this location
    std::vector<NPC> get_npcs(){
        return npcs;
    }

    //XX adding an item to the 'Locations NPC vector'???? 
    void add_item(Item item){
        items.push_back(item);
    }

    //return all item for this location
    std::vector<Item> get_items(){
        return items;
    }

    std::string get_description(){
        return description;
    }

    //identify the current location has been visited
    void set_visited(){
        visited = true; 
    }

    bool get_visited(){
        return visited;
    }

    std::string get_name() const {
        return name;
    }

    //remove an item from the location
    void remove_item(Item& item) {
        auto it = std::find(items.begin(), items.end(), item);
        if (it != items.end()) {
            items.erase(it);
        } else {
            std::cerr << "Error: Item not found in this location.\n";
        }
    }

    std::map<std::string, Location*> get_neighbors() const {
        return neighbors;
    }

    //overloading the stream operator
    friend std::ostream& operator<<(std::ostream& os, const Location& location){
        //format 'Padnos Hall - Lots of science labs are in this building.'
        os << location.name << "-" << location.description << "\n\n";
        
        os << "You see the following people:\n";
        for (const auto& npc : location.npcs){
            os << "- " << npc.getName() << "\n";
        }
        
        os << "\nYou see the following Items:\n";
        for (const auto& item : location.items){
            os << "- " << item << "\n";
        }

        os << "\n You can go in the following directions:\n";
        for(const auto& pair : location.neighbors){
            os << "[" << pair.first << "] -> " << pair.second->name << "\n";
        }

        return os;
    }

};


#endif