#ifndef SDK_ICLIENTMODELRENDERABLE_HPP
#define SDK_ICLIENTMODELRENDERABLE_HPP

#include "Defs.hpp"

MAKE_CENUM_Q(ModelDataCategory, u32,
	LIGHTING_MODEL, 0, // data type returned is a RenderableLightingModel
	STENCIL, 1 // data type returned is a ShaderStencilState
);

class IClientModelRenderable
{
public:
	virtual bool GetRenderData(void* data, ModelDataCategory category) = 0;
};

#endif // SDK_ICLIENTMODELRENDERABLE_HPP
