#ifndef SDK_CONCOMMANDBASE_HPP
#define SDK_CONCOMMANDBASE_HPP

#include "../Util/Defs.hpp"
#include "Color.hpp"

typedef i32 CVarDLLIdentifier;
class Cvar;
typedef void (*ChangeCallbackFn)(Cvar* cvar, const char* oldValue, f32 flOldValue);

class ConCommandBase
{
public:
	virtual ~ConCommandBase() = 0;

	virtual bool IsCommand() const = 0;

	// Check flag
	virtual bool IsFlagSet(i32 flag) const = 0;
	// Set flag
	virtual void AddFlags(i32 flags) = 0;
	// Clear flag
	virtual void RemoveFlags(i32 flags) = 0;

	virtual int GetFlags() const = 0;

	// Return name of cvar
	virtual const char* GetName() const = 0;

	// Return help text for cvar
	virtual const char* GetHelpText() const = 0;

	virtual bool IsRegistered() const = 0;

	// Returns the DLL identifier
	virtual CVarDLLIdentifier GetDLLIdentifier() const = 0;

protected:
	virtual void Create(const char* name, const char* helpString = 0, i32 flags = 0) = 0;

	// Used internally by OneTimeInit to initialize/shutdown
	virtual void Init() = 0;

private:
	// Next ConVar in chain
	// Prior to register, it points to the next convar in the DLL.
	// Once registered, though, m_pNext is reset to point to the next
	// convar in the global list
	ConCommandBase* next;

	// Has the cvar been added to the global list?
	bool registered;

	// Static data
	const char* name;
	const char* helpString;

	// ConVar flags
	i32 flags;
};

#endif // SDK_CONCOMMANDBASE_HPP
