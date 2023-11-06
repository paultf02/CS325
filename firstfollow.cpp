// We take as input transformedgrammar.txt
// and then we calculate FIRST for every production and FOLLOW for every non terminal

// we want production class. 
// list of productions
// first is a function that takes a production as input and produces a vector of terminals as output
// the function operating on a production uses what aspect? it finds the unions

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class Production{
    std::string head;
    std::vector<std::string> disjuncs;
}

std::vector<std::string> first(Production p){
    std::vector<std::string> firstset;
    
    return firstset;
}

int main(){
    std::ifstream myfile ("transformedgrammar.txt");
    std::string mystring;
    if (myfile.is_open()){
        while (myfile.good()){
            myfile >> mystring;
            std::cout << mystring << '\n';
        }
    }
    myfile.close();
    return 0;
}