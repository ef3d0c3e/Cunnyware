#ifndef UTILS_DEFS_HPP
#define UTILS_DEFS_HPP

#define VECTOR_USE_STD
#include "Vector.hpp"
#include "Cenum.hpp"
#include "CStructs.hpp"
#include <array>
#include <cstdint>
#include <functional>
#include <string>

using MPV = MPVector<std::size_t>;

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

template <class T, std::size_t N>
struct __Vec;
template <class T, std::size_t N>
struct __VecA;
template <class T, std::size_t N>
struct __QAngle;

template <class T>
struct __Vec<T, 2>
{
	T x, y;

	template <std::size_t i>
	inline constexpr const T& Get() const noexcept
	{
		if constexpr (i == 0)
			return x;
		else if constexpr (i == 1)
			return y;
	}
	template <std::size_t i>
	inline constexpr T& Get() noexcept
	{
		if constexpr (i == 0)
			return x;
		else if constexpr (i == 1)
			return y;
	}

	inline constexpr const T& operator[](std::size_t i) const noexcept
	{
		switch (i)
		{
			case 0:
				return x;
			case 1:
				return y;
		}
		__builtin_unreachable();
	}
	inline constexpr T& operator[](std::size_t i) noexcept
	{
		switch (i)
		{
			case 0:
				return x;
			case 1:
				return y;
		}
		__builtin_unreachable();
	}
};

template <class T>
struct __Vec<T, 3>
{
	T x, y, z;

	template <std::size_t i>
	inline constexpr const T& Get() const noexcept
	{
		if constexpr (i == 0)
			return x;
		else if constexpr (i == 1)
			return y;
		else if constexpr (i == 2)
			return z;
	}
	template <std::size_t i>
	inline constexpr T& Get() noexcept
	{
		if constexpr (i == 0)
			return x;
		else if constexpr (i == 1)
			return y;
		else if constexpr (i == 2)
			return z;
	}

	inline constexpr const T& operator[](std::size_t i) const noexcept
	{
		switch (i)
		{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
		}
		__builtin_unreachable();
	}
	inline constexpr T& operator[](std::size_t i) noexcept
	{
		switch (i)
		{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
		}
		__builtin_unreachable();
	}
};

template <class T>
struct __Vec<T, 4>
{
	T x, y, z, w;

	template <std::size_t i>
	inline constexpr const T& Get() const noexcept
	{
		if constexpr (i == 0)
			return x;
		else if constexpr (i == 1)
			return y;
		else if constexpr (i == 2)
			return z;
		else if constexpr (i == 3)
			return w;
	}
	template <std::size_t i>
	inline constexpr T& Get() noexcept
	{
		if constexpr (i == 0)
			return x;
		else if constexpr (i == 1)
			return y;
		else if constexpr (i == 2)
			return z;
		else if constexpr (i == 3)
			return w;
	}

	inline constexpr const T& operator[](std::size_t i) const noexcept
	{
		switch (i)
		{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			case 3:
				return w;
		}
		__builtin_unreachable();
	}
	inline constexpr T& operator[](std::size_t i) noexcept
	{
		switch (i)
		{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			case 3:
				return w;
		}
		__builtin_unreachable();
	}
};

template <class T>
struct __attribute__((aligned(16))) __VecA<T, 3>
{
	T x, y, z;

	template <std::size_t i>
	inline constexpr const T& Get() const noexcept
	{
		if constexpr (i == 0)
			return x;
		else if constexpr (i == 1)
			return y;
		else if constexpr (i == 2)
			return z;
	}
	template <std::size_t i>
	inline constexpr T& Get() noexcept
	{
		if constexpr (i == 0)
			return x;
		else if constexpr (i == 1)
			return y;
		else if constexpr (i == 2)
			return z;
	}

	inline constexpr const T& operator[](std::size_t i) const noexcept
	{
		switch (i)
		{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
		}
		__builtin_unreachable();
	}
	inline constexpr T& operator[](std::size_t i) noexcept
	{
		switch (i)
		{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
		}
		__builtin_unreachable();
	}
};

template <class T>
struct __attribute__((aligned(16))) __VecA<T, 4>
{
	T x, y, z, w;

	template <std::size_t i>
	inline constexpr const T& Get() const noexcept
	{
		if constexpr (i == 0)
			return x;
		else if constexpr (i == 1)
			return y;
		else if constexpr (i == 2)
			return z;
		else if constexpr (i == 3)
			return w;
	}
	template <std::size_t i>
	inline constexpr T& Get() noexcept
	{
		if constexpr (i == 0)
			return x;
		else if constexpr (i == 1)
			return y;
		else if constexpr (i == 2)
			return z;
		else if constexpr (i == 3)
			return w;
	}

	inline constexpr const T& operator[](std::size_t i) const noexcept
	{
		switch (i)
		{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			case 3:
				return w;
		}
		__builtin_unreachable();
	}
	inline constexpr T& operator[](std::size_t i) noexcept
	{
		switch (i)
		{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			case 3:
				return w;
		}
		__builtin_unreachable();
	}
};

template <class T>
struct __QAngle<T, 3>
{
	T pitch, yaw, roll;

	template <std::size_t i>
	inline constexpr const T& Get() const noexcept
	{
		if constexpr (i == 0)
			return pitch;
		else if constexpr (i == 1)
			return yaw;
		else if constexpr (i == 2)
			return roll;
	}
	template <std::size_t i>
	inline constexpr T& Get() noexcept
	{
		if constexpr (i == 0)
			return pitch;
		else if constexpr (i == 1)
			return yaw;
		else if constexpr (i == 2)
			return roll;
	}

	inline constexpr const T& operator[](std::size_t i) const noexcept
	{
		switch (i)
		{
			case 0:
				return pitch;
			case 1:
				return yaw;
			case 2:
				return roll;
		}
		__builtin_unreachable();
	}
	inline constexpr T& operator[](std::size_t i) noexcept
	{
		switch (i)
		{
			case 0:
				return pitch;
			case 1:
				return yaw;
			case 2:
				return roll;
		}
		__builtin_unreachable();
	}
};

typedef MPV::Vector<f32, 4, __Vec> Vec4;
typedef MPV::Vector<f32, 3, __Vec> Vec3;
typedef MPV::Vector<f32, 2, __Vec> Vec2;
typedef MPV::Vector<i32, 4, __Vec> Vec4i;
typedef MPV::Vector<i32, 3, __Vec> Vec3i;
typedef MPV::Vector<i32, 2, __Vec> Vec2i;
typedef MPV::Vector<f32, 4, __VecA> Vec4a;
typedef MPV::Vector<f32, 3, __VecA> Vec3a;
typedef MPV::Vector<u8, 4, __Vec> Vec4b;
using Quaternion = Vec4;
using RadianEuler = Vec3;

typedef MPV::Vector<Vec4, 4, __Vec> Mat4x4;
typedef MPV::Vector<Vec3, 3, __Vec> Mat3x3;
typedef MPV::Vector<Vec4, 3, __Vec> Mat3x4;
typedef MPV::Vector<Vec3, 4, __Vec> Mat4x3;
typedef MPV::Vector<Vec2, 2, __Vec> Mat2x2;
typedef MPV::Vector<Vec2, 3, __Vec> Mat3x2;
typedef MPV::Vector<Vec2, 4, __Vec> Mat4x2;

typedef MPV::Vector<Vec2i, 2, __Vec> Rect2i; // Rect_t
typedef MPV::Vector<Vec3i, 2, __Vec> Rect3i; // Rect3D_t
typedef MPV::Vector<Vec2, 2, __Vec> Rect2;

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

typedef MPV::Vector<f32, 3, __QAngle> QAngle;


#endif // UTILS_DEFS_HPP
