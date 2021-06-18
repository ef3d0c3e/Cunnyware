#ifndef SDK_VARMAPPING_HPP
#define SDK_VARMAPPING_HPP

#include "CUtl/Vector.hpp"

class IInterpolatedVar;

class VarMapEntry
{
public:
	u16 type;
	u16 needsToInterpolate; // Set to false when this var doesn't
		// need Interpolate() called on it anymore.
	void* data;
	IInterpolatedVar* watcher;
};

struct VarMapping
{
	VarMapping()
	{
		interpolatedEntries = 0;
	}

	CUtlVector<VarMapEntry> entries;
	i32 interpolatedEntries;
	f32 lastInterpolationTime;
};

#endif // SDK_VARMAPPING_HPP
