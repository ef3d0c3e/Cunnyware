#ifndef SDK_AUDIOSTATE_HPP
#define SDK_AUDIOSTATE_HPP

#include "../Util/Defs.hpp"

struct AudioState
{
	AudioState():
		isUnderwater(false)
	{}

	Vec3 origin;
	QAngle angles;
	bool isUnderwater;
};

#endif // SDK_AUDIOSTATE_HPP
