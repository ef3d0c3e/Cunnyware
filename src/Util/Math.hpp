#ifndef UTIL_MATH_HPP
#define UTIL_MATH_HPP

#include "Defs.hpp"

namespace Math
{
void NormalizeAngles(QAngle& angle);
f32 GetFov(const QAngle& viewAngle, const QAngle& aimAngle);
void VectorAngles(const Vec3& forward, QAngle &angles);
QAngle CalcAngle(const Vec3& src, const Vec3& dst);
}

#endif // UTIL_MATH_HPP
