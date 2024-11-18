#include <iostream>
#include <source_location>
#include <string>
#include <string_view>
enum class Err_Code
{
    err_delnode,
    err_delchild,
    err_atback,
    err_aschild,
    err_set,
    err_checkrepeat,
    err_getidx,
    err_variant,
    err_nofix_del,
    err_op,
    err_find,
    err_parse,
    err_serial
};
#define STR(x)     \
    case x:        \
        return #x; \
        break;
inline constexpr auto penum_err(Err_Code e)
{
    switch (e)
    {
        STR(Err_Code::err_aschild)
        STR(Err_Code::err_atback)
        STR(Err_Code::err_delchild)
        STR(Err_Code::err_delnode)
        STR(Err_Code::err_checkrepeat)
        STR(Err_Code::err_find)
        STR(Err_Code::err_getidx)
        STR(Err_Code::err_op)
        STR(Err_Code::err_nofix_del)
        STR(Err_Code::err_set)
        STR(Err_Code::err_variant)
        STR(Err_Code::err_parse)
        STR(Err_Code::err_serial)
        default:
            // TODO
            break;
    }
    return "------------------";
}

template <Err_Code C>
struct error : public std::exception
{
    std::string ret;
    error(const char* fmsg, const int lmsg, const char* msg)
    {
        ret.append("[Error! Func:");
        ret.append(fmsg);
        ret.append(" in Line:");
        ret.append(std::to_string(lmsg));
        ret.append("],");
        ret.append("[Msg:");
        ret.append(msg);
        ret.append(",ErrorCode:");
        ret.append(penum_err(C));
        ret.append("]");
    }
    const char* what() const throw()
    {
        return this->ret.c_str();
    }
};
