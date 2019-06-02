//
//  main.cpp
//  Filozofowie1
//
//  Created by Krzysztof Kapała on 02/06/2019.
//  Copyright © 2019 Krzysztof Kapała. All rights reserved.
//
#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "md5.h"
#include <thread>
#include <unistd.h>
#include <atomic>
#include <mutex>
#include <typeinfo>

// Tested hash 53e21a352428181343474073e76918ed


using namespace std;

#define threadCount 6000
std::atomic<int> finishedThreads; //jak 6000 to koniec
std::atomic<bool> finish; // jeśli znajdzie hasło to true
std::string findPassword;
// std::string toCrack;
static std::mutex isFindPasswordMutex = std::mutex();;
vector<thread> threads;
std::atomic<char*> toCrack;
std::atomic<bool> isFindPassword;
std::vector<bool> isEndedThread;


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

void startTmpThread(vector<string> passwords, int id) {
    
    
    std::string temporaryPassword;
    std::string hashedPassword;
    while (!isFindPassword.load() && passwords.size() > 0)
    {
        temporaryPassword = passwords.back();
        passwords.pop_back();
        hashedPassword = md5(temporaryPassword);
        //std::cout << hashedPassword << std::endl;
        if(std::strcmp(hashedPassword.c_str(),toCrack.load()) == 0)
        {
            cout<<"\n Find Password\n";
            isFindPasswordMutex.lock();
            findPassword = temporaryPassword;
            isFindPassword.store(true);
            isFindPasswordMutex.unlock();
        }
    }
    
    isEndedThread[id] = true;

    finishedThreads += 1;
}

void init(char* pass1)
{
    finishedThreads.store(0);
    finish.store(false);
    toCrack.store(pass1);
    isFindPassword.store(false);
}

int main(int argc, char **argv) {

    //    string path = argv[1]
    string path = "./passwords.txt";
    int lines = numberOfLines(path);
    init(argv[1]);


    vector<string> currentPasswords;
    int lastIndex = 0;
    int sizeOfVector = 650; //        500mb/128 = 3 906 250 / 6000 = 651,041666
    int i = 0;
    bool isEnd = false;
    while (lastIndex < lines) {
        currentPasswords = nextPasswordsVector(sizeOfVector, lastIndex, path);
        lastIndex += sizeOfVector;
        
        if (threads.size() < threadCount) {
            threads.push_back(thread(startTmpThread, currentPasswords, i));
            isEndedThread.push_back(false);
            //cout<<"\nCreateThread\n";
        } else {
            while (!isEnd) {
                for(auto x = 0; x < isEndedThread.size(); x++)
                {
                    if(isEndedThread[x] == false)
                        break;
                    isEnd = true;
                }
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
    
    std::cout<< "Password: " << findPassword << std::endl;

    printf("%i",i);
    return 0;
}
