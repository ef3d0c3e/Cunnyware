#ifndef SDK_CUTL_MEMORYALIGNED_HPP
#define SDK_CUTL_MEMORYALIGNED_HPP

#include "Vector.hpp"

template <class T, i32 Alignment>
class CUtlMemoryAligned	: public CUtlMemory<T>
{

};

#endif // SDK_CUTL_MEMORYALIGNED_HPP
