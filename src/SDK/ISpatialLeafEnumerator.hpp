#ifndef SDK_ISPATIALLEAFENUMERATOR_HPP
#define SDK_ISPATIALLEAFENUMERATOR_HPP

#include "../Util/Defs.hpp"

class ISpatialLeafEnumerator
{
public:
	// call back with a leaf and a context
	// The context is completely user defined; it's passed into the enumeration
	// function of ISpatialQuery.
	// This gets called	by the enumeration methods with each leaf
	// that passes the test; return true to continue enumerating,
	// false to stop

	virtual bool EnumerateLeaf(i32 leaf, i32 context) = 0;
};

#endif // SDK_ISPATIALLEAFENUMERATOR_HPP
