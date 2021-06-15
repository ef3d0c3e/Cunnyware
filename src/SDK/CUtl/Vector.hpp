#ifndef SDK_CUTL_VECTOR_HPP
#define SDK_CUTL_VECTOR_HPP

#include "Memory.hpp"

template <class T, class A = CUtlMemory<T>>
class CUtlVector : non_constructible
{
	typedef A CAllocator;

public:
	typedef T ElemType;

protected:
	// Can't copy this unless we explicitly do it!
	CUtlVector(CUtlVector const& vec)
	{
		[]<bool v = false>() { static_assert(v, "Cannot copy!"); }();
	}

	CAllocator memory;
	i32 size;

	// For easier access to the elements through the debugger
	// it's in release builds so this can be used in libraries correctly
	T* elements;
};

#endif // SDK_CUTL_VECTOR_HPP
