#include "Visuals.hpp"
#include "Util.hpp"
#include "Info.hpp"
#include "../Config.hpp"
#include "../Interface.hpp"
#include "../SDK/IMatRenderContext.hpp"
#include "../SDK/DrawModelState.hpp"
#include "../SDK/IMaterial.hpp"
#include "../SDK/C_BasePlayer.hpp"

// {{{ Configuration
EXPORT(bool, Settings::Chams::Enemies::drawOriginalModel) = true;
std::vector<ChamsMat> Settings::Chams::Enemies::materials;
EXPORT(bool, Settings::Chams::Enemies::validMaterials) = false;
EXPORT(bool, Settings::Chams::Allies::drawOriginalModel) = true;
std::vector<ChamsMat> Settings::Chams::Allies::materials;
EXPORT(bool, Settings::Chams::Allies::validMaterials) = false;
// }}}

typedef void (*DrawModelExecuteFn)(void*, IMatRenderContext*, const DrawModelState&, const ModelRenderInfo&, Mat3x4* customBoneToWorld);

IMaterial* mat = nullptr;

[[nodiscard]] inline ImVec4 ColorLerp(const std::vector<ImVec4>& colors, f32 p)
{
	if (colors.size() == 1)
		return colors[0];

	p = std::max(0.f, std::min(p, 1.f)); // Clamp in [0, 1]
	p *= colors.size()-1;
	u32 c1 = p * (colors.size()-1);
	u32 c2 = (c1+1) % (colors.size());
	p -= c1;
	c1 %= colors.size();

	// Guarantee for the alpha value, because alpha value of .9999f will look very different from a value of 1.f
	if (colors[c1].w == colors[c2].w) [[likely]]
		return ImVec4(
			colors[c1].x*p + colors[c2].x*(1.f-p),
			colors[c1].y*p + colors[c2].y*(1.f-p),
			colors[c1].z*p + colors[c2].z*(1.f-p),
			colors[c1].w);

	return ImVec4(
		colors[c1].x*p + colors[c2].x*(1.f-p),
		colors[c1].y*p + colors[c2].y*(1.f-p),
		colors[c1].z*p + colors[c2].z*(1.f-p),
		colors[c1].w*p + colors[c2].w*(1.f-p)
	);
}

bool DrawPlayer(void* thisptr, IMatRenderContext* renderContext, const DrawModelState& state, const ModelRenderInfo& info, Mat3x4* customBoneToWorld)
{
	C_BasePlayer* lp = (C_BasePlayer*)entityList->GetClientEntity(engine->GetLocalPlayer());
	if (!lp)
		return true;

	C_BasePlayer* p = (C_BasePlayer*)entityList->GetClientEntity(engine->GetLocalPlayer());
	if (!PlayerAdditionalInfo::ShouldDraw(p))
		return true;

	if (p == lp) // Localplayer
	{
		return true;
	}
	else if (p->GetTeam() != lp->GetTeam()) // Enemy
	{
		for (auto& m : Settings::Chams::Enemies::materials)
		{
			if (!m.mat)
				return true;

			if (m.playerModulation != ChamsMat::PlayerModulation::NONE)
			{
				ImVec4 color;
				switch (m.playerModulation)
				{
					case ChamsMat::PlayerModulation::STATIC:
						color = ColorLerp(m.colors, m.value);
						break;
					case ChamsMat::PlayerModulation::HEALTH:
						color = ColorLerp(m.colors, p->GetHealth()/100.f);
						break;
					case ChamsMat::PlayerModulation::ARMOR:
						color = ColorLerp(m.colors, p->GetArmor()/100.f);
						break;
					case ChamsMat::PlayerModulation::AMMO:
						{
							auto weapon = PlayerAdditionalInfo::GetWeapon(p);
							if (!weapon) [[unlikely]]
								color = ColorLerp(m.colors, m.value);
							else
								color = ColorLerp(m.colors, weapon->GetAmmo()/weapon->GetCSWpnData()->GetClipSize());
						}
						break;
					case ChamsMat::PlayerModulation::DISTANCE:
						color = ColorLerp(m.colors, PlayerAdditionalInfo::GetDistance(p)/m.value);
						break;
					case ChamsMat::PlayerModulation::PING:
						color = ColorLerp(m.colors, (*csPlayerResource)->GetPing(p->GetIndex())/m.value);
						break;
					case ChamsMat::PlayerModulation::DORMANT:
						color = ColorLerp(m.colors, PlayerAdditionalInfo::DormantTime(p)/m.value);
						break;
				}
				m.mat->ColorModulate(color.x, color.y, color.z);
				m.mat->AlphaModulate(color.w);

				modelRender->ForcedMaterialOverride(m.mat);
				modelRenderVMT->GetOriginalMethod<DrawModelExecuteFn>(21)(thisptr, renderContext, state, info, customBoneToWorld);
			}
		}

		return Settings::Chams::Enemies::drawOriginalModel;
	}
	else // Ally
	{

		return Settings::Chams::Allies::drawOriginalModel;
	}
}

bool Chams::DrawModelExecute(void* thisptr, IMatRenderContext* renderContext, const DrawModelState& state, const ModelRenderInfo& info, Mat3x4* customBoneToWorld)
{
	if(!engine->IsInGame())
		return true;
	if (!Settings::Visuals::enabled)
		return true;
	if (!info.model)
		return true;

	static bool created = false;
	if (!created)
	{
		mat = Util::CreateMaterial("ayololcustom", "VertexLitGeneric",
			"\"VertexLitGeneric\"\n"
			"{\n"
			"	\"$basetexture\" \"VGUI/white_additive\"\n"
			"	\"$ignorez\" \"1\"\n"
			"	\"$nofog\" \"1\"\n"
			"	\"$model\" \"1\"\n"
			"	\"$nocull\" \"1\"\n"
			"	\"$halflambert\" \"1\"\n"
			"}\n");

		created = true;
	}

	const char* name = modelInfo->GetModelName(info.model);

	if (strstr(name, "models/player"))
		return DrawPlayer(thisptr, renderContext, state, info, customBoneToWorld);

	return true;
}
