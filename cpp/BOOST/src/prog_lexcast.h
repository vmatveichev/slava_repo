#include "prog.h"
#include <sstream>
#include <boost/lexical_cast.hpp>

class ProgBoostLexCast
{
public:
    void main();
};

using boost::lexical_cast;
using boost::bad_lexical_cast;

void ProgBoostLexCast::main()
{
    cout << "ProgBoostLexCast::start" << endl;
    
    // we have many functions for converting from string to numbers (int, long, double etc...)
    // and they are named differently: atoi, atof, atol, strtod, strtof, strtoll...
    // also we have a method to convert anything to string like:
    stringstream strm;
    int a = 555;
    strm << a;
    string str = strm.str();
    cout << "resulting string constructed by stringstream: " << str << endl;
    
    // But using sstream for converting of number to string is too inefficient
    // we can use old C functions sprintf(), sscanf() but it's also inconvenient
    // we also have itoa() function  but it's not standard
    
    // so, we are going to use boost library which is both convenient and efficient
    // do not forget to call using boost::lexical_cast above

    str = "Message: " + lexical_cast<string>('A') + "=" + lexical_cast<string>(34.5);
    cout << "resulting string constructing with lexical cast: " << str << endl;
    
    // and we can cast to numbers:
    int b = lexical_cast<int>("123456");
    cout << "resulting number constructing with lexical cast: " << b << endl;
    
    // we can catch bad_lexical_cast
    // do not forget to call using boost::bad_lexical_cast above
    try
    {
        int c = lexical_cast<int>("123456.54321");
    }
    catch (bad_lexical_cast& e)
    {
        cout << "Exception caught: " << e.what() << endl;
    }
    
    //but we can call lexical_cast with exact number of symbols for cast
    cout << "resulting number: " << lexical_cast<int>("123abc", 3) << endl;
 
    cout << "ProgBoostLexCast::end" << endl;
}
