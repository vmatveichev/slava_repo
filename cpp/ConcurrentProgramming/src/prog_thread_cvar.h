#include "prog.h"
#include <deque>

std::deque<int> q;
std::mutex muCVar;
std::condition_variable cond;

class ProgThreadCondVar
{
public:
    void main();
};

void function_cvar_1()
{
    int count = 10;
    while (count > 0)
    {
        std::unique_lock<mutex> locker(muCVar); //unique_lock is used because it's repeated here more than 1 time
        q.push_front(count);
        cout << "function_cvar_1: push_front " << count << endl;
        locker.unlock();
        //cond.notify_one(); // notifying one waiting thread if there is one
        cond.notify_all(); // notifying all waiting threads
        std::this_thread::sleep_for(chrono::milliseconds(10));
        count --;
    }
    cout << "function_cvar_1: the loop is finished" << endl;
}

void function_cvar_2()
{
    int data = 0;
    while (data != 1)
    {
        std::unique_lock<mutex> locker(muCVar);
        if (!q.empty())
        {
            data = q.back();
            q.pop_back();
            locker.unlock();
            cout << "t2 got a value from t1: " << data << endl;
        } else {
            locker.unlock();
            // everything is thread safe but we spend unnecessary time in looping here
            // until new element is placed to the queue in function_cvar_1() above.
            // to avoid it we can wait some time before next cycle. But it's not optimal
            // also because the time can't be easily calculated properly.
            
            //std::this_thread::sleep_for(chrono::seconds(5));

            // So, we use conditional variables in such cases where can't exactly know
            // the time (BTW this time can be variable) when the element is placed
            // to the deque. Please refer to the function_cvar_3().
        }
    }
}

void function_cvar_3()
{
    int data = 0;
    while (data != 1)
    {
        std::unique_lock<mutex> locker(muCVar); //unique_lock is used because it's repeated here more than 1 time
        //cond.wait(locker); // spurious wake
        cond.wait(locker, [](){ return !q.empty(); }); // spurious wake - it means that wait can be triggered not
                                                       // only by notify method but by predefined condition also 
        data = q.back();
        q.pop_back();
        locker.unlock();
        std::this_thread::sleep_for(chrono::milliseconds(50));
        cout << "t2 got a value from t1: " << data << endl;
    }
}

void ProgThreadCondVar::main()
{
    cout << "ProgThreadCondVar::main" << endl;

    std::thread t1(function_cvar_1);
    //std::thread t2(function_cvar_2);
    std::thread t2(function_cvar_3);
    t1.join();
    t2.join();
}
