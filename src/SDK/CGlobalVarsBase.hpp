#ifndef SDK_CGLOBALVARSBASE_HPP
#define SDK_CGLOBALVARSBASE_HPP

#include "Defs.hpp"

class CSaveRestoreData;
class CGlobalVarsBase : public non_constructible
{
public:
	// Absolute time (per frame still - Use Plat_f32Time() for a high precision real time
	//  perf clock, but not that it doesn't obey host_timescale/host_framerate)
	f32 realtime;
	// Absolute frame counter - continues to increase even if game is paused
	i32 framecount;
	// Non-paused frametime
	f32 absoluteframetime;

	// Current time
	//
	// On the client, this (along with tickcount) takes a different meaning based on what
	// piece of code you're in:
	//
	//   - While receiving network packets (like in PreDataUpdate/PostDataUpdate and proxies),
	//     this is set to the SERVER TICKCOUNT for that packet. There is no interval between
	//     the server ticks.
	//     [server_current_Tick * tick_interval]
	//
	//   - While rendering, this is the exact client clock
	//     [client_current_tick * tick_interval + interpolation_amount]
	//
	//   - During prediction, this is based on the client's current tick:
	//     [client_current_tick * tick_interval]
	f32 curtime;

	// Time spent on last server or client frame (has nothing to do with think intervals)
	f32 frametime;
	// current maxplayers setting
	i32 maxClients;

	// Simulation ticks - does not increase when game is paused
	i32 tickcount;

	// Simulation tick interval
	f32 interval_per_tick;

	// interpolation amount ( client-only ) based on fraction of next tick which has elapsed
	f32 interpolation_amount;
	i32 simTicksThisFrame;

	i32 network_protocol;

	// current saverestore data
	CSaveRestoreData* pSaveData;

	// Set to true in client code.
	bool client;

	bool remoteClient;

	// 100 (i.e., tickcount is rounded down to this base and then the "delta" from this base is networked
	i32 timestampNetworkingBase;
	// 32 (entindex() % nTimestampRandomizeWindow ) is subtracted from gpGlobals->tickcount to set the networking basis, prevents
	//  all of the entities from forcing a new PackedEntity on the same tick (i.e., prevents them from getting lockstepped on this)
	i32 timestampRandomizeWindow;
};

#endif // SDK_CGLOBALVARSBASE_HPP
