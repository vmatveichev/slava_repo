#include "prog.h"

std::mutex mu;

void shared_print(string str, int id)
{
    //mu.lock(); <- is not safe in case if next instruction throws an exception
    std::lock_guard<std::mutex> locker(mu);
    cout << str << id << endl;
    //mu.unlock();
} // please check LogFile class instead

class ProgThreadMu
{
    public:
        void main();
};

void ProgThreadMu::main()
{
    cout << "ProgThreadMu::main" << endl;
    LogFile log("/home/slava/temp/progs/C++/ConcurrentProgramming/LogFiles/logFileMu.txt");
    std::thread t1(function1<LogFile>, std::ref(log));
    
    for (auto i=0; i<100; i++)
    {
        log.shared_print("from main: ", i);
    }
    
    t1.join();
}
