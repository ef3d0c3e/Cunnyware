#ifndef SDK_IMATERIALSYSTEMHARDWARECONFIG_HPP
#define SDK_IMATERIALSYSTEMHARDWARECONFIG_HPP

#include "ImageFormat.hpp"

MAKE_CENUM_Q(VertexCompressionType, i32,
	// This indicates an uninitialized VertexCompressionType_t value
	INVALID, 0xFFFFFFFF,

	// 'VERTEX_COMPRESSION_NONE' means that no elements of a vertex are compressed
	NONE, 0,

	// Currently (more stuff may be added as needed), 'VERTEX_COMPRESSION_ON' means:
	//  - if a vertex contains VERTEX_ELEMENT_NORMAL, this is compressed
	//    (see CVertexBuilder::CompressedNormal3f)
	//  - if a vertex contains VERTEX_ELEMENT_USERDATA4 (and a normal - together defining a tangent
	//    frame, with the binormal reconstructed in the vertex shader), this is compressed
	//    (see CVertexBuilder::CompressedUserData)
	//  - if a vertex contains VERTEX_ELEMENT_BONEWEIGHTSx, this is compressed
	//    (see CVertexBuilder::CompressedBoneWeight3fv)
	ON, 1);

MAKE_CENUM_Q(HDRType, i32,
	NONE, 0,
	INTEGER, 1,
	FLOAT, 2);

MAKE_CENUM_Q(ShadowFilterMode, i32,
	SHADOWFILTERMODE_DEFAULT, 0,
	NVIDIA_PCF, 0,
	ATI_NO_PCF_FETCH4, 1,
	NVIDIA_PCF_CHEAP, 2,
	ATI_NOPCF, 3,
	GAMECONSOLE_NINE_TAP_PCF, 0,
	GAMECONSOLE_SINGLE_TAP_PCF, 1,
	SHADOWFILTERMODE_FIRST_CHEAP_MODE, 2);

MAKE_CENUM_Q(CSMQualityMode, i32,
	VERY_LOW, 0,
	LOW, 1,
	MEDIUM, 2,
	HIGH, 3,
	TOTAL_MODES, 4);

MAKE_CENUM_Q(CSMShaderMode, i32,
	LOW_OR_VERY_LOW, 0,
	MEDIUM, 1,
	HIGH, 2,
	ATIFETCH4, 3,
	TOTAL_MODES, 4);

class IMaterialSystemHardwareConfig
{
public:
	virtual i32 GetFrameBufferColorDepth() const = 0;
	virtual i32 GetSamplerCount() const = 0;
	virtual bool HasSetDeviceGammaRamp() const = 0;
	virtual bool SupportsStaticControlFlow() const = 0;
	virtual VertexCompressionType SupportsCompressedVertices() const = 0;
	virtual i32 MaximumAnisotropicLevel() const = 0; // 0 means no anisotropic filtering
	virtual i32 MaxTextureWidth() const = 0;
	virtual i32 MaxTextureHeight() const = 0;
	virtual i32 TextureMemorySize() const = 0;
	virtual bool SupportsMipmappedCubemaps() const = 0;

	virtual i32 NumVertexShaderConstants() const = 0;
	virtual i32 NumPixelShaderConstants() const = 0;
	virtual i32 MaxNumLights() const = 0;
	virtual i32 MaxTextureAspectRatio() const = 0;
	virtual i32 MaxVertexShaderBlendMatrices() const = 0;
	virtual i32 MaxUserClipPlanes() const = 0;
	virtual bool UseFastClipping() const = 0;

	// This here should be the major item looked at when checking for compat
	// from anywhere other than the material system	shaders
	virtual i32 GetDXSupportLevel() const = 0;
	virtual const char* GetShaderDLLName() const = 0;

	virtual bool ReadPixelsFromFrontBuffer() const = 0;

	// Are dx dynamic textures preferred?
	virtual bool PreferDynamicTextures() const = 0;

	virtual bool SupportsHDR() const = 0;

	virtual bool NeedsAAClamp() const = 0;
	virtual bool NeedsATICentroidHack() const = 0;

	// This is the max dx support level supported by the card
	virtual i32 GetMaxDXSupportLevel() const = 0;

	// Does the card specify fog color in linear space when sRGBWrites are enabled?
	virtual bool SpecifiesFogColorInLinearSpace() const = 0;

	// Does the card support sRGB reads/writes?
	virtual bool SupportsSRGB() const = 0;

	virtual bool FakeSRGBWrite() const = 0;

	virtual bool CanDoSRGBReadFromRTs() const = 0;

	virtual bool SupportsGLMixedSizeTargets() const = 0;

	virtual bool IsAAEnabled() const = 0; // Is antialiasing being used?

	// NOTE: Anything after this was added after shipping HL2.
	virtual i32 GetVertexSamplerCount() const = 0;
	virtual i32 GetMaxVertexTextureDimension() const = 0;

	virtual i32 MaxTextureDepth() const = 0;

	virtual HDRType GetHDRType() const = 0;
	virtual HDRType GetHardwareHDRType() const = 0;

	virtual bool SupportsStreamOffset() const = 0;

	virtual i32 StencilBufferBits() const = 0;
	virtual i32 MaxViewports() const = 0;

	virtual void OverrideStreamOffsetSupport(bool overrideEnabled, bool enableSupport) = 0;

	virtual ShadowFilterMode GetShadowFilterMode(bool forceLowQualityShadows, bool PS30) const = 0;

	virtual i32 NeedsShaderSRGBConversion() const = 0;

	virtual bool UsesSRGBCorrectBlending() const = 0;

	virtual bool HasFastVertexTextures() const = 0;
	virtual i32 MaxHWMorphBatchCount() const = 0;

	virtual bool SupportsHDRMode(HDRType HDRMode) const = 0;

	virtual bool GetHDREnabled() const = 0;
	virtual void SetHDREnabled(bool bEnable) = 0;

	virtual bool SupportsBorderColor() const = 0;
	virtual bool SupportsFetch4() const = 0;

	virtual f32 GetShadowDepthBias() const = 0;
	virtual f32 GetShadowSlopeScaleDepthBias() const = 0;

	virtual bool PreferZPrepass() const = 0;

	virtual bool SuppressPixelShaderCentroidHackFixup() const = 0;
	virtual bool PreferTexturesInHWMemory() const = 0;
	virtual bool PreferHardwareSync() const = 0;
	virtual bool ActualHasFastVertexTextures() const = 0;

	virtual bool SupportsShadowDepthTextures() const = 0;
	virtual ImageFormat GetShadowDepthTextureFormat() const = 0;
	virtual ImageFormat GetHighPrecisionShadowDepthTextureFormat() const = 0;
	virtual ImageFormat GetNullTextureFormat() const = 0;
	virtual i32 GetMinDXSupportLevel() const = 0;
	virtual bool IsUnsupported() const = 0;

	virtual f32 GetLightMapScaleFactor() const = 0;
	virtual bool SupportsCascadedShadowMapping() const = 0;
	virtual CSMQualityMode GetCSMQuality() const = 0;
	virtual bool SupportsBilinearPCFSampling() const = 0;
	virtual CSMShaderMode GetCSMShaderMode(CSMQualityMode qualityLevel) const = 0;
};

#endif // SDK_IMATERIALSYSTEMHARDWARECONFIG_HPP
