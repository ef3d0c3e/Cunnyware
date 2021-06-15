#ifndef SDK_CUTL_MEMORY_HPP
#define SDK_CUTL_MEMORY_HPP

#include "../../Util/Defs.hpp"

template <class T, class I = int>
class CUtlMemory : non_constructible
{
public:
	class Iterator
	{
	public:
		Iterator(I i)
		: index(i)
		{}
		I index;

		bool operator==(const Iterator it) const
		{
			return index == it.index;
		}
		bool operator!=(const Iterator it) const
		{
			return index != it.index;
		}
	};
protected:
	MAKE_CENUM_Q(External, i32,
		BUFFER_MARKER, -1,
		CONST_BUFFER_MARKER, -2);

	T* memory;
	i32 allocationCount;
	i32 growSize;
};

#endif // SDK_CUTL_MEMORY_HPP
