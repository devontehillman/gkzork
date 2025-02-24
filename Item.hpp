#ifndef __HPP__ITEM__
#define __HPP__ITEM__

#include <string>
#include <iostream>
#include <stdexcept> //found this from google's AI overview

class Item {
    
    private:
    std::string itemname;
    std::string descriptname;
    int calories;
    float weight;

    void set_itemname(const std::string& item_name) {
        if (item_name == "") {
            throw std::invalid_argument("item name is blank, fix this now!");
        }
        else {
            this->itemname = item_name;
        }
    }

    void set_itemcalories(int item_calories) {
        if (item_calories < 0 || item_calories > 1000) {
            throw std::invalid_argument("calories of item is not between 0-1000");
        }
        else {
            this->calories = item_calories;
        }
    }

    void set_itemdescription(const std::string& item_descript) {
        if (item_descript == "") {
            throw std::invalid_argument("description of item is blank, fix this now!");
        }
        else {
            this->descriptname = item_descript;
        }
    }

    void set_itemweight(float item_weight) {
        if (item_weight < 0 || item_weight > 500) {
            throw std::invalid_argument("weight of item is not between 0-500");
        }
        else {
            this->weight = item_weight;
        }
    }

    public:
    Item(const std::string& itemname, const std::string& descriptname, int calories, float weight) {
        // if (itemname == "") {
        //     throw std::invalid_argument("item name is blank, fix this now!");
        // }
        // else {
        //     this->itemname = item_name;
        // }
        set_itemname(itemname);
        set_itemdescription(descriptname);
        set_itemcalories(calories);
        set_itemweight(weight);


        // this->itemname = itemname;
        // this->descriptname = descriptname;
        // this->calories = calories;
        // this->weight = weight;
    }

    // void set_itemname(std::string item_name) {
    //     if (item_name == "") {
    //         throw std::invalid_argument("item name is blank, fix this now!");
    //     }
    //     else {
    //         this->itemname = item_name;
    //     }
    // }

    // void set_itemcalories(int item_calories) {
    //     if (item_calories < 0 || item_calories > 1000) {
    //         throw std::runtime_error("calories of item is not between 0-1000");
    //     }
    //     else {
    //         calories = item_calories;
    //     }
    // }

    // void set_itemdescription(std::string item_descript) {
    //     if (item_descript == "") {
    //         throw std::runtime_error("description of item is blank, fix this now!");
    //     }
    //     else {
    //         descriptname = item_descript;
    //     }
    // }

    // void set_itemweight(float item_weight) {
    //     if (item_weight < 0 || item_weight > 500) {
    //         throw std::runtime_error("weight of item is not between 0-500");
    //     }
    //     else {
    //         weight = item_weight;
    //     }
    // }




    friend std::ostream& operator<<(std::ostream& os, const Item& obj){
    os << obj.itemname << "(" << obj.calories << " calories) - " << obj.weight << " lb - " << obj.descriptname;
    // os << obj.itemname << "description: " << obj.descriptname << "calories " 
    // << obj.calories << " weight: " << obj.weight;
    return os;
    };

};








#endif 