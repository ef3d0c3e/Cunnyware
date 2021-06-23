#ifndef SDK_RAY_HPP
#define SDK_RAY_HPP

#include "../Util/Defs.hpp"

struct Ray
{
	Vec3 start; // starting point, centered within the extents
	Vec3 delta; // direction + length of the ray
	Vec3 startOffset; // Add this to m_Start to get the actual ray start
	Vec3 extents; // Describes an axis aligned box extruded along a ray
	const Mat3x4* worldAxisTransform;
	bool isRay; // are the extents zero?
	bool isSwept; // is delta != 0?

	Ray(): worldAxisTransform(nullptr)
	{}

	void Init(Vec3 start, Vec3 end)
	{
		delta = end - start;
		isSwept = (delta.LengthSqr() != 0);
		extents.x = extents.y = extents.z = 0.0f;
		worldAxisTransform = nullptr;
		isRay = true;
		startOffset = Vec3(0.0f, 0.0f, 0.0f);
		this->start = start;
	}

	void Init(const Vec3& start, const Vec3& end, const Vec3& mins, const Vec3& maxs)
	{
		delta = end - start;

		worldAxisTransform = nullptr;
		isSwept = ( delta.LengthSqr() != 0 );

		extents = maxs - mins;
		extents *= 0.5f;
		isRay = (extents.LengthSqr() < 1e-6);

		// Offset m_Start to be in the center of the box...
		startOffset = maxs + mins;
		startOffset *= 0.5f;
		this->start = start + startOffset;
		startOffset *= -1.0f;
	}
};

#endif // SDK_RAY_HPP
