#ifndef SDK_VPLANE_HPP
#define SDK_VPLANE_HPP

#include "../Util/Defs.hpp"


class VPlane
{
public:
	static constexpr inline f32 Epsilon = 0.01f;
	Vec3 normal;
	Vec3 dist;
};

#endif // SDK_VPLANE_HPP
