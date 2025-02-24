#include "game.hpp"
#include "Item.hpp"
#include "npc.hpp"
#include "location.hpp"
#include <iostream>
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()

// Implementation of the show_help function
void show_help(const std::vector<std::string>& args) {
    std::cout << "Showing help information...\n";
}

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

// Set up commands inside the game.
std::map<std::string, Command> Game::setup_commands() {
    commands["show help"] = show_help;
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

    }

    catch (const std::runtime_error& error) {
        std::cerr << "Warning Error! " << error.what() << std::endl;
    }

     // Select and return a random starting location.
     return random_location();
}

// Main game loop method.
void Game::play() {
    std::cout << "Game play is starting...\n";
    // Insert game loop logic here.
}