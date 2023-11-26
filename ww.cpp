#include <vector>
#include <memory>
#include <utility>
#include <iostream>

using std::vector;
using std::unique_ptr;
using std::make_unique;

class Animal{
public:
  std::string name = "animal";
  std::string id = "default";
  Animal(){
    std::cout << "I am class Animal\n";
  }
};

void populate_vecuniq(vector<unique_ptr<Animal>> &v){
    auto a1 = make_unique<Animal>();
    a1->id = "alice";
    auto a2 = make_unique<Animal>();
    a2->id = "bob";
    v.push_back(std::move(a1));
    v.push_back(std::move(a2));
}

int main(){
    vector<unique_ptr<Animal>> vec;
    populate_vecuniq(vec);
    for (auto &elem : vec){
        std::cout << elem->id << '\n';
    }
    return 0;
}