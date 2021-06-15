#ifndef IACHIEVEMENTMGR_HPP
#define IACHIEVEMENTMGR_HPP

#include "CUtl/Vector.hpp"
#include "IAchievement.hpp"

class CBaseAchievement;
class IAchievementMgr
{
public:
	virtual IAchievement* GetAchievementByIndex(i32 index, i32 playerSlot) = 0;
	virtual IAchievement* GetAchievementByDisplayOrder(i32 orderIndex, i32 playerSlot) = 0;
	virtual CBaseAchievement* GetAchievementByID(i32 id, i32 playerSlot) = 0;
	virtual i32 GetAchievementCount() = 0;
	virtual void InitializeAchievements() = 0;
	virtual void AwardAchievement(i32 achievementID, i32 playerSlot) = 0;
	virtual void OnMapEvent(const char* eventName, i32 playerSlot) = 0;
	virtual void SaveGlobalStateIfDirty() = 0;
	virtual bool HasAchieved(const char* name, i32 playerSlot) = 0;
	virtual const CUtlVector<i32>& GetAchievedDuringCurrentGame(i32 nPlayerSlot) = 0;
};

#endif // IACHIEVEMENTMGR_HPP
