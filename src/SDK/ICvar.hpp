#ifndef SDK_ICVAR_HPP
#define SDK_ICVAR_HPP

#include "CUtl/Vector.hpp"
#include "IAppSystem.hpp"
#include "ConCommand.hpp"

MAKE_CENUMV_Q(FCvar, u32, NONE, 0);

MAKE_CENUMV_Q(CvarType, u32, INT, 2, FLOAT, 4, STRING, 15);

class Cvar : public ConCommandBase
{
public:
	virtual ~Cvar() = 0;

	virtual bool IsFlagSet(i32 flag) const = 0;
	virtual const char* GetHelpText() const = 0;
	virtual bool IsRegistered() const = 0;
	virtual const char* GetName() const = 0;
	// Return name of command (usually == GetName(), except in case of FCVAR_SS_ADDED vars
	virtual const char* GetBaseName() const = 0;
	virtual i32 GetSplitScreenPlayerSlot() const = 0;

	virtual void AddFlags(i32 flags) = 0;
	virtual i32 GetFlags() const = 0;
	virtual bool IsCommand() const = 0;

	// Retrieve value
	virtual float GetFloat() const = 0;
	virtual int GetInt() const = 0;
	inline bool GetBool()
	{
		return !!GetInt();
	}

	// Any function that allocates/frees memory needs to be virtual or else you'll have crashes
	//  from alloc/free across dll/exe boundaries.

	// These just call into the IConCommandBaseAccessor to check flags and set the var (which ends up calling InternalSetValue).
	virtual void SetValue(const char* value) = 0;
	virtual void SetValue(f32 value) = 0;
	virtual void SetValue(i32 value) = 0;
	virtual void SetValue(Color value) = 0;

	void SetV(i32 value)
	{
		return getvfunc<void (*)(void*, i32)>(this, 19)(this, value);
	}

	i32 GetI()
	{
		return getvfunc<i32 (*)(void*)>(this, 16)(this);
	}

	// Value
	struct CVValue
	{
		char* string;
		i32 stringLength;

		// Values
		f32 fValue;
		i32 iValue;
	};

	inline CVValue& GetRawValue()
	{
		return value;
	}
	inline const CVValue& GetRawValue() const
	{
		return value;
	}

private:
	// Called by CCvar when the value of a var is changing.
	virtual void InternalSetValue(const char* value) = 0;
	// For CVARs marked FCVAR_NEVER_AS_STRING
	virtual void InternalSetFloatValue(f32 fNewValue) = 0;
	virtual void InternalSetIntValue(i32 nValue) = 0;
	virtual void InternalSetColorValue(Color value) = 0;

	virtual bool ClampValue(f32& value) = 0;
	virtual void ChangeStringValue(const char* tempVal, f32 oldValue) = 0;

	virtual void Create(const char* vame, const char* defaultValue, i32 flags = 0, const char* helpString = 0, bool min = false, f32 fmin = 0.0, bool max = false, f32 fmax = false, ChangeCallbackFn callback = 0) = 0;

	// Used internally by OneTimeInit to initialize.
	virtual void Init() = 0;

protected:
	// This either points to "this" or it points to the original declaration of a ConVar.
	// This allows ConVars to exist in separate modules, and they all use the first one to be declared.
	// m_pParent->m_pParent must equal m_pParent (ie: m_pParent must be the root, or original, ConVar).
	Cvar* parent;

	// Static data
	const char* defaultValue;

	CVValue value;

	// Min/Max values
	bool hasMin;
	f32 minVal;
	bool hasMax;
	f32 maxVal;

	// Call this function when ConVar changes
	CUtlVector<ChangeCallbackFn> changeCallbacks;
};

class IConsoleDisplayFunc
{
public:
	virtual void ColorPrint(const Color& color, const char* message) = 0;
	virtual void Print(const char* message) = 0;
	virtual void DPrint(const char* message) = 0;

	virtual void GetConsoleText(char* text, std::size_t size) const = 0;
};

class ICvarQuery : public IAppSystem
{
public:
	// Can these two convars be aliased?
	virtual bool AreConVarsLinkable(const Cvar* child, const Cvar* parent) = 0;
};

class ICvar : public IAppSystem
{
public:
	// Allocate a unique DLL identifier
	virtual CVarDLLIdentifier AllocateDLLIdentifier() = 0;

	// Register, unregister commands
	virtual void RegisterConCommand(ConCommandBase* commandBase) = 0;
	virtual void UnregisterConCommand(ConCommandBase* commandBase) = 0;
	virtual void UnregisterConCommands(CVarDLLIdentifier id) = 0;

	// If there is a +<varname> <value> on the command line, this returns the value.
	// Otherwise, it returns NULL.
	virtual const char* GetCommandLineValue(const char* pVariableName) = 0;

	// Try to find the cvar pointer by name
	virtual ConCommandBase* FindCommandBase(const char* name) = 0;
	virtual const ConCommandBase* FindCommandBase(const char* name) const = 0;
	virtual Cvar* FindVar(const char* var_name) = 0;
	virtual const Cvar* FindVar(const char* var_name) const = 0;
	virtual ConCommand* FindCommand(const char* name) = 0;
	virtual const ConCommand* FindCommand(const char* name) const = 0;



	// Install a global change callback (to be called when any convar changes)
	virtual void InstallGlobalChangeCallback(ChangeCallbackFn callback) = 0;
	virtual void RemoveGlobalChangeCallback(ChangeCallbackFn callback) = 0;
	virtual void CallGlobalChangeCallbacks(Cvar* var, const char* oldString, f32 oldValue) = 0;

	// Install a console printer
	virtual void InstallConsoleDisplayFunc(IConsoleDisplayFunc* displayFunc) = 0;
	virtual void RemoveConsoleDisplayFunc(IConsoleDisplayFunc* displayFunc) = 0;
	virtual void ConsoleColorPrintf(const ColorRGBA& color, const char* format, ...) const = 0; //25
	virtual void ConsolePrintf(const char* format, ...) const = 0;
	virtual void ConsoleDPrintf(const char* format, ...) const = 0;

	
	// Reverts cvars which contain a specific flag
	virtual void RevertFlaggedConVars(i32 flag) = 0;

	// Method allowing the engine ICvarQuery interface to take over
	// A little hacky, owing to the fact the engine is loaded
	// well after ICVar, so we can't use the standard connect pattern
	virtual void InstallCVarQuery(ICvarQuery* query) = 0;

	virtual void SetMaxSplitScreenSlots(i32 slots) = 0;
	virtual int GetMaxSplitScreenSlots() const = 0;

	virtual void AddSplitScreenConVars() = 0;
	//virtual void RemoveSplitScreenConVars(CVarDLLIdentifier id) = 0;

	virtual int GetConsoleDisplayFuncCount() const = 0;
	virtual void GetConsoleText(i32 displayFuncIndex, char* pchText, std::size_t bufSize) const = 0;

	// internals for  ICVarIterator
	class ICVarIteratorInternal;

	virtual ICVarIteratorInternal* FactoryInternalIterator() = 0;
};

#endif // SDK_ICVAR_HPP
