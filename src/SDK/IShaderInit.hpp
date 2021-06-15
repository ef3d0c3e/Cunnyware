#ifndef SDK_ISHADERINIT_HPP
#define SDK_ISHADERINIT_HPP

#include "IMaterialVar.hpp"

class IShaderInit : non_constructible
{
public:
	// Loads up a texture
	virtual void LoadTexture(IMaterialVar* textureVar, const char* textureGroupName) = 0;
	virtual void LoadBumpMap(IMaterialVar* textureVar, const char* textureGroupName) = 0;
	virtual void LoadCubeMap(IMaterialVar** params, IMaterialVar* textureVar) = 0;
};

#endif // SDK_ISHADERINIT_HPP
