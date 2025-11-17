#include "prog.h"

myException ex("!!! MY EXCEPTION !!!");
 
class ProgThread
{
    public:
        void main();
};

class Fctor
{
    public:
        void operator()(string& msg)
        {
            cout << "thread_f2 (id=" << std::this_thread::get_id() << "): " << msg << endl;
            msg += " ADDITIONAL STRING!!!";
        }
}; 

void thread_f1(int tmp)
{
    cout << "thread_f1: " << tmp << endl;
}

void ProgThread::main()
{
    cout << "ProgThread::main" << endl;
    cout << "ProgThread::main: hardware_concurrency = " << std::thread::hardware_concurrency() << endl << endl;
    
    std::thread t1(thread_f1, 15);
    //t1.join();
    //t1.detach();
    
    //Fctor fct;
    string s = "Hello, I'm Slava";
    std::thread t2((Fctor()), std::ref(s));
    std::thread t3 = std::move(t2);
//    t3.join();    
    try
    {
        cout << "main thread (id=" << std::this_thread::get_id() << "): " << s << endl;
        //throw ex;
    }
    catch (myException ex) 
    {
        t1.join();
//        t2.join();
        cout << ex.getStr() << endl;
        throw ex;
    }

    if (t1.joinable())
        t1.join();
    if (t3.joinable())
        t3.join();

}
