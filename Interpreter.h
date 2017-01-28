#ifndef _INTERPRETER_h
#define _INTERPRETER_h

#include <Arduino.h>
#include "Regexp.h"

template<class T, void(T::*PTR)(const char *, const unsigned int, const MatchState &), size_t I>
struct bind_regex_member
{
	typedef void(*fn_type)(const char *, const unsigned int, const MatchState &);
	explicit bind_regex_member(T* _ptr)
	{
        	ptr = _ptr;
    	}
        static void func(const char * match, const unsigned int length, const MatchState & ms)
        {
                (ptr->*PTR)(match, length, ms);
        }
        operator fn_type()
        {
                return &func;
        }
private:
        static T*  ptr;
};

template<class T, void(T::*PTR)(const char *, const unsigned int, const MatchState &), size_t  I>
T* bind_regex_member<T, PTR, I>::ptr = NULL;

class InterpreterClass
{
 public:
	void init();
	void MatchAddressCallback(const char * match, const unsigned int length, const MatchState & ms);
};

extern InterpreterClass Interpreter;

#endif

