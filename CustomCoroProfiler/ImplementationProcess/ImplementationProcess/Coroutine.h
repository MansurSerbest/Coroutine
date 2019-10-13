//=========================================================
// Graduation Work 2018 - 2019
// Digital arts and Entertainment: Game Development
// Mansur Serbest
//=========================================================

#pragma once
#include <boost/context/fiber.hpp>
#include <cstdarg>
#include <iostream>
#include <functional>

namespace ctx = boost::context;

template <typename T, typename ...Args>
class Coroutine;


template<typename T>
class Caller
{
	static_assert(!(std::is_pointer<T>::value), "Caller<>: No pointers template types are allowed!");

public:
	Caller()
		:m_Fiber{}, m_Value{}
	{

	}

	Caller(T& value)
		:m_Value{ value }
	{
		
	}

	Caller(ctx::fiber&& f, T& value)
		:m_Value{ value }
	{
		m_Fiber = std::move(f);
	}

	~Caller()
	{
		//Don't delete coroutine here, because coroutine lifetime is handled within context where coroutine is created
	}

	void SetFiber(ctx::fiber&& f)
	{
		m_Fiber = std::move(f);
	}

	ctx::fiber&& GetFiber()
	{
		return std::move(m_Fiber);
	}

	void operator()()
	{
		//Suspend without value
		if (m_Fiber)
			m_Fiber = std::move(m_Fiber).resume();
	}

	void operator()(T value)
	{
		//Suspend with value
		if (m_Fiber)
		{
			m_Value = value;
			m_Fiber = std::move(m_Fiber).resume();
		}
	}

private:
	ctx::fiber m_Fiber;
	T& m_Value;
};

template<typename T, typename ...Args>
class Coroutine
{
	//Check if template type is pointer. If so, assert the compilation: no pointers are allowed!
	static_assert(!(std::is_pointer<T>::value), "Coroutine<>: No pointer template types are allowed!");

public:
	Coroutine()
		:m_Fiber{}, m_Caller{ m_Value }, m_Value{}, m_FuncPtr{}
	{

	}

	//After the function argument, all the argument that needs to be passed to 'function'
	//needs to be captured and passed to the 'function' when called in the constructor.
	//Use Variadic Functions
	template<typename Func, typename ...Args>
	Coroutine(Func func, Args... args)
		:m_Value{}, m_Fiber{}, m_Caller{m_Value}, m_FuncPtr{func}
	{
		//Try to execute the Created function in the function that will be used by the fiber
		auto plugFunc = [func, this, args...](ctx::fiber&& f)
		{
			//Pass the m_Caller as a reference to the function and set the fiber of m_Caller to f
			this->GetCaller().SetFiber(std::move(f));
			func(this->GetCaller(), args...);
			return this->GetCaller().GetFiber(); //You need to get the fiber inside the m_caller, because we put it there!
		};

		ctx::fiber f{ plugFunc };
		m_Fiber = std::move(f);
	};

	void Resume()
	{
		if (m_Fiber)
		{
			m_Fiber = std::move(m_Fiber).resume();
			if (!m_Fiber)
			{
				m_Fiber = ctx::fiber{};
			}
		}
	}

	template<typename ...Args>
	void Reset(Args... args)
	{
		auto plugFunc = [this, args...](ctx::fiber&& f)
		{
			this->GetCaller().SetFiber(std::move(f));	
			this->m_FuncPtr(this->GetCaller(), args...);
			return this->GetCaller().GetFiber();
		};

		ctx::fiber tempF{ plugFunc };
		m_Fiber = std::move(tempF);
	}

	bool IsValid()
	{
		if (m_Fiber)
			return true;
		return false;
	}

	Caller<T>& GetCaller()
	{
		return m_Caller;
	}

	T& GetValue()
	{
		return m_Value;
	}

	void SetValue(T value)
	{
		m_Value = value;
	}

	//Deleted and Custom
	Coroutine(const Coroutine& coro) = delete;
	Coroutine& operator=(const Coroutine& coro) = delete;
	Coroutine& operator=(Coroutine &&coro) = delete;
	Coroutine(Coroutine &&coro) = delete;

private:
	ctx::fiber m_Fiber;
	T m_Value;
	Caller<T> m_Caller;
	std::function<void(Caller<T>&, Args...)> m_FuncPtr;
};
