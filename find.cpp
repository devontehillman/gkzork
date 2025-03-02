#include <iostream>
#include <vector>
#include <algorithm> // For std::find_if

class Item {
private:
    std::string name;

public:
    Item(const std::string& name) : name(name) {}

    // Getter for name
    std::string get_name() const { return name; }
};

int main() {
    std::vector<Item> items = {
        Item("Apple"),
        Item("Banana"),
        Item("Cherry")
    };

    std::string userInput;
    std::cout << "Enter item name to search: ";
    std::cin >> userInput;

    // Use std::find_if with a lambda to search
    auto it = std::find_if(items.begin(), items.end(), 
        [&userInput](const Item& item) {
            return item.get_name() == userInput; 
        }
    );

    if (it != items.end()) {
        std::cout << "Found: " << it->get_name() << std::endl;
    } else {
        std::cout << "Item not found." << std::endl;
    }

    return 0;
}