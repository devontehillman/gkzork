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
#include <map> // For std::find
#include <sstream> // For std::istringstream


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

/*
If the std::vector target item is in the room it will remove it from
the room’s inventory, add it to the user’s inventory, and add the weight of
the item to the user’s carried weight.
*/
void Game::take(const std::vector<std::string>& target) {
    if (target.empty()) {
        std::cout << "Take what?\n";
        return;
    }

    std::string userInput = target[0]; //string rep of the item we want
    std::vector<Item> items = currentLocation.get_items(); // list of items at the current location

    for (auto it = items.begin(); it != items.end(); ++it) {
        if (it->get_name() == userInput) {
            // Item found in the room
            Item item = *it;
            currentLocation.remove_item(item); // Remove item from room's inventory
            inventory.push_back(item); // Add item to user's inventory
            weight += item.get_weight(); // Add item's weight to user's carried weight
            std::cout << "You have taken the " << item.get_name() << ".\n";
            return;
        }
    }
    std::cout << "There is no " << userInput << " here.\n";
    // //iterates through the items list if the item is inside the list it == the last item in the list. 
    // auto it = std::find(items.begin(), items.end(), [userInput](const Item& item) {
    //     return item.get_name() == userInput; 
    // });

    // if (it != items.end()) {
    //     // Item found in the room
    //     Item item = *it;
    //     currentLocation.remove_item(item); // Remove item from room's inventory
    //     inventory.push_back(item); // Add item to user's inventory
    //     weight += item.get_weight(); // Add item's weight to user's carried weight
    //     std::cout << "You have taken the " << item.get_name() << ".\n";
    // } else {
    //     std::cout << "There is no " << userInput << " here.\n"; //xx might not need because the remove item prints item not available if it dne
    // }
}

/*
Removes the std::vector target item (if it exists) from the user’s inventory, 
adds it to the current location’s inventory and decreases the user’s carried weight.
The function will then check if the current location is the woods. 
If it is, it will check if the item given was edible. If the item is edible, reduce
the number of calories the item has from the total the elf needs. 
If the item was not edible, transport the player to a new location by setting the
current location equal to the return from random_location
*/
void Game::give(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Give what?\n";
        return;
    }

    // checking is the user has the item in their inventory
    std::string userInput = args[0]; // storing the user input in a var
    auto it = std::find_if(inventory.begin(), inventory.end(), [&userInput](const Item& item) {
        return item.get_name() == userInput;
    });

    if (it != inventory.end()) {
        // Item found in the user's inventory
        Item item = *it;
        inventory.erase(it); // Remove item from user's inventory
        currentLocation.add_item(item); // Add item to current location's inventory
        weight -= item.get_weight(); // Decrease user's carried weight
        std::cout << "You have given the " << item.get_name() << ".\n";

        // Check if the current location is the woods
        if (currentLocation.get_name() == "Woods") {
            if (item.get_calories() != 0) {
                // Reduce the number of calories the elf needs
                numCalories -= item.get_calories();
                std::cout << "The elf consumes the " << item.get_name() << ".\n";
            } else {
                // Transport the player to a new location
                currentLocation = random_location();
                std::cout << "The item was not edible. You have been transported to a new location.\n";
            }
        }
    } else {
        std::cout << "You don't have a " << userInput << " to give.\n";
    }
}

/*
look(std::vector<std::string> target) method.
This method will print the current location, a vector of Items
in the location or a message indicating there are none, a vector of NPCs in
the room or the message “You are alone.”, and a vector of directions in
which the player can go. If a location has been visited previously, print
the direction and the location. Otherwise, simply print the direction.
*/
void Game::look(const std::vector<std::string>& args) {
    std::cout << "You are at: " << currentLocation.get_name() << "\n";
    std::cout << currentLocation.get_description() << "\n";

    // Print items in the location
    std::vector<Item> items = currentLocation.get_items();
    if (!items.empty()) {
        std::cout << "Items here:\n";
        for (const auto& item : items) {
            std::cout << "- " << item.get_name() << "\n";
        }
    } else {
        std::cout << "There are no items here.\n";
    }

    // Print NPCs in the location
    std::vector<NPC> npcs = currentLocation.get_npcs();
    if (!npcs.empty()) {
        std::cout << "NPCs here:\n";
        for (const auto& npc : npcs) {
            std::cout << "- " << npc.getName() << ": " << npc.getDescription() << "\n";
        }
    } else {
        std::cout << "You are alone.\n";
    }

    // Print directions and neighboring locations
    std::map<std::string, Location*> neighbors = currentLocation.get_neighbors();
    if (!neighbors.empty()) {
        std::cout << "You can go:\n";
        for (const auto& neighbor : neighbors) {
            const std::string& direction = neighbor.first;
            Location* location = neighbor.second;
            if (location->get_visited()) {
                std::cout << "- " << direction << " to " << location->get_name() << "\n";
            } else {
                std::cout << "- " << direction << "\n";
            }
        }
    } else {
        std::cout << "There are no exits here. Game is broken\n";
    }
}

void Game::quit(const std::vector<std::string>& args) {
    std::cout << "Are you sure you want to quit? (yes/no): ";
    std::string response;
    std::cin >> response;
    if (response == "yes" || response == "y") {
        gameActive = false;
        std::cout << "Thank you for playing. Goodbye!\n";
    } else {
        std::cout << "Continuing the game...\n";
    }
}

// Set up commands inside the game.
std::map<std::string, Command> Game::setup_commands() {
    std::map<std::string, Command> cmds;
    cmds["show help"] = std::bind(&Game::show_help, this, std::placeholders::_1);
    //cmds["meet"]      = std::bind(&Game::meet, this, std::placeholders::_1);
    cmds["take"]      = std::bind(&Game::take, this, std::placeholders::_1); //xx 
    cmds["give"]      = std::bind(&Game::give, this, std::placeholders::_1); //xx
    //cmds["go"]        = std::bind(&Game::go, this, std::placeholders::_1);
    //cmds["show items"]= std::bind(&Game::show_items, this, std::placeholders::_1);
    cmds["look"]      = std::bind(&Game::look, this, std::placeholders::_1); //xx
    cmds["quit"]      = std::bind(&Game::quit, this, std::placeholders::_1); //xx
    return cmds;
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

/*
• a void play() method. This is the core game loop. It should first print a
message describing the game, then call the method to vector all commands.
Then, while the game is still in progress, it will loop. 

In the loop, we will prompt the user for a command. The user may enter multiple words
in a prompt. We will split the user’s input into a vector of words. We
can split on spaces with code tokens = user_response.split(). Once
we have the tokens vector, create a variable called command and set it
equal to the first element in the vector. Then, remove the first element
with the del(tokens[0]) command. Then, use the code target = '
'.join(tokens) code to put the remaining tokens together. Thus, if the
user enters 'talk ball of light" then command will be equal to talk, and target will be ball of light split
into a std::vector on the spaces. We will then call the function from the commands map by using this key. Pass
target as a parameter to the called function. If the command does not exist in
the map print a message to the user telling them so.
Once the loop ends (i.e. the in-progress variable is false), check if the elf got
enough calories. If it did, print a success message and quit. Otherwise, print a
failure message and quit.

*/
void Game::play() {
    // std::cout << "Game play is starting..." << std::endl;
    // // Here you might run your input loop or game state updates

    // // Demonstration: show help information by calling the command directly.
    // std::vector<std::string> args;  // No arguments needed in this example.
    // show_help(args);

    std::cout << "Welcome to the game! Your adventure begins now...\n";
    std::cout << "Type 'show help' to see available commands.\n";

    while (gameActive) {
        std::cout << "> ";
        std::string userInput;
        std::getline(std::cin, userInput);

        // Split user input into tokens
        std::istringstream iss(userInput);
        std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

        if (tokens.empty()) {
            continue;
        }

        std::string command = tokens[0];

        if (tokens.size() > 1 && tokens[1] == "help") {
            command = tokens[0] + " " + tokens[1];
            tokens.erase(tokens.begin());
        }
        // check of token == to two word commands XX
        tokens.erase(tokens.begin());


        if (commands.find(command) != commands.end()) {
            commands[command](tokens);
        } else {
            std::cout << "Unknown command: " << command << "\n";
        }
    }

    if (numCalories <= 0) {
        std::cout << "Congratulations! The elf has received enough calories.\n";
    } else {
        std::cout << "Game over. The elf did not receive enough calories.\n";
    }
}