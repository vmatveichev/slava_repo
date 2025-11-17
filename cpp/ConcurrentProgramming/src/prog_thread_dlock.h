#include "prog.h"

class ProgThreadDLock
{
    public:
        void main();
};

class LogFileDLock
{
    std::mutex _mu;
    std::mutex _mu2;
    ofstream _f;

public:
    LogFileDLock(string fileName)
    {
        _f.open(fileName);
    }
    ~LogFileDLock()
    {
        _f.close();
    }
    void shared_print(string str, int id)
    {
        std::lock(_mu, _mu2);
        std::lock_guard<std::mutex> locker(_mu, std::adopt_lock);
        std::lock_guard<std::mutex> locker2(_mu2, std::adopt_lock);
        _f << str << id << endl;
    }
    void shared_print2(string str, int id)
    {
        std::lock(_mu, _mu2);
        // std::lock_guard<std::mutex> locker2(_mu2); <- don't mix lock order to avoid deadlock!
        std::lock_guard<std::mutex> locker(_mu, std::adopt_lock);
        std::lock_guard<std::mutex> locker2(_mu2, std::adopt_lock);
        _f << str << id << endl;
    }
};

void ProgThreadDLock::main()
{
    cout << "ProgThreadDLock::main" << endl;
    LogFileDLock log("/home/slava/temp/progs/C++/ConcurrentProgramming/LogFiles/logFileDl.txt");
    std::thread t1(function1<LogFileDLock>, std::ref(log));
    
    for (auto i=0; i<100; i++)
    {
        log.shared_print2("from main: ", i);
    }
    
    t1.join();
}
