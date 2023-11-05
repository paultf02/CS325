// We take as input transformedgrammar.txt
// and then we calculate FIRST for every production and FOLLOW for every non terminal

// we want production class. 
// list of productions
// first is a function that takes a production as input and produces a vector of terminals as output

#include <iostream>
#include <fstream>
#include <string>

int main(){
    std::ifstream myfile ("transformedgrammar.txt");
    std::string mystring;
    if (myfile.is_open()){
        while (myfile.good()){
            myfile >> mystring;
            std::cout << mystring;
        }
    }
    myfile.close();
    return 0;
}