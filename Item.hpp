#ifndef ITEM_H
#define ITEM.H

#include <string>
#include <iostream>

class Item {
    
    public:
    std::string itemname;
    std::string descriptname;
    int calories;
    float weight;

    public:
    Item(std::string itemname, std::string descriptname, int calories, float weight) {
        this->itemname = itemname;
        this->descriptname = descriptname;
        this->calories = calories;
        this->weight = weight;
    }


    friend std::ostream& operator<<(std::ostream& os, const Item& obj){
    os << obj.itemname << "description: " << obj.descriptname << "calories " 
    << obj.calories << "weight: " << obj.weight;
    return os;
    };

};








#endif 