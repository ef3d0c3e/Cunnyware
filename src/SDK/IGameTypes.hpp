#ifndef SDK_IGAMETYPES_HPP
#define SDK_IGAMETYPES_HPP

#include "CUtl/Vector.hpp"
class WeaponProgression;
class CUtlStringList;

class IGameTypes
{
public:
	virtual ~IGameTypes() = 0;

	// Initialization. "force" will reload the data from file even if this interface has already been initialized.
	virtual bool Initialize(bool force = false) = 0;
	virtual bool IsInitialized(void) const = 0;

	//
	// Game Types and Modes
	//

	// Set the game type and mode convars from the given strings.
	virtual bool SetGameTypeAndMode(const char* gameType, const char* gameMode) = 0;
	virtual bool GetGameTypeAndModeFromAlias(const char* modeAlias, i32& outGameType, i32& outGameMode) = 0;
	virtual bool SetGameTypeAndMode(i32 gameType, i32 gameMode) = 0;
	virtual void SetAndParseExtendedServerInfo(KeyValues* extendedServerInfo) = 0;
	virtual void CheckShouldSetDefaultGameModeAndType(const char* mapNameFull) = 0;

	// Get the indexes for the current game type and mode.
	virtual i32 GetCurrentGameType() const = 0;
	virtual i32 GetCurrentGameMode() const = 0;

	virtual const char* GetCurrentMapName() = 0;

	// Get the current game type and mode UI strings.
	virtual const char* GetCurrentGameTypeNameID() = 0;
	virtual const char* GetCurrentGameModeNameID() = 0;

	// Apply the game mode convars for the current game type and mode.
	virtual bool ApplyConvarsForCurrentMode(bool isMultiplayer) = 0;

	// Output the values of the convars for the current game mode.
	virtual void DisplayConvarsForCurrentMode() = 0;

	// Returns the weapon progression for the current game type and mode.
	virtual const CUtlVector<WeaponProgression>* GetWeaponProgressionForCurrentModeCT() = 0;
	virtual const CUtlVector<WeaponProgression>* GetWeaponProgressionForCurrentModeT() = 0;

	virtual i32 GetNoResetVoteThresholdForCurrentModeCT() = 0;
	virtual i32 GetNoResetVoteThresholdForCurrentModeT() = 0;

	virtual const char* GetGameTypeFromInt(i32 gameType) = 0;
	virtual const char* GetGameModeFromInt(i32 gameType, i32 gameMode) = 0;

	virtual bool GetGameModeAndTypeIntsFromStrings(const char* szGameType, const char* szGameMode, i32& iOutGameType, i32& iOutGameMode) = 0;
	virtual bool GetGameModeAndTypeNameIdsFromStrings(const char* szGameType, const char* szGameMode, const char*& szOutGameTypeNameId, const char*& szOutGameModeNameId) = 0;


	virtual bool CreateOrUpdateWorkshopMapGroup(const char* szName, const CUtlStringList& vecMapNames) = 0;
	virtual bool IsWorkshopMapGroup(const char* szMapGroupName) = 0;

	//
	// Maps
	//

	// Get maps from mapgroup and get mapgroup from type and mode
	virtual const char* GetRandomMapGroup(const char* gameType, const char* gameMode) = 0;
	virtual const char* GetFirstMap(const char* mapGroup) = 0;
	virtual const char* GetRandomMap(const char* mapGroup) = 0;
	virtual const char* GetNextMap(const char* mapGroup, const char* mapName) = 0;

	virtual i32 GetMaxPlayersForTypeAndMode(i32 iType, i32 iMode) = 0;

	virtual bool IsValidMapGroupName(const char*  mapGroup) = 0;
	virtual bool IsValidMapInMapGroup(const char*  mapGroup, const char* mapName) = 0;
	virtual bool IsValidMapGroupForTypeAndMode(const char*  mapGroup, const char* gameType, const char* gameMode) = 0;

	// Apply the convars for the given map.
	virtual bool ApplyConvarsForMap(const char* mapName, bool isMultiplayer) = 0;

	// Get specifics about a map.
	virtual bool GetMapInfo(const char* mapName, u32& richPresence) = 0;

	// Returns the available character model names (T or CT) for the given map.
	virtual const CUtlStringList* GetTModelsForMap(const char* mapName) = 0;
	virtual const CUtlStringList* GetCTModelsForMap(const char* mapName) = 0;
	virtual const CUtlStringList* GetHostageModelsForMap(const char* mapName) = 0;
	virtual const i32 GetDefaultGameTypeForMap(const char* mapName) = 0;
	virtual const i32 GetDefaultGameModeForMap(const char* mapName) = 0;

	// Returns the view model arms name (T or CT) for the given map.
	virtual const char* GetTViewModelArmsForMap(const char* mapName) = 0;
	virtual const char* GetCTViewModelArmsForMap(const char* mapName) = 0;

	// Item requirements for the map
	virtual const char* GetRequiredAttrForMap(const char* mapName) = 0;
	virtual i32 GetRequiredAttrValueForMap(const char* mapName) = 0;
	virtual const char* GetRequiredAttrRewardForMap(const char* mapName) = 0;
	virtual i32 GetRewardDropListForMap(const char* mapName) = 0;

	// Map group properties
	virtual const CUtlStringList* GetMapGroupMapList(const char* mapGroup) = 0;
	
	virtual bool GetRunMapWithDefaultGametype() = 0;
	virtual void SetRunMapWithDefaultGametype(bool bDefaultGametype) = 0;

	virtual bool GetLoadingScreenDataIsCorrect() = 0;
	virtual void SetLoadingScreenDataIsCorrect(bool bLoadingScreenDataIsCorrect) = 0;

	//
	// Custom Bot Difficulty for Offline games
	//

	// Sets the bot difficulty for Offline games.
	virtual bool SetCustomBotDifficulty(i32 botDiff) = 0;

	// Returns the bot difficulty for Offline games.
	virtual i32 GetCustomBotDifficulty() = 0;

	virtual i32 GetCurrentServerNumSlots() = 0;
	virtual i32 GetCurrentServerSettingInt(const char* szSetting, i32 iDefaultValue = 0) = 0;

	virtual bool GetGameTypeFromMode(const char* szGameMode, const char* &gameTypeOut) = 0;

	virtual bool LoadMapEntry(KeyValues* pKV) = 0;
};

#endif // SDK_IGAMETYPES_HPP
