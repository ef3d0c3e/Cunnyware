#ifndef UTILS_DEFS_HPP
#define UTILS_DEFS_HPP

#include "Vector.hpp"
#include "Cenum.hpp"
#include "CStructs.hpp"

typedef std::uint8_t u8;
typedef std::uint16_t u16;
typedef std::uint32_t u32;
typedef std::uint64_t u64;
typedef std::int8_t i8;
typedef std::int16_t i16;
typedef std::int32_t i32;
typedef std::int64_t i64;
typedef float f32;
typedef double f64;

typedef Vector<f32, 4> Vec4;
typedef Vector<f32, 3> Vec3;
typedef Vector<f32, 2> Vec2;
typedef Vector<i32, 3> Vec3i;
typedef Vector<i32, 2> Vec2i;
using Quaternion = Vec4;

typedef Vector<Vec4, 4> Mat4x4;
typedef Vector<Vec3, 3> Mat3x3;
typedef Vector<Vec3, 4> Mat3x4;
typedef Vector<Vec3, 4> Mat4x3;
typedef Vector<Vec2, 2> Mat2x2;
typedef Vector<Vec2, 3> Mat3x2;
typedef Vector<Vec2, 4> Mat4x2;

typedef Vector<Vec2i, 2> Rect2i; // Rect_t
typedef Vector<Vec3i, 2> Rect3i; // Rect3D_t
typedef Vector<Vec2, 2> Rect2;

struct VRect // vrect_t
{
	Rect2i rect;
	VRect* next;
};

struct non_constructible
{
	non_constructible() noexcept = delete;
	//~non_constructible() noexcept = delete;
};

template <class T, std::size_t N>
struct __QAngle;

template <>
struct __QAngle<f32, 3>
{
	f32 pitch, yaw, roll;

	const f32& operator[](std::size_t i) const noexcept
	{ return *(reinterpret_cast<const f32*>(this) + i); }
	f32& operator[](std::size_t i) noexcept
	{ return *(reinterpret_cast<f32*>(this) + i); }
};

typedef Vector<f32, 3, __QAngle> QAngle;


#endif // UTILS_DEFS_HPP
