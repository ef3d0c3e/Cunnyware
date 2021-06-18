#ifndef SDK_CGLOBALVARS_HPP
#define SDK_CGLOBALVARS_HPP

#include "CGlobalVarsBase.hpp"

MAKE_CENUM_Q(MapLoadType, u32,
	NewGame, 0,
	LoadGame, 1,
	Transition, 2,
	Background, 3);

struct edict_t;

class CGlobalVars : public CGlobalVarsBase
{	
public:
	// Current map
	string_t		mapname;
	string_t		mapGroupName;
	i32				mapversion;
	string_t		startspot;
	MapLoadType	eLoadType;		// How the current map was loaded
	bool			bMapLoadFailed;	// Map has failed to load, we need to kick back to the main menu

	// game specific flags
	bool			deathmatch;
	bool			coop;
	bool			teamplay;
	// current maxentities
	i32				maxEntities;

	i32				serverCount;
	edict_t			*edicts;
};

#endif // SDK_CGLOBALVARS_HPP
