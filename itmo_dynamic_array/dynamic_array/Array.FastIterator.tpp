#pragma once
#include "Array.h"

template <typename T>
Array<T>::FastConstIter::FastConstIter(const Array* array, const int idx)
	: _idx(idx), _array(array) {}

template <typename T>
Array<T>::FastConstIter::operator int() const
{
	return _idx;
}

template <typename T>
typename Array<T>::FastConstIter& Array<T>::FastConstIter::operator++()
{
	_idx += 1;
	return *this;
}

template <typename T>
typename Array<T>::FastConstIter& Array<T>::FastConstIter::operator++(int)
{
	FastConstIter buffer = *this;
	_idx += 1;
	return buffer;
}

template <typename T>
typename Array<T>::FastConstIter& Array<T>::FastConstIter::operator--()
{
	_idx -= 1;
	return *this;
}

template <typename T>
typename Array<T>::FastConstIter& Array<T>::FastConstIter::operator--(int)
{
	FastConstIter buffer = *this;
	_idx -= 1;
	return buffer;
}

template <typename T>
typename Array<T>::FastConstIter& Array<T>::FastConstIter::operator+=(const FastConstIter& other)
{
	_idx += other._idx;
	return *this;
}

template <typename T>
typename Array<T>::FastConstIter& Array<T>::FastConstIter::operator-=(const FastConstIter& other)
{
	_idx -= other._idx;
	return *this;
}

template <typename T>
typename Array<T>::FastConstIter& Array<T>::FastConstIter::operator+=(const int idx)
{
	_idx += idx;
	return *this;
}

template <typename T>
typename Array<T>::FastConstIter& Array<T>::FastConstIter::operator-=(const int idx)
{
	_idx -= idx;
	return *this;
}

template <typename T>
typename Array<T>::FastConstIter Array<T>::FastConstIter::operator+(const FastConstIter& other) const
{
	return FastConstIter(_array, _idx + other._idx);
}

template <typename T>
typename Array<T>::FastConstIter Array<T>::FastConstIter::operator-(const FastConstIter& other) const
{
	return FastConstIter(_array, _idx - other._idx);
}

template <typename T>
typename Array<T>::FastConstIter Array<T>::FastConstIter::operator+(const int idx) const
{
	return FastConstIter(_array, _idx + idx);
}

template <typename T>
typename Array<T>::FastConstIter Array<T>::FastConstIter::operator-(const int idx) const
{
	return FastConstIter(_array, _idx - idx);
}

template <typename T>
const T& Array<T>::FastConstIter::operator*() const
{
	return (*_array)[_idx];
}

template <typename T>
bool Array<T>::FastConstIter::operator==(const FastConstIter& other) const
{
	return _array == other._array && _idx == other._idx;
}

template <typename T>
bool Array<T>::FastConstIter::operator!=(const FastConstIter& other) const
{
	return !(*this == other);
}

template <typename T>
Array<T>::FastIter::FastIter(Array* array, int idx) : FastConstIter(array, idx)
{
	this->_array = array;
}

template <typename T>
typename Array<T>::FastIter Array<T>::FastIter::operator+(const FastIter& other) const
{
	return FastIter(_array, this->_idx + other._idx);
}

template <typename T>
typename Array<T>::FastIter Array<T>::FastIter::operator-(const FastIter& other) const
{
	return FastIter(_array, this->_idx - other._idx);
}

template <typename T>
typename Array<T>::FastIter Array<T>::FastIter::operator+(int idx) const
{
	return FastIter(_array, this->_idx + idx);
}

template <typename T>
typename Array<T>::FastIter Array<T>::FastIter::operator-(int idx) const
{
	return FastIter(_array, this->_idx - idx);
}

template <typename T>
const T& Array<T>::FastIter::operator*() const
{
	return (*_array)[this->_idx];
}

template <typename T>
T& Array<T>::FastIter::operator*()
{
	return (*_array)[this->_idx];
}
