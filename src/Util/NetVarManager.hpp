#ifndef SDK_UTILS_NETVARMANAGER_HPP
#define SDK_UTILS_NETVARMANAGER_HPP
#include "../SDK/Recv.hpp"

namespace NetVarManager
{
	std::vector<RecvTable*> GetTables();
	RecvTable* GetTable(std::vector<RecvTable*> tables, const char* tableName);
	i32 GetOffset(std::vector<RecvTable*> tables, const char* tableName, const char *propName);
	i32 GetProp(std::vector<RecvTable*> tables, const char* tableName, const char* propName, RecvProp** prop = 0);
	i32 GetProp(std::vector<RecvTable*> tables, RecvTable* recvTable, const char* propName, RecvProp** prop = 0);
	std::string DumpTable(RecvTable* table, i32 depth);
	bool HookProp(const char* tableName, const char* propName, RecvVarProxyFn f);
	void DumpNetvars();
}

#endif // SDK_UTILS_NETVARMANAGER_HPP
