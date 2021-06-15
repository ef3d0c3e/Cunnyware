#ifndef SDK_ASPECTRATIOINFO_HPP
#define SDK_ASPECTRATIOINFO_HPP

#include "../Util/Defs.hpp"

struct AspectRatioInfo
{
	bool isWidescreen;
	bool isHidef;
	f32 frameBufferAspectRatio;
	f32 physicalAspectRatio;
	f32 frameBuffertoPhysicalScalar;
	f32 physicalToFrameBufferScalar;
	bool initialized;
};

#endif // SDK_ASPECTRATIOINFO_HPP
