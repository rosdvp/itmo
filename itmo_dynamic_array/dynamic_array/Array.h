#pragma once

template<typename T>
class Array final
{
public:
	//---------Construction--------------
	Array();
	Array(int capacity);
	Array(const Array<T>& other);
	Array(Array<T>&& other) noexcept;

	~Array();

	Array<T>& operator=(const Array<T>& other);
	Array<T>& operator=(Array<T>&& other) noexcept;

	//---------Operations--------------
	void Insert(const T& value);
	void Insert(int index, const T& value);
	void Remove(int index);

	const T& operator[](int index) const;
	T& operator[](int index);

	int Size() const;

	//---------Iterator--------------
	class ConstIterator
	{
	public:
		ConstIterator(const Array<T>* array, int startIdx, int lastIdxExclusive);

		const T& Get() const;
		void Next();
		bool HasNext() const;

		ConstIterator& operator++();
		ConstIterator& operator++(int);
		const T& operator*();

		bool operator==(const ConstIterator& other) const;
		bool operator!=(const ConstIterator& other) const;
	protected:
		const int _idxStep;
		const int _lastIdxExclusive;
		int _idx;

	private:
		const Array<T>* const _array;
	};

	class Iterator : public ConstIterator
	{
	public:
		Iterator(Array* array, int startIdx, int lastIdxExclusive);

		void Set(const T& value);

		T& operator*();
	private:
		Array<T>* const _array;
	};

	Iterator Iter();
	ConstIterator Iter() const;
	Iterator ReverseIter();
	ConstIterator ReverseIter() const;

	Iterator begin();				//name required by for-range
	Iterator end();					//name required by for-range
	ConstIterator begin() const;	//name required by for-range
	ConstIterator end() const;		//name required by for-range
	ConstIterator cbegin() const;	//name required by for-range (actually not, but the name is used for integrity)
	ConstIterator cend() const;		//name required by for-range (actually not, but the name is used for integrity)


	//---------For testing purpose--------------
	int GetCapacity() const;
	const T* GetValuesPointer() const;

private:
	static constexpr int DEFAULT_CAPACITY = 8;
	static constexpr float CAPACITY_EXPAND_K = 2;

	int _capacity;
	int _size;

	T* _values;

	void IncreaseCapacity();
	void FreeValues();


	//---------Metaprogramming magic--------------

	template <int I> struct Choice : Choice<I + 1> { static_assert(I < 4, "Array can work only with movable or copyable type"); };
	template <> struct Choice<4> { };

	void ReplaceWithCopyOrMove(T* source, T* dest)
	{
		ReplaceWithCopyOrMove(source, dest, Choice<0>());
	}

	template<typename U = T, typename = std::enable_if_t<std::is_move_assignable<U>::value>>
	void ReplaceWithCopyOrMove(T* source, T* dest, Choice<0>);
	template<typename U = T, typename = std::enable_if_t<std::is_move_constructible<U>::value>>
	void ReplaceWithCopyOrMove(T* source, T* dest, Choice<1>);
	template<typename U = T, typename = std::enable_if_t<std::is_copy_assignable<U>::value>>
	void ReplaceWithCopyOrMove(T* source, T* dest, Choice<2>);
	template<typename U = T, typename = std::enable_if_t<std::is_copy_constructible<U>::value>>
	void ReplaceWithCopyOrMove(T* source, T* dest, Choice<3>);
};
#include "Array.tpp"
#include "Array.Iterator.tpp"