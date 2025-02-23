#ifndef __HPP__ITEM__
#define __HPP__ITEM__

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

    void set_itemname(std::string itemname) {
        if (itemname == "") {
            // raise error
        }
        else {
            itemname = itemname;
        }
    }

    void set_itemcalories(int item_calories) {
        if (item_calories < 0 || item_calories > 1000) {
            // raise error
        }
        else {
            calories = item_calories;
        }
    }

    void set_itemdescription(std::string item_descript) {
        if (item_descript == "") {
            // raise error
        }
        else {
            descriptname = item_descript;
        }
    }

    void set_itemweight(float item_weight) {
        if (item_weight < 0 || item_weight > 500) {
            // raise error
        }
        else {
            weight = item_weight;
        }
    }




    friend std::ostream& operator<<(std::ostream& os, const Item& obj){
    os << obj.itemname << "(" << obj.calories << " calories) - " << obj.weight << " lb - " << obj.descriptname;
    // os << obj.itemname << "description: " << obj.descriptname << "calories " 
    // << obj.calories << " weight: " << obj.weight;
    return os;
    };

};








#endif 