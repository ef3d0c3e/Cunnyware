#include "Info.hpp"
#include "../SDK/Recv.hpp"
#include "../SDK/C_BasePlayer.hpp"
#include "../Interface.hpp"
#include <chrono>

static std::array<PlayerAdditionalInfo, 64 /* TODO: 32 + (0 = world?) */> informations;

PlayerAdditionalInfo::PlayerAdditionalInfo()
{
	dormant = false;
	dormantTime = 0;
}

PlayerAdditionalInfo::~PlayerAdditionalInfo()
{}

bool PlayerAdditionalInfo::ShouldDraw(const C_BasePlayer* player)
{
	const PlayerAdditionalInfo& info = informations[player->GetIndex()];
	if (!info.dormant)
		return true;

	return info.dormantTime < 1500;
}

u32 PlayerAdditionalInfo::DormantTime(const C_BasePlayer* player)
{
	return informations[player->GetIndex()].dormantTime;
}

void PlayerAdditionalInfo::NewRound()
{
	for (auto& info : informations)
	{
		info.dormant = false;
		info.dormantTime = 0;
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

		if (p->IsDormant())
		{
			if (info.dormant)
				info.dormantTime += std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime).count();
			else
				info.dormantTime = 0, info.dormant = true;
		}
		else if (info.dormant)
			info.dormant = false, info.dormantTime = 0;
		
	}

	lastTime = now;
}
