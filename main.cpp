#include <iostream>
#include <string>
#include "Item.hpp"
#include "npc.hpp"

int main() {
    Item booze("Booze", "Drink your pain away", 215, 11.75);

    // Creating an NPC with multiple messages
    NPC npc1("Guard", "A stern-looking watchman.", {"Halt! Who goes there?", "Stay out of trouble.", "Move along."});

    // Display NPC name and description using overloaded <<
    std::cout << npc1 << std::endl;

    // Getting messages one by one
    std::cout << "NPC says: " << npc1.getNextMessage() << std::endl;
    std::cout << "NPC says: " << npc1.getNextMessage() << std::endl;
    std::cout << "NPC says: " << npc1.getNextMessage() << std::endl;
    std::cout << "NPC says: " << npc1.getNextMessage() << std::endl; // Loops back to first message

    // Using overloaded << operator
    std::cout << "NPC Name: " << npc1 << booze << std::endl;
    std::cout << "Item here: " << booze << std::endl;

    return 0;
}

