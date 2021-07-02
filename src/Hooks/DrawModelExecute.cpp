#include "Hooks.hpp"
#include "../Hacks/Visuals.hpp"

void Hooks::DrawModelExecute(void* thisptr, IMatRenderContext* renderContext, const DrawModelState& state, const ModelRenderInfo& info, Mat3x4* customBoneToWorld)
{
	if (Chams::DrawModelExecute(thisptr, renderContext, state, info, customBoneToWorld))
	{
		modelRender->ForcedMaterialOverride(nullptr);
		modelRenderVMT->GetOriginalMethod<void (*)(void*, IMatRenderContext*, const DrawModelState&, const ModelRenderInfo&, Mat3x4*)>
			(21)(thisptr, renderContext, state, info, customBoneToWorld);
	}
}
