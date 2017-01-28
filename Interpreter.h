#ifndef _INTERPRETER_h
#define _INTERPRETER_h

#include <Arduino.h>
#include "Regexp.h"

template<class T, void(T::*PTR)(GlobalMatchCallback), size_t I>
struct bind_regex_member
{
	typedef void(*fn_type)(GlobalMatchCallback);
	explicit bind_regex_member(const T* _ptr)
	{
		ptr = _ptr;
	}
	static void func(GlobalMatchCallback f)
	{
		(ptr->*PTR)(f);
	}
	operator fn_type()
	{
		return &func;
	}
private:
	static const T*  ptr;
};

template<class T, void(T::*PTR)(GlobalMatchCallback), size_t I>
const T* bind_regex_member<T, PTR, I>::ptr = NULL;

class InterpreterClass
{
 public:
	void init();
	GlobalMatchCallback MatchAddressCallback;
};

extern InterpreterClass Interpreter;

#endif

