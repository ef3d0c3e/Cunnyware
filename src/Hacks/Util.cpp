#include "Util.hpp"
#include "../Interface.hpp"
#include "../SDK/IEngineTrace.hpp"

bool Util::IsDangerZone()
{
	return (gameTypes->GetCurrentGameType() == GameTypes::DANGER_ZONE);
}

bool Util::IsVisible(C_BasePlayer* player, i32 bone, f32 fov, bool smoke_check)
{
	return false;
	/*
	C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
	if (!localplayer)
		return true;

	if (player == localplayer)
		return true;

	if (!localplayer->GetAlive())
	{
		if (*localplayer->GetObserverMode() == ObserverMode::IN_EYE && localplayer->GetObserverTarget())
			localplayer = (C_BasePlayer*)entityList->GetClientEntityFromHandle(localplayer->GetObserverTarget());

		if (!localplayer)
			return true;
	}

	Vector e_vecHead = player->GetBonePosition(bone);
	Vector p_vecHead = localplayer->GetEyePosition();

	QAngle viewAngles;
	engine->GetViewAngles(viewAngles);

	// FIXME: scale fov by distance? its not really working that well...
	//if (Math::GetFov(viewAngles, Math::CalcAngle(p_vecHead, e_vecHead)) > fov)
	//	return false;

	Ray ray;
	trace_t tr;
	ray.Init(p_vecHead, e_vecHead);
	CTraceFilter traceFilter;
	traceFilter.skip = localplayer;
	trace->TraceRay(ray, MASK_VISIBLE_AND_NPCS, &traceFilter, &tr);

	//if (smoke_check && LineGoesThroughSmoke(p_vecHead, e_vecHead, true))
	//	return false;

	return tr.entityHit == player;
	*/
}

bool Util::IsSpotVisible(C_BasePlayer* player, Vec3 spot, f32 fov, bool smoke_check)
{
	
}

bool Util::IsVisibleThroughEnemies(C_BasePlayer *player, i32 bone, f32 fov, bool smoke_check)
{
	
}

bool Util::IsSpotVisibleThroughEnemies(C_BasePlayer *player, Vec3 spot, f32 fov, bool smoke_check)
{
	
}

bool Util::IsPlanting(C_BasePlayer* player)
{
	
}

bool Util::IsTeamMate(C_BasePlayer* player, C_BasePlayer* localplayer)
{
	
}

