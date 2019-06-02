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
#include <thread>
#include <unistd.h>
#include <atomic>
#include <mutex>

using namespace std;

#define threadCount 6000
std::atomic<int> finishedThreads; //jak 6000 to koniec
std::atomic<bool> finish; // jeśli znajdzie hasło to true
string findPassword;
static std::mutex findPasswordMutex = std::mutex();;
vector<thread> threads;

vector<string>nextPasswordsVector(int sizeOfVector, int lastIndex, string path) {
    vector<string> currentPasswords;
    
    ifstream file(path);
    string temp;
    int i = 0;
    
    while (getline(file, temp))
    {
        if (i > lastIndex) {
            currentPasswords.push_back(temp);
        }
        
        if (i == lastIndex + sizeOfVector ) {
            break;
        }
        
        i++;
    }
    file.close();
    
    return currentPasswords;
}

void stopAllThreads() {
    for ( auto&& t : threads) {
        t.join();
    }
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

void startTmpThread(vector<string> passwords) {
    
    std::atomic<bool> findPassword;
    findPassword.store(false);
    std::string temporaryPassword;
    std::string hashedPassword;

    while (findPassword.load() && passwords.size() > 0)
    {
        temporaryPassword = passwords.back();
        passwords.pop_back();
        hashedPassword = md5(temporaryPassword);
    }
    

    finishedThreads += 1;
}

int main(int argc, char **argv) {
    finishedThreads.store(0);
    finish.store(false);
    //    string path = argv[1]
    string path = "./passwords.txt";
    int lines = numberOfLines(path);
    
    vector<string> currentPasswords;
    int lastIndex = 0;
    int sizeOfVector = 650; //        500mb/128 = 3 906 250 / 6000 = 651,041666
    int i = 0;
    while (lastIndex < lines) {
        currentPasswords = nextPasswordsVector(sizeOfVector, lastIndex, path);
        lastIndex += sizeOfVector;
        
        if (threads.size() < threadCount) {
            threads.push_back(thread(startTmpThread, currentPasswords));
        } else {
            while (finishedThreads < threadCount) {
                //czekanie az sie skończą xd
            }
        }
        
        if (finishedThreads == threadCount) {
            stopAllThreads();
            finishedThreads = 0;
            threads.clear();
            printf("%i\n",i);
        }
        
        i++;
        
        if (finish == true) break;
    }
    stopAllThreads();
    
    printf("%i",i);
    return 0;
}
