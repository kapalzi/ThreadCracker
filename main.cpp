//
//  main.cpp
//  Filozofowie1
//
//  Created by Krzysztof Kapała on 02/06/2019.
//  Copyright © 2019 Krzysztof Kapała. All rights reserved.
//

#include <vector>
#include <string>
#include <fstream>
#include "md5.h"

using namespace std;

vector<string>nextPasswordsVector(int sizeOfVector, int lastIndex, string path) {
    vector<string> currentPasswords;
    
    ifstream file(path);
    string temp;
    int i = 0;
    
    while (getline(file, temp))
    {
        if (i > lastIndex) {
            currentPasswords.push_back(md5(temp));
        }
        
        if (i == lastIndex + sizeOfVector ) {
            break;
        }
        
        i++;
    }
    file.close();
    
    return currentPasswords;
}

int numberOfLines(string path) {
    
    ifstream file(path);
    string temp;
    int i = 0;
    
    while (getline(file, temp))
    {
        i++;
    }
    file.close();
    
    return i;
}

int main(int argc, char **argv) {
//    string path = argv[1]
    string path = "/Users/krzysztof/Documents/PWr/semestr-6/so2-projekt/etap2/Filozofowie1/Filozofowie1/passwords.txt";
    int lines = numberOfLines(path);
    
    vector<string> currentPasswords;
    int lastIndex = 0;
    int sizeOfVector = 10000;
    while (lastIndex < lines) {
        clock_t begin_time = clock();
        currentPasswords = nextPasswordsVector(sizeOfVector, lastIndex, path);
        lastIndex += sizeOfVector;
        
        
        printf("%lu\n",currentPasswords.size());
        printf("%f\n",float(clock () - begin_time)/CLOCKS_PER_SEC);
    }

    return 0;
}
