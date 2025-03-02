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
    std::cout << "Available commands:\n";
    std::cout << "  show help      - Display this help message\n";
    std::cout << "  meet <npc>     - Meet an NPC in the current location\n";
    std::cout << "  take <item>    - Take an item from the current location\n";
    std::cout << "  give <item>    - Give an item to the current location or NPC\n";
    std::cout << "  go <direction> - Move to a neighboring location\n";
    std::cout << "  show items     - Show items in your inventory\n";
    std::cout << "  look           - Look around the current location\n";
    std::cout << "  Attack         - slapp that hoe if you ain't got nothing in your inventory. ";
    std::cout << "Otherwise, use that weapon you may have\n"; //xx Edit
    std::cout << "  Consume <item> - drink item \n"; //xx Edit
    std::cout << "  quit        - Quit the game\n";
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

void Game::meet(const std::vector<std::string>& target) {

    std::vector<NPC> getNpcs = currentLocation.get_npcs(); // store current room's NPCs

    std::string userInput = target[0]; //string rep of the NPC we're meeting

    for (auto curNPC : getNpcs) { // loop through each NPC that's in the room
        // auto curNPCname = std::find(target.begin(), target.end(), curNPC.getName());
        // found the std::find function from AI by the way 
        // try to find the NPC's name in the vector of strings we passed in the parameter

        if (curNPC.getName() == userInput) { // if this NPC name we tried to find doesn't reach 
        // the end of the iteration for the vector, this means the name is definitely in vector
            std::cout << curNPC.getDescription() << std::endl;
            return;

        // if (curNPCname != target.end()) { // if this NPC name we tried to find doesn't reach 
        // // the end of the iteration for the vector, this means the name is definitely in vector
        //     std::cout << curNPC.getDescription() << std::endl;
        //     return;
            // Now print the description the NPC gave us when we meet him/her
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

    if (weight > 30) { // checks to see if player's weight is over 30 pounds.
        std::cerr << "the player's weight is over " << weight << " lbs, can't move!";
        return; // not sure if I'm supposed to return nothing or someething else. Can't be the
        // weight because this function can't return an int type.
    }
    else {

        std::string userInput = target[0]; //string rep of the Location we're going to

        for (auto& keyvalue_pair : currentLocation.get_neighbors()) { // iterate through the
            // map/dictionary with the elements being the key and value paired up, for example:
            // key for first iteration could be a string - "North", value associated to that key
            // is the Location object.
            // auto curKey = std::find(target.begin(), target.end(), keyvalue_pair.first);
            // trying to find the key element this loop is currently looking at, in the vector of 
            // strings we passed in the parameter

            if (keyvalue_pair.first == userInput) {
                // looping through game class to find set current location
                for (size_t i = 0; i < world.size(); ++i) { 
                    if (world[i].get_name() == keyvalue_pair.second.get_name()) {
                        currentLocation = world[i];
                        return;
                    }
                }
                // the player's current location needs to be
                // set to the value that matches the current key from the map, but problem is that the
                // "currentLocation" is a Location type and value is a Location* type right now, which
                // isn't matching types. 
                
                // FIXED ISSUES in location.hpp by removing pointers for
                // "neighbors" map and changed if statement to "if(pair.second.name == newLocation.name)"
                // in add neighbor function.
            }

            // if (curKey != target.end()) { // if this key we tried to find doesn't reach 
            // // the end of the iteration for the vector, this means the key in map is definitely in vector
            //     currentLocation = keyvalue_pair.second; 
            //     std::cout << currentLocation << std::endl; //xx
            //     return;
            //     // the player's current location needs to be
            //     // set to the value that matches the current key from the map, but problem is that the
            //     // "currentLocation" is a Location type and value is a Location* type right now, which
            //     // isn't matching types. 
                
            //     // FIXED ISSUES in location.hpp by removing pointers for
            //     // "neighbors" map and changed if statement to "if(pair.second.name == newLocation.name)"
            //     // in add neighbor function.
            // }
        }

        std::cout << "Can't go to the direction you provided" << std::endl;
        return;
    }
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


    // Game::inventory()



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
look(std::vector<std::string> target) method.
This method will print the current location, a vector of Items
in the location or a message indicating there are none, a vector of NPCs in
the room or the message “You are alone.”, and a vector of directions in
which the player can go. If a location has been visited previously, print
the direction and the location. Otherwise, simply print the direction.
*/
void Game::look(const std::vector<std::string>& target) {
    std::cout << "You are at: " << currentLocation.get_name() << "\n";
    std::cout << currentLocation << "\n";
    
    // // Print items in the location
    // std::vector<Item> items = currentLocation.get_items();
    // if (!items.empty()) {
    //     std::cout << "Items here:\n";
    //     for (const auto& item : items) {
    //         std::cout << "- " << item.get_name() << "\n";
    //     }
    // } else {
    //     std::cout << "There are no items here.\n";
    // }

    // // Print NPCs in the location
    // std::vector<NPC> npcs = currentLocation.get_npcs();
    // if (!npcs.empty()) {
    //     std::cout << "NPCs here:\n";
    //     for (const auto& npc : npcs) {
    //         std::cout << "- " << npc.getName() << ": " << npc.getDescription() << "\n";
    //     }
    // } else {
    //     std::cout << "You are alone.\n";
    // }

    // Print directions and neighboring locations
    std::map<std::string, Location> neighbors = currentLocation.get_neighbors();
    if (!neighbors.empty()) {
        std::cout << "You can go:\n";
        for (const auto& neighbor : neighbors) {
            const std::string& direction = neighbor.first;
            Location location = neighbor.second;
            if (location.get_visited()) {
                std::cout << "- " << direction << " to " << location.get_name() << "\n";
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
    cmds["meet"]      = std::bind(&Game::meet, this, std::placeholders::_1);
    cmds["take"]      = std::bind(&Game::take, this, std::placeholders::_1); //xx 
    cmds["give"]      = std::bind(&Game::give, this, std::placeholders::_1); //xx
    cmds["go"]        = std::bind(&Game::go, this, std::placeholders::_1);
    cmds["show items"]= std::bind(&Game::show_items, this, std::placeholders::_1);
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
    
        Item booze("Booze", "Booze: (A good way to drink your pain away!", 215, 1.55); // 1
        Item Beef("Beef", "Beef: (This will save your hunger!", 180, 7.75); // 2
        Item Knife("Knife", "Knife: (Stab a scary NPC in the gut!", 0, 2.12); // 3
        Item Mango("Mango", "Mango: (It is bright yellow, so it is super sweet!", 125, 1.10); // 4
        Item Peanuts("Peanuts", "Peanuts: (Eat a few to fill empty tummy!", 196, 3.22); // 5
        Item Hi_C("Hi-C", "Hi-C: (You can drink this to quench thirst!", 171, 1.30); // 6
        Item Chainsaw("Chainsaw", "Chainsaw: (Slice your enemies!!!!", 0, 13.85); // 7
        Item Broccoli("Broccoli", "Broccoli: (Healthy and low-calorie intake", 40, 0.98); // 8
        Item Cauliflower("Cauliflower", "Cauliflower: (Tastier alternative to Broccoli!", 500, 1.06); // 9
        // changed calorie of Cauliflower to 500 to test, change back to 40 when done
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

        // Set neighbor relationships if desired, e.g.:
        // world[0].add_neighbor("east", &world[1]);  // etc.

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

        //NPC 
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

        //std::cout << currentLocation << std::endl; //xx
        std::string userInput;
        std::getline(std::cin, userInput);

        // Split user input into tokens
        std::istringstream iss(userInput);
        std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

        if (tokens.empty()) {
            continue;
        }

        std::string command = tokens[0];

        if (tokens.size() > 1 && tokens[1] == "help" || tokens[1] == "items") {
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
        //currentLocation = random_location(); //xx
    }

    if (numCalories <= 0) {
        std::cout << "Congratulations! The elf has received enough calories.\n";
    } else {
        std::cout << "Game over. The elf did not receive enough calories.\n";
    }
}