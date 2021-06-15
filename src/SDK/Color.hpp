#ifndef SDK_COLOR_HPP
#define SDK_COLOR_HPP

#include "../Util/Defs.hpp"
#include "../ImGUI/imgui.h"
#include "../ImGUI/imgui_internal.h"

template <class T, std::size_t N>
struct __Color_container;

template <class T>
struct __Color_container<T, 4>
{
	T r, g, b, a;

	T& operator[](std::size_t i)
	{ return *(reinterpret_cast<T*>(this) + i); }

	const T& operator[](std::size_t i) const
	{ return *(reinterpret_cast<const T*>(this) + i); }
};



struct Color : public Vector<i32, 4, __Color_container>
{
	using base_type = Vector<i32, 4, __Color_container>;

	Color() noexcept:
		base_type(0, 0, 0, 255)
	{}

	Color(ImColor color) noexcept:
		Color(color.Value.x*255, color.Value.y*255, color.Value.z*255, color.Value.w*255)
	{}

	explicit operator ImColor() const noexcept
	{
		return ImColor(r, g, b, a); // int constructor
	}

	Color(i32 _r, i32 _g, i32 _b, i32 _a = 255) noexcept:
		base_type(_r, _g, _b, _a)
	{}

	static Color FromHSB(f32 hue, f32 saturation, f32 brightness)
	{
		f32 h = hue == 1.0f ? 0 : hue * 6.0f;
		f32 f = h - (int)h;
		f32 p = brightness * (1.0f - saturation);
		f32 q = brightness * (1.0f - saturation * f);
		f32 t = brightness * (1.0f - (saturation * (1.0f - f)));

		if (h < 1)
		{
			return Color(
					(u8)(brightness * 255),
					(u8)(t * 255),
					(u8)(p * 255)
			);
		}
		else if (h < 2)
		{
			return Color(
					(u8)(q * 255),
					(u8)(brightness * 255),
					(u8)(p * 255)
			);
		}
		else if (h < 3)
		{
			return Color(
					(u8)(p * 255),
					(u8)(brightness * 255),
					(u8)(t * 255)
			);
		}
		else if (h < 4)
		{
			return Color(
					(u8)(p * 255),
					(u8)(q * 255),
					(u8)(brightness * 255)
			);
		}
		else if (h < 5)
		{
			return Color(
					(u8)(t * 255),
					(u8)(p * 255),
					(u8)(brightness * 255)
			);
		}
		else
		{
			return Color(
					(u8)(brightness * 255),
					(u8)(p * 255),
					(u8)(q * 255)
			);
		}
	}
};

// color32
struct ColorRGBA : Vector<u8, 4, __Color_container>
{
	using base_type = Vector<u8, 4, __Color_container>;
	ColorRGBA(u8 _r, u8 _g, u8 _b, u8 _a = 255) noexcept:
		base_type(_a, _g, _b, _a)
	{}

	~ColorRGBA() {}
};

#endif // SDK_COLOR_HPP
