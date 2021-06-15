#ifndef SDK_ISPSHAREDMEMORY_HPP
#define SDK_ISPSHAREDMEMORY_HPP

#include "../Util/Defs.hpp"

class ISPSharedMemory
{
public:
	virtual bool Init(std::size_t size) = 0; //Initial implementation assumes the size is fixed/hardcoded, returns true if this call actually created the memory, false if it already existed
	virtual u8* Base() = 0;
	virtual std::size_t Size() = 0;

	virtual void AddRef() = 0;
	virtual void Release() = 0;
};

#endif // SDK_ISPSHAREDMEMORY_HPP
