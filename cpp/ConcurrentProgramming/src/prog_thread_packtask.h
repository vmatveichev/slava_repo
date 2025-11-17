#include "prog.h"

class ProgThreadPackTask
{
    public:
        void main();
};

int factorial_pt(int num)
{
    int res = 1;
    for (int i = num; i > 1; i--)
        res *= i;

    cout << std::this_thread::get_id() << ": Factorial result calculated for " << num << " is " << res << endl;
    
    return res;
}

std::deque<std::packaged_task<int()>> pt_d;
std::mutex mu_pt;
std::condition_variable cond_pt;

void thread_1()
{
    cout << std::this_thread::get_id() << ": thread_1 start" << endl;
    
    std::packaged_task<int()> t;
    {
        std::unique_lock<std::mutex> locker(mu_pt);
        cond_pt.wait(locker, [](){return !pt_d.empty();});
        t = std::move(pt_d.front());
        pt_d.pop_front();
    }
    t();
    cout << std::this_thread::get_id() << ": thread_1 end" << endl;
}

void ProgThreadPackTask::main()
{
    cout << std::this_thread::get_id() << ": ProgThreadPackTask::main" << endl;
    std::packaged_task<int(int)> pt(factorial_pt);

    //.... something can happen here

    pt(6); // can even be called in different context. can be called only one time for each task
    // int x = pt(6) <-can't be called because t always returns void. The following way is possible:
    int x = pt.get_future().get();
    cout << std::this_thread::get_id() << ": x = " << x << endl;

    // std::packaged_task<int(int)> pt1(factorial_pt, 6); <- we can't use attributes for factorial function here.
    // we case specify it in the following way (please note that we don't specify incoming params here <int()>):
    std::packaged_task<int()> pt1(std::bind(factorial_pt, 7));
    pt1();
    x = pt1.get_future().get();
    cout << std::this_thread::get_id() << ": x = " << x << endl;
    
    // I can also avoid packaged tasks
    cout << std::this_thread::get_id() << ": before std::bind(factorial_pt, 8)" << endl;
    auto b = std::bind(factorial_pt, 8); // b is funciton object here
    b();
    
    // but comparing to function objects the packaged tasks can be used later even in other thread
    cout << std::this_thread::get_id() << ": creating of deque of packaged tasks" << endl;
    std::thread th1(thread_1);

    std::packaged_task<int()> pt2(std::bind(factorial_pt, 9));
    std::future<int> fu = pt2.get_future();
    {
        std::lock_guard<std::mutex> locker(mu_pt);
        pt_d.push_back(std::move(pt2));
    }
    cond_pt.notify_one();
    
    cout << std::this_thread::get_id() << ": received value of packaged task result is " << fu.get() << endl;

    th1.join();
}
