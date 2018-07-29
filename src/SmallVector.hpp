#pragma once

#include <vector>

template <class T>
class	SmallVector
{
	static constexpr size_t _stackSize = 6;
	T _stack[_stackSize];
	std::vector<T>* _backup;
	size_t _size;
public:
	SmallVector()
	{
		_backup = nullptr;
		_size = 0;
	}
	~SmallVector()
	{
		delete _backup;
	}

	void	push_back(const T& item)
	{
		if (_size < _stackSize)
		{
			_stack[_size] = item;
			_size++;
		}
		else
		{
			_backup = new std::vector<T>;
			_backup->push_back(item);
			_size++;
		}
	}
	size_t	size() const
	{
		return _size;
	}
	
	T&	operator[](size_t index)
	{
		if (index < _stackSize)
			return _stack[index];
		return (*_backup)[index - _size];
	}
	const T&	operator[](size_t index) const
	{
		if (index < _stackSize)
			return _stack[index];
		return (*_backup)[index - _size];
	}	
};
