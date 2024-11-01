#include <iostream>
#include <source_location>
#include <string_view>

template<class T>
struct error{
    error(const char* fmsg,const int lmsg,const char* msg,bool is_throw=true){
        if (auto p = static_cast<T*>(this)) {
            std::cerr<<"[Error! In Func:"<<fmsg<<"] in Line:"<<lmsg<<"\n";
            std::cerr<<"[More Info:"<<msg<<"]\n";
            if (is_throw) {
                p->throw_this();
            }
            
        }
    }
};
#define NewERRType(X) struct X : error<X>{ \
    void throw_this(){ \
        throw this; \
    } \
};
NewERRType(err_delnode)
NewERRType(err_delchild)
NewERRType(err_atback)
NewERRType(err_aschild)
NewERRType(err_set)
NewERRType(err_checkrepeat)
NewERRType(err_getidx)
NewERRType(err_variant)
NewERRType(err_nofix_del)
NewERRType(err_op)
NewERRType(err_find)
