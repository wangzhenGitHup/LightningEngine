#pragma once

enum MemoryLabel
{
#define MEMORY_LABEL(Name) kMem##Name##Id,
	MEMORY_LABEL(Default)
	
#undef MEMORY_LABEL
	kMemNormalLabelCount,
	kMemLabelCount
};