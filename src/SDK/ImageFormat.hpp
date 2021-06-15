#ifndef SDK_IMAGEFORMAT_HPP
#define SDK_IMAGEFORMAT_HPP

#include "../Util/Defs.hpp"

MAKE_CENUM_Q(ImageFormat, i32,
	UNKNOWNN, -1,
	RGBA8888, 0,
	ABGR8888, 1,
	RGB888, 2,
	BGR888, 3,
	RGB565, 4,
	I8, 5,
	IA88, 6,
	P8, 7,
	A8, 8,
	RGB888_BLUESCREEN, 9,
	BGR888_BLUESCREEN, 10,
	ARGB8888, 11,
	BGRA8888, 12,
	DXT1, 13,
	DXT3, 14,
	DXT5, 15,
	BGRX8888, 16,
	BGR565, 17,
	BGRX5551, 18,
	BGRA4444, 19,
	DXT1_ONEBITALPHA, 20,
	BGRA5551, 21,
	UV88, 22,
	UVWQ8888, 23,
	RGBA16161616F, 24,
	RGBA16161616, 25,
	UVLX8888, 26,
	R32F, 27,			// Single-channel 32-bit floating point
	RGB323232F, 28,	// NOTE: D3D9 does not have this format
	RGBA32323232F, 29,
	RG1616F, 30,
	RG3232F, 31,
	RGBX8888, 32,

	NULL_, 33,			// Dummy format which takes no video memory

	// Compressed normal map formats
	ATI2N, 34,			// One-surface ATI2N / DXN format
	ATI1N, 35,			// Two-surface ATI1N format

	RGBA1010102, 36,	// 10 bit-per component render targets
	BGRA1010102, 37,
	R16F, 38,			// 16 bit FP format

	// Depth-stencil texture formats
	D16, 39,
	D15S1, 40,
	D32, 41,
	D24S8, 42,
	LINEAR_D24S8, 43,
	D24X8, 44,
	D24X4S4, 45,
	D24FS8, 46,
	D16_SHADOW, 47,	// Specific formats for shadow mapping
	D24X8_SHADOW, 48,	// Specific formats for shadow mapping

	// supporting these specific formats as non-tiled for procedural cpu access (360-specific)
	LINEAR_BGRX8888, 49,
	LINEAR_RGBA8888, 50,
	LINEAR_ABGR8888, 51,
	LINEAR_ARGB8888, 52,
	LINEAR_BGRA8888, 53,
	LINEAR_RGB888, 54,
	LINEAR_BGR888, 55,
	LINEAR_BGRX5551, 56,
	LINEAR_I8, 57,
	LINEAR_RGBA16161616, 58,

	LE_BGRX8888, 59,
	LE_BGRA8888, 60);

#endif // SDK_IMAGEFORMAT_HPP
