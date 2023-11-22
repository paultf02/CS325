#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <utility>

// int current = 9;

// int f(){
//     int temp = 7;
//     return (current = temp);
// }

// int main(){
//     std::cout << "current="<< current << '\n';
//     int n = f();
//     std::cout << "after f"<< '\n';
//     std::cout << "n="<< n << '\n';
//     std::cout << "current="<< current << '\n';
//     current = 1;
//     std::cout << "after reassigning current"<< current << '\n';
//     std::cout << "n="<< n << '\n';
//     std::cout << "current="<< current << '\n';
//     return 0;
// }

class Animal{
public:
  std::string name = "animal";
  Animal(){
    std::cout << "I am parent class Animal\n";
  }
};

class Dog : public Animal{
public:
  std::string name = "dog";
  Dog(){
    std::cout << "I am child class Dog\n";
  }
};

class Cat : public Animal{
public:
  std::string name = "cat";
  Cat(){
    std::cout << "I am child class Cat\n";
  }
};

int main(){
    std::cout << "declaring vector:\n";
    std::vector<std::unique_ptr<Animal>> vec(3);
    std::cout << "declaring Animal:\n";
    std::unique_ptr<Animal> a = std::make_unique<Animal>();
    std::cout << a->name << '\n';
    std::cout << "declaring Dog:\n";
    std::unique_ptr<Dog> d = std::make_unique<Dog>();
    std::cout << d->name << '\n';
    std::cout << "declaring Cat:\n";
    std::unique_ptr<Cat> c = std::make_unique<Cat>();
    std::cout << c->name << '\n';

    

    std::cout << "moving to vector\n";
    vec[0] = std::move(a);
    vec[1] = std::move(d);
    vec[2] = std::move(c);

    for (auto &elem : vec){
        std::cout << elem->name << '\n';
    }
    return 0;
}