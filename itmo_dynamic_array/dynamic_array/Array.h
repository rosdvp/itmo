#pragma once

// Forward declare the class template
template<typename T>
class Array;


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

	Array<T>& operator=(const Array<T>& other); //although, copy-swap is used, stay for self-assignment optimization
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


	//---------Fast Iterator--------------
	class FastConstIter
	{
	public:
		FastConstIter(const Array* array, int idx);
		FastConstIter& operator++();
		FastConstIter& operator++(int);
		FastConstIter& operator--();
		FastConstIter& operator--(int);
		FastConstIter& operator+=(const FastConstIter& other);
		FastConstIter& operator-=(const FastConstIter& other);
		FastConstIter& operator+=(int idx);
		FastConstIter& operator-=(int idx);
		FastConstIter operator+(const FastConstIter& other) const;
		FastConstIter operator-(const FastConstIter& other) const;
		FastConstIter operator+(int idx) const;
		FastConstIter operator-(int idx) const;
		const T& operator*() const;
		bool operator==(const FastConstIter& other) const;
		bool operator!=(const FastConstIter& other) const;
		operator int() const;
	protected:
		int _idx;
	private:
		const Array<T>* _array;
	};

	class FastIter : public FastConstIter
	{
	public:
		FastIter(Array* array, int idx);
		FastIter operator+(const FastIter& other) const;
		FastIter operator-(const FastIter& other) const;
		FastIter operator+(int idx) const;
		FastIter operator-(int idx) const;
		const T& operator*() const;
		T& operator*();
	private:
		Array* _array;
	};

	FastIter begin();				//name required by for-range
	FastIter end();					//name required by for-range
	FastConstIter begin() const;	//name required by for-range
	FastConstIter end() const;		//name required by for-range
	FastConstIter cbegin() const;	//name required by for-range (actually not, but the name is used for integrity)
	FastConstIter cend() const;		//name required by for-range (actually not, but the name is used for integrity)


	//---------For testing purpose--------------
	int GetCapacity() const;
	const T* GetItemsPointer() const;

private:
	static constexpr int DEFAULT_CAPACITY = 8;
	static constexpr float CAPACITY_EXPAND_K = 2;
	static constexpr bool IS_ITEMS_MOVABLE = std::is_move_constructible<T>::value;

	int _capacity;
	int _size;

	T* _items;

	void IncreaseCapacity();
	void TryDestructAndFreeItems();
	void SwapArray(Array<T>& other) noexcept;

	//---------Metaprogramming magic--------------
	/**
	 * \param dest MUST be constructed.
	 */
	template<typename U = T>
	std::enable_if_t<std::is_move_assignable<U>::value || std::is_move_constructible<U>::value>
	ReplaceByMoveOrCopy(T* source, T* dest);

	/**
	 * \param dest MUST be constructed.
	 */
	template<typename U = T>
	std::enable_if_t<!std::is_move_assignable<U>::value && !std::is_move_constructible<U>::value>
	ReplaceByMoveOrCopy(T* source, T* dest);

	/**
	* \param dest MUST be constructed.
	*/
	template<typename U = T>
	std::enable_if_t<std::is_move_assignable<U>::value>
	ReplaceByMove(T* source, T* dest);

	/**
	* \param dest MUST be constructed.
	*/
	template<typename U = T>
	std::enable_if_t<!std::is_move_assignable<U>::value>
	ReplaceByMove(T* source, T* dest);

	/**
	* \param dest MUST be constructed.
	*/
	template<typename U = T> 
	std::enable_if_t<std::is_copy_assignable<U>::value>
	ReplaceByCopy(const T* source, T* dest);

	/**
	* \param dest MUST be constructed.
	*/
	template<typename U = T>
	std::enable_if_t<!std::is_copy_assignable<U>::value>
	ReplaceByCopy(const T* source, T* dest);

	/**
	 * \param dest MUST NOT be constructed
	 */
	template<typename U = T>
	std::enable_if_t<std::is_move_constructible<U>::value>
	ConstructByMoveOrCopy(T& source, T* dest);

	/**
	 * \param dest MUST NOT be constructed
	 */
	template<typename U = T>
	std::enable_if_t<!std::is_move_constructible<U>::value>
	ConstructByMoveOrCopy(T& source, T* dest);
};

#include "Array.tpp"
#include "Array.Iterator.tpp"
#include "Array.FastIterator.tpp"