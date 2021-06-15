#ifndef SDK_SHADERPARAMINFO_HPP
#define SDK_SHADERPARAMINFO_HPP

#include "../Util/Defs.hpp"

MAKE_CENUM_Q(ShaderParamType, i32,
	TEXTURE, 0,
	INTEGER, 1,
	COLOR, 2,
	VEC2, 3,
	VEC3, 4,
	VEC4, 5,
	ENVMAP, 6,
	FLOAT, 7,
	BOOL, 8,
	FOURCC, 9,
	MATRIX, 10,
	MATERIAL, 11,
	STRING, 12);

struct ShaderParamInfo
{
	const char* name;
	const char* help;
	ShaderParamType type;
	const char* defaultValue;
	i32 flags;
};

#endif // SDK_SHADERPARAMINFO_HPP
