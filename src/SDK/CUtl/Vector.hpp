#ifndef SDK_CUTL_VECTOR_HPP
#define SDK_CUTL_VECTOR_HPP

#include "Memory.hpp"

template <class T, class A = CUtlMemory<T>>
class CUtlVector
{
	typedef A CAllocator;

public:
	typedef T ElemType;

public:
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
public:

	[[nodiscard]] const T& operator[](std::size_t i) const noexcept
	{ return elements[i]; }
	[[nodiscard]] T& operator[](std::size_t i) noexcept
	{ return elements[i]; }
	[[nodiscard]] i32 Size() const noexcept
	{ return size; }
};

#endif // SDK_CUTL_VECTOR_HPP
