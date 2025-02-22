#include "House.hpp" // Include the House class definition
#include <algorithm> // Include algorithm library for std::sort
#include <iostream> // Include iostream library for std::cout
#include <random> // Include random library for random number generation
#include <vector> // Include vector library for std::vector
 
// Function to print the inventory of houses
void printInventory(std::vector<House> inventory){
    for(auto it=inventory.begin(); it != inventory.end(); ++it){
        std::cout << *it << std::endl; // Print each house in the inventory
    }
}
 
int main(int argc, char** argv){
    std::vector<House> inventory; // Vector to store the inventory of houses
    std::random_device rd; // Random device to seed the random number generator
    std::mt19937 engine(rd()); // Mersenne Twister random number generator
    std::uniform_int_distribution<int> distribution(1, 6000); // Uniform distribution for square footage
    
    // Generate 100 houses with random square footage
    for(int i=0; i<100; ++i){
        House tmp(0, 500000, 4); // Create a house with default values
        int sqft = distribution(engine); // Generate random square footage
        tmp.sqft = sqft; // Set the square footage of the house
        inventory.push_back(tmp); // Add the house to the inventory
    }
    
    std::sort(inventory.begin(), inventory.end()); // Sort the inventory of houses
    printInventory(inventory); // Print the sorted inventory

    return 0; // Return 0 to indicate successful execution
}