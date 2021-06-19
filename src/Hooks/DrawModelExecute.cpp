#include "Hooks.hpp"

void Hooks::DrawModelExecute(void* thisptr, IMatRenderContext* renderContext, const DrawModelState& state, const ModelRenderInfo& info, Mat3x4* customBoneToWorld)
{
	modelRenderVMT->GetOriginalMethod<void (*)(void*, IMatRenderContext*, const DrawModelState&, const ModelRenderInfo&, Mat3x4*)>
		(21)(thisptr, renderContext, state, info, customBoneToWorld);
}
