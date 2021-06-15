#ifndef SDK_CONCOMMAND_HPP
#define SDK_CONCOMMAND_HPP

#include "CCommand.hpp"
#include "ConCommandBase.hpp"

typedef void (*CommandCallbackV1Fn)();
typedef void (*CommandCallbackFn)(const CCommand&);
class ICommandCallback
{
public:
	virtual void CommandCallback(const CCommand& command) = 0;
};

constexpr static std::size_t CommandCompletionMaxItems = 64;
constexpr static std::size_t CommandCompletionItemLength = 64;
typedef i32 (*CommandCompletionCallbackFn)(const char* partial, std::array<std::array<char, CommandCompletionMaxItems>, CommandCompletionItemLength>& commands);
class ICommandCompletionCallback
{
public:
	virtual int CommandCompletionCallback(const char* partial, class CUtlVector_CUtlString& commands) = 0;
};

class ConCommand : public ConCommandBase
{
	virtual ~ConCommand() = 0;
	virtual bool IsCommand() const = 0;

	virtual int AutoCompleteSuggest(const char* partial, class CUtlVector_CUtlString& commands) = 0;

	virtual bool CanAutoComplete() = 0;

	// Invoke the function
	virtual void Dispatch(const CCommand& command) = 0;

private:
	// NOTE: To maintain backward compat, we have to be very careful:
	// All public virtual methods must appear in the same order always
	// since engine code will be calling into this code, which *does not match*
	// in the mod code; it's using slightly different, but compatible versions
	// of this class. Also: Be very careful about adding new fields to this class.
	// Those fields will not exist in the version of this class that is instanced
	// in mod code.

	// Call this function when executing the command
	union
	{
		CommandCallbackV1Fn fnCommandCallbackV1;
		CommandCallbackFn fnCommandCallback;
		ICommandCallback* pCommandCallback;
	};

	union
	{
		CommandCompletionCallbackFn fnCompletionCallback;
		ICommandCompletionCallback* pCommandCompletionCallback;
	};

	bool hasCompletionCallback = 1;
	bool usingNewCommandCallback = 1;
	bool usingCommandCallbackInterface = 1;
};

#endif // SDK_CONCOMMAND_HPP
