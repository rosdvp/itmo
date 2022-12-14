#pragma once
#include "Array.h"


template <typename T>
Array<T>::ConstIterator::ConstIterator(const Array<T>* array, const int startIdx, const int lastIdxExclusive)
	: _idxStep(lastIdxExclusive >= startIdx ? 1 : -1),
	_lastIdxExclusive(lastIdxExclusive),
	_idx(startIdx),
	_array(array) { }

template <typename T>
const T& Array<T>::ConstIterator::Get() const
{
	return (*_array)[_idx];
}

template <typename T>
void Array<T>::ConstIterator::Next()
{
	_idx += _idxStep;
}

template <typename T>
bool Array<T>::ConstIterator::HasNext() const
{
	return _idx != _lastIdxExclusive;
}


template <typename T>
Array<T>::Iterator::Iterator(Array<T>* array, const int startIdx, const int lastIdxExclusive)
	: ConstIterator::ConstIterator(array, startIdx, lastIdxExclusive),
	_array(array) { }

template <typename T>
void Array<T>::Iterator::Set(const T& value)
{
	(*_array)[Iterator::_idx] = value;
}

template <typename T>
T& Array<T>::Iterator::operator*()
{
	return (*_array)[Iterator::_idx];
}
