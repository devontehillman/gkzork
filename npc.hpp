#ifndef __HPP__HOUSE__
#define __HPP__HOUSE__

#include <iostream>
#include <vector>


class NPC {
    private:
        std::string name;
        std::string description;
        int messageNumber;
        std::vector<std::string> messages; // Vector to store multiple messages

    public:
        //  initializer list constructor
        NPC(std::string npcName, std::string description, std::vector<std::string> npcMessages)
        // Initializer List:   
        : name(npcName), description(description), messages(npcMessages), messageNumber(0){}

        // Getter function for name
        std::string getName() const { return name; }

        // Getter function for description
        std::string getDescription() const { return description; }

        // Method to get the next message and increment messageNumber
        std::string getNextMessage() {

            std::string message = messages[messageNumber]; // Get current message
            messageNumber = (messageNumber + 1) % messages.size(); // Increment & loop back

            return message;
        }

        // Overload << operator as a friend function
        friend std::ostream& operator<<(std::ostream& os, const NPC& npc) {
            os << npc.name << " " << npc.description;
            return os;
        }
};


#endif