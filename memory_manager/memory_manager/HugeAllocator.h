#pragma once
class HugeAllocator
{
public:
	HugeAllocator();
	~HugeAllocator();

	void Init();
	void Destroy();

	void* Alloc(size_t size);
	void Free(void* p);

private:

#if _DEBUG
private:
	bool _isInitialized;
	int _currAllocatedCount;
#endif
};