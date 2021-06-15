#ifndef SDK_SURFINFO_HPP
#define SDK_SURFINFO_HPP

#include "VPlane.hpp"
#include <array>

class SurfInfo
{
public:
	// Shape of the surface.
	static constexpr inline std::size_t SurfInfoVertsMax = 16;
	std::array<Vec3, SurfInfoVertsMax> verts;
	u64 numVerts;

	// Plane of the surface.
	VPlane plane;

	// For engine use only..
	void* engineData;
};

#endif // SDK_SURFINFO_HPP
