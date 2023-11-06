// We take as input transformedgrammar.txt
// and then we calculate FIRST for every production and FOLLOW for every non terminal

// we want production class. 
// list of productions
// first is a function that takes a production as input and produces a vector of terminals as output
// the function operating on a production uses what aspect? it finds the unions


/*
COMPUTING FIRST SET ALGORITHM
1. first(epsilon) = {}
2. first(terminal) = {terminal}
3. first(ab...c...d) = 
    i) first(a) if a is not nullable
    ii) ((first(a) U first(b) U ...) - epsilon) U first(c) if a, b, ... are nullable and c is not nullable
    iii) first(a) U ... U first(d) if all of a, ..., d are nullable
4. first( a | b | ... | c ) = first(a) U ... first(c)

COMPUTING NULLABLE
1. nullable(epsilon) = true
2. nullable(terminal) = false
3. nullable(ab...c) = nullable(a) AND nullable(b) ... AND nullable(c)
4. nullable( a | b | ... | c ) = nullable(a) OR ... OR nullable(c)
    
*/
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