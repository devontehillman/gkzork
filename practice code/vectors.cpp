#include <iostream>
#include <array>
#include <vector>

using namespace std;

int main(){
    vector<string> foods ={"grapes", "carrots", "lemons"};
    foods.push_back("Tortiallas");
    for(auto food : foods){
        std::cout << food << " ";
    }

    vector<string> foods_again ={"watermelons", "mangos", "kiwis"};

    for (auto it : foods_again) {

    // if (std::find(currentLocation.get_npcs().begin(), 
    // currentLocation.get_npcs().end(), it) != currentLocation.get_npcs().end()) {

        cout << it << " ";
        // target.getNextMessage();
    }
    cout << endl;

    


    system("pause");
    return 0;
}