#ifndef UTIL_MATH_HPP
#define UTIL_MATH_HPP

#include "Defs.hpp"

namespace Math
{
void NormalizeAngles(QAngle& angle) noexcept;
f32 GetFov(const QAngle& viewAngle, const QAngle& aimAngle) noexcept;
void VectorAngles(const Vec3& forward, QAngle &angles) noexcept;
QAngle CalcAngle(const Vec3& src, const Vec3& dst) noexcept;
void AngleVectors(const QAngle& angles, Vec3* forward, Vec3* right = nullptr, Vec3* up = nullptr) noexcept;
Vec3 AngleForward(const QAngle& angles) noexcept;

MAKE_CENUM_Q(IntersectionType, u8,
	NONE, 0,
	POINT, 1,
	COLL, 2);
std::pair<IntersectionType, Vec2> Intersection(const Rect2& s1, const Rect2& s2, f32 eps = 0.05f) noexcept;
}

#endif // UTIL_MATH_HPP
