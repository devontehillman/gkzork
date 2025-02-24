#ifndef HPP_GAME_H  // Fixed include guard (non-reserved name)
#define HPP_GAME_H

#include <map>
#include <vector>
#include <functional>  // Added for std::function
#include <string>      // Added for std::string
#include "Item.hpp"
#include "location.hpp"

// Forward declaration of show_help
void show_help(const std::vector<std::string>&);

using Command = std::function<void(const std::vector<std::string>&)>;  // Proper command type

class Game {
private:
    std::map<std::string, Command> commands;  // Single commands map
    std::vector<Item> inventory;
    int weight;
    std::vector<Location> world;
    Location currentLocation;
    int numCalories = 1000;
    bool gameActive;

public:

    // std::map<std::string, Command> setup_commands(){
    //     commands["show help"] = show_help;

    //     return commands;
    // }

    // Location random_location(){

    // }

    // Location create_world(){
        
        
    //     return random_location();
    // }

    // void play(){

    // }

    //Game(){
        // commands = setup_commands();//set the command map equal to the return call from our setup_commands function.
        // currentLocation = create_world();
    //}
    // Member function declarations
    Game();

    std::map<std::string, Command> setup_commands();
    Location random_location();
    Location create_world();
    void play();
};


#endif