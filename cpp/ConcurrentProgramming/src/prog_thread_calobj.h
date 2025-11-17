#include "prog.h"
#include <vector>
#include <algorithm>

std::mutex mu_co;
std::condition_variable cv_co;

class ProgThreadCalObj
{
    public:
        void main();
};

class A
{
public:
    A() { cout << std::this_thread::get_id() << ": I'm in constructor of A" << endl; }
    A(const A &) { cout << std::this_thread::get_id() << ": I'm in constructor of copy" << endl; }
    void f(int x, char c)
    {
        cout << std::this_thread::get_id() << ": I'm in method void f("<< x <<", "<< c <<")" << endl;
        cv_co.notify_all();
    }
    long g(double x) { cout << std::this_thread::get_id() << ": I'm in method long g("<< x << ")" << endl; return 0; }
    int operator()(int N)
    {
        
        cout << std::this_thread::get_id() << ": I'm in operator(" << N << ")" << endl;
        cv_co.notify_all();
        return 0;
    }
};

void foo(int x) { cout << std::this_thread::get_id() << ": I'm in foo(" << x << ")" << endl; cv_co.notify_all();}

void ProgThreadCalObj::main()
{
    cout << "ProgThreadCalObj::main" << endl;
    cout << std::this_thread::get_id() << ": main thread id" << endl;
    
    // as we can see in all cases below we point object and value and method a(int) is called. It means callable.
    // it's done for example in the following functions taking callable object as parameter:
    // std::bind(a, 6), std::async(std::launch::async, a, 6), std::call_once(once_flag, a, 6)
    
    std::vector<std::thread> threads;
    std::unique_lock<std::mutex> locker(mu_co);
    
    A a;
    cout << "before std::thread t1(a, 1)" <<endl;
    threads.push_back(std::thread(a, 1)); // creates a copy of a() in different thread
    //t1.join();
    cv_co.wait(locker);

    cout << "before t2(std::ref(a), 2)" <<endl;
    threads.push_back(std::thread(std::ref(a), 2)); // calls a() in different thread
    //t2.join();
    cv_co.wait(locker);

    cout << "before t3(A(), 3)" <<endl;
    threads.push_back(std::thread(A(), 3)); // creates temp A and new thread and moves this A to this new thread
    //t3.join();
    cv_co.wait(locker);

    cout << "before t4([](int x){return x*x;}, 4)" << endl;
    threads.push_back(std::thread([](int x){cv_co.notify_all();return x*x;}, 4)); // lambda
    //t4.join();
    cv_co.wait(locker);

    cout << "before t5(foo, 5)" <<endl;
    threads.push_back(std::thread(foo, 5));
    //t5.join();
    cv_co.wait(locker);

    cout << "before t6(&A::f, a, 6, 's')" <<endl;
    threads.push_back(std::thread(&A::f, a, 6, 's')); // creates copy of a and invokes a.f(6, 's') in different thread
    //t6.join();
    cv_co.wait(locker);

    cout << "before t7(&A::f, &a, 7, 'w')" <<endl;
    threads.push_back(std::thread(&A::f, &a, 7, 'w')); // just calls a.f(7, 'w') in different thread
    //t7.join();
    cv_co.wait(locker);

    cout << "before t8(std::move(a), 8)" <<endl;
    threads.push_back(std::thread(std::move(a), 8)); // moves a from main thread to child one and calls a(8) there. a is no longer usable in parent thread!
    //t8.join();
    cv_co.wait(locker);

    //std::for_each(threads.begin(), threads.end(), [](std::thread& t){t.join();}); <-one way
    // or another way:
    for(auto &t: threads)
        t.join();
}
