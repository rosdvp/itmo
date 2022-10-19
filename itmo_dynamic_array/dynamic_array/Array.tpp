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
	_items   = static_cast<T*>(malloc(sizeof(T) * capacity));
	for (int i = 0; i < _capacity; i++)
		new(_items + i) T();
}

template <typename T>
Array<T>::Array(const Array<T>& other)
{
	_capacity = other._capacity;
	_size     = other._size;
	_items   = static_cast<T*>(malloc(sizeof(T) * _capacity));
	for (int i = 0; i < _capacity; i++)
		new(_items + i) T(other._items[i]);
}

template <typename T>
Array<T>::Array(Array<T>&& other) noexcept
{
	SwapArray(other);
}

template <typename T>
Array<T>::~Array()
{
	TryDestructAndFreeItems();
}

template <typename T>
Array<T>& Array<T>::operator=(const Array<T>& other)
{
	if (this != &other)
	{
		Array<T> temp(other);
		SwapArray(temp);
	}
	return *this;
}

template <typename T>
Array<T>& Array<T>::operator=(Array<T>&& other) noexcept
{
	if (this != &other)
		SwapArray(other);
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
		ReplaceByMoveOrCopy(_items + i - 1, _items + i);
	_size++;
	
	ReplaceByCopy(&value, _items + index);
}

template <typename T>
void Array<T>::Remove(int index)
{
	_size -= 1;
	for (int i = index; i < _size; i++)
		ReplaceByMoveOrCopy(_items + i + 1, _items + i);
}

template <typename T>
const T& Array<T>::operator[](int index) const
{
	return _items[index];
}

template <typename T>
T& Array<T>::operator[](int index)
{
	return _items[index];
}

template <typename T>
int Array<T>::Size() const
{
	return _size;
}


template <typename T>
int Array<T>::GetCapacity() const
{
	return _capacity;
}

template <typename T>
const T* Array<T>::GetValuesPointer() const
{
	return _items;
}

template <typename T>
void Array<T>::IncreaseCapacity()
{
	int newCapacity = static_cast<int>(_capacity * CAPACITY_EXPAND_K);
	T* newItems = static_cast<T*>(malloc(newCapacity * sizeof(T)));
	RearrangeAllItemsByMoveOrCopy(newItems, _capacity);
	for (int i = _capacity; i < newCapacity; i++)
		new(newItems + i) T();
	_capacity = newCapacity;
	_items = newItems;
}


template <typename T>
void Array<T>::TryDestructAndFreeItems()
{
	if (_items != nullptr)
	{
		for (int i = 0; i < _capacity; i++)
			_items[i].~T();
		free(_items);
	}
}

template <typename T>
void Array<T>::SwapArray(Array<T>& other) noexcept
{
	std::swap(_capacity, other._capacity);
	std::swap(_size, other._size);
	_items = other._items;
	other._items = nullptr;
}

template <typename T>
template <typename U>
std::enable_if_t<std::is_move_constructible<U>::value> Array<T>::
RearrangeAllItemsByMoveOrCopy(T* dest, int count)
{
	for (int i = 0; i < count; i++)
		new(dest + i) T(std::move(_items[i]));
	free(_items);
}

template <typename T>
template <typename U>
std::enable_if_t<!std::is_move_constructible<U>::value> Array<T>::
RearrangeAllItemsByMoveOrCopy(T* dest, int count)
{
	for (int i = 0; i < count; i++)
		new(dest + i) T(_items[i]);
	TryDestructAndFreeItems();
}


template <typename T>
template <typename U>
std::enable_if_t<std::is_move_assignable<U>::value || std::is_move_constructible<U>::value>
Array<T>::ReplaceByMoveOrCopy(T* source, T* dest)
{
	ReplaceByMove(source, dest);
}

template <typename T>
template <typename U>
std::enable_if_t<!std::is_move_assignable<U>::value && !std::is_move_constructible<U>::value>
Array<T>::ReplaceByMoveOrCopy(T* source, T* dest)
{
	ReplaceByCopy(source, dest);
}

template <typename T>
template <typename U>
std::enable_if_t<std::is_move_assignable<U>::value>
Array<T>::ReplaceByMove(T* source, T* dest)
{
	*dest = std::move(*source);
}

template <typename T>
template <typename U>
std::enable_if_t<!std::is_move_assignable<U>::value>
Array<T>::ReplaceByMove(T* source, T* dest)
{
	(*dest).~T();
	new(dest) T(std::move(*source));
}

template <typename T>
template <typename U>
std::enable_if_t<std::is_copy_assignable<U>::value>
Array<T>::ReplaceByCopy(const T* source, T* dest)
{
	*dest = *source;
}

template <typename T>
template <typename U>
std::enable_if_t<!std::is_copy_assignable<U>::value>
Array<T>::ReplaceByCopy(const T* source, T* dest)
{
	(*dest).~T();
	new (dest) T(source);
}



template <typename T>
typename Array<T>::Iterator Array<T>::Iter() { return Iterator(this, 0, _size); }

template <typename T>
typename Array<T>::ConstIterator Array<T>::Iter() const { return ConstIterator(this, 0, _size); }

template <typename T>
typename Array<T>::Iterator Array<T>::ReverseIter() { return Iterator(this, _size - 1, -1); }

template <typename T>
typename Array<T>::ConstIterator Array<T>::ReverseIter() const { return ConstIterator(this, _size - 1, -1); }


template <typename T>
typename Array<T>::FastIter Array<T>::begin() { return FastIter(this, 0); }

template <typename T>
typename Array<T>::FastIter Array<T>::end() { return FastIter(this, _size); }

template <typename T>
typename Array<T>::FastConstIter Array<T>::begin() const { return FastConstIter(this, 0); }

template <typename T>
typename Array<T>::FastConstIter Array<T>::end() const { return FastConstIter(this, _size); }

template <typename T>
typename Array<T>::FastConstIter Array<T>::cbegin() const { return FastConstIter(this, 0); }

template <typename T>
typename Array<T>::FastConstIter Array<T>::cend() const { return FastConstIter(this, _size); }
