#ifndef SDK_IAPPSYSTEM_HPP
#define SDK_IAPPSYSTEM_HPP

#include "../Util/Defs.hpp"
#include "../Util/Util.hpp"

MAKE_CENUMV_Q(InitReturnVal, i32,
	INIT_FAILED, 0,
	INIT_OK, 1);

MAKE_CENUMV_Q(AppSystemTier, i32,
	APP_SYSTEM_TIER0, 0,
	APP_SYSTEM_TIER1, 1,
	APP_SYSTEM_TIER2, 2,
	APP_SYSTEM_TIER3, 3,
	APP_SYSTEM_TIER_OTHER, 4);

struct AppSystemInfo
{
	const char* moduleName;
	const char* interfaceName;
};

typedef void* (*CreateInterfaceFn)(const char*, i32*);

class IAppSystem
{
public:
	// Here's where the app systems get to learn about each other
	virtual bool Connect(CreateInterfaceFn factory) = 0;
	virtual void Disconnect() = 0;

	// Here's where systems can access other interfaces implemented by this object
	// Returns nullptr if it doesn't implement the requested interface
	virtual void* QueryInterface(const char* interfaceName) = 0;

	// Init, shutdown
	virtual InitReturnVal Init() = 0;
	virtual void Shutdown() = 0;

	// Returns all dependent libraries
	virtual const AppSystemInfo* GetDependencies() = 0;

	// Returns the tier
	virtual AppSystemTier GetTier() = 0;

	// Reconnect to a particular interface
	virtual void Reconnect(CreateInterfaceFn factory, const char* interfaceName) = 0;

	// Returns whether or not the app system is a singleton
	virtual bool IsSingleton() = 0;
};

#endif // SDK_IAPPSYSTEM_HPP
