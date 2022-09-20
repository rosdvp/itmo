#pragma once
#include <algorithm>
#include <cstdlib>

#include "Array.h"

template <typename T>
Array<T>::Array(): Array(DEFAULT_CAPACITY) { }

template <typename T>
Array<T>::Array(const int capacity)
{
	_capacity = capacity;
	_size     = 0;
	_values   = static_cast<T*>(malloc(sizeof(T) * capacity));
}

template <typename T>
Array<T>::Array(const Array<T>& other)
{
	_capacity = other._capacity;
	_size     = other._size;
	_values   = static_cast<T*>(malloc(sizeof(T) * _capacity));
	memcpy(_values, other._values, sizeof(T) * _size);
}

template <typename T>
Array<T>::Array(Array<T>&& other) noexcept
{
	_capacity     = other._capacity;
	_size         = other._size;
	_values       = other._values;
	other._values = nullptr;
}

template <typename T>
Array<T>::~Array()
{
	FreeValues();
}

template <typename T>
Array<T>& Array<T>::operator=(const Array<T>& other)
{
	if (this != &other)
	{
		FreeValues();
		_capacity = other._capacity;
		_size     = other._size;
		_values   = static_cast<T*>(malloc(sizeof(T) * _capacity));
		memcpy(_values, other._values, sizeof(T) * _size);
	}
	return *this;
}

template <typename T>
Array<T>& Array<T>::operator=(Array<T>&& other) noexcept
{
	if (this != &other)
	{
		FreeValues();
		_capacity     = other._capacity;
		_size         = other._size;
		_values       = other._values;
		other._values = nullptr;
	}
	return *this;
}

template <typename T>
void Array<T>::Insert(const T& value)
{
	Insert(_size, value);
}

template <typename T>
void Array<T>::Insert(int index, const T& value)
{
	if (_size == _capacity)
		IncreaseCapacity();
	
	for (int i = _size; i > index; i--)
		ReplaceWithCopyOrMove(_values + i - 1, _values + i);
	_size++;

	_values[index] = value;
}

template <typename T>
void Array<T>::Remove(int index)
{
	_values[index].~T();
	_size--;
	for (int i = index; i < _size; i++)
		ReplaceWithCopyOrMove(_values + i + 1, _values + i);
}

template <typename T>
const T& Array<T>::operator[](int index) const
{
	return _values[index];
}

template <typename T>
T& Array<T>::operator[](int index)
{
	return _values[index];
}

template <typename T>
int Array<T>::Size() const
{
	return _size;
}


template <typename T>
typename Array<T>::Iterator Array<T>::Iter()
{
	return Iterator(this, 0, _size);
}

template <typename T>
typename Array<T>::ConstIterator Array<T>::Iter() const
{
	return ConstIterator(this, 0, _size);
}

template <typename T>
typename Array<T>::Iterator Array<T>::ReverseIter()
{
	return Iterator(this, _size - 1, -1);
}

template <typename T>
typename Array<T>::ConstIterator Array<T>::ReverseIter() const
{
	return ConstIterator(this, _size - 1, -1);
}

template <typename T>
typename Array<T>::Iterator Array<T>::begin()
{
	return Iter();
}

template <typename T>
typename Array<T>::Iterator Array<T>::end()
{
	return Iterator(this, _size, _size);
}

template <typename T>
typename Array<T>::ConstIterator Array<T>::begin() const
{
	return cbegin();
}

template <typename T>
typename Array<T>::ConstIterator Array<T>::end() const
{
	return cend();
}

template <typename T>
typename Array<T>::ConstIterator Array<T>::cbegin() const
{
	return Iter();
}

template <typename T>
typename Array<T>::ConstIterator Array<T>::cend() const
{
	return ConstIterator(this, _size, _size);
}


template <typename T>
int Array<T>::GetCapacity() const
{
	return _capacity;
}

template <typename T>
const T* Array<T>::GetValuesPointer() const
{
	return _values;
}

template <typename T>
void Array<T>::IncreaseCapacity()
{
	_capacity    = static_cast<int>(_capacity * CAPACITY_EXPAND_K);
	T* newValues = static_cast<T*>(malloc(_capacity * sizeof(T)));

	for (int i = 0; i < _size; i++)
		ReplaceWithCopyOrMove(_values + i, newValues + i);

	free(_values);
	_values = newValues;
}


template <typename T>
void Array<T>::FreeValues()
{
	if (_values != nullptr)
	{
		for (int i = 0; i < _size; i++)
			_values[i].~T();
		free(_values);
	}
}

template <typename T>
template <typename U, typename>
void Array<T>::ReplaceWithCopyOrMove(T* source, T* dest, Choice<0>)
{
	*dest = std::move(*source);
}

template <typename T>
template <typename U, typename>
void Array<T>::ReplaceWithCopyOrMove(T* source, T* dest, Choice<1>)
{
	*dest = T(std::move(*source));
}

template <typename T>
template <typename U, typename>
void Array<T>::ReplaceWithCopyOrMove(T* source, T* dest, Choice<2>)
{
	*dest = *source;
}

template <typename T>
template <typename U, typename>
void Array<T>::ReplaceWithCopyOrMove(T* source, T* dest, Choice<3>)
{
	*dest = T(*source);
}
