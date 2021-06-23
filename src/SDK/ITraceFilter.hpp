#ifndef SDK_ITRACEFILTER_HPP
#define SDK_ITRACEFILTER_HPP

#include "Defs.hpp"

MAKE_CENUM_Q(TraceType, i32,
	EVERYTHING, 0,
	WORLD_ONLY, 1,
	ENTITIES_ONLY, 2,
	EVERYTHING_FILTER_PROPS, 3);

class ITraceFilter
{
public:
	virtual bool ShouldHitEntity(C_BaseEntity* entity, i32 contentsMask) = 0;
	virtual TraceType GetTraceType() const = 0;
};

#endif // SDK_ITRACEFILTER_HPP
