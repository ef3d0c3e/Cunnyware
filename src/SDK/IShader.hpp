#ifndef SDK_ISHADER_HPP
#define SDK_ISHADER_HPP

#include "ShaderParamInfo.hpp"
#include "IShaderInit.hpp"
#include "IMaterialSystemHardwareConfig.hpp"


class IShaderShadow;
class IShaderDynamicAPI;
class CBasePerMaterialContextData;
class CBasePerInstanceContextData;
class IShader : non_constructible
{
public:
	// Returns the shader name
	virtual char const* GetName() const = 0;

	// returns the shader fallbacks
	virtual char const* GetFallbackShader(IMaterialVar** params) const = 0;

	// Shader parameters
	virtual i32 GetParamCount() const = 0;
	virtual const ShaderParamInfo& GetParamInfo(i32 paramIndex) const = 0;

	// These functions must be implemented by the shader
	virtual void InitShaderParams(IMaterialVar** ppParams, const char* pMaterialName) = 0;
	virtual void InitShaderInstance(IMaterialVar** ppParams, IShaderInit* pShaderInit, const char* pMaterialName, const char* pTextureGroupName) = 0;
	virtual void DrawElements(IMaterialVar** params, i32 nModulationFlags, IShaderShadow* pShaderShadow, IShaderDynamicAPI* pShaderAPI, VertexCompressionType vertexCompression, CBasePerMaterialContextData** pContextDataPtr, CBasePerInstanceContextData** pInstanceDataPtr) = 0;

	// FIXME: Figure out a better way to do this?
	virtual i32 ComputeModulationFlags(IMaterialVar** params, IShaderDynamicAPI* pShaderAPI) = 0;
	virtual bool NeedsPowerOfTwoFrameBufferTexture(IMaterialVar** params, bool bCheckSpecificToThisFrame = true) const = 0;
	virtual bool NeedsFullFrameBufferTexture(IMaterialVar** params, bool bCheckSpecificToThisFrame) const = 0;
	virtual bool IsTranslucent(IMaterialVar** params) const = 0;

	virtual i32 GetFlags() const = 0;
};


#endif // SDK_ISHADER_HPP
