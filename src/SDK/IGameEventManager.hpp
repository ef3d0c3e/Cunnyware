#ifndef SDK_GAMEEVENTSMANAGER_HPP
#define SDK_GAMEEVENTSMANAGER_HPP

#include "IBaseInterface.hpp"
#include "Defs.hpp"
class IGameEvent;
class IGameEventListener;
class bf_write;
class bf_read;
class KeyValues;

class IGameEventManager : public IBaseInterface
{
public:
	virtual ~IGameEventManager() = 0;
	virtual i32 LoadEventsFromFile(const char* filename) = 0;
	virtual void Reset() = 0;
	virtual bool AddListener(IGameEventListener* listener, const char* name, bool serverside) = 0;
	virtual bool FindListener(IGameEventListener* listener, const char* name) = 0;
	virtual void RemoveListener(IGameEventListener* listener) = 0;
	virtual void AddListenerGlobal(IGameEventListener* listener, bool serverside) = 0;
	virtual IGameEvent* CreateEvent(const char* name, bool force = false, i32* cookie = nullptr) = 0;
	virtual bool FireEvent(IGameEvent* event, bool bDontBroadcast = false) = 0;
	virtual bool FireEventClientSide(IGameEvent* event) = 0;
	virtual IGameEvent* DuplicateEvent(IGameEvent* event) = 0;
	virtual void FreeEvent(IGameEvent* event) = 0;
	virtual bool SerializeEvent(IGameEvent* event, bf_write* buffer) = 0;
	virtual IGameEvent* UnserializeEvent(bf_read* buffer) = 0;
	virtual KeyValues* GetEventDataTypes(IGameEvent* event) = 0;
};


#endif // SDK_GAMEEVENTSMANAGER_HPP
