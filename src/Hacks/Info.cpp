#include "Info.hpp"
#include "../SDK/Recv.hpp"
#include "../SDK/C_BasePlayer.hpp"
#include "../Interface.hpp"
#include <chrono>

static std::array<PlayerAdditionalInfo, 32+1> informations;

PlayerAdditionalInfo::PlayerAdditionalInfo()
{
	dormant = false;
	dormantTime = 0;
	visible = false;
}

PlayerAdditionalInfo::~PlayerAdditionalInfo()
{}

bool PlayerAdditionalInfo::ShouldDraw(const C_BasePlayer* player)
{
	const PlayerAdditionalInfo& info = informations[player->GetIndex()];
	if (!info.dormant)
		return true;

	return info.dormantTime < GetMaxDormantTime();
}

bool PlayerAdditionalInfo::IsDormant(const C_BasePlayer* player)
{
	return informations[player->GetIndex()].dormant;
}

u32 PlayerAdditionalInfo::DormantTime(const C_BasePlayer* player)
{
	return informations[player->GetIndex()].dormantTime;
}

bool PlayerAdditionalInfo::IsVisible(const C_BasePlayer* player)
{
	return informations[player->GetIndex()].visible;
}

const std::array<char, PlayerInfo::MaxPlayerNameLength>& PlayerAdditionalInfo::GetName(const C_BasePlayer* player)
{
	return informations[player->GetIndex()].info.name;
}

void PlayerAdditionalInfo::NewRound()
{
	for (auto& info : informations)
	{
		info.dormant = false;
		info.dormantTime = 0;
		info.visible = false;
	}
}

void PlayerAdditionalInfo::Paint()
{
	static std::chrono::time_point<std::chrono::high_resolution_clock> lastTime = std::chrono::high_resolution_clock::now();

	std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
	for (i32 i = 1; i < std::min(entityList->GetHighestEntityIndex(), static_cast<i32>(informations.size())); ++i)
	{
		auto ent = entityList->GetClientEntity(i);
		if (!ent)
			continue;

		auto client = ent->GetClientClass();
		if (client->classID != EClassIds::CCSPlayer)
			continue;

		auto p = reinterpret_cast<C_BasePlayer*>(ent);

		PlayerAdditionalInfo& info = informations[i];
		engine->GetPlayerInfo(i, &info.info);

		if (p->IsDormant())
		{
			if (info.dormant)
				info.dormantTime += std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime).count();
			else
				info.dormantTime = 0, info.dormant = true;
		}
		else if (info.dormant)
			info.dormant = false, info.dormantTime = 0;
		
		//TODO: check for visible
		//Util::IsVisible(p, Bones::HEAD, 180.f, false)
		if (info.visible && info.dormant) [[unlikely]]
			info.dormantTime = GetMaxDormantTime() + 1; // Hide
	}

	lastTime = now;
}
