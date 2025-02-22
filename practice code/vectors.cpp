#include <iostream>
#include <array>
#include <vector>

using namespace std;

int main(){
    vector<string> foods ={"grapes", "carrots", "lemons"};
    foods.push_back("Tortiallas");
    for(auto food : foods){
        cout << food << endl;
    }

    system("pause");
    return 0;
}