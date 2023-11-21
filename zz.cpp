#include <iostream>

int current = 9;

int f(){
    int temp = 7;
    return (current = temp);
}

int main(){
    std::cout << "current="<< current << '\n';
    int n = f();
    std::cout << "after f"<< '\n';
    std::cout << "n="<< n << '\n';
    std::cout << "current="<< current << '\n';
    current = 1;
    std::cout << "after reassigning current"<< current << '\n';
    std::cout << "n="<< n << '\n';
    std::cout << "current="<< current << '\n';
    return 0;
}