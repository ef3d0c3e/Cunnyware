#ifndef SDK_IACHIEVEMENT_HPP
#define SDK_IACHIEVEMENT_HPP

#include "../Util/Defs.hpp"

class IAchievement
{
public:
	virtual i32 GetAchievementID() = 0;
	virtual const char *GetName() = 0;
	virtual i32 GetFlags() = 0;
	virtual i32 GetGoal() = 0;
	virtual i32 GetCount() = 0;
	virtual bool IsAchieved() = 0;
	virtual i32 GetPoi32Value() = 0;
	virtual bool ShouldSaveWithGame() = 0;
	virtual bool ShouldHideUntilAchieved() = 0;
	virtual const char *GetIconPath() = 0;
	virtual i32 GetDisplayOrder() = 0;
};

#endif // SDK_IACHIEVEMENT_HPP
