#include "prog.h"
#include <future>

std::mutex muFp;
std::condition_variable cvarFp;

class ProgThreadFProm
{
    public:
        void main();
};

void factorial(int num, int& res)
{
    int i = num;
    while (i != 0)
    {
        res *= i;
        i--;
    }
    cout << "Factorial result calculated by  " << std::this_thread::get_id() << " for " << num << " is " << res << endl;

    // we should use the construction below to guarantee that child thread will
    // calculate res first and only after that it will be used by main thread
    cvarFp.notify_all();
}

int factorial_fu(int num)
{
    int res = 1;
    int i = num;
    while (i != 0)
    {
        res *= i;
        i--;
    }
    cout << "Factorial result calculated by  " << std::this_thread::get_id() << " for " << num << " is " << res << endl;
    
    // ... new child thread using res value can be created here for example ...
    
    return res;
}

int factorial_prom(std::future<int>& fu_p)
{
    int res = 1;
    cout << "Factorial_using_promises starts here " << endl;

    int i, num;
    i = num = fu_p.get();
    while (i != 0)
    {
        res *= i;
        i--;
    }
    cout << "Factorial_using_promises result calculated by " << std::this_thread::get_id() << " for " << num << " is " << res << endl;
    
    return res;
}

int factorial_prom2(std::shared_future<int> shf)
{
    int res = 1;
    cout << "Factorial_using_promises starts here " << endl;

    int i, num;
    i = num = shf.get();
    while (i != 0)
    {
        res *= i;
        i--;
    }
    cout << "Factorial_using_promises result calculated by " << std::this_thread::get_id() << " for " << num << " is " << res << endl;
    
    return res;
}

void ProgThreadFProm::main()
{
    int res = 1;

    cout << "ProgThreadFProm::main thread is " << std::this_thread::get_id() << endl;

    cout << "main thread asks child thread to calculate factorial of " << 5 << " using cond var" << endl;
    // we should use the construction below to guarantee that child thread will
    // calculate res first and only after that it will be used by main thread
    std::thread t1(factorial, 5, std::ref(res));

    {
        std::unique_lock<std::mutex> locker(muFp);
        cvarFp.wait(locker);
    }
    cout << "Main thread. Received result using cond_var is " << res << endl;

    // but we have easier way to get res before it can be used by main thread.
    // by default async() doesn't create new thread. New thread can be created
    // inside.
    // async() function is called immediatelly here.
    cout << "main thread asks fu to calculate factorial of " << 4 << " using default fu" << endl;
    std::future<int> fu = std::async(factorial_fu, 4);
    cout << "Main thread. Check that fu was already calculated before this trace" << endl;
    res = fu.get();
    cout << "Main thread. fu.get(). Received result by future is " << res << endl;

    // The same situation. asynch function is called in the same thread but
    // it's deferred untill fu.get() is called
    cout << "main thread asks fu to calculate factorial of " << 3 << " using deferred fu" << endl;
    std::future<int> fu1 = std::async(std::launch::deferred, factorial_fu, 3);
    cout << "Main thread. Check that fu was not calculated yet for deferred option" << endl;
    res = fu1.get();
    cout << "Main thread. Received deferred result is " << res << endl;

    // here we call async with a request to create a separate thread to executed factorial_fu
    // 
    cout << "main thread asks fu to calculate factorial of " << 2 << " using fu executed by other thread" << endl;
    std::future<int> fu2 = std::async(std::launch::async, factorial_fu, 2);
    cout << "Main thread. Check that fu was already calculated before fu.get() is called" << endl;
    res = fu2.get();
    cout << "Main thread. Received result by future is " << res << endl;
    
    // we can combine two flags: std::launch::async | std::launch::deferred

    //res = fu.get() CRASH! get() is only available once!


    cout << "-- let's check PROMISE --" << endl;
    // promises are used to communicate with child thread.
    std::promise<int> p;
    std::future<int> fu_p = p.get_future();
    cout << "Main thread. Before factorial call with promises " << endl;
    std::future<int> fu_fp = std::async(std::launch::async, factorial_prom, std::ref(fu_p));
    cout << "Main thread. After factorial call with promises " << endl;
    p.set_value(6);
    res = fu_fp.get();
    cout << "Main thread. Received result by future with promises is " << res << endl;
    
    // Please note that if we have factorial function to be executed by many child threads with
    // own futures then I don't always need to create own promises for each future.
    std::promise<int> p1;
    std::future<int> fu_p1 = p1.get_future();
    std::shared_future<int> shf = fu_p1.share();
    
    // I can't just pass ref(fu_p) to each async because .get() inside can be called only one time!
    // So, instead of creating additional futures for each async we can use shared futures.
    // SHARED FUTURES CAN BE PASSED BY VALUE (not always by ref or moved)!!!

    std::future<int> fu_fp1 = std::async(std::launch::async, factorial_prom2, shf);
    std::future<int> fu_fp2 = std::async(std::launch::async, factorial_prom2, shf);
    std::future<int> fu_fp3 = std::async(std::launch::async, factorial_prom2, shf);
    //... 10 threads

    p1.set_value(7);
    
    cout << "Main thread. Received results by multiple futures with promises are " << fu_fp1.get() << " " << fu_fp2.get() << " " << fu_fp3.get() << " " << endl;

    t1.join();
}
