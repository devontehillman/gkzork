#include "game.hpp"
#include "Item.hpp"
#include "npc.hpp"
#include "location.hpp"
#include <iostream>
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
#include <functional>  // For std::bind
#include <chrono>
#include <iomanip>
// #include <map> // For std::find, but we many not need to include map to use std::find


// Constructor Implementation
Game::Game() {
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(nullptr)));
    
    // Initialize commands map
    commands = setup_commands();
    
    // Create the world
    // (Assuming create_world() populates the world vector appropriately)
    currentLocation = create_world();
    
    // Other default variable initializations:
    weight = 0;
    gameActive = true;
}

// Implementation of the show_help function
void Game::show_help(const std::vector<std::string>& args) {
    std::cout << "Showing help information...\n";
}

// Implementation of talk function
// void Game::talk(NPC& target) {
    
    // if (std::find(numbers.begin(), numbers.end(), target) != numbers.end()) {
    // if (std::find(currentLocation.get_npcs().begin(), 
    // currentLocation.get_npcs().end(), target) != currentLocation.get_npcs().end()) {

    //     std::cout << target.getNextMessage() << std::endl;
    //     // target.getNextMessage();
    // }

// }

void Game::meet(std::vector<std::string>& target) {

    std::vector<NPC> getNpcs = currentLocation.get_npcs(); // store current room's NPCs

    for (auto it : getNpcs) { // loop through each NPC that's in the room
        auto curNPCname = std::find(target.begin(), target.end(), it.getName()); 
        // try to find the NPC's name in the vector of strings we passed in the parameter

        if (curNPCname != target.end()) { // if this NPC name we tried to find doesn't reach 
        // the end of the iteration for the vector, this means the name is definitely in vector
            std::cout << it.getDescription() << std::endl;
            // Now print the description the NPC gave us when we meet him/her
        }
    }

}

// Set up commands inside the game.
std::map<std::string, Command> Game::setup_commands() {
    std::map<std::string, Command> cmds;
    cmds["show help"] = std::bind(&Game::show_help, this, std::placeholders::_1);
    // cmds["show help"] = [this](const std::vector<std::string> arg) { show_help(arg); };
    // Add other command mappings as needed.
    return commands;
}

// Choose and return a random Location.
// (A simple placeholder implementation is provided.)
Location Game::random_location() {
    // For example, if world is populated, pick a random index.

    if (!world.empty()) {
        size_t index = static_cast<size_t>(rand() % world.size());
        return world[index];
    }
    // Return a default location if world vector is empty.
    return Location("Default Location", "A nondescript location.");
}

// Create the game world and return the starting location.
Location Game::create_world() {
    try {
    
        Item booze("Booze", "Booze: (A good way to drink your pain away!", 215, 11.75);

        // // Creating an NPC with multiple messages
        NPC npc1("Guard", "A stern-looking watchman.", {"Halt! Who goes there?", "Stay out of trouble.", "Move along."});

        // // // Display NPC name and description using overloaded <<
        // std::cout << npc1 << std::endl;

        // // // Getting messages one by one
        // std::cout << "NPC says: " << npc1.getNextMessage() << std::endl;
        // std::cout << "NPC says: " << npc1.getNextMessage() << std::endl;
        // std::cout << "NPC says: " << npc1.getNextMessage() << std::endl;
        // std::cout << "NPC says: " << npc1.getNextMessage() << std::endl; // Loops back to first message

        // // // Using overloaded << operator
        // std::cout << "NPC Name: " << npc1 << std::endl;
        // std::cout << "Item here: " << booze << std::endl;

        
        // Populate the world vector with several Locations.
        Location loc1("Library", "A quiet place full of books.");
        Location loc2("Cafeteria", "Bustling with activity and the smell of food.");
        Location loc3("Gym", "Loud and energetic.");

        // Example: add locations into the world.
        world.push_back(loc1);
        world.push_back(loc2);
        world.push_back(loc3);

        // Set neighbor relationships if desired, e.g.:
        // world[0].add_neighbor("east", &world[1]);  // etc.

        world[0].add_npc(npc1);

    }

    catch (const std::runtime_error& error) {
        std::cerr << "Warning Error! " << error.what() << std::endl;
    }

     // Select and return a random starting location.
     return random_location();
}

// Main game loop method.
void Game::play() {
    std::cout << "Game play is starting..." << std::endl;
    // Here you might run your input loop or game state updates

    // Demonstration: show help information by calling the command directly.
    std::vector<std::string> args;  // No arguments needed in this example.
    show_help(args);
}