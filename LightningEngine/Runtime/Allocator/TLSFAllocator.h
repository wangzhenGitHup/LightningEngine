#pragma once

#include <new>
#include "MemoryLabel.h"

//4×Ö½ÚÄÚ´æ¶ÔÆë
#define LIGHTNING_ENGINE_ALIGN 4
#define LIGHTNING_ENGINE_ADJUST_MEMORY_SIZE(size) ((size + (LIGHTNING_ENGINE_ALIGN - 1)) & (LIGHTNING_ENGINE_ALIGN - 1)

void* operator new(std::size_t size);
void* operator new(std::size_t size, MemoryLabel memID);
void* operator new[](std::size_t size, MemoryLabel memID);

#if LIGHTNING_ENGINE_IOS || LIGHTNING_ENGINE_ANDROID
#else
void operator delete[](void* ptr);
void operator delete(void* ptr);
#endif

void operator delete(void* ptr, MemoryLabel memID);
void operator delete[](void* ptr, MemoryLabel memID);