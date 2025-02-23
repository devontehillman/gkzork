#include <iostream>
#include <string>
#include "Item.hpp"

class NPC {
private:
    std::string name;
    std::string description;
    int messageNumber;
    // messages;

public:
    //  initializer list constructor
    NPC(std::string npcName, std::string description)
    // Initializer List:   
    : name(npcName), description(description){}

    // Getter function for name
    std::string getName() const { return name; }

     // Getter function for description
     std::string getDescription() const { return description; }

    // Overload << operator as a friend function
    friend std::ostream& operator<<(std::ostream& os, const NPC& npc) {
        os << npc.name, npc.description;
        return os;
    }
};

int main() {
    NPC npc1("Guard", "Halt! Who goes there?");
    Item booze("Booze", "Drink your pain away", 215, 11.75);

    
    // Using overloaded << operator
    std::cout << "NPC Name: " << npc1 << booze << std::endl;
    std::cout << "Item here: " << booze << std::endl;



    return 0;
}

