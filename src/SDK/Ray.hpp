#ifndef SDK_RAY_HPP
#define SDK_RAY_HPP

#include "../Util/Defs.hpp"

struct Ray : non_constructible
{
	Vec3 start; // starting point, centered within the extents
	Vec3 delta; // direction + length of the ray
	Vec3 startOffset; // Add this to m_Start to get the actual ray start
	Vec3 extents; // Describes an axis aligned box extruded along a ray
	const Mat3x4* worldAxisTransform;
	bool isRay; // are the extents zero?
	bool isSwept; // is delta != 0?
};

#endif // SDK_RAY_HPP
