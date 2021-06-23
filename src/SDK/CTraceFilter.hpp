#ifndef SDK_CTRACEFILTER_HPP
#define SDK_CTRACEFILTER_HPP

#include "ITraceFilter.hpp"

class CTraceFilter : public ITraceFilter
{
public:
	bool ShouldHitEntity(C_BaseEntity* entityHandle, i32 contentsMask)
	{
		return !(entityHandle == skip);
	}

	virtual TraceType GetTraceType() const
	{
		return TraceType::EVERYTHING;
	}

	void* skip;
};

#endif // SDK_CTRACEFILTER_HPP
