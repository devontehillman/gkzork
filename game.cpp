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
#include <map> // For std::find, but we many not need to include map to use std::find


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

    for (auto curNPC : getNpcs) { // loop through each NPC that's in the room
        auto curNPCname = std::find(target.begin(), target.end(), curNPC.getName());
        // found the std::find function from AI by the way 
        // try to find the NPC's name in the vector of strings we passed in the parameter

        if (curNPCname != target.end()) { // if this NPC name we tried to find doesn't reach 
        // the end of the iteration for the vector, this means the name is definitely in vector
            std::cout << curNPC.getDescription() << std::endl;
            return;
            // Now print the description the NPC gave us when we meet him/her
        }
    }

    std::cout << "NPC not found" << std::endl;
    return;
    }



void Game::go(std::vector<std::string>& target) {

    if (currentLocation.get_locations().empty()) {
        std::cout << "There aren't any neighbors for current location" << std::endl;
    }

    currentLocation.set_visited(); // sets visited status to true for current location

    if (weight > 30) { // checks to see if player's weight is over 30 pounds.
        std::cerr << "the player's weight is over " << weight << " lbs, can't move!";
        return; // not sure if I'm supposed to return nothing or someething else. Can't be the
        // weight because this function can't return an int type.
    }
    else {

        for (auto& keyvalue_pair : currentLocation.get_locations()) { // iterate through the
            // map/dictionary with the elements being the key and value paired up, for example:
            // key for first iteration could be a string - "North", value associated to that key
            // is the Location object.
            auto curKey = std::find(target.begin(), target.end(), keyvalue_pair.first);
            // trying to find the key element this loop is currently looking at, in the vector of 
            // strings we passed in the parameter

            if (curKey != target.end()) { // if this key we tried to find doesn't reach 
        // the end of the iteration for the vector, this means the key in map is definitely in vector
                currentLocation = keyvalue_pair.second; 
                return;
                // the player's current location needs to be
                // set to the value that matches the current key from the map, but problem is that the
                // "currentLocation" is a Location type and value is a Location* type right now, which
                // isn't matching types. 
                
                // FIXED ISSUES in location.hpp by removing pointers for
                // "neighbors" map and changed if statement to "if(pair.second.name == newLocation.name)"
                // in add neighbor function.
            }


        }

        std::cout << "Can't go to the direction you provided" << std::endl;
        return;
    }

    // Game::inventory()



}

void Game::show_items(std::vector<std::string>& target) {

    if (inventory.empty()) {
        std::cout << "You have no items in your inventory" << std::endl;
    }

    std::cout << "Current total weight we're carrying is " << weight <<
    ". The items being carried are ---> ";
    
    for (auto item : inventory) { // iterate through each item player has in inventory
        std::cout << item << "." << std::endl; // then print current item's info by
        // calling the class itself, since it returns an ostream created in Item.hpp
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
    
        Item booze("Booze", "Booze: (A good way to drink your pain away!", 215, 1.55); // 1
        Item Beef("Beef", "Beef: (This will save your hunger!", 180, 7.75); // 2
        Item Knife("Knife", "Knife: (Stab a scary NPC in the gut!", 0, 2.12); // 3
        Item Mango("Mango", "Mango: (It is bright yellow, so it is super sweet!", 125, 1.10); // 4
        Item Peanuts("Peanuts", "Peanuts: (Eat a few to fill empty tummy!", 196, 3.22); // 5
        Item Hi_C("Hi-C", "Hi-C: (You can drink this to quench thirst!", 171, 1.30); // 6
        Item Chainsaw("Chainsaw", "Chainsaw: (Slice your enemies!!!!", 0, 13.85); // 7
        Item Broccoli("Broccoli", "Broccoli: (Healthy and low-calorie intake", 40, 0.98); // 8
        Item Cauliflower("Cauliflower", "Cauliflower: (Tastier alternative to Broccoli!", 45, 1.06); // 9
        Item Cilantro("Cilantro", "Cilantro: (Swallow by itself or put on top of meat!", 28, 0.32); // 10

        // // Creating 5 NPCs with multiple messages
        NPC Guard("Guard", "A stern-looking watchman.", 
        {"Halt! Who goes there?", "Stay out of trouble.", "Move along."});

        NPC Elf("Elf", "she lurking in the woods.", 
        {"Give me fooodd?", "You are silly!", "I do not need you no more"});

        NPC BigFoot("BigFoot", "He is scary looking.", 
        {"I will stomp you", "hand me cookie!", "I am your worst nightmare!"});

        NPC Clown("Clown", "He looks like a Maniac!", 
        {"Want some candy?", "Dance salsa with me", "See you tomorrow"});

        NPC WonderWoman("WonderWoman", "Very pretty hero chick.", 
        {"I am a villian now", "Leave before I hit you"});
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
        Location loc3("Blue Connection", "So many people here inside the POD store.");
        Location loc4("Kirkhof", "An abundance of kids visiting for the first time.");
        Location loc5("Bathroom", "It stinks so bad in the men's bathroom.");
        Location loc6("Mackinac", "Everyone's walking fast to their classes.");
        Location loc7("AuSable", "It's dead in this hall, barely anybody.");
        Location loc8("Gym", "Loud and energetic.");

        // Example: add locations into the world.
        world.push_back(loc1); // Library
        world.push_back(loc2); // Cafeteria
        world.push_back(loc3); // Blue Connection
        world.push_back(loc4); // Kirkhof
        world.push_back(loc5); // Bathroom
        world.push_back(loc6); // Mackinac
        world.push_back(loc7); // AuSable
        world.push_back(loc8); // Gym

        // Set neighbor relationships if desired, e.g.:
        // world[0].add_neighbor("east", &world[1]);  // etc.

        world[0].add_neighbor("east", loc4); // world[0] == library, east from library 
        // will be Kirkhof (loc4) check!
        world[0].add_neighbor("west", loc5); // world[0] == library, west from library 
        // will be Bathroom (loc5) check!
        world[0].add_neighbor("north", loc3); // world[0] == library, north from library 
        // will be Blue Connection (loc3) check!
        world[0].add_neighbor("south", loc1); // world[0] == library, south from library 
        // will be Cafeteria (loc2) check!

        world[1].add_neighbor("east", loc4); // world[0] == Cafeteria, east from Cafeteria 
        // will be gym (loc4) check!
        world[1].add_neighbor("west", loc5); // world[0] == Cafeteria, west from Cafeteria 
        // will be kirkhof (loc5) check!
        world[1].add_neighbor("north", loc3); // world[0] == Cafeteria, north from Cafeteria 
        // will be Library (loc3) check!
        world[1].add_neighbor("south", loc1); // world[0] == Cafeteria, south from Cafeteria 
        // will be Mackinac (loc2) check!

        world[2].add_neighbor("east", loc4); // world[0] == Blue Connection, east from Blue Connection 
        // will be Mackinac (loc4) check!
        world[2].add_neighbor("west", loc5); // world[0] == Blue Connection, west from Blue Connection 
        // will be Gym (loc5) check!
        world[2].add_neighbor("north", loc3); // world[0] == Blue Connection, north from Blue Connection 
        // will be Kirkhof (loc3) check!
        world[2].add_neighbor("south", loc1); // world[0] == Blue Connection, south from Blue Connection 
        // will be Library (loc2) check!

        world[3].add_neighbor("east", loc4); // world[0] == Kirkhof, east from Kirkhof 
        // will be Cafeteria (loc4) check!
        world[3].add_neighbor("west", loc5); // world[0] == Kirkhof, west from Kirkhof 
        // will be Library (loc5) check!
        world[3].add_neighbor("north", loc3); // world[0] == Kirkhof, north from Kirkhof 
        // will be AuSable (loc3) check!
        world[3].add_neighbor("south", loc1); // world[0] == Kirkhof, south from Kirkhof 
        // will be Blue Connection (loc2) check!

        world[4].add_neighbor("east", loc4); // world[0] == Bathroom, east from Bathroom 
        // will be Library (loc4) check!
        world[4].add_neighbor("west", loc5); // world[0] == Bathroom, west from Bathroom 
        // will be Mackinac (loc5) check! 
        world[4].add_neighbor("north", loc3); // world[0] == Bathroom, north from Bathroom 
        // will be Gym (loc3) check!
        world[4].add_neighbor("south", loc1); // world[0] == Bathroom, south from Bathroom 
        // will be AuSable (loc2) check!

        world[5].add_neighbor("east", loc4); // world[0] == Mackinac, east from Mackinac 
        // will be Bathroom (loc4) check!
        world[5].add_neighbor("west", loc5); // world[0] == Mackinac, west from Mackinac 
        // will be Blue Connection (loc5) check!
        world[5].add_neighbor("north", loc3); // world[0] == Mackinac, north from Mackinac 
        // will be Cafeteria (loc3) check!
        world[5].add_neighbor("south", loc1); // world[0] == Mackinac, south from Mackinac 
        // will be Gym (loc2) check!

        world[6].add_neighbor("east", loc4); // world[0] == AuSable, east from AuSable 
        // will be Blue Connection (loc4) check!
        world[6].add_neighbor("west", loc5); // world[0] == AuSable, west from AuSable 
        // will be Cafeteria (loc5) check! 
        world[6].add_neighbor("north", loc3); // world[0] == AuSable, north from AuSable 
        // will be Bathroom (loc3) check!
        world[6].add_neighbor("south", loc1); // world[0] == AuSable, south from AuSable 
        // will be Kirkhof (loc2) check!

        world[7].add_neighbor("east", loc4); // world[0] == Gym, east from Gym 
        // will be Blue Connection (loc4) check!
        world[7].add_neighbor("west", loc5); // world[0] == Gym, west from Gym 
        // will be Cafeteria (loc5) check!
        world[7].add_neighbor("north", loc3); // world[0] == Gym, north from Gym 
        // will be Mackinac (loc3) check!
        world[7].add_neighbor("south", loc1); // world[0] == Gym, south from Gym 
        // will be Bathroom (loc2) check!



        world[1].add_npc(BigFoot); // Cafeteria
        world[4].add_npc(WonderWoman); // Bathroom
        world[3].add_npc(Elf); // Kirkhof
        world[5].add_npc(Guard); // Mackinac
        world[7].add_npc(Clown); // Gym

        world[0].add_item(Knife); // library
        world[0].add_item(Cauliflower); // library
        world[1].add_item(Mango); // Cafeteria
        world[2].add_item(Cilantro); // Blue Connection
        world[2].add_item(Peanuts); // Blue Connection
        world[3].add_item(Chainsaw); // Kirkhof
        world[4].add_item(Broccoli); // Bathroom
        world[5].add_item(Hi_C); // Mackinac
        world[5].add_item(Beef); // Mackinac
        world[6].add_item(booze); // AuSable

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