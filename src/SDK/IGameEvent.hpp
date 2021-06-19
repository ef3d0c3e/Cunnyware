#ifndef SDK_IGAMEEVENT_HPP
#define SDK_IGAMEEVENT_HPP

#include "Defs.hpp"

class IGameEvent
{
public:
	virtual ~IGameEvent() = 0;
	virtual const char* GetName() const = 0;

	virtual bool IsReliable() const = 0;
	virtual bool IsLocal() const = 0;
	virtual bool IsEmpty(const char* keyname = nullptr) = 0;

	virtual bool GetBool(const char* keyname = nullptr, bool default_value = false) = 0;
	virtual i32 GetInt(const char* keyname = nullptr, i32 default_value = 0) = 0;
	virtual u64 GetUint64(const char* keyname = nullptr, u64 default_value = 0) = 0;
	virtual f32 GetFloat(const char* keyname = nullptr, f32 default_value = 0.0f) = 0;
	virtual const char* GetString(const char* keyname = nullptr, const char* default_value = "") = 0;
	virtual const wchar_t* GetWString(const char* keyname = nullptr, const wchar_t* default_value = L"") = 0;
	virtual const void* GetPtr(const char* keyname = nullptr, const void* default_values = nullptr) = 0;

	virtual void SetBool(const char* keyname, bool value) = 0;
	virtual void SetInt(const char* keyname, i32 value) = 0;
	virtual void SetUint64(const char* keyname, u64 value) = 0;
	virtual void SetFloat(const char* keyname, f32 value) = 0;
	virtual void SetString(const char* keyname, const char* value) = 0;
	virtual void SetWString(const char* keyname, const wchar_t* value) = 0;
	virtual void SetPtr(const char* keyname, const void* value) = 0;
};

#endif // SDK_IGAMEEVENT_HPP
