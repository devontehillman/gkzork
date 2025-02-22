#ifndef __HPP__HOUSE__
#define __HPP__HOUSE__

#include <iostream>

class House {
  public:
  House(int sqft, double askingPrice, int numBathrooms){
    this->sqft = sqft;
    this->askingPrice = askingPrice;
    this->numBathrooms = numBathrooms;
  }
  int sqft;
  double askingPrice;
  int numBathrooms;
  
  
  friend bool operator<(const House& l, const House& r){
    return l.sqft < r.sqft && l.numBathrooms < r.numBathrooms;
    }
  
  
  friend std::ostream& operator<<(std::ostream& os, const House& obj){
    os << obj.sqft << ", " << obj.numBathrooms << " $" << obj.askingPrice;
    return os;
    }
};

#endif