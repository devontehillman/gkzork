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
    int inventoryCapacity = 30;
    float weight;
    std::vector<Location> world;
    Location currentLocation;
    int numCalories = 500;
    bool gameActive;
    std::tm* localTime;
    
    public:
    
    Game(); //this is the constructor but we define it in game.cpp
    
    // Forward declaration of actions
    void show_help(const std::vector<std::string>& target);
    void talk(const std::vector<std::string>& target); 
    void meet(const std::vector<std::string>& target);
    void take(const std::vector<std::string>& target);
    void give(const std::vector<std::string>& target);
    void attack(const std::vector<std::string>& target);
    void consume(const std::vector<std::string>& target);
    void go(const std::vector<std::string>& target);
    void show_items(const std::vector<std::string>& target); 
    void look(const std::vector<std::string>& target);
    void quit(const std::vector<std::string>& target);

    std::map<std::string, Command> setup_commands();
    Location random_location();
    Location create_world();
    void play();
};


#endif