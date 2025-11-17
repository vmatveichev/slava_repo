#include "prog_thread.h"
#include "prog_thread_mu.h"
#include "prog_thread_dlock.h"
#include "prog_thread_ulock.h"
#include "prog_thread_cvar.h"
#include "prog_thread_fprom.h"
#include "prog_thread_calobj.h"
#include "prog_thread_packtask.h"

int main()
{
    cout << "\nHI SLAVA!!!" << endl;
    cout <<"\n==============  THREAD MANAGEMENT (+ Exceptions) ==============\n";
    ProgThread pt;
    try
    {
        pt.main();
    }
    catch (myException ex)
    {
        cout << "exception " << ex.getStr() << " is caught" << endl;
    }
    
    cout <<"\n==============  DATA RACE and MUTEX (+ files) ==============\n";
    ProgThreadMu ptMu;
    ptMu.main();    

    cout <<"\n==============  DEADLOCK ==============\n";
    ProgThreadDLock ptDl;
    ptDl.main();    

    cout <<"\n==============  UNIQUE LOCK AND LAZY INITIALIZATION (+ std::call_once with lambda) ==============\n";
    ProgThreadULock ptUl;
    ptUl.main();    

    cout <<"\n==============  CONDITIONAL VARIABLES ==============\n";
    ProgThreadCondVar ptCv;
    ptCv.main();    

    cout <<"\n==============  FUTURE, PROMISE and async() ==============\n";
    ProgThreadFProm ptFp;
    ptFp.main();    

    cout <<"\n==============  CALLABLE OBJECTS (+ constructors) ==============\n";
    ProgThreadCalObj ptCo;
    ptCo.main();    

    cout <<"\n==============  PACKAGED TASK ==============\n";
    ProgThreadPackTask ptPt;
    ptPt.main();    
}
