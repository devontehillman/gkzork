#ifndef HPP_GAME_H  // Fixed include guard (non-reserved name)
#define HPP_GAME_H

#include <map>
#include <vector>
#include <functional>  // Added for std::function
#include <string>      // Added for std::string
#include "Item.hpp"
#include "location.hpp"
#include "npc.hpp"


using Command = std::function<void(const std::vector<std::string>&)>;  // Proper command type

class Game {
    private:
    std::map<std::string, Command> commands;  // Single commands map
    std::vector<Item> inventory;
    int weight;
    std::vector<Location> world;
    Location currentLocation;
    int numCalories = 0;
    bool gameActive;
    
    public:
    
    Game(); //this is the constructor but we define it in game.cpp
    
    // Forward declaration of actions
    void show_help(const std::vector<std::string>& target);
    void meet(const std::vector<std::string>& target); //Hrc
    void take(const std::vector<std::string>& target); //xx
    void give(const std::vector<std::string>& target); //xx
    void go(const std::vector<std::string>& target); //Hrc
    void show_items(const std::vector<std::string>& target);//Hrc
    void look(const std::vector<std::string>& target); //xx
    void quit(const std::vector<std::string>& target); //xx


    std::map<std::string, Command> setup_commands();
    Location random_location();
    Location create_world();
    void play();
};


#endif