#include "prog.h"

class ProgThreadULock
{
    public:
        void main();
};

class LogFileULock
{
    string _fileName;
    std::mutex _mu;
    //std::mutex _mu_open;
    std::once_flag _flag;
    ofstream _f;

public:
    LogFileULock(string fileName) : _fileName(fileName) {}

    ~LogFileULock()
    {
        _f.close();
    }
    void shared_print(string str, auto id)
    {
        /* The construction below is thread safe but it's not optimal because
           on each iteration we lock the mutex and check if the file already opened.
           Please refer to call_once construction below for solution.
        {
            std::unique_lock<mutex> locker2(_mu_open);
            if (!_f.is_open())
            {
                _f.open("/home/slava/temp/progs/C++/ConcurrentProgramming/LogFiles/logFileUl.txt");
            }
        }*/
        
        std::call_once(_flag, [&](){ _f.open(_fileName); });

        std::unique_lock<mutex> locker(_mu);
        _f << str << id << endl;
    }
};

void ProgThreadULock::main()
{
    cout << "ProgThreadULock::main" << endl;
    LogFileULock log("/home/slava/temp/progs/C++/ConcurrentProgramming/LogFiles/logFileUl.txt");
    std::thread t1(function1<LogFileULock>, std::ref(log));
    
    for (auto i=0; i<100; i++)
    {
        log.shared_print("from main: ", i);
    }
    
    t1.join();
}
