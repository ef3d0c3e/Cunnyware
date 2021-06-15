#ifndef SDK_CSTEAMAPICONTEXT_HPP
#define SDK_CSTEAMAPICONTEXT_HPP

#include "../Util/Defs.hpp"

class ISteamUser;
class ISteamFriends;
class ISteamUtils;
class ISteamMatchmaking;
class ISteamUserStats;
class ISteamApps;
class ISteamMatchmakingServers;
class ISteamNetworking;
class ISteamRemoteStorage;

class CSteamAPIContext : public non_constructible
{
public:
	ISteamUser*               SteamUser()               { return steamUser; }
	ISteamFriends*            SteamFriends()            { return steamFriends; }
	ISteamUtils*              SteamUtils()              { return steamUtils; }
	ISteamMatchmaking*        SteamMatchmaking()        { return steamMatchmaking; }
	ISteamUserStats*          SteamUserStats()          { return steamUserStats; }
	ISteamApps*               SteamApps()               { return steamApps; }
	ISteamMatchmakingServers* SteamMatchmakingServers() { return steamMatchmakingServers; }
	ISteamNetworking*         SteamNetworking()         { return steamNetworking; }
	ISteamRemoteStorage*      SteamRemoteStorage()      { return steamRemoteStorage; }

private:
	ISteamUser* steamUser;
	ISteamFriends* steamFriends;
	ISteamUtils* steamUtils;
	ISteamMatchmaking* steamMatchmaking;
	ISteamUserStats* steamUserStats;
	ISteamApps* steamApps;
	ISteamMatchmakingServers* steamMatchmakingServers;
	ISteamNetworking* steamNetworking;
	ISteamRemoteStorage* steamRemoteStorage;
};

#endif // SDK_CSTEAMAPICONTEXT_HPP
