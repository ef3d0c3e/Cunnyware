#ifndef SDK_MATERIALSYSTEMINFO_HPP
#define SDK_MATERIALSYSTEMINFO_HPP

#include "../Util/Defs.hpp"
#include "ImageFormat.hpp"
#include <array>

MAKE_CENUM_Q(MaterialSystemConfigFlags, u32,
	WINDOWED, 1<<0,
	RESIZING, 1<<1,
	NO_WAIT_FOR_VSYNC, 1<<3,
	STENCIL, 1<<4,
	DISABLE_SPECULAR, 1<<7,
	DISABLE_BUMPMAP, 1<<8,
	ENABLE_PARALLAX_MAPPING, 1<<9,
	USE_Z_PREFILL, 1<<10,
	ENABLE_HDR, 1<<12,
	LIMIT_WINDOWED_SIZE, 1<<13,
	SCALE_TO_OUTPUT_RESOLUTION, 1<<14,
	USING_MULTIPLE_WINDOWS, 1<<15,
	DISABLE_PHONG, 1<<16,
	NO_WINDOW_BORDER, 1<<17);


struct MaterialVideoMode
{
	i32 width;          // if width and height are 0 and you select 
	i32 height;         // windowed mode, it'll use the window size
	ImageFormat format; // use ImageFormats (ignored for windowed mode)
	i32 refreshRate;    // 0 == default (ignored for windowed mode)
};

struct MaterialSystemConfig
{
	bool Windowed() const
	{
		return (flags & MaterialSystemConfigFlags::WINDOWED) != 0;
	}
	bool NoWindowBorder() const
	{
		return (flags & MaterialSystemConfigFlags::NO_WINDOW_BORDER) != 0;
	}
	bool Resizing() const
	{
		return (flags & MaterialSystemConfigFlags::RESIZING) != 0;
	}
	bool WaitForVSync() const
	{
		return (flags & MaterialSystemConfigFlags::NO_WAIT_FOR_VSYNC) == 0;
	}
	bool Stencil() const
	{
		return (flags & MaterialSystemConfigFlags::STENCIL) != 0;
	}
	bool UseSpecular() const
	{
		return (flags & MaterialSystemConfigFlags::DISABLE_SPECULAR) == 0;
	}
	bool UseBumpmapping() const
	{
		return (flags & MaterialSystemConfigFlags::DISABLE_BUMPMAP) == 0;
	}
	bool UseParallaxMapping() const
	{
		return (flags & MaterialSystemConfigFlags::ENABLE_PARALLAX_MAPPING) != 0;
	}
	bool UseZPrefill() const
	{
		return (flags & MaterialSystemConfigFlags::USE_Z_PREFILL) != 0;
	}
	bool HDREnabled() const
	{
		return (flags & MaterialSystemConfigFlags::ENABLE_HDR) != 0;
	}
	bool LimitWindowedSize() const
	{
		return (flags & MaterialSystemConfigFlags::LIMIT_WINDOWED_SIZE) != 0;
	}
	bool ScaleToOutputResolution() const
	{
		return (flags & MaterialSystemConfigFlags::SCALE_TO_OUTPUT_RESOLUTION) != 0;
	}
	bool UsingMultipleWindows() const
	{
		return (flags & MaterialSystemConfigFlags::USING_MULTIPLE_WINDOWS) != 0;
	}
	bool ShadowDepthTexture() const
	{
		return shadowDepthTexture;
	}
	bool MotionBlur() const
	{
		return motionBlur;
	}
	bool SupportFlashlight() const
	{
		return supportFlashlight;
	}
	bool UsePhong() const
	{
		return (flags & MaterialSystemConfigFlags::DISABLE_PHONG) == 0;
	}

	void SetFlag(unsigned int flag, bool val)
	{
		if (val)
		{
			flags |= flag;
		}
		else
		{
			flags &= ~flag;
		}
	}

	// control panel stuff
	MaterialVideoMode videoMode;
	f32 monitorGamma;
	f32 gammaTVRangeMin;
	f32 gammaTVRangeMax;
	f32 gammaTVExponent;
	bool gammaTVEnabled;

	bool wantTripleBuffered; // We only get triple buffering if fullscreen and vsync'd
	i32 AASamples;
	i32 forceAnisotropicLevel;
	i32 skipMipLevels;
	i32 dxSupportLevel;
	u32 flags;
	bool editMode; // true if in Hammer.
	u8 proxiesTestMode; // 0 = normal, 1 = no proxies, 2 = alpha test all, 3 = color mod all
	bool compressedTextures;
	bool filterLightmaps;
	bool filterTextures;
	bool reverseDepth;
	bool bufferPrimitives;
	bool drawFlat;
	bool measureFillRate;
	bool visualizeFillRate;
	bool noTransparency;
	bool softwareLighting;
	bool allowCheats;
	char showMipLevels;
	bool showLowResImage;
	bool showNormalMap;
	bool mipMapTextures;
	u8 fullbright;
	bool fastNoBump;
	bool suppressRendering;

	// debug modes
	bool showSpecular; // This is the fast version that doesn't require reloading materials
	bool showDiffuse; // This is the fast version that doesn't require reloading materials

	u32 windowedSizeLimitWidth;
	u32 windowedSizeLimitHeight;
	i32 AAQuality;
	bool shadowDepthTexture;
	bool motionBlur;
	bool supportFlashlight;

	bool paintInGame;
	bool paintInMap;
};

struct MaterialAdapterInfo
{
	static constexpr inline std::size_t MaterialAdapterNameLength = 512;
	std::array<char, MaterialAdapterNameLength> driverName;
	u32 vendorID;
	u32 deviceID;
	u32 subSysID;
	u32 revision;
	i32 DXSupportLevel; // This is the *preferred* dx support level
	i32 minDXSupportLevel;
	i32 maxDXSupportLevel;
	u32 driverVersionHigh;
	u32 driverVersionLow;
};

struct MaterialSystemHardwareIdentifier
{
	char* cardName;
	u32 vendorID;
	u32 deviceID;
};
#endif // SDK_MATERIALSYSTEMINFO_HPP
