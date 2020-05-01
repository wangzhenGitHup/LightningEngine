#include "TLSFAllocator.h"
#include "tlsf/tlsf.h"

static unsigned long s_reservedTotalMemory = 50485760;
static unsigned int  s_reservedSize = 504857600; //500MB
static tlsf_t s_TLSF = nullptr;
static bool s_bAppQuited = false;

extern "C" void InitMemory()
{
	void* buffer = malloc(s_reservedSize);
	s_TLSF = tlsf_create_with_pool(buffer, s_reservedSize);
}

extern "C" void OnQuitMemory()
{
	s_bAppQuited = true;
}

static void* GetMemory(size_t size)
{
	if (nullptr == s_TLSF)
	{
		InitMemory();
	}

	return tlsf_malloc(s_TLSF, size);
}

static void Recycle(void* ptr)
{
	if (s_bAppQuited)
	{
		return;
	}

	tlsf_free(s_TLSF, ptr);
}

void* operator new(std::size_t size)
{
	void* ptr = GetMemory(size);
	return ptr;
}

void* operator new(std::size_t size, MemoryLabel memID)
{
	void* ptr = GetMemory(size);
	return ptr;
}

void* operator new[](std::size_t size, MemoryLabel memID)
{
	void* ptr = GetMemory(size);
	return ptr;
}

void operator delete(void* ptr, MemoryLabel memID)
{}

void operator delete[](void* ptr, MemoryLabel memID)
{}

void operator delete(void* ptr)
{
	Recycle(ptr);
}

void operator delete[](void* ptr)
{
	Recycle(ptr);
}