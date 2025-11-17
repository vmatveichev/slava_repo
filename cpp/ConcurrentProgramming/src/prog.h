#ifndef PROG_H
#define PROG_H

#include <iostream>
#include <exception>
#include <string>
#include <thread>
#include <mutex>
#include <fstream>
#include <condition_variable>

using namespace std;

class myException : public exception
{
    string _exStr;
    
    public:
    myException(string str): _exStr(str) {}
    string getStr() {return _exStr;}
};

class LogFile
{
    std::mutex _mu;
    ofstream _f;

public:
    LogFile(string fileName)
    {
        _f.open(fileName);
    }
    ~LogFile()
    {
        _f.close();
    }
    void shared_print(string str, int id)
    {
        // unique_lock is more heavy from performance perspective than lock_guard
        // but it can be locked/unlocked several times. And it can be defered to lock it later.
        std::unique_lock<std::mutex> locker(_mu, std::defer_lock); 
        
        // ... DO something else
        
        locker.lock();
        _f << str << id << endl;
        locker.unlock();
        locker.lock();
        locker.unlock();
        std::unique_lock<std::mutex> locker2 = std::move(locker);
        locker2.lock();
        locker2.unlock();
    }
};

template <typename T>
void function1(T& log)
{
    for (auto i=0; i>-100; i--)
    {
        log.shared_print("from t1: ", i);
    }
}

#endif
