#include "game.hpp"
#include "Item.hpp"
#include "npc.hpp"
#include "location.hpp"
#include <iostream>
#include <cstdlib>  // For rand()
#include <functional>  // For std::bind
#include <chrono>
#include <iomanip>
#include <map> // For std::find
#include <sstream> // For std::istringstream
#include <ctime> // For displaying time in hr, min, & sec

Game::Game() {
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(nullptr)));

    // std::time_t getCurTime = std::time(nullptr);

    // // Convert that current time to a local time
    // localTime = std::localtime(&getCurTime);

    // Initialize commands map, instance of the game, loop stop condition,
    commands = setup_commands();
    currentLocation = create_world();
    weight = 0;
    gameActive = true;
}

// Prints a manuel like page of available commands for the user
void Game::show_help(const std::vector<std::string>& args) {

    // this time gets seconds since 1970-01-01 00:00:00 UTC.
    // For example: 1608436190 is a possible time to get since 1970
    std::time_t getCurTime = std::time(nullptr);

    // Since 1608436190 is a pointless way of looking at time,
    // Lets convert it to the regular 12 or 24 hour time, most
    // people are used to
    localTime = std::localtime(&getCurTime);

    std::cout << "What time is it? " <<
    std::put_time(localTime, "%I:%M:%S %p") << std::endl;
    std::cout << "Available commands:\n";
    std::cout << "  show help, display help, present help - Display this help message\n";
    std::cout << "  meet <npc>, encounter <npc>, confront <npc> - Meet an NPC in the current location\n";
    std::cout << "  take <item>, get <item>, grab <item> - Take an item from the current location\n";
    std::cout << "  give <item>, feed <item>, lend <item> - Give an item to the current location or NPC\n";
    std::cout << "  go <direction>, walk <direction>, run <direction> - Move to a neighboring location\n";
    std::cout << "  show items, display items, or present items - Show items in your inventory\n";
    std::cout << "  look, see, investigate - Look around the current location\n";
    std::cout << "  Attack <NPC>- slapp that hoe if you ain't got nothing in your inventory. ";
    std::cout << "  Otherwise, use that weapon you may have\n"; //xx Edit
    std::cout << "  Consume <item> - drink item \n"; //xx Edit
    std::cout << "  quit, terminate, retire - Quit the game\n";
}


//Iterates through the NPC's list of messages
void Game::talk(const std::vector<std::string>& target) {
    std::vector<NPC>& getNpcs = currentLocation.get_npcs(); 
    //find the target npc 
    for (auto& curNPC : getNpcs) {
        if (curNPC.getName() == target[0]) { 
        std::cout << curNPC.getNextMessage() << std::endl;
        return;
        }
    }
    std::cout << target[0] << " does not exist at this location\n" << std::endl;
    return;
}

void Game::meet(const std::vector<std::string>& target) {

    std::vector<NPC>& getNpcs = currentLocation.get_npcs(); // store current room's NPCs

    std::string userInput = target[0]; //string rep of the NPC we're meeting

    for (auto curNPC : getNpcs) { // loop through each NPC that's in the room

        if (curNPC.getName() == userInput) { // if current NPC is the same as the name user inputs
            std::cout << curNPC.getDescription() << std::endl;
            // Now print the description the NPC gives us when we meet him/her
            return; 
        }
    }
    std::cout << "NPC not found" << std::endl;
    return;
}



void Game::go(const std::vector<std::string>& target) {

    if (currentLocation.get_neighbors().empty()) {
        std::cout << "There aren't any neighbors for current location" << std::endl;
    }

    currentLocation.set_visited(); // sets visited status to true for current location

    if (weight > inventoryCapacity) { // checks to see if player's weight is over 30 pounds.
        std::cerr << "the player's weight is over " << weight << " lbs, can't move!";
        return; 
    }

    else {
        std::string userInput = target[0]; //string rep of the Location we're going to

        for (auto& keyvalue_pair : currentLocation.get_neighbors()) { // iterate through the
            // map/dictionary with the elements being the key and value paired up, for example:
            // key for first iteration could be a string - "North", value associated to that key
            // is the Location object we want to go to.

            if (keyvalue_pair.first == userInput) { // If the key direction from the game's current
                // location is the same as the direction the user inputs

                for (size_t i = 0; i < world.size(); ++i) { // looping thru each Location in game's world
                    if (world[i].get_name() == keyvalue_pair.second.get_name()) {
                        // If current game instance Location name is the same as the current locations 
                        // neighbor map name.

                        currentLocation = world[i];
                        // set current location to the instance in the game instance 
                        // (we had issue with current locations neighbor map)
                        return;
                    }
                }
            }
        }
        std::cout << "Can't go to the direction you provided" << std::endl;
        return;
    }
}

/*
Takes an item from a location and places it to user’s inventory, and add the weight of
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
    return;
}

void Game::show_items(const std::vector<std::string>& target) {

    if (inventory.empty()) {
        std::cout << "You have no items in your inventory" << std::endl;
        return;
    }

    std::cout << "Current total weight we're carrying is " << weight <<
    "lb. The items being carried are --->" << std::endl;
    
    for (auto item : inventory) { // iterate through each item player has in inventory
        std::cout << item << "." << std::endl; // then print current item's info by
        // calling the class itself, since it returns an ostream created in Item.hpp
    }
}


void Game::attack(const std::vector<std::string>& target) {

    std::vector<NPC>& getNpcs = currentLocation.get_npcs(); // store current room's NPCs

    std::string userInput = target[0]; //string rep of the NPC we want to attack

    auto it = std::find_if(getNpcs.begin(), getNpcs.end(), [&userInput](const NPC& npc) {
        return npc.getName() == userInput;
    });

    if (it != getNpcs.end()) {
        getNpcs.erase(it);
        std::cout << "You kilt " << userInput << ". You're a menace to society!" << std::endl;
        return;
    }

    // for (auto curNPC : getNpcs) { // loop through each NPC that's in the room

    //     if (curNPC.getName() == userInput) { // if current NPC is the same as the name user inputs

    //         NPC copycurNPC = curNPC;
    //         getNpcs.erase(copycurNPC);


    //         std::cout << curNPC.getDescription() << std::endl;
    //         // Now print the description the NPC gives us when we meet him/her
    //         return; 
    //     }
    // }
    std::cout << "NPC not found, you can't kill an NPC that don't exist" << std::endl;
    return;

}

// void Game::consume(const std::vector<std::string>& target) {

// }

/*
Removes the target item from the user's inventory, adds it to the current location's inventory,
and decreases carried weight. If the location is the woods and the item is edible, reduce the elf's 
required calories. If not edible, move the player to a random location.
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
                std::cout << "The elfs needs: " << numCalories << ".\n";
            if (numCalories <= 0) {
                gameActive = false;
            }
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
Displays the current location, available items (or a message if none), 
NPCs (or "You are alone." if none), and possible directions. Previously visited locations 
are shown with their names; otherwise, only the direction is displayed.
*/
void Game::look(const std::vector<std::string>& target) {
    std::cout << "You are at: " << currentLocation.get_name() << "\n";

    //accessing neighbors for iteration
    std::map<std::string, Location> neighbors = currentLocation.get_neighbors();
    
    
    
    if (!currentLocation.get_neighbors().empty()) {
        std::cout << currentLocation << std::endl;
        //std::cout << "You can go:\n";
        // for (const auto& neighbor : neighbors) {
        //     const std::string& direction = neighbor.first;
        //     Location location = neighbor.second;
        //     if (location.get_visited()) {
        //         std::cout << "- " << direction << " to " << location.get_name() << "\n";
        //     } else {
        //         std::cout << "- " << direction << "\n";
        //     }
        // }
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

void Game::consume(const std::vector<std::string>& target) {
    if (target.empty()) {
        std::cout << "Consume what?\n";
        return;
    }

    std::string userInput = target[0]; // string representation of the item we want to consume
    auto it = std::find_if(inventory.begin(), inventory.end(), [&userInput](const Item& item) {
        return item.get_name() == userInput;
    });

    if (it != inventory.end()) {
        // Item found in the user's inventory
        Item item = *it;
        if (item.get_calories() == 0) {
            std::cout << "You can't consume this item.\n";
            return;
        }
        inventory.erase(it); // Remove item from user's inventory
        weight -= item.get_weight(); // Decrease user's carried weight
        std::cout << "You have consumed the " << item.get_name() << ".\n";
        inventoryCapacity += 20; // Increase the amount inventory capacity by 50
        std::cout << "Your inventory capacity has increased.\n";
    } else {
        std::cout << "You don't have that.\n";
    }
}

// Set up commands inside the game.
std::map<std::string, Command> Game::setup_commands() {
    std::map<std::string, Command> cmds;
    cmds["show help"] = std::bind(&Game::show_help, this, std::placeholders::_1);
    cmds["display help"] = std::bind(&Game::show_help, this, std::placeholders::_1);
    cmds["present help"] = std::bind(&Game::show_help, this, std::placeholders::_1);
    cmds["talk"]      = std::bind(&Game::talk, this, std::placeholders::_1);
    cmds["speak"]      = std::bind(&Game::talk, this, std::placeholders::_1);
    cmds["rattle"]      = std::bind(&Game::talk, this, std::placeholders::_1);
    cmds["meet"]      = std::bind(&Game::meet, this, std::placeholders::_1);
    cmds["encounter"]      = std::bind(&Game::meet, this, std::placeholders::_1);
    cmds["confront"]      = std::bind(&Game::meet, this, std::placeholders::_1);
    cmds["take"]      = std::bind(&Game::take, this, std::placeholders::_1); 
    cmds["get"]      = std::bind(&Game::take, this, std::placeholders::_1);
    cmds["grab"]      = std::bind(&Game::take, this, std::placeholders::_1); 
    cmds["give"]      = std::bind(&Game::give, this, std::placeholders::_1); 
    cmds["feed"]      = std::bind(&Game::give, this, std::placeholders::_1);
    cmds["lend"]      = std::bind(&Game::give, this, std::placeholders::_1); 
    cmds["go"]        = std::bind(&Game::go, this, std::placeholders::_1);
    cmds["walk"]        = std::bind(&Game::go, this, std::placeholders::_1);
    cmds["run"]        = std::bind(&Game::go, this, std::placeholders::_1);
    cmds["show items"]= std::bind(&Game::show_items, this, std::placeholders::_1);
    cmds["display items"]= std::bind(&Game::show_items, this, std::placeholders::_1);
    cmds["present items"]= std::bind(&Game::show_items, this, std::placeholders::_1);
    cmds["look"]      = std::bind(&Game::look, this, std::placeholders::_1);
    cmds["see"]      = std::bind(&Game::look, this, std::placeholders::_1); 
    cmds["investigate"]      = std::bind(&Game::look, this, std::placeholders::_1); 
    cmds["quit"]      = std::bind(&Game::quit, this, std::placeholders::_1); 
    cmds["terminate"]      = std::bind(&Game::quit, this, std::placeholders::_1); 
    cmds["retire"]      = std::bind(&Game::quit, this, std::placeholders::_1); 
    cmds["consume"] = std::bind(&Game::consume, this, std::placeholders::_1); 
    return cmds;
}

// Choose and return a random Location.
// (A simple placeholder implementation is provided.)
Location Game::random_location() {
    // For example, if world is populated, pick a random index.

    if (!world.empty()) {
        size_t index = static_cast<size_t>(rand() % world.size());
        std::cout << "World is not empty" << std::endl; //xx
        return world[index];
    }
    // Return a default location if world vector is empty.
    std::cout << "World is empty" << std::endl; //xx
    return Location("Default Location", "A nondescript location.");
}

// Create the game world and return the starting location.
Location Game::create_world() {
    try {

        // Created 10 Items
        Item booze("Booze", "Booze: (A good way to drink your pain away!", 215, 1.55); 
        Item Beef("Beef", "Beef: (This will save your hunger!", 180, 7.75);
        Item Knife("Knife", "Knife: (Stab a scary NPC in the gut!", 0, 2.12); 
        Item Mango("Mango", "Mango: (It is bright yellow, so it is super sweet!", 125, 1.10); 
        Item Peanuts("Peanuts", "Peanuts: (Eat a few to fill empty tummy!", 196, 3.22); 
        Item Hi_C("Hi-C", "Hi-C: (You can drink this to quench thirst!", 171, 1.30);
        Item Chainsaw("Chainsaw", "Chainsaw: (Slice your enemies!!!!", 0, 13.85); 
        Item Broccoli("Broccoli", "Broccoli: (Healthy and low-calorie intake", 40, 0.98); 
        Item Cauliflower("Cauliflower", "Cauliflower: (Tastier alternative to Broccoli!", 40, 1.06);
        Item Cilantro("Cilantro", "Cilantro: (Swallow by itself or put on top of meat!", 28, 0.32); 

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

        // Populate the world vector with several Locations.
        Location loc1("Library", "A quiet place full of books.");
        Location loc2("Cafeteria", "Bustling with activity and the smell of food.");
        Location loc3("Blue Connection", "So many people here inside the POD store.");
        Location loc4("Kirkhof", "An abundance of kids visiting for the first time.");
        Location loc5("Bathroom", "It stinks so bad in the men's bathroom.");
        Location loc6("Mackinac", "Everyone's walking fast to their classes.");
        Location loc7("AuSable", "It's dead in this hall, barely anybody.");
        Location loc8("Gym", "Loud and energetic.");
        Location loc9("Woods", "It is raining heavy in this bih");
        Location loc10("Laker Village", "It is getting buck wild 'round here.");

        // Example: add locations into the world.
        world.push_back(loc1); // Library
        world.push_back(loc2); // Cafeteria
        world.push_back(loc3); // Blue Connection
        world.push_back(loc4); // Kirkhof
        world.push_back(loc5); // Bathroom
        world.push_back(loc6); // Mackinac
        world.push_back(loc7); // AuSable
        world.push_back(loc8); // Gym
        world.push_back(loc9); // Woods
        world.push_back(loc10); // Laker Village

        //world[0] = lib
        world[0].add_neighbor("east", world[3]); //east is Kirkhof (loc4) 
        world[0].add_neighbor("west", world[4]); //west is Bathroom (loc5) 
        world[0].add_neighbor("north", world[2]); //north is Blue Connection (loc3) 
        world[0].add_neighbor("south", world[1]); //south is Cafeteria (loc2) 

        // world[1] == Cafeteria,
        world[1].add_neighbor("east", world[7]); //east is Gym (loc8) 
        world[1].add_neighbor("west", world[6]); //west is Ausable (loc7) 
        world[1].add_neighbor("north", world[0]); //north is Library (loc1) 
        world[1].add_neighbor("south", world[5]); //south is Mackinac (loc6) 
        
        // world[2] == Blue Connection,
        world[2].add_neighbor("east", world[9]); // east is Laker Village (loc10) 
        world[2].add_neighbor("west", world[8]); // west is Woods (loc9) 
        world[2].add_neighbor("south", world[0]); // south is Library (loc1) 

        // world[3] =  Kirkhof
        world[3].add_neighbor("west", world[0]); //west is Library (loc1)
        world[3].add_neighbor("north", world[9]); //north is Laker Village (loc10)
        world[3].add_neighbor("south", world[7]); //south is Gym (loc8)

        //world[4] = Bathroom,
        world[4].add_neighbor("east", world[0]); //east is Library (loc1) 
        world[4].add_neighbor("north", world[8]); //north is Woods (loc9) 
        world[4].add_neighbor("south", world[6]); //south is AuSable (loc7) 

        //world[5] = Mackinac
        world[5].add_neighbor("north", world[1]); // north is Cafeteria (loc2)
        
        //world[6] = AuSable
        world[6].add_neighbor("east", world[1]); // east is cafeteria (loc2)
        world[6].add_neighbor("north", world[4]); //north is bathroom (loc5)

        // world[7] == Gym
        world[7].add_neighbor("west", world[1]); // west is Cafeteria (loc2) 
        world[7].add_neighbor("north", world[3]); // north is Kirkhof (loc4)

        //world[8] = Woods     
        world[8].add_neighbor("east", world[2]); // east is Blue C (loc3) 
        world[8].add_neighbor("south", world[4]); //south is Bathroom (loc5) 

        //world[9] = Laker Village     
        world[9].add_neighbor("west", world[2]); //west is Blue C (loc3) 
        world[9].add_neighbor("south", world[3]); //south is Kirkhof (loc4)

        //NPCs 
        world[1].add_npc(BigFoot); // Cafeteria
        world[7].add_npc(WonderWoman); // Gym
        world[8].add_npc(Elf); // Woods
        world[5].add_npc(Guard); // Mackinac
        world[4].add_npc(Clown); // Bathroom

        //Items
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

/*
core game loop.  prompts user for input, splits it into a command and target, and executes the 
corresponding function from the command map. If the command is invalid, it notifies 
the user. loop checks if the elf consumed enough calories, displaying 
a success or failure message accordingly.
*/
void Game::play() {
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

        // clear the terminal
        if (command == "clear") {
            #ifdef _WIN32
            system("cls");
            #else
            system("clear");
            #endif
            continue;
        }
        
        // Remove specific preposition words from tokens
        std::vector<std::string> wordsToRemove = {
        "a", "abaft", "aboard", "about", "above", "absent", "across",
        "afore", "after", "against", "along", "alongside", "amid",
        "amidst", "among", "amongst", "an", "anenst", "apropos",
        "apud", "around", "as", "aside", "astride", "at", "athwart",
        "atop", "barring", "before", "behind", "below", "beneath",
        "beside", "besides", "between", "beyond", "but", "by",
        "circa", "concerning", "despite", "down", "during", "except",
        "excluding", "failing", "following", "for", "forenenst",
        "from", "given", "in", "including", "inside", "into", "lest",
        "like", "mid", "midst", "minus", "modulo", "me", "near", "next",
        "notwithstanding", "of","off", "on", "onto", "opposite",
        "out", "outside", "over", "pace", "past", "per", "plus", "pro",
        "qua", "regarding", "round", "sans", "save", "since", "than",
        "the", "through", "throughout", "till", "times", "to", "toward",
        "towards", "under", "underneath", "unlike", "until", "unto", "up",
        "upon", "versus", "via", "vice", "with", "within", "without", "worth"
        };
        
        tokens.erase(std::remove_if(tokens.begin(), tokens.end(), 
            [&wordsToRemove](const std::string& word) {
            return std::find(wordsToRemove.begin(), wordsToRemove.end(), word) != wordsToRemove.end();
            }), tokens.end());
        
        if (tokens.size() > 1 && tokens[1] == "help" || tokens[1] == "items") {
            command = tokens[0] + " " + tokens[1];
            tokens.erase(tokens.begin());
        }
        
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