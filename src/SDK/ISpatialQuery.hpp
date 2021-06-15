#ifndef SDK_ISPATIALQUERY_HPP
#define SDK_ISPATIALQUERY_HPP

#include "ISpatialLeafEnumerator.hpp"
#include "Ray.hpp"


class ISpatialQuery : non_constructible
{
public:
	// Returns the number of leaves
	virtual i32 LeafCount() const = 0;

	// Enumerates the leaves along a ray, box, etc.
	virtual bool EnumerateLeavesAtPoint(const Vec3& point, ISpatialLeafEnumerator* lenum, i32 context) = 0;
	virtual bool EnumerateLeavesInBox(const Vec3& mins, const Vec3& maxs, ISpatialLeafEnumerator* lenum, i32 context) = 0;
	virtual bool EnumerateLeavesInSphere(const Vec3& center, f32 radius, ISpatialLeafEnumerator* lenum, i32 context) = 0;
	virtual bool EnumerateLeavesAlongRay(const Ray& ray, ISpatialLeafEnumerator* lenum, i32 context) = 0;
	virtual bool EnumerateLeavesInSphereWithFlagSet(const Vec3& center, f32 radius, ISpatialLeafEnumerator* pEnum, i32 context, i32 nFlagsCheck) = 0;
	virtual i32 ListLeavesInBox(const Vec3& mins, const Vec3& maxs, unsigned short* list, i32 listMax) = 0;

	// Used to determine which leaves passed in (specified by leafcount, pLeafs, and nLeafStride )
	// are within radius flRadius of vecCenter and have the flag set.
	// The result is placed in the pLeafsInSphere array, which specifies _indices_ into the original pLeafs array
	// The number of leaves found within the sphere is the return value.
	// The caller is expected to have allocated at least nLeafCount pLeafsInSphere to place the results into
	virtual i32 ListLeavesInSphereWithFlagSet(i32* leafsInSphere, const Vec3& center, f32 radius, i32 leafCount, const u16* leafs, i32 leafStride = sizeof(u16), i32 nFlagsCheck = 0xFFFFFFFF) = 0;
};

#endif // SDK_ISPATIALQUERY_HPP
